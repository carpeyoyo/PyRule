// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 30, 2017
// Contains Modified surface and Modified surface List and methods
// See included License file for license

#ifndef MODIFIED_H
#define MODIFIED_H

#include <stdlib.h>

typedef struct{ // Modified Surface
  float x1,y1;
  float x2,y2;
  float x3,y3;
  float red,green,blue;
  float distance;  
} Modified;

// Modified's methods
int Modified_Compare(const void *aa, const void *bb);

typedef struct{ // Special List for Modified Surface
  Modified **array;
  size_t array_size;
  size_t array_current_size;
} List_Modified;

// List_Modified's Methods
List_Modified *List_Modified_Setup(void);
void List_Modified_Cleanup(List_Modified *list);
void List_Modified_append(List_Modified *list, Modified *mod);
void List_Modified_sort(List_Modified *list);


#endif
