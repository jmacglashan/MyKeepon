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

MyKeeponControlPanel::Values MyKeeponControlPanel::syncValues = Values();
MyKeeponControlPanel::DanceValues MyKeeponControlPanel::syncDanceValues = DanceValues();

set<MyKeeponControlPanel*> MyKeeponControlPanel::theSyncPanels = set<MyKeeponControlPanel*>();

MyKeeponControlPanel::MyKeeponControlPanel(const ofVec2f p):
mGui(p.x,p.y,0,0) {
	bDelete = false;
	bSerialInited = false;
	bUpdateSerialList = true;
	bIsSync = false;
	bUpdateGuiFromValues = false;
	
	// temporary common dimensions variable
	float tDim = 0;

	//////////////// my GUI
	mGui.setFont("verdana.ttf");
	mGui.addWidgetDown(new ofxUILabel("Control Panel", OFX_UI_FONT_MEDIUM));
	mGui.addSpacer(mGui.getRect()->width,4);
	////// Serial Port list
	mSerialList = (ofxUIDropDownList*) mGui.addWidgetDown(new ofxUIDropDownList("Serial List", updateSerialList()));
	mSerialList->setAutoClose(true);
	tDim = mSerialList->getRect()->height;
	mGui.addSpacer(10*tDim,5);

	////// 2D Pad for Pan/Tilt
	m2DPad = (ofxUI2DPad*) mGui.addWidgetDown(new ofxUI2DPad("Pan/Tilt", ofPoint(0,1), ofPoint(0,1), ofPoint(0.5,0.5),
									  10*tDim, 10*tDim));
	////// Motor speeds
	mPanSpeed = (ofxUISlider*) mGui.addWidgetDown(new ofxUISlider("Pan Speed", 0,1, 0.5,10*tDim,tDim));
	mTiltSpeed = (ofxUISlider*) mGui.addWidgetDown(new ofxUISlider("Tilt Speed", 0,1, 0.5,10*tDim,tDim));
	mSideSpeed = (ofxUISlider*) mGui.addWidgetDown(new ofxUISlider("PonSide Speed", 0,1, 0.5,10*tDim,tDim));
	// synch button
	mGui.addWidgetDown(new ofxUIToggle("Synchronize",false,tDim,tDim,0,0,OFX_UI_FONT_MEDIUM));
	mGui.addSpacer(10*tDim,5);

	////// Dance interface
	mGui.addWidgetDown(new ofxUILabel("enable double reverse", OFX_UI_FONT_MEDIUM));
	mPanDance = (ofxUIToggleMatrix*) mGui.addWidgetDown(new ofxUIToggleMatrix(3*tDim, tDim, 1, 3, "Pan Dance"));
	mTiltDance = (ofxUIToggleMatrix*) mGui.addWidgetDown(new ofxUIToggleMatrix(3*tDim, tDim, 1, 3, "Tilt Dance"));
	mSideDance = (ofxUIToggleMatrix*) mGui.addWidgetDown(new ofxUIToggleMatrix(3*tDim, tDim, 1, 3, "Side Dance"));
	mGui.addSpacer(10*tDim,5);

	// remove button
	mGui.addWidgetDown(new ofxUILabelButton("Remove", false));

	// finish gui stuff
	mGui.autoSizeToFitWidgets();
	mGui.setColorBack(ofColor(100,200));
	ofAddListener(mGui.newGUIEvent,this,&MyKeeponControlPanel::guiListener);
}

MyKeeponControlPanel::~MyKeeponControlPanel(){
	mSerial.close();
	theSyncPanels.erase(this);
}

