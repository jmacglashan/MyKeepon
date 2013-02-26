#pragma once
#include "ofxXmlSettings.h"

class ScriptReader{
	public:
		ScriptReader();
		~ScriptReader();
		void loadScript(string fileName);
		const string getCommand();
		const unsigned int getDelay();
		void popCommand();
		const bool hasCommand();
	private:
		ofxXmlSettings theXml;
		unsigned int currCommand;
};
