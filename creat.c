#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void creat(char * req) {
	DIRENTRY * entry = get_entry(req);
	if (entry == NULL || entry->DIR_Attr == 0x10) {
		unsigned int first_cluster = get_next_free_cluster();

		if (first_cluster == 0xFFFFFFFF) {
			printf("ERROR: Image is full.\n");
			return;
		}

		DIRENTRY * new_entry = (DIRENTRY *)malloc(sizeof(DIRENTRY));
		sanitize(req);
		set_name_for_entry(new_entry, req);
		allocate_cluster(first_cluster);
		set_cluster_for_entry(new_entry, first_cluster);
		new_entry->DIR_Attr = 0x00;
		new_entry->DIR_FileSize = 0;
		write_entry_to_directory(new_entry);
	} else {
		printf("Error: File already exists.\n");
	}
}
