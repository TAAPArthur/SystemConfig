#!/bin/tcc -run
#define _GNU_SOURCE
#include <sched.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd;
    int uid = getuid();
    int gid = getuid();
    char buffer[255] = {0};
    if(argc < 3) {
        dprintf(2, "usage %s uid gid [cmd [args]]\n", argv[0]);
        exit(2);
    }

    int newuid = atoi(argv[1]);
    int newgid = atoi(argv[2]);
    unshare(CLONE_NEWUSER);

    fd=open("/proc/self/uid_map",O_WRONLY);
    sprintf(buffer, "%d %d 1", newuid, uid);
    write(fd, buffer, strlen(buffer));
    close(fd);

    fd=open("/proc/self/gid_map",O_WRONLY);
    sprintf(buffer, "%d %d 1", newgid, gid);
    write(fd, buffer, strlen(buffer));
    close(fd);
    if(argv[3])
        execvp(argv[3], argv+3);
    else
        execl("/bin/sh", "/bin/sh", NULL);
}
