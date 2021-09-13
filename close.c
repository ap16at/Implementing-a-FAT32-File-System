#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void close(const char* fname) {
	DIRENTRY * file = get_entry(fname);

	if (file == NULL) {
		printf("ERROR: %s does not exist.\n", fname);
	} else if (file->DIR_Attr == 0x10) {
		printf("ERROR: %s is a directory.\n", fname);
	} else if (is_file_open(fname)) {
		for(int i = 0; i < current_table_size; i++) {
			if (strcmp((char *)table[i]->name, fname) == 0) {
				table[i] = NULL;
				for(int j = i+1; j < current_table_size; j++) {
					table[i] = table[j];
				}
				current_table_size--;
			}
		}
	} else {
		printf("ERROR: %s was not open.\n", fname);
	}
}
