
# XrayNotes Example Script. You may Modify and redistribute this without
# restriction, with the sole exception on identiying it as XrayNotes Script

# The XNT environment will pre-load the 'xnt' module

import sys

# In order to see messages with the print statement we need to redirect
# stdout to the GUI edit box. It is also a good practice to allow
# one to see syntax errors and unhandled exceptions and other messages.
# This code is also available and loadable as a module called 'catcher'

class StdoutCatcher:
    def __init__(self):
        self.data = ''

    def write(self, stuff):
        try:
            self.data = self.data + stuff
            xnt.printx(stuff);
        except:
            xnt.printx("exception in catcher");

sys.stdout = StdoutCatcher()


print
print 'Contents of XrayNotes Python sys:'
print

cnt = 1
for vv in dir(sys):
    if cnt % 8 == 0:
        print
    print " %s " % (str(vv)),
    cnt += 1

print
print

cnt = 1
for vv in dir(sys):
    if cnt % 8 == 0:
        print

    print " %s " % (vv),
    cnt += 1

print
print


