// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Code for Python C API interface
// See included License file for license

#include <Python.h>
#include <stdio.h>
#include "python_module.h"
#include "python_module_definitions.h"
#include "column_matrix.h"

// Static Python Info Object
static PythonInfo *py_info = NULL;

// Python Methods
int PyRuleObject_init(PyRuleObject *self, PyObject *args, PyObject *kwds){
  // PyRule object constructor
  int answer;

  answer = 0;

  if (PyList_Type.tp_init((PyObject *)self,args,kwds) < 0){
    answer = -1;
  }
  else{
    self->obj = Object_Setup();
    self->red = 1.0;
    self->green = 1.0;
    self->blue = 0.0;
    self->show_value = -1;
  }

  return answer;
}

void PyRuleObject_dealloc(PyRuleObject *self){
  // PyRule object destructor 
  Object_Cleanup(self->obj);
  
  Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject *PyRuleObject_size(PyRuleObject *self, PyObject *args){
  // Returns number of items.
  // Pre: None
  // Post: Returns number of three points surfaces
  int answer;
  answer = (int) self->obj->current_size;
  return Py_BuildValue("i",answer);
}

PyObject *PyRuleObject_get(PyRuleObject *self, PyObject *args){
  // Returns tuple containing three (x,y,z) tuples representing triangle
  // Pre: Index of desired triangles
  // Post: Returns previously mentioned tuple, or None if index out of range
  size_t i;
  int return_none;
  PyObject *answer;
  PyObject *p1,*p2,*p3;
  double temp1,temp2,temp3;
  float *array;
  Surface *surface;
  
  return_none = 0;

  answer = NULL;
  
  if (!PyArg_ParseTuple(args,"k",&i)){
    return_none = 1;
  }
  else{
    if ((0 <= i) && (i < self->obj->current_size)){
      surface = self->obj->surfaces[i];

      if (surface->points_size >= 9){
	array = surface->points;
	
	// First point
	temp1 = array[i];
	i++;
	temp2 = array[i];
	i++;
	temp3 = array[i];
	i++;
	p1 = Py_BuildValue("fff",temp1,temp2,temp3);
	
	// Second point
	temp1 = array[i];
	i++;
	temp2 = array[i];
	i++;
	temp3 = array[i];
	i++;
	p2 = Py_BuildValue("fff",temp1,temp2,temp3);
	
	// Third point
	temp1 = array[i];
	i++;
	temp2 = array[i];
	i++;
	temp3 = array[i];
	p3 = Py_BuildValue("fff",temp1,temp2,temp3);
	
	answer = Py_BuildValue("OOO",p1,p2,p3);
      }
    }
    else{
      return_none = 1;
    }
  }

  if (return_none == 1){
    Py_RETURN_NONE;
  }
  else{
    return answer;
  }
}

PyObject *PyRuleObject_append(PyRuleObject *self, PyObject *args){
  // Appends three points to end of list
  // Pre: Tuple contains three (x,y,z) tuples
  // Post: Returns Py_True on success, Py_False on error
  PyObject *p1,*p2,*p3;
  float *points;
  int answer;
  Surface *surface;

  answer = 0;

  points = (float *) calloc(sizeof(float),9);

  if (!PyArg_ParseTuple(args,"OOO",&p1,&p2,&p3)){
    answer = -1;
  }
  else{
    if (!PyArg_ParseTuple(p1,"fff",&(points[0]),&(points[1]),&(points[2]))){
      answer = -1;
    }
    else{
      if (!PyArg_ParseTuple(p2,"fff",&(points[3]),&(points[4]),&(points[5]))){
	answer = -1;
      }
      else{
	if (!PyArg_ParseTuple(p3,"fff",&(points[6]),&(points[7]),&(points[8]))){
	  answer = -1;
	}
	else{
	  surface = Surface_Setup();
	  Surface_SetColor(surface,self->red,self->green,self->blue);
	  Surface_SwitchPoints(surface,points,9);
	  Object_AppendSurface(self->obj,surface);
	}
      }
    }
  }

  if (answer == 0){
    Py_RETURN_TRUE;
  }
  else{
    free(points);
    Py_RETURN_FALSE;
  }
}

PyObject *PyRuleObject_copy(PyRuleObject *self, PyObject *temp){
  // Method for deepcopy
  return NewObject(self);
}

PyObject *PyRuleObject_show(PyRuleObject *self, PyObject *temp){
  // Adds objects' Object to in py_info
  // Pre: None
  // Post: Adds this objects's Object to py_info's objects array and sets this object's show_value
  int answer;
  
  answer = 0;

  if (py_info == NULL){
    answer = 1;
  }
  else{
    if (self->show_value == -1){ // New
      self->show_value = PythonInfo_AddObjectCopy(py_info,self->obj);
    }
    else{ // Already called to show before
      PythonInfo_InsertObjectCopy(py_info,self->obj,self->show_value);
    }
  }

  // Return answer
  if (answer == 0){
    Py_RETURN_TRUE;
  }
  else{
    Py_RETURN_FALSE;
  }
}

PyObject *PyRuleObject_translate(PyRuleObject *self, PyObject *args){
  // Translates object's Object by given x,y,z values
  // Pre: Takes x,y,z parameters for matrix
  // Post: Object's model matrix is modified. Returns None.
  float x,y,z;
  float *matrix;
  
  if (PyArg_ParseTuple(args,"fff",&x,&y,&z)){
    matrix = TranslationMatrix(x,y,z);
    Object_ModifyModel(self->obj,matrix);
    free(matrix);
  }
  
  Py_RETURN_NONE;
}

PyObject *PyRuleObject_scale(PyRuleObject *self, PyObject *args){
  // Scales object's Object by given x,y,z values
  // Pre: Takes x,y,z parameters for matrix
  // Post: Object's model matrix is modified. Returns None.
  float x,y,z;
  float *matrix;
  
  if (PyArg_ParseTuple(args,"fff",&x,&y,&z)){
    matrix = ScaleMatrix(x,y,z);
    Object_ModifyModel(self->obj,matrix);
    free(matrix);
  }
  
  Py_RETURN_NONE;
}

PyObject *PyRuleObject_rotatex(PyRuleObject *self, PyObject *args){
  // Rotates the object's Object around x axis
  // Pre: Takes angle parameter for matrix
  // Post: Object's model matrix is modified. Returns None.
  float angle;
  float *matrix;
  
  if (PyArg_ParseTuple(args,"f",&angle)){
    matrix = XRotationMatrix(angle);
    Object_ModifyModelAngle(self->obj,matrix);
    free(matrix);
  }
  
  Py_RETURN_NONE;
}

PyObject *PyRuleObject_rotatey(PyRuleObject *self, PyObject *args){
  // Rotates the object's Object around y axis
  // Pre: Takes angle parameter for matrix
  // Post: Object's model matrix is modified. Returns None.
  float angle;
  float *matrix;
  
  if (PyArg_ParseTuple(args,"f",&angle)){
    matrix = YRotationMatrix(angle);
    Object_ModifyModelAngle(self->obj,matrix);
    free(matrix);
  }
  
  Py_RETURN_NONE;
}

PyObject *PyRuleObject_rotatez(PyRuleObject *self, PyObject *args){
  // Rotates the object's Object around z axis
  // Pre: Takes angle parameter for matrix
  // Post: Object's model matrix is modified. Returns None.
  float angle;
  float *matrix;
  
  if (PyArg_ParseTuple(args,"f",&angle)){
    matrix = ZRotationMatrix(angle);
    Object_ModifyModelAngle(self->obj,matrix);
    free(matrix);
  }
  
  Py_RETURN_NONE;
}

PyObject *PyRuleObject_color(PyRuleObject *self, PyObject *args){
  // PyRule Object set Color
  // Pre: Argument passes red, green, blue
  // Post: Returns None.
  float r,g,b;
  
  if (PyArg_ParseTuple(args,"fff",&r,&g,&b)){
    self->red = r;
    self->green = g;
    self->blue = b;
  }
  
  Py_RETURN_NONE;
}

static PyMethodDef PyRuleObject_methods[] = {
  {"size",(PyCFunction)PyRuleObject_size,METH_NOARGS,PyDoc_STR("Returns number of triangle surfaces.")},
  {"get",(PyCFunction)PyRuleObject_get,METH_VARARGS,PyDoc_STR("Returns three point tuple.")},
  {"append",(PyCFunction)PyRuleObject_append,METH_VARARGS,PyDoc_STR("Appends three point tuple to end of object.")},
  {"__deepcopy__",(PyCFunction)PyRuleObject_copy,METH_VARARGS,PyDoc_STR("Override of copy class deepcopy method")},
  {"show",(PyCFunction)PyRuleObject_show,METH_NOARGS,PyDoc_STR("Adds object to list to be drawn.")},
  {"translate",(PyCFunction)PyRuleObject_translate,METH_VARARGS,PyDoc_STR("Translates Object by x,y,z")},
  {"scale",(PyCFunction)PyRuleObject_scale,METH_VARARGS,PyDoc_STR("Scales Object by x,y,z")},
  {"rotatex",(PyCFunction)PyRuleObject_rotatex,METH_VARARGS,PyDoc_STR("Rotates Object around x axis")},
  {"rotatey",(PyCFunction)PyRuleObject_rotatey,METH_VARARGS,PyDoc_STR("Rotates Object around y axis")},
  {"rotatez",(PyCFunction)PyRuleObject_rotatez,METH_VARARGS,PyDoc_STR("Rotates Object around z axis")},
  {"color",(PyCFunction)PyRuleObject_color,METH_VARARGS,PyDoc_STR("Object color: red, green, blue.")},
  //{"flush",(PyCFunction)PyRuleObject_flush,METH_VARARGS,PyDoc_STR("Flush for standard output")},
  {NULL,NULL,0,NULL},
};

static PyTypeObject PyRuleObjectType = {
  PyVarObject_HEAD_INIT(NULL,0)
    //0,                       /* ob_size */
    "PyRule.Object",         /* tp_name */
    sizeof(PyRuleObject),          /* tp_basicsize */
    0,                       /* tp_itemsize */
    (destructor)PyRuleObject_dealloc,                       /* tp_dealloc */
    0,                       /* tp_print */
    0,                       /* tp_getattr */
    0,                       /* tp_setattr */
    0,                       /* tp_compare */
    0,                       /* tp_repr */
    0,                       /* tp_as_number */
    0,                       /* tp_as_sequence */
    0,                       /* tp_as_mapping */
    0,                       /* tp_hash */
    0,                       /* tp_call */
    0,                       /* tp_str */
    0,                       /* tp_getattro */
    0,                       /* tp_setattro */
    0,                       /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Interface to PyRule programmable CAD.",                       /* tp_doc */
    0,                       /* tp_traverse */
    0,                       /* tp_clear */
    0,                       /* tp_richcompare */
    0,                       /* tp_weaklistoffset */
    0,                       /* tp_iter */
    0,                       /* tp_iternext */
    PyRuleObject_methods,                       /* tp_methods */
    0,                       /* tp_members */
    0,                       /* tp_getset */
    0,                       /* tp_base */
    0,                       /* tp_dict */
    0,                       /* tp_descr_get */
    0,                       /* tp_descr_set */
    0,                       /* tp_dictoffset */
    (initproc)PyRuleObject_init,   /* tp_init */
    0,                       /* tp_alloc */
    0,                       /* tp_new */
};

PyObject *NewObject(PyRuleObject *self){
  // Used by deepcopy function which needs to reference SimpleObjectType
  PyRuleObject *new;
  
  new = (PyRuleObject *) PyRuleObjectType.tp_alloc(&PyRuleObjectType,0);
  if (new != NULL){
    new->show_value = -1;
    new->red = self->red;
    new->green = self->green;
    new->blue = self->blue;
    new->obj = Object_CreateCopy(self->obj);
  }
  
  return (PyObject *)new;
}

// PyRule Standard Output Object
int PyRuleStandardOutput_init(PyRuleStandardOutput *self, PyObject *args, PyObject *kwds){
  // Standard Output object constructor
  int answer;

  answer = 0;

  if (PyList_Type.tp_init((PyObject *)self,args,kwds) < 0){
    answer = -1;
  }

  return answer;
}

void PyRuleStandardOutput_dealloc(PyRuleStandardOutput *self){
  // Standard Output object destructor
  Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject *PyRuleStandardOutput_write(PyRuleStandardOutput *self, PyObject *args){
  char *value;
  
  if (PyArg_ParseTuple(args,"s",&value)){
    fprintf(stdout,"%s",value);
  }
  
  Py_RETURN_NONE;
}

PyObject *PyRuleStandardOutput_flush(PyRuleStandardOutput *self, PyObject *args){
  fflush(stdout);
  Py_RETURN_NONE;
}

static PyMethodDef PyRuleStandardOutput_methods[] = {
  {"write",(PyCFunction)PyRuleStandardOutput_write,METH_VARARGS,PyDoc_STR("Write for standard output")},
  {"flush",(PyCFunction)PyRuleStandardOutput_flush,METH_VARARGS,PyDoc_STR("Flush for standard output")},
  {NULL,NULL,0,NULL},
};

static PyTypeObject PyRuleStandardOutputType = {
  PyVarObject_HEAD_INIT(NULL,0)
    //0,                       /* ob_size */
    "PyRule.StandardOutput",         /* tp_name */
    sizeof(PyRuleStandardOutput),          /* tp_basicsize */
    0,                       /* tp_itemsize */
    (destructor)PyRuleStandardOutput_dealloc,                       /* tp_dealloc */
    0,                       /* tp_print */
    0,                       /* tp_getattr */
    0,                       /* tp_setattr */
    0,                       /* tp_compare */
    0,                       /* tp_repr */
    0,                       /* tp_as_number */
    0,                       /* tp_as_sequence */
    0,                       /* tp_as_mapping */
    0,                       /* tp_hash */
    0,                       /* tp_call */
    0,                       /* tp_str */
    0,                       /* tp_getattro */
    0,                       /* tp_setattro */
    0,                       /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Standard Output Replacement for PyRule.",                       /* tp_doc */
    0,                       /* tp_traverse */
    0,                       /* tp_clear */
    0,                       /* tp_richcompare */
    0,                       /* tp_weaklistoffset */
    0,                       /* tp_iter */
    0,                       /* tp_iternext */
    PyRuleStandardOutput_methods,                       /* tp_methods */
    0,                       /* tp_members */
    0,                       /* tp_getset */
    0,                       /* tp_base */
    0,                       /* tp_dict */
    0,                       /* tp_descr_get */
    0,                       /* tp_descr_set */
    0,                       /* tp_dictoffset */
    (initproc)PyRuleStandardOutput_init,   /* tp_init */
    0,                       /* tp_alloc */
    0,                       /* tp_new */
};

// PyRule Standard Error Methods
int PyRuleStandardError_init(PyRuleStandardError *self, PyObject *args, PyObject *kwds){
  // Standard Error object constructor
  int answer;

  answer = 0;

  if (PyList_Type.tp_init((PyObject *)self,args,kwds) < 0){
    answer = -1;
  }

  return answer;
}

void PyRuleStandardError_dealloc(PyRuleStandardError *self){
  // Standard Error object destructor
  Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject *PyRuleStandardError_write(PyRuleStandardError *self, PyObject *args){
  char *value;
  
  if (PyArg_ParseTuple(args,"s",&value)){
    fprintf(stderr,"%s",value);
  }
  
  Py_RETURN_NONE;
}

PyObject *PyRuleStandardError_flush(PyRuleStandardError *self, PyObject *args){
  fflush(stderr);
  Py_RETURN_NONE;
}

static PyMethodDef PyRuleStandardError_methods[] = {
  {"write",(PyCFunction)PyRuleStandardError_write,METH_VARARGS,PyDoc_STR("Write for standard error")},
  {"flush",(PyCFunction)PyRuleStandardError_flush,METH_VARARGS,PyDoc_STR("Flush for standard error")},
  {NULL,NULL,0,NULL},
};

static PyTypeObject PyRuleStandardErrorType = {
  PyVarObject_HEAD_INIT(NULL,0)
    //0,                       /* ob_size */
    "PyRule.StandardError",         /* tp_name */
    sizeof(PyRuleStandardError),          /* tp_basicsize */
    0,                       /* tp_itemsize */
    (destructor)PyRuleStandardError_dealloc,                       /* tp_dealloc */
    0,                       /* tp_print */
    0,                       /* tp_getattr */
    0,                       /* tp_setattr */
    0,                       /* tp_compare */
    0,                       /* tp_repr */
    0,                       /* tp_as_number */
    0,                       /* tp_as_sequence */
    0,                       /* tp_as_mapping */
    0,                       /* tp_hash */
    0,                       /* tp_call */
    0,                       /* tp_str */
    0,                       /* tp_getattro */
    0,                       /* tp_setattro */
    0,                       /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Standard Error Replacement for PyRule.",                       /* tp_doc */
    0,                       /* tp_traverse */
    0,                       /* tp_clear */
    0,                       /* tp_richcompare */
    0,                       /* tp_weaklistoffset */
    0,                       /* tp_iter */
    0,                       /* tp_iternext */
    PyRuleStandardError_methods,                       /* tp_methods */
    0,                       /* tp_members */
    0,                       /* tp_getset */
    0,                       /* tp_base */
    0,                       /* tp_dict */
    0,                       /* tp_descr_get */
    0,                       /* tp_descr_set */
    0,                       /* tp_dictoffset */
    (initproc)PyRuleStandardError_init,   /* tp_init */
    0,                       /* tp_alloc */
    0,                       /* tp_new */
};

// PyRule Module Init Function
static PyRuleStandardOutput *standard;
static PyRuleStandardError *standard_error;

static struct PyModuleDef Module_defin = {
  PyModuleDef_HEAD_INIT,
  "PyRule",
  NULL, // Module Documentation
  -1,
  NULL // Module_methods
};

PyMODINIT_FUNC PyInit_PyRule(void){
  PyObject *module;
  module = PyModule_Create(&Module_defin);
  // Add Generic Object
  PyRuleObjectType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&PyRuleObjectType) == 0){
    Py_INCREF(&PyRuleObjectType);
    PyModule_AddObject(module,"Object",(PyObject *)&PyRuleObjectType);
  }
  // Add Standard Output // Need to move these to RunPython or own module.
  PyRuleStandardOutputType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&PyRuleStandardOutputType) == 0){
    Py_INCREF(&PyRuleStandardOutputType);
    PyModule_AddObject(module,"StandardOutput",(PyObject *)&PyRuleStandardOutputType);
    standard = (PyRuleStandardOutput *) PyRuleStandardOutputType.tp_alloc(&PyRuleStandardOutputType,0);
    if (standard != NULL){
      PySys_SetObject("stdout",(PyObject *)standard);
    }
  }
  // Add Standard Error
  PyRuleStandardErrorType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&PyRuleStandardErrorType) == 0){
    Py_INCREF(&PyRuleStandardErrorType);
    PyModule_AddObject(module,"StandardError",(PyObject *)&PyRuleStandardErrorType);
    standard_error = (PyRuleStandardError *) PyRuleStandardErrorType.tp_alloc(&PyRuleStandardErrorType,0);
    if (standard_error != NULL){
      PySys_SetObject("stderr",(PyObject *)standard_error);
    }
  }
  return module;
}

