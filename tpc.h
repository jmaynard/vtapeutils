/* vtapeutils - Virtual tape management utilities

  TPC.H - data definitions and function prototypes for TPC (DECUS archive)
          processing

This file is part of the vtapeutils package of virtual tape management
utilities. The package is hosted at Github. Complete information may be
found at the summary page, https://github.com/jmaynard/vtapeutils .

Copyright James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

/* TPC tape block header. One per block, at the beginning. Used mainly for
    DECUS tape images, of which Tim Shoppa is reported to have "a couple
    thousand" in his archives. A tapemark is a single header of 0. */
typedef struct _TPC_HDR {
  unsigned char block_len[2];		/* length of this block */
} TPC_HDR;

#define TPC_MAX_RECLEN 65535		/* Maximum record length */

/* Functions. */
int tpc_read(VTAPE_FILE *infile, unsigned char *buffer, unsigned int maxlen);
int tpc_write(VTAPE_FILE *outfile, unsigned char *buffer,
              unsigned int reclength);
int tpc_open(VTAPE_FILE *file, char *filename, char *mode);
int tpc_close(VTAPE_FILE *file);
