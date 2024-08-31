/* vtapeutils - Virtual tape management utilities

  FAKETAPE.C - Flex-ES FakeTape tape processing routines

FakeTape is a trademark of Fundamental Software, Inc.

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
#include "faketape.h"

int faketape_read(VTAPE_FILE *infile, unsigned char *buffer,
                  unsigned int maxlen)
{
  FAKETAPE_HDR header;
  unsigned int reclen, prev_reclen, check_value;

  /* Get the length of the current record. */
  if (fread(&header,(sizeof header),1,infile->file) < 1) {
    return -1;
  }
  reclen = faketape_length_convert(header.block_len);

  /* Tapemark? If so, return; we're done. */
  if (reclen == 0) {
    return 0;
  }

  /* Record too large for user-supplied buffer? Return error if so. */
  if (reclen > maxlen) {
    return -1;
  }

  /* Check the validity of the header, and return error if invalid. */
  prev_reclen = faketape_length_convert(header.prev_block_len);
  check_value = faketape_length_convert(header.check_value);
  if (check_value != (reclen ^ prev_reclen)) {
    return -1;
  }

  /* Get the data. */
  if (fread(buffer,1,reclen,infile->file) < reclen) {
    return -1;
  }
  
  /* Return the number of data bytes read. */
  return reclen;
}

int faketape_write(VTAPE_FILE *outfile, unsigned char *buffer,
              unsigned int reclength)
{
  FAKETAPE_HDR header;
  char convert_buffer[5];

  /* Record too big for FAKETAPE? Error if so. */
  if (reclength > FAKETAPE_MAX_RECLEN) {
    return -1;
  }

  /* Set up header with previous and current record lengths. */
  sprintf(convert_buffer,"%04X",reclength);
  memcpy(header.block_len,convert_buffer,4);
  sprintf(convert_buffer,"%04X",outfile->prev_block_len);
  memcpy(header.prev_block_len,convert_buffer,4);
  sprintf(convert_buffer,"%04X",outfile->prev_block_len ^ reclength);
  memcpy(header.check_value,convert_buffer,4);

  /* Write the header. */
  if (fwrite(&header,sizeof header,1,outfile->file) < 1) {
    return -1;
  }
  
  /* Save this record length as the next record's previous record length. */ 
  outfile->prev_block_len = reclength;

  /* Tapemark? We're done. */
  if (reclength == 0) {
    return 0;
  }

  /* Write the data. */
  if (fwrite(buffer,1,reclength,outfile->file) < reclength) {
    return -1;
  }
  
  /* We're finished. Return the record length. */
  return reclength;
}

int faketape_open(VTAPE_FILE *file, char *filename, char *mode)
{
  if ((file->file = fopen(filename,mode)) == NULL) return -1;
  return 0;
}

int faketape_close(VTAPE_FILE *file)
{
  if (fclose(file->file) == EOF) return -1;
  return 0;
}

unsigned int faketape_length_convert(unsigned char *length)
{
  char buffer[5];
  
  memcpy(buffer,length,4);
  buffer[4] = '\0';
  return strtol(buffer, (char **)NULL, 16);
}