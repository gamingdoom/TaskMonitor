#include "main.h"
#include "cpu.h"
#include "mem.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <unistd.h>

GtkWidget *tv; // Tree View
GtkWidget *total; // Initialization label
GtkTreeViewColumn *col1;
GtkTreeViewColumn *col2;
GtkTreeViewColumn *col3;
GtkTreeViewColumn *col4;
bool remTotal = false;
bool First = true;
GtkListStore *store;

static void listStoreAddNRows(int n, GtkTreeModel *model) {
    GtkTreeIter iter;
    for (int i = 0; i < n; i++) {
        //printf("appending row\n");
        gtk_list_store_append(GTK_LIST_STORE(model), &iter);
        //gtk_tree_model_row_inserted(model, gtk_tree_model_get_path(model, &iter), &iter);
    }
    return;
}

static void UpdateView(GtkTreeView *tv, GtkTreeModel *model, struct statistics stats)
{
    gboolean valid;
    GtkTreeIter iter;
    GtkTreeIter curr_iter;
    int rows = 0, rowsneeded = 0;

    valid = gtk_tree_model_get_iter_first(model, &iter);

    if (valid == false)
        listStoreAddNRows(1, model);

    valid = gtk_tree_model_get_iter_first(model, &iter);
    // Add or Remove rows so that the total number of rows is equal to stats.pidQty
    while (valid){
        valid = gtk_tree_model_iter_next(model, &iter);
        rows = gtk_tree_model_iter_n_children(model, NULL);
        rows += rowsneeded;
        if (valid == false && stats.pidQty > rows){
            rowsneeded++;
            valid = true;
        }
        else if (valid == true && stats.pidQty < rows){
           gtk_list_store_remove(store, &iter);
           gtk_tree_model_row_deleted(model, gtk_tree_model_get_path(model, &iter));
        }
    }
    listStoreAddNRows(rowsneeded, model);
    // Update the values of each row
    valid = gtk_tree_model_get_iter_first(model, &iter);
    int iteration = 0;
    while (valid)
    {      
        if (iteration == 0)
            gtk_tree_model_get_iter_first(model, &curr_iter);
        else
            gtk_tree_model_iter_next(model, &curr_iter);

        valid = gtk_tree_model_iter_next(model, &iter);
        char *MemUsedStr = malloc(128*sizeof(char));

        snprintf(MemUsedStr, 128*sizeof(char), "%.1f MiB", stats.pidMem[iteration]);

        gtk_list_store_set(GTK_LIST_STORE(model), &curr_iter,
                           0, stats.pids[iteration],
                           1, stats.pidName[iteration],
                           2, stats.pidUsage[iteration],
                           3, MemUsedStr,
                           -1);
        gtk_tree_model_row_changed(model, gtk_tree_model_get_path(model, &curr_iter), &curr_iter);
        free(MemUsedStr);
        iteration++;
    }
    // Update Column Titles
    char CPUTotalStr[128], MemTotalStr[128], PIDAmmountStr[128]; 
    
    // Make new column Titles
    snprintf(PIDAmmountStr, 128*sizeof(char), "%d\nPID", stats.pidQty);
    snprintf(CPUTotalStr, 128*sizeof(char), "%d %%\nCPU Usage (%%)", stats.TotalCPUPercent);
    snprintf(MemTotalStr, 128*sizeof(char), "%.1f GiB\nMemory Usage", stats.memGB);
    
    // Set column titles
    gtk_tree_view_column_set_title(col1, PIDAmmountStr);
    gtk_tree_view_column_set_title(col3, CPUTotalStr);
    gtk_tree_view_column_set_title(col4, MemTotalStr);
}

