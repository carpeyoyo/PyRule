# Joshua Mazur (carpeyoyo.github.io)
# Last Edited: Mar. 15, 2017
# Demonstrates rotating an object. 
# See included License file for license

import PyRule
import sys
import math

def main():    
    # Points as three element tuple
    l = 50
    ml = -1 * l

    # Points forming three sided tuple.
    a = (ml,l,ml)
    b = (l,l,ml)
    c = (ml,ml,ml)
    d = (l,ml,ml)
    e = (l,l,l)
    f = (l,ml,l)
    g = (ml,l,l)

    # PyRule Object
    o = PyRule.Object()

    print("Getting ready to append.");
    
    o.color(1,0,0) # Setting color
    o.append(a,d,b)
    o.append(a,c,d)

    o.color(0,1,0)
    o.append(b,d,f)
    o.append(f,e,b)

    o.color(0,0,1)
    o.append(g,a,b)
    o.append(b,e,g)
    
    print("Getting Ready to rotate.")
    o.rotatex(math.pi / -4)
    o.rotatey(math.pi / -4)
    o.rotatez(math.pi / -4)
    
    print("Getting ready to show.")
    o.show()
        
main()
