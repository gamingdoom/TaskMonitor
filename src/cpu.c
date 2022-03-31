#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/resource.h>
#include <dirent.h>
#include <glib.h>
#include <assert.h>
#include "cpu.h"
#include "main.h"


int TotalCPUUtil() {
    FILE *procstat;
    char line[1000];
    char *token;
    char *str, *tofree;
    char valArr[32][32];
    char newValArr[32][32];
    int i = 0;

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

    // Wait and reset
    i = 0;
    free(tofree);
    // 0.017 second sleep 
    usleep(17000);

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
    else { return -1; } //Error
}

int PIDCPUUsage(int pid){
    FILE *procstat;
    char *str, *tofree, *line, *token;
    char psvalArr[64][128];
    char psnewValArr[64][128];
    char valArr[64][128];
    char newValArr[64][128];
    int i = 0;
    char namebuff[512];

    line = malloc(1024*sizeof(char));

    // Insert PID into /proc/PID/stat
    snprintf(namebuff, 512, "/proc/%d/stat", pid);

    // Read /proc/PID/stat
    procstat = fopen(namebuff, "r");
    if( access(namebuff, F_OK) == 0 ){
        fgets(line, 1024, procstat);
        fclose(procstat);
    }else {return 0;}
    

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
    fgets(line, 1024 * sizeof(char), procstat);
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

    // Wait 0.017 seconds (~1 jiffy)
    usleep(17000);

    // Read /proc/PID/stat
    procstat = fopen(namebuff, "r");
    if( access(namebuff, F_OK) == 0 ){
        fgets(line, 1024, procstat);
        fclose(procstat);
    }else {return 0;}

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
    fgets(line, 1024 * sizeof(char), procstat);
    fclose(procstat);

    // Get new values into array from the first line of /proc/stat
    tofree = str = strdup(line);
    while ((token = strsep(&str, " "))){
        strcpy(psnewValArr[i], token);
        i++;
    }

    free(tofree);
    free(line);
    i = 0;

    // Calculate percentage CPU usage
    int workOld = atoi(valArr[13]) + atoi(valArr[14]);
    // Need to get from proc stat
    int totalOld = atoi(psvalArr[2]) + atoi(psvalArr[3]) + atoi(psvalArr[4]) + atoi(psvalArr[5]) + atoi(psvalArr[6]) + atoi(psvalArr[7]) + atoi(psvalArr[8]) + atoi(psvalArr[9]) + atoi(psvalArr[10]) + atoi(psvalArr[11]);

    int workNew = atoi(newValArr[13]) + atoi(newValArr[14]);
    // Need to get from proc stat
    int totalNew = atoi(psnewValArr[2]) + atoi(psnewValArr[3]) + atoi(psnewValArr[4]) + atoi(psnewValArr[5]) + atoi(psnewValArr[6]) + atoi(psnewValArr[7]) + atoi(psnewValArr[8]) + atoi(psnewValArr[9]) + atoi(psnewValArr[10]) + atoi(psnewValArr[11]);

    float work = workNew - workOld;
    float total = totalNew - totalOld;
    
    if (total > 0.0){
        double CPUPercent = (work/total)*100;
        int rtnCPUPercent = fabs(round(CPUPercent));
        //printf("%d %d%%\n", pid, rtnCPUPercent);
        return rtnCPUPercent;
    }
    else{ return 0; } //No Work Observed or error
}

int getPIDs(int *pids){
    int pidQty = 0;
    struct dirent *files;
    int i = 0;

    // Get amount of PIDs
    DIR *proc = opendir("/proc/");
    while ((files = readdir(proc)) != NULL){
        if (atoi(files->d_name) != 0)
        {
            pidQty++;
        }
    }

    // Reset
    closedir(proc);
    // Check if just trying to get num of pids
    if (pids)
    {
        //pids = malloc(pidQty*sizeof(int));
        files = NULL;
        proc = opendir("/proc/");

        // Store pids into malloced array
        while ((files = readdir(proc)) != NULL){
            if (atoi(files->d_name) != 0)
            {   
                if (i < (pidQty + 128)){
                    pids[i] = atoi(files->d_name);
                    i++;
                }
                else {break;}
            }
        }
        closedir(proc);
    }
    return pidQty;
}

struct threadInfo *getProcessCPUUtil(void *tinfo){
    struct threadInfo *ti = (struct threadInfo *)tinfo;

    for (int i = 0; i < ti->pidQty; i++) {
        assert (i != ti->pidQty);
        ti->pidUsage[i] = PIDCPUUsage(ti->pids[i]);
    }
    return ti;
}

