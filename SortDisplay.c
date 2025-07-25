#include "SortDisplay.h"

G_DEFINE_TYPE(MySortDisplay, my_sort_display, GTK_TYPE_DRAWING_AREA)

static void my_sort_display_size_allocate(GtkWidget *widget, GtkAllocation *allocation);

static gboolean my_sort_display_configure_event(GtkWidget *widget, GdkEventConfigure *event);

static void my_sort_display_realize(GtkWidget *widget);

static gboolean my_sort_display_draw(GtkWidget *widget, cairo_t *cr);

static void my_sort_display_class_init(MySortDisplayClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    // Connect the draw signal
    widget_class->draw = my_sort_display_draw;

    // Connect the configure event
    widget_class->configure_event = my_sort_display_configure_event;

    // Connect the size allocate signal
    widget_class->size_allocate = my_sort_display_size_allocate;
    // Set the realize function
    widget_class->realize = my_sort_display_realize;
}

static void my_sort_display_init(MySortDisplay *self)
{
    // Initialize the surface to NULL
    self->surface = NULL;
    self->width = 0;
    self->height = 0;
    self->data = NULL;
    self->num_bars = 0;

    // Set the widget to be app paintable
    gtk_widget_set_app_paintable(GTK_WIDGET(self), TRUE);
}

GtkWidget *my_sort_display_new(void)
{
    return GTK_WIDGET(g_object_new(MY_TYPE_SORT_DISPLAY, NULL));
}

cairo_surface_t *my_sort_display_get_surface(MySortDisplay *self)
{
    if (self)
    {
        // Return the surface if it exists
        if (self->surface)
            return self->surface;

        // If the surface is NULL, create a new one with default size
        self->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 600);
        return self->surface;
    }
    return NULL;
}

const int *my_sort_display_get_data(MySortDisplay *self)
{
    return self->data;
}

void my_sort_display_set_data(MySortDisplay *self, int *data, int num_bars)
{
    self->data = data;
    self->num_bars = num_bars;
}

// Define the size allocate function
static void my_sort_display_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
    MySortDisplay *self = MY_SORT_DISPLAY(widget);

    // Update the widget's allocation
    gtk_widget_set_allocation(widget, allocation);

    // Update the width and height
    self->width = allocation->width;
    self->height = allocation->height;

    // Ensure the widget has a valid GdkWindow
    if (gtk_widget_get_window(widget) == NULL)
        return;

    // Free the old surface if it exists
    if (self->surface)
        cairo_surface_destroy(self->surface);

    // Create a new surface for the widget
    self->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                               self->width,
                                               self->height);
}

// Define the configure event handler
static gboolean my_sort_display_configure_event(GtkWidget *widget, GdkEventConfigure *event)
{
    MySortDisplay *self = MY_SORT_DISPLAY(widget);

    // Free the old surface if it exists
    if (self->surface)
        cairo_surface_destroy(self->surface);

    // Create a new surface with the updated dimensions
    // Create a new surface for the widget
    self->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                               event->width,
                                               event->height);

    self->width = event->width;
    self->height = event->height;

    return TRUE;
}

// Define the realize function
static void my_sort_display_realize(GtkWidget *widget)
{
    MySortDisplay *self = MY_SORT_DISPLAY(widget);

    // Chain up to the parent class's realize function
    GTK_WIDGET_CLASS(my_sort_display_parent_class)->realize(widget);
}

// Define the draw function
static gboolean my_sort_display_draw(GtkWidget *widget, cairo_t *cr)
{
    MySortDisplay *self = MY_SORT_DISPLAY(widget);

    if (self->surface == NULL)
        return FALSE;

    // Paint the surface onto the widget
    if (self->surface)
    {
        // Set the source surface to the drawing context
        cairo_set_source_surface(cr, self->surface, 0, 0);
        cairo_paint(cr);
    }

    return TRUE;
}

// Function to shift bar chart data
void shift_bar_chart_data(int *data, int num_bars)
{
    if (num_bars > 1)
    {
        int temp = data[num_bars - 1];
        for (int i = num_bars - 1; i > 0; i--)
        {
            data[i] = data[i - 1];
            data[i - 1] = temp;
        }
    }
}

// Function to update the surface with new content
void my_sort_display_update_surface(gpointer user_data)
{
    MySortDisplay *self = MY_SORT_DISPLAY(user_data);

    if (self && self->surface)
    {
        const int *data = my_sort_display_get_data(self);
        int num_bars = self->num_bars;

        cairo_t *surface_cr = cairo_create(self->surface);

        cairo_set_source_rgb(surface_cr, 0.0, 0.0, 0.0); // Set background color to black
        cairo_paint(surface_cr);
#if 1
        GtkAllocation allocation;
        gtk_widget_get_allocation(GTK_WIDGET(self), &allocation);
        int widget_width = allocation.width;
        int widget_height = allocation.height;

        int bar_width = widget_width / 20;
        int bar_spacing = bar_width / 4;

        int x = bar_spacing;
        for (int i = 0; i < num_bars; i++)
        {
            int bar_height = (data[i] * widget_height) / 100;

            cairo_set_source_rgb(surface_cr, 0.2, 0.6, 0.8);
            cairo_rectangle(surface_cr, x, widget_height - bar_height, bar_width, bar_height);
            cairo_fill(surface_cr);

            x += bar_width + bar_spacing;
        }
#endif
        cairo_destroy(surface_cr);
        gtk_widget_queue_draw(GTK_WIDGET(self));
    }
}