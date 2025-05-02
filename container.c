// includes
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
// #include


// int create_container(void *args) {

// }


int main(int argc, char **argv) {
    printf("Hello world! %d, %s\n", argc, argv[0]);
    // parse flags
    if (argc == 1) {

    }

    // start new shell process
    pid_t pid;
    if ((pid = fork()) == 0) { // TODO: check for errors
        char *av[] = {"/usr/bin/sh", NULL};
        // char *arr[] = {"/bin/echo", "hello", NULL};
        pid = getpid();
        signal(SIGTTOU, SIG_IGN);
        printf("pgid: %d\n", getpgid(pid));
        if (setpgid(pid, pid) < 0) {
            perror("setpgid");
            exit(1);
        } // TODO: check for errors
        printf("child %d : %d, parent %d\n", pid, getpgid(pid), getppid());
        if (tcsetpgrp(STDIN_FILENO, pid) < 0) {
            perror("tcsetpgrp");
            exit(1);
        } // TODO: check for errors
        // if (tcsetpgrp(STDOUT_FILENO, pid) < 0) {
        //     perror("tcsetpgrp");
        //     exit(1);
        // } // TODO: check for errors
        execv("/usr/bin/sh", av);
        perror("execv");
        exit(1);
    }
    sleep(2);
    printf("child %d, parent %d : %d\n", pid, getpid(), getpgid(getpid()));
    // char *arr[] = {"/usr/bin/sh", NULL};
    // execv("/usr/bin/sh", arr);

    int status = 0;
    wait(&status);

    return 0;
}