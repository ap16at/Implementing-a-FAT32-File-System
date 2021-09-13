#include <stdio.h>
#include "shared.h"

void size(const char * req) {
    DIRENTRY * entry = get_entry(req);

    if (entry == NULL) {
        printf("ERROR: File not found.\n");
    } else if (entry->DIR_Attr == 0x10) {
        printf("ERROR: %s is a directory.\n", req);
    } else {
        printf("Size of %s is: %i bytes.\n", req, entry->DIR_FileSize);
    }
}