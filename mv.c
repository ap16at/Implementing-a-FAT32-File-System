#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void mv(char * from, char * to) {
	DIRENTRY * entry_from = get_entry(from);
	DIRENTRY * entry_to = get_entry(to);
	unsigned int previous_cluster;
	char * previous_path = (char *)malloc(strlen(current_path) + 1);
	strcpy(previous_path, current_path);

	if (entry_from == NULL) {
		printf("ERROR: %s does not exist.\n", from);
		return;
	} else {
		unsigned int from_cluster = get_cluster_from_entry(entry_from);
		if (from_cluster == 0 || from_cluster == 2) {
			printf("ERROR: cannot move root directory.\n");
			return;
		}
	}

	if (is_dir(entry_from)) {
		if (is_dir(entry_to)) {
			remove_existing_entry(entry_from);
			previous_cluster = current_cluster;
			cd(to);
			create_entry_from_existing(entry_from);
			current_cluster = previous_cluster;
			current_directory = get_directory_entries(current_cluster);
			strcpy(current_path, previous_path);
		} else if (is_file(entry_to)) {
			printf("ERROR: Cannot move directory: invalid destination argument.\n");
		}
	} else if (is_file(entry_from)) {
		if (is_dir(entry_to)) {
			remove_existing_entry(entry_from);
			previous_cluster = current_cluster;
			cd(to);
			create_entry_from_existing(entry_from);
			current_cluster = previous_cluster;
			current_directory = get_directory_entries(current_cluster);
			strcpy(current_path, previous_path);
		} else if (is_file(entry_to)) {
			printf("ERROR: %s is already being used by another file.\n", to);
		} else {
			sanitize(to);
			remove_existing_entry(entry_from);
			set_name_for_entry(entry_from, to);
			write_entry_to_directory(entry_from);
		}
	}
}

int is_file(DIRENTRY * entry) {
	if (entry == NULL) return 0;

	return entry->DIR_Attr == 0x00
			|| entry->DIR_Attr == 0x01
			|| entry->DIR_Attr == 0x20;
}

int is_dir(DIRENTRY * entry) {
	if (entry == NULL) return 0;

	return entry->DIR_Attr == 0x10;
}

void create_entry_from_existing(DIRENTRY * entry) {
	unsigned int first_cluster = get_next_free_cluster();
	if (first_cluster == 0xFFFFFFFF) {
		printf("ERROR: Image is full.\n");
		return;
	}
	allocate_cluster(first_cluster);
	set_cluster_for_entry(entry, first_cluster);
	write_entry_to_directory(entry);
}