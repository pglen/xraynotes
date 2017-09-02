import xnt, sys, catcher

# -------------------------------------------------------------------------
print
print "**** Methods in sys:"
print

cnt = 1
for vv in dir(sys):
    if cnt % 8 == 0:
        print
    print " %s " % (str(vv)),
    cnt += 1

print
print

# -------------------------------------------------------------------------

print "**** Doc of sys:"
print

print sys.__doc__


