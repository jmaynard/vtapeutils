/* vtapeutils - Virtual tape management utilities

  FAKETAPE.H - data definitions and function prototypes for FLEX-ES FakeTape
                processing

FakeTape is a trademark of Fundamental Software, Inc.

This file is part of the vtapeutils package of virtual tape management
utilities. The package is hosted at SourceForge. Complete information may be
found at the summary page, http://sourceforge.net/projects/vtapeutils/ .

Copyright (c) 2007, James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

/* FakeTape tape block header. FSI calls this an interblock gap (IBG) in
    their documentation. One per block, at the beginning.  A tapemark is a
    block with a length of 0. */
typedef struct _FAKETAPE_HDR {
  unsigned char prev_block_len[4];	/* length of prev block, ASCII hex */
  unsigned char block_len[4];		/* length of this block, ASCII hex */
  unsigned char check_value[4];		/* Binary XOR of two previous values,
                                           ASCII hex */
} FAKETAPE_HDR;

#define FAKETAPE_MAX_RECLEN 65535		/* Maximum record length */

/* Functions. */
int faketape_read(VTAPE_FILE *infile, unsigned char *buffer,
                  unsigned int maxlen);
int faketape_write(VTAPE_FILE *outfile, unsigned char *buffer,
                    unsigned int reclength);
int faketape_open(VTAPE_FILE *file, char *filename, char *mode);
int faketape_close(VTAPE_FILE *file);
unsigned int faketape_length_convert(unsigned char *length);
