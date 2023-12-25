/* vtapeutils - Virtual tape management utilities

  EBCDIC.H - data definitions and function prototypes for ASCII to EBCDIC
             translation

This file is part of the vtapeutils package of virtual tape management
utilities. The package is hosted at SourceForge. Complete information may be
found at the summary page, http://sourceforge.net/projects/vtapeutils/ .

Copyright (c) 2007, James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

/* Functions. */
void ebcdic_to_ascii(unsigned char *inbuffer, unsigned char *outbuffer,
  int maxcount);
void ascii_to_ebcdic(unsigned char *inbuffer, unsigned char *outbuffer,
  int maxcount);
