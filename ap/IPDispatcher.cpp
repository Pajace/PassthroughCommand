/*
 * IPDispatcher.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: pajace
 */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <sys/sem.h>

#include "IPDispatcher.h"
#include "XDevice.h"
#include "IptLog.h"
#include "IptException.h"
#include "../Resource.h"

using namespace std;

// Global static pointer used to ensure a single instance of the class.
IPDispatcher* IPDispatcher::m_pInstance = NULL;

static const string EMPTY="000000000000"; // given 12's 0 means empty, i.e. the IP is free

IPDispatcher* IPDispatcher::getInstance() {

	if (!m_pInstance) {
		m_pInstance = new IPDispatcher();
	}

	return m_pInstance;
}
IPDispatcher::~IPDispatcher() {
	delete m_pInstance;
}

string IPDispatcher::getIPPoolDataFile() const {
	return "/tmp/ipt_ip_pool.dat";
}

/**
 * getIPAddress(string serialNumberValue):IPaddress
 * serialNumberValue : the device's serial that request IP address
 */
IPaddress IPDispatcher::getIPAddress(string serialNumberValue) {

	IptLog::getInstance()->logDebug("IPDispatcher::getIPAddress: Enter Critical Section.");

	// Read serial-ip match file from IP pool
	fstream inOutFile(&getIPPoolDataFile()[0], ios::in | ios::out);

	if(!inOutFile) {
		IptLog::getInstance()->logAlert("IPDispatcher::getIPAddress: IP pool file cannot be open.");
		throw IpPoolCrashException();
	}

	// Transfer file data to object array
	XDevice devices[254];
	string ipAddressString;
	string deviceSerialString;
	for (int i=0; i<254; i++) {
		inOutFile >> ipAddressString >> deviceSerialString;
		devices[i].setSerialNumber(deviceSerialString);
		devices[i].setIpAddress(ipAddressString);
	}

	// and choose an IP
	IPaddress dispatchIP;
	bool isAlreadyGetIP = false;
	for (int i=0; i<254; i++) {
		if (devices[i].getSerialNumber().compare(serialNumberValue)==0) {
			// if true, means that serialNumber have already request an IP.
			dispatchIP.setIPAddress(devices[i].getIpAddress().toString());
			devices[i].setSerialNumber(serialNumberValue);

			IptLog::getInstance()->logError(
					"IPDispatcher::getIPAddress: The device ["+
					serialNumberValue +
					"] Request IP address more than once.");

			isAlreadyGetIP = true;
			break;
		}
	}

	if (isAlreadyGetIP==false) {
		for (int i=0; i<254; i++) {
			if( devices[i].getSerialNumber() == EMPTY) {
				devices[i].setSerialNumber(serialNumberValue);
				dispatchIP.setIPAddress(devices[i].getIpAddress().toString());
				break;
			}
		}
	}

	// write back to ip pool
	fstream writeBack(&getIPPoolDataFile()[0], ios::out);
	for (int i=0; i<254; i++) {
		writeBack << devices[i].getIpAddress().toString() << " " << devices[i].getSerialNumber() << endl;
	}

	IptLog::getInstance()->logDebug("IPDispatcher::getIPAddress: Leave Critical Section.");
	return dispatchIP;
}


bool IPDispatcher::backIPAddress(string sn) {
	IptLog::getInstance()->logDebug("IPDispatcher::backIPAddress: Enter Critical Section.");

	bool isBackIPOK = false;

	fstream inOutFile(&getIPPoolDataFile()[0], ios::in | ios::out);

	if(!inOutFile) {
		IptLog::getInstance()->logError("IPDispatcher::backIPAddress: IP pool file cannot open.");
		throw IpPoolCrashException();
	}

	XDevice devices[254];
	string ipAddressString;
	string deviceSerialString;

	for (int i=0; i<254; i++) {
		inOutFile >> ipAddressString >> deviceSerialString;
		devices[i].setSerialNumber(deviceSerialString);
		devices[i].setIpAddress(ipAddressString);
	}

	for (int i=0; i<254; i++) {
		if (devices[i].getSerialNumber().compare(sn) == 0) {
			devices[i].setSerialNumber(EMPTY);
			isBackIPOK = true;
			break;
		}
	}

	// write back to ip pool
	fstream writeBack(&getIPPoolDataFile()[0], ios::out);
	for (int i=0; i<254; i++) {
		writeBack << devices[i].getIpAddress().toString() << " " << devices[i].getSerialNumber() << endl;
	}

	IptLog::getInstance()->logDebug("IPDispatcher::backIPAddress: Leave Critical Section.");
	return isBackIPOK;
}

bool IPDispatcher::initIPAddressPool() {

	ofstream initFile(&getIPPoolDataFile()[0], ios::out );
	if (!initFile ) {
		IptLog::getInstance()->logAlert(
				"IPDispatcher::initIPAddressPool: "
				"IP Pool Cannot be create. Please check the directory"+
				getIPPoolDataFile()
				);
		throw IpPoolCrashException();
	}

	XDevice device;
	string addressString;
	stringstream ss;

	for (int i=1; i<=254; i++) {
		// initilize string & device
		addressString = IPT_SUB_NETWORK_PRE;
		ss.str("");
		ss.clear();

		// seting
		ss << i;
		addressString.append(ss.str());

		// assign device information for device
		if (addressString == IPT_SUB_NETWORK_GATEWAY ) {
			device.setSerialNumber("111111111111");
		} else {
			device.setSerialNumber(EMPTY);
		}
		device.setIpAddress(addressString);

		initFile << device.getIpAddress().toString() << " "
				<< device.getSerialNumber() << endl;
	}
	return true;
}

bool IPDispatcher::removeIPAddressPool() {
	char* ipPoolFileName = &getIPPoolDataFile()[0];
	if (remove(ipPoolFileName)!=0)
		return false;
	else {
		return true;
	}
}
