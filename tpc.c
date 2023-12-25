/* vtapeutils - Virtual tape management utilities

  TPC.C - TPC (DECUS archive) tape processing routines

This file is part of the vtapeutils package of virtual tape management
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
#include "tpc.h"

int tpc_read(VTAPE_FILE *infile, unsigned char *buffer, unsigned int maxlen)
{
  TPC_HDR header;
  int reclen;

  /* Get the length of the current record. */
  if (fread(&header,(sizeof header),1,infile->file) < 1) {
    return -1;
  }
  reclen = ((int)header.block_len[1] << 8) + header.block_len[0];

  /* Tapemark? If so, return; we're done. */
  if (reclen == 0) {
    return 0;
  }

  /* Record too large for user-supplied buffer? Return error if so. */
  if (reclen > maxlen) {
    return -1;
  }

  /* Get the data. */
  if (fread(buffer,1,reclen,infile->file) < reclen) {
    return -1;
  }
  
  /* If odd-length record, read and throw away an extra byte. */
  if ((reclen % 2) == 1) {
    if (fread(&header,1,1,infile->file) < 1) {
      return -1;
    }
  }

  /* Return the number of data bytes read. */
  return reclen;
}

int tpc_write(VTAPE_FILE *outfile, unsigned char *buffer,
              unsigned int reclength)
{
  TPC_HDR header;

  /* Record too big for TPC? Error if so. */
  if (reclength > TPC_MAX_RECLEN) {
    return -1;
  }

  /* Set up header with record length. */
  header.block_len[1] = reclength >> 8;
  header.block_len[0] = reclength & 0xff;
  
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
  
  /* If record length is odd, add a padding byte. */
  if ((reclength % 2) == 1) {
    if (fwrite("\0",1,1,outfile->file) < 1) {
      return -1;
    }
  }
  
  /* We're finished. Return the record length (ignoring the pad, if any). */
  return reclength;
}

int tpc_open(VTAPE_FILE *file, char *filename, char *mode)
{
  if ((file->file = fopen(filename,mode)) == NULL) return -1;
  return 0;
}

int tpc_close(VTAPE_FILE *file)
{
  if (fclose(file->file) == EOF) return -1;
  return 0;
}
