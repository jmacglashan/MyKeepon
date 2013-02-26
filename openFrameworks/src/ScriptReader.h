#pragma once
#include "ofxXmlSettings.h"

class ScriptReader{
	public:
		ScriptReader();
		~ScriptReader();
		void loadScript(string fileName);
		const string& getCommand() const;
		const unsigned int& getDelay() const;
		void popCommand();
		const bool hasCommand();
		void reset();
	private:
		ofxXmlSettings theXml;
		unsigned int currCommandIndex;
		string currCommand;
		unsigned int currDelay;
};
