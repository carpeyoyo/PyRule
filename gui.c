// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Code for GUI interface.
// See included License file for license

#include "gui.h"
#include <gdk/gdkkeysyms.h> // GTK key Symbols
#include <cairo-svg.h> // For saving SVG images
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "column_matrix.h"
#include <math.h>

#define INCREMENT 0.0174532925

// AppInfo Methods
AppInfo *AppInfoSetup(void){
  // AppInfo Constructor
  AppInfo *info;

  info = (AppInfo *) malloc(sizeof(AppInfo));

  // Draw Area
  info->drawarea_width = 0;
  info->drawarea_height = 0;

  // Message Queue Setup
  info->message_queue = MessageQueue_Setup();

  // Queues
  info->to_python = Queue_Setup(2);
  info->from_python = Queue_Setup(2);
  info->to_compute = Queue_Setup(10);
  info->from_compute = Queue_Setup(10);

  // Python Module Information
  info->py_info = SetupPythonInfo(info->to_python,info->from_python);
  info->py_info->mq = info->message_queue;
  info->executing = 0;

  // Program Strings
  info->file_path = NULL;
  info->directory_path = NULL;

  // Draw Properties
  info->projection = NULL;
  info->view_angle = IdentityMatrix();
  info->view_trans = IdentityMatrix();
  info->objects = NULL;
  info->objects_size = 0;
  info->canvas = NULL;

  // Compute Thread
  info->compute_info = ComputeThread_Setup(info->to_compute,info->from_compute);
  
  return info;
}

void AppInfoCleanup(AppInfo *info){
  // AppInfo Destructor
  size_t i;
  if (info != NULL){
    // Python Module Info
    if (info->executing == 1){
      InterruptScript();
      MessageQueue_clear(info->message_queue);
    }

    // Python Info
    CleanupPythonInfo(info->py_info);
    
    // Message Queue Cleanup (Needs to be removed after py_info)
    MessageQueue_Cleanup(info->message_queue);

    // Program Strings
    if (info->file_path != NULL){
      g_free(info->file_path);
    }
    if (info->directory_path != NULL){
      g_free(info->directory_path);
    }

    // Compute Thread
    ComputeThread_Cleanup(info->compute_info);

    // Queue Cleanup
    Queue_Cleanup(info->to_python);
    Queue_Cleanup(info->from_python);
    Queue_Cleanup(info->to_compute);
    Queue_Cleanup(info->from_compute);

    // Draw Properties
    if (info->projection != NULL){
      free(info->projection);
    }
    if (info->view_angle != NULL){
      free(info->view_angle);
    }
    if (info->view_trans != NULL){
      free(info->view_trans);
    }
    if (info->objects != NULL){
      for (i=0; i<info->objects_size; i++){
	Object_Cleanup(info->objects[i]);
      }
    }
    if (info->canvas != NULL){
      cairo_surface_destroy(info->canvas);
    }
    
    // Free Object
    free(info);
  }
}

void AppInfo_AddDraw(AppInfo *info){
  // Queue's ComputeInfoTo Object for draw using info's current values
  ComputeInfoTo *to;
  size_t i;
  if (info != NULL){
    to = ComputeInfoTo_Setup(info->to_compute,info->from_compute);
    to->width = (float) info->drawarea_width;
    to->height = (float) info->drawarea_height;
    if (info->projection != NULL){
      to->projection = ReturnCopyMatrix(info->projection);
    }
    if (info->view_angle != NULL){
      to->view_angle = ReturnCopyMatrix(info->view_angle);
    }
    if (info->view_trans != NULL){
      to->view_trans = ReturnCopyMatrix(info->view_trans);
    }
    if (info->objects != NULL){
      to->objects = (Object **) calloc(sizeof(Object *),info->objects_size);
      to->objects_size = info->objects_size;
      for (i=0; i<to->objects_size; i++){
	to->objects[i] = Object_CreateCopy(info->objects[i]);
      }
    }
    Queue_Add(info->to_compute,(void *)to);
  }
}

