
# XrayNotes Example Script. You may Modify and redistribute this without
# restriction, with the sole exception on identiying it as XrayNotes Script
# The XNT environment will pre-load the 'xnt' module

import sys, xnt, catcher, array

print 'XrayNotes buffer moire test'

catcher.softload("C:\\images\\8-24\\DSCF0001.JPG")

buff = xnt.buffer()
info = xnt.imginfo()

# --------------------------------------------------------------------
# Put moire onto the image

xarr = []; loop = 0; slices = 10; hslices = slices

# Generate a gray DOT
xarr.append(chr(128))
xarr.append(chr(128))
xarr.append(chr(128))

buff2 = buffer(array.array("c", xarr ));
print 'Acquire image buffer from DOC ... ',
mainarr = array.array("c", buff );
print "OK"

loop = 0;  start =  0;  row = 0

def mulpix(val2):
    val2 *= 2
    if val2 > 255:
        val2 = 255
    return val2

def divpix(val2):
    val2 /= 2
    return val2

# Scan lines
while loop < info[1] - 3:

    # Inner loop, scan one line
    loop2 = 0;
    while loop2 < info[2] * 3:
        if loop2 % 12 == 0:
            offset = start + loop * info[2] + loop2
            if loop % 2 == 0:
                offset += 6
            mainarr[offset + 0] = chr( divpix( ord(mainarr[offset + 0]) ) )
            mainarr[offset + 1] = chr( divpix( ord(mainarr[offset + 1]) ) )
            mainarr[offset + 2] = chr( divpix( ord(mainarr[offset + 2]) ) )
        loop2 += 3

    # print progress indicator
    if loop % 200 == 0:
        print
        print 'Processing ... ',
    if loop % 20 == 0:
        print loop,

    loop += 1

print 'Put image buffer to  DOC ... ',
buff[0:len(buff)] = mainarr[0:len(mainarr)]
print "OK"

xnt.inval()

print
print "EOScript"

