// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 30, 2017
// Definition file for Object and method for compute thread
// See included License file for license

#ifndef COMPUTE_THREAD_H
#define COMPUTE_THREAD_H

#include <pthread.h>
#include "common.h"
#include <stdlib.h>
#include "queue.h"
#include "modified.h"

typedef struct{
  pthread_t thread;
  Queue *to;
  Queue *from;
} ComputeThreadInfo;

// ComputeThreadInfo Methods
ComputeThreadInfo *ComputeThread_Setup(Queue *to, Queue *from);
void ComputeThread_Cleanup(ComputeThreadInfo *info);

#endif