// GUI Setup
void gtk_setup(int argc, char **argv, AppInfo *info){
  // GTK Custom Setup in preparation for gtk_main
  GtkBuilder *builder;
  GtkWidget *temp;
  gchar *buffer;
  
  gtk_init(&argc,&argv);

  // Builder
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder,"main_window.glade",NULL);
  
  // Main Window
  info->window = GTK_WIDGET(gtk_builder_get_object(builder,"main_window"));
  g_signal_connect(info->window,"destroy",G_CALLBACK(on_window_main_destroy),NULL);

  // Drawing frame
  temp = GTK_WIDGET(gtk_builder_get_object(builder,"draw_frame"));
  gtk_widget_set_size_request(temp,512,512); // Setting initial drawing frame size

  // Message Textview
  info->message_textview = (GtkTextView *) GTK_WIDGET(gtk_builder_get_object(builder,"program_output_textview"));
  gtk_text_view_set_editable(info->message_textview,FALSE);
  buffer = (gchar *) calloc(sizeof(gchar),50);
  snprintf(buffer,49,"Welcome to PyRule.\n");
  info->message_textbuffer = gtk_text_view_get_buffer(info->message_textview);
  gtk_message_printf(info->message_textbuffer,buffer);
  free(buffer);

  // Argument Textbuffer
  temp = GTK_WIDGET(gtk_builder_get_object(builder,"arguments_textview"));
  info->argument_textbuffer = gtk_text_view_get_buffer((GtkTextView *)temp);
  g_signal_connect((GtkTextBuffer *)info->argument_textbuffer,"end-user-action",G_CALLBACK(BufferOnlySpaces),NULL);

  // Execute Button
  info->execute_button = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(builder,"execute_button"));
  g_signal_connect(info->execute_button,"clicked",G_CALLBACK(execute_button_function),(void *)info);

  // Interrupt Button
  info->interrupt_button = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(builder,"interrupt_button"));
  g_signal_connect(info->interrupt_button,"clicked",G_CALLBACK(interrupt_button_function),(void *)info);
  gtk_widget_set_sensitive((GtkWidget *)info->interrupt_button,FALSE);

  // Program Output Clear Button
  info->output_clear_button = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(builder,"program_output_clear_button"));
  g_signal_connect(info->output_clear_button,"clicked",G_CALLBACK(program_output_clear_button_function),(void *)info);

  // Program Output Copy Button
  info->output_copy_button = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(builder,"program_output_copy_button"));
  g_signal_connect(info->output_copy_button,"clicked",G_CALLBACK(program_output_copy_button_function),(void *)info);

  // Save Svg Button
  info->save_svg_button = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(builder,"save_svg_button"));
  g_signal_connect(info->save_svg_button,"clicked",G_CALLBACK(save_svg_button_function),(void *)info);

  // Positive X Axis Button
  info->positive_x_axis_button = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(builder,"positive_x_axis_button"));
  g_signal_connect(info->positive_x_axis_button,"clicked",G_CALLBACK(positive_x_axis_button_function),(void *)info);

  // Negative X Axis Button
  info->negative_x_axis_button = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(builder,"negative_x_axis_button"));
  g_signal_connect(info->negative_x_axis_button,"clicked",G_CALLBACK(negative_x_axis_button_function),(void *)info);  

  // Positive X Axis Button
  info->positive_y_axis_button = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(builder,"positive_y_axis_button"));
  g_signal_connect(info->positive_y_axis_button,"clicked",G_CALLBACK(positive_y_axis_button_function),(void *)info);

  // Negative Y Axis Button
  info->negative_y_axis_button = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(builder,"negative_y_axis_button"));
  g_signal_connect(info->negative_y_axis_button,"clicked",G_CALLBACK(negative_y_axis_button_function),(void *)info);  

  // Positive Z Axis Button
  info->positive_z_axis_button = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(builder,"positive_z_axis_button"));
  g_signal_connect(info->positive_z_axis_button,"clicked",G_CALLBACK(positive_z_axis_button_function),(void *)info);

  // Negative Z Axis Button
  info->negative_z_axis_button = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(builder,"negative_z_axis_button"));
  g_signal_connect(info->negative_z_axis_button,"clicked",G_CALLBACK(negative_z_axis_button_function),(void *)info);  
  
  // Perspective Buttons
  info->orthographic_button = (GtkRadioButton *) GTK_WIDGET(gtk_builder_get_object(builder,"orthographic_button"));
  info->perspective_button = (GtkRadioButton *) GTK_WIDGET(gtk_builder_get_object(builder,"perspective_button"));
  gtk_radio_button_join_group(info->perspective_button,info->orthographic_button);
  info->projection_state = 0;
  gtk_toggle_button_set_active((GtkToggleButton *)info->orthographic_button,TRUE);
  g_signal_connect(info->orthographic_button,"toggled",G_CALLBACK(orthographic_button_function),(void *)info);
  g_signal_connect(info->perspective_button,"toggled",G_CALLBACK(perspective_button_function),(void *)info);
 
  // Program File chooser
  temp = GTK_WIDGET(gtk_builder_get_object(builder,"program_file_chooser"));
  gtk_file_chooser_set_action((GtkFileChooser *)temp,GTK_FILE_CHOOSER_ACTION_OPEN);
  g_signal_connect(temp,"file-set",G_CALLBACK(python_file_chooser_file_set),(void *)info);

  // Working Directory Chooser
  temp = GTK_WIDGET(gtk_builder_get_object(builder,"working_directory_chooser"));
  gtk_file_chooser_set_action((GtkFileChooser *)temp,GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
  g_signal_connect(temp,"file-set",G_CALLBACK(python_working_directory_chooser_file_set),(void *)info);

  // Draw_area Signals 
  info->drawarea = (GtkDrawingArea *) GTK_WIDGET(gtk_builder_get_object(builder,"draw_area"));
  g_signal_connect(info->drawarea,"draw",G_CALLBACK(draw_area_draw_function),(void *)info);
  g_signal_connect(info->drawarea,"size-allocate",G_CALLBACK(draw_area_size_allocation_function),(void *)info);
  gtk_widget_add_events((GtkWidget *)info->drawarea,GDK_KEY_PRESS_MASK);
  g_signal_connect(info->drawarea,"key_press_event",G_CALLBACK(drawarea_key_press_function),(void *)info);
  gtk_widget_add_events((GtkWidget *)info->drawarea,GDK_BUTTON_PRESS_MASK);
  g_signal_connect(info->drawarea,"button-press-event",G_CALLBACK(draw_area_button_press_function),(void *)info);
  gtk_widget_set_can_focus((GtkWidget *)info->drawarea,TRUE);

  // Axis Draw Area
  info->axis_draw_area = (GtkDrawingArea *) GTK_WIDGET(gtk_builder_get_object(builder,"axis_drawing_area"));
  g_signal_connect(info->axis_draw_area,"draw",G_CALLBACK(axis_draw_area_draw_function),(void *)info);
  gtk_widget_add_events((GtkWidget *)info->axis_draw_area,GDK_BUTTON_PRESS_MASK);
  g_signal_connect(info->axis_draw_area,"button-press-event",G_CALLBACK(draw_area_button_press_function),(void *)info);

  // Timeout Function Signal
  g_timeout_add(10,timeout_function,(void *)info);
    
  // Showing window
  gtk_widget_show(info->window);
  
  // Unref Builder
  g_object_unref(builder);
}

