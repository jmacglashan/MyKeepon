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

	/////////////// my GUI

	mGui.setFont("verdana.ttf");
	mGui.addWidgetDown(new ofxUILabel("Control Panel", OFX_UI_FONT_MEDIUM));
	mGui.addSpacer(mGui.getRect()->width,4);
	mDDList = (ofxUIDropDownList *) mGui.addWidgetDown(new ofxUIDropDownList("Serial List", updateSerialList()));
	mDDList->setAutoClose(true);

	mGui.addWidgetDown(new ofxUILabelButton("Remove", false));
	// TODO:  add other stuff here

	mGui.autoSizeToFitWidgets();
	mGui.setColorBack(ofColor(100,200));
	ofAddListener(mGui.newGUIEvent,this,&MyKeeponControlPanel::guiListener);
}

MyKeeponControlPanel::~MyKeeponControlPanel(){}

void MyKeeponControlPanel::update(){
	// check if we have to update the list of serial connections on this panel
	if(bUpdateSerialList){
		mDDList->clearToggles();
		updateSerialList();
		for(int i=0; i<theSerials.size(); i++){
			mDDList->addToggle(theSerials.at(i));
		}
		bUpdateSerialList = false;
	}
	// TODO: set up timers and stuff to send signals to serial port
}

void MyKeeponControlPanel::guiListener(ofxUIEventArgs &args){
	string name = args.widget->getName();
	if((name.compare("Remove") == 0) && (((ofxUIButton*)args.widget)->getValue())){
		bDelete = true;
	}
	else if(name.compare("Serial List") == 0) {
		ofxUIDropDownList *ddlist = (ofxUIDropDownList *) args.widget;
		if(ddlist->getSelected().size()) {
			string selection = ddlist->getSelected()[0]->getName();
			// check if refresh
			if(selection.compare("Refresh List") == 0){
				bUpdateSerialList = true;
			}
			// else setup internal serial
			else{
				mSerial.close();
				mSerial.setup(selection, 115200);
			}
			ddlist->clearSelected();
		}
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
