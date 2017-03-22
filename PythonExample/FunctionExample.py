# Joshua Mazur (carpeyoyo.github.io)
# Last Edited: Mar. 21, 2017
# Example of create a object using PyRule.
# See included License file for license

import PyRule
import math
from copy import deepcopy

def create_triangular_pyramid(length):
    height = math.sqrt(0.5 * math.pow(length,2))
    obj = PyRule.Object()
    # Base
    a = (0,0,length)
    b = (length,0,length)
    c = (0,0,0)
    d = (length,0,0)
    # Peak
    e = (0.5 * length,height,length)
    f = (0.5 * length,height,0)
    ## Surfaces
    # Bottom
    obj.append(a,b,c)
    obj.append(b,d,c)
    # Front
    obj.color(1,0,0) # Red
    obj.append(f,c,d)
    # Back
    obj.color(0,0,1) # Blue
    obj.append(e,b,a)
    # Top
    obj.color(0,1,0) # Green
    obj.append(e,a,c)
    obj.append(e,c,f)
    obj.color(1,1,1) # White
    obj.append(e,f,d)
    obj.append(b,e,d)
    return obj

def main():
    a = create_triangular_pyramid(100)
    b = deepcopy(a)
    b.rotatez(0.5 * math.pi)
    b.scale(2,2,2)
    a.show()
    b.show()
    c = deepcopy(a)
    c.scale(0.5,0.5,0.5)
    c.rotatez(math.pi)
    c.show()
    d = deepcopy(a)
    d.scale(1.5,1.5,1.5)
    d.rotatez(1.5 * math.pi)
    d.show()

main()