// Common Functions
void gtk_message_printf(GtkTextBuffer *text_buffer, gchar *message){
  // Copies message to message_textview
  GtkTextIter end_iter;
  gtk_text_buffer_get_end_iter(text_buffer,&end_iter);
  gtk_text_buffer_insert(text_buffer,&end_iter,message,-1);
}

float degree_from_radian(float radian){
  float answer;
  answer = radian * 180.0 / M_PI;
  return answer;
}

float radian_from_degree(float degree){
  float answer;
  answer = degree * M_PI / 180.0;
  return answer;
}

// Main Window
void on_window_main_destroy(void){
  // Called when window is called to destroy.
  gtk_main_quit();
}

//// Widget Signals
// Button Functions
void execute_button_function(GtkButton *widget, gpointer g_data){
  // Function called to execute script
  AppInfo *info;
  size_t size;
  gchar *buffer;
  GtkTextIter start,end;
  PythonInfoTo *py_to;

  info = (AppInfo *) g_data;
  py_to = PythonInfoTo_Setup(info->to_python,info->from_python);
  
  //// Copying Strings
  // File path
  if (info->file_path != NULL){
    size = strlen(info->file_path) + 1; 
    py_to->filename = (char *) calloc(sizeof(char),size);
    strcpy(py_to->filename,info->file_path);
  }
  // Working Directory
  if (info->directory_path != NULL){
    size = strlen(info->directory_path) + 1;
    py_to->directory = (char *) calloc(sizeof(char),size);
    strcpy(py_to->directory,info->directory_path);
  }
  // Arguments
  gtk_text_buffer_get_start_iter(info->argument_textbuffer,&start);
  gtk_text_buffer_get_end_iter(info->argument_textbuffer,&end);
  buffer = gtk_text_buffer_get_text(info->argument_textbuffer,&start,&end,FALSE);
  size = strlen(buffer) + 1;
  if (size > 1){
    py_to->arguments = (char *) calloc(sizeof(char),size);
    strcpy(py_to->arguments,buffer);
  }
  g_free(buffer);
  
  // Thread now executing
  info->executing = 1;

  // Sending out information
  if (info->to_python != NULL){
    Queue_Add(info->to_python,(void *)py_to);
  }
  
  // Switching button sensitivity
  gtk_widget_set_sensitive((GtkWidget *)info->interrupt_button,TRUE);
  gtk_widget_set_sensitive((GtkWidget *)info->execute_button,FALSE);
}

void interrupt_button_function(GtkButton *widget, gpointer g_data){
  // Function to cause interrupt in execute script
  printf("Interrupt button Function.\n");
  InterruptScript();
}

void program_output_clear_button_function(GtkButton *widget, gpointer g_data){
  // Function Program Output Clear Button
  GtkTextIter start,end;
  AppInfo *info;

  info = (AppInfo *) g_data;
  
  gtk_text_buffer_get_start_iter(info->message_textbuffer,&start);
  gtk_text_buffer_get_end_iter(info->message_textbuffer,&end);

  gtk_text_buffer_delete(info->message_textbuffer,&start,&end);
}

