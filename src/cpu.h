struct pidinfo
{
    int pid;
    char name[256];
};


// Declare Functions
int TotalCPUUtil();
int *ProcessCPUUtil();
int PIDCPUUsage(int pid);