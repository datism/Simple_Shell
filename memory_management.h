#include<stdlib.h>

#include "structures.h"


#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

void delocate_memory(Command **command);


Command** alocate_memory();

#endif