// Define threadInfo struct
struct threadInfo{
    int pidQty;
    int *pids;
    int *pidUsage;
};

// Declare Functions
int TotalCPUUtil();
int getPIDs(int *pids);
void *ProcessCPUUtil(void *stats);
int PIDCPUUsage(int pid);
void getPIDName(int pid, char *name);