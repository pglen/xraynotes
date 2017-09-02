
# XrayNotes Example Script. You may Modify and redistribute this without
# restriction, with the sole exception on identiying it as XrayNotes Script
# The XNT environment will pre-load the 'xnt' module

import sys, xnt, catcher

print
print 'Contents of XrayNotes dir(sys.path) Python Path:'
print

cnt = 1
for vv in dir(sys.path):
    if cnt % 8 == 0:
        print
    print " %s " % (str(vv)),
    cnt += 1

print
print
print 'Contents of XrayNotes (sys.path) Python Path:'
print

cnt = 1
for vv in sys.path:
    if cnt % 4 == 0:
        print
    print " %s " % (vv),
    cnt += 1

print
print






