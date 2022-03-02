#include "statistics.h"
#include <stdio.h>

int main(){
    struct statistics stats;
    int *pids;
    int *pidcpu;

    // Get per-process CPU usage
    ProcessCPUUtil(pids, pidcpu);
    // Get total CPU usage into struct
    stats.TotalCPUPercent = TotalCPUUtil();
    printf("CPU Usage = %d%%\n", stats.TotalCPUPercent);
}