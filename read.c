#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void read(const char * file, const char * size) {
	DIRENTRY *entry = get_entry(file);

	if (entry == NULL) {
		printf("ERROR: %s does not exist.\n", file);
		return;
	}

	if (entry != NULL && entry->DIR_Attr == 0x10) {
		printf("ERROR: %s is a directory\n", file);
	} else if (!is_file_open(file)) {
		printf("ERROR: %s is not open.\n", file);
	} else if (!is_file_open_read(file)) {
		printf("ERROR: %s is not open for reading.\n", file);
	} else {
		// Not yet implemented
	}
}
