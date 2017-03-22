// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Object Queue for threads.
// See included License file for license

#ifndef QUEUE_H
#define QUEUE_H

#include <semaphore.h>

// Queue Object
typedef struct{
  // Queue's array 
  void **array;
  int array_size;
  // Location
  int in_index;
  int out_index;
  // Semaphores
  sem_t in_sem;
  sem_t out_sem;
} Queue;

// Queue Methods
Queue *Queue_Setup(int size);
void Queue_Cleanup(Queue *q);
void Queue_Add(Queue *q, void *obj); // Blocks
void *Queue_Next(Queue *q); // Blocks
void *Queue_TryNext(Queue *q); // Nonblocking

#endif
