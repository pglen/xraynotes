XrayNotes Plugin 

Convert current image to Grayscale.

This DLL uses the macro

// This is the offical way to gray it (rounded)
#define RGB2GRAY(r,g,b) ((((int)b)*100 + ((int)g)*600 + ((int)r)*300) >> 10)

See plugbw2 for an alternative gray scale routine.