// Functions used by RunPython
// Decode argument string
void SetupArgument(const char *arg, const char *filename){
  // Sets up Python's system argument list for python
  // Pre: The argument string
  // Post: Argument List should be set for system.
  wchar_t **argv;
  wchar_t **argv_temp;
  int argv_size;
  int argc;
  int arg_size;
  int i,j,k;
  char *temp;
  char c;
  
  argc = 1;
  argv_size = 10;

  argv = (wchar_t **) calloc(sizeof(wchar_t *),argv_size);

  if (filename != NULL){
    argv[0] = Py_DecodeLocale(filename,NULL);
  }
  else{
    argv[0] = Py_DecodeLocale(" ",NULL);
  }

  if (arg != NULL){
    arg_size = strlen(arg);
    temp = (char *) calloc(sizeof(char),arg_size + 1);
    j = 0;
    for (i=0; i<arg_size; i++){
      c = arg[i];
      if (c == ' '){
	if (j > 0){
	  temp[j] = '\0';
	  j = 0;
	  if (argc == argv_size){
	    argv_size += 10;
	    argv_temp = (wchar_t **) calloc(sizeof(wchar_t *),argv_size);
	    for (k=0; k<argc; k++){
	      argv_temp[k] = argv[k];
	    }
	    free(argv);
	    argv = argv_temp;
	  }
	  argv[argc] = Py_DecodeLocale(temp,NULL);
	  argc++;
	}
      }
      else{
	temp[j] = c;
	j++;
      }
    }
    if (j > 0){
      temp[j] = '\0';
      if (argc == argv_size){
	argv_size += 10;
	argv_temp = (wchar_t **) calloc(sizeof(wchar_t *),argv_size);
	for (k=0; k<argc; k++){
	  argv_temp[k] = argv[k];
	}
	free(argv);
	argv = argv_temp;
      }
      argv[argc] = Py_DecodeLocale(temp,NULL);
      argc++;
    }
    free(temp);
  }

  PySys_SetArgvEx(argc,argv,0);

  for (i=0; i<argc; i++){
    PyMem_RawFree(argv[i]);
  }

  free(argv);
}

