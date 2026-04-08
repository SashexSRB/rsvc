#pragma once

#include <stdbool.h>
#include <sys/types.h>
#include <time.h>

#define SV_DIR		    "/etc/runit/sv"
#define ENABLED_DIR	    "/etc/runit/runsvdir/default"
#define SUPERVISE_DIR	"/run/runit/service"

typedef enum {
    STATE_DOWN,
    STATE_RUN,
    STATE_FINISH,
    STATE_UNKNOWN
} ServiceState;

typedef struct {
    char         name[256];
    ServiceState state;
    bool         enabled;
    pid_t        pid;
    time_t       uptime;
} ServiceInfo;

// Main functions
int rsvcList(void);
int rsvcEnable(const char *name);
int rsvcDisable(const char *name);
int rsvcStart(const char *name);
int rsvcStop(const char *name);
int rsvcRestart(const char *name);

// Internal helpers

/**
 * Checks if service folder exists in /etc/runit/sv/
 */
bool serviceExists(const char *name);

/**
 * Checks for symlink in /etc/runit/runsvdir/default/
 */
bool isEnabled(const char *name);

/**
 * Determines DOWN/RUN/FINISH
 */
ServiceState getServiceState(const char *name);

/**
 * Reads PID and validates it
 */
pid_t getServicePid(const char *name);

/**
 * Calculats how long the service has been running
 */
time_t getServiceUptime(const char *name);

/**
 * Prints the service list table header
 */
void printTableHeader(void);

/**
 * Prints a row for the specified service.
 */ 
void printServiceRow(const ServiceInfo *svc);

