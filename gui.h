// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Code for GUI interface.
// See included License file for license

#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "queue.h"
#include "message_queue.h"
#include "python_module.h"
#include "common.h"
#include "compute_thread.h"

typedef struct{
  // GTK Widgets
  GtkWidget *window;
  // Text
  GtkTextView *message_textview;
  GtkTextBuffer *message_textbuffer;
  GtkTextBuffer *argument_textbuffer;
  // Buttons
  GtkButton *execute_button;
  GtkButton *interrupt_button;
  GtkButton *output_clear_button;
  GtkButton *output_copy_button;
  GtkButton *save_svg_button;
  GtkButton *positive_x_axis_button;
  GtkButton *negative_x_axis_button;
  GtkButton *positive_y_axis_button;
  GtkButton *negative_y_axis_button;
  GtkButton *positive_z_axis_button;
  GtkButton *negative_z_axis_button;
  GtkRadioButton *orthographic_button;
  GtkRadioButton *perspective_button;
  // Draw Area
  GtkDrawingArea *drawarea;
  int drawarea_width;
  int drawarea_height;
  // Axis Draw Area
  GtkDrawingArea *axis_draw_area;
  // Message Queue
  MessageQueue *message_queue;
  // Python Module Information
  PythonInfo *py_info;
  int executing;
  // Program Strings 
  char *file_path;
  char *directory_path;
  // ComputeThread
  ComputeThreadInfo *compute_info;
  // Queues
  Queue *to_python;
  Queue *from_python;
  Queue *to_compute;
  Queue *from_compute;
  // Projection State
  int projection_state; // 0 if orthogonal, 1 if perspective
  // Draw Properties
  float *projection;
  float *view_angle;
  float *view_trans;
  Object **objects;
  size_t objects_size;
  cairo_surface_t *canvas;
} AppInfo;

// AppInfo Methods
AppInfo *AppInfoSetup(void);
void AppInfoCleanup(AppInfo *info);
void AppInfo_AddDraw(AppInfo *info);

// GUI Setup
void gtk_setup(int argc, char **argv, AppInfo *info);

// Common Functions
void gtk_message_printf(GtkTextBuffer *text_buffer, gchar *message);
float degree_from_radian(float radian);
float radian_from_degree(float degree);

// Main Window
void on_window_main_destroy(void);

//// Widget Signals 
// Button Functions
void execute_button_function(GtkButton *widget, gpointer g_data);
void interrupt_button_function(GtkButton *widget, gpointer g_data);
void program_output_clear_button_function(GtkButton *widget, gpointer g_data);
void program_output_copy_button_function(GtkButton *widget, gpointer g_data);
void save_svg_button_function(GtkButton *widget, gpointer g_data);
void positive_x_axis_button_function(GtkButton *widget, gpointer g_data);
void negative_x_axis_button_function(GtkButton *widget, gpointer g_data);
void positive_y_axis_button_function(GtkButton *widget, gpointer g_data);
void negative_y_axis_button_function(GtkButton *widget, gpointer g_data);
void positive_z_axis_button_function(GtkButton *widget, gpointer g_data);
void negative_z_axis_button_function(GtkButton *widget, gpointer g_data);
void orthographic_button_function(GtkButton *widget, gpointer g_data);
void perspective_button_function(GtkButton *widget, gpointer g_data);
// File Choosers 
void python_file_chooser_file_set(GtkWidget *widget, gpointer g_data);
void python_working_directory_chooser_file_set(GtkWidget *widget, gpointer g_data);
// Draw Area Functions
gboolean draw_area_draw_function(GtkWidget *widget, cairo_t *cr, gpointer g_data);
void draw_area_size_allocation_function(GtkWidget *widget, GdkRectangle *allocation, gpointer g_data);
gboolean drawarea_key_press_function(GtkWidget *widget, GdkEvent *event, gpointer g_data);
void draw_area_button_press_function(GtkWidget *widget, GdkEvent *event, gpointer g_data);
// Axis Draw Area
void axis_draw_area_draw_function(GtkWidget *widget, cairo_t *cr, gpointer g_data);
// Textbuffer functions
void BufferOnlySpaces(GtkTextBuffer *textbuffer, gpointer user_data);
void BufferNumberOnly(GtkTextBuffer *textbuffer, gpointer user_data);
float retrieve_float_from_textbuffer(GtkTextBuffer *text_buffer);
void set_textbuffer_from_float(GtkTextBuffer *text_buffer, float value);
// Timeout Function
gboolean timeout_function(gpointer user_data);

#endif
