/* vtapeutils - Virtual tape management utilities

  STDLABEL.C - Processes ANSI standard tape labels in EBCDIC or ASCII

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
#include "stdlabel.h"
#include "ebcdic.h"

int do_standard_label(VTAPE_FILE *tape_file, unsigned char *label)
{
  ansi_label translated_label;
  
  if (memcmp(label,"VOL1",4) == 0) {
    memcpy(translated_label.raw_label,label,80);
    print_volume_label(translated_label);
    return TRUE;
  } else {
    if (memcmp(label,"HDR1",4) == 0) {
      memcpy(translated_label.raw_label,label,80);
      print_header1_label(translated_label);
      return TRUE;
    } else {
      if ((memcmp(label,"EOF1",4) == 0) ||
          (memcmp(label,"EOV1",4) == 0)) {
        memcpy(translated_label.raw_label,label,80);
        print_trailer1_label(tape_file, translated_label);
        return TRUE;
      } else {
        if ((memcmp(label,"HDR2",4) == 0)) {
          memcpy(translated_label.raw_label,label,80);
          print_hdrtlr2_label(translated_label);
          return TRUE;
        }
      }
    }
  }
  ebcdic_to_ascii(label,translated_label.raw_label,80);
  if (memcmp(translated_label.raw_label,"VOL1",4) == 0) {
    print_volume_label(translated_label);
    return TRUE;
  } else {
    if (memcmp(translated_label.raw_label,"HDR1",4) == 0) {
      print_header1_label(translated_label);
      return TRUE;
    } else {
      if ((memcmp(translated_label.raw_label,"EOF1",4) == 0) ||
          (memcmp(translated_label.raw_label,"EOV1",4) == 0)) {
        print_trailer1_label(tape_file, translated_label);
        return TRUE;
      } else {
        if ((memcmp(translated_label.raw_label,"HDR2",4) == 0)) {
          print_hdrtlr2_label(translated_label);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

void print_volume_label(ansi_label label)
{
  printf("Standard labeled tape, volume %.6s, owner %.17s\n",
          label.vol1.volser, label.vol1.owner);
}

void print_header1_label(ansi_label label)
{
  printf("Begin dataset %.17s, number %.4s, created %.6s, expires %.6s\n",
          label.hdr1.dataset_name, label.hdr1.dataset_seq, 
          label.hdr1.creation_date, label.hdr1.expiration_date);
}

void print_trailer1_label(VTAPE_FILE *tape_file, ansi_label label)
{
  int label_block_count;
  char block_count_buffer[11];

  printf("End dataset   %.17s, %.4s%.6s blocks, written on %.13s\n",
          label.eof1.dataset_name, label.eof1.block_count_high,
          label.eof1.block_count, label.eof1.system_type);
  
  memcpy(block_count_buffer,label.eof1.block_count_high,4);
  memcpy(block_count_buffer+4,label.eof1.block_count,6);
  block_count_buffer[10] = '\0';
  label_block_count = strtol(block_count_buffer,(char **)NULL,10);
  
  if (label_block_count != tape_file->fileblkcnt) {
    printf("** WARNING ** Actual number of blocks in file is %d\n",
            tape_file->fileblkcnt);
  }
}

void print_hdrtlr2_label(ansi_label label)
{
  char blocking[3];
  
  if (label.hdr2.block_attr[0] == 'R') {
    strcpy(blocking,"BS");
  } else {
    blocking[0] = label.hdr2.block_attr[0];
    if (blocking[0] == ' ')
      blocking[0] = '\0';
    else
      blocking[1] = '\0';
  }
  printf("  blocksize %.5s, lrecl %.5s, record format %.1s%s%.1s, ",
          label.hdr2.block_length, label.hdr2.record_length,
          label.hdr2.record_format, blocking, label.hdr2.control_char);
  printf("written by %.17s\n", label.hdr2.creating_job);
}
