#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void write(const char * file, const char * size, const char * string) {
  DIRENTRY *entry = get_entry(file);

  if (entry == NULL) {
    printf("ERROR: %s does not exist.\n", file);
    return;
  }

  if (entry != NULL && entry->DIR_Attr == 0x10) {
    printf("ERROR: %s is a directory\n", file);
  } else if (!is_file_open(file)) {
    printf("ERROR: %s is not open.\n", file);
  } else if (!is_file_open_write(file)) {
    printf("ERROR: %s is not open for writing.\n", file);
  } else {
    // Not yet implemented.
  }
}
