#include "rsvc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void printUsage(void) {
    printf("Usage: rsvc <command> [service]\n\n");
    printf("Commands:\n");
    printf("  list                   List all enabled services\n");
    printf("  enable  <service>      Enable a service\n");
    printf("  disable <service>      Disable a service\n");
    printf("  start   <service>      Start a service\n");
    printf("  stop    <service>      Stop a service\n");
    printf("  restart <service>      Restart a service\n");
    printf("  status  <service>      Show status of a service (future)\n");
    printf("\n");
    printf("Examples:\n");
    printf("  rsvc list\n");
    printf("  rsvc enable sshd\n");
    printf("  rsvc start nginx\n");
}

static int checkArgs(int argc) {
    if (argc < 3) {
        fprintf(stderr, "Error: service name required.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    const char *cmd = argv[1];

    if (strcmp(cmd, "list") == 0) {
        return rsvcList();        
    } else if (strcmp(cmd, "enable") == 0) {
        checkArgs(argc);
        fprintf(stderr, "NOT IMPLEMENTED, EXITING...\n");
        return 0; // temp
    } else if (strcmp(cmd, "disable") == 0) {
        checkArgs(argc);
        fprintf(stderr, "NOT IMPLEMENTED, EXITING...\n");
        return 0; // temp
    } else if (strcmp(cmd, "start") == 0 || strcmp(cmd, "stop") == 0 || strcmp(cmd, "restart") == 0) {
        checkArgs(argc);
        fprintf(stderr, "%s NOT IMPLEMENTED, EXITING...\n", cmd);
        return 0; // temp
    } else if (strcmp(cmd, "--help") == 0 || strcmp(cmd, "-h") == 0) {
        printUsage();
        return 0;
    } else {
        fprintf(stderr, "Unknown command: %s\n\n", cmd);
        printUsage();
        return 1;
    }
}










