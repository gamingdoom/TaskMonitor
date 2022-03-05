#include "cpu.h"
#include "mem.h"

struct statistics {
    int TotalCPUPercent;
    int *pids;
    int *pidUsage;
    long double memMB;
    double memGB;
    float *pidMem;
};