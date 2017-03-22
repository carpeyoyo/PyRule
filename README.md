# PyRule

    The programmable python CAD. PyRule is licensed under the GNU GENERAL PUBLIC LICENSE Version 3. See included license file for details. 

    This project is currently a proof of concept. Right now simple code can be written in Python that causes an object to be displayed on the drawing area. 

    Included are several example scripts in the "PythonExample" directory. 

    Essentially there is a single object used to draw in the PyRule module. It's name is "PyRule.Object" and it has the following methods: 

1. size()
    * No Arguments given
    * Returns the current size of the object. 

2. get(index)
    * Takes an number indicating the index of the desired surface.
    * Returns a three point tuple representing the surface: ((x1,y1,z1),(x2,y2,z2),(x3,y3,z3)).

3. append(tuple1,tuple2,tuple3)
    * Appends surface to Object. Takes three tuples append((x1,y1,z1),(x2,y2,z2),(x3,y3,z3)).
    * Returns True if surface was appended, False otherwise.

4. \__deepcopy__("Object to Copy")
    * Used by deepcopy to create a copy of the PyRule Object.
    * No arguments are given.
    * Returns PyRule.Object object with identical properties to the current Object, except it is not set as having been shown even if the original object has been.

5. show()
    * Adds PyRule Object to list to be drawn.
    * No Arguments are given.
    * Object will be drawn when script finishes. Returns True if method was successful, False otherwise.

6. translate(x,y,z)
    * Translates the PyRule Object by the given amount.
    * Takes three floats corresponding to the desired translation in the x, y, and z directions.
    * Returns None

7. scale(x,y,z)
    * Scales the PyRule Object by the given amount.
    * Takes three floats corresponding to the desired scale in the x, y, and z directions.
    * Returns None

8. rotatex(angle)
    * Rotates the PyRule Object around the x axis by the given angle in radians.
    * Returns None

9. rotatey(angle)
    * Rotates the PyRule Object around the y axis by the given angle in radians.
    * Returns None

10. rotatez(angle)
    * Rotates the PyRule Object around the z axis by the given angle in radians.
    * Returns None

11. color(red,green,blue)
    * Sets the color for the object. Any surfaces appended to the Object will have this color.
    * The rgb values as floats.
    * Returns None