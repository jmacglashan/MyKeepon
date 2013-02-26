#include "ScriptReader.h"

ScriptReader::ScriptReader() {

}
ScriptReader::~ScriptReader() {}

void ScriptReader::loadScript(const string fileName) {
	theXml.loadFile(fileName);
}

const string ScriptReader::getCommand() {
	// clamp currCommand to not overshoot tags
	currCommand = (currCommand >= theXml.getNumTags("cmd"))?theXml.getNumTags("cmd")-1:currCommand;
	return theXml.getValue("cmd", "", currCommand);

}
const unsigned int ScriptReader::getDelay() {
	// clamp currCommand to not overshoot tags
	currCommand = (currCommand >= theXml.getNumTags("cmd"))?theXml.getNumTags("cmd")-1:currCommand;
	return theXml.getAttribute("cmd", "time", 0, currCommand);
}

// currCommand can point to one past last tag
void ScriptReader::popCommand() {
	if(currCommand < theXml.getNumTags("cmd")) {
		currCommand++;
	}
}

const bool ScriptReader::hasCommand() {
	return (currCommand < theXml.getNumTags("cmd"));
}
