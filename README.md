fl4shk_gbemu
============

Incomplete GB (and eventually GBC) emulator.  Currently there is CPU emulation
(not counting bugs it should be complete), interrupt emulation, LCD emulation 
(which might also contain bugs), and OAM DMA functionality.

For graphics rendering and sound playing, I will be using SFML 2.0 (that means
it's a dependency!).

Currently there is not support for ROM images larger than 32 KB.  There is 
also not yet support for save data (though most ROM images that are 32 KB tend
to not have save data anyway).
