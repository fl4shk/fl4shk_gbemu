fl4shk_gbemu
============

Incomplete GB (and eventually GBC) emulator.  Currently there is (buggy) CPU 
emulation, interrupt emulation, LCD emulation, OAM DMA functionality, timers, 
and background/window rendering.

Currently there is not support for ROM images larger than 32 KB.  There is also not 
yet support for save data (though ROM images that are 32 KB tend to not have save 
data anyway).

Also, the filename of the ROM to be loaded must be game.gb.  I hardcoded it so as to 
avoid messing around with command line arguments.  On Linux, not having a file called 
game.gb in the directory where this thing is being launched causes a segmentation 
fault.
