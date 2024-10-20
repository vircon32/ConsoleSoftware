ASM 03: MOVE (BASIC)
-------------------------------
This is an example assembly program where you control a robot
and can move him freely by pressing directions on the d-pad.

You can move in 8 directions (diagonals included). The program
texture includes the robot facing at every direction, so when
it moves it will be drawn facing your current direction.

These robot images are arranged in a 3x3 matrix. This makes it
convenient for the program to define the regions all at once.
It also makes it easier to select a region to draw based on
current direction. The additional center image is a dummy, it
should never be drawn!

There is no movement animation here, and the program will not
prevent you from leaving the screen.

At top-left the program will show the current robot position
in X and Y coordinates using the default BIOS text font.

If you want to learn assembly, I recommend that you try to modify
this program and attempt to make variants that do the following:

A) Prevent the character from leaving the screen.
B) Make the robot move with some inertia: instead of having a
   constant speed, the robot builds up speed up to a maximum
   when some direction is pressed. And for frames when no
   direction is pressed, its speed decreases until it stops.


CREDITS:
-------------------
Background made from "Sci-fi Interior tiles"
Link:     https://opengameart.org/content/sci-fi-interior-tiles
Author:   Buch
License:  CC0