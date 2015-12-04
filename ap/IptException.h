/*
 * IptException.h
 *
 *  Created on: Sep 1, 2011
 *      Author: pajace
 */

#ifndef IPTEXCEPTION_H_
#define IPTEXCEPTION_H_

#include <stdexcept>
#include "../Resource.h"
using namespace std;

class IptAlreadyEnableException : public runtime_error {
public:
	IptAlreadyEnableException():runtime_error("ipt: This device has already enable ipt mode."){};
};

class IptAlreadyDisableException : public runtime_error {
public:
	IptAlreadyDisableException():runtime_error("ipt: This device has already disable ipt mode."){};
};

class DeviceWasnotPluggedException : public runtime_error {
public:
	DeviceWasnotPluggedException() : runtime_error("ipt: This device was not plugged!!"){};
};

class IpPoolCrashException : public runtime_error {
public:
	IpPoolCrashException() : runtime_error("ipt: IP pool crash exception"){};
};

class AndroidDeviceSnFormatIncorrectException : public runtime_error {
public:
	AndroidDeviceSnFormatIncorrectException() : runtime_error("ipt: The format of Android device's format is not correct."){};
};

class IptEnvironmentDidnotSetupException : public runtime_error {
public:
	IptEnvironmentDidnotSetupException() : runtime_error("ipt: You have to set up and initial ipt environment first."){};
};

class NoAdbCommandException : public runtime_error {
public:
	NoAdbCommandException() : runtime_error("ipt: You have to install "+ ADB_COMMAND +" command first or set up the PATH include that command."){};
};

class NoBridgeUtilsException : public runtime_error {
public:
	NoBridgeUtilsException() : runtime_error("ipt: You have to install brctl. You can install it by typing:\n apt-get install bridge-utils."){};
};

class IptDirectoryErrorException : public runtime_error {
public:
	IptDirectoryErrorException() : runtime_error("ipt: cannot access /usr/local/sbin/ipt\n Please put the ipt command into /usr/local/sbin."){};
};

#endif /* IPTEXCEPTION_H_ */
//DeviceWasnotPluggedException
