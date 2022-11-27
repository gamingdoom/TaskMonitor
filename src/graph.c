#include "graph.h"
#include "main.h"
#include "cairo.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>

void drawFunc(GtkDrawingArea* drawing_area, cairo_t* cr, int width, int height, gpointer user_data){
    float w = height;
    float h = width;
    struct graphData data = *(struct graphData*)user_data;

    float x = 0 , y = 0;

    GdkRGBA light_blue, light_blue_transparent;
    gdk_rgba_parse(&light_blue, "0,120,215");
    gdk_rgba_parse(&light_blue_transparent, "0,120,215,0.5");

    // Connect points for each data point
    gdk_cairo_set_source_rgba(cr, &light_blue);
    cairo_set_line_width(cr, 5);
    for (int i = 0; i < data.dataPointQty; i++){
        x = (w/MAX_DATA_POINTS)*i;
        y = (h/data.graphHeight)*data.dataPointArr[i];
        //cairo_move_to(cr, x, y);
        cairo_line_to(cr, x, y);
        cairo_stroke(cr);
    }

    return;
}

struct graphData newGraphData(int dataPointQty){
    struct graphData data;
    dataPointQty = (dataPointQty > MAX_DATA_POINTS) ? MAX_DATA_POINTS : dataPointQty;
    data.dataPointQty = dataPointQty;
    data.dataPointArr = malloc(MAX_DATA_POINTS * sizeof(int));
    return data;
}
// Data 1 should be the least recent data
struct graphData mergeGraphData(struct graphData *data1, struct graphData *data2){
    if (data1 == NULL)
        return *data2;
    if (data2 == NULL)
        return *data1;

    struct graphData data;
    if (data1->dataPointQty + data2->dataPointQty > MAX_DATA_POINTS){
        data.dataPointQty = MAX_DATA_POINTS;
        data.dataPointArr = malloc(MAX_DATA_POINTS * sizeof(int));
        int i = MAX_DATA_POINTS;
        for (i = MAX_DATA_POINTS; i > 0; i--){
            data.dataPointArr[i] = data2->dataPointArr[i];
        }
        for (; i > 0; i--){
            data.dataPointArr[i] = data1->dataPointArr[i];
        }
        return data;
    }
    else {
        data.dataPointQty = data1->dataPointQty + data2->dataPointQty;
        data.dataPointArr = malloc(data.dataPointQty * sizeof(int));
        for (int i = 0; i < data1->dataPointQty; i++){
            data.dataPointArr[i] = data1->dataPointArr[i];
        }
        for (int i = 0; i < data2->dataPointQty; i++){
            data.dataPointArr[i + data1->dataPointQty] = data2->dataPointArr[i];
        }
        return data;
    }

}