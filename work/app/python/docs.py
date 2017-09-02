import xnt, sys, catcher

print "Number of docs:", xnt.numdocs();
print "List of docs:", xnt.list();

try:
    xnt.setactive(0);
except:
    print "Exception",  sys.exc_info()

