// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Code for Python C API interface
// See included License file for license

#ifndef PYTHON_MODULE_DEFINITIONS_H
#define PYTHON_MODULE_DEFINITIONS_H

#include "object.h"

// PyRule Object
typedef struct{
  PyObject_HEAD;
  Object *obj;
  int show_value;
  float red,green,blue; // Object color
} PyRuleObject;

// PyRule Object Common Function
void IncreaseArray(PyRuleObject *obj, int increase);
PyObject *NewObject(PyRuleObject *self);

// PyRule Object Methods
static int PyRuleObject_init(PyRuleObject *self, PyObject *args, PyObject *kwds);
static void PyRuleObject_dealloc(PyRuleObject *self);
static PyObject *PyRuleObject_size(PyRuleObject *self, PyObject *args);
static PyObject *PyRuleObject_get(PyRuleObject *self, PyObject *args);
static PyObject *PyRuleObject_append(PyRuleObject *self, PyObject *args);
static PyObject *PyRuleObject_copy(PyRuleObject *self, PyObject *temp);
static PyObject *PyRuleObject_show(PyRuleObject *self, PyObject *temp);
static PyObject *PyRuleObject_translate(PyRuleObject *self, PyObject *args);
static PyObject *PyRuleObject_scale(PyRuleObject *self, PyObject *args);
static PyObject *PyRuleObject_rotatex(PyRuleObject *self, PyObject *args);
static PyObject *PyRuleObject_rotatey(PyRuleObject *self, PyObject *args);
static PyObject *PyRuleObject_rotatez(PyRuleObject *self, PyObject *args);
static PyObject *PyRuleObject_color(PyRuleObject *self, PyObject *args);

// PyRule Standard Output Object
typedef struct{
  PyObject_HEAD;
  int console_output;
} PyRuleStandardOutput;

// PyRule Standard Output Methods
static int PyRuleStandardOutput_init(PyRuleStandardOutput *self, PyObject *args, PyObject *kwds);
static void PyRuleStandardOutput_dealloc(PyRuleStandardOutput *self);
static PyObject *PyRuleStandardOutput_write(PyRuleStandardOutput *self, PyObject *args);
static PyObject *PyRuleStandardOutput_flush(PyRuleStandardOutput *self, PyObject *args);
static PyObject *PyRuleStandardOutput_ConsoleOutput(PyRuleStandardOutput *self, PyObject *args);

// PyRule Standard Error Object
typedef struct{
  PyObject_HEAD;
  int console_output;
} PyRuleStandardError;

// PyRule Standard Error Methods
static int PyRuleStandardError_init(PyRuleStandardError *self, PyObject *args, PyObject *kwds);
static void PyRuleStandardError_dealloc(PyRuleStandardError *self);
static PyObject *PyRuleStandardError_write(PyRuleStandardError *self, PyObject *args);
static PyObject *PyRuleStandardError_flush(PyRuleStandardError *self, PyObject *args);
static PyObject *PyRuleStandardError_ConsoleOutput(PyRuleStandardError *self, PyObject *args);

#endif
