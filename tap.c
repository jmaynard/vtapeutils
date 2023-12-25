/* vtapeutils - Virtual tape management utilities

  TAP.C - SIMH/E11 processing routines

This file is part of the vtapeutils package of virtual tape management
utilities. The package is hosted at SourceForge. Complete information may be
found at the summary page, http://sourceforge.net/projects/vtapeutils/ .

Copyright (c) 2005, 2007, James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vtape.h"
#include "tap.h"

int tap_read(VTAPE_FILE *infile, unsigned char *buffer, unsigned int maxlen)
{
  TAP_HDR header, trailer;
  unsigned int reclen;

  /* Get the length of the current record. */
  if (fread(&header,(sizeof header),1,infile->file) < 1) {
    return -1;
  }
  reclen = ((unsigned int)header.block_len[2] << 16) + 
            ((unsigned int)header.block_len[1] << 8) + header.block_len[0];

  /* Tapemark? If so, return; we're done. */
  if ((reclen == 0) && (header.flag == 0)) {
    return 0;
  }

  /* End of file marker? Treat as tapemark. */
  if ((header.flag == TAP_FLAG_METADATA) && (reclen == TAP_METADATA_END)) {
    return 0;
  }

  /* Record too large for user-supplied buffer? Return error if so. */
  if (reclen > maxlen) {
    return -1;
  }

  /* Other metadata values are reserved, and other flag values are illegal
      except for the error record marker; treat all as I/O error. */
  if (header.flag != 0) {
    return -1;
  }

  /* Get the data. */
  if (fread(buffer,1,reclen,infile->file) < reclen) {
    return -1;
  }
  
  /* Get the record trailer. */
  if (fread(&trailer,(sizeof header),1,infile->file) < 1) {
    return -1;
  }

  /* If odd-length record and header and trailer don't match, the record may
      be padded; shift the trailer down a byte, then read one more. */
  if (((reclen % 2) == 1) && (memcmp(&header,&trailer,sizeof header) != 0)) {
    memmove(&trailer,&trailer+1,(sizeof trailer)-1);
    if (fread(&trailer+sizeof trailer-1,1,1,infile->file) < 1) {
      return -1;
    }
  }

  /* If the header and trailer still don't match, error. */
  if (memcmp(&header,&trailer,sizeof header) != 0) {
    return -1;
  }

  /* Return the number of data bytes read. */
  return reclen;
}

int tap_write(VTAPE_FILE *outfile, unsigned char *buffer,
              unsigned int reclength)
{
  TAP_HDR header;

  /* Record too big for TAP? Error if so. */
  if (reclength > TAP_MAX_RECLEN) {
    return -1;
  }

  /* Set up header with record length and zero flag byte. */
  header.block_len[2] = reclength >> 16;
  header.block_len[1] = reclength >> 8;
  header.block_len[0] = reclength & 0xff;
  header.flag = 0;
  
  /* Write the header. */
  if (fwrite(&header,sizeof header,1,outfile->file) < 1) {
    return -1;
  }

  /* Tapemark? We're done. */
  if (reclength == 0) {
    return 0;
  }

  /* Write the data. */
  if (fwrite(buffer,1,reclength,outfile->file) < reclength) {
    return -1;
  }
  
  /* If record length is odd and the pad odd records flag is set, add a
      padding byte. */
  if (((reclength % 2) == 1) && outfile->padodd) {
    if (fwrite("\0",1,1,outfile->file) < 1) {
      return -1;
    }
  }
  
  /* Write the header again as the trailer. */
  if (fwrite(&header,sizeof header,1,outfile->file) < 1) {
    return -1;
  }

  /* We're finished. Return the record length (ignoring the pad, if any). */
  return reclength;
}

int tap_open(VTAPE_FILE *file, char *filename, char *mode)
{
  if ((file->file = fopen(filename,mode)) == NULL) return -1;
  return 0;
}

int tap_close(VTAPE_FILE *file)
{
  TAP_HDR header;

  /* Set up the header for the EOF marker; this library doesn't care about it,
      but other programs might. */
  header.flag = TAP_FLAG_METADATA;
  header.block_len[2] = (TAP_METADATA_END >> 16) & 0xff;
  header.block_len[1] = (TAP_METADATA_END >> 8) & 0xff;
  header.block_len[0] = TAP_METADATA_END & 0xff;

  /* Write the header. */
  if (fwrite(&header,sizeof header,1,file->file) < 1) {
    return -1;
  }

  if (fclose(file->file) == EOF) return -1;
  return 0;
}