void program_output_copy_button_function(GtkButton *widget, gpointer g_data){
  // Function for copying program output
  GdkDisplay *display;
  GtkClipboard *clipboard;
  GtkTextIter start,end;
  AppInfo *info;
  gchar *buffer;

  info = (AppInfo *) g_data;

  gtk_text_buffer_get_start_iter(info->message_textbuffer,&start);
  gtk_text_buffer_get_end_iter(info->message_textbuffer,&end);

  buffer = gtk_text_buffer_get_text(info->message_textbuffer,&start,&end,FALSE);
  
  display = gdk_display_get_default();

  clipboard = gtk_clipboard_get_default(display);

  gtk_clipboard_set_text(clipboard,buffer,-1);

  g_free(buffer);
}

void save_svg_button_function(GtkButton *widget, gpointer g_data){
  // Function to save current canvas as SVG
  // Currently just creates a canvas and copies it to the given filename in order to test
  AppInfo *info;
  cairo_surface_t *svg_surface;
  cairo_t *svg_cr;
  int x,y;
  GtkWidget *dialog;
  GtkFileChooser *chooser;
  gint status;
  char *filename;
  GtkFileChooserAction action;

  info = (AppInfo *) g_data;

  if (info->canvas != NULL){
  
    x = info->drawarea_width;
    y = info->drawarea_height;
    
    // Save dialog
    action = GTK_FILE_CHOOSER_ACTION_SAVE;
    dialog = gtk_file_chooser_dialog_new ("Save File",
					  (GtkWindow *)info->window,
					  action,
					  "Cancel",
					  GTK_RESPONSE_CANCEL,
					  "Save",
					  GTK_RESPONSE_ACCEPT,
					  NULL);
    
    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_do_overwrite_confirmation(chooser,TRUE);
    gtk_file_chooser_set_filename(chooser,"test.svg");
    
    status = gtk_dialog_run(GTK_DIALOG(dialog));
    if (status == GTK_RESPONSE_ACCEPT){
      filename = gtk_file_chooser_get_filename(chooser);
      
      if (filename != NULL){
	// SVG
	svg_surface = cairo_svg_surface_create(filename,(double) x, (double) y);
	svg_cr = cairo_create(svg_surface);
	
	cairo_set_source_surface(svg_cr,info->canvas,0,0);
	cairo_paint(svg_cr);
	
	cairo_destroy(svg_cr);
	cairo_surface_destroy(svg_surface);
	
	// Free filename
	g_free(filename);
      }
    }
    
    // Destroy Dialog
    gtk_widget_destroy(dialog);  
  }
}

void positive_x_axis_button_function(GtkButton *widget, gpointer g_data){
  // Change view angle from positive x axis
  AppInfo *info;
  
  info = (AppInfo *) g_data;

  if (info->view_angle != NULL){
    free(info->view_angle);
  }
  
  info->view_angle = YRotationMatrix(M_PI/2.0);

  AppInfo_AddDraw(info);

  gtk_widget_queue_draw((GtkWidget *)info->axis_draw_area);
}

void negative_x_axis_button_function(GtkButton *widget, gpointer g_data){
  // Change view angle from negative y axis
  AppInfo *info;

  info = (AppInfo *) g_data;

  if (info->view_angle != NULL){
    free(info->view_angle);
  }
  
  info->view_angle = YRotationMatrix(-0.5 * M_PI);

  AppInfo_AddDraw(info);

  gtk_widget_queue_draw((GtkWidget *)info->axis_draw_area);
}

void positive_y_axis_button_function(GtkButton *widget, gpointer g_data){
  // Change view angle from positive y axis
  AppInfo *info;
  
  info = (AppInfo *) g_data;

  if (info->view_angle != NULL){
    free(info->view_angle);
  }
  
  info->view_angle = XRotationMatrix(M_PI/2.0);

  AppInfo_AddDraw(info);

  gtk_widget_queue_draw((GtkWidget *)info->axis_draw_area);
}

void negative_y_axis_button_function(GtkButton *widget, gpointer g_data){
  // Change view angle from negative y axis
  AppInfo *info;

  info = (AppInfo *) g_data;

  if (info->view_angle != NULL){
    free(info->view_angle);
  }
  
  info->view_angle = XRotationMatrix(-0.5*M_PI);

  AppInfo_AddDraw(info);  

  gtk_widget_queue_draw((GtkWidget *)info->axis_draw_area);
}  

void positive_z_axis_button_function(GtkButton *widget, gpointer g_data){
  // Change view angle from positive z axis
  AppInfo *info;

  info = (AppInfo *) g_data;

  if (info->view_angle != NULL){
    free(info->view_angle);
  }
  
  info->view_angle = YRotationMatrix(M_PI);

  AppInfo_AddDraw(info);    

  gtk_widget_queue_draw((GtkWidget *)info->axis_draw_area);
}

void negative_z_axis_button_function(GtkButton *widget, gpointer g_data){
  // Change view angle from negative z axis
  AppInfo *info;
  
  info = (AppInfo *) g_data;

  if (info->view_angle != NULL){
    free(info->view_angle);
  }
  
  info->view_angle = IdentityMatrix();

  AppInfo_AddDraw(info);      
  
  gtk_widget_queue_draw((GtkWidget *)info->axis_draw_area);
}  

