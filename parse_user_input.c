#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "shared.h"

typedef struct {
	int size;
	char **items;
} tokenlist;

char *get_input(void);
tokenlist *get_tokens(char *input);
tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);
void clearscr(void);

void parse_user_input(void) {
	while (1) {
        printf("~%s> ", current_path);

        char *input = get_input();
        tokenlist *tokens = get_tokens(input);

        if (strlen(input) == 0) {
			printf("ERROR: Please enter a command.\n");
            continue;
		} else if (strcmp(input, "clear") == 0) {
			clearscr();
		} else if (strcmp(input, "exit") == 0) {
            break;
		} else if (strcmp(input, "info") == 0) {
            info();
		} else if (strcmp(tokens->items[0], "size") == 0) {
			if (tokens->size == 2) {
				size(tokens->items[1]);
			} else {
				printf("ERROR: Usage: size FILENAME.\n");
			}
		} else if (strcmp(tokens->items[0], "ls") == 0) {
			if (tokens->size == 1) {
				print_current_directory();
			} else if (tokens->size == 2) {
				ls(tokens->items[1]);
			} else {
				printf("ERROR: Usage: ls DIRNAME.\n");
			}
		} else if (strcmp(tokens->items[0], "cd") == 0) {
			if (tokens->size == 1) {
				cd_to_root();
			} else if (tokens->size == 2) {
				cd(tokens->items[1]);
			} else {
				printf("ERROR: Usage: cd DIRNAME.\n");
			}
		} else if (strcmp(tokens->items[0], "creat") == 0) {
			if (tokens->size == 2) {
				creat(tokens->items[1]);
			} else {
				printf("Error: Usage: creat FILENAME.\n");
			}
		} else if (strcmp(tokens->items[0], "mkdir") == 0) {
			if (tokens->size == 2) {
				mkdir(tokens->items[1]);
			} else {
				printf("Error: Usage: mkdir DIRNAME.\n");
			}
		} else if (strcmp(tokens->items[0], "mv") == 0) {
			if (tokens->size == 3) {
				mv(tokens->items[1], tokens->items[2]);
			} else {
				printf("Error: Usage: mv FROM TO.\n");
			}
		} else if (strcmp(tokens->items[0], "rm") == 0) {
			if (tokens->size == 2) {
				rm(tokens->items[1]);
			} else {
				printf("Error: Usage: rm FILENAME.\n");
			}
		} else if (strcmp(tokens->items[0], "rmdir") == 0) {
			if (tokens->size == 2) {
				rmdir(tokens->items[1]);
			} else {
				printf("Error: Usage: rmdir DIRNAME.\n");
			}
		} else if (strcmp(tokens->items[0], "open") == 0) {
			if (tokens->size == 3) {
				open(tokens->items[1], tokens->items[2]);
			} else {
				printf("Error: Usage: open FILENAME MODE.\n");
			}
		} else if (strcmp(tokens->items[0], "close") == 0) {
			if (tokens->size == 2) {
				close(tokens->items[1]);
			} else {
				printf("Error: Usage: close FILENAME.\n");
			}
		} else if (strcmp(tokens->items[0], "cp") == 0) {
			if (tokens->size == 3) {
				cp(tokens->items[1], tokens->items[2]);
			} else {
				printf("Error: Usage: cp FILENAME TO.\n");
			}
		} else if (strcmp(tokens->items[0], "lseek") == 0) {
			if (tokens->size == 3) {
				lseek(tokens->items[1], tokens->items[2]);
			} else {
				printf("Error: Usage: lseek FILENAME OFFSET.\n");
			}
		} else if (strcmp(tokens->items[0], "read") == 0) {
			if (tokens->size == 3) {
				read(tokens->items[1], tokens->items[2]);
			} else {
				printf("Error: Usage: read FILENAME SIZE.\n");
			}
		} else if (strcmp(tokens->items[0], "write") == 0) {
			if (tokens->size == 4) {
				write(tokens->items[1], tokens->items[2], tokens->items[3]);
			} else {
				printf("Error: Usage: write FILENAME SIZE STRING.\n");
			}
		} else {
			printf("ERROR: Invalid command.\n");
		}

        free(input);
        free_tokens(tokens);
    }
}

void clearscr(void) {
	#ifdef _WIN32
		system("cls");
	#elif defined(unix) || defined(__unix__) || defined(__unix)
		system("clear");
	#elif defined(__APPLE__) && defined(__MACH__)
		system("clear");
	#else
		printf("ERROR: OS not supported.\n");
	#endif
}

void sanitize(char *str) {
	int w = 0;
	for (int i = 0; str[i]; i++)
        if (isalpha(str[i]) || isdigit(str[i]) || str[i] == '.') {
            str[w] = str[i];
            str[w] = tolower(str[w]);
            w++;
        }
    str[w] = '\0';
}

tokenlist *new_tokenlist(void) {
	tokenlist *tokens = (tokenlist *) malloc(sizeof(tokenlist));
	tokens->size = 0;
	tokens->items = (char **) malloc(sizeof(char *));
	tokens->items[0] = NULL; /* make NULL terminated */
	return tokens;
}

void add_token(tokenlist *tokens, char *item) {
	int i = tokens->size;

	tokens->items = (char **) realloc(tokens->items, (i + 2) * sizeof(char *));
	tokens->items[i] = (char *) malloc(strlen(item) + 1);
	tokens->items[i + 1] = NULL;
	strcpy(tokens->items[i], item);

	tokens->size += 1;
}

char *get_input(void) {
	char *buffer = NULL;
	int bufsize = 0;

	char line[5];
	while (fgets(line, 5, stdin) != NULL) {
		int addby = 0;
		char *newln = strchr(line, '\n');
		if (newln != NULL)
			addby = newln - line;
		else
			addby = 5 - 1;

		buffer = (char *) realloc(buffer, bufsize + addby);
		memcpy(&buffer[bufsize], line, addby);
		bufsize += addby;

		if (newln != NULL)
			break;
	}

	buffer = (char *) realloc(buffer, bufsize + 1);
	buffer[bufsize] = 0;

	return buffer;
}

tokenlist *get_tokens(char *input) {
	char *buf = (char *) malloc(strlen(input) + 1);
	strcpy(buf, input);

	tokenlist *tokens = new_tokenlist();

	char *tok = strtok(buf, " ");
	while (tok != NULL) {
		add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}

	free(buf);
	return tokens;
}

void free_tokens(tokenlist *tokens) {
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);
	free(tokens->items);
	free(tokens);
}
