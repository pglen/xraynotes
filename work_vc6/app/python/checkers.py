
# XrayNotes Example Script. You may Modify and redistribute this without
# restriction, with the sole exception on identiying it as XrayNotes Script
# The XNT environment will pre-load the 'xnt' module

import sys, xnt, catcher
import array

# --------------------------------------------------------------------
# Put a checkers pattern onto the current image

print 'XrayNotes Checkers Test'

catcher.softload("C:\\images\\8-24\\DSCF0001.JPG")

buff = xnt.buffer()
print "Buffer type:", type(buff);
print "Buffer length:", len(buff);

info = xnt.imginfo()
print "Image dimensions:", info
print "Image dim check: (should be 3 bytes per pixel) ", len(buff) / (info[0] * info[1])

xarr = []; loop = 0; slices  = 8;

# Generate a gray scanline:
while loop < info[2] / (3*2 * slices):  # RGB * HALF FILL
    xarr.append(chr(128))
    xarr.append(chr(128))
    xarr.append(chr(128))
    loop += 1
buff2 = buffer(array.array("c", xarr ));

loop = 0; start =  0;  row = 0;  inc = info[2] / slices;

# Draw Checkers
while loop < len(buff) - 2 * len(buff2):
    offset = start + loop
    # Shift only at xx lines
    if ((loop / info[2]) / (inc/6)) % 2 == 0:
        offset += inc / 2;
    buff[ offset : offset + len(buff2)] = buff2
    loop += inc

xnt.inval()