void MyKeeponControlPanel::update(){
	// check if we have to update the list of serial connections on this panel
	if(bUpdateSerialList){
		mSerialList->clearToggles();
		for(int i=0; i<theSerials.size(); i++){
			mSerialList->addToggle(theSerials.at(i));
		}
		bUpdateSerialList = false;
	}
	// clear serial pipe
	if(bSerialInited) {
		mSerial.flush();
	}
	
	// check if we have to update due to sync
	if(bUpdateGuiFromValues){
		m2DPad->setValue(ofPoint(mValues.pan,mValues.tilt));
		mPanSpeed->setValue(mValues.panSpeed);
		mTiltSpeed->setValue(mValues.tiltSpeed);
		mSideSpeed->setValue(mValues.sideSpeed);
		bUpdateGuiFromValues = false;
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
		mValues.pan = ((ofxUI2DPad *)args.widget)->getScaledValue().x;
		mValues.tilt = ((ofxUI2DPad *)args.widget)->getScaledValue().y;
		if(!bIsSync) {
			sendPanAndTilt();
		}
		else {
			syncValues = mValues;
			sendSyncPanAndTilt();
		}
	}
	else if(name.compare("Pan Speed") == 0) {
		mValues.panSpeed = ((ofxUISlider *)args.widget)->getScaledValue();
		if(!bIsSync) {
			sendPanSpeed();
		}
		else {
			syncValues = mValues;
			sendSyncPanSpeed();
		}
	}
	else if(name.compare("Tilt Speed") == 0) {
		mValues.tiltSpeed = ((ofxUISlider *)args.widget)->getScaledValue();
		if(!bIsSync) {
			sendTiltSpeed();
		}
		else {
			syncValues = mValues;
			sendSyncTiltSpeed();
		}
	}
	else if(name.compare("PonSide Speed") == 0) {
		mValues.sideSpeed = ((ofxUISlider *)args.widget)->getScaledValue();
		if(!bIsSync) {
			sendSideSpeed();
		}
		else {
			syncValues = mValues;
			sendSyncSideSpeed();
		}
	}
	// management stuff
	else if(name.compare("Synchronize") == 0){
		bIsSync = ((ofxUIButton*)args.widget)->getValue();
		if(bIsSync) {
			// if first item, copy to syncValue
			if(theSyncPanels.size() < 1){
				syncValues = mValues;
			}
			else{
				mValues = syncValues;
				bUpdateGuiFromValues = true;
			}
			// add to set of sync panels
			theSyncPanels.insert(this);
		}
		else {
			// remove from vector of sync panels
			theSyncPanels.erase(this);
		}
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

//// helpers
void MyKeeponControlPanel::sendPanAndTilt() {
	if(bSerialInited){
		string msg = "MOVE PAN "+ofToString((int)ofMap(mValues.pan, 0,1, -90,90))+";";
		mSerial.writeBytes((unsigned char*)msg.c_str(), msg.size());
		msg = "MOVE TILT "+ofToString((int)ofMap(mValues.tilt, 0,1, -90,90))+";";
		mSerial.writeBytes((unsigned char*)msg.c_str(), msg.size());
	}
}
void MyKeeponControlPanel::sendPanSpeed() {
	if(bSerialInited) {
		string msg = "SPEED PAN "+ofToString((int)ofMap(mValues.panSpeed, 0,1, 64,250))+";";
		mSerial.writeBytes((unsigned char*)msg.c_str(), msg.size());
	}
}
void MyKeeponControlPanel::sendTiltSpeed() {
	if(bSerialInited) {
		string msg = "SPEED TILT "+ofToString((int)ofMap(mValues.tiltSpeed, 0,1, 64,250))+";";
		mSerial.writeBytes((unsigned char*)msg.c_str(), msg.size());
	}
}
void MyKeeponControlPanel::sendSideSpeed() {
	if(bSerialInited) {
		// TODO: change output min/max to prevent reaching limits
		string msg = "SPEED PONSIDE "+ofToString((int)ofMap(mValues.sideSpeed, 0,1, 0,255))+";";
		mSerial.writeBytes((unsigned char*)msg.c_str(), msg.size());
	}
}

//// static helpers
void MyKeeponControlPanel::sendSyncPanAndTilt() {
	for(set<MyKeeponControlPanel*>::const_iterator it=theSyncPanels.begin(); it!=theSyncPanels.end(); ++it){
		(*it)->mValues = syncValues;
		(*it)->sendPanAndTilt();
		(*it)->bUpdateGuiFromValues = true;
	}
}
void MyKeeponControlPanel::sendSyncPanSpeed() {
	for(set<MyKeeponControlPanel*>::const_iterator it=theSyncPanels.begin(); it!=theSyncPanels.end(); ++it){
		(*it)->mValues = syncValues;
		(*it)->sendPanSpeed();
		(*it)->bUpdateGuiFromValues = true;
	}
}
void MyKeeponControlPanel::sendSyncTiltSpeed() {
	for(set<MyKeeponControlPanel*>::const_iterator it=theSyncPanels.begin(); it!=theSyncPanels.end(); ++it){
		(*it)->mValues = syncValues;
		(*it)->sendTiltSpeed();
		(*it)->bUpdateGuiFromValues = true;
	}
}
void MyKeeponControlPanel::sendSyncSideSpeed() {
	for(set<MyKeeponControlPanel*>::const_iterator it=theSyncPanels.begin(); it!=theSyncPanels.end(); ++it){
		(*it)->mValues = syncValues;
		(*it)->sendSideSpeed();
		(*it)->bUpdateGuiFromValues = true;
	}
}
