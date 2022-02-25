#include "statistics.h"
#include "cpu.h"
#include <stdio.h>

int main(){
    struct statistics stats;
    // Get total CPU usage into struct
    stats.TotalCPUPercent = TotalCPUUtil();
    printf("CPU Usage = %d%%\n", stats.TotalCPUPercent);
}