// Filepath Setup
void SetupFilepath(const char *directory){
  wchar_t *file_path;
  wchar_t *file_path_new;
  char *path;
  char *mpath;
  size_t mpath_size;
  size_t file_path_new_size;
  file_path = Py_GetPath(); // Points into static storage
  path = Py_EncodeLocale(file_path,NULL);
  mpath_size = strlen(path) + strlen(directory) + 2;
  mpath = (char *) calloc(sizeof(char),mpath_size);
  strcpy(mpath,directory);
  strcat(mpath,":"); // Delimiter for unix, Windows would use ";"
  strcat(mpath,path);
  PyMem_Free(path);
  file_path_new = Py_DecodeLocale(mpath,&file_path_new_size);
  free(mpath);
  if (file_path_new_size == (mpath_size - 1)){
    PySys_SetPath(file_path_new);
  }
  else{
    fprintf(stderr,"Unable to add working directory.\n");
    fflush(stderr);
  }
  PyMem_RawFree(file_path_new);
}

// Main Execution of Python
void RunPython(PythonInfo *info){
  FILE *myfile;
  wchar_t *program;
  PyObject *temp;

  py_info = info;
  
  info->status = -1;

  myfile = fopen(info->filename,"r");
  if (myfile != NULL){

    program = Py_DecodeLocale("Pyrule",NULL);
    if (program != NULL){
      PyImport_AppendInittab("PyRule",PyInit_PyRule);
      Py_SetProgramName(program);

      // Initialize
      //Py_Initialize();
      Py_InitializeEx(0);

      // Setting File Path
      if (info->directory != NULL){
	SetupFilepath(info->directory);
      }

      // Argument List
      SetupArgument(info->arguments,info->filename);

      // PyImport Module
      PyImport_ImportModule("PyRule");
      
      // Run Program
      info->status = PyRun_SimpleFileEx(myfile,info->filename,1);

      temp = (PyObject *) standard;
      if (temp != NULL){
	Py_CLEAR(temp);
      }

      temp = (PyObject *) standard_error;
      if (temp != NULL){
	Py_CLEAR(temp);
      }
      
      Py_FinalizeEx();
      PyMem_RawFree(program);
    }
  }

  py_info = NULL;
}

