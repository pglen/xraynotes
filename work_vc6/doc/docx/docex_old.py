#!/usr/bin/env python

import sys, os, string, re, glob, tree
from datetime import *

"""
This is source code filter. Embedded in code the '//-' comment
line denotes filterable items. Will write HTML from parsed data.

Files:

    head.html   -- if found in current dir will use as header
    foot.html   -- if found in current dir will use as footer
    
    *.cpp       -- if no comline argument found, assumes this as default
    
Commands:

Page extraction syntax:

\page [pagename] [title]

    Stores all entries found after this heading in the 'pagename' file.
        .html suffix is automatically appended.
    If no file specified, deaults to "misc.html"
    If no title specified, defaults to 'pagename'

\context [cxname] [title]

    Accumulates entries after this heading to the current page
    under 'cxname' section, grouped by 'cxname'.
    If no name specified, "defcontext" is assumed.
    If no title specified, blank string is assumed   

\table [tname]

    Accumulates entries after this heading to the current table
    under the 'tname' table. '\table' with no name ends table
    accumulation. Table accumulation is also suspended with ecountering
    'context' or 'page' keywords. Table lines are parsed as rows.
    Column break is inserted if double space is encountered.
    (The column break replaces the double space)

Substitutions:

    <tablehere name> substitute named table with this string entry
    <linkhere pagename [linktitle]> crate a link to a generated page

    Any other strings (including html) is passed verbatim to the current
    html file.

Unaccumulated filterable entries are collected in the file 'misc.html'.

"""

marker = "//-"     # marks a collectable comment

#LF = '\x0a'
#total = 0

# --------------------------------------------------------------
# Configure it here:

htmldir = "html\\"
cppdir = "..\\app"

# --------------------------------------------------------------
# Global tree nodes

root = tree.Node("root")
troot = tree.Node("troot")

# --------------------------------------------------------------
# Parsable items

page    = "\\page"
context = "\\context"
table   = "\\table"

tablehere = "<tablehere "
linkhere = "<linkhere "

parsable = [page, context, table]

defpage     = "misc.html"
defcontext  = "defcontext"
deftable    = ""

lastpage = defpage
lastcontext = defcontext
lasttable = deftable          

currdir = ""

# --------------------------------------------------------------
def main ():

    global currdir, defpage, defcontext, deftable

    #print "Source code filter"    
    currdir = os.getcwd() + "\\";   #print currdir
    
    #os.chdir ("..\\app")

    # Create default file name/header
    root.getnode(defpage).additem("Misc. Collected Items")

    # Create empty header for default file default context
    root.getnode(defpage).getnode(defcontext). \
                    getnode("Header").additem("")
                
    cpp_files = glob.glob('*.cpp')

    for fn in cpp_files:    
        if fn == "pageless.cpp":
            dofile(fn)

    # These are left in for the curious:
    #print "parse tree: "
    #for dd in tree.recurslist(root):
    #   print dd.path()
    #print "parse tree ends"
        
    #print;   print "parse tree data: "
    #for dd in tree.recursall(root):
    #    print dd
    #print "parse tree ends"

    #print;   print "table tree data: "
    #for dd in tree.recursall(troot):
    #    print dd
    #print "table tree ends"

    emitfiles()
   
#endof main
    
# enclose string in quotes for easy printing
def qt(instr):    return "'" + instr + "'"

# process one file 
def dofile(fname):

    print "Processing " + fname

    #f = open("shadesdlg.cpp")
    ff = open(fname)   
    try:
        for line in ff:
            idx = line.find(marker)
            if idx >= 0:
                parseline (line[idx + len(marker):])
    finally:
        ff.close()

def     parseline(line):
    
    global root, lastpage, lastcontext, lasttable
    
    line = line.strip()     
    if getheader(parsable, line) == 0:
        if lasttable != "":
            # convert tabs into <td>
            line = "<tr><td>" + line.replace("  ", "<td>")
            troot.getnode(lasttable).additem(line)
        else:
            root.getnode(lastpage).getnode(lastcontext).additem(line)
        
