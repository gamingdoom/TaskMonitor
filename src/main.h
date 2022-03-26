#include <gtk/gtk.h>

struct statistics {
    int TotalCPUPercent;
    int *pids;
    int *pidUsage;
    long double memMB;
    double memGB;
    float *pidMem;
    char **pidName;
};

enum {
    PID,
    NAME,
    CPUUSAGE,
    MEMUSED,
    N_COLUMNS
};