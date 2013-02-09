#include "MyKeeponControlPanel.h"

vector<string> MyKeeponControlPanel::theSerials = vector<string>();

vector<string>& MyKeeponControlPanel::updateSerialList(){
	theSerials.clear();
	theSerials.push_back("Refresh List");
	
	ofSerial tSerial;
	vector<ofSerialDeviceInfo> serialList = tSerial.getDeviceList();
	for(int i=0; i<serialList.size(); i++){
		string thisDevicePath = serialList.at(i).getDevicePath();
		theSerials.push_back(thisDevicePath);
	}
	return theSerials;
}

MyKeeponControlPanel::MyKeeponControlPanel(const ofVec2f p):
mGui(p.x,p.y,0,0) {
	bDelete = false;
	bSerialInited = false;

	//////////////// my GUI
	mGui.setFont("verdana.ttf");
	mGui.addWidgetDown(new ofxUILabel("Control Panel", OFX_UI_FONT_MEDIUM));
	mGui.addSpacer(mGui.getRect()->width,4);
	////// Serial Port list
	mDDList = (ofxUIDropDownList *) mGui.addWidgetDown(new ofxUIDropDownList("Serial List", updateSerialList()));
	mDDList->setAutoClose(true);
	////// 2D Pad for Pan/Tilt
	mGui.addWidgetDown(new ofxUI2DPad("Pan/Tilt", ofPoint(-100,100), ofPoint(-100,100), ofPoint(0,0),
									  10*mDDList->getRect()->height, 10*mDDList->getRect()->height));
	///// Motor speeds
	mGui.addWidgetDown(new ofxUISlider("Pan Speed", 0, 255, 128,10*mDDList->getRect()->height,mDDList->getRect()->height));
	mGui.addWidgetDown(new ofxUISlider("Tilt Speed", 0, 255, 128,10*mDDList->getRect()->height,mDDList->getRect()->height));
	mGui.addWidgetDown(new ofxUISlider("PonSide Speed", 0, 255, 128,10*mDDList->getRect()->height,mDDList->getRect()->height));

	// synch button
	mGui.addWidgetDown(new ofxUIToggle("Synchronize",false,mDDList->getRect()->height,mDDList->getRect()->height,0,0,OFX_UI_FONT_MEDIUM));
	// remove button
	mGui.addWidgetDown(new ofxUILabelButton("Remove", false));

	// finish gui stuff
	mGui.autoSizeToFitWidgets();
	mGui.setColorBack(ofColor(100,200));
	ofAddListener(mGui.newGUIEvent,this,&MyKeeponControlPanel::guiListener);
}

MyKeeponControlPanel::~MyKeeponControlPanel(){
	mSerial.close();
}

void MyKeeponControlPanel::update(){
	// check if we have to update the list of serial connections on this panel
	if(bUpdateSerialList){
		mDDList->clearToggles();
		for(int i=0; i<theSerials.size(); i++){
			mDDList->addToggle(theSerials.at(i));
		}
		bUpdateSerialList = false;
	}
	if(bSerialInited) {
		mSerial.flush();
	}
	// TODO: set up timers and stuff to send signals to serial port
}

void MyKeeponControlPanel::guiListener(ofxUIEventArgs &args){
	string name = args.widget->getName();
	// Serial list bureaucracy
	if(name.compare("Serial List") == 0) {
		ofxUIDropDownList *ddlist = (ofxUIDropDownList *) args.widget;
		if(ddlist->getSelected().size()) {
			string selection = ddlist->getSelected()[0]->getName();
			if(selection.compare("Refresh List") == 0){
				// refresh static list, but only update dropdown on next call to update()
				updateSerialList();
				bUpdateSerialList = true;
				ddlist->clearSelected();
			}
			// else, if clicked on an item, try to setup a serial connection
			// TODO: add some feedback as to whether we're connected and to what
			else if(!ddlist->isOpen()) {
				mSerial.close();
				bSerialInited = mSerial.setup(selection, 115200);
			}
		}
	}
	// immediate-mode stuff
	else if(name.compare("Pan/Tilt") == 0) {
		if(bSerialInited) {
			int panV = (int)((ofxUI2DPad *)args.widget)->getScaledValue().x;
			int tiltV = (int)((ofxUI2DPad *)args.widget)->getScaledValue().y;
			string msg = "MOVE PAN "+ofToString(panV)+";";
			mSerial.writeBytes((unsigned char*)msg.c_str(), msg.size());
			msg = "MOVE TILT "+ofToString(tiltV)+";";
			mSerial.writeBytes((unsigned char*)msg.c_str(), msg.size());
		}
	}
	else if(name.compare("Pan Speed") == 0) {
		if(bSerialInited) {
			int speedV = (int)((ofxUISlider *)args.widget)->getScaledValue();
			string msg = "SPEED PAN "+ofToString(speedV)+";";
			mSerial.writeBytes((unsigned char*)msg.c_str(), msg.size());
		}
	}
	else if(name.compare("Tilt Speed") == 0) {
		if(bSerialInited) {
			int speedV = (int)((ofxUISlider *)args.widget)->getScaledValue();
			string msg = "SPEED TILT "+ofToString(speedV)+";";
			mSerial.writeBytes((unsigned char*)msg.c_str(), msg.size());
		}
	}
	else if(name.compare("PonSide Speed") == 0) {
		if(bSerialInited) {
			int speedV = (int)((ofxUISlider *)args.widget)->getScaledValue();
			string msg = "SPEED PONSIDE "+ofToString(speedV)+";";
			mSerial.writeBytes((unsigned char*)msg.c_str(), msg.size());
		}
	}
	// management stuff
	else if((name.compare("Synchronize") == 0) && (((ofxUIButton*)args.widget)->getValue())){
		// TODO: synch code
	}
	else if((name.compare("Remove") == 0) && (((ofxUIButton*)args.widget)->getValue())){
		bDelete = true;
	}
}

const bool& MyKeeponControlPanel::toDelete() const{
	return bDelete;
}

void MyKeeponControlPanel::setX(const int x_){
	mGui.setPosition(x_, mGui.getRect()->y);
}

const ofRectangle MyKeeponControlPanel::getRectangle() {
	return ofRectangle(mGui.getRect()->x,mGui.getRect()->y,mGui.getRect()->width,mGui.getRect()->height);
}
