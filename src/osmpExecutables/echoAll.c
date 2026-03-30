/******************************************************************************
 * FILE: echoall.c
 * AUTHOR: BS-Team
 * DESCRIPTION:
 * First OSMP Executable, it simply echoes all arguments passed to it.
 ******************************************************************************/

#include <stdio.h>

#define UNUSED(x) (void)(x);

int main(int argc, char **argv) {
    UNUSED(argc);
    UNUSED(argv);

    fprintf(stdout, "Geben Sie hier zunächst alle Argumente aus.\n");

    fprintf(stdout, "Dann die tid, pid und ppid in einer Schleife.\n");

    return 0;
}
