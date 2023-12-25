/* vtapeutils - Virtual tape management utilities

  VTAPECP.C - Copies one virtual tape to another, with format conversion

This file is part of the vtapeutils package of virtual tape management
utilities. The package is hosted at SourceForge. Complete information may be
found at the summary page, http://sourceforge.net/projects/vtapeutils/ .

Copyright (c) 2005, 2007, James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "vtape.h"

#define MAXREC 65535

int main(int argc, char *argv[])
{
  int result;
  int totalblkcnt, filecnt, reclen;
  VTAPE_FILE intape, outtape;
  unsigned char buffer[MAXREC];
  
  if (argc != 3) {
    printf("Usage: %s <inputtape> <outputtape>\n",argv[0]);
    exit(1);
  }

  result = vtape_open(&intape, argv[1], "rb", VTAPE_UNKNOWN, 0, 0);
  if (result < 0) {
    printf("Error opening input tape file %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  result = vtape_open(&outtape, argv[2], "wb", VTAPE_UNKNOWN, 32767, FALSE);
  if (result < 0) {
    printf("Error opening output tape file %s: %s\n", argv[1],
            strerror(errno));
    exit(1);
  }
  totalblkcnt = 0;
  filecnt = 1;
  
  while (1) {
    reclen = vtape_read(&intape, buffer, MAXREC);
    if (reclen < 0) {
      if (vtape_eof(&intape)) break;
      printf("Error reading input tape file: %s\n", strerror(errno));
      exit(1);
    }
    if (vtape_write(&outtape, buffer, reclen) < reclen) {
      printf("Error writing output tape file: %s\n", strerror(errno));
      exit(1);
    }
    if (reclen == 0) {
      filecnt++;
    } else {
      totalblkcnt++;
    }
  }
  if (vtape_close(&intape) < 0) {
    printf("Error closing input tape file: %s\n", strerror(errno));
    exit(1);
  }
  if (vtape_close(&outtape) < 0) {
    printf("Error closing output tape file: %s\n", strerror(errno));
    exit(1);
  }
  printf("Complete. %d records in %d files processed.\n", totalblkcnt,
          filecnt++);
  return 0;
}
