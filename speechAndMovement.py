#!/usr/bin/python

import time
import threading
import os
import random


class RobotAndSpeechController:
    def __init__(self, k, movementHandler=None):
        self.k = k
        self.isRunning = False
        self.voice = SpeechVoiceHandler()
        if movementHandler is None:
            self.movement = SpeechMovementHandler(k)
        else:
            self.movement = movementHandler

    def say(self, text):
        self.voice.text = text
        self.movement.text = text

        vt = ThreadWrapper(self.voice)

        if self.k is not None:
            mt = ThreadWrapper(self.movement)

        vt.start()

        if self.k is not None:
            mt.start()

        vt.join()
        self.movement.continueRunning = False
        if self.k is not None:
            mt.join()


class ThreadWrapper(threading.Thread):
    def __init__(self, threadController):
        threading.Thread.__init__(self)
        self.threadController = threadController

    def run(self):
        self.threadController.run()


class SpeechVoiceHandler:
    def __init__(self):
        self.text = ''

    def run(self):
        nt = self.text
        if not nt.startswith('"'):
            nt = '"' + nt + '"'
        os.system('say ' + nt)


class SpeechMovementHandler:
    def __init__(self, k):
        self.text = ''
        self.k = k
        self.tilt = 0
        self.side = 1
        self.continueRunning = False
        self.sleepTime = 1.5

    def run(self):
        self.continueRunning = True
        while self.continueRunning:
            # choose a movement type
            r = random.randint(0, 1)
            if r == 0:
                # do side
                validSides = [0, 1, 2]
                validSides.remove(self.side)
                newSide = random.sample(validSides, 1)[0]
                self.moveSide(newSide)
            else:
                # do tilt
                validTilts = [-50, 0, 50]
                validTilts.remove(self.tilt)
                newTilt = random.sample(validTilts, 1)[0]
                self.moveTilt(newTilt)
        self.movebackToNeutral()

    def moveSide(self, side):
        if side == 0:
            self.k.moveSideLeft()
        elif side == 1:
            if self.side == 0:
                self.k.moveSideCenterFromLeft()
            else:
                self.k.moveSideCenterFromRight()

        else:
            self.k.moveSideRight()

        time.sleep(self.sleepTime)

        self.side = side

    def moveTilt(self, tilt):
        self.k.moveTilt(tilt)
        time.sleep(self.sleepTime)
        self.tilt = tilt

    def movebackToNeutral(self):
        self.moveSide(1)
        self.moveTilt(0)
