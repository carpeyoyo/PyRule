// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 28, 2017
// Code for Python C API interface
// See included License file for license

#ifndef PYTHON_MODULE_H
#define PYTHON_MODULE_H

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "object.h"

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
  // Status
  int status;
} PythonInfo;

// PythonInfo methods
PythonInfo *SetupPythonInfo(void);
void CleanupPythonInfo(PythonInfo *obj);
void PythonInfo_ClearFileProperties(PythonInfo *info);
void PythonInfo_ResetDataProperties(PythonInfo *info);
int PythonInfo_AddObjectCopy(PythonInfo *info, Object *obj);
int PythonInfo_InsertObjectCopy(PythonInfo *info, Object *obj, int index);

// Functions to execute python script
void RunPython(PythonInfo *info);

// Interrupt Running Script
int InterruptScript(void);

#endif
