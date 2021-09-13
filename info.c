#include <stdio.h>
#include "shared.h"

void info(void) {
	printf("Bytes per sector: %i\n", BPB_BytsPerSec);
	printf("Sectors per cluster: %i\n", BPB_SecPerClus);
	printf("Reserved sector count: %i\n", BPB_RsvdSecCnt);
	printf("Number of FATs: %i\n", BPB_NumFATs);
	printf("Total sectors: %i\n", BPB_TotSec32);
	printf("FAT size: %i\n", BPB_FATSz32);
	printf("Root cluster: %i\n", BPB_RootClus);
}
