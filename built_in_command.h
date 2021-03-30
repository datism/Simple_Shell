#include "structures.h"

#ifndef BUILT_IN_COMMAND
#define BUILT_IN_COMMAND

int c_exit(Command *command);
void cd(Command *command);
void c_path(Command *command);

#endif