#include "rsvc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

static int compareServices(const void* a, const void* b) {
    const ServiceInfo* sa = (const ServiceInfo*)a;
    const ServiceInfo* sb = (const ServiceInfo*)b;
    return strcasecmp(sa->name, sb->name);
}

static const char* stateToStr(ServiceState state) {
    switch (state) {
        case STATE_RUN:     return "running";
        case STATE_DOWN:    return "down";
        case STATE_FINISH:  return "finishing";
        default:            return "unknown";
    }
}

static const char* enabledToStr(bool enabled) {
    return enabled ? "yes" : "no";
}

void printTableHeader(void) {
    printf("%-20s %-10s %-10s %-8s %-8s\n",
           "SERVICE", "STATE", "ENABLED", "PID", "TIME");
    
    printf("%-20s %-10s %-10s %-8s %-8s\n",
           "--------------------", "----------", "----------",
           "--------", "--------");
}

void printServiceRow(const ServiceInfo* svc) {
    char time_str[32] = "-";
    if (svc->uptime > 0) {
        if (svc->uptime < 60)
            snprintf(time_str, sizeof(time_str), "%lds", svc->uptime);
        else if (svc->uptime < 3600)
            snprintf(time_str, sizeof(time_str), "%ldm", svc->uptime / 60);
        else if (svc->uptime < 86400)
            snprintf(time_str, sizeof(time_str), "%ldh", svc->uptime / 3600);
        else
            snprintf(time_str, sizeof(time_str), "%ldd", svc->uptime / 86400);
    }

    printf("%-20s %-10s %-10s %-8d %-8s\n",
           svc->name,
           stateToStr(svc->state),
           enabledToStr(svc->enabled),
           svc->pid,
           time_str
    );
}

int rsvcList(void) {
    DIR* dir = opendir(ENABLED_DIR);
    if (!dir) {
        perror("Cannot open enabled services directory");
        return 1;
    }

    ServiceInfo* services = NULL;
    size_t count = 0;
    size_t capacity = 16; // Initial
    
    services = malloc(capacity * sizeof(ServiceInfo));
    if (!services) {
        closedir(dir);
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    } 

    struct dirent* entry; 
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        // Resize array if needed
        if (count >= capacity) {
            capacity *= 2;
            ServiceInfo* temp = realloc(services, capacity * sizeof(ServiceInfo));
            if (!temp) {
                free(services);
                closedir(dir);
                fprintf(stderr, "Memory reallocation failed.\n");
                return 1;
            }
            services = temp;
        }
    
        ServiceInfo* svc = &services[count];

        strncpy(svc->name, entry->d_name, sizeof(svc->name) - 1);
        svc->name[sizeof(svc->name)-1] = '\0';

        svc->enabled = true;
        svc->state   = getServiceState(svc->name);
        svc->pid     = getServicePid(svc->name);
        svc->uptime  = getServiceUptime(svc->name);

        count++;
    }

    closedir(dir);

    // Sort alphabeticaly by service name
    qsort(services, count, sizeof(ServiceInfo), compareServices);

    // Print table
    printTableHeader();
    for (size_t i = 0; i < count; i++) {
        printServiceRow(&services[i]);
    }

    free(services);
    return 0;
    return 0;
}
