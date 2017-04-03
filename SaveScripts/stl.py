# Joshua Mazur (carpeyoyo.github.io)
# Last Edited: April 2, 2017
# Saves current data as a binary stl file
# See included License file for license

import PyRuleSave
from struct import pack

def triangle(File,
             normal_x,normal_y,normal_z,
             vertex1_x,vertex1_y,vertex1_z,
             vertex2_x,vertex2_y,vertex2_z,
             vertex3_x,vertex3_y,vertex3_z):
    #Normal vector
    File.write(pack("<fff",normal_x,normal_y,normal_z))
    # Three points
    File.write(pack("<fff",vertex1_x,vertex1_y,vertex1_z))
    File.write(pack("<fff",vertex2_x,vertex2_y,vertex2_z))
    File.write(pack("<fff",vertex3_x,vertex3_y,vertex3_z))
    # Attribute Byte Count
    File.write(pack("<H",0));

def main():
    name = PyRuleSave.filename()
    file = open(name,"wb")

    # Header 
    for i in range(80):
        file.write(pack("<B",0));

    # Number of triangles
    size = PyRuleSave.size()
    file.write(pack("<I",size));

    for i in range(0,size,1):
        ((p1x,p1y,p1z),(p2x,p2y,p2z),(p3x,p3y,p3z),(nx,ny,nz),(red,green,blue)) = PyRuleSave.get(i)
        triangle(file,
                 nx,ny,nz,
                 p1x,p1y,p1z,
                 p2x,p2y,p2z,
                 p3x,p3y,p3z)
    
    file.close()

main()
