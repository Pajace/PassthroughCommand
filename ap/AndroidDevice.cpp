/*
 * AndroidDevice.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: pajace
 */
#include <string>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "IPaddress.h"
#include "AndroidDevice.h"
#include "IptLog.h"
#include "IptException.h"
#include "ExternalCommander.h"
#include "../Resource.h"
using namespace std;

AndroidDevice::AndroidDevice(){}
AndroidDevice::AndroidDevice(string serialNumberString) {
	if (setSerialNumber(serialNumberString)==false)
		throw AndroidDeviceSnFormatIncorrectException();
}

bool AndroidDevice::setSerialNumber(string serialNumber) {
	if (serialNumber.size() == 12 && (isContainWhiteSpace(_serialNumber)==false)) {
		_serialNumber = serialNumber;
		return true;
	}
	return false;
}

bool AndroidDevice::setIpAddress(string ipAddress) {
	bool isSetting = _ipAddress.setIPAddress(ipAddress);
	return isSetting;
}

bool AndroidDevice::setIptProductId(string iptProductId) {
	if ( iptProductId.size() == 4 && (isContainWhiteSpace(iptProductId)==false)) {
		_iptProductId = iptProductId;
		return true;
	}
	return false;
}

/**
 * Default: the pluggedDeviceList is empty
 * pluggedDeviceList is for testing use.
 */
bool AndroidDevice::isDevicePlugged(string pluggedDeviceList) {
	stringstream s("");
	int devicePosition=-1;

	if (pluggedDeviceList=="") {
		for (int tryToGetDevice=0; tryToGetDevice<10; tryToGetDevice++) {
			s << tryToGetDevice;
			pluggedDeviceList = ExternalCommander::getInstance()->getExternalCommandResult(ADB_COMMAND+" devices");
			devicePosition = pluggedDeviceList.find(_serialNumber);
			IptLog::getInstance()->logDebug("AndroidDevice::isDevicePlugged: ran command : #" + s.str() +" - "+ ADB_COMMAND + " devices");
			IptLog::getInstance()->logDebug("AndroidDevice::isDevicePlugged: getCommandResult : " + pluggedDeviceList );

			if (devicePosition <= 1000 && devicePosition != -1) {
				return true;
			}
			sleep(1);
			s.str("");
			s.clear();
		}
		pluggedDeviceList = ExternalCommander::getInstance()->getExternalCommandResult(ADB_COMMAND+" devices");
	}

	devicePosition = pluggedDeviceList.find(_serialNumber);
	if(devicePosition>=10000 || devicePosition == -1) { // some machine(i.e. virtual machine) will find at the end, even the pattern does'nt exit.
		s << devicePosition;
		IptLog::getInstance()->logDebug("AndroidDevice::isDevicePlugged: devicePosition is " + s.str());
		IptLog::getInstance()->logDebug("AndroidDevice::isDevicePlugged: Device[" + _serialNumber + "] was not plugged. ->" + pluggedDeviceList);
		return false;
	}
	return true;
}

/**
 * Default: the usbNicStatusFile is empty
 * usbNicStatusFile is for test use.
 */
bool AndroidDevice::isDeviceEnableIptMode(string usbNicStatusFile) {

	if (usbNicStatusFile=="")
		usbNicStatusFile = ExternalCommander::getInstance()->getDeviceUsbNicStatus(_serialNumber);

	if (usbNicStatusFile=="") // find again, it still cannot find usb0, that means the status of ipt is disable.
		return false;

	int pos = usbNicStatusFile.find("0.0.0.0");
	int posr = usbNicStatusFile.rfind("0.0.0.0");

	if ( (pos>=0 && pos <=5000) && (posr>=0 && posr <=5000) ) { // some machine(i.e. virtual machine) will find at the end, even the pattern does'nt exit.
		IptLog::getInstance()->logDebug("AndroidDevice::isDeviceEnableIptMode: Device["+_serialNumber+"] was not enable ipt. -> \n"+ usbNicStatusFile);
		return false; // because twice "0.0.0.0" means not enable ipt
	}

	return true;
}

string AndroidDevice::getSerialNumber() const {
	return _serialNumber;
}

IPaddress AndroidDevice::getIpAddress() const {
	return _ipAddress;
}
string AndroidDevice::getIptProductId() const {
	return _iptProductId;
}

bool AndroidDevice::isContainWhiteSpace(string sourceString) {
	char testContinerChar[sourceString.size()];
	strcpy(testContinerChar, sourceString.c_str());
	for (unsigned int i=0; i<sourceString.size(); i++) {
		if(testContinerChar[i]== ' ')
			return true;
	}
	return false;
}
