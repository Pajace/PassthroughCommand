/*
 * AndroidDevice.h
 *
 *  Created on: Aug 25, 2011
 *      Author: pajace
 */

#ifndef ANDROID_DEVICE_H_
#define ANDROID_DEVICE_H_
#include <string>
#include "IPaddress.h"
using std::string;


class AndroidDevice {
public:
	AndroidDevice();
	AndroidDevice(string serialNumber);
	bool setSerialNumber(string);
	bool setIpAddress(string);
	bool setIptProductId(string);
	bool isDevicePlugged(string);
	bool isDeviceEnableIptMode(string usbNicStatusFile);

	string getSerialNumber() const;
	IPaddress getIpAddress() const;
	string getIptProductId() const;

private:
	string _serialNumber;
	IPaddress _ipAddress;
	string _iptProductId;
	bool isContainWhiteSpace(string);
};

#endif /* ANDROID_DEVICE_H_ */
