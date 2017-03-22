// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Object Queue for threads.
// See included License file for license

#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

Queue *Queue_Setup(int size){
  // Queue Constructor
  // Pre: None
  // Post: Returns pointer to initialized Queue object
  Queue *q;
  int i;
  int status1,status2;
  q = (Queue *) malloc(sizeof(Queue));
  if (q != NULL){
    q->array_size = size;
    q->array = (void **) calloc(sizeof(void *),size);
    if (q->array == NULL){
      free(q);
      q = NULL;
    }
    else{
      // Setting up semaphores
      status1 = sem_init(&(q->in_sem),0,size);
      status2 = sem_init(&(q->out_sem),0,0);
      if ((status1 != 0) || (status2 != 0)){
	if (status1 == 0){
	  sem_destroy(&(q->in_sem));
	}
	if (status2 == 0){
	  sem_destroy(&(q->out_sem));
	}
	free(q);
	q = NULL;
      }
      else{
	// Setting initial index values
	q->in_index = 0;
	q->out_index = 0;
	// Setting values to NULL
	for (i=0; i<size; i++){
	  q->array[i] = NULL;
	}
      }
    }
  }
  return q;
}

void Queue_Cleanup(Queue *q){
  // Queue Destructor
  // Pre: Valid pointer to Queue object
  // Post: All value and Queue object are free if given pointer is valid.
  if (q != NULL){
    // Freeing Queue's array
    while (q->in_index != q->out_index){
      free(q->array[q->out_index]);
      q->out_index += 1;
      if (q->out_index >= q->array_size){
	q->out_index = 0;
      }
    }
    // Destroying semaphores
    sem_destroy(&(q->in_sem));
    sem_destroy(&(q->out_sem));
  }
}

void Queue_Add(Queue *q, void *obj){ // Nonblocking
  // Adds object to given queue.
  // Pre: Non NULL pointers to queue and void pointer object
  // Post: obj is set in next in_index, out_sem in posted.
  if ((q != NULL) && (obj != NULL)){
    sem_wait(&(q->in_sem));
    q->array[q->in_index] = obj;
    sem_post(&(q->out_sem));
    q->in_index += 1;
    if (q->in_index >= q->array_size){
      q->in_index = 0;
    }
  }
}

void *CommonNextFunction(Queue *q){
  // Used by Queue_Next and Queue_TryNext to execute common retrieval code.
  // Pre: Assumes q is a valid pointer
  // Post: Returns pointer stored at out_index and posts in_sem
  void *answer;
  answer = q->array[q->out_index];
  q->out_index += 1;
  if (q->out_index == q->array_size){
    q->out_index = 0;
  }
  sem_post(&(q->in_sem));
  return answer;
}

void *Queue_Next(Queue *q){ // Blocks
  // Returns next pointer in Queue.
  // Pre: Valid pointer to q
  // Post: If q is equal to NULL, NULL is returned. Otherwise the pointer from the current out_index is returned.
  void *answer;
  answer = NULL;
  if (q != NULL){
    sem_wait(&(q->out_sem));
    answer = CommonNextFunction(q);
  }
  return answer;
}

void *Queue_TryNext(Queue *q){ // Nonblocking
  // Returns next pointer in Queue if at least one object is waiting in queue.
  // Pre: Valid pointer to q
  // Post: If q is equal to NULL, NULL is returned. If there are no items waiting, NULL is returned.
  //       Otherwise pointer to next item in queue is returned.
  void *answer;
  int status;
  answer = NULL;
  if (q != NULL){
    status = sem_trywait(&(q->out_sem));
    if (status == 0){
      answer = CommonNextFunction(q);
    }
  }
  return answer;
}
