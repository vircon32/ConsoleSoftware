ASM 02: BOUNCING BALL
------------------------
This is an example assembly program that shows a ball moving
through the screen in a straight line and bouncing against
the screen edges.

The program shows how to separate definitions and collision
code in different source files using the %include directive.
Bouncing is done by just reflecting velocity on X or Y, and
on collisions a bounce sound will play.

If you want to learn assembly, I recommend that you try to modify
this program and attempt to make variants that do the following:

A) Change the code so that instead of independent initial
   components for speed X and speed Y, you define an initial
   speed and angle. HINT: There is no CPU instruction for
   cosine, but you can calculate it with this formula:
   cos(x) = sqrt( 1 - sin(x)^2 ).
B) Implement gravity by making the ball velocity increase
   downwards by a constant every frame. Try adjusting this
   value until ball movement looks realistic.
C) Add a rectangle on the screen and make the ball bounce
   against it using the current collision subroutines.


CREDITS:
-------------------
Ball image made from "Golf Ball"
Link:     https://opengameart.org/content/golf-ball
Author:   jpsweber
License:  CC0

Bounce sound made with JFXR generator
https://jfxr.frozenfractal.com
