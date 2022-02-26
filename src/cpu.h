struct pidinfo
{
    int pid;
    char name[256];
};


// Declare Functions
int TotalCPUUtil();
struct pidinfo* ProcessCPUUtil();