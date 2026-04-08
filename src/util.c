#include "rsvc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>

bool serviceExists(const char *name) {
    if (!name || !*name) return false;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", SV_DIR, name);
   
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

bool isEnabled(const char *name) {
    if (!name || !*name) return false;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", ENABLED_DIR, name);

    struct stat st;
    return lstat(path, &st) == 0 && S_ISLNK(st.st_mode);
}

ServiceState getServiceState(const char *name) {
    if (!name || !*name) return STATE_UNKNOWN;
    
    char stat_path[512];
    snprintf(stat_path, sizeof(stat_path), "%s/%s/supervise/stat", SUPERVISE_DIR, name);

    FILE* f = fopen(stat_path, "r");
    if (!f) return STATE_DOWN;

    char state_str[32] = {0};
    if (fgets(state_str, sizeof(state_str), f)) {
        state_str[strcspn(state_str, "\n")] = '\0';

        if (strcmp(state_str, "run") == 0)
            return STATE_RUN;
        else if (strcmp(state_str, "down") == 0)
            return STATE_DOWN;
        else if (strcmp(state_str, "finish") == 0)
            return STATE_FINISH;
        else if (strstr(state_str, "run") != NULL)
            return STATE_RUN; // in case of extra info
    }
    fclose(f);

    return STATE_DOWN;
}

pid_t getServicePid(const char *name) {
    if (!name || !*name) return 0;
    
    char pid_path[512];
    snprintf(pid_path, sizeof(pid_path), "%s/%s/supervise/pid", SUPERVISE_DIR, name);

    FILE* f = fopen(pid_path, "r");
    if (!f) return 0;

    pid_t pid = 0;
    if (fscanf(f, "%d", &pid) != 1) pid = 0;
   
    fclose(f);

    // Verify PID is still active
    if (pid > 0 && kill(pid, 0) == 0) return pid;

    return 0;
}

time_t getServiceUptime(const char *name) {
    if (!name || !*name) return 0;

    char stat_path[512];
    snprintf(stat_path, sizeof(stat_path), "%s/%s/supervise/stat", SUPERVISE_DIR, name);

    struct stat st;
    if (stat(stat_path, &st) == 0) return time(NULL) - st.st_mtime;

    // Fallback: use supervise directory time
    char supervise_path[512];
    snprintf(supervise_path, sizeof(supervise_path), "%s/%s/supervise", SUPERVISE_DIR, name);
    if (stat(supervise_path, &st) == 0) return time(NULL) - st.st_mtime;

    return 0;
}

