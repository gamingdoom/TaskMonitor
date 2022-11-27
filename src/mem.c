#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>

float memUsage(){
    FILE *meminfo;
    long double memtotal, memavail;
    float memUsed;

    // Read /proc/meminfo
    meminfo = fopen("/proc/meminfo", "r");
    fscanf(meminfo, "MemTotal: %Lf kB MemFree: %*d kB MemAvailable: %Lf kB", &memtotal, &memavail);
    fclose(meminfo);

    memUsed = (memtotal - memavail)/1024;

    return memUsed;
}

int getTotalMem(){
    FILE *meminfo;
    long double memtotal;

    // Read /proc/meminfo
    meminfo = fopen("/proc/meminfo", "r");
    fscanf(meminfo, "MemTotal: %Lf kB", &memtotal);
    fclose(meminfo);

    return memtotal/1024;
}

float procMemUsage(int pid){
    FILE *procpidstatm;
    char namebuf[1024]; sprintf(namebuf,"/proc/%d/statm", pid);
    float rss;

    // Parse /proc/[pid]/stat
    if(access(namebuf, F_OK) == 0){
        procpidstatm = fopen(namebuf, "r");
        fscanf(procpidstatm, "%*u %f", &rss);
        fclose(procpidstatm);
    }

    // /proc/[pid]/statm has it's data in pages so we need to convert to MiB
    rss = rss * getpagesize() / 1048576;
    
    return rss;
}

float *ProcessMemUtil(int *pids, int pidsLen){
    float *pidsMem = malloc((pidsLen + 128) * sizeof(float));
    
    for (int i = 0; i < pidsLen; i++) {
        pidsMem[i] = procMemUsage(pids[i]);
    }

    return pidsMem;
}