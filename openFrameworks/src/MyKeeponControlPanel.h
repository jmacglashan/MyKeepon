#pragma once

#include "ofRectangle.h"
#include "ofxUI.h"
#include <set>

class MyKeeponControlPanel{
	public:
		MyKeeponControlPanel(const ofVec2f p);
		~MyKeeponControlPanel();
		void update();
		void guiListener(ofxUIEventArgs &args);
		const bool& toDelete() const;
		void setX(const int x_);
		const ofRectangle getRectangle();
		struct Values {
			float pan = 0.5, tilt = 0.5;
			float panSpeed = 0.5, tiltSpeed = 0.5, sideSpeed = 0.5;
		};
		struct DanceValues {
			bool pan[3] = {false,false,false};
			bool tilt[3] = {false,false,false};
			bool side[3] = {false,false,false};
			float speed = 0.5;
		};
	private:
		// GUI handlers
		ofxUICanvas mGui;
		ofxUIDropDownList* mSerialList;
		ofxUI2DPad* m2DPad;
		ofxUISlider *mPanSpeed, *mTiltSpeed, *mSideSpeed;
		ofxUIToggleMatrix *mPanDance, *mTiltDance, *mSideDance;
		// Serial
		ofSerial mSerial;
		// bools
		bool bDelete, bUpdateSerialList, bSerialInited, bIsSync, bUpdateGuiFromValues;
		Values mValues;
		// helpers
		void sendPanAndTilt();
		void sendPanSpeed();
		void sendTiltSpeed();
		void sendSideSpeed();
		// dance state variables
		DanceValues mDanceValues;
		// statics
		static vector<string> theSerials;
		static vector<string>& updateSerialList();
		static Values syncValues;
		static DanceValues syncDanceValues;
		static set<MyKeeponControlPanel*> theSyncPanels;
		static void sendSyncPanAndTilt();
		static void sendSyncPanSpeed();
		static void sendSyncTiltSpeed();
		static void sendSyncSideSpeed();
};
