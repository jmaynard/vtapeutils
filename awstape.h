/* vtapeutils - Virtual tape management utilities

  AWSTAPE.H - Data definitions and function prototypes for AWSTAPE processing

This file is part of the vtapeutils package of virtual tape management
utilities. The package is hosted at Github. Complete information may be
found at the summary page, https://github.com/jmaynard/vtapeutils .

Copyright James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

/* AWSTAPE header. One per block, at the beginning. Multiple blocks can
    be used to represent a tape record longer than will fit in one block.
    Blocks can be 64K long, but some systems will not accept blocks longer
    than 4K. Lengths are stored in little-endian order. Tapemarks are
    signaled with current block length of 0 and tapemark flag set. */
typedef struct _AWSTAPE_HDR {
  unsigned char this_block_len[2];	/* length of this block */
  unsigned char prev_block_len[2];	/* length of previous block */
  unsigned char flags_1;		/* first flag byte */
  unsigned char flags_2;		/* second flag byte - unused */
} AWSTAPE_HDR;

/* Flag byte 1 definitions. */
#define AWS_FLAG_STARTREC 0x80		/* block begins a tape record */
#define AWS_FLAG_ENDREC 0x20		/* block ends a tape record */
#define AWS_FLAG_TAPEMARK 0x40		/* this header is a tapemark */
#define AWS_HET_COMPRESSION 0x03	/* Hercules Emulated Tape: */
#define AWS_HET_COMP_ZLIB 0x01		/* record is compressed with zlib */
#define AWS_HET_COMP_BZ2 0x02		/* record is compressed with bzip2 */

/* Maximum size of an AWSTAPE block. This is enforced at file open: if the
    caller requests larger chunks, the size is silently lowered to this value
    so the block size in the header doesn't overflow. Since this value is
    only used to write a file, there's little point in complaining. */
#define AWS_MAX_BLOCKSIZE 65535		/* unsigned 16-bit value */

/* Functions. */

int awstape_read(VTAPE_FILE *infile, unsigned char *buffer,
                  unsigned int maxlen);
int awstape_write(VTAPE_FILE *outfile, unsigned char *buffer,
                  unsigned int reclength);
int awstape_open(VTAPE_FILE *file, char *filename, char *mode);
int awstape_close(VTAPE_FILE *file);
