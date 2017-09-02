import xnt, sys, catcher, time

def softopen(file, verbose = False):

    """Open file only if there is none open with this name"""

    lst = xnt.list();
    for num,str in lst:
        if file == str:
            break
    else:
        xnt.load(file)

xnt.clear();

str1 = "C:\\images\\8-24\\DSCF0001.JPG"
str2 = "C:\\images\\08-03-2007\\IMG_0113.jpg"

print "Number of docs already in:", xnt.numdocs()

softopen(str1);
softopen(str2);





