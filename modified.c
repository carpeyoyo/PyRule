// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 30, 2017
// Contains Modified surface and Modified surface List and methods
// See included License file for license

#include "modified.h"

#define LIST_MODIFIED_INCREMENT 100

// Modified's methods
int Modified_Compare(const void *aa, const void *bb){
  // Compare function used by List_Modified's sort function
  // Pre: Two pointers to Modified objects
  // Post: Returns number of qsort
  int answer;
  Modified *a;
  Modified *b;

  a = *(Modified **) aa;
  b = *(Modified **) bb;
  
  if (a->distance > b->distance){
    answer = 1;
  }
  else if (a->distance < b->distance){
    answer = -1;
  }
  else if (a->distance == b->distance){
    answer = 0;
  }
  
  return answer;
}

// List_Modified's Methods
List_Modified *List_Modified_Setup(void){
  // List_Modified constructor
  List_Modified *list;
  list = (List_Modified *) malloc(sizeof(List_Modified));
  list->array_size = LIST_MODIFIED_INCREMENT;
  list->array = (Modified **) calloc(sizeof(Modified *),list->array_size);
  list->array_current_size = 0;
  return list;
}

void List_Modified_Cleanup(List_Modified *list){
  // List_Modified destructor
  size_t i;
  if (list != NULL){
    for (i=0; i<list->array_current_size; i++){
      free(list->array[i]);
    }
    free(list->array);
    free(list);
  }
}

void List_Modified_append(List_Modified *list, Modified *mod){
  // Appends mod pointer to list
  // Pre: Pointer to List_Modified object, and Modified Object
  // Post: Modified Object pointer is now in the list's array, and will be freed when
  //       the list is finished.
  Modified **new;
  size_t i;
  if ((list != NULL) && (mod != NULL)){
    if (list->array_current_size == list->array_size){
      list->array_size += LIST_MODIFIED_INCREMENT;
      new = (Modified **) calloc(sizeof(Modified *),list->array_size);
      for (i=0; i<list->array_current_size; i++){
	new[i] = list->array[i];
      }
      free(list->array);
      list->array = new;
    }
    list->array[list->array_current_size] = mod;
    list->array_current_size += 1;
  }
}

void List_Modified_sort(List_Modified *list){
  // Sorts Modified objects in array by their distance values, greatest to least.
  // Pre: Valid pointer to List_Modified object
  // Post: all value in array are now sorted.
  if (list != NULL){
    if (list->array_current_size > 1){
      qsort(list->array,list->array_current_size,sizeof(Modified *),Modified_Compare);
    }
  }
}
