#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void mkdir(char * req) {
	DIRENTRY * entry = get_entry(req);
	if (entry == NULL || entry->DIR_Attr == 0x00) {
		unsigned int first_cluster = get_next_free_cluster();

		if (first_cluster == 0xFFFFFFFF) {
			printf("ERROR: Image is full.\n");
			return;
		}

		// create the new directory direntry
		DIRENTRY * new_entry = (DIRENTRY *)malloc(sizeof(DIRENTRY));
		sanitize(req);
		set_name_for_entry(new_entry, req);
		allocate_cluster(first_cluster);
		set_cluster_for_entry(new_entry, first_cluster);
		new_entry->DIR_Attr = 0x10;
		new_entry->DIR_FileSize = 0;
		write_entry_to_directory(new_entry);
		// navigate to the new directory
		unsigned int previous_cluster = current_cluster;
		cd((const char *)new_entry->DIR_name);
		// create current directory direntry
		DIRENTRY * current = (DIRENTRY *)malloc(sizeof(DIRENTRY));
		set_name_for_entry(current, ".");
		set_cluster_for_entry(current, current_cluster);
		current->DIR_Attr = 0x10;
		current->DIR_FileSize = 0;
		write_entry_to_directory(current);
		// create parent directory direntry
		DIRENTRY * parent = (DIRENTRY *)malloc(sizeof(DIRENTRY));
		set_name_for_entry(parent, "..");
		set_cluster_for_entry(parent, previous_cluster);
		parent->DIR_Attr = 0x10;
		parent->DIR_FileSize = 0;
		write_entry_to_directory(parent);
		// create empty direntry
		DIRENTRY * empty = (DIRENTRY *)malloc(sizeof(DIRENTRY));
		empty->DIR_name[0] = 0x0;
		write_entry_to_directory(empty);
		// navigate to parent directory
		cd("..");
	} else {
		printf("Error: Directory already exists.\n");
	}
}
