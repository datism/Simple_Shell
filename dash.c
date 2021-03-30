#include <stdlib.h>
#include <stdio.h>

#include "structures.h"
#include "main_loop.h"

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        perror("Too many arguments.\n");
        exit(1);
    }
    if (argc == 1)
    {
        run_shell(NULL);
    }
    else
    {
        run_shell(argv[1]);
    }
}