// includes
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
// #include

#define CONTAINER_MEMORY 4096

int create_container(void *args) {
    // char *args[] = {"/bin/chroot", ".", "sh", NULL};
    char *av[] = {"/bin/sh", NULL};

    // set pgid and take terminal control
    int pid = getpid();
    if (setpgid(pid, pid) < 0) {
        perror("setpgid");
        exit(1);
    }

    // printf("child %d : %d, parent %d\n", pid, getpgid(pid), getppid());
    signal(SIGTTOU, SIG_IGN);
    if (tcsetpgrp(STDIN_FILENO, pid) < 0) {
        perror("tcsetpgrp");
        exit(1);
    }
    // if (tcsetpgrp(STDOUT_FILENO, pid) < 0) {
    //     perror("tcsetpgrp");
    //     exit(1);
    // }
    // if (tcsetpgrp(STDERR_FILENO, pid) < 0) {
    //     perror("tcsetpgrp");
    //     exit(1);
    // }

    // check if system image is present
    struct stat *astat = malloc(sizeof(struct stat));
    if (stat("alpine", astat) < 0) { // TODO: download and extract image, flags for image URL/location
        perror("stat");
    }
    free(astat);
    chdir("alpine");
    chroot(".");
    mount("proc", "proc", "proc", 0, NULL);

    // mount system image

    // mount proc
    
    execv("/bin/sh", av);
    perror("execv");
    exit(1);
}


int main(int argc, char **argv) {
    printf("Hello world! %d, %s\n", argc, argv[0]);
    // parse flags
    if (argc == 1) {

    }

    int clone_flags = CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWNET; // pfmn

    // start new shell process
    pid_t pid;
    void *stack = malloc(CONTAINER_MEMORY);
    if ((pid = clone(create_container, stack + CONTAINER_MEMORY, clone_flags, NULL)) < 0) {
        perror("clone");
        exit(1);
    }
    sleep(2);
    // printf("child %d, parent %d : %d\n", pid, getpid(), getpgid(getpid()));

    int status = 0;
    wait(&status);
    // free(stack);

    return 0;
}