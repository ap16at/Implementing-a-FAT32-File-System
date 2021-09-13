#include <stdio.h>
#include <stdlib.h>
#include "shared.h"

int main() {
    parse_boot_sector();
    current_path = (char *)malloc(sizeof(char *));
    current_cluster = BPB_RootClus;
    current_directory = get_directory_entries(current_cluster);
    parse_user_input();
    free(current_path);
	return 0;
}