/*
 * Arguments.h
 *
 *  Created on: Aug 18, 2011
 *      Author: pajace
 */

#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_
#include <string>
#include "IptCmd.h"
using namespace std;

class Arguments {
public:

	Arguments();
	bool validateIpAddress(const string);
	bool validateUsbCode(const string);
	bool validateSerialNumber(const string);
	string getSerialNumber() const;
	string getDnsIP() const;
	string getIptProductID() const;
	string CharPointerArrayToString(int, char* argv[]);
	string getRndisName() const;
	IptCmd getIPTCommand(string);
	string getUsage();

private:
	string unknowFlag;
	string serialNumber;
	string dnsIP;
	string iptProductId;
	string iptSwitch;
	string rndisName;
};

#endif /* ARGUMENTS_H_ */
