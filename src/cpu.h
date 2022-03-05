struct pidinfo
{
    int pid;
    char name[256];
};


// Declare Functions
int TotalCPUUtil();
int getPIDs(int *pids);
int ProcessCPUUtil(int *pids, int *PIDCPU);
int PIDCPUUsage(int pid);