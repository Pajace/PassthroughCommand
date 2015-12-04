/*
 * ExternalCommander.h
 *
 *  Created on: Aug 24, 2011
 *      Author: pajace
 */

#ifndef EXTERNALCOMMANDER_H_
#define EXTERNALCOMMANDER_H_
#include <string>
#include <vector>
using namespace std;

class ExternalCommander {
public:
	static const int CMD_NOT_FOUND = 32512;
	static ExternalCommander* getInstance();

	string getDeviceUsbNicStatus(string deviceSerialNumber);
	vector<string> getBindBridgeCmd(string bridge, string bindedInterface);
	string getExternalCommandResult(string command) const;
	void waitForDevice(string deviceSN);
	string getInternalNetworkDNS() const;


	int runExternalCommand(char command[]);
	int runExternalCommand(const vector<string> &command);
	int runExternalCommand(string command);
private:
	ExternalCommander(){}; // private so that it can not be called
	ExternalCommander(ExternalCommander const&){}; // copy constructor is private
	static ExternalCommander* m_pInstance;


};

#endif /* EXTERNALCOMMANDER_H_ */
