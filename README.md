fl4shk_gbemu
============

Incomplete GB (and eventually GBC) emulator.  Currently there is CPU emulation, 
interrupt emulation, LCD emulation, OAM DMA functionality, and background/window 
rendering.

SFML 2.0 is a dependency (along with whatever its dependencies are).  Linux Mint 15 
and Ubuntu 13.04 do not ship with SFML 2.0 in their repositories.  If you are using 
either of those distros, you will probably have to compile it yourself.

Currently there is not support for ROM images larger than 32 KB.  There is also not 
yet support for save data (though ROM images that are 32 KB tend to not have save 
data anyway).

Also, the filename of the ROM to be loaded must be game.gb.  I hardcoded it so as to 
avoid messing around with command line arguments.  On Linux, not having a file called 
game.gb in the directory where this thing is being launched causes a segmentation 
fault.  

There are currently CPU bugs that make it impossible to run most 32 KB ROMs.  
However, I have included a test ROM that DOES work.  It is called testrom.  Rename it 
to game.gb if you wish to run it.
