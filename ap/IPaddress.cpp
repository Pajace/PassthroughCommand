/*
 * IPaddress.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: pajace
 */

#include "IPaddress.h"
#include <string>
#include <stdio.h>
#include <string.h>
#include "IptLog.h"
using std::string;

// Default Constructor
IPaddress::IPaddress(string ipAddressString) {
	setIPAddress(ipAddressString);
}

bool IPaddress::setIPAddress(string ipAddressString) {
	bool isIPCurrect = validateIpAddress(ipAddressString);
	if (isIPCurrect == true) {
		ipaddress = ipAddressString;
		return true;
	}
	IptLog::getInstance()->logInfo("The IP address that user entered is not valid.");
	return false;
}

string IPaddress::toString() const {
	return ipaddress;
}

bool IPaddress::validateIpAddress(const string ipAddress) {
	unsigned int n1, n2, n3, n4;
	const char *ipStr = &ipAddress[0];
	if(sscanf(ipStr,"%u.%u.%u.%u", &n1, &n2, &n3, &n4) != 4)
		return false;

	if ((n1!=0) && (n1<=255) && (n2<=255) && (n3<=255) && (n4<=255) ) {
		char buf[64];
		sprintf(buf, "%u.%u.%u.%u", n1, n2, n3, n4);
		if (strcmp(buf, ipStr))
			return false;
		return true;
	}
	return false;
}
