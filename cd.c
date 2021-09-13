#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "shared.h"

void cd(const char * req) {
	DIRENTRY * entry = get_entry(req);

    if (current_cluster == BPB_RootClus && strcmp(req, ".") == 0) {
        return;
	} else if (current_cluster == BPB_RootClus && strcmp(req, "..") == 0) {
        printf("ERROR: Already in the root directory.\n");
    } else if (entry == NULL) {
        printf("ERROR: Directory not found.\n");
    } else if (entry->DIR_Attr != 0x10) {
        printf("ERROR: %s is not a directory.\n", req);
    } else {
		current_cluster = get_cluster_from_entry(entry);
        if (current_cluster == 0) current_cluster = BPB_RootClus;
		current_directory = get_directory_entries(current_cluster);
        if (strcmp(req, "..") == 0) {
            set_path_as_dir_above();
        } else if (strcmp(req, ".") != 0) {
            set_current_path((char *)entry->DIR_name);
        }
	}
}

void cd_to_root() {
    current_cluster = BPB_RootClus;
    current_directory = get_directory_entries(current_cluster);
    set_current_path("");
    current_path = (char *)realloc(current_path, sizeof(char) + 1);
    strcpy(current_path, "");
}

void set_current_path(const char * req) {
    char * req_sanitized = (char *)malloc(strlen(req) + 1);
    strcpy(req_sanitized, req);
    sanitize(req_sanitized);
    current_path = (char *)realloc(current_path,
                    sizeof(req_sanitized) + sizeof(current_path) + 2);
    strcat(current_path, "/");
    strcat(current_path, req_sanitized);
    free(req_sanitized);
}

void set_path_as_dir_above() {
    char * new_path = (char *)malloc(strlen(current_path) + 1);
    strcpy(new_path, current_path);

    int slash_num_counter = 0;
    int slash_num = 0;
    int i;
    for(i = 0; i < strlen(new_path); i++) {
        if (new_path[i] == '/') slash_num++;
    }
    for(i = 0; i < strlen(new_path); i++) {
        if (new_path[i] == '/') slash_num_counter++;
        if (slash_num_counter == slash_num) break;
        new_path[i] = current_path[i];
    }
    new_path[i] = '\0';
    strcpy(current_path, new_path);
}