void *ProcessCPUUtil(void *stats){
    struct statistics stat = *(struct statistics *)stats;
    int pidQty = stat.pidQty;
    int *pids = stat.pids;
    int *PIDCPU = stat.pidUsage;

    // Get number of available threads (2 already used by program)
    int threads = sysconf(_SC_NPROCESSORS_ONLN) - 2;
    int x = 0;

        if (threads > 0)
        {
            int **splitPids = malloc(sizeof(int*) * threads);;
            struct threadInfo **threadInfo = malloc(sizeof(struct threadInfo*) * threads);
            GThread **threadArray = malloc(sizeof(GThread*) * threads);
            int offset = 0;
            int i = 0;

            for (x = 0; x < threads; x++) {
                // Split pids by the number of threads
                // Last thread gets the remainder + the normal amount
                if (x == threads - 1)
                    splitPids[x] = malloc(sizeof(int) * ((pidQty - pidQty % threads) / threads) + pidQty % threads);
                else
                    splitPids[x] = malloc(sizeof(int) * ((pidQty - pidQty % threads) / threads));
                // Last thread gets the remainder + the normal amount
                if (x == threads - 1)
                    memcpy(splitPids[x], &pids[(pidQty - pidQty % threads) / threads * x], sizeof(int) * ((pidQty - pidQty % threads) / threads) + pidQty % threads);
                else
                    memcpy(splitPids[x], &pids[(pidQty - pidQty % threads) / threads * x], sizeof(int) * ((pidQty - pidQty % threads) / threads));
                

                // Create array threadinfo structs
                threadInfo[x] = malloc(sizeof(struct threadInfo*));

                if (x == threads - 1)
                    threadInfo[x]->pidQty = ((pidQty - pidQty % threads) / threads) + pidQty % threads;
                else
                    threadInfo[x]->pidQty = (pidQty - pidQty % threads) / threads;

                threadInfo[x]->pids = malloc(sizeof(int) * threadInfo[x]->pidQty);
                threadInfo[x]->pidUsage = malloc(threadInfo[x]->pidQty * sizeof(int));
                memcpy(threadInfo[x]->pids, splitPids[x], sizeof(int) * threadInfo[x]->pidQty);


                // Create thread
                threadArray[x] = g_thread_new("procCPUUtilWorkerThread", (GThreadFunc)getProcessCPUUtil, threadInfo[x]);
            }

            // Join threads
            for (x = 0; x < threads; x++) {
                threadInfo[x] = g_thread_join(threadArray[x]);
                assert(threadInfo[x] != NULL);
            }

            // Merge results
            for (x = 0; x < threads; x++) {
                for (i = 0; i < threadInfo[x]->pidQty; i++) {
                    stat.pidUsage[offset + i] = threadInfo[x]->pidUsage[i];
                }
                offset += i;
            }

            // Free memory
            for (x = 0; x < threads; x++) {
                free(threadInfo[x]->pids);
                free(threadInfo[x]->pidUsage);
                free(threadInfo[x]);
                free(splitPids[x]);
            }

            free(threadArray);
            free(threadInfo);
            free(splitPids);
        }
        else
        {
            struct threadInfo *tinfo = malloc(sizeof(struct threadInfo*));
            tinfo->pidQty = pidQty;
            tinfo->pids = pids;
            tinfo->pidUsage = PIDCPU;
            getProcessCPUUtil(tinfo);
            PIDCPU = tinfo->pidUsage;
            free(tinfo);
        }
    return 0;
}

void getPIDName(int pid, char *name){
    char *line = malloc(1024*sizeof(char));
    char *token;
    char *toFree;
    FILE *procstat;

    // Open /proc/PID/stat
    char namebuff[1024];
    sprintf(namebuff, "/proc/%d/stat", pid);
    procstat = fopen(namebuff, "r");
    if ( access(namebuff, F_OK) == 0 ){
        fgets(line, 1024, procstat);
        fclose(procstat);
    }
    else 
    {
        return;
    }

    // Get name from the first line of /proc/PID/stat
    toFree = line;
    while ((token = strsep(&line, " "))){
        if (strncmp(token, "(", 1) == 0){
            strcpy(name, token);
            break;
        }
    }

    free(toFree);

    // Remove the parenthesis
    if (name[strlen(name) - 1] == ')' && name[0] == '('){
        name[strlen(name) - 1] = '\0';
        memmove(name, name+1, sizeof(char) * strlen(name));
    }
    
    return;
}