void orthographic_button_function(GtkButton *widget, gpointer g_data){
  // Sets Projection matrix in common to a Perspective Matrix
  AppInfo *info;
  float right,left,top,bottom,far,near;
  info = (AppInfo *) g_data;
  if (info->projection_state != 0){ 
    info->projection_state = 0;
    right = ((float) info->drawarea_width) / 2.0;
    left = -1.0 * right;
    top = ((float) info->drawarea_height) / 2.0;
    bottom = -1.0 * top;
    far = 601.0;
    near = 1.0;
    if (info->projection != NULL){
      free(info->projection);
    }
    info->projection = OrthographicMatrix(right,left,top,bottom,far,near);
    AppInfo_AddDraw(info);      
  }
}

void perspective_button_function(GtkButton *widget, gpointer g_data){
  // Sets Projection matrix in common to a orthographic matrix
  AppInfo *info;
  float right,left,top,bottom,far,near;
  info = (AppInfo *) g_data;
  if (info->projection_state != 1){
    info->projection_state = 1;
    right = ((float) info->drawarea_width) / 2.0;
    left = -1.0 * right;
    top = ((float) info->drawarea_height) / 2.0;
    bottom = -1.0 * top;
    far = 601.0;
    near = 1.0;
    if (info->projection != NULL){
      free(info->projection);
    }
    info->projection = PerspectiveMatrix(right,left,top,bottom,far,near);
    AppInfo_AddDraw(info);
  }
}

// File Choosers
void python_file_chooser_file_set(GtkWidget *widget, gpointer g_data){
  // Python file to be executed.
  AppInfo *info;

  info = (AppInfo *) g_data;

  if (info->file_path != NULL){
    g_free(info->file_path);
  }
  
  info->file_path = gtk_file_chooser_get_filename((GtkFileChooser *)widget);
}

void python_working_directory_chooser_file_set(GtkWidget *widget, gpointer g_data){
  // Python file working directory
  AppInfo *info;

  info = (AppInfo *) g_data;

  if (info->directory_path != NULL){
    g_free(info->directory_path);
  }

  info->directory_path = gtk_file_chooser_get_filename((GtkFileChooser *)widget);
}  

// Draw Area Functions
gboolean draw_area_draw_function(GtkWidget *widget, cairo_t *cr, gpointer g_data){
  // Draw function for draw_area
  // Pre: Needs drawarea_width and drawarea_height set to the correct dimensions of the drawarea canvas
  // Post: Image is drawn using given cairo context.
  AppInfo *info;
  int x,y;
  
  info = (AppInfo *) g_data;
  
  x = info->drawarea_width;
  y = info->drawarea_height;

  // Background rectangle
  cairo_set_source_rgb(cr,0,0,0);
  cairo_rectangle(cr,0,0,x,y);
  cairo_fill(cr);

  if (info->canvas == NULL){
    // Crossed Lines
    cairo_set_line_width(cr,2.0);
    
    cairo_set_source_rgb(cr,0,1,0); // Green line
    cairo_move_to(cr,0,0);
    cairo_line_to(cr,x,y);
    cairo_stroke(cr);
    
    cairo_set_source_rgb(cr,1,0,0); // Red line
    cairo_move_to(cr,x,0);
    cairo_line_to(cr,0,y);
    cairo_stroke(cr);
  }
  else{
    cairo_set_source_surface(cr,info->canvas,0,0);
    cairo_paint(cr);
  }
  
  return FALSE;
}

void draw_area_size_allocation_function(GtkWidget *widget, GdkRectangle *allocation, gpointer g_data){
  // Size Allocation for draw_area canvas
  AppInfo *info;
  float x,y;
  float *matrix;
  float right,left,top,bottom,far,near;

  info = (AppInfo *) g_data;
  
  info->drawarea_width = gtk_widget_get_allocated_width(widget);
  info->drawarea_height = gtk_widget_get_allocated_height(widget);

  x = (float) info->drawarea_width;
  y = (float) info->drawarea_height;
  
  right = x / 2.0;
  left = -1.0 * right;
  top = y / 2.0;
  bottom = -1.0 * top;
  far = 1000.0;
  near = 1.0;

  if (info->projection_state == 1){ // Projection matrix
    matrix = PerspectiveMatrix(right,left,top,bottom,far,near);
  }
  else{ // Otherwise Orthographic
    matrix = OrthographicMatrix(right,left,top,bottom,far,near);
  }

  if (info->projection != NULL){
    free(info->projection);
  }
  info->projection = matrix;

  AppInfo_AddDraw(info);
}

