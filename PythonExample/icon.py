# Joshua Mazur (carpeyoyo.github.io)
# Last Edited: Mar. 15, 2017
# Simple Program used to draw PyRule's icon.
# See included License file for license

import PyRule

def icon():
    #Points
    a = (0,0.866,0)
    b = (-0.25,0.433,0)
    c = (0.25,0.433,0)
    d = (-0.5,0,0)
    e = (0.5,0,0)
    f = (0,0,0)
    # Object
    o = PyRule.Object()
    # Red Triangle
    o.color(1,0,0)
    o.append(b,d,f)
    # Green Triangle
    o.color(0,1,0)
    o.append(f,e,c)
    # Yellow Diamond
    o.color(1,1,0)
    o.append(a,b,f)
    o.append(a,f,c)
    # Translate
    o.translate(0,-0.433,0)
    # Scaling
    o.scale(300,300,300)
    # Display on screen
    o.show()

icon()
