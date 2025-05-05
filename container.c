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

int host_network(pid_t veth_pid, char *host_ip) {
    pid_t pid;
    int status;
    // init veth
    printf("new veth\n");
    if ((pid = fork()) == 0) {
        signal(SIGTTOU, SIG_IGN);
        char pid_string[21]; // supports up to 64-bit pids
        sprintf(pid_string, "%d", veth_pid);
        char *args[] = {"/bin/ip", "link", "add", "host", "netns", "1", "type", "veth", "peer", "container", "netns", pid_string, NULL};
        execv("/bin/ip", args);
        perror("execv");
        exit(1);
    }
    
    
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        exit(1);
    }

    printf("ip addr\n");

    // add ip addr to veth
    if ((pid = fork()) == 0) {
        signal(SIGTTOU, SIG_IGN);
        char *args[] = {"/bin/ip", "addr", "add", host_ip, "dev", "host", NULL};
        execv("/bin/ip", args);
        perror("execv");
        exit(1);
    }

    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        exit(1);
    }

    printf("link up\n");
    // set veth up
    if ((pid = fork()) == 0) {
        signal(SIGTTOU, SIG_IGN);
        char *args[] = {"/bin/ip", "link", "set", "host", "up", NULL};
        execv("/bin/ip", args);
        perror("execv");
        exit(1);
    }

    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        exit(1);
    }

    return 0;
}

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
    
    
    // mount system image
    chdir("alpine");
    chroot(".");

    // mount proc
    mount("proc", "proc", "proc", 0, NULL);

    // TODO: set up networking
    
    // start shell
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
    printf("child %d, parent %d : %d\n", pid, getpid(), getpgid(getpid()));
    host_network(pid, "10.0.0.9/24"); // TODO: unique hostnames and ip addresses per container?
    // design decision - do I return terminal control and sleep until a new container is requested?

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        exit(1);
    }
    free(stack);

    return 0;
}