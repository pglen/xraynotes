
# XrayNotes Example Script. You may Modify and redistribute this without
# restriction, with the sole exception on identiying it as XrayNotes Script
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#

# The XNT environment will pre-load the 'xnt' module

import sys, xnt, catcher, sqlite3

print 'XrayNotes database Demo Test'

con = sqlite3.connect(r"c:\xraynotes\db\address.sqlt")
cur = con.cursor()

buffer = "PRAGMA table_info(address)"
cur.execute(buffer)

row2 = ()

# Save results:
for row in cur:
    row2 += (row,)

#buffer = "select * from address where lastname = 'Mike'"
buffer = "select * from address "

recs = 0

try:
    cur.execute(buffer)

    for row in cur:
        # Pair it up
        [row3] = (row,)
        loop = 0
        for row4 in row2:
            if "Name" in row4[1]:
                print "%s: '%s'  " % (row4[1],  row3[loop] ),
            loop += 1
        print
        recs += 1;

    print recs, "records shown."

except sqlite3.Error, e:
    print "An error occurred:", e.args[0]
buffer = ""

con.close()