PythonInfo *SetupPythonInfo(void){
  // Sets up PythonInfo structure
  PythonInfo *info;
  
  info = (PythonInfo *) malloc(sizeof(PythonInfo));

  // File Properties
  info->filename = NULL;
  info->directory = NULL;
  info->arguments = NULL;

  // Data Properties
  info->objects = NULL;
  PythonInfo_ResetDataProperties(info);

  return info;
}

void CleanupPythonInfo(PythonInfo *obj){
  // Cleanup of PythonInfo Structure
  size_t i;
  if (obj != NULL){
    if (obj->filename != NULL){
      free(obj->filename);
    }
    if (obj->directory != NULL){
      free(obj->directory);
    }
    if (obj->arguments != NULL){
      free(obj->arguments);
    }
    if (obj->objects != NULL){
      for (i=0; i<obj->objects_current_size; i++){
	Object_Cleanup(obj->objects[i]);
      }
      free(obj->objects);
    }
  }
}

void PythonInfo_ClearFileProperties(PythonInfo *info){
  // Clears the File Properties's pointers and sets them to NULL
  if (info != NULL){
    if (info->filename != NULL){
      free(info->filename);
      info->filename = NULL;
    }
    if (info->directory != NULL){
      free(info->directory);
      info->directory = NULL;
    }
    if (info->arguments != NULL){
      free(info->arguments);
      info->arguments = NULL;
    }
  }
}

