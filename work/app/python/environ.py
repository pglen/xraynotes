
# XrayNotes Example Script. You may Modify and redistribute this without
# restriction, with the sole exception on identiying it as XrayNotes Script
# The XNT environment will pre-load the 'xnt' module

import sys, xnt, catcher, os

print 'XrayNotes Environment'

#print os.environ

for aa in os.environ:
	print  aa, "=>", os.environ[aa]