gboolean drawarea_key_press_function(GtkWidget *widget, GdkEvent *event, gpointer g_data){
  // Key press function for draw_area canvas
  // Key symbol definitions can be found in gdk/gdkkeysyms.h header file
  gint answer;
  GdkEventKey *key_event;
  float *angle;
  float *translation;
  AppInfo *info;
  int call_draw;
  float x,y,z;
  float *current;

  info = (AppInfo *) g_data;
  
  answer = FALSE;  
  call_draw = 0;
  
  angle = NULL;
  translation = NULL;

  key_event = (GdkEventKey *) event;
  switch(key_event->keyval){
  case (GDK_KEY_Left):
    angle = YRotationMatrix(INCREMENT);
    answer = TRUE;
    break;
  case (GDK_KEY_Right):
    angle = YRotationMatrix(-INCREMENT);
    answer = TRUE;
    break;
  case (GDK_KEY_Up):
    angle = XRotationMatrix(INCREMENT);
    answer = TRUE;
    break;
  case (GDK_KEY_Down):
    angle = XRotationMatrix(-INCREMENT);
    answer = TRUE;
    break;
  case (GDK_KEY_equal):
    answer = TRUE;
    break;
  case (GDK_KEY_minus):
    answer = TRUE;
    break;
  case (GDK_KEY_comma):
    angle = ZRotationMatrix(INCREMENT);
    answer = TRUE;
    break;
  case (GDK_KEY_period):
    angle = ZRotationMatrix(-INCREMENT);
    answer = TRUE;
    break;
  }

  if (angle != NULL){
    current = info->view_angle;
    if (current != NULL){
      info->view_angle = (float *) calloc(sizeof(float),16);
      FourByFour_FourByFour(angle,current,info->view_angle);
      call_draw = 1;
      gtk_widget_queue_draw((GtkWidget *)info->axis_draw_area);
      free(current);
    }
    free(angle);
  }

  if (call_draw == 1){
    AppInfo_AddDraw(info);
  }
  
  return answer;
}

void draw_area_button_press_function(GtkWidget *widget, GdkEvent *event, gpointer g_data){
  // Button press function for draw_area canvas
  AppInfo *info;
  info = (AppInfo *) g_data;
  gtk_widget_grab_focus((GtkWidget *)info->drawarea);
}

// Sort Structure for Axis Draw Area Draw Function below
typedef struct{
  float depth;
  int axis; // 0 if x axis, 1 if y axis, 2 if z axis
} axis_draw_area_depth;

// Compare function for sorting axis_draw_area_depth object for draw function below
int Compare_Axis_Depth(const void *aa, const void *bb){
  // Sorts from greatest to least greatest depth value.
  axis_draw_area_depth *a;
  axis_draw_area_depth *b;
  int answer;

  a = (axis_draw_area_depth *) aa;
  b = (axis_draw_area_depth *) bb;

  if (a->depth > b->depth){
    answer = 1;
  }
  else if (a->depth < b->depth){
    answer = -1;
  }
  else if (a->depth == b->depth){
    answer = 0;
  }

  return answer;
}

// Axis Draw Area Draw Function
void axis_draw_area_draw_function(GtkWidget *widget, cairo_t *cr, gpointer g_data){
  // Draw Function for axis draw in menu
  float width,height;
  float answer[4];
  float vector[4];
  float *mvp;
  float **array;
  float right,left,top,bottom;
  float length;
  float x1,y1;
  float x2,y2;
  float x3,y3;
  float x,y;
  AppInfo *info;
  cairo_text_extents_t extents;
  char character[2];
  axis_draw_area_depth depth_array[3];
  int i;
  float red,green,blue;

  info = (AppInfo *) g_data;

  // Dimensions
  width = (float) gtk_widget_get_allocated_width(widget);
  height = (float) gtk_widget_get_allocated_height(widget);

  right = width / 2.0;
  left = -1.0 * right;
  top = height / 2.0;
  bottom = -1.0 * top;

  if (width < height){
    length = width / 3.0;
  }
  else{
    length = height / 3.0;
  }

  // Matrices
  array = (float **) calloc(sizeof(float *),3);
  array[0] = info->view_angle;
  array[1] = TranslationMatrix(0.0,0.0,-300.0);
  array[2] = OrthographicMatrix(right,left,top,bottom,600.0,1.0);
  mvp = MultipleMatrices(array,3);
  free(array[1]);
  free(array[2]);
  free(array);

  // Text Properties
  cairo_select_font_face(cr,"cairo :monospace",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr,12);
  cairo_text_extents(cr,"X",&extents);
  
  // X Axis
  vector[0] = length;
  vector[1] = 0.0;
  vector[2] = 0.0;
  vector[3] = 1.0;
  FourByFour_FourByOne(mvp,vector,answer);

  x1 = right + (answer[0] * right);
  y1 = height - (top + (answer[1] * top));

  depth_array[0].depth = answer[2];
  depth_array[0].axis = 0; // X Axis

  // Y Axis
  vector[0] = 0.0;
  vector[1] = length;
  FourByFour_FourByOne(mvp,vector,answer);

  x2 = right + (answer[0] * right);
  y2 = height - (top + (answer[1] * top));

  depth_array[1].depth = answer[2];
  depth_array[1].axis = 1; // Y Axis

  // Z Axis
  vector[1] = 0.0;
  vector[2] = length;
  FourByFour_FourByOne(mvp,vector,answer);

  x3 = right + (answer[0] * right);
  y3 = height - (top + (answer[1] * top));

  depth_array[2].depth = answer[2];
  depth_array[2].axis = 2; // Z Axis

  free(mvp);

  // Sorting
  qsort(depth_array,3,sizeof(axis_draw_area_depth),Compare_Axis_Depth);

  //// Actual drawing.
  // Background rectangle
  cairo_set_source_rgb(cr,0,0,0);
  cairo_rectangle(cr,0,0,width,height);
  cairo_fill(cr);

  // Lines
  blue = 0.0;
  for (i=0; i<3; i++){
    switch(depth_array[i].axis){
    case(0): // X axis
      strcpy(character,"X");
      x = x1;
      y = y1;
      red = 1.0;
      green = 0.0;
      break;
    case(1): // Y Axis
      strcpy(character,"Y");
      x = x2;
      y = y2;
      red = 0.0;
      green = 1.0;
      break;
    case(2): // Z Axis
      strcpy(character,"Z");
      x = x3;
      y = y3;
      red = 1.0;
      green = 1.0;
      break;
    }

    // Color
    cairo_set_source_rgb(cr,red,green,blue);

    // Line
    cairo_move_to(cr,right,top);
    cairo_line_to(cr,x,y);
    cairo_stroke(cr);

    // Label
    if (x < right){
      x -= extents.width + 1;
    }
    if (y > top){
      y += extents.height + 1;
    }
    cairo_move_to(cr,x,y);
    cairo_show_text(cr,character);
  }
}

