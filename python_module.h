// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Code for Python C API interface
// See included License file for license

#ifndef PYTHON_MODULE_H
#define PYTHON_MODULE_H

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "message_queue.h"
#include "object.h"
#include "common.h"

// PythonInfo structure
typedef struct{
  // File Properties
  char *filename;
  char *directory;
  char *arguments;
  // Data Properties
  Object **objects;
  size_t objects_current_size;
  size_t objects_size;
  // Thread information
  pthread_t thread_id;
  // Message Queue
  MessageQueue *mq;
  // Queues
  Queue *to;
  Queue *from;
  // Status
  int status;
} PythonInfo;

// PythonInfo methods
PythonInfo *SetupPythonInfo(Queue *to, Queue *from);
void CleanupPythonInfo(PythonInfo *obj);
void PythonInfo_ClearFileProperties(PythonInfo *info);
void PythonInfo_ResetDataProperties(PythonInfo *info);
int PythonInfo_AddObjectCopy(PythonInfo *info, Object *obj);
int PythonInfo_InsertObjectCopy(PythonInfo *info, Object *obj, int index);

// Functions to execute python script
void RunPython(PythonInfo *info); // Run in same thread

// Interrupt Running Script
int InterruptScript(void);

#endif
