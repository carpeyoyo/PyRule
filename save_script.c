// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: April 2, 2017
// Main file for script that runs python save scripts
// See included License file for license

#include "python_save.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv){
  int answer;
  PythonSaveInfo *info;
  size_t size;
  int fd;
  int state;
  int exit_loop;
  Object **new;
  Object *temp_object;
  size_t i,j,k;
  
  answer = EXIT_FAILURE;

  if (argc != 4){
    printf("Wrong number of arguments.\n");
  }
  else{
    info = PythonSaveInfo_Setup();

    // Python Save Script Full Path
    size = strlen(argv[1]) + 1;
    info->filename = (char *) calloc(sizeof(char),size);
    strcpy(info->filename,argv[1]);

    // Pipe File Descriptor
    fd = -1;
    sscanf(argv[2],"%d",&fd);

    // Filename to save file
    size = strlen(argv[3]) + 1;
    info->savename = (char *) calloc(sizeof(char),size);
    strcpy(info->savename,argv[3]);

    // Retrieve data
    if (fd != -1){
      exit_loop = 0;
      while (exit_loop == 0){
	size = read(fd,&state,sizeof(int));

	switch(state){
	case(OBJECT_NEW):
	case(OBJECT_ANOTHER):
	  if (info->objects_current_size == info->objects_size){
	    info->objects_size += 10;
	    new = (Object **) calloc(sizeof(Object *),info->objects_size);
	    for (i=0; i<info->objects_current_size; i++){
	      new[i] = info->objects[i];
	    }
	    if (info->objects != NULL){
	      free(info->objects);
	    }
	    info->objects = new;
	  }
	  temp_object = Object_From_File(fd);
	  if (temp_object != NULL){
	    info->objects[info->objects_current_size] = temp_object;
	    info->objects_current_size += 1;
	  }
	  break;
	case(OBJECT_END):
	  exit_loop = 1;	
	  break;
	}
      }

      info->surfaces_size = 0;
      for (i=0; i<info->objects_current_size; i++){
	temp_object = info->objects[i];
	Object_ModifyPointsFromMatrices(temp_object);
	info->surfaces_size += temp_object->current_size;
      }

      info->surfaces = (Surface **) calloc(sizeof(Surface *),info->surfaces_size);
     
      j = 0;
      for (i=0; i<info->objects_current_size; i++){
	temp_object = info->objects[i];
	for (k=0; k<temp_object->current_size; k++){
	  info->surfaces[j] = temp_object->surfaces[k];
	  j++;
	}
      }
      
      RunSaveScript(info);
      
      // Determining whether python script finished successfully.
      if (info->status == 0){
	answer = EXIT_SUCCESS;
      }

      // Closing file descriptor
      close(fd);
    }
    
    PythonSaveInfo_Cleanup(info);
  }
    
  return answer;
}