// Textbuffer functions
void BufferOnlySpaces(GtkTextBuffer *textbuffer, gpointer user_data){
  // Removes any whitespace other then spaces."
  GtkTextIter start,end;
  gchar *buffer;
  gchar *output;
  size_t size;
  size_t i,j;
  gchar c;
  GtkTextMark *mark;
  GtkTextIter iter;
  gint offset;
  
  mark = gtk_text_buffer_get_insert(textbuffer);
  gtk_text_buffer_get_iter_at_mark(textbuffer,&iter,mark);
  offset = gtk_text_iter_get_offset(&iter);

  gtk_text_buffer_get_start_iter(textbuffer,&start);
  gtk_text_buffer_get_end_iter(textbuffer,&end);
  buffer = gtk_text_buffer_get_text(textbuffer,&start,&end,FALSE);
  size = strlen(buffer);
  if (size > 0){
    output = (gchar *) calloc(sizeof(gchar),size+1);
    j = 0;
    for (i=0; i<size; i++){
      c = buffer[i];
      if ((c != '\t') && (c != '\n')){
	output[j] = c;
	j++;
      }
      else{
	offset = offset - 1;
      }
    }
    output[j] = '\0';
    gtk_text_buffer_set_text(textbuffer,output,-1);
    free(output);
  }
  g_free(buffer);

  gtk_text_buffer_get_iter_at_offset(textbuffer,&iter,offset);
  gtk_text_buffer_place_cursor(textbuffer,&iter);
}

void BufferNumberOnly(GtkTextBuffer *textbuffer, gpointer user_data){
  // Called when contents of buffer has changed to make sure only numbers are added.
  // Post: Make changes to textbuffer.
  GtkTextIter start,end;
  gchar *buffer;
  gchar *output;
  int i,j;
  size_t size;
  gchar temp;
  int decimal;
  int number_count;
  GtkTextMark *mark;
  GtkTextIter iter;
  gint offset;
  
  mark = gtk_text_buffer_get_insert(textbuffer);
  gtk_text_buffer_get_iter_at_mark(textbuffer,&iter,mark);
  offset = gtk_text_iter_get_offset(&iter);
  
  decimal = 0; // Only allows one decimal point.
  number_count = 0;
  gtk_text_buffer_get_start_iter(textbuffer,&start);
  gtk_text_buffer_get_end_iter(textbuffer,&end);
  buffer = gtk_text_buffer_get_text(textbuffer,&start,&end,FALSE);
  size = strlen(buffer);
  if (size > 0){
    output = (gchar *) calloc(sizeof(gchar),size+1);
    j = 0;
    for (i=0; i<size; i++){
      temp = buffer[i];
      if ((temp >= '0') && (temp <= '9')){
	output[j] = temp;
	j++;
	number_count++;
      }
      else if ((temp == '.') && (number_count > 0) && (decimal == 0)){
	output[j] = temp;
	j++;
	decimal++;
      }
      else if ((j == 0) && ((temp == '+') || (temp == '-'))){
	output[j] = temp;
	j++;
      }
      else{
	offset = offset - 1;
      }
    }
    output[j] = '\0';
    gtk_text_buffer_set_text(textbuffer,output,-1);
    free(output);
  }
  g_free(buffer);

  gtk_text_buffer_get_iter_at_offset(textbuffer,&iter,offset);
  gtk_text_buffer_place_cursor(textbuffer,&iter);
}

