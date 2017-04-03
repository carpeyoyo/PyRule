// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: April 1, 2017
// Code for Python C API for saving data
// See included License file for license

#include <Python.h>
#include "python_save.h"
#include <stdio.h>

static PythonSaveInfo *save_info = NULL;

PythonSaveInfo *PythonSaveInfo_Setup(void){
  // PythonSaveInfo object constructor
  PythonSaveInfo *info;

  info = (PythonSaveInfo *) malloc(sizeof(PythonSaveInfo));

  info->filename = NULL;
  info->savename = NULL;
  info->objects = NULL;
  info->objects_current_size = 0;
  info->objects_size = 0;
  info->status = -1;
  info->surfaces = NULL;
  info->surfaces_size = 0;
  
  return info;
}

void PythonSaveInfo_Cleanup(PythonSaveInfo *info){
  // PythonSaveInfo object destructor
  size_t i;
  if (info != NULL){
    if (info->filename != NULL){
      free(info->filename);
    }
    if (info->savename != NULL){
      free(info->savename);
    }
    if (info->objects != NULL){
      for (i=0; i<info->objects_current_size; i++){
	Object_Cleanup(info->objects[i]);
      }
      free(info->objects);
    }
    if (info->surfaces != NULL){
      free(info->surfaces);
    }
    free(info);
  }
}

static PyObject *PyRuleSave_size(PyObject *self, PyObject *args){
  // Size method
  int size;
  if (save_info != NULL){
    size = (int) save_info->surfaces_size;
    return Py_BuildValue("i",size);
  }
  Py_RETURN_NONE;
}

static PyObject *PyRuleSave_get(PyObject *self, PyObject *args){
  // Get method
  size_t index;
  Surface *surface;
  PyObject *p1,*p2,*p3;
  PyObject *normal;
  PyObject *color;
  float *array;
  if (save_info != NULL){
    if (PyArg_ParseTuple(args,"k",&index)){
      if ((0 <= index) && (index < save_info->surfaces_size)){
	surface = save_info->surfaces[index];
	array = surface->points;
	if ((array != NULL) && (surface->points_size >= 9)){
	  p1 = Py_BuildValue("fff",array[0],array[1],array[2]);
	  p2 = Py_BuildValue("fff",array[3],array[4],array[5]);
	  p3 = Py_BuildValue("fff",array[6],array[7],array[8]);
	}
	else{
	  p1 = Py_BuildValue("s",NULL);
	  p2 = Py_BuildValue("s",NULL);
	  p3 = Py_BuildValue("s",NULL);
	}
	color = Py_BuildValue("fff",surface->red,surface->green,surface->blue);
	array = surface->normal;
	if (array != NULL){
	  normal = Py_BuildValue("fff",array[0],array[1],array[2]);
	}
	else{
	  normal = Py_BuildValue("fff",0,0,0);
	}
	return Py_BuildValue("OOOOO",p1,p2,p3,normal,color);
      }
    }
  }
  Py_RETURN_NONE;
}

static PyObject *PyRuleSave_filename(PyObject *self, PyObject *args){
  // Filename method
  if (save_info != NULL){
    return Py_BuildValue("s",save_info->savename);
  }
  else{
    Py_RETURN_NONE;
  }
}

static PyMethodDef PyRuleSave_methods[] = {
  {"size",(PyCFunction)PyRuleSave_size,METH_NOARGS,PyDoc_STR("Returns number of objects.")},
  {"get",(PyCFunction)PyRuleSave_get,METH_VARARGS,PyDoc_STR("Retrieve single object.")},
  {"filename",(PyCFunction)PyRuleSave_filename,METH_NOARGS,PyDoc_STR("Retrieves the file in which to save the info.")},
  {NULL,NULL,0,NULL},
};

static struct PyModuleDef Module_defin = {
  PyModuleDef_HEAD_INIT,
  "PyRuleSave",
  NULL, // Documentation
  -1,
  PyRuleSave_methods // Methods
};

PyMODINIT_FUNC PyInit_PyRuleSave(void){
  PyObject *module;
  module = PyModule_Create(&Module_defin);
  return module;
}

void RunSaveScript(PythonSaveInfo *info){
  // Runs actual script
  FILE *myfile;
  wchar_t *program;
  
  save_info = info;

  myfile = fopen(info->filename,"r");
  if (myfile != NULL){

    program = NULL;
    program = Py_DecodeLocale("PyruleSave",NULL);

    if (program != NULL){
      PyImport_AppendInittab("PyRuleSave",PyInit_PyRuleSave);
      Py_SetProgramName(program);
      Py_InitializeEx(0);
      PyImport_ImportModule("PyRuleSave");
      info->status = PyRun_SimpleFileEx(myfile,info->filename,1);
      Py_FinalizeEx();
      PyMem_RawFree(program);
    }
  } 
}
