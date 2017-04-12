# PyRule

<p> The programmable python CAD. PyRule is licensed under the GNU GENERAL PUBLIC LICENSE Version 3. See included license file for details. </p>

<p> This project is currently a proof of concept designed for Linux operating systems. Right now simple code can be written in Python that causes an object to be displayed on the drawing area, the drawing area can be saved as a PNG or SVG image, and the data can be saved in a limited way as either a binary STL or using a custom script. </p>

<p> Included are several example scripts in the "PythonExample" directory, and the "SaveScripts" directory contains the stl.py file that is called when the STL save button is pressed and can be used as an example of a custom save file script. </p>

<p> The details of the different methods for both the PyRule and PyRuleSave modules can be found at this project's <a href="https://github.com/carpeyoyo/PyRule/wiki">wiki</a>. </p>

## Installation

<p> PyRule uses GTK+ and Python 3.6 libraries. In Ubuntu, using </p> 

```
sudo apt install libpython3.6-dev libgtk-3-dev
``` 
<p> installed the necessary libraries. </p> 

<p> After having downloaded the git repository from Github, simple run the make command to use the included makefile to build the project files. The GUI depends on a Glade file and two other execution files in the project directory, so pyrule much either be launched from this directory or have it included in path. To launch simple run the "pyrule" elf file. 

