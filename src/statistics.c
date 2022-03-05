#include "statistics.h"
#include "cpu.h"
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    struct statistics stats;
    int *pids = malloc(1*sizeof(int));
    int *pidcpu = malloc(1*sizeof(int));

    // Get per-process CPU usage into struct
    int pidQty = ProcessCPUUtil(pids, pidcpu);
    stats.pids = pids;
    stats.pidUsage = pidcpu;
    // Get total CPU usage into struct
    stats.TotalCPUPercent = TotalCPUUtil();
    // Total Memory Usage
    stats.memGB = (stats.memMB = memUsage())/1024.0;
    // Get per-process memory usage
    stats.pidMem = ProcessMemUtil(stats.pids, pidQty);

    printf("CPU Usage = %d%%\nMemory Usage (GB) = %.1fGB\n", stats.TotalCPUPercent, stats.memGB);

    free(pids);
    free(pidcpu);

    return 0;
}