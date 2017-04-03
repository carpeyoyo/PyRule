// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 29, 2017
// Generic Object and Surface definitions
// See included License file for license

#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>

#define OBJECT_NEW 100
#define OBJECT_ANOTHER 101
#define OBJECT_END 102

// Generic Surface Object
typedef struct{
  float *points;
  size_t points_size;
  float red,green,blue;
  float *normal;
} Surface;

// Generic Surface Methods
Surface *Surface_Setup(void);
void Surface_Cleanup(Surface *s);
Surface *Surface_CreateCopy(Surface *s);
void Surface_SetColor(Surface *s, float red, float green, float blue);
void Surface_SwitchPoints(Surface *s, float *points, size_t points_size);

// Generic Object
typedef struct{
  Surface **surfaces;
  size_t size;
  size_t current_size;
  float *model;
  float *model_angle;
} Object;

// Generic Object Methods
Object *Object_Setup(void);
Object *Object_From_File(int fd);
void Object_Cleanup(Object *o);
void Object_AppendSurface(Object *o, Surface *s);
Object *Object_CreateCopy(Object *o);
void Object_ModifyModel(Object *o, float *matrix);
void Object_ModifyModelAngle(Object *o, float *matrix);
void Object_To_File(Object *o, int fd, int state);
void Object_ModifyPointsFromMatrices(Object *o);

#endif
