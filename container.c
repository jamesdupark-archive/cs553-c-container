// includes
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <errno.h>
// #include

#define CONTAINER_MEMORY 4096

int setup_sysimg(char *url) {
    struct stat *astat = malloc(sizeof(struct stat));
    if (stat("alpine", astat) == 0) {
        chdir("alpine");
        return 1;
    }
    free(astat);
    // dir doesn't exist, create and pull image
    mkdir("alpine", 0755);
    chdir("alpine");

    // pull system image
    pid_t pid;
    int status;
    if ((pid = fork()) == 0) {
        char *args[] = {"curl", "-o", "sysimg.tar.gz", url, NULL};
        execvp("curl", args);
        perror("curl");
        sem_unlink("veth");
        exit(1);
    }

    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        sem_unlink("veth");
        exit(1);
    }

    // tar to unzip
    if ((pid = fork()) == 0) {
        char *rgs[] = {"tar", "xvf", "sysimg.tar.gz", NULL};
        execvp("tar", rgs);
        perror("tar");
        sem_unlink("veth");
        exit(1);
    }

    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        sem_unlink("veth");
        exit(1);
    }

    // remove file
    if (remove("sysimg.tar.gz") < 0) {
        perror("remove");
        sem_unlink("veth");
        exit(1);
    }

    return 0;
}

int add_veth(pid_t veth_pid, char* endpoint1, char* endpoint2) {
    pid_t pid;
    int status;
    // init veth
    if ((pid = fork()) == 0) {
        // signal(SIGTTOU, SIG_IGN);
        char pid_string[21]; // supports up to 64-bit pids
        sprintf(pid_string, "%d", veth_pid);
        char *args[] = {"ip", "link", "add", endpoint1, "netns", "1", "type", "veth", "peer", endpoint2, "netns", pid_string, NULL};
        execvp("ip", args);
        perror("execvp");
        exit(1);
    }
    
    
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        exit(1);
    }

    return 0;
}

int init_veth(char *host_ip, char *hostname) {
    pid_t pid;
    int status;
    // add ip addr to veth
    if ((pid = fork()) == 0) {
        // signal(SIGTTOU, SIG_IGN);
        char *args[] = {"ip", "addr", "add", host_ip, "dev", hostname, NULL};
        execvp("ip", args);
        perror("execvp");
        exit(1);
    }

    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        exit(1);
    }

    // set veth up
    if ((pid = fork()) == 0) {
        // signal(SIGTTOU, SIG_IGN);
        char *args[] = {"ip", "link", "set", hostname, "up", NULL};
        execvp("ip", args);
        perror("execvp");
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
    char *av[] = {"sh", NULL};

    // set up semaphore
    sem_t *sem;
    if ((sem = sem_open("veth", 0)) == SEM_FAILED) {
        perror("semopen");
        sem_unlink("veth");
        exit(1);
    }

    // set pgid and take terminal control
    int pid = getpid();
    if (setpgid(pid, pid) < 0) {
        perror("setpgid");
        sem_unlink("veth");
        exit(1);
    }

    // printf("child %d : %d, parent %d\n", pid, getpgid(pid), getppid());
    signal(SIGTTOU, SIG_IGN);
    if (tcsetpgrp(STDIN_FILENO, pid) < 0) {
        perror("tcsetpgrp");
        sem_unlink("veth");
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
    
    
    // mount system image
    if (chroot(".") < 0) {
        perror("chroot");
        sem_unlink("veth");
        exit(1);
    }
    
    // mount proc
    mount("proc", "proc", "proc", 0, NULL);
    
    // set up networking
    if (sem_wait(sem) < 0) { // wait for host side to set up
        perror("semwait");
        sem_unlink("veth");
        exit(1);
    }
    sem_close(sem);
    sem_unlink("veth");
    init_veth("10.0.0.5/24", "container");
    
    // start shell
    execvp("sh", av);
    perror("execvp");
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

    // donwload filesystem if necessary
    setup_sysimg("https://dl-cdn.alpinelinux.org/alpine/v3.17/releases/x86_64/alpine-minirootfs-3.17.3-x86_64.tar.gz");

    // set up networking semaphore
    sem_t *sem;
    if ((sem = sem_open("veth", O_CREAT | O_EXCL, O_RDWR, 0)) == SEM_FAILED) {
        perror("semopen");
        sem_unlink("veth");
        exit(1);
    }
    
    // clone
    if ((pid = clone(create_container, stack + CONTAINER_MEMORY, clone_flags, NULL)) < 0) {
        perror("clone");
        sem_unlink("veth");
        exit(1);
    }
    // printf("child %d, parent %d : %d\n", pid, getpid(), getpgid(getpid()));
    // set up host networking    
    add_veth(pid, "host", "container");
    init_veth("10.0.0.9/24", "host"); // TODO: unique hostnames and ip addresses per container?
    if (sem_post(sem) < 0) {
        perror("sempost");
        sem_unlink("veth");
        exit(1);
    }
    sem_close(sem);
    // design decision - do I return terminal control and sleep until a new container is requested?

    sleep(1);
    int status = 0;
    if (waitpid(pid, &status, WUNTRACED) < 0) {
        perror("waitpid");
        sem_unlink("veth");
        exit(1);
    }
    free(stack);
    sem_unlink("veth");
    printf("exiting container!\n");

    return 0;
}