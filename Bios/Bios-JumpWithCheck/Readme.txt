BIOS: Jump with check
-----------------------
This is the minimal expression of a Vircon32 BIOS. It was the first written BIOS (before there was even any C compiler), so it had to be fully coded in assembly.

It has no logo screen, and it will only check if there is a cartridge connected and immediately transfer execution to it. It will simply show a red screen if there is no cartridge.

Note that even with no logo and no text font, every Vircon32 BIOS is required to include exactly 1 texture and 1 sound to be loaded. That's why this BIOS has an empty dummy for each.

Also, this BIOS reacts to any hardware error by just showing an empty blue screen without any information about the error.