gboolean populateStats(void* repeat){
    struct statistics stats;
    int pidQty = getPIDs(NULL);
    stats.pids = (int*) malloc((pidQty + 128) * sizeof(int));
    stats.pidUsage = (int*) malloc((pidQty + 128) * sizeof(int));

    stats.pidQty = getPIDs(stats.pids);

    stats.pidName = malloc((stats.pidQty + 128) * sizeof(char*));
    for (int i = 0; i < stats.pidQty; i++) {
        stats.pidName[i] = (char*) malloc(sizeof(char) * 128);
        getPIDName(stats.pids[i], stats.pidName[i]);
    }

    // Get total CPU usage into struct
    stats.TotalCPUPercent = TotalCPUUtil();
    // Get per-process CPU usage into struct
    GThread *processCPUThread = g_thread_new("procCPUUtil", ProcessCPUUtil, (void*)&stats);
    // Total Memory Usage
    stats.memGB = (stats.memMB = memUsage())/1024.0;
    // Get per-process memory usage
    stats.pidMem = ProcessMemUtil(stats.pids, pidQty);

    // Join procCPUUtil thread
    g_thread_join(processCPUThread);

    // Average total CPU usage (for more accurate total CPU usage)
    stats.TotalCPUPercent = (stats.TotalCPUPercent + TotalCPUUtil()) / 2;

    // Remove label from box if label is in box
    if (remTotal == false) {
        gtk_box_remove(GTK_BOX(gtk_widget_get_parent(total)), total);
        remTotal = true;
    }

    if (First == true){
        store = gtk_list_store_new(N_COLUMNS, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING);
        //initTreeModel(stats);
        gtk_tree_view_set_model(GTK_TREE_VIEW(tv), GTK_TREE_MODEL(store));
        First = false;
    }// else {
        UpdateView(GTK_TREE_VIEW(tv), GTK_TREE_MODEL(gtk_tree_view_get_model(GTK_TREE_VIEW(tv))), stats);
    //}

    free(stats.pidMem);
    free(stats.pids);
    free(stats.pidUsage);
    for (int i = 0; i < stats.pidQty; i++)
        free(stats.pidName[i]);
    free(stats.pidName);

    if ((bool)repeat == true)
        populateStats((void*)repeat);

    return G_SOURCE_CONTINUE;
}
void *doThread(){
    populateStats((void*)true);
    return NULL;
}

static void activate (GtkApplication* app){
    // Set window items and layout
    GtkWidget *window;
    GtkWidget *sbox;
    GtkWidget *totalBox;
    GtkCellRenderer *ren;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Task Monitor");
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 500);

    total = gtk_label_new("Initalizing");
    totalBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(totalBox), total);
    gtk_window_set_child(GTK_WINDOW(window), totalBox);

    sbox = gtk_scrolled_window_new();
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(sbox), 640);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(sbox), 480);

    gtk_box_append(GTK_BOX(totalBox), sbox);
    
    tv = gtk_tree_view_new();
    ren = gtk_cell_renderer_text_new();
    // Create Columns
    col1 = gtk_tree_view_column_new_with_attributes("\nPID", ren, "text", PID, NULL);
    col2 = gtk_tree_view_column_new_with_attributes("Name", ren, "text", NAME, NULL);
    col3 = gtk_tree_view_column_new_with_attributes("\nCPU Usage (%)", ren, "text", CPUUSAGE, NULL);
    col4 = gtk_tree_view_column_new_with_attributes("\nMemory Usage", ren, "text", MEMUSED, NULL);

    // Append Columns
    gtk_tree_view_append_column(GTK_TREE_VIEW(tv), col1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tv), col2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tv), col3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tv), col4);

    // Make Columns Sortable
    gtk_tree_view_column_set_sort_column_id(col1, PID);
    gtk_tree_view_column_set_sort_column_id(col2, NAME);
    gtk_tree_view_column_set_sort_column_id(col3, CPUUSAGE);
    gtk_tree_view_column_set_sort_column_id(col4, MEMUSED);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sbox), tv);

    gtk_box_set_homogeneous(GTK_BOX(totalBox), FALSE);

    gtk_widget_show(window);

    // Start thread
    g_thread_new("thread", doThread, NULL);
    //pthread_create(&thread, NULL, populateStats, (void*)false);
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