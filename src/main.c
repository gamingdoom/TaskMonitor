#include "main.h"
#include "cpu.h"
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>

int main(int argc, char *argv[]){
    GtkWidget *window;
    GtkBuilder *builder = NULL;

    gtk_init();

    builder = gtk_builder_new();

    if( gtk_builder_add_from_file (builder,"gui/TaskMonitor.glade" , NULL) == 0)
    {
        printf("gtk_builder_add_from_file FAILED\n");
        return 1;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder,"Task Monitor"));

    gtk_widget_show(window);

    struct statistics stats;
    int *pids = malloc(1*sizeof(int));
    int *pidcpu = malloc(1*sizeof(int));

    // Get per-process CPU usage into struct
    int pidQty = ProcessCPUUtil(pids, pidcpu);
    stats.pids = pids;
    stats.pidUsage = pidcpu;
    // Get total CPU usage into struct
    stats.TotalCPUPercent = TotalCPUUtil();
    // Total Memory Usage
    stats.memGB = (stats.memMB = memUsage())/1024.0;
    // Get per-process memory usage
    stats.pidMem = ProcessMemUtil(stats.pids, pidQty);

    printf("CPU Usage = %d%%\nMemory Usage (GB) = %.1fGB\n", stats.TotalCPUPercent, stats.memGB);

    free(pids);
    free(pidcpu);

    return 0;
}