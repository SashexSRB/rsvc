#include "rsvc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

static int runSvCommand(const char *action, const char *service) {
    if (!serviceExists(service)) {
        fprintf(stderr, "Error: Service '%s' does not exist in %s\n", service, SV_DIR);
        return 1;
    }

    printf("%s service : %s ... ", action, service);
    fflush(stdout);

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execlp("sv", "sv", action, service, NULL);

        // If we got here, exec fucked up
        perror("execlp sv failed");
        exit(1);
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("done\n");
            return 0;
        } else {
            printf("failed\n");
            return 1;
        }
    } else {
        perror("fork");
        return 1;
    }
}

int rsvcStart(const char *service) {
    return runSvCommand("start", service);
}

int rsvcStop(const char *service) {
    return runSvCommand("stop", service);
}

int rsvcRestart(const char *service) {
    return runSvCommand("restart", service);
}

