ASM 01: COUNTDOWN
------------------------
This is an example assembly program that shows a 5-second countdown.

Note that this can be done without the list of characters, by just
using something like the following instead:

MOV R1, '0'
IADD R1, Seconds

However, the example uses the list to illustrate declarations of
data literals and working with pointers.

If you want to learn assembly, I recommend that you try to modify
this program and attempt to make variants that do the following:

A) Let you restart the count by pressing START
B) Let you add 1 second to the count by pressing UP
C) Show the digits larger, by applying a scale of 3X
D) Play a short 'beep' sound every time a second passes
