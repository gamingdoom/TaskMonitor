struct graphData{
    int *dataPointArr;
    int dataPointQty;
    int graphHeight;
};
typedef struct _GtkDrawingArea GtkDrawingArea;
typedef struct _cairo cairo_t;
typedef void *gpointer;
typedef struct _GtkWidget GtkWidget;
struct graphFuncData{
    GtkWidget *CPUButton;
    GtkWidget *MemButton;
    GtkWidget *drawingArea;
} ;
// Define Functions
void drawFunc(GtkDrawingArea* drawing_area, cairo_t* cr, int width, int height, gpointer user_data);
struct graphData newGraphData(int dataPointQty);
struct graphData mergeGraphData(struct graphData *data1, struct graphData *data2);

#define MAX_DATA_POINTS 50