import xnt, sys, catcher

xnt.clear()

# -------------------------------------------------------------------------
print
print "**** Methods in xnt:"
print

cnt = 1
for vv in dir(xnt):
    if cnt % 8 == 0:
        print
    print " %s " % (str(vv)),
    cnt += 1

print
print

# -------------------------------------------------------------------------

print "**** Doc of xnt:"
print

print xnt.__doc__




