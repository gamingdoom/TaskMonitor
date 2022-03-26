#include "main.h"
#include "cpu.h"
#include "mem.h"
#include <bits/pthreadtypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <pthread.h>
//gtk/gtk.h included in main.h

GtkWidget *tv; // Tree View

void *populateStats(void* repeat){
    //struct populateStatsArgs psa = *(struct populateStatsArgs*)PSA;
    GtkListStore *store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    //GtkWidget *tv = psa.tv;
    struct statistics stats;
    int pidQty = getPIDs(NULL);
    stats.pids = (int*) malloc((pidQty + 128) * sizeof(int));
    stats.pidUsage = (int*) malloc((pidQty + 128) * sizeof(int));
    char *MemUsageStr = malloc(sizeof(char)*32), *CPUUsageStr = malloc(sizeof(char)*32), *PIDStr = malloc(sizeof(char) * 32); 

    pidQty = getPIDs(stats.pids);

    stats.pidName = malloc((pidQty + 128) * sizeof(char*));
    for (int i = 0; i < pidQty; i++) {
        stats.pidName[i] = (char*) malloc(sizeof(char) * 128);
        getPIDName(stats.pids[i], stats.pidName[i]);
    }

    // Get per-process CPU usage into struct
    ProcessCPUUtil(stats.pids, stats.pidUsage, pidQty);
    // Get total CPU usage into struct
    stats.TotalCPUPercent = TotalCPUUtil();
    // Total Memory Usage
    stats.memGB = (stats.memMB = memUsage())/1024.0;
    // Get per-process memory usage
    stats.pidMem = ProcessMemUtil(stats.pids, pidQty);

    snprintf(CPUUsageStr, 256*sizeof(char), "%d %%", stats.TotalCPUPercent);
    snprintf(MemUsageStr, 256*sizeof(char), "%.1f GiB", stats.memGB);

    gtk_list_store_insert_with_values(store, NULL, -1, PID, "Total", NAME, "N/A", CPUUSAGE, CPUUsageStr, MEMUSED, MemUsageStr, -1);

    for (int i = 0; i < pidQty; i++)
    {
        if (i < (pidQty + 128)){
            snprintf(PIDStr, 32*sizeof(char), "%d", stats.pids[i]);
            snprintf(CPUUsageStr, 32*sizeof(char), "%d %%", stats.pidUsage[i]);
            snprintf(MemUsageStr, 32*sizeof(char), "%.1f MiB", stats.pidMem[i]);

            gtk_list_store_insert_with_values(store, NULL, -1, PID, PIDStr, NAME, stats.pidName[i],CPUUSAGE, CPUUsageStr, MEMUSED, MemUsageStr, -1);
        } else {
            printf("Warn: i > pidQty+128\n");
            break;
        }
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(tv), GTK_TREE_MODEL(store));

    free(stats.pidMem);
    free(stats.pids);
    free(stats.pidUsage);
    free(CPUUsageStr);
    free(MemUsageStr);
    free(PIDStr);
    free(stats.pidName);

    g_object_unref(store);

    if ((bool)repeat == true)
        populateStats((void *)true);

    return 0;
}

static void activate (GtkApplication* app, gpointer user_data){
    // Set window items and layout
    GtkWidget *window;
    GtkWidget *sbox;
    GtkCellRenderer *ren;
    pthread_t thread;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Task Monitor");
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

    sbox = gtk_scrolled_window_new();
    gtk_scrolled_window_set_max_content_width(GTK_SCROLLED_WINDOW(sbox), 640);
    gtk_scrolled_window_set_max_content_height(GTK_SCROLLED_WINDOW(sbox), 480);

    gtk_window_set_child(GTK_WINDOW(window), sbox);

    tv = gtk_tree_view_new();
    ren = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tv), -1, "PID", ren, "text", PID, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tv), -1, "Name", ren, "text", NAME, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tv), -1, "CPU %", ren, "text", CPUUSAGE, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tv), -1, "Memory Usage", ren, "text", MEMUSED, NULL);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sbox), tv);

    gtk_widget_show(window);

    populateStats((void*)false);
    pthread_create(&thread, NULL, populateStats, (void*)true);
}

int main(int argc, char *argv[]){
    // GTK Boilerplate
    GtkApplication *app;
    int rtnCode;
    
    app = gtk_application_new("org.taskmonitor.TaskMonitor", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    rtnCode = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref (app);

    return rtnCode;
}