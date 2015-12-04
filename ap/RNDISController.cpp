/*
 * RNDISController.cpp
 *
 *  Created on: Aug 29, 2011
 *      Author: pajace
 */
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <vector>
#include "RNDISController.h"
#include "IptLog.h"
#include "../Resource.h"
using namespace std;

static const string TAG="#-///";
static const int ENABLE = 1;
static const int DISABLE = 0;

RNDISController::RNDISController() {
	udevConfigFile = PERSISTENCE_IPT_CONFIG;
	ifstream inFile(getUdevConfigFile().c_str(), ios::in);
	if (!inFile) generateConfigFile();
	inFile.close();
}

RNDISController::~RNDISController() {
}

bool RNDISController::isDoubleEnable(const vector<string> &udevRulecontent, string serialNumber) {
	for (size_t i=0; i<udevRulecontent.size(); i++) {
		if (isHeader(udevRulecontent[i]) &&
			  (parsingSerialNumber(udevRulecontent[i]).compare(serialNumber)==0) )
			  return true;
	}
	return false;
}

void RNDISController::writeConfigBack2SystemFile(const vector<string> &udevRuleContent, int enableOrDisable)
{
    ofstream outFile(udevConfigFile.c_str(), ios::out);
    for(size_t i = 0;i < udevRuleContent.size();i++){
    	if ( enableOrDisable == ENABLE )
    		outFile << udevRuleContent[i] << endl;
    	else if ( enableOrDisable == DISABLE )
    		outFile << udevRuleContent[i] << (i==(udevRuleContent.size()-1)? "": "\n");
    }
    outFile.close();
}

vector<string> RNDISController::getUdevConfigContent() {
	ifstream inFile(udevConfigFile.c_str(), ios::in);
	if (!inFile) {
		IptLog::getInstance()->logAlert("RNDISController::disablePersistent: Cannot open udev configuration file. [" +udevConfigFile + "]" );
		generateConfigFile();
		inFile.open(udevConfigFile.c_str(), ios::in);
	}
	vector<string> udevRuleContent(0);
	string tmp;
	while ( inFile.good() && !inFile.eof() ) {
		getline(inFile,  tmp);
		udevRuleContent.push_back(tmp);
	}
	return udevRuleContent;
}

bool RNDISController::enablePersistent(string serialNumber, string iptProductID) {
	IptLog::getInstance()->logDebug("RNDiscontroller::enablePersistent: Enter Critical Section.");

	vector<string> udevRuleContent(getUdevConfigContent());

	if (isDoubleEnable(udevRuleContent, serialNumber)==true)
		return false;

	// Plus a rule on temp
	udevRuleContent.pop_back(); // Because getline() always get more one line of eof, so i have to minus it.
	udevRuleContent.push_back(getUdevHeaderOfRNDIS(serialNumber));
	udevRuleContent.push_back(getUdevRuleOfRNDIS(serialNumber, iptProductID));

    writeConfigBack2SystemFile(udevRuleContent, ENABLE);

	IptLog::getInstance()->logDebug("RNDiscontroller::enablePersistent: Leave Critical Section.");
	return isSuccessToAdd(serialNumber);
}

bool RNDISController::disablePersistent(string serialNumber) {

	IptLog::getInstance()->logDebug("RNDISController::disablePersistent: Enter Critical Section.");

	// Step 1: Read udev rule into temp
	vector<string> temp(getUdevConfigContent());

	// Step 2: parsing udev rule to find target device's rule then delete it
	//         even more than one rule are the same serial number
	string targetSerialNumber("");
	vector<string> udevRuleContent(0);
	for (size_t i=0; i < temp.size(); i++) {
		if (isHeader(temp.at(i))) {
			targetSerialNumber = parsingSerialNumber(temp.at(i));
			if (targetSerialNumber.compare(serialNumber)==0) {
				i++; // matched!! skip this rule, i.e. means delete it.
			} else {
				udevRuleContent.push_back(temp[i]);
			}
		} else {
			udevRuleContent.push_back(temp[i]);
		}
	}

	// Step 3:
	writeConfigBack2SystemFile(udevRuleContent, DISABLE);

	IptLog::getInstance()->logDebug("RNDISController::disablePersistent: Leave Critical Section.");

	// 4. return success to remove that rule or not
	return isSuccessToRemove(serialNumber);
}

