#!/usr/bin/python

import sys
import socket
from speechAndMovement import RobotAndSpeechController
from myKeeponController import MyKeepon


def receiveLopp(rs, c):
    while True:
        msg = c.recv(1024)
        if len(msg) > 0:
            rs.say(msg)


def main():
    if len(sys.argv) != 2:
        print 'requires port_number'
        sys.exit(0)

    s = socket.socket()  # Create a socket object
    host = socket.gethostname()  # Get local machine name
    host = 'localhost'  # Do this if we are running only on localhost and not visible to the world
    port = int(sys.argv[1])  # Reserve a port for your service.
    s.bind((host, port))  # Bind to the port

    rs = RobotAndSpeechController(MyKeepon())

    print 'i am', host
    s.listen(5)  # Now wait for client connection.
    while True:
        c, addr = s.accept()  # Establish connection with client.
        print 'Got connection from', addr
        receiveLopp(rs, c)
        c.close()


if __name__ == "__main__":
    main()
