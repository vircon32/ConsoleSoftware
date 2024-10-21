ASM 04: MOVE (ANIMATED)
----------------------------------
(NOTE: This program expands the example "Move Basic", which is a
more basic version with no animation. Please see that one first)

This is an example assembly program where you control a
character and can move him with the d-pad. It is based on the
previous example, but here movement is restricted to just 4
directions (no diagonals). If a diagonal is pressed, the last
pressed d-pad control will be prioritized. Many games choose
to use this kind of movement.

The program texture includes walking cycles for your character,
so when moving your character will be drawn with a walking
animation facing your current direction.

The texture only includes the animation in 3 directions (up,
down, right). So to draw the character facing left the program
program will use the same animation as when walking right, but
draw it mirrored in X by using an X scale factor = -1.

The program will use the BIOS text font to write the current
character position at top-left. But this time, instead of just
writing 3 digits for X and Y, it will only write the needed
digits for each (for instance, it will write 8 instead of 008).
It will also add a title string before each variable.

If you want to learn assembly, I recommend that you try to modify
this program and attempt to make variants that do the following:

A) Add a footsteps sound loop and make it sound continuously,
   but only when the character is walking.
B) Replace the background with an image larger than the screen
   (but remember that texture size limit 1024x1024 is pixels).
   Then adapt the way you draw both character and background
   to make the screen scroll. HINT: Store "world coordinates"
   of your character within that background and draw from a
   "camera view" that is centered on the character.


CREDITS:
-------------------
Background texture made from "Mars Background Pixel Art"
Link:     https://opengameart.org/content/mars-background-pixel-art
Author:   Quantiset
License:  CC0