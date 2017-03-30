// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 29, 2017
// Common Information for app
// See included License file for license

#include "common.h"

ComputeInfoTo *ComputeInfoTo_Setup(Queue *to, Queue *from){
  // Constructor
  ComputeInfoTo *info;
  info = (ComputeInfoTo *) malloc(sizeof(ComputeInfoTo));
  info->width = 1;
  info->height = 1;
  info->projection = NULL;
  info->view_angle = NULL;
  info->view_trans = NULL;
  info->objects = NULL;
  info->objects_size = 0;
  info->to = to;
  info->from = from;
  return info;
}

void *ComputeInfoTo_Cleanup(ComputeInfoTo *info){
  // Destructor
  size_t i;
  if (info != NULL){
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
      free(info->objects);
    }
    free(info);
  }
}

ComputeInfoFrom *ComputeInfoFrom_Setup(void){
  // Constructor
  ComputeInfoFrom *info;
  info = (ComputeInfoFrom *) malloc(sizeof(ComputeInfoFrom));
  if (info != NULL){
    info->canvas = NULL;
  }
  return info;
}

void ComputeInfoFrom_Cleanup(ComputeInfoFrom *info){
  // Destructor
  if (info != NULL){
    if (info->canvas != NULL){
      cairo_surface_destroy(info->canvas);
    }
    free(info);
  }
}
