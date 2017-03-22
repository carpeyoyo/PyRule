// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Message Queue for communication between threads
// See included License file for license

#include "message_queue.h"
#include <stdlib.h>
#include <string.h>

MessageQueue *MessageQueue_Setup(void){
  // Constructor for MessageQueue structure
  MessageQueue *q;
  int i;

  q = (MessageQueue *) malloc(sizeof(MessageQueue));

  q->array_size = MessageQueueSize;
  q->array = (char **) calloc(sizeof(char *),q->array_size);

  for (i=0; i<q->array_size; i++){
    q->array[i] = NULL;
  }
  
  q->send_index = 0;
  q->receive_index = 0;

  sem_init(&(q->send_sem),0,q->array_size);
  sem_init(&(q->receive_sem),0,0);
  
  return q;
}

void MessageQueue_Cleanup(MessageQueue *q){
  // Destructor for MessageQueue structure
  int i;
  if (q != NULL){
    if (q->array != NULL){
      for (i=0; i<q->array_size; i++){
	if (q->array[i] != NULL){
	  free(q->array[i]);
	}
      }
    free(q->array);
    }
    sem_destroy(&(q->send_sem));
    sem_destroy(&(q->receive_sem));
    free(q);
  }
}

void MessageQueue_send(MessageQueue *q, const char *message){
  // Blocking send which places message string into queue.
  size_t size;
  char *message_copy;
  if (q != NULL){
    sem_wait(&(q->send_sem));
    size = strlen(message) + 1;
    message_copy = (char *) calloc(sizeof(char),size);
    strcpy(message_copy,message);
    q->array[q->send_index] = message_copy;
    q->send_index += 1;
    if (q->send_index == MessageQueueSize){
      q->send_index = 0;
    }
    sem_post(&(q->receive_sem));
  }
}

char *MessageQueue_receive(MessageQueue *q){
  // Non-Blocking receive, returns next message if currently available
  char *answer;
  int status;
  answer = NULL;
  if (q != NULL){
    status = sem_trywait(&(q->receive_sem));
    if (status == 0){
      answer = q->array[q->receive_index];
      q->array[q->receive_index] = NULL;
      q->receive_index += 1;
      if (q->receive_index == MessageQueueSize){
	q->receive_index = 0;
      }
      sem_post(&(q->send_sem));
    }
  }
  return answer;
}

void MessageQueue_clear(MessageQueue *q){
  // Clears Message Queue on the recieve side.
  // Pre: Valid pointer to MessageQueue object.
  // Post: If pointer is valid, frees strings in array and posts send_sem
  int i;
  char *temp;
  if (q != NULL){
    for (i=0; i<q->array_size; i++){
      temp = MessageQueue_receive(q);
      if (temp == NULL){
	break;
      }
      else{
	free(temp);
      }
    }
  }
}
