# Joshua Mazur (carpeyoyo.github.io)
# Last Edited: Mar. 15, 2017
# Demonstrates accessing system properties are the same as before.
#     And that the arguments include those passed from PyRule's GUI.
# See included License file for license

import PyRule
import sys

def main():
    # Simple Program to test the custom system arguments and
    # path setup from PyRule. 
    print("PyRule System:");
    print("System arguments: ",sys.argv)
    print("System path: ",sys.path)

main()
