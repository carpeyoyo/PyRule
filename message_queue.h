// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Message Queue for communication between threads
// See included License file for license

#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <semaphore.h>

#define MessageQueueSize 250

typedef struct{
  char **array;
  int array_size;
  int send_index;
  int receive_index;
  sem_t send_sem;
  sem_t receive_sem;
} MessageQueue;

// MessageQueue message
MessageQueue *MessageQueue_Setup(void);
void MessageQueue_Cleanup(MessageQueue *q);

void MessageQueue_send(MessageQueue *q, const char *message);
char *MessageQueue_receive(MessageQueue *q);
void MessageQueue_clear(MessageQueue *q);

#endif
