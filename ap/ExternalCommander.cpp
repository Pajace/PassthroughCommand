/*
 * ExternalCommander.cpp
 *
 *  Created on: Aug 24, 2011
 *      Author: pajace
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ExternalCommander.h"
#include "IptLog.h"
#include "../Resource.h"
using namespace std;

ExternalCommander* ExternalCommander::m_pInstance = NULL;

ExternalCommander* ExternalCommander::getInstance() {
	if (!m_pInstance)
		m_pInstance = new ExternalCommander();
	return m_pInstance;
}

string ExternalCommander::getDeviceUsbNicStatus(string deviceSerialNumber){

	string command = ADB_COMMAND;
	command.append(" -s ");
	command.append(deviceSerialNumber);
	command.append(" shell netcfg | grep usb0");
	string result = getExternalCommandResult(command);
	return result;
}

string ExternalCommander::getInternalNetworkDNS() const {
	string getDNScommand = "nslookup 127.0.0.1 | grep Address | awk {'print$2'} | sed 's/#53.*//g'";
	string dns("");
	dns = getExternalCommandResult(getDNScommand);
	if (dns.empty()) {
		IptLog::getInstance()->logAlert("ExternalCommander::getInternalNetworkDNS: Can not get the DNS. ");
		cerr << "[IPT Message] Cannot get the internal network dns, use " + IPT_BACKUP_DNS_IP +" to replace it." << endl;
		dns = IPT_BACKUP_DNS_IP;
	} else if (dns.find("127.0.0.1")!=std::string::npos){
		IptLog::getInstance()->logAlert("Your host's NDS setting is not correct. Replace 168.95.1.1 with your DNS.");
		dns = "168.95.1.1";
	}
	return dns;
}

string ExternalCommander::getExternalCommandResult(string command) const {

	FILE* stream = popen(&command[0], "r");

	if (!stream) return "ERROR";
	char buffer[128];
	string result("");
	while(!feof(stream)) {
		if(fgets(buffer, 128, stream) != NULL)
			result += buffer;
	}
	pclose(stream);
	return result;
}


int ExternalCommander::runExternalCommand(char command[]) {
	int result = -10;
	string cmd(command);
	IptLog::getInstance()->logInfo("Run Command : " + cmd);
#ifdef _DEBUG_
	cout << "Run command : " << cmd << endl;
#endif
	result = system(command);
	if (result==ExternalCommander::CMD_NOT_FOUND) {
		string cmdString(command);
		IptLog::getInstance()->logAlert("ExternalCommander::runExternalcommand: Command not found - " + cmdString);
	}
	return result;
}

int ExternalCommander::runExternalCommand(string command) {
	int result = -10;
	IptLog::getInstance()->logInfo("Run Command : " + command);
#ifdef _DEBUG_
	cout << "Run command : " <<command << endl;
#endif
	result = system(command.c_str());
	if (result==ExternalCommander::CMD_NOT_FOUND) {
		IptLog::getInstance()->logError("ExternalCommander::runExternalCommand: Command not found - " + command);
	}
	return result;
}

int ExternalCommander::runExternalCommand(const vector<string> &command) {
	int result = -10;
	string cmdString("");
	for (size_t i=0; i<command.size(); i++) {
#ifdef _DEBUG_
		cout << "Run command : " << command.at(i) << endl;
#endif
		IptLog::getInstance()->logInfo("Run Command : " + command.at(i));
		result = system(command.at(i).c_str());
		if (result==ExternalCommander::CMD_NOT_FOUND) {
			cmdString.append(command.at(i)+"\n");
			IptLog::getInstance()->logAlert("ExternalCommander::runExternalcommand: Command not found - " + cmdString);
		}
	}
	return result;
}
