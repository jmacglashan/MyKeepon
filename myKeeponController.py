#!/usr/bin/python

import serial
import time
import threading

# /dev/cu.usbmodem1421
# /dev/tty.usbmodemfa141    <- this is the old tty
class MyKeepon:
    def __init__(self, tty='/dev/cu.usbmodem1421', baud=115200, receiver=None):
        self.open()
        # self.recThread = MyKeeponReceiverThread(self.ser, receiver)
        # self.recThread.start()

    def open(self, tty='/dev/cu.usbmodem1411', baud=115200):
        self.ser = serial.Serial(tty, baud)

    def close(self):
        # self.recThread.shouldRun = False
        # self.recThread.join()
        self.ser.close()

    def write(self, command):
        # self.open()
        if not command.endswith(';'):
            command = command + ';'
        try:
            self.ser.write(command)
            # time.sleep(0.1)
            # self.ser.flushOutput()
            # self.close()
        except:
            print 'Exception!'

    def moveUp(self):
        self.write('MOVE PON UP')

    def moveDown(self):
        self.write('MOVE PON DOWN')

    def moveHalfUp(self):
        self.write('MOVE PON HALFUP')

    def moveHalfDown(self):
        self.write('MOVE PON HALFDOWN')

    def movePan(self, delta):
        if delta < -100 or delta > 100:
            print 'Error, pan delta must be between -100 and +100 (inclusively); entered ' + str(delta)
        else:
            self.write('MOVE PAN ' + str(delta))

    def moveTilt(self, delta):
        if delta < -100 or delta > 100:
            print 'Error, tilt delta must be between -100 and +100 (inclusively); entered ' + str(delta)
        else:
            self.write('MOVE TILT ' + str(delta))

    def moveSideCycle(self):
        self.write('MOVE SIDE CYCLE')

    def moveSideCenterFromLeft(self):
        self.write('MOVE SIDE CENTERFROMLEFT')

    def moveSideRight(self):
        self.write('MOVE SIDE RIGHT')

    def moveSideCenterFromRight(self):
        self.write('MOVE SIDE CENTERFROMRIGHT')

    def moveSideLeft(self):
        self.write('MOVE SIDE LEFT')

    def playSoundFor(self, sound, duration):
        if sound < 0 or sound > 63:
            print 'Error, sound id must be between 0 and 63 (inclusively); entered ' + str(sound)
        else:
            self.write('SOUND REPEAT ' + str(sound))
            time.sleep(duration)
            self.write('SOUND STOP')


class MyKeeponReceiverThread(threading.Thread):
    def __init__(self, ser, receiver=None):
        threading.Thread.__init__(self)
        self.ser = ser
        if receiver == None:
            # self.receiver = MyKeeonReceiverLogger()
            self.receiver = MyKeeponReceiver()
        else:
            self.receiver = receiver
        self.receiver.setSerial(ser)
        self.shouldRun = True

    def run(self):
        while self.shouldRun:
            rec = self.ser.readline().strip()
            comps = rec.split(' ')
            if comps[0] == 'BUTTON':
                if comps[1] == 'HEAD':
                    if comps[2] == 'ON':
                        self.receiver.buttonHeadOn()
                    else:
                        self.receiver.buttonHeadOff()

                elif comps[1] == 'FRONT':
                    if comps[2] == 'ON':
                        self.receiver.buttonFrontOn()
                    else:
                        self.receiver.buttonFrontOff()

                elif comps[1] == 'BACK':
                    if comps[2] == 'ON':
                        self.receiver.buttonBackOn()
                    else:
                        self.receiver.buttonBackOff()

                elif comps[1] == 'RIGHT':
                    if comps[2] == 'ON':
                        self.receiver.buttonRightOn()
                    else:
                        self.receiver.buttonRightOff()

                elif comps[1] == 'LEFT':
                    if comps[2] == 'ON':
                        self.receiver.buttonLeftOn()
                    else:
                        self.receiver.buttonLeftOff()

            elif comps[0] == 'AUDIO':
                if comps[1] == 'RANGE':
                    self.receiver.audioRange(int(comps[2]))
                elif comps[1] == 'MEAN':
                    self.receiver.audioMean(int(comps[2]))


class MyKeeponReceiver:
    def __init__(self):
        self.ser = None

    def setSerial(self, ser):
        self.ser = ser

    def run(self):
        pass

    def buttonHeadOn(self):
        pass

    def buttonFrontOn(self):
        pass

    def buttonBackOn(self):
        pass

    def buttonRightOn(self):
        pass

    def buttonLeftOn(self):
        pass

    def buttonHeadOff(self):
        pass

    def buttonFrontOff(self):
        pass

    def buttonBackOff(self):
        pass

    def buttonRightOff(self):
        pass

    def buttonLeftOff(self):
        pass

    def audioRange(self, aRange):
        pass

    def audioMean(self, aMean):
        pass


class MyKeeonReceiverLogger(MyKeeponReceiver):
    def __init__(self):
        MyKeeponReceiver.__init__(self)

    def buttonHeadOn(self):
        print 'BUTTON HEAD ON'

    def buttonFrontOn(self):
        print 'BUTTON FRONT ON'

    def buttonBackOn(self):
        print 'BUTTON BACK ON'

    def buttonRightOn(self):
        print 'BUTTON RIGHT ON'

    def buttonLeftOn(self):
        print 'BUTTON LEFT ON'

    def buttonHeadOff(self):
        print 'BUTTON HEAD OFF'

    def buttonFrontOff(self):
        print 'BUTTON FRONT OFF'

    def buttonBackOff(self):
        print 'BUTTON BACK OFF'

    def buttonRightOff(self):
        print 'BUTTON RIGHT OFF'

    def buttonLeftOff(self):
        print 'BUTTON LEFT OFF'

    def audioRange(self, aRange):
        print 'AUDIO RANGE', aRange

    def audioMean(self, aMean):
        print 'AUDIO MEAN', aMean
