/* vtapeutils - Virtual tape management utilities

  AWSTAPE.C - AWSTAPE processing routines

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
#include "awstape.h"

int awstape_read(VTAPE_FILE *infile, unsigned char *buffer,
                  unsigned int maxlen)
{
  AWSTAPE_HDR header;
  unsigned int reclen = 0, blklen;
  int finished;
  unsigned char *bufptr = buffer;

  /* Get the first block header. */
  if (fread(&header,(sizeof header),1,infile->file) < 1) {
    return -1;
  }

  /* Tapemark? If so, return; we're done. */
  blklen = ((int)header.this_block_len[1] << 8) + header.this_block_len[0];
  if ((blklen == 0) && (header.flags_1 & AWS_FLAG_TAPEMARK) != 0) {
    return 0;
  }

  /* First block of record? If not, we're hosed somehow. */
  if ((header.flags_1 & AWS_FLAG_STARTREC) == 0) {
    return -1;
  }

  /* Tapemark flag set on nonzero-length record? Error. */
  if ((header.flags_1 & AWS_FLAG_TAPEMARK) != 0) {
    return -1;
  }

  /* Read blocks until we get the end of record flag or run out of room. */
  finished = FALSE;
  while (!finished) {
    reclen += blklen;
    if (reclen > maxlen) {
      return -1;
    }

    /* Get the data. */
    if (fread(bufptr,1,blklen,infile->file) < blklen) {
      return -1;
    }
    bufptr += blklen;

    /* End of record flag set? If so, we're finished. */
    finished = (header.flags_1 & AWS_FLAG_ENDREC) != 0;
    if (!finished) {

      /* Get the next block header. */
      if (fread(&header,(sizeof header),1,infile->file) < 1) {
        return -1;
      }

      /* Tapemark or start-of-record flag set? Error. */
      if ((header.flags_1 & AWS_FLAG_TAPEMARK) != 0) {
        return -1;
      }

      /* Figure out the next block length. */
      blklen = ((int)header.this_block_len[1] << 8) + header.this_block_len[0];
    }
  }

  /* Return the number of data bytes read. */
  return reclen;
}

int awstape_write(VTAPE_FILE *outfile, unsigned char *buffer,
                  unsigned int reclength)
{
  AWSTAPE_HDR header;
  int remain = reclength, blklen;
  unsigned char *bufptr = buffer;

  /* Tapemark? If so, write just a tapemark header. */
  if (reclength == 0) {
    memset(&header,0,sizeof header);
    header.flags_1 = AWS_FLAG_TAPEMARK;
    header.prev_block_len[1] = (outfile->prev_block_len >> 8) & 0xff;
    header.prev_block_len[0] = outfile->prev_block_len & 0xff;
    if (fwrite(&header,sizeof header,1,outfile->file) < 1) {
      return -1;
    }
    outfile->prev_block_len = 0;
    return 0;
  }

  /* Mark the beginning of the block. This flag will be cleared later. */
  header.flags_1 = AWS_FLAG_STARTREC;
  header.flags_2 = 0;

  /* We may be writing several blocks to hold this record. */
  while (remain > 0) {

    /* Block length is the smaller of the data remaining to be written and
        the maximum AWSTAPE chunk size. */
    blklen = (remain > outfile->maxchunk) ? outfile->maxchunk : remain;
    remain -= blklen;

    /* Set up header with record lengths. */
    header.this_block_len[1] = (blklen >> 8) & 0xff;
    header.this_block_len[0] = blklen & 0xff;
    header.prev_block_len[1] = (outfile->prev_block_len >> 8) & 0xff;
    header.prev_block_len[0] = outfile->prev_block_len & 0xff;

    /* If this is the last block, flag it. */
    if (remain == 0) header.flags_1 |= AWS_FLAG_ENDREC;
  
    /* Write the header. */
    if (fwrite(&header,sizeof header,1,outfile->file) < 1) {
      return -1;
    }

    /* Write the data. */
    if (fwrite(bufptr,1,blklen,outfile->file) < blklen) {
      return -1;
    }

    /* Clear the start of record flag. */
    header.flags_1 = 0;
    
    /* Point to the next chunk's bit of data. */
    bufptr += blklen;
    
    /* Update the previous block length for the next block. */
    outfile->prev_block_len = blklen;
  }
  
  /* We're finished. Return the record length. */
  return reclength;
}

int awstape_open(VTAPE_FILE *file, char *filename, char *mode)
{
  if ((file->file = fopen(filename,mode)) == NULL) return -1;
  return 0;
}

int awstape_close(VTAPE_FILE *file)
{
  if (fclose(file->file) == EOF) return -1;
  return 0;
}
