#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

const unsigned int EMPTY_BYTE = 0xE5;

void rmdir(char * req) {
	DIRENTRY * entry = get_entry(req);
		unsigned int previous_cluster;
		char * previous_path = (char *)malloc(strlen(current_path) + 1);
		strcpy(previous_path, current_path);

	if (entry == NULL) {
		printf("ERROR: Directory does not exist.\n");
	} else if (entry->DIR_Attr == 0x00) {
		printf("ERROR: Cannot use rmdir on File.\n");
	} else {
		previous_cluster = current_cluster;
		cd(req);
		for(int i = 0; i < current_directory_size; i++) {
		deallocate_from_cluster(get_cluster_from_entry(current_directory[i]));
		remove_existing_entry(current_directory[i]);
		}
		current_cluster = previous_cluster;
		current_directory = get_directory_entries(current_cluster);
		strcpy(current_path, previous_path);
		deallocate_from_cluster(get_cluster_from_entry(entry));
		remove_existing_entry(entry);
	}
}
