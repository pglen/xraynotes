import xnt, sys, catcher, time

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



# ---------------------------------------------------------------
def exitfunc():

    """Execute code at script exit and script abort."""

    print "Exit function to restore tool windows ..."
    xnt.suppshow(1)

# ---------------------------------------------------------------
def hiressleep(sec):

    """Sleep with the machine mostly alive"""

    resolution = 15
    global time
    total = 0.0
    while True:
        time.sleep(float(sec)/resolution)
        # Call nice to allow the application to process msgs
        xnt.nice()
        # Timeout?
        total += float(sec)/resolution
        if total > sec:
            break

# ---------------------------------------------------------------
timeout = 1;
count = 1;

sys.exitfunc = exitfunc
print "Number of docs already loaded: %d " % (xnt.numdocs())

softload("C:\\images\\8-24\\DSCF0001.JPG", True)
softload("C:\\images\\08-03-2007\\IMG_0113.jpg", True)

xnt.mindlg();
xnt.suppshow(0)

while True:
    hiressleep(timeout)
    xnt.nextdoc()
    xnt.message("Iteration %s" % count)
    print "Iteration: ", count
    count += 1
    if count > 10:
        break;

xnt.maxdlg();
exitfunc()

