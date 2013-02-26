#include "ScriptReader.h"

ScriptReader::ScriptReader() {
	this->reset();
}
ScriptReader::~ScriptReader() {}

void ScriptReader::loadScript(const string fileName) {
	theXml.loadFile(fileName);
	this->reset();
}

const string& ScriptReader::getCommand() const {
	return currCommand;
}
const unsigned int& ScriptReader::getDelay() const {
	return currDelay;
}

// currCommandIndex can point to one past last tag
void ScriptReader::popCommand() {
	if(currCommandIndex < theXml.getNumTags("cmd")-1) {
		currCommandIndex++;
		currCommand = theXml.getValue("cmd", "", currCommandIndex);
		currDelay = theXml.getAttribute("cmd", "time", 1e10, currCommandIndex);
	}
	else if(currCommandIndex < theXml.getNumTags("cmd")) {
		currCommandIndex++;
		// now currCommandIndex points beyond last tag, but currComand and currDelay have last tag values
	}
}

const bool ScriptReader::hasCommand() {
	return (currCommandIndex < theXml.getNumTags("cmd"));
}

void ScriptReader::reset() {
	currCommandIndex = 0;
	currCommand = "";
	currDelay = 1e10;

	if(theXml.bDocLoaded) {
		currCommand = theXml.getValue("cmd", "", currCommandIndex);
		currDelay = theXml.getAttribute("cmd", "time", 1e10, currCommandIndex);
	}
}

