#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

DIRENTRY ** get_directory_entries(const unsigned int n) {
	FILE* fatf;
	fatf = fopen("fat32.img", "r");

	static DIRENTRY * entries[2048];
	current_directory_size = 0;
	int current_cluster = n;
	DIRENTRY entry;

	do {
		unsigned int first_sector = get_first_sector_of_cluster(current_cluster);
		unsigned int offset = 0;
		while (1) {
			DIRENTRY * entry = (DIRENTRY *)malloc(sizeof(DIRENTRY));
			fseek(fatf, first_sector + offset, SEEK_SET);
			fread(entry, 32, 1, fatf);

			if (entry->DIR_name[0] == 0x00 || offset >= BPB_BytsPerSec * BPB_SecPerClus) {
				break;
			} else if (entry->DIR_name[0] != 0xE5 && entry->DIR_Attr != 0x0F) {
				entries[current_directory_size++] = entry;
			}
			offset += 32;
		}
		current_cluster = get_next_cluster(current_cluster);
	} while (current_cluster != 0x0FFFFFF8);
	return entries;
}

int sanitized_strcmp(const char * str1, const char * str2) {
	char * str1_sanitized = (char*)malloc(strlen(str1) + 1);
	char * str2_sanitized = (char *)malloc(strlen(str2) + 1);
	strcpy(str1_sanitized, str1);
	strcpy(str2_sanitized, str2);
	sanitize(str1_sanitized);
	sanitize(str2_sanitized);
	int x = strcmp(str1_sanitized, str2_sanitized);
	free(str1_sanitized);
	free(str2_sanitized);
	return x;
}

void print_current_directory() {
    for(int i = 0; i < current_directory_size; i++) {
		char * entry_name = (char *)current_directory[i]->DIR_name;
		char * entry_name_sanitized = (char*)malloc(strlen(entry_name) + 1);
		strcpy(entry_name_sanitized, entry_name);
		sanitize(entry_name_sanitized);
		printf("%s    ", entry_name_sanitized);
		free(entry_name_sanitized);
		if (i > 10) printf("\n");
    }
	printf("\n");
}

DIRENTRY * get_entry(const char * req) {
    for (int i = 0; i < current_directory_size; i++) {
		const char * entry_name = (const char *)current_directory[i]->DIR_name;
        if (sanitized_strcmp(entry_name, req) == 0) {
			return current_directory[i];
		}
    }
	return NULL;
}

unsigned int get_cluster_from_entry(const DIRENTRY * entry) {
	unsigned int cluster = entry->DIR_FstClusLO[0];
	cluster |= entry->DIR_FstClusLO[1] << 8;
	cluster |= entry->DIR_FstClusHI[0] << 16;
	cluster |= entry->DIR_FstClusHI[1] << 24;
	return cluster;
}

unsigned int get_first_sector_of_cluster(const unsigned int n) {
	unsigned int first_data_sector = BPB_RsvdSecCnt + BPB_NumFATs * BPB_FATSz32;
	unsigned int first_sector_of_cluster = (n - 2) * BPB_SecPerClus + first_data_sector;
	return first_sector_of_cluster * BPB_BytsPerSec;
}

unsigned int get_next_cluster(const unsigned int n) {
	FILE* fatf;
	fatf = fopen("fat32.img", "r");
	unsigned int FATOffset = n * 4;
	unsigned int ThisFATSecNum =  BPB_RsvdSecCnt + FATOffset / BPB_BytsPerSec;
	unsigned int num;

	fseek(fatf, ThisFATSecNum * BPB_BytsPerSec + FATOffset, SEEK_SET);
	fread(&num, 4, 1, fatf);
	fclose(fatf);

	return num;
}

unsigned int get_next_free_cluster(void) {
	unsigned int pos = FAT_Start;
	unsigned int cluster_num = 0;
	unsigned int num;
	FILE* fatf;
	fatf = fopen("fat32.img", "r");

	do {
		fseek(fatf, pos, SEEK_SET);
		fread(&num, 4, 1, fatf);
		pos += 4;
		++cluster_num;
	} while(num != 0x00000000 || pos >= FAT_End);

	return --cluster_num;
}

void allocate_cluster(int n) {
    unsigned int offset = FAT_Start + n * 4;
    unsigned int num = 0xFFFFFFFF;

    FILE * fatf = fopen("fat32.img", "r+");
    fseek(fatf, offset, SEEK_SET);
    fwrite(&num, 4, 1, fatf);
    fclose(fatf);
}

void set_cluster_for_entry(DIRENTRY * entry, unsigned int n) {
	entry->DIR_FstClusLO[0] = n;
	entry->DIR_FstClusHI[1] = n >> 24;
	entry->DIR_FstClusHI[0] = n >> 16;
	entry->DIR_FstClusLO[1] = n >> 8;
}

void set_name_for_entry(DIRENTRY * entry, const char * req) {
    for(int i = 0; i < 11; i++) {
        if (i < strlen(req)) {
            entry->DIR_name[i] = req[i];
        } else {
            entry->DIR_name[i] = ' ';
        }
    }
    entry->DIR_name[10] = '\0';
}

void write_entry_to_directory(DIRENTRY * new_entry) {
	FILE* fatf = fopen("fat32.img", "r+");
	unsigned int first_sector = get_first_sector_of_cluster(current_cluster);
	unsigned int offset = 0;

	while (1) {
		DIRENTRY * entry = (DIRENTRY *)malloc(sizeof(DIRENTRY));
		fseek(fatf, first_sector + offset, SEEK_SET);
		fread(entry, 32, 1, fatf);

		if (entry->DIR_name[0] == 0x00 || entry->DIR_name[0] == 0xE5) {
			fseek(fatf, first_sector + offset, SEEK_SET);
			fwrite(new_entry, 32, 1, fatf);
			break;
		} else if (offset >= BPB_BytsPerSec * BPB_SecPerClus) {
			printf("ERROR: No space for file.\n");
			break;
		}
		offset += 32;
	}

	fclose(fatf);
	current_directory = get_directory_entries(current_cluster);
}
