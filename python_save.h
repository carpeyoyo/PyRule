// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: April 1, 2017
// Code for Python C API for saving data
// See included License file for license

#ifndef PYTHON_SAVE_H
#define PYTHON_SAVE_H

#include "object.h"
#include <stdlib.h>

typedef struct{
  char *filename;
  char *savename;
  Object **objects;
  size_t objects_current_size;
  size_t objects_size;
  int status;
  Surface **surfaces;
  size_t surfaces_size;
} PythonSaveInfo;

PythonSaveInfo *PythonSaveInfo_Setup(void);
void PythonSaveInfo_Cleanup(PythonSaveInfo *info);

void RunSaveScript(PythonSaveInfo *info);

#endif

