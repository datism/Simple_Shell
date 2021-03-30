#define _DEFAULT_SOURCE

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#include "structures.h"
#include "input_manipulation.h"
#include "memory_management.h"
#include "built_in_command.h"

int g_pids;

void run_program(Command *command);

int main(int argc, char *argv[])
{

	char *prompt = "dash> ";
	char *input;
	while (1)
	{
		g_pids = 1;
		// print prompt
		printf("%s", prompt);

		// take input
		input= take_input();

		//prase command
		Command **command = parse_command(input);
		int list_index;
		for (list_index = 0; list_index < g_pids; ++list_index)
		{
			if (command[list_index] == NULL || (command[list_index])->parsed_command[0] == NULL)
			{
				continue;
			}

			// exit commnand
			if (strcmp((command[list_index])->parsed_command[0], "exit") == 0)
				if ( c_exit(command[list_index]) == -1 )
					continue;

			if (strcmp((command[list_index])->parsed_command[0], "cd") == 0)
			{
				cd(command[list_index]);
				continue;

			}

			if (strcmp((command[list_index])->parsed_command[0], "path") == 0)
			{
				c_path(command[list_index]);
				continue;
			}

			run_program((command[list_index]));
		}

		// int i;
		// for  ( i = 0; i < pids; ++i)
		// {
		// 	int cindex = 0;
		// 	printf("Command: ");
		// 	while( (command[i])->parsed_command[cindex] != NULL)
		// 	{
		// 		printf("%s ", (command[i])->parsed_command[cindex]);
		// 		cindex++;
		// 	}
		// 	printf("\n");
		// 	if ( (command[i])->r_file != NULL )
		// 		printf("Redirect to: %s\n", (command[i])->r_file);
		// }
		pid_t rc = wait(NULL);
		delocate_memory(command);
	}
}

void run_program(Command *command)
{
	int rc = fork();
	if (rc < 0)
	{
		//fork failed. Exit
		perror("fork failed\n");
		exit(1);
	}
	else if  (rc == 0)
	{
		if (command->r_file != NULL)
		{
			close(STDOUT_FILENO);
			open(command->r_file, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
		}     
        execvp(command->parsed_command[0], command->parsed_command);
        // run program failed
        perror("program failed\n"); 
        exit(1);
	}
	else
	{
		return;
	}
}
