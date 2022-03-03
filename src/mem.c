#include <stdio.h>

float memUsage(){
    FILE *meminfo;
    long memtotal, memavail;

    // Read /proc/meminfo
    meminfo = fopen("/proc/meminfo", "r");
    fscanf(meminfo, "MemTotal: %ld kB MemFree: %*d kB MemAvailable: %ld kB", &memtotal, &memavail);
    fclose(meminfo);

    // Print used memory
    printf("%ld kB of memory used\n", memtotal - memavail);

    return 0.0;
}