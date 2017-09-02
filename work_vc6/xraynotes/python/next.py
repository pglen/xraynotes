import  xnt, sys, catcher, time

def nextfunc():

    global time
    timeout = 1;

    if xnt.numdocs() < 2:
        print "Cannot play, less than 2 documents loaded."
        return

    #xnt.mindlg();
    xnt.message("Running next doc script, three 1 second iterations")

    #print "Minimize xnt dialog"
    xnt.mindlg()
    xnt.suppshow(0)

    time.sleep(timeout)
    print "Switching to next doc"
    xnt.nextdoc()
    time.sleep(timeout)

    xnt.nextdoc()
    xnt.message("Iteration 1")
    time.sleep(timeout)

    #xnt.nextdoc()
    #xnt.message("Iteration 2")
    #time.sleep(timeout)

    #xnt.prevdoc()
    #xnt.message("Iteration 3")
    #time.sleep(timeout)

    print "Maximize xnt dialog"
    xnt.maxdlg();
    xnt.suppshow(1)

nextfunc()




