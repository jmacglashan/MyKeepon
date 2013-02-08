#include "MyKeeponControlPanel.h"

MyKeeponControlPanel::MyKeeponControlPanel(const ofVec2f p):
mGui(p.x,p.y,0,0) {
	bDelete = false;

	/////////////// my GUI
	// TODO: setup vector with list of serial ports

	mGui.setFont("verdana.ttf");
	mGui.addWidgetDown(new ofxUILabel("Control Panel", OFX_UI_FONT_MEDIUM));
	mGui.addSpacer(mGui.getRect()->width,4);
	mGui.addWidgetDown(new ofxUILabelButton("Remove", false));
	// TODO:  add other stuff here

	mGui.autoSizeToFitWidgets();
	mGui.setColorBack(ofColor(100,200));
	ofAddListener(mGui.newGUIEvent,this,&MyKeeponControlPanel::guiListener);
}

MyKeeponControlPanel::~MyKeeponControlPanel(){}

void MyKeeponControlPanel::update(){
	// set up timers and stuff to send signals to serial port
}

void MyKeeponControlPanel::guiListener(ofxUIEventArgs &args){
	string name = args.widget->getName();
	if((name.compare("Remove") == 0) && (((ofxUIButton*)args.widget)->getValue())){
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
