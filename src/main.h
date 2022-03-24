struct statistics {
    int TotalCPUPercent;
    int *pids;
    int *pidUsage;
    long double memMB;
    double memGB;
    float *pidMem;
};

enum {
    PID,
    CPUUSAGE,
    MEMUSED,
    N_COLUMNS
};