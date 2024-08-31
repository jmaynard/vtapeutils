/* vtapeutils - Virtual tape management utilities

  XTAPE.H - data definitions and function prototypes for XTAPE
            (Multiprise 3000 XTAPE utility) processing

This file is part of the vtapeutils package of virtual tape management
utilities. The package is hosted at Github. Complete information may be
found at the summary page, https://github.com/jmaynard/vtapeutils .

Copyright James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

/* XTAPE tape block header. One per block, at the beginning. A tapemark
   is a block with length of 0x4d54 and contents of 'TMTM'. Note that
   the length is inaccurate, so if we get this length, we have to
   check for the tapemark contents before treating it as an actual
   length. */
typedef struct _XTAPE_HDR {
  unsigned char block_len[2];		/* length of this block */
  unsigned char unused[2];      /* I think this is unused */
} XTAPE_HDR;

#define XTAPE_MAX_RECLEN 65535		/* Maximum record length */
#define XTAPE_TAPEMARK_RECLEN 0x544d /* Tapemark record length, byteswapped*/
#define XTAPE_TAPEMARK_CONTENTS "TMTM" /* Contents of a tapemark block */
#define XTAPE_TAPEMARK_LENGTH 4 /* Length of the tapemark contents */

/* Functions. */
int xtape_read(VTAPE_FILE *infile, unsigned char *buffer, unsigned int maxlen);
int xtape_write(VTAPE_FILE *outfile, unsigned char *buffer,
              unsigned int reclength);
int xtape_open(VTAPE_FILE *file, char *filename, char *mode);
int xtape_close(VTAPE_FILE *file);
