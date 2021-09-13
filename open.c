#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void open(const char* fname, const char* mode) {
	DIRENTRY * entry_fname = get_entry(fname);

	if (entry_fname == NULL) {
		printf("ERROR: %s does not exist.\n", fname);
	} else if (strcmp(mode, "r") != 0 &&
		strcmp(mode, "w") != 0 &&
		strcmp(mode, "rw") != 0 &&
		strcmp(mode, "wr") != 0) {
			printf("ERROR: %s is an invalid mode\n", mode);
	} else if (is_file_open(fname)) {
		printf("ERROR: %s is already open.\n", fname);
	} else if (entry_fname != NULL && entry_fname->DIR_Attr == 0x10) {
		printf("ERROR: %s is a Directory.\n", fname);
	} else if (entry_fname != NULL && entry_fname->DIR_Attr == 0x01) {
		if (strcmp(mode, "w") == 0 ||
			strcmp(mode, "rw") == 0 ||
			strcmp(mode, "wr") == 0) {
			printf("ERROR: %s is READ-ONLY\n", fname);
		}
	} else {
		TableEntry * new_entry = (TableEntry *)malloc(sizeof(TableEntry));
		static TableEntry * entries[2048];
		strcpy(new_entry->name, fname);
		strcpy(new_entry->mode, mode);
		unsigned int cluster = get_cluster_from_entry(entry_fname);
		new_entry->TE_FstClus = cluster;
		entries[current_table_size++] = new_entry;
		table = entries;
	}
}
