/* vtapeutils - Virtual tape management utilities

  TAP.H - data definitions and function prototypes for SIMH/E11 processing

This file is part of the vtapeutils package of virtual tape amnagement
utilities. The package is hosted at SourceForge. Complete information may be
found at the summary page, http://sourceforge.net/projects/vtapeutils/ .

Copyright (c) 2005, James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

/* SIMH and E11 tape block header. Two per tape record, one at the beginning,
    one at the end, must be equal. Metadata markers only occur once.
    Tapemarks are a single block header of 0. SIMH and E11 formats are
    identical except that SIMH format pads odd-length data blocks with a
    single byte of 0 at the end (which is not included in the block length),
    and E11 does not. This library will accept either transparently, and
    can generate either depending on a flag supplied when the file is
    opened. */
typedef struct _TAP_HDR {
  unsigned char block_len[3];		/* length of this block */
  unsigned char flag;			/* flag byte */
} TAP_HDR;

/* Flag definitions. */
#define TAP_FLAG_METADATA 0xff		/* header is a metadata marker */
#define TAP_METADATA_END 0xffffffu	/* metadata marker: end of tape */
#define TAP_FLAG_ERROR 0x80		/* record is an error record */

#define TAP_MAX_RECLEN 16777215		/* maximum length of a tape record */

/* Functions. */
int tap_read(VTAPE_FILE *infile, char *buffer, unsigned int maxlen);
int tap_write(VTAPE_FILE *outfile, char *buffer, unsigned int reclength);
int tap_open(VTAPE_FILE *file, char *filename, char *mode);
int tap_close(VTAPE_FILE *file);
