#include <gtk/gtk.h>

struct statistics {
    int TotalCPUPercent;
    int *pids;
    int *pidUsage;
    long double memMB;
    double memGB;
    float *pidMem;
};

struct populateStatsArgs {
    bool repeat;
    GtkWidget *tv;
};

enum {
    PID,
    CPUUSAGE,
    MEMUSED,
    N_COLUMNS
};