/* vtapeutils - Virtual tape management utilities

  VTAPE.C - Virtual tape library interface routines

This file is part of the vtapeutils package of virtual tape amnagement
utilities. The package is hosted at SourceForge. Complete information may be
found at the summary page, http://sourceforge.net/projects/vtapeutils/ .

Copyright (c) 2005, James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vtape.h"
#include "awstape.h"
#include "tap.h"
#include "tpc.h"

int vtape_read(VTAPE_FILE *infile, char *buffer, unsigned int maxlen)
{
  int result;

  switch (infile->type) {
    case VTAPE_AWSTAPE: result=awstape_read(infile,buffer,maxlen); break;
    case VTAPE_TAP: result=tap_read(infile,buffer,maxlen); break;
    case VTAPE_TPC: result=tpc_read(infile,buffer,maxlen); break;
  }
  return result;
}

int vtape_write(VTAPE_FILE *outfile, char *buffer, unsigned int reclength)
{
  int result;

  switch (outfile->type) {
    case VTAPE_AWSTAPE: result=awstape_write(outfile,buffer,reclength); break;
    case VTAPE_TAP: result=tap_write(outfile,buffer,reclength); break;
    case VTAPE_TPC: result=tpc_write(outfile,buffer,reclength); break;
  }
  return result;
}

int vtape_open(VTAPE_FILE *file, char *filename, char *mode, VTAPE_TYPE vttype,
                unsigned int maxchunk, int padodd)
{
  int result, fnamelen;

  file->type = vttype;
  if (file->type == VTAPE_UNKNOWN) {
    fnamelen = strlen(filename);
    if (strcasecmp(filename+fnamelen-4,".aws") == 0) {
      file->type = VTAPE_AWSTAPE;
    } else if (strcasecmp(filename+fnamelen-4,".tap") == 0) {
      file->type = VTAPE_TAP;
    } else if (strcasecmp(filename+fnamelen-4,".tap") == 0) {
      file->type = VTAPE_TPC;
    } else {
      return -1;
    }
  }
  switch (file->type) {
    case VTAPE_AWSTAPE: result=awstape_open(file,filename,mode); break;
    case VTAPE_TAP: result=tap_open(file,filename,mode); break;
    case VTAPE_TPC: result=tpc_open(file,filename,mode); break;
  }
  file->maxchunk = maxchunk;
  file->prev_block_len = 0;
  file->padodd = padodd;
  return result;
}

int vtape_close(VTAPE_FILE *file)
{
  int result;

  switch (file->type) {
    case VTAPE_AWSTAPE: result=awstape_close(file); break;
    case VTAPE_TAP: result=tap_close(file); break;
    case VTAPE_TPC: result=tpc_close(file); break;
  }
  return result;
}

int vtape_eof(VTAPE_FILE *file)
{
  return feof(file->file);
}
