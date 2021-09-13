#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void rm(char * req) {
	DIRENTRY * entry = get_entry(req);

	if (entry == NULL) {
		printf("ERROR: File does not exist.\n");
	} else if (entry->DIR_Attr == 0x10) {
		printf("ERROR: Cannot use rm on Directory.\n");
	} else{
		unsigned int previous_cluster = current_cluster;
		deallocate_from_cluster(get_cluster_from_entry(entry));
		remove_existing_entry(entry);
	}
}

void remove_existing_entry(DIRENTRY * entry) {
	FILE* fatf = fopen("fat32.img", "r+");
	unsigned int first_sector = get_first_sector_of_cluster(current_cluster);
	unsigned int offset = 0;
	int index = 0;

	while (1) {
		DIRENTRY * seek_entry = (DIRENTRY *)malloc(sizeof(DIRENTRY));
		fseek(fatf, first_sector + offset, SEEK_SET);
		fread(seek_entry, 32, 1, fatf);

		if (seek_entry->DIR_name[0] == 0x00 || offset >= BPB_BytsPerSec * BPB_SecPerClus) {
			break;
		} else if (seek_entry->DIR_Attr != 0x0F) {
			const char * seek_name = (const char *)seek_entry->DIR_name;
			const char * entry_name = (const char *)entry->DIR_name;

			if (sanitized_strcmp(seek_name, entry_name) == 0) {
				if (index < current_directory_size - 1) {
					seek_entry->DIR_name[0] = 0xE5;
				} else {
					seek_entry->DIR_name[0] = 0x00;
				}
				fseek(fatf, first_sector + offset, SEEK_SET);
				fwrite(seek_entry, 32, 1, fatf);
				break;
			}
			index++;
		}
		offset += 32;
	}

	fclose(fatf);
	current_directory = get_directory_entries(current_cluster);
}

void deallocate_from_cluster(int n) {
	FILE * fatf = fopen("fat32.img", "r+");
	unsigned int cluster = n;

	do {
		unsigned int first_sector = get_first_sector_of_cluster(cluster);
		unsigned int offset = 0;
		do {
			unsigned int num = 0x0000000;
			fseek(fatf, first_sector + offset, SEEK_SET);
			fwrite(&num, 32, 1, fatf);
			offset += 32;
		} while (offset < BPB_BytsPerSec * BPB_SecPerClus);
		cluster = get_next_cluster(cluster);
	} while (cluster != 0x0FFFFFF8);
}
