#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "cpu.h"


int TotalCPUUtil() {
    FILE *procstat;
    char line[1000];
    char *token;
    char *str, *tofree;
    char valArr[15][32];
    char newValArr[15][32];
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

#include <sys/resource.h>
#include <dirent.h>

struct pidinfo* ProcessCPUUtil(){
    
    struct rusage *procusage;
    struct dirent *files;
    int* NumProc;
    int* pids;
    int i;
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
    tofree = pids = malloc(i*sizeof(int));
    i = 0;
    // Store pids into malloced array
    while ((files = readdir(proc)) != NULL){
        if (atoi(files->d_name) != 0)
        {
            pids[i] = atoi(files->d_name);
            printf("%d\n", pids[i]);
            i++;
        }
    }

    free(tofree);
    return 0;
}