#define _XOPEN_SOURCE 700 
#define _DEFAULT_SOURCE


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_PATH 128
#define MAX_COMMAND 32

int pids = 1;

typedef struct Command 
{
	char **parsed_command;
	char *file_redirect_to;
}Command;


void set_path();
void add_current_directory();
char *Input();
char** parse_line(char *input);
void exec(Command *command);
void change_directory(char *directory);
Command **parse_command(char *input);



int main(int argc, char *argv[])
{

	char *prompt = "dash> ";
	char *input;
	while(1)
	{
		pids = 1;
		// print prompt
		printf("%s", prompt);

		// take input command
		input= Input();

		//prase command
		Command **command = parse_command(input);
		int list_index;
		for(list_index = 0; list_index < pids; ++list_index)
		{
			if(command[list_index] == NULL || (command[list_index])->parsed_command[0] == NULL)
			{
				continue;
			}

			// exit commnand
			if(strcmp((command[list_index])->parsed_command[0], "exit") == 0)
			{
				if((command[list_index])->parsed_command[1] != NULL)
				{
					perror("Exit does not take any parameters.\n");
					continue;
				}
				else
					exit(0);
			}

			if(strcmp((command[list_index])->parsed_command[0], "cd") == 0)
			{
				if((command[list_index])->parsed_command[1] == NULL || (command[list_index])->parsed_command[2] != NULL)
					perror("cd must take one argument.\n");
				else 
					change_directory((command[list_index])->parsed_command[1]);
				continue;

			}

			if(strcmp((command[list_index])->parsed_command[0], "path") == 0)
			{
				char *path;
				if ((command[list_index])->parsed_command[1] == NULL)
					path = strdup("");
				else
					path = strdup((command[list_index])->parsed_command[1]);
				set_path(path, 1);

				int i = 2;
				while((command[list_index])->parsed_command[i] != NULL)
				{
					set_path((command[list_index])->parsed_command[i], 0);
					i++;
				}
				free(path);
				continue;
			}

			exec((command[list_index]));
		}

		// int i;
		// for( i = 0; i < pids; ++i)
		// {
		// 	int cindex = 0;
		// 	printf("Command: ");
		// 	while( (command[i])->parsed_command[cindex] != NULL)
		// 	{
		// 		printf("%s ", (command[i])->parsed_command[cindex]);
		// 		cindex++;
		// 	}
		// 	printf("\n");
		// 	if( (command[i])->file_redirect_to != NULL )
		// 		printf("Redirect to: %s\n", (command[i])->file_redirect_to);
		// }
	}
}

char *Input()
{
	char *command;
	size_t buffer_size = 64;
	size_t characters;

	command = (char *)malloc(buffer_size * sizeof(char));

	if(command == NULL)
	{
		perror("Unable to alocate memory in input\n");
		exit(1);
	}

	characters = getline(&command,&buffer_size,stdin);

	if(characters == -1)
	{
		perror("cannot read command\n");
		exit(1);
	}

	if(command[characters -1] == '\n')
	{
		command[characters - 1] = '\0';
	}

	return command;
}

char** parse_line(char *input)
{
	char** parsed_line = (char **) malloc(MAX_COMMAND  * sizeof(void *));
	if(parsed_line == NULL)
	{
		perror("Unable to alocate memory in prase\n");
		exit(1);
	}
	int windex = 0;
    
    while( (parsed_line[windex] = strsep(&input , " ")) != NULL )
    {	
    	if( strcmp(parsed_line[windex], "&") == 0)
    	{
    		pids++;
    	}
    	windex++;
    }
	return parsed_line;
}

Command** alocate_memory()
{
	Command **command_list = (Command **) malloc(sizeof(void *) * pids);
	int i;
	for(i = 0; i < pids; ++i)
	{
		*(command_list + i) = (Command *) malloc(sizeof(Command));
		(*(command_list + i))->parsed_command = (char **) malloc(MAX_COMMAND  * sizeof(void *));
	}
	return command_list;
}

Command **parse_command(char *input)
{
	char** parsed_line = parse_line(input);
	Command** command_list = alocate_memory();
	int line_index;
	int list_index = 0;
	int command_index = 0;
	int redirection = 0;
	for(line_index = 0; parsed_line[line_index] != NULL; ++line_index)
	{
		if(strcmp(parsed_line[line_index], "&") == 0)
		{
			redirection = 0;
			command_index = 0;
			++list_index;
			continue;
		}

		if(strcmp(parsed_line[line_index], ">") == 0)
		{
			if(redirection == 1)
			{
				perror("Multiple redirection operators\n");
				return  NULL;
			}
			redirection = 1;
			if(parsed_line[line_index + 1] != NULL && (strcmp(parsed_line[line_index + 1], ">") != 0) )
			{
				line_index++;
				if( ((command_list[list_index])->file_redirect_to = strdup(parsed_line[line_index])) == NULL )
				{
					perror("Unable to strdup in parse_command\n");
					return  NULL;
				}

				if( (parsed_line[line_index+1] != NULL) )
				{
					if ( (strcmp(parsed_line[line_index+1], "&") == 0) )
					{
						continue;
					}
					perror("Canot redirect to multiple file\n");
					return NULL;
				}
			}
			else
			{
				perror("Must redirect to a file\n");
				return NULL;

			}
			continue;
		}

		if( ((command_list[list_index])->parsed_command[command_index] = strdup(parsed_line[line_index])) == NULL )
		{
			perror("Unable to strdup in parse_command\n");
			return  NULL;
		}
		else
		{
			command_index++;
		}
	}
	return command_list;

}


void exec(Command *command)
{
	int rc = fork();
	if(rc < 0)
	{
		//fork failed. Exit
		perror("fork failed\n");
		exit(1);
	}
	else if (rc == 0)
	{
		if(command->file_redirect_to != NULL)
		{
			close(STDOUT_FILENO);
			open(command->file_redirect_to, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
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

char* Current_directory()
{
	char *current_directory, *ptr;
    if ( (current_directory = (char *) malloc( sizeof(char) * MAX_PATH)) != NULL)
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

void set_path(char *path, int set_or_add)
{
	if(set_or_add == 1)
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