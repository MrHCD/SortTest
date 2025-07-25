#ifndef SORT_DISPLAY_H
#define SORT_DISPLAY_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MY_TYPE_SORT_DISPLAY my_sort_display_get_type()

G_DECLARE_FINAL_TYPE(MySortDisplay, my_sort_display, MY, SORT_DISPLAY, GtkLabel)

struct _MySortDisplay
{
    GtkDrawingArea parent_instance;
    cairo_surface_t *surface; // Surface to draw on
    int width;                // Width of the drawing area
    int height;               // Height of the drawing area
    int* data;
    int num_bars;
};

struct _MySortDisplayClass
{
    GtkDrawingAreaClass parent_class;
};

GtkWidget *my_sort_display_new(void);

cairo_surface_t *my_sort_display_get_surface(MySortDisplay *self);

G_END_DECLS

const int* my_sort_display_get_data(MySortDisplay *self);

void my_sort_display_set_data(MySortDisplay *self, int *data, int num_bars);

// Declare the function prototype for shift_bar_chart_data
void shift_bar_chart_data(int *data, int num_bars);

// Function to update the surface with new content
void my_sort_display_update_surface(gpointer user_data);

#endif  //SORT_DISPLAY_H