// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Main function for PyRule Program
// See included License file for license

#include "gui.h"
#include <stdlib.h>
#include <stdio.h>

// Main Function
int main(int argc, char **argv){
  AppInfo *info;

  // Application info Setup
  info = AppInfoSetup();

  // Setup gui
  gtk_setup(argc, argv, info);

  // Main loop
  gtk_main();

  // Application info cleanup
  AppInfoCleanup(info);
  
  return 0;
}
