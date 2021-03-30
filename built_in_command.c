#define _XOPEN_SOURCE 700 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "built_in_command.h"
#include "structures.h"

void set_path(char *path, int set_or_add)
{
	if (set_or_add == 1)
	{
		setenv("PATH", path, 1);
		return;
	}
	char *current_path;
	current_path = getenv("PATH");
	current_path = strcat(current_path, ":");
	current_path = strcat(current_path, path);
	return;
}

char* Current_directory()
{
	char *current_directory, *ptr;
    if ((current_directory = (char *) malloc( sizeof(char) * MAX_PATH)) != NULL)
    {
        if ((ptr = getcwd(current_directory, MAX_PATH)) == NULL )
        {
            exit(1);
        }
    }
    else
    {
        exit(1);
    }
    set_path(current_directory, 0);
    return current_directory;
}

void change_directory(char *directory)
{
	if ( chdir(directory) != 0 )
	{
		printf("chdir() failed\n");
		return;
	}
	printf( "%s\n", Current_directory());
	return;
}

int c_exit(Command *command)
{
	if (command->parsed_command[1] != NULL)
	{
		perror("Exit does not take any parameters.\n");
		return -1;
	}
	exit(0);
}

void cd(Command *command)
{
	if (command->parsed_command[1] == NULL || command->parsed_command[2] != NULL)
		perror("cd must take one argument.\n");
	change_directory(command->parsed_command[1]);
}

void c_path(Command *command)
{
	char *path;
	if (command->parsed_command[1] == NULL)
		path = strdup("");
	else
		path = strdup(command->parsed_command[1]);
	set_path(path, 1);

	int i = 2;
	while  (command->parsed_command[i] != NULL)
	{
		set_path(command->parsed_command[i], 0);
		i++;
	}
	free(path);
}