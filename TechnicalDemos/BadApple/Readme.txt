===========================================================
          "BAD APPLE" DEMO FOR VIRCON32 CONSOLE
-----------------------------------------------------------
             Released on December 31st, 2024
===========================================================
This source code produces a Vircon32 ROM that plays the now
famous musical video Bad Apple. This project is meant just
as a technical demo, since this console is not designed to
be able to play continuous full-screen videos.


CREDITS:
---------------------------

    - The original song is Bad Apple!! from game "Lotus Land
      Story", belonging to the TouHou Project saga.
    - The remixed version of the song used here was done by
      Masayoshi Minoshima and Nomico.
    - The silhouette animation video was made by Anira.
    
    The Vircon32 demo itself was created by Carra.


TECHNICAL NOTES:
---------------------------
Vircon32 can only draw images from the predefined textures
contained in each cartridge, so the only way to store and
display a video is to encode each frame in some way that
allows it to be redrawn on the fly later.

For this demo the 6562 frames of the video were reduced to
size 480x360 and pure monochrome (black and white). Then
each frame is divided into its 360 scanlines and each line
is encoded as a series of black segments drawn over a full
white background. The frames are then reconstructed at the
original rate of 30 frames per second.

The encoded video is simply a sequence of 32-bit words, in
which each word has the following 4 possible meanings:

Case 1:
  Word 0xFFFFEEEE signals the end of the video.

Case 2:
  Word 0xFFFFFFFF signals the end of current frame.

Case 3:
  When the word's higher 16 bits are 0xFFFF, this signals
  the start of a new horizontal line. The Y coordinate for
  the new line is given by the word's lower 16 bits.

Case 4:
  Any other value represents a black horizontal segment to
  be drawn at the current line: 
  - The 16 higher bits contain the segment starting X
  - The 16 lower bits contain the segment ending X

This Vircon32 demo only implements the decoding. The video
was encoded using a separate program for PC, also available
as open source in the Vircon32 repositories.


LICENSE:
---------------------------

    This demo itself is open source, and is offered under the
    3-Clause BSD License. But please be aware that the original
    song and video are copyrighted by their original authors.
    
    This demo is only intented as a technical showcase for
    Vircon32, and not meant as copyright infringement. By now
    making all sorts of machines display the Bad Apple is a
    widespread practice and has become a meme, much like trying
    to make them run Doom.
