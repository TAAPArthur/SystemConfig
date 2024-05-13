#!/bin/tcc -run

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

void setDesiredTerminalAttributes(int fd) {
    struct termios term;
    tcgetattr(fd, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(fd, 0, &term);
}
int createMasterTTY() {
    int master_tty = posix_openpt(O_RDWR);
    if (master_tty == -1 || grantpt(master_tty) == -1 || unlockpt(master_tty) == -1) {
        perror("Failed to create/grant/unlock tty");
        exit(1);
    }

    struct winsize const win = {10, 10};
    ioctl(master_tty, TIOCSWINSZ, &win);
    setDesiredTerminalAttributes(master_tty);
    return master_tty;
}

int createSlaveTTY(int master_tty) {
    const char* name = ptsname(master_tty);
    if (!name) {
        perror("Failed to get name of tty");
        exit(2);
    }
    int fd = open(name, O_RDWR);
    if (fd == -1 ){
        perror("Failed to open tty");
        exit(3);
    }
    setDesiredTerminalAttributes(fd);
    return fd;
}

#define NUM_POLL_FDS 2
void run(int masterFd) {
    struct pollfd pollfds[NUM_POLL_FDS] = {
        {STDIN_FILENO, POLLIN},
        {masterFd, POLLIN},
    };
    int outputFd[NUM_POLL_FDS] = {masterFd, STDOUT_FILENO};
    char buffer[255];
    int liveFds = NUM_POLL_FDS;
    while (liveFds) {
        int ret = poll(pollfds, NUM_POLL_FDS, -1);
        if (ret == 0)
            continue;
        for (int i = 0; i < NUM_POLL_FDS; i++) {
            if (pollfds[i].revents) {
                ret = 0;
                if (pollfds[i].revents & POLLIN)
                    ret = read(pollfds[i].fd, buffer, sizeof(buffer));
                if (ret == 0) {
                    pollfds[i].fd = ~pollfds[i].fd;
                    liveFds--;
                } else if (ret > 0) {
                    int r = write(outputFd[i], buffer, ret);
                    assert(r == ret);
                } else {
                    perror("failed read");
                }
            }
        }
    }
}

int main (int argc, char *argv[]) {
    int masterFd = createMasterTTY();
    int slaveFd;
    switch (fork()) {
        case -1:
            perror("fork failed");
            return 1;
        case 0:
            slaveFd = createSlaveTTY(masterFd);
            dup2(slaveFd, STDIN_FILENO);
            dup2(slaveFd, STDOUT_FILENO);
            dup2(slaveFd, STDERR_FILENO);
            close(masterFd);
            close(slaveFd);
            execvp(argv[1], argv + 1);
            perror("exec failed");
            return 2;
        default:
            run(masterFd);
    }
    return 0;
}
