/* vtapeutils - Virtual tape management utilities

  STDLABEL.H - data definitions and function prototypes for ANSI standard
               label processing

This file is part of the vtapeutils package of virtual tape management
utilities. The package is hosted at Github. Complete information may be
found at the summary page, https://github.com/jmaynard/vtapeutils .

Copyright James R. Maynard, III
 All rights reserved.

See the file LICENSE in this distribution for license terms.
*/

/* This union defines all of the label types in the ANSI standard in one,
    easy-to-use package. */
typedef union _ansi_label {
  unsigned char raw_label[81];
  struct {
    unsigned char label_id[3];		/* VOL */
    unsigned char label_num[1];		/* 1 */
    unsigned char volser[6];		/* volume serial number */
    unsigned char accessibility[1];	/* blank = secured */
    unsigned char reserved1[26];	/* reserved */
    unsigned char owner[14];		/* owner identifier */
    unsigned char reserved2[28];	/* reserved */
    unsigned char label_level[1];	/* label standard level */
  } vol1;
  struct {
    unsigned char label_id[3];		/* HDR */
    unsigned char label_num[1];		/* 1 */
    unsigned char dataset_name[17];	/* last 17 characters of dataset name */
    unsigned char first_volser[6];	/* volume serial of the first volume
                                           in a multivolume dataset */
    unsigned char volume_seq[4];	/* multivolume dataset volume seq # */
    unsigned char dataset_seq[4];	/* number of this dataset on tape */
    unsigned char generation_num[4];	/* generation number of this dataset */
    unsigned char generation_ver[2];	/* version number of this dataset */
    unsigned char creation_date[6];	/* dataset creation date */
    unsigned char expiration_date[6];	/* dataset expiration date */
    unsigned char dataset_security[1];	/* is this dataset secured? */
    unsigned char reserved[26];		/* reserved in header labels */
  } hdr1;
  struct {
    unsigned char label_id[3];		/* EOF/EOV */
    unsigned char label_num[1];		/* 1 */
    unsigned char dataset_name[17];	/* last 17 characters of dataset name */
    unsigned char first_volser[6];	/* volume serial of the first volume
                                           in a multivolume dataset */
    unsigned char volume_seq[4];	/* multivolume dataset volume seq # */
    unsigned char dataset_seq[4];	/* number of this dataset on tape */
    unsigned char generation_num[4];	/* generation number of this dataset */
    unsigned char generation_ver[2];	/* version number of this dataset */
    unsigned char creation_date[6];	/* dataset creation date */
    unsigned char expiration_date[6];	/* dataset expiration date */
    unsigned char dataset_security[1];	/* is this dataset secured? */
    unsigned char block_count[6];	/* count of blocks in this dataset */
    unsigned char system_type[13];	/* type of system that wrote tape */
    unsigned char reserved[3];		/* reserved */
    unsigned char block_count_high[4];	/* high order digits of block count */
  } eof1;
  struct {
    unsigned char label_id[3];		/* HDR/EOF/EOV */
    unsigned char label_num[1];		/* 2 */
    unsigned char record_format[1];	/* record format: F, V, U */
    unsigned char block_length[5];	/* block length, 1-32767 */
    unsigned char record_length[5];	/* logical record length, 1-32767 */
    unsigned char density[1];		/* tape density */
    unsigned char dataset_position[1];	/* has a volume switch occurred? */
    unsigned char creating_job[17];	/* job/step that created dataset */
    unsigned char recording_tech[2];	/* tape recording technique */
    unsigned char control_char[1];	/* control character type */
    unsigned char reserved1[1];		/* reserved */
    unsigned char block_attr[1];	/* blocked/spanned/standard blocking */
    unsigned char reserved2[3];		/* reserved */
    unsigned char drive_serial[5];	/* writing drive serial number */
    unsigned char checkpoint_dataset[1];/* is this a checkpoint dataset? */
    unsigned char reserved3[22];	/* reserved */
    unsigned char large_block_len[10];	/* large block length (>32K) */
  } hdr2;
} ansi_label;

/* Functions. */
int do_standard_label(VTAPE_FILE *tape_file, unsigned char *label);
void print_volume_label(ansi_label label);
void print_header1_label(ansi_label label);
void print_trailer1_label(VTAPE_FILE *tape_file, ansi_label label);
void print_hdrtlr2_label(ansi_label label);
