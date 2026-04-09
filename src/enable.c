#include "rsvc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int rsvcEnable(const char *service) {
    if (!serviceExists(service)) {
        fprintf(stderr, "Error: Service '%s' does not exist.\n", service);
        return 1;
    }

    if (isEnabled(service)) {
        printf("Service '%s' is already enabled.\n", service);
        return 0;
    }

    char target[512];
    char linkpath[512];

    snprintf(target, sizeof(target), "%s/%s", SV_DIR, service);
    snprintf(linkpath, sizeof(linkpath), "%s/%s", ENABLED_DIR, service);

    printf("Enabling service: %s ... ", service);
    fflush(stdout);

    if (symlink(target, linkpath) == 0) {
        printf("done\n");
        printf("-> Run 'rsvc start %s' to start it now.\n", service);
        return 0;
    } else {
        perror("failed");
        return 1;
    }
}

int rsvcDisable(const char *service) {
    if (!isEnabled(service)) {
        printf("Service '%s' is not enabled.\n", service);
        return 0;
    }

    char linkpath[512];
    snprintf(linkpath, sizeof(linkpath), "%s/%s", ENABLED_DIR, service);

    printf("Disabling service: %s ... ", service);
    fflush(stdout);

    if (unlink(linkpath) == 0) {
        printf("done\n");
        return 0;
    } else {
        perror("failed");
        return 1;
    }
}
