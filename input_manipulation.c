#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "input_manipulation.h"
#include "structures.h"
#include "memory_management.h"

int g_pids;

void remove_white_space(char *arr)
{
	int n = strlen(arr);
	for (int i = 0; i < n; i++)
    {
    	if(arr[i] == '\t')
    		arr[i] = ' ';
    }
    // space is 1 when a space character is found and
    // 0 when any non-space character is found
    int space = 0;
 
    // `k` points to the next free position
    int k = 0;
 
    // iterate through the characters of the string
    for (int i = 0; i < n; i++)
    {
        // handle leading spaces in the string
        while (k == 0 && i < n && arr[i] == ' ') {
            i++;
        }
 
        // if the current character is a space
        if (arr[i] == ' ')
        {
            // if the flag was 0 earlier, i.e., the first occurrence of a
            // space after a word
            if (!space)
            {
                // copy current char (whitespace) at the next free index
                // and set the flag
                arr[k++] = arr[i];
                space = 1;
            }
        }
        // if the current character is a punctuation mark
        else {
            // copy the current character at the next free index
            arr[k++] = arr[i];
            space = 0;
        }
    }
 
    // handle trailing spaces in the string
    arr[k] = '\0';
}


char *take_input(char *batch, FILE *file)
{
	char *command;
	size_t buffer_size = 0;
	size_t characters;

	command = (char *)malloc(buffer_size * sizeof(char));
	if (command == NULL)
	{
		perror("Unable to alocate memory in input\n");
		exit(1);
	}

	if(batch != NULL)
		characters = getline(&command, &buffer_size, file);
	else
		characters = getline(&command,&buffer_size,stdin);

	if (characters == -1)
	{
		perror("cannot read command\n");
		return NULL;
	}

	if (command[characters -1] == '\n')
	{
		command[characters - 1] = '\0';
	}
	remove_white_space(command);
	return command;
}

char** parse_line(char *input)
{
	char** parsed_line = (char **) malloc(MAX_COMMAND  * sizeof(void *));
	if (parsed_line == NULL)
	{
		perror("Unable to alocate memory in prase\n");
		exit(1);
	}
	int windex = 0;
    
    while ( (parsed_line[windex] = strsep(&input , " ")) != NULL )
    {	
    	if ( strcmp(parsed_line[windex], "&") == 0)
    	{
    		g_pids++;
    	}
    	windex++;
    }
	return parsed_line;
}

Command **parse_command(char *input)
{
	char** parsed_line = parse_line(input);
	Command** command_list = alocate_memory();
	int line_index;
	int list_index = 0;
	int command_index = 0;
	int redirection = 0;
	for (line_index = 0; parsed_line[line_index] != NULL; ++line_index)
	{
		if (strcmp(parsed_line[line_index], "&") == 0)
		{
			redirection = 0;
			command_index = 0;
			++list_index;
			continue;
		}

		if (strcmp(parsed_line[line_index], ">") == 0)
		{
			if (redirection == 1)
			{
				perror("Multiple redirection operators\n");
				return  NULL;
			}
			redirection = 1;
			if (parsed_line[line_index + 1] != NULL && (strcmp(parsed_line[line_index + 1], ">") != 0) )
			{
				line_index++;
				if (((command_list[list_index])->r_file = strdup(parsed_line[line_index])) == NULL )
				{
					perror("Unable to strdup in parse_command\n");
					return  NULL;
				}

				if (parsed_line[line_index+1] != NULL) 
				{
					if  (strcmp(parsed_line[line_index+1], "&") == 0)
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

		if (((command_list[list_index])->parsed_command[command_index] = strdup(parsed_line[line_index])) == NULL )
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