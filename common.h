// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Common Information for app
// See included License file for license

#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <cairo.h>
#include "object.h"
#include "queue.h"

// ComputeInfoTo Object
typedef struct{
  // Canvas Properties
  float width,height;
  // Matrices
  float *projection; // Projection Matrix
  float *view_angle; // View Matrix for angle
  float *view_trans; // View Matrix for camera translation
  // Objects Array
  Object **objects;
  size_t objects_size;
  // Queues
  Queue *to;
  Queue *from;
  // Exit State
  int exit_state; // 1 for exit
} ComputeInfoTo;

ComputeInfoTo *ComputeInfoTo_Setup(Queue *to, Queue *from);
void *ComputeInfoTo_Cleanup(ComputeInfoTo *info);

// ComputeInfoFrom Object
typedef struct{
  // Canvas Properties
  float width,height;
  cairo_surface_t *canvas;  
} ComputeInfoFrom;

ComputeInfoFrom *ComputeInfoFrom_Setup(void);
void ComputeInfoFrom_Cleanup(ComputeInfoFrom *info);

// PythonInfoTo Object
typedef struct{
  // Script Properties
  char *filename;
  char *directory;
  char *arguments;
  // Queues
  Queue *to;
  Queue *from;
  // Exit State
  int exit_state; // 1 for exit 
} PythonInfoTo;

PythonInfoTo *PythonInfoTo_Setup(Queue *to, Queue *from);
void PythonInfoTo_Cleanup(PythonInfoTo *info);

typedef struct{
  // Data Properties
  Object **objects;
  size_t objects_current_size;
  // Scripts ending status
  int status;
} PythonInfoFrom;

PythonInfoFrom *PythonInfoFrom_Setup(void);
void PythonInfoFrom_Cleanup(PythonInfoFrom *info);

#endif
