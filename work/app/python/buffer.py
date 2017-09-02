
# XrayNotes Example Script. You may Modify and redistribute this without
# restriction, with the sole exception on identiying it as XrayNotes Script
# The XNT environment will pre-load the 'xnt' module

import sys, xnt, catcher, array

print 'XrayNotes buffer test'

catcher.softload("C:\\images\\8-24\\DSCF0001.JPG")

buff = xnt.buffer()
print "Buffer type:", type(buff), "Buffer length:", len(buff)

info = xnt.imginfo()
print "Image dimensions:", info

# --------------------------------------------------------------------
# Put moire onto the image

xarr = []; loop = 0; slices = 10; hslices = slices

# Generate a gray DOT
xarr.append(chr(128))
xarr.append(chr(128))
xarr.append(chr(128))

buff2 = buffer(array.array("c", xarr ));

#mainarr = buffer(array.array("c", buff ));

loop = 0;  start =  0;  row = 0

# Scan lines
while loop < info[1] - 2:
    # Inner loop, scan one line
    loop2 = 0;
    while loop2 < info[2] * 3:
        if loop2 % 6 == 0:
            offset = start + loop * info[2] + loop2
            if loop % 2 == 0:
                offset += 6
            buff[ offset : offset + len(buff2)] = buff2
        loop2 += 1
    loop += 1
    # print progress indicator
    if loop % 20 == 0:
        print loop,
    if loop % 200 == 0:
        print

xnt.inval()

print "EOScript"

