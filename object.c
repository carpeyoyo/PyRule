// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Generic Object and Surface definitions
// See included License file for license

#include "object.h"
#include "column_matrix.h"
#include <stdio.h>

#define SURFACES_SIZE 10;

// Surface Methods
Surface *Surface_Setup(void){
  // Constructor for Surface
  Surface *s;

  s = (Surface *) malloc(sizeof(Surface));

  s->points = NULL;
  s->points_size = 0;

  s->red = 1.0;
  s->green = 1.0;
  s->blue = 0.0;

  s->normal = NULL;
  
  return s;
}

void Surface_Cleanup(Surface *s){
  // Destructor for Surface
  if (s != NULL){
    if (s->points != NULL){
      free(s->points);
    }
    if (s->normal != NULL){
      free(s->normal);
    }
    free(s);
  }
}

Surface *Surface_CreateCopy(Surface *s){
  // Copy Constructor for Surface object
  // Pre: Valid ponter to Surface object
  // Post: Returns pointer to new Surface object is pointer was valid, NULL otherwise.
  Surface *a;
  size_t i;
  
  a = NULL;

  if (s != NULL){
    a = (Surface *) malloc(sizeof(Surface));

    a->points_size = s->points_size;
    
    if (s->points == NULL){
      a->points = NULL;
    }
    else{
      a->points = (float *) calloc(sizeof(float),a->points_size);
      for (i=0; i<a->points_size; i++){
	a->points[i] = s->points[i];
      }
    }

    a->red = s->red;
    a->green = s->green;
    a->blue = s->blue;

    if (s->normal == NULL){
      a->normal = NULL;
    }
    else{
      a->normal = (float *) calloc(sizeof(float),3);
      for (i=0; i<3; i++){
	a->normal[i] = s->normal[i];
      }
    }
  }

  return a;
}

void Surface_SetColor(Surface *s, float red, float green, float blue){
  // Sets surfaces color values
  // Pre: Valid pointer to Surface, and rgb float values
  // Post: Sets colors in Surface object.
  if (s != NULL){
    s->red = red;
    s->green = green;
    s->blue = blue;
  }
}

void Surface_SwitchPoints(Surface *s, float *points, size_t points_size){
  // Switches points
  // Pre: Pointer to float array and its size
  // Post: Sets points and points_size in Surface object. Also sets normal vector using first
  //       three points. 
  float *b;
  float *c;
  
  if (s != NULL){
    if (s->points != NULL){
      free(s->points);
    }
    s->points = points;
    s->points_size = points_size;
    if (points_size >= 9){
      b = points + 3;
      c = points + 6;
      if (s->normal != NULL){
	free(s->normal);
      }
      s->normal = SurfaceNormalVector3D(points,b,c);
    }
  }
}

// Object Methods
Object *Object_Setup(void){
  // Object Constructor
  Object *o;

  o = (Object *) malloc(sizeof(Object));

  o->surfaces = NULL;
  o->size = 0;
  o->current_size = 0;

  o->model = IdentityMatrix();
  o->model_angle = IdentityMatrix();
  
  return o;
}

void Object_Cleanup(Object *o){
  // Object Destructor
  size_t i;

  if (o != NULL){
    if (o->surfaces != NULL){
      for (i=0; i<o->size; i++){
	Surface_Cleanup(o->surfaces[i]);
	o->surfaces[i] = NULL;
      }
      free(o->surfaces);
      o->surfaces = NULL;
    }

    if (o->model != NULL){
      free(o->model);
      o->model = NULL;
    }

    if (o->model_angle != NULL){
      free(o->model_angle);
      o->model_angle = NULL;
    }
    
    free(o);
  }
}

