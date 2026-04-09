#include "rsvc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *name;
    Command     cmd;
} CommandEntry;

static const CommandEntry commands[] = {
    { "list",    CMD_LIST    },
    { "start",   CMD_START   },
    { "stop",    CMD_STOP    },
    { "restart", CMD_RESTART },
    { "enable",  CMD_ENABLE  },
    { "disable", CMD_DISABLE },
    { "help",    CMD_HELP    },
    { "--help",  CMD_HELP    },
    { "-h",      CMD_HELP    },
    { NULL,      CMD_UNKNOWN } // end marker 
};

static Command getCommand(const char *arg) {
    if (!arg) return CMD_UNKNOWN;

    for (const CommandEntry *c = commands; c->name != NULL; ++c) {
        if (strcmp(arg, c->name) == 0) return c->cmd;
    }
    return CMD_UNKNOWN;
}

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
    
    Command cmd = getCommand(argv[1]);

    switch (cmd) {
        case CMD_LIST:
            return rsvcList();

        case CMD_START:
            checkArgs(argc);
            return rsvcStart(argv[2]);

        case CMD_STOP:
            checkArgs(argc);
            return rsvcStop(argv[2]);
        
        case CMD_RESTART:
            checkArgs(argc);
            return rsvcRestart(argv[2]);

        case CMD_ENABLE:
            checkArgs(argc);
            return rsvcEnable(argv[2]);

        case CMD_DISABLE:
            checkArgs(argc);
            return rsvcDisable(argv[2]);

        case CMD_HELP:
            printUsage();
            return 0;

        case CMD_UNKNOWN:
        default:
            fprintf(stderr, "Unknown command: %s\n\n", argv[1]);
            printUsage();
            return 1;
    }
}










