/* vtapeutils - Virtual tape management utilities

  VTAPE.H - Virtual tape library interface data definitions and function
            prototypes

This file is part of the vtapeutils package of virtual tape amnagement
utilities. The package is hosted at SourceForge. Complete information may be
found at the summary page, http://sourceforge.net/projects/vtapeutils/ .

Copyright (c) 2005, James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

/* Virtual tape file types. To add a new file type, add a new entry to the end
    of this list, then add open/close/read/write calls to the library and
    call them from vtape_open/close/read/write. */
typedef enum _VTAPETYPE {
  VTAPE_UNKNOWN = 0,
  VTAPE_AWSTAPE,
  VTAPE_TAP,
  VTAPE_TPC,
} VTAPE_TYPE;

/* Virtual tape file control block. Contains all of the flags and file-related
    workareas for all known formats. Indicates which format the tape file is.
    Used by the various format-specific routines. */
typedef struct _VTAPE_FILE {
  FILE *file;			/* pointer to the actual file */
  VTAPE_TYPE type;		/* type of this tape file */
  unsigned int prev_block_len;	/* length of previous block (AWSTAPE) */
  unsigned int maxchunk;	/* maximum chunk length (AWSTAPE) */
  int padodd;			/* TRUE if padding odd length records (TAP) */
} VTAPE_FILE;

/* Convenience definitions, in case they don't get defined elsewhere. */
#if !defined(TRUE)
#define TRUE 1
#endif
#if !defined(FALSE)
#define FALSE 0
#endif

/* Function prototypes. */
int vtape_read(VTAPE_FILE *infile, char *buffer, unsigned int maxlen);
int vtape_write(VTAPE_FILE *outfile, char *buffer, unsigned int reclength);
int vtape_open(VTAPE_FILE *file, char *filename, char *mode, VTAPE_TYPE vttype,
                unsigned int maxchunk, int padodd);
int vtape_close(VTAPE_FILE *file);
int vtape_eof(VTAPE_FILE *file);
