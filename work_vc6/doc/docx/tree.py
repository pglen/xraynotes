#!/usr/bin/env python

"""
Module: Tree.
Create a tree structure, as simple as possible.
"""

#"""This is a tree data test"""

import sys, os, string, re, glob

LF = '\x0a'
total = 0

lastnode = 0
 
class Node:  
    """
    Class Node. This class repredents a node in the tree with
    branches (folders) and leaves (items). Folders are containers
    for more nodes. Multiple folders and items can be stored in one node.

    Entry points summary:
    
        addfolder(str|node) -- to add a folder (dir, brancy)
        additem(str)        -- to add an item (file, leaf)
        getroot()           -- to get root node of node
        getnode(str)        -- get node, create if not there
        path()              -- get path of current node
        name()              -- get current name

        Access to internals:

        parent      -- parent node, self==parent when root
        _name       -- name of the node
        items       -- list of items (files, leaves)
        folders     -- list of folders (dirs, branches)
        
    Iterator(s):
    
        Will walk the path upwards until encounters root
        
    Generators:
    
        itemlist        --  items in the node
        folderlist      --  folders in the node
        recurslist      --  folders below node
        recursall       --  everyting below node

        recursall returns full paths suitable for listing the whole tree
        
    """

    # Note: Create a copy of params
    def __init__(self, xname = "", xitem = [], xfolder = []):
        
        self._name = str(xname);
        self.items = xitem[:]
        self.folders = xfolder[:]
        self.parent = self
        
    def __repr__(self):
        return "Node Class"
    
    def __str__(self):
        return "name: '" + self.name() + "' items=(" + str(self.items) + \
               ")  " + "folders=(" + str(self.folders) + ")"

    "Add a new folder to the current node"
    def addfolder(self, folder):
        global lastnode;
        if isinstance(folder, str):
            folder2 = Node(folder)
        elif isinstance(folder, Node):
            folder2 = folder
        else:
            raise TypeError, "Invalid type to Node::addfolder"         
        self.folders.append(folder2)
        
        # this is done to speed up access to the last added
        lastnode = self.lastnode = self.folders[len(self.folders) - 1]
        folder2.parent = self
        return self.lastnode

    "Get node, create if not there"
    def getnode(self, nnn, add = 1):
        if not isinstance(nnn, str):
            raise TypeError, "Invalid type to Node::getnode"
    
        for aa in self.folders:
            if(aa.name() == nnn):
                return aa
        # If it did not find it, add new
        if add:
            return self.addfolder(Node(nnn))
        # Will raise exception
        return 0

    #" get node from path"
    #def getpathnode(self, fff, add = 1):
    #   ppp = self.path()
        
    "Add a new item to the current node"
    def additem(self, item):
        self.items.append(item)
        return self;

    " Get the path of current node"
    def path(self):
        xself = self;  ppp = ""
        # Descend until self==parent (root)
        while True:
            ppp = "/" + xself.name() + ppp
            if xself == xself.parent:
                break;
            xself = xself.parent
        return ppp

    " Get the root node from this path"
    def getroot(self):
        xself = self;
        # Descend until self==parent (root)
        while True:
            xself = xself.parent
            if xself == xself.parent:
                break
        return xself

    def name(self):
        return self._name;
    
    def __iter__(self):
        self.index = self
        self.last = False
        return self
    
    def next(self):
        if self.last:
            raise StopIteration
        # Signal this as the last one:
        if self.index == self.index.parent:
            self.last = True           
        oldindex = self.index
        self.index = self.index.parent
        return oldindex

    def itemlist(self):
        for aa in self.items:
            yield aa

    def folderlist(self):
        for aa in self.folders:
            yield aa
        
# end class

# ------------------------------------------------------------
# Internal: Generate complete tree list into var

def __genrec(tree, var):
    var.append(tree);
    for aa in tree.folders:
        __genrec(aa, var)

# ------------------------------------------------------------
# Generators for dirs and items

def itemlist(tree):
    for aa in tree.items:
        yield aa
    
def folderlist(tree):
    for aa in tree.folders:
        yield aa
        
def recurslist(tree):
    var = [];
    __genrec(tree, var)
    for aa in var:
        yield aa

def recursall(tree):
    var = [];
    __genrec(tree, var)
    for aa in var:
        for bb in itemlist(aa):
            yield aa.path() + "/" + bb
        
# ------------------------------------------------------------
# Recursive tree printing

def printtree(tree, level = 0, printitems = 1):
    
    print tree.path()

    if printitems:
        level += 1;
        str2 = " " * (len(tree.parent.path()) + 1)
        for aaa in tree.items:
            print str2 + aaa

    # recursive call
    for aa in tree.folders:
        printtree(aa, level, printitems)  

def printdir(tree):
    print tree.path();
    for aa in folderlist(tree):
        printdir(aa);
        
# -----------------------------------------------------------------

def main ():

    root = Node("root");

    #print "root path=" + root.path()
    #print  dir(root); print
   
    root.additem("item1");    root.additem("item2");
    root.getnode("folder2").additem("item_for_folder2a")
    root.addfolder(Node("folder3"));
    
    #print "lastnode=" , lastnode.path(), root.lastnode.path()

    root.getnode("folder3").additem("item_for_folder3b")
    
    f4 = root.getnode("folder3").addfolder(Node("folder4"))
    root.getnode("folder3").getnode("folder4").additem("item_for_folder4a")

    f5 = root.getnode("folder3").getnode("folder4").addfolder("Folder_for_folder4")
    f5.additem("item_for_folder5a")

    print "f5 path='" + f5.path() + "'"
    print "getroot", f5.getroot().path()
    print "f5 path: '" + f5.path() + "'"
    
    #print "Start printTree: ";
    #print "---------------------------------------------"
    #printtree (root) 
    #print "Start printTree dir: "
    print "---------------------------------------------"
    
    printtree (root, 0, 0)

    print "---------------------------------------------"
    
    print "iterator: "    

    for dd in f5:
        print dd.name()

    print "generator1: "    
    for dd in root.itemlist():
        print dd

    print "generator2: "    
    for dd in root.folderlist():
        print dd.name()
        
    print "generator3: "    
    for dd in recurslist(root):
        print dd.path()
        
        #for ee in itemlist (dd):
        #    print " " * len(dd.parent.path()),ee

    print "generator4: "    
    for dd in recursall(root):
        print dd
    
    print "printdir:"
    printdir(root)
    return
    
if __name__ == '__main__':
    main ()
