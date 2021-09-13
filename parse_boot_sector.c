#include <stdio.h>
#include "shared.h"

void parse_boot_sector(void) {
	FILE* fatf;
	fatf = fopen("fat32.img", "r");
	unsigned int num;

	fseek(fatf, 11, SEEK_SET);
    fread(&num, 2, 1, fatf);
    BPB_BytsPerSec = num;
	num = 0;
	fseek(fatf, 13, SEEK_SET);
    fread(&num, 1, 1, fatf);
    BPB_SecPerClus = num;
	num = 0;
	fseek(fatf, 14, SEEK_SET);
    fread(&num, 2, 1, fatf);
    BPB_RsvdSecCnt = num;
	num = 0;
	fseek(fatf, 16, SEEK_SET);
    fread(&num, 1, 1, fatf);
    BPB_NumFATs = num;
	num = 0;
	fseek(fatf, 32, SEEK_SET);
    fread(&num, 4, 1, fatf);
    BPB_TotSec32 = num;
	num = 0;
	fseek(fatf, 36, SEEK_SET);
    fread(&num, 4, 1, fatf);
    BPB_FATSz32 = num;
	num = 0;
	fseek(fatf, 44, SEEK_SET);
    fread(&num, 4, 1, fatf);
    BPB_RootClus = num;
	fclose(fatf);

	FAT_Start = ( BPB_RsvdSecCnt + ( (BPB_RootClus * 4) / BPB_BytsPerSec ) ) * BPB_BytsPerSec;
	FAT_End = BPB_FATSz32 * BPB_BytsPerSec + FAT_Start;
}
