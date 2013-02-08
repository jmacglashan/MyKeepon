#pragma once

#include "ofRectangle.h"
#include "ofxUI.h"

class MyKeeponControlPanel{
	public:
		MyKeeponControlPanel(const ofVec2f p);
		~MyKeeponControlPanel();
		void update();
		void guiListener(ofxUIEventArgs &args);
		const bool& toDelete() const;
		void setX(const int x_);
		const ofRectangle getRectangle();
	private:
		ofxUICanvas mGui;
		ofxUIDropDownList* mDDList;
		ofSerial mSerial;
		bool bDelete, bUpdateSerialList;
		static vector<string> theSerials;
		static vector<string>& updateSerialList();
		// put the dance state stuff here too
};
