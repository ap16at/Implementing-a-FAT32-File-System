# COP4610 Project 3: Implementing a FAT32 File System

## Team Members
- Tomas Munoz-Moxey
- Andrew Perez-Napan
- John Washer

## Division of Labor
- Part 1: exit - Tomas
- Part 2: info - Tomas
- Part 3: size - Tomas
- Part 4: ls - John, Tomas
- Part 5: cd - John
- Part 6: creat - John, Andrew
- Part 7: mkdir - John, Andrew
- Part 8: mv - John, Andrew
- Part 9: open - Andrew
- Part 10: close - Andrew
- Part 14: rm - John
- Part 15: cp - John, Tomas
- rmdir - John

### Git Log
See screenshot [here](https://www.dropbox.com/s/cb0rpz1muy7ksiv/gitlog.png?dl=0).  
Please note that due to differing experiences with source control, the git log may not accurately reflect the division of labor outlined above. Additionally one final commit by John will not be present, but will contain only the final updates to this file.

## Archive Contents
`README.md` contains a brief breakdown of the project and relevant information for grading.

##### File Breakdown

`shared.h` is a header file containing all helper functions and structs for the table entries and DIRENTRY entries.  
`main.c` is a C program used to parse the boot sector, set the current path, cluster, and directory. It then calls `parse_user_input()` and frees the current path.  
`parse_boot_sector.c` is a C program that opens the `fat32.img` file and sets the BPB fields.  
`parse_directory.c` is a C program storing all our helper functions. Including functions that: return directory entries, finds the cluster from an entry, finds the first sector of a cluster, finds the next cluster, finds the next free cluster, and writes an entry to the directory.  
`parse_table.c` is a C program that contains a function that checks for open files.  
`parse_user_input.c` is a C program that calls different functions depending on the user's input.  
`size.c` is a C program that returns the `DIR_FileSize` of a file DIRENTRY.  
`ls.c` is a C program that calls `print_current_directory()` and prints the entries in the current directory.  
`cd.c` is a C program that changes the current directory to the one entered by the user.  
`creat.c` is a C program that allows the user to create files and writes the entry to the image.  
`mkdir.c` is a C program that alllows the user to create a directory and writes the entry to the image.  
`mv.c` is a C program that allows the user to move a file or directory from one directory to another.  
`open.c` is a C program that adds a a struct object to a table List. This struct marks the file as open and sets its mode to the user's input.  
`close.c` is a C program that finds a file in the table list and removes it from that list.  
`rm.c` is a C program that allows the user to remove a file from a directory and deallocates the cluster it was kept in.  
`cp.c` is a C program that allows the user to copy a file to a directory.  
`rmdir.c` is a C program that allows the user to remove a directory and deallocates the cluster it was kept in.  
`lseek.c` is a C program that takes in a filename and offset, and outputs errors if the input is invalid in any way.  
`read.c` is a C program that takes in a filename and size, and outputs errors if the input is invalid in any way.  
`write.c` is a C program that takes in a filename, size, and string, and outputs errors if the input is invalid in any way.  
`Makefile` creates `project3`, an executable that allows for testing.  

## Compilation and Execution
Project was completed and tested on `cs.linprog.fsu.edu`.

##### To test `project3`:
```
> make
> ./project3
```
To clean, execute `make clean`.

## Known Bugs and Unfinished Portions
- creat
  - If a file is created with the same name as a directory, that name can be used repeatedly forever because only the directory is detected.
- lseek
  - Cannot compare the offset input to the `DIR_FileSize`
- mv
  - Even if there is not enough space to `mv`, the original file/directory will be removed.
### Unfinished portions:
- creat
  - Implementation for printing `bytes used` and `bytes remaining`
- mkdir
  - Implementation for printing `bytes used` and `bytes remaining`
- lseek
  - Call from user input works but function does nothing.
- read
  - Does not read the file.
- write
  - Does not write to the file.
