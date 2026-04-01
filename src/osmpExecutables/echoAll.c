/******************************************************************************
 * FILE: echoall.c
 * AUTHOR: BS-Team
 * DESCRIPTION:
 * First OSMP Executable, it simply echoes all arguments passed to it.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

void print_arguments(int argc, char **argv) {
    int index = 1;
    while (index < argc) {
        fprintf(stdout, "argv[%d] = %s\n", index, argv[index]);
        index++;
    }
}

int print_process_information(int loops) {
    int iteration = 0;
    while (iteration < loops) {
        long tid = syscall(SYS_gettid);
        if (tid < 0) {
            perror("syscall(SYS_gettid) fehlgeschlagen");
            return EXIT_FAILURE;
        }

        fprintf(stdout, "Iteration %d: PID=%ld PPID=%ld TID=%ld\n", iteration + 1,
                (long)getpid(), (long)getppid(), tid);
        iteration++;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Fehler: Keine Nutzargumente angegeben.\n");
        fprintf(stderr, "Aufrufbeispiel: %s hallo welt\n", argv[0]);
        return EXIT_FAILURE;
    }

    print_arguments(argc, argv);
    return print_process_information(3);
}