def     getheader (par, line):

    global root, lastpage, lastcontext, lasttable
    global defpage, defcontext, deftable
    
    #print "getheader line: " + qt(line)
    
    ret = 0
    for ppp in par:
        idx = line.find(ppp)
        if idx >= 0:
            #print "found line: " + qt(line)
            qt(line[idx+len(ppp):])
            
            com = line.split(" ", 2)
            
            # page?
            # ---------------------------------------------------------                
            if com[0] == par[0]:
                if len(com) < 2:
                    # Page with no title
                    com.append(defpage )
                    com.append("Misc. Items Accumulated")
                elif len(com) < 3:
                    com.append(com[1])

                if not com[1].endswith(".html"):
                    com[1] += ".html"

                com[1] = com[1].lower()

                lastpage = com[1]
                lastcontext = defcontext

                # create empty header for default context
                root.getnode(lastpage).getnode(lastcontext). \
                    getnode("Header").additem("")
                
                root.getnode(com[1]).additem(com[2])
         
            # ---------------------------------------------------------                
            # context?
            if com[0] == par[1]:
                if len(com) < 2:
                    # context with no name
                    com.append(defcontext )
                    
                # context with no title
                if len(com) < 3:
                    com.append("")

                root.getnode(lastpage).getnode(com[1]). \
                    getnode("Header").additem(com[2])
                
                #  Save last context
                lastcontext = com[1]
                lasttable = ""

            # ---------------------------------------------------------                
            # table?
            if com[0] == par[2]:
                if len(com) < 2:
                    lasttable = ""
                else:
                    # create blank holder, add options if any
                    if len(com) < 3:
                        com.append(" border=1 align=center width=800 ")
                                   
                    troot.getnode(com[1]).getnode("Options"). \
                                additem(com[2])
                    lasttable = com[1]
                                                
            ret  = 1
            
    return ret    

def  emitfiles():

    global tree, currdir, htmldir

    # pre load global header/footer
    hhfp = open(currdir + "head.html")
    head = hhfp.read(); hhfp.close()   #print head

    ff = open(currdir +  "foot.html")
    foot = ff.read(); ff.close()   #print foot

    dt = datetime.now();
    timestr = '%02d %02d/%02d %02d:%02d:%02d' \
           %  (dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second)
    foot = foot.replace("$datetime", timestr)

    for dd in tree.folderlist(root):

        print "  Emitting data for: " + dd.name()
        
        hhh = open(currdir + "\\" + htmldir + dd.name(), "w+")
        hhh.write(head)       
        
        #print "File Header = " + dd.items[0]
        hhh.write("<h1>" + dd.items[0]+ "</h1>");

        coll = 0;
        for ee in tree.folderlist(dd):
            
            #print     "    context: " + qt(ee.name())
            
            # Write title for context:
            hx = ee.getnode("Header").items[0]          
            if len(hx) > 0:
                hhh.write("<h3>" + hx + "</h3>");
                
            for ff in tree.itemlist(ee):
                #print "        item: " + ff
                ff = subtable(ff)        
                ff = sublink(ff)                
                hhh.write(ff + "\n")
                coll += 1;
                
        if coll == 0:
            hhh.write("No contents found for this file")
            
        hhh.write(foot)
        hhh.close

# Substitite table if found
def subtable(line):

    idx = line.find(tablehere)
    if idx  >= 0:
        idx2 = line.find( ">", idx)
        if idx2  >= 0:                          
            tname = line[idx + len(tablehere):idx2]
            tname.strip()
            #print "table sub:  " + qt(tname)
            xtable = tableassembly(tname)
            line = line[:idx] + xtable +  line[idx2 + 1:]
            
    return line

# Return table contents or error string
def tableassembly(tname):

    global troot

    # Does this node (...uhm...table) exist?
    if troot.getnode(tname, 0) == 0:
        #print "No such table " + tname
        xtable = "<p><table border=1 bgcolor=#ffdddd align=center>"\
                 "<tr><td>DocX warning: Non existent table " \
                 "substitution was attempted. <br>" \
                 "Attempted table name: '%s' </table>"  % tname
    else:
        #print "table assembly: " + tname
        xtable = "<p><table "
        opt = troot.getnode(tname).\
                getnode("Options").items[0]
        xtable += opt + ">"
        for rr in tree.itemlist(troot.getnode(tname)):
            xtable += rr + "\\n";
        xtable += "</table>"
    return xtable

# Substitite link if found
def  sublink(line):

    idx = line.find(linkhere)
    if idx  >= 0:
        idx2 = line.find( ">", idx)
        if idx2  >= 0:                          
            # substitute link if any
            lname = line[idx + len(linkhere):idx2]
            com = lname.split(" ", 1)
            if len(com) < 2:
                com.append(com[0])
               
            com[0] = com[0].strip().lower() + ".html"
            #print "linkhere: " , com
            line = line[:idx] + "<a href=" + com[0] + ">" +\
                   com[1] + "</a>"  + line[idx2 + 1:] 
    return line
                      
if __name__ == '__main__':
    main ()