void Object_AppendSurface(Object *o, Surface *s){
  // Appends surface to object. Surface now belongs to Object
  // Pre: Valid pointers to Object and Surface.
  // Post: if both are valid pointers, s is added to surfaces.
  size_t i;
  Surface **new;
  if ((o != NULL) && (s != NULL)){
    if (o->surfaces == NULL){ // First Call to append
      o->size = SURFACES_SIZE;
      o->surfaces = (Surface **) calloc(sizeof(Surface *),o->size);
      o->current_size = 0;
    }
    else{
      if (o->size == o->current_size){ // Need to increase array size
	o->size += SURFACES_SIZE;
	new = (Surface **) calloc(sizeof(Surface *),o->size);
	for (i=0; i<o->current_size; i++){
	  new[i] = o->surfaces[i];
	}
	free(o->surfaces);
	o->surfaces = new;
      }
    }
    // Append surface
    o->surfaces[o->current_size] = s;
    o->current_size += 1;
  }
}

Object *Object_CreateCopy(Object *o){
  // Copy Constructor for Object
  // Pre: Valid pointer to Object
  // Post: Returns pointer to new Object if argument pointer was valid.
  //       Null otherwise.
  Object *a;
  size_t i;
  
  a = NULL;

  if (o != NULL){
    a = (Object *) malloc(sizeof(Object));
    if (o->surfaces == NULL){
      a->size = 0;
      a->current_size = 0;
      a->surfaces = NULL;
    }
    else{
      a->size = o->size;
      a->current_size = o->current_size;
      a->surfaces = (Surface **) calloc(sizeof(Surface *),a->size);
      for (i=0; i<a->current_size; i++){
	a->surfaces[i] = Surface_CreateCopy(o->surfaces[i]);
      }
    }
    if (o->model == NULL){
      a->model = IdentityMatrix();
    }
    else{
      a->model = (float *) calloc(sizeof(float),16);
      for (i=0; i<16; i++){
	a->model[i] = o->model[i];
      }
    }
    if (o->model_angle == NULL){
      a->model_angle = IdentityMatrix();
    }
    else{
      a->model_angle = (float *) calloc(sizeof(float),16);
      for (i=0; i<16; i++){
	a->model_angle[i] = o->model_angle[i];
      }
    }
  }

  return a;
}

void Object_ModifyModel(Object *o, float *matrix){
  // Multiplies the model matrix by the given matrix
  // Pre: Valid pointer to the object containing the model and valid pointer to 16 element column matrix
  // Post: If pointers are valid, model of object will be set equal to the product of the given matrix
  //       and the old matrix.
  float *new;
  if ((o != NULL) && (matrix != NULL)){
    if (o->model == NULL){
      o->model = ReturnCopyMatrix(matrix);
    }
    else{
      new = (float *) calloc(sizeof(float),16);
      FourByFour_FourByFour(matrix,o->model,new);
      free(o->model);
      o->model = new;
    }
  }
}

void Object_ModifyModelAngle(Object *o, float *matrix){
  // Multiplies the model and model_angle matrices by the given matrix
  // Pre: Valid pointers to Object containing the matrices to be modified, and a valid pointer to the
  //      16 element column matrix array in which they will be modified.
  // Post: If pointers are valid, both matrices will be modified by the given matrix.
  float *new_model;
  float *new_model_angle;
  if ((o != NULL) && (matrix != NULL)){
    if (o->model == NULL){
      o->model = ReturnCopyMatrix(matrix); // As if matrix was multipled by Identity matrix
    }
    else{
      new_model = (float *) calloc(sizeof(float),16);
      FourByFour_FourByFour(matrix,o->model,new_model);
      free(o->model);
      o->model = new_model;
    }
    if (o->model_angle == NULL){
      o->model_angle = ReturnCopyMatrix(matrix); // As if matrix was multiplied by Identity matrix
    }
    else{
      new_model_angle = (float *) calloc(sizeof(float),16);
      FourByFour_FourByFour(matrix,o->model_angle,new_model_angle);
      free(o->model_angle);
      o->model_angle = new_model_angle;
    }
  }
}