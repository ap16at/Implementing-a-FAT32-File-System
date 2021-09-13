// for parsing boot sector
unsigned int BPB_BytsPerSec;
unsigned int BPB_SecPerClus;
unsigned int BPB_RsvdSecCnt;
unsigned int BPB_NumFATs;
unsigned int BPB_TotSec32;
unsigned int BPB_FATSz32;
unsigned int BPB_RootClus;

unsigned int FAT_Start;
unsigned int FAT_End;

// for working in directories
typedef struct DIRENTRY {
  unsigned char DIR_name[11];
  unsigned char DIR_Attr;
  unsigned char DIR_NTRes;
  unsigned char DIR_CrtTimeTenth;
  unsigned short DIR_CrtTime;
  unsigned short DIR_CrtDate;
  unsigned short DIR_LstAccDAte;
  unsigned char DIR_FstClusHI[2];
  unsigned short DIR_WrtTime;
  unsigned short DIR_WrtDate;
  unsigned char DIR_FstClusLO[2];
  unsigned int DIR_FileSize;
} __attribute__((packed)) DIRENTRY;

// Table for open files

typedef struct TableEntry{
  char name[32];
  unsigned int TE_FstClus;
  char mode[8];
  unsigned int Offset;
} TableEntry;

TableEntry ** table;
int current_table_size;

DIRENTRY ** current_directory;
int current_directory_size;
unsigned int current_cluster;
char * current_path;

// commands
void info(void);
void size(const char *);
void ls(const char *);
void cd(const char *);
void creat(char *);
void mkdir(char *);
void mv(char *, char *);
void rm(char *);
void rmdir(char *);
void open(const char*, const char*);
void close(const char*);
void lseek(const char *, const char *);
void read(const char *, const char *);
void write(const char *, const char *, const char *);
void cp(char *, char *);

// helpers
DIRENTRY ** get_directory_entries(const unsigned int);
DIRENTRY * get_entry(const char *);
unsigned int get_first_sector_of_cluster(const unsigned int);
unsigned int get_cluster_from_entry(const DIRENTRY *);
unsigned int get_next_cluster(const unsigned int);
unsigned int get_next_free_cluster(void);
int sanitized_strcmp(const char *, const char *);
int is_file_open(const char *);
int is_file_open_read(const char *);
int is_file_open_write(const char *);
int is_file(DIRENTRY *);
int is_dir(DIRENTRY *);
void create_entry_from_existing(DIRENTRY *);
void remove_existing_entry(DIRENTRY *);
void allocate_cluster(int);
void deallocate_from_cluster(int);
void set_cluster_for_entry(DIRENTRY *, unsigned int);
void set_name_for_entry(DIRENTRY *, const char *);
void write_entry_to_directory(DIRENTRY *);
void remove_existing_entry(DIRENTRY *);
void print_current_directory(void);
void parse_boot_sector(void);
void parse_user_input(void);
void sanitize(char *);
void set_current_path(const char *);
void set_path_as_dir_above(void);
void cd_to_root(void);
