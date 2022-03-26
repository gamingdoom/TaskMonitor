// Declare Functions
int TotalCPUUtil();
int getPIDs(int *pids);
int ProcessCPUUtil(int *pids, int *PIDCPU, int pidQty);
int PIDCPUUsage(int pid);
void getPIDName(int pid, char *name);