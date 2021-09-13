#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

TableEntry* FTRoot = NULL;

int is_file_open(const char * fname) {
	for (int i = 0; i < current_table_size; i++) {
		if (strcmp((char *)table[i]->name, fname) == 0) {
			return 1;
		}
	}
	return 0;
}

int is_file_open_read(const char * fname) {
	for (int i = 0; i < current_table_size; i++) {
		if (strcmp((char *)table[i]->name, fname) == 0) {
			if ((strcmp((char *)table[i]->mode, "r") == 0) ||
			(strcmp((char *)table[i]->mode, "rw") == 0) ||
			(strcmp((char *)table[i]->mode, "wr") == 0)) {
				return 1;
			}
		}
	}
  return 0;
}

int is_file_open_write(const char * fname) {
	for (int i = 0; i < current_table_size; i++) {
		if (strcmp((char *)table[i]->name, fname) == 0) {
			if ((strcmp((char *)table[i]->mode, "w") == 0) ||
				(strcmp((char *)table[i]->mode, "rw") == 0) ||
				(strcmp((char *)table[i]->mode, "wr") == 0)) {
					return 1;
			}
		}
	}
	return 0;
}
