#include <stdio.h>

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

float procMemUsage(int pid){
    FILE *procpidstat;
    char namebuf[1024]; sprintf(namebuf,"/proc/%d/stat", pid);
    long int rss;

    // Parse /proc/[pid]/stat
    procpidstat = fopen(namebuf, "r");
    // See https://man7.org/linux/man-pages/man5/proc.5.html
    fscanf(procpidstat, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld %*ld %*ld %*ld %*ld %*llu %*lu %ld", &rss);
    fclose(procpidstat);

    printf("%ld\n", rss);
    
    return 0.0;
}