# Joshua Mazur (carpeyoyo.github.io)
# Last Edited: April 5, 2017
# Demonstrates creating child class of PyRule.Object
# See included License file for license

import PyRule
from copy import deepcopy

class Square(PyRule.Object):
    def __init__(self):
        PyRule.Object.__init__(self)
        self.length = 0;
        self.width = 0;

    def SetDimensions(self,new_length,new_width):
        self.length = new_length
        self.width = new_width
        a = (0,0,0)
        b = (self.length,0,0)
        c = (self.length,self.width,0)
        d = (0,self.width,0)
        self.append(a,b,c)
        self.append(a,c,d)
        
    def __deepcopy__(self,memo):
        answer = Square()
        answer.length = deepcopy(self.length)
        answer.width = deepcopy(self.width)
        answer._copy_values(self)
        answer._reset_show()
        return answer

def main():
    square = Square()
    square.SetDimensions(100,100)
    square.color(0,1,0)
    square.show()

    square2 = deepcopy(square)
    square2.translate(-200,0,0)
    square2.color_all_surfaces(0,1,0)
    square2.show()

    square3 = deepcopy(square)
    square3.translate(-100,-100,50)
    for i in range(0,square3.size(),1):
        square3.color_surface(i,1,0,0)
    square3.show()

    square.color_all_surfaces(0,0,1)
    square.show()

main()
