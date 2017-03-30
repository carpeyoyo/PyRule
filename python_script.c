// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 29, 2017
// Code for GUI interface.
// See included License file for license

#include <stdio.h>
#include <stdlib.h>
#include "python_module.h"
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv){
  PythonInfo *py_info;
  int i;
  int answer;
  size_t size;
  int fd;
  int temp;
  size_t j;
  
  if (argc != 5){
    answer = EXIT_FAILURE;
    fprintf(stderr,"Wrong number of arguments.\nReceived: %d\n",argc);
    fflush(stderr);
  }
  else{
    py_info = SetupPythonInfo();

    // Setting filename
    size = strlen(argv[1]);
    py_info->filename = (char *) calloc(sizeof(char),size+1);
    strcpy(py_info->filename,argv[1]);

    // Setting working directory
    size = strlen(argv[2]);
    if (size > 0){
      py_info->directory = (char *) calloc(sizeof(char),size+1);
      strcpy(py_info->directory,argv[2]);
    }

    // Setting argument string
    size = strlen(argv[3]);
    if (size > 0){
      py_info->arguments = (char *) calloc(sizeof(char),size+1);
      strcpy(py_info->arguments,argv[3]);
    }
    
    RunPython(py_info);

    // Return status.
    if (py_info->status == 0){
      answer = EXIT_SUCCESS;

      sscanf(argv[4],"%d",&fd);

      if (fd == -1){
	fprintf(stderr,"File descriptor invalid.\n");
	fflush(stderr);
      }
      else{
	fflush(stdout);
	if (py_info->objects_current_size > 0){
	  fprintf(stdout,"Number of Objects to send: %lu\n",py_info->objects_current_size);
	  fflush(stdout);
	  Object_To_File(py_info->objects[0],fd,OBJECT_NEW);
	  j = 1;
	  while (j < py_info->objects_current_size){
	    Object_To_File(py_info->objects[j],fd,OBJECT_ANOTHER);
	    j++;
	  }
	  temp = OBJECT_END;
	  write(fd,&temp,sizeof(int));
	}
	else{
	  fprintf(stdout,"Sending no objects.\n");
	  fflush(stdout);
	}
	close(fd);
      }
    }
    else{
      answer = EXIT_FAILURE;
    }
  }
    
  return answer;
}