void PythonInfo_ResetDataProperties(PythonInfo *info){
  // Resets the data information to Setup defaults
  size_t i;
  if (info != NULL){
    if (info->objects != NULL){
      for (i=0; i<info->objects_current_size; i++){
	Object_Cleanup(info->objects[i]);
      }
      info->objects = NULL;
    }
    info->objects_current_size = 0;
    info->objects_size = 10;
    info->objects = (Object **) calloc(sizeof(Object *),info->objects_current_size);
  }
}

int PythonInfo_AddObjectCopy(PythonInfo *info, Object *obj){
  // Adds copy of obj to info's objects array
  // Pre: Valid pointer to PythonInfo object and Object object to be copied
  // Post: Obj is added if neither info or obj are null, current index is returned, or -1 is returned on error.
  Object **new;
  size_t i;
  int answer;
  answer = -1;
  if ((info != NULL) && (obj != NULL)){
    if (info->objects_current_size == info->objects_size){ // Need to increase array size
      info->objects_size += 10;
      new = (Object **) calloc(sizeof(Object *),info->objects_size);
      for (i=0;i<info->objects_current_size;i++){
	new[i] = info->objects[i];
      }
      free(info->objects);
      info->objects = new;
    }
    answer = info->objects_current_size;
    info->objects[info->objects_current_size] = Object_CreateCopy(obj);
    info->objects_current_size += 1;
  }
  return answer;
}

int PythonInfo_InsertObjectCopy(PythonInfo *info, Object *obj, int index){
  // Adds copy of obj to info's object array at index
  // Pre: Pointer to PythonInfo object, pointer to Object object, and index of desired obj to be replaced
  // Post: If info and obj are valid, and index is within range, the Object currently at that index will
  //       be replaced and the index will be returned. Otherwise -1 is returned on error.
  int answer;
  answer = -1;

  if ((info != NULL) && (obj != NULL)){
    if ((0 <= index) && (index < ((int) info->objects_current_size))){
      Object_Cleanup(info->objects[index]);
      info->objects[index] = Object_CreateCopy(obj);
      answer = index;
    }
  }

  return answer;
}

// Interrupt
int quit(void *arg){
  PyErr_SetInterrupt();
  return -1;
}

int InterruptScript(void){
  return Py_AddPendingCall(quit,NULL);
}
