from Tkinter import *
import xnt, sys, catcher

# The XrayNotes Environment does not set this up:
sys.argv = "XrayNotes"

print "Hello  firsttgui"

def press2(self, event):
	print "Button Pressed"
	xnt.msgbox("ww")

root = Tk()

#w = Label(root, text="Hello, world!")
#w.pack()

b = Button(root, text="OK")
b["command"] = "press2"
b["fg"] = "black"
b.pack()

root.mainloop()

