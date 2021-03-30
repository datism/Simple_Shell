#ifndef STRUCTURES_H
#define STRUCTURES_H 

#define MAX_PATH 128
#define MAX_COMMAND 32

extern int g_pids;

typedef struct Command 
{
	char **parsed_command;
	char *r_file;
}Command;

#endif