float retrieve_float_from_textbuffer(GtkTextBuffer *text_buffer){
  // Retrieves the float value currently stored in text_buffer.
  // Pre: Pointer to GtkTextBuffer to retrieve value from.
  // Post: Returns float value on success, NAN otherwise.
  float answer;
  GtkTextIter start,end;
  gchar *buffer;
  float temp;
  int status;
  answer = NAN;
  if (text_buffer != NULL){
    gtk_text_buffer_get_start_iter(text_buffer,&start);
    gtk_text_buffer_get_end_iter(text_buffer,&end);
    buffer = gtk_text_buffer_get_text(text_buffer,&start,&end,FALSE);
    if (strlen(buffer) > 0){
      status = sscanf(buffer,"%f",&temp);
      if (status == 1){
	answer = temp;
      }
    }
    g_free(buffer);
  }
  return answer;
}

void set_textbuffer_from_float(GtkTextBuffer *text_buffer, float value){
  // Sets the text_buffer to the given value
  // Pre: The text_buffer to change, and the float value to set it to.
  // Post: Text_buffer is changed.
  gchar *buffer;
  if (text_buffer != NULL){
    buffer = (gchar *) calloc(sizeof(gchar),500);
    snprintf(buffer,499,"%f",value);
    gtk_text_buffer_set_text(text_buffer,buffer,-1);
    free(buffer);
  }
}

// Timeout Function
gboolean timeout_function(gpointer user_data){
  // Timeout Function for polling other threads' actions
  AppInfo *info;
  char *value, *message, *new_message;
  size_t message_size,new_size;
  size_t i;
  PythonInfoFrom *from;
  ComputeInfoFrom *compute_from;

  info = (AppInfo *) user_data;
  
  // Checking Message Queue
  value = MessageQueue_receive(info->message_queue);
  if (value != NULL){
    message_size = strlen(value) + 1;
    message = (char *) calloc(sizeof(char),message_size);
    strcpy(message,value);
    free(value);
    for (i=0; i<MessageQueueSize; i++){
      value = MessageQueue_receive(info->message_queue);
      if (value == NULL){
	break;
      }
      new_size = message_size + strlen(value) + 1;
      new_message = (char *) calloc(sizeof(char),new_size);
      strcpy(new_message,message);
      free(message);
      message = new_message;
      message_size = new_size;
      strcat(message,value);
      free(value);
    }
    gtk_message_printf(info->message_textbuffer,message);
    free(message);
  }

  // Checking if Python thread is finished
  if (info->executing == 1){
    from = Queue_TryNext(info->from_python);
    if (from != NULL){
      info->executing = 0;
      // End Message
      if (from->status == 0){ // Success
	if (from->objects_current_size == 0){ // No objects to show
	  gtk_message_printf(info->message_textbuffer,"Script Finished.\nNo objects were shown.\n");
	}
	else{ // Objects to display
	  // Display Message
	  message = (char *) calloc(sizeof(char),1000);
	  snprintf(message,999,"Script Finished.\nObjects to display: %lu\n",from->objects_current_size);
	  gtk_message_printf(info->message_textbuffer,message);
	  free(message);
	  // Switch objects over
	  if (info->objects != NULL){
	    for (i=0;i<info->objects_size;i++){
	      Object_Cleanup(info->objects[i]);
	    }
	    free(info->objects);
	  }
	  info->objects = (Object **) calloc(sizeof(Object *),from->objects_current_size);
	  info->objects_size = from->objects_current_size;
	  for (i=0; i<info->objects_size; i++){
	    info->objects[i] = Object_CreateCopy(from->objects[i]);
	  }
	  // Call for draw
	  AppInfo_AddDraw(info);
	}
      }
      else{ // Failure
	gtk_message_printf(info->message_textbuffer,"Script Failed.\n");
      }
      // Switching button sensitivity
      gtk_widget_set_sensitive((GtkWidget *)info->interrupt_button,FALSE);
      gtk_widget_set_sensitive((GtkWidget *)info->execute_button,TRUE);
      // Freeing from
      PythonInfoFrom_Cleanup(from);
    }
  }

  compute_from = Queue_TryNext(info->from_compute);
  if (compute_from != NULL){
    if (info->canvas != NULL){
      cairo_surface_destroy(info->canvas);
    }
    info->canvas = compute_from->canvas;
    compute_from->canvas = NULL;
    ComputeInfoFrom_Cleanup(compute_from);
    gtk_widget_queue_draw((GtkWidget *)info->drawarea);
  }

  return TRUE;
}
