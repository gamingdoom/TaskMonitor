struct pidinfo
{
    int pid;
    char name[256];
};


// Declare Functions
int TotalCPUUtil();
void ProcessCPUUtil(int *pids, int *PIDCPU);
int PIDCPUUsage(int pid);