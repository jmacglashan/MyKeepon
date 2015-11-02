import threading

__author__ = 'James MacGlashan'

import os
from Tkinter import *

class VoiceGUIHandler:
    def __init__(self, robotImage='keeponImage.gif'):
        self.text = ''
        self.robotImage=robotImage
        self.app = TKApp(robotImage)

    def run(self):
        nt = self.text
        if not nt.startswith('"'):
            nt = '"' + nt + '"'
        os.system('say ' + nt)
        #self.varText.set(self.text)
        self.app.setText(self.text)


class TKApp(threading.Thread):

    def __init__(self, robotImage='keeponImage.gif'):
        threading.Thread.__init__(self)
        self.robotImage=robotImage
        self.varText = None
        self.start()

    def setText(self, text):
        self.varText.set(text)

    def run(self):
        self.root = Tk()
        logo = PhotoImage(file=self.robotImage)
        w1 = Label(self.root, image=logo).pack(side="top")
        self.varText = StringVar()
        self.varText.set('')
        w2 = Label(self.root,
                   justify=LEFT,
                   textvariable=self.varText,
                   wraplength=580).pack(side="bottom")

        self.root.resizable(width=FALSE, height=FALSE)
        self.root.geometry('600x600')

        self.root.mainloop()
