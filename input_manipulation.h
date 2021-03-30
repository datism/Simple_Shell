#include <stdio.h>

#include "structures.h"

#ifndef INPUT_MANIPULATION_H
#define INPUT_MANIPULATION_H

char *take_input(char *batch, FILE *file);
Command **parse_command(char *input);

#endif