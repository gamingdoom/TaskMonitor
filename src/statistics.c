#include "statistics.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    struct statistics stats;
    int *pids = malloc(1*sizeof(int));
    int *pidcpu = malloc(1*sizeof(int));

    // Get per-process CPU usage into struct
    ProcessCPUUtil(pids, pidcpu);
    stats.pids = pids;
    stats.pidUsage = pidcpu;
    // Get total CPU usage into struct
    stats.TotalCPUPercent = TotalCPUUtil();
    printf("CPU Usage = %d%%\n", stats.TotalCPUPercent);
    // Memory Usage
    memUsage();

    free(pids);
    free(pidcpu);

    return 0;
}