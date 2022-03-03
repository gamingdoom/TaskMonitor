struct pidinfo
{
    int pid;
    char name[256];
};


// Declare Functions
int TotalCPUUtil();
int getPIDs(int *pids);
void ProcessCPUUtil(int *pids, int *PIDCPU);
int PIDCPUUsage(int pid);