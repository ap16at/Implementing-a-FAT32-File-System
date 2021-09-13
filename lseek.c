#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void lseek(const char * file, const char * offset) {
	DIRENTRY * entry = get_entry(file);

	if (entry == NULL) {
		printf("ERROR: %s does not exist.\n", file);
		return;
	}

	if (entry != NULL && entry->DIR_Attr == 0x10) {
		printf("ERROR: %s is a directory.\n", file);
	} else {
		// Not yet implemented.
	}
}
