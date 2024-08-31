/* vtapeutils - Virtual tape management utilities

  XTAPE.C - XTAPE (Multiprise 3000 tape utility) tape processing routines

This file is part of the vtapeutils package of virtual tape management
utilities. The package is hosted at Github. Complete information may be
found at the summary page, https://github.com/jmaynard/vtapeutils .

Copyright James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vtape.h"
#include "xtape.h"

int xtape_read(VTAPE_FILE *infile, unsigned char *buffer, unsigned int maxlen)
{
  XTAPE_HDR header;
  int reclen;
  int buffer_offset = 0;

  /* Get the length of the current record. */
  if (fread(&header,(sizeof header),1,infile->file) < 1) {
    return -1;
  }
  reclen = ((int)header.block_len[1] << 8) + header.block_len[0];

  /* Tapemark? If so, return; we're done. */
  if (reclen == XTAPE_TAPEMARK_RECLEN) {
    if (fread(buffer,XTAPE_TAPEMARK_LENGTH,1,infile->file) < 1)
    {
      return -1;  /* Oops: file ends before tapemark contents */
    }
    if (memcmp(buffer,XTAPE_TAPEMARK_CONTENTS,XTAPE_TAPEMARK_LENGTH) == 0)
    {
      return 0;
    }
    /* We got a block that's the actual length of the claimed tapemark
       header. Account for the bytes we've read and continue. */
    buffer_offset = XTAPE_TAPEMARK_LENGTH;
  }

  /* Record too large for user-supplied buffer? Return error if so. */
  if (reclen > maxlen) {
    return -1;
  }

  /* The record length includes the header. Compensate. */
  reclen -= sizeof header;

  /* Get the data. */
  if (fread(buffer+buffer_offset,1,reclen,infile->file) < reclen) {
    printf("Error reading record data: %s",strerror(ferror(infile->file)));
    return -1;
  }
  
  /* Return the number of data bytes read. */
  return reclen;
}

int xtape_write(VTAPE_FILE *outfile, unsigned char *buffer,
              unsigned int reclength)
{
  XTAPE_HDR header;

  /* Record too big for XTAPE? Error if so. */
  if (reclength > XTAPE_MAX_RECLEN) {
    return -1;
  }

  /* Tapemark? Write it and return. */
  if (reclength == 0) {
    header.block_len[1] = 'T';
    header.block_len[0] = 'M';
    header.unused[0] = header.unused[1] = 0;
    if (fwrite(&header,sizeof header,1,outfile->file) < 1) {
      return -1;
    }
    if (fwrite(XTAPE_TAPEMARK_CONTENTS,1,
              sizeof XTAPE_TAPEMARK_CONTENTS,outfile->file)
              < sizeof XTAPE_TAPEMARK_CONTENTS) {
      return -1;
    }
    return 0;
  }

  /* Set up header with record length. */
  header.block_len[1] = reclength >> 8;
  header.block_len[0] = reclength & 0xff;
  header.unused[0] = header.unused[1] = 0;
  
  /* Write the header. */
  if (fwrite(&header,sizeof header,1,outfile->file) < 1) {
    return -1;
  }

  /* Write the data. */
  if (fwrite(buffer,1,reclength,outfile->file) < reclength) {
    return -1;
  }
  
  /* We're finished. Return the record length (ignoring the pad, if any). */
  return reclength;
}

int xtape_open(VTAPE_FILE *file, char *filename, char *mode)
{
  if ((file->file = fopen(filename,mode)) == NULL) return -1;
  return 0;
}

int xtape_close(VTAPE_FILE *file)
{
  if (fclose(file->file) == EOF) return -1;
  return 0;
}
