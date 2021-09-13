#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void ls(const char * req) {
	unsigned int previous_cluster = current_cluster;
	DIRENTRY * entry = get_entry(req);

	if (current_cluster == BPB_RootClus && strcmp(req, ".") == 0) {
		print_current_directory();
	} else if (entry == NULL) {
        printf("ERROR: Directory not found.\n");
    } else if (entry->DIR_Attr != 0x10) {
        printf("ERROR: %s is not a directory.\n", req);
    } else {
		current_cluster = get_cluster_from_entry(entry);
        if (current_cluster == 0) current_cluster = BPB_RootClus;
		current_directory = get_directory_entries(current_cluster);
		print_current_directory();
		current_cluster = previous_cluster;
		current_directory = get_directory_entries(current_cluster);
	}
}
