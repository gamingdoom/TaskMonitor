#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "cpu.h"


int TotalCPUUtil() {
    FILE *procstat;
    char line[1000];
    char *token;
    char *str, *tofree;
    char valArr[32][32];
    char newValArr[32][32];
    int i = 0;
    float usage;

    // Read /proc/stat
    procstat = fopen("/proc/stat", "r");
    fgets(line, sizeof line, procstat);
    fclose(procstat);

    // Get old values into array from the first line of /proc/stat
    tofree = str = strdup(line);
    while ((token = strsep(&str, " "))){
        strcpy(valArr[i], token);
        i++;
    }

    // Wait 10ms and reset
    i = 0;
    free(tofree);
    // 0.1 seconds
    usleep(100000);

    // Read /proc/stat
    procstat = fopen("/proc/stat", "r");
    fgets(line, sizeof line, procstat);
    fclose(procstat);

    // Get new  values into array from the first line of /proc/stat
    tofree = str = strdup(line);
    while ((token = strsep(&str, " "))){
        strcpy(newValArr[i], token);
        i++;
    }
    free(tofree);
    
    // Calculate percentage CPU usage
    int workOld = atoi(valArr[2]) + atoi(valArr[3]) + atoi(valArr[4]);
    int totalOld = atoi(valArr[2]) + atoi(valArr[3]) + atoi(valArr[4]) + atoi(valArr[5]) + atoi(valArr[6]) + atoi(valArr[7]) + atoi(valArr[8]) + atoi(valArr[9]) + atoi(valArr[10]) + atoi(valArr[11]);

    int workNew = atoi(newValArr[2]) + atoi(newValArr[3]) + atoi(newValArr[4]);
    int totalNew = atoi(newValArr[2]) + atoi(newValArr[3]) + atoi(newValArr[4]) + atoi(newValArr[5]) + atoi(newValArr[6]) + atoi(newValArr[7]) + atoi(newValArr[8]) + atoi(newValArr[9]) + atoi(newValArr[10]) + atoi(newValArr[11]);

    float work = workNew - workOld;
    float total = totalNew - totalOld;
    
    if (total != 0){
        double CPUPercent = (work/total)*100;
        int rtnCPUPercent = round(CPUPercent);
        return rtnCPUPercent;
    }
    else{ return -1; } //Error
}

int PIDCPUUsage(int pid){
    FILE *procstat;
    char *line;
    char *token;
    char *str, *tofree;
    char psvalArr[32][64];
    char psnewValArr[32][64];
    char valArr[32][64];
    char newValArr[32][64];
    int i = 0;
    float usage;
    char namebuff[512];

    line = malloc(1024*sizeof(char));

    // Insert PID into /proc/PID/stat
    snprintf(namebuff, 512, "/proc/%d/stat", pid);


    // Read /proc/PID/stat
    procstat = fopen(namebuff, "r");
    if( access(namebuff, F_OK) == 0 ){
        fgets(line, 1024, procstat);
        fclose(procstat);
    }
    

    // Get old values into array from the first line of /proc/PID/stat
    tofree = str = strdup(line);
    while ((token = strsep(&str, " "))){
        strcpy(valArr[i], token);
        i++;
    }

    // Reset
    i = 0;
    free(tofree);
    free(line);
    line = malloc(1024*sizeof(char));

    // Read /proc/stat
    procstat = fopen("/proc/stat", "r");
    fgets(line, sizeof line, procstat);
    fclose(procstat);

    // Get old values into array from the first line of /proc/stat
    tofree = str = strdup(line);
    while ((token = strsep(&str, " "))){
        strcpy(psvalArr[i], token);
        i++;
    }

    // Reset
    i = 0;
    free(tofree);
    free(line);
    line = malloc(1024*sizeof(char));

    // Wait 0.05 seconds
    usleep(50000);

    // Read /proc/PID/stat
    procstat = fopen(namebuff, "r");
    if( access(namebuff, F_OK) == 0 ){
        fgets(line, 1024, procstat);
        fclose(procstat);
    }

    // Get new  values into array from the first line of /proc/PID/stat
    tofree = str = strdup(line);
    while ((token = strsep(&str, " "))){
        strcpy(newValArr[i], token);
        i++;
    }

    // Reset
    i = 0;
    free(tofree);
    free(line);
    line = malloc(1024*sizeof(char));

    // Read /proc/stat
    procstat = fopen("/proc/stat", "r");
    fgets(line, sizeof line, procstat);
    fclose(procstat);

    // Get old values into array from the first line of /proc/stat
    tofree = str = strdup(line);
    while ((token = strsep(&str, " "))){
        strcpy(psnewValArr[i], token);
        i++;
    }

    //printf("14: %d 15: %d new14: %d new15: %d\n", atoi(valArr[13]), atoi(valArr[14]),atoi(newValArr[13]) ,atoi(newValArr[14]));

    free(tofree);
    free(line);
    i = 0;

    // Calculate percentage CPU usage
    int workOld = atoi(valArr[13]) + atoi(valArr[14]);
    // Need to get from proc stat
    int totalOld = atoi(psvalArr[2]) + atoi(psvalArr[3]) + atoi(psvalArr[4]) + atoi(psvalArr[5]) + atoi(psvalArr[6]) + atoi(psvalArr[7]) + atoi(psvalArr[8]) + atoi(psvalArr[9]) + atoi(psvalArr[10]) + atoi(psvalArr[11]);

    int workNew = atoi(newValArr[13]) + atoi(newValArr[14]);
    // Need to get from proc stat
    int totalNew = atoi(newValArr[2]) + atoi(psnewValArr[3]) + atoi(psnewValArr[4]) + atoi(psnewValArr[5]) + atoi(psnewValArr[6]) + atoi(psnewValArr[7]) + atoi(psnewValArr[8]) + atoi(psnewValArr[9]) + atoi(psnewValArr[10]) + atoi(psnewValArr[11]);

    float work = workNew - workOld;
    float total = totalNew - totalOld;
    
    if (total != 0){
        double CPUPercent = (work/total)*100;
        int rtnCPUPercent = fabs(round(CPUPercent));
        printf("%d %d%%\n", pid, rtnCPUPercent);
        return rtnCPUPercent;
    }
    else{ return -1; } //Error
}

#include <sys/resource.h>
#include <dirent.h>

int *ProcessCPUUtil(){
    
    struct rusage *procusage;
    struct dirent *files;
    int* NumProc;
    int* pids;
    int i = 0;
    int* tofree;

    // List files in /proc and put the pids in pid array
    DIR *proc = opendir("/proc/");
    // Get num of processes for malloc
    while ((files = readdir(proc)) != NULL){
        if (atoi(files->d_name) != 0)
        {
            i++;
        }
    }

    // Reset
    closedir(proc);
    tofree = pids = malloc(i*sizeof(int));
    files = NULL;
    i = 0;
    proc = opendir("/proc/");

    // Store pids into malloced array
    while ((files = readdir(proc)) != NULL){
        if (atoi(files->d_name) != 0)
        {
            pids[i] = atoi(files->d_name);
            i++;
        }
    }

    // Iterate over every PID to get it's CPU usage
    int j;
    int *PIDCPU;

    tofree = PIDCPU = malloc(i*sizeof(int));
    for (j=0; j < i; j++) {
        PIDCPU[j] = PIDCPUUsage(pids[j]);
    }

    free(pids);
    free(tofree);
    return PIDCPU;
}
