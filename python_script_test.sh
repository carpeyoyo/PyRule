#!/bin/bash

python_script=$PWD/python_script
python_filename=$PWD/PythonExample/sys.py
working_directory=$PWD/PythonExample
argument_string=aaa
pipe_fd=-1

echo Python Script: $python_script
echo Python Filename: $python_filename
echo Working Directory: $working_directory
echo Argument String: $argument_string
echo Pipe File Descriptor: $pipe_fd

$python_script $python_filename $working_directory $argument_string $pipe_fd

echo Status: $?
