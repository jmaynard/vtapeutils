/* vtapeutils - Virtual tape management utilities

  VTAPELIST.C - Lists the contents of a virtual tape

This file is part of the vtapeutils package of virtual tape amnagement
utilities. The package is hosted at SourceForge. Complete information may be
found at the summary page, http://sourceforge.net/projects/vtapeutils/ .

Copyright (c) 2005, James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "vtape.h"

#define MAXREC 65535

void do_standard_label(char *label)
{
}

int main(int argc, char *argv[])
{
  int result;
  int totalblkcnt, filecnt, fileblkcnt, reclen, maxrec, minrec;
  VTAPE_FILE tape;
  char buffer[MAXREC];
  
  if (argc != 2) {
    printf("Usage: %s <tapefile>\n",argv[0]);
    exit(1);
  }

  result = vtape_open(&tape, argv[1], "rb", VTAPE_UNKNOWN, 0, 0);
  if (result < 0) {
    printf("Error opening tape file %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  totalblkcnt = fileblkcnt = maxrec = 0;
  minrec=MAXREC;
  filecnt = 1;
  
  while (1) {
    reclen = vtape_read(&tape, buffer, MAXREC);
    if (reclen < 0) {
      if (vtape_eof(&tape)) break;
      printf("Error reading tape file: %s\n", strerror(errno));
      exit(1);
    } else if (reclen == 0) {
      printf("File %d: %d record%s", filecnt++, fileblkcnt,
              (fileblkcnt != 1) ? "s" : "");
      if (fileblkcnt > 0) {
        printf(", minimum record length %d, maximum record length %d",
                minrec, maxrec);
      }
      fileblkcnt = maxrec = 0;
      minrec = MAXREC;
      printf("\n");
    } else {
      fileblkcnt++;
      totalblkcnt++;
      if (reclen > maxrec) maxrec = reclen;
      if (reclen < minrec) minrec = reclen;
      if (reclen = 80) do_standard_label(buffer);
    }
  }
  printf("Complete. %d records processed.\n", totalblkcnt);
  return 0;
}
