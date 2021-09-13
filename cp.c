#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void cp(char * filename, char * to) {
	DIRENTRY * entry_from = get_entry(filename);
    DIRENTRY * entry_to = get_entry(to);
    unsigned int previous_cluster;
	char * previous_path = (char *)malloc(strlen(current_path) + 1);
	strcpy(previous_path, current_path);

    if (entry_from == NULL) {
		printf("ERROR: %s does not exist.\n", filename);
		return;
	} else if (is_dir(entry_from)) {
        printf("ERROR: %s is a directory.\n", filename);
		return;
    }

	if (is_file(entry_to)) {
		printf("ERROR: %s already exists.\n", to);
	} else if (is_dir(entry_to)) {
		previous_cluster = current_cluster;
		cd(to);
		create_entry_from_existing(entry_from);
		current_cluster = previous_cluster;
		current_directory = get_directory_entries(current_cluster);
		strcpy(current_path, previous_path);
	} else if (entry_to == NULL) {
		sanitize(to);
		set_name_for_entry(entry_from, to);
		write_entry_to_directory(entry_from);
	}
}