bool RNDISController::isSuccessToAdd(string serialNumber) {

    vector<string> checkTemp(getUdevConfigContent());
    bool isAdded = false;
    for (size_t i=0; i<checkTemp.size(); i++) {
    	// It means that the serial have success to added.
    	if ( isHeader(checkTemp.at(i)) && (parsingSerialNumber(checkTemp.at(i)).compare(serialNumber)==0))
    		isAdded = true;
    }
    return isAdded;
}


bool RNDISController::isSuccessToRemove(string serialNumber) {

	vector<string> checkTemp(getUdevConfigContent());
	for (size_t i = 0; i<checkTemp.size(); i++) {
		// It means that rule of serial number still exists.
		if ( isHeader(checkTemp.at(i)) && parsingSerialNumber(checkTemp.at(i)).compare(serialNumber)==0) {
			return false;
		}
	}
	return true;
}

bool RNDISController::checkSerialNumber(string header) {
	char *headerChar = &header[0];
	for (int i=5; i<=16; i++) {
		if (headerChar[i]== ' ' ||
				headerChar[i]=='/' ||
				headerChar[i]=='\\' ||
				headerChar[i]=='.' ||
				headerChar[i]=='-')
			return false;
	}
	return true;
}

bool RNDISController::isHeader(string header) {
	char *headerChar = &header[0];
	if (headerChar[0]=='#' &&
			headerChar[1]=='-' &&
			headerChar[2]=='/' &&
			headerChar[3]=='/' &&
			headerChar[4]=='/' &&
			checkSerialNumber(header))
		return true;
	else
		return false;
}

string RNDISController::parsingSerialNumber(string header) const{
	stringstream ss;
	ss.str("");
	ss.clear();
	string serialNumber;
	char *headerChar = &header[0];
	for (int i=5; i<=16; i++)
		ss << headerChar[i];
	serialNumber = ss.str();

	return serialNumber;
}

string RNDISController::getUdevHeaderOfRNDIS(string serialNumber) const {
	string header(TAG);
	header.append(serialNumber);
	return header;
}

string RNDISController::getUdevRuleOfRNDIS(string serialNumber, string iptProductId) const {
	//ACTION==\"add\", SUBSYSTEM==\"usb\", DRIVERS==\"usb\", ATTRS{idProduct}!=\"0ffc\", ATTRS{serial}==\"ATSRUBY00045\", RUN+=\"/usr/local/sbin ipt s serialNumber ON\"
	string rule("ACTION==\"add\", SUBSYSTEM==\"usb\", DRIVERS==\"usb\", ATTRS{idProduct}!=\"");
	rule.append(iptProductId);
	rule.append("\", ATTRS{serial}==\"");
	rule.append(serialNumber);
	rule.append("\", RUN+=\"/usr/local/sbin/ipt s ");
	rule.append(serialNumber);
	rule.append(" ON\"");

	return rule;
}

void RNDISController::setUdevConfigFile(string udevConfigFileString) {
	udevConfigFile = udevConfigFileString;
}

string RNDISController::getUdevConfigFile() const {
	return udevConfigFile;
}

void RNDISController::generateConfigFile() {
	ofstream outFile(&udevConfigFile[0], ios::out);
	outFile << "# This file maintains persistent names for network interfaces." << endl;
	outFile << "# See udev(7) for syntax." << endl;
	outFile << "#" << endl;
	outFile << "# Entries are automatically added by the 75-persistent-androidusbnet.rules" << endl;
	outFile << "# file; however you are also free to add your own entries." << endl;
	outFile << "# DON'T MODIFY THE FOLLOW SETTING TO PREVENT ERROR BY IPT PARSING." << endl;
	outFile << endl;
	outFile.close();
}

void RNDISController::generateConfigFile_AutoBindRndis(string configFileName) {
	ofstream outFile(configFileName.c_str(), ios::out);
	outFile << "# This file maintains persistent names for network interfaces." << endl;
	outFile << "# See udev(7) for syntax." << endl;
	outFile << "#" << endl;
	outFile << "# Entries are automatically added by the " << configFileName << ". " << endl;
	outFile << "# file; however you are also free to add your own entries." << endl;
	outFile << "# DON'T MODIFY THE FOLLOW SETTING TO PREVENT ERROR BY IPT PARSING." << endl;
	outFile << RNDIS_BIND_UDEV_RULE << endl;
	outFile << endl;
	outFile.close();
}
