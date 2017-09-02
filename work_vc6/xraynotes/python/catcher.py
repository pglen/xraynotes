import xnt, sys

# In order to see messages with the print statement we need to redirect
# stdout to the GUI edit box. It is also a good practice, because it
# allows one to see unhandled exceptions and messages


# ---------------------------------------------------------------
def softload(file, verbose = False):

    """Open file only if there is none open with this name"""
    lst = xnt.list();
    found = False
    for num,str in lst:
        if file == str:
            #print "found fname in list"
            found = True
            break

    if not found:
        xnt.load(file)
    else:
        if verbose:
            print "File: '%s' already loaded." % file


#xnt.msgbox(__name__)

class StdoutCatcher:
    def __init__(self):
        self.data = ''

    def write(self, stuff):
        try:
            self.data = self.data + stuff
            xnt.printx(str(stuff));

        except:
            xnt.msgbox("Cannot write to GUI out")

sys.stdout = StdoutCatcher()

#if __name__ == '__main__':
#    xnt.msgbox("Running as main")
#print "Running catcher"



