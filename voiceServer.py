from Tkinter import *
import socket
import threading
import os


class VoiceApp:

    def __init__(self, port=12345, robotImage='keeponImage.gif'):

        self.root = Tk()
        logo = PhotoImage(file=robotImage)
        w1 = Label(self.root, image=logo).pack(side="top")
        self.varText = StringVar()
        self.varText.set('')
        w2 = Label(self.root,
                   justify=LEFT,
                   textvariable=self.varText,
                   wraplength=580).pack(side="bottom")

        self.root.resizable(width=FALSE, height=FALSE)
        self.root.geometry('600x600')

        NetworkThread(self)

        self.root.mainloop()

    def setText(self, txt):
        self.varText.set(txt)
        nt = txt
        if not nt.startswith('"'):
            nt = '"' + nt + '"'
        os.system('say ' + nt)


class NetworkThread(threading.Thread):
    def __init__(self, app):
        threading.Thread.__init__(self)
        self.app = app
        self.start()

    def run(self):
        self.s = socket.socket()  # Create a socket object
        self.host = socket.gethostname()  # Get local machine name
        self.host = 'localhost'  # Do this if we are running only on localhost and not visible to the world
        self.port = int(sys.argv[1])  # Reserve a port for your service.
        self.s.bind((self.host, self.port))  # Bind to the port

        print 'i am', self.host
        self.s.listen(5)  # Now wait for client connection.

        while True:
            c, addr = self.s.accept()  # Establish connection with client.
            print 'Got connection from', addr
            self.receiveLoop(c)
            c.close()

    def receiveLoop(self, c):
        while True:
            msg = c.recv(1024)
            if len(msg) > 0:
                self.app.setText(msg)



def main():
    app = VoiceApp()


if __name__ == '__main__':
    main()
