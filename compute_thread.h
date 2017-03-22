// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Definition file for Object and method for compute thread
// See included License file for license

#ifndef COMPUTE_THREAD_H
#define COMPUTE_THREAD_H

#include <pthread.h>
#include "common.h"
#include <stdlib.h>
#include "queue.h"

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

typedef struct{
  pthread_t thread;
  Queue *to;
  Queue *from;
} ComputeThreadInfo;

// ComputeThreadInfo Methods
ComputeThreadInfo *ComputeThread_Setup(Queue *to, Queue *from);
void ComputeThread_Cleanup(ComputeThreadInfo *info);

#endif
