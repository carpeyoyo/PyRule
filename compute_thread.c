// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 30, 2017
// Definition file for Object and method for compute thread
// See included License file for license

#include "compute_thread.h"
#include "column_matrix.h"
#include <stdio.h>
#include <math.h>

// Thread function for pthread
void *compute_thread(void *arg){
  // Thread for computing projection onto screen.
  ComputeThreadInfo *info;
  int status;
  float **mvp_array;
  float width,height;
  float half_width,half_height;
  Object **objects;
  size_t size;
  size_t i,j,k;
  cairo_surface_t *surface;
  cairo_t *cr;
  Object *current;
  float *mvp;
  float answer[8];
  float *current_answer;
  float b[4];
  Surface *current_surface;
  Modified *mod;
  List_Modified *list;
  float depth;
  float color_ratio;
  float light[3];
  float angle[16];
  float *view_angle;
  float normal[4];
  float rotated_normal[4];
  ComputeInfoTo *to;
  ComputeInfoFrom *from;

  info = (ComputeThreadInfo *) arg;

  light[0] = 0.0;
  light[1] = 1.0;
  light[2] = -1.0;
  Normalize(light,3);
  normal[3] = 1.0;
  
  mvp_array = (float **) calloc(sizeof(float *),4);
  mvp_array[0] = NULL;

  while(1){
    to = (ComputeInfoTo *) Queue_Next(info->to);
    if (to == NULL){
      break;
    }
    if (to->exit_state == 1){
      break;
    }
    
    //// Retrieveing Values
    // Dimensions
    width = to->width;
    height = to->height;
    half_width = width / 2.0;
    half_height = height / 2.0;
    // Matrices
    mvp_array[1] = view_angle = ReturnCopyMatrix(to->view_angle);
    mvp_array[2] = ReturnCopyMatrix(to->view_trans);
    mvp_array[3] = ReturnCopyMatrix(to->projection);
    // Objects
    size = to->objects_size;
    if (size > 0){
      objects = to->objects;  
      //// Drawing
      // Setup
      list = List_Modified_Setup();
      for (i=0; i<size; i++){
	current = objects[i];

	// MVP
	mvp_array[0] = current->model;
	mvp = MultipleMatrices(mvp_array,4);

	// MV angle
	FourByFour_FourByFour(view_angle,current->model_angle,angle);
	
	for (j=0; j<current->current_size; j++){
	  current_surface = current->surfaces[j];
	  k = 0;
	  current_answer = answer;
	  depth = 0;
	  while (k<9){
	    b[0] = current_surface->points[k];
	    k++;
	    b[1] = current_surface->points[k];
	    k++;
	    b[2] = current_surface->points[k];
	    k++;
	    b[3] = 1.0;
	    FourByFour_FourByOne(mvp,b,current_answer);
	    depth += current_answer[2];
	    current_answer += 2;
	  }

	  k = 0; // Stretching projection to screen dimensions
	  while (k < 6){
	    answer[k] = half_width + (answer[k] * half_width);
	    k++;
	    answer[k] = height - (half_height + (answer[k] * half_height));
	    k++;
	  }

	  mod = (Modified *) malloc(sizeof(Modified));
	  mod->x1 = answer[0];
	  mod->y1 = answer[1];
	  mod->x2 = answer[2];
	  mod->y2 = answer[3];
	  mod->x3 = answer[4];
	  mod->y3 = answer[5];
	  normal[0] = current_surface->normal[0];
	  normal[1] = current_surface->normal[1];
	  normal[2] = current_surface->normal[2];
	  FourByFour_FourByOne(angle,normal,rotated_normal);
	  color_ratio = DotProduct(light,rotated_normal,3);
	  color_ratio = clamp(color_ratio,0.5,1.0);
	  mod->red = current_surface->red * color_ratio;
	  mod->green = current_surface->green * color_ratio;
	  mod->blue = current_surface->blue * color_ratio;
	  mod->distance = depth;
	  List_Modified_append(list,mod);
	}
	
	free(mvp);
      }

      List_Modified_sort(list);
      
      if (list->array_current_size > 0){
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	cr = cairo_create(surface);
	for (i=0; i<list->array_current_size; i++){
	  mod = list->array[i];
	  cairo_set_source_rgb(cr,mod->red,mod->green,mod->blue);
	  cairo_move_to(cr,mod->x1,mod->y1);
	  cairo_line_to(cr,mod->x2,mod->y2);
	  cairo_line_to(cr,mod->x3,mod->y3);
	  cairo_close_path(cr);
	  cairo_fill(cr);
	}
	cairo_destroy(cr);
      }
      else{
	surface = NULL;
      }

      // Returning information to GUI thread
      from = ComputeInfoFrom_Setup();
      from->canvas = surface;
      from->width = width;
      from->height = height;
      from->modified_points = list;
      Queue_Add(info->from,(void *)from);
      
      // Cleanup To
      ComputeInfoTo_Cleanup(to);
    }
      
    //// Cleanup
    // Matrices
    if (mvp_array[1] != NULL){
      free(mvp_array[1]);
    }
    if (mvp_array[2] != NULL){
      free(mvp_array[2]);
    }
    if (mvp_array[3] != NULL){
      free(mvp_array[3]);
    }
  }

  if (mvp_array != NULL){
    free(mvp_array);
  }
			  
  pthread_exit(NULL);
}

ComputeThreadInfo *ComputeThread_Setup(Queue *to, Queue *from){
  // ComputeThreadInfo Constructor
  // Returns NULL if common is not valid pointer
  ComputeThreadInfo *info;
  pthread_attr_t attr;

  info = NULL;

  info = (ComputeThreadInfo *) malloc(sizeof(ComputeThreadInfo));
  
  // Thread Setup
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
  
  pthread_create(&(info->thread),&attr,compute_thread,(void *)info);
  
  pthread_attr_destroy(&attr);
  
  // Queues
  info->to = to;
  info->from = from;
  
  return info;
}

void ComputeThread_Cleanup(ComputeThreadInfo *info){
  // ComputeThreadInfo *Destructor
  ComputeInfoTo *to;
  if (info != NULL){
    // Signal exit for thread
    to = ComputeInfoTo_Setup(info->to,info->from);
    to->exit_state = 1;
    Queue_Add(info->to,(void *)to);
    pthread_join(info->thread,NULL);
    // Freeing ComputeThread
    free(info);
  }
}
