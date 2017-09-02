
# XrayNotes Example Script. You may Modify and redistribute this without
# restriction, with the sole exception on identiying it as XrayNotes Script
# The XNT environment will pre-load the 'xnt' module

import sys, xnt, catcher, array

print 'XrayNotes Moire'

catcher.softload("C:\\images\\8-24\\DSCF0001.JPG")

buff = xnt.buffer()
info = xnt.imginfo()

# --------------------------------------------------------------------
# Put moire onto the image

xarr = [];

# Generate a gray DOT
xarr.append(chr(128))
xarr.append(chr(128))
xarr.append(chr(128))

buff2 = buffer(array.array("c", xarr ));

loop = 0;  start =  0;  row = 0

# Scan lines
while loop < info[1] - 3:
    # Inner loop, scan pixels
    loop2 = 0;
    while loop2 < info[2] * 3:
        if loop2 % 6 == 0:
            offset = start + loop * info[2] + loop2
            if loop % 2 == 0:
                offset += 6
            buff[ offset : offset + len(buff2)] = buff2
        loop2 += 1

    # print progress indicator
    if loop % 200 == 0:
        print
        print 'Processing ... ',
    if loop % 20 == 0:
        print loop,

    loop += 1

xnt.inval()

print
print "EOScript"

