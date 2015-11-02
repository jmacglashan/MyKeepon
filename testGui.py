__author__ = 'alerus'

from Tkinter import *



def hello():
    v.set('Changed you!')



root = Tk()
root.geometry('600x600')

logo = PhotoImage(file="keeponImage.gif")
w1 = Label(root, image=logo).pack(side="top")
explanation = 'At present, only GIF and PPM/PGM formats are supported, but an ' \
              'interface exists to allow additional image file formats to be added easily.'
v = StringVar()
v.set(explanation)
w2 = Label(root,
           textvariable=v,
           wraplength=580
           )

w2.pack(side="bottom")



button = Button(root, text='Press Me', command=hello).pack(side='bottom')





root.mainloop()


