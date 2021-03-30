#include<stdlib.h>

#include "memory_management.h"
#include "structures.h"

int g_pids;

void delocate_memory(Command **command)
{
	int i;
	for (i = 0; i < g_pids; ++i)
	{
		free((command[i])->parsed_command);
		free((command[i])->r_file);
		free(command[i]);
	}
	free(command);
}

Command** alocate_memory()
{
	Command **command_list = (Command **) malloc(sizeof(void *) * g_pids);
	int i;
	for (i = 0; i < g_pids; ++i)
	{
		*(command_list + i) = (Command *) malloc(sizeof(Command));
		(*(command_list + i))->parsed_command = (char **) malloc(MAX_COMMAND  * sizeof(void *));
	}
	return command_list;
}
