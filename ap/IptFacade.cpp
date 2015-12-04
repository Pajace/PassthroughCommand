/*
 * IptFacade.cpp
 *
 *  Created on: Aug 24, 2011
 *      Author: pajace
 */

#include <stdlib.h>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include "IptFacade.h"
#include "IPDispatcher.h"
#include "RNDISController.h"
#include "IPaddress.h"
#include "XDevice.h"
#include "IptException.h"
#include "ExternalCommander.h"
#include "IptLog.h"
#include "IptCommand.h"
#include "../Resource.h"

#include "Semaphore.h"

using namespace std;

// Global static pointer used to ensure a single instance of the class.
IptFacade* IptFacade::m_pInstance = NULL;

bool isIptDirectoryCorrect() {
	string iptcmd = ExternalCommander::getInstance()->getExternalCommandResult("ls " + IPT_COMMAND_DIRECTORY + "ipt");
	if ((iptcmd.find("/usr/local/sbin/ipt")) == STRING_NOT_FOUND)
		return false;
	return true;
}

bool checkAdbcommandAvailable() {

	string adbCommand =
			ExternalCommander::getInstance()->getExternalCommandResult(
					ADB_COMMAND + " 2>&1");
	if ((adbCommand.find("Android Debug Bridge version")) == STRING_NOT_FOUND) {
		IptLog::getInstance()->logDebug(adbCommand);
		return false;
	}
	return true;
}

bool isIptBridgeExist() {

	string isBridgeExist =
			ExternalCommander::getInstance()->getExternalCommandResult("brctl show | grep " + BRIDGE_IF_NAME + " | awk {'print$1'} ");
	if( isBridgeExist.empty() )
		return false;
	return true;

}

bool checkBridgeUtilsAvaliable() {
	string bridgeUtils =
			ExternalCommander::getInstance()->getExternalCommandResult(
					"brctl 2>&1");
	if ((bridgeUtils.find("Usage: brctl [commands]")) == STRING_NOT_FOUND)
		return false;
	return true;
}

void checkDevicePluggedOrNot(XDevice device) throw (runtime_error) {
	if (device.isDevicePlugged("") == false) {
		IptLog::getInstance()->logError(
				"IptFacade::checkDeviceCanTurnIptOfforNot: ["
						+ device.getSerialNumber()
						+ "] The device was not plugged.");
		throw DeviceWasnotPluggedException();
	}
}

void initialDevice(XDevice device) throw (runtime_error) {

	checkDevicePluggedOrNot(device);

	if (device.isDeviceEnableIptMode("") == true) {
		IptLog::getInstance()->logError("IptFacade::initialDevice: ["
				+ device.getSerialNumber()
				+ "] The device has already enable ipt mode.");
		throw IptAlreadyEnableException();
	}
}

void checkDeviceCanTurnIptOfforNot(XDevice device) throw (runtime_error) {

	checkDevicePluggedOrNot(device);

	if (device.isDeviceEnableIptMode("") == false) {
		IptLog::getInstance()->logError(
				"IptFacade::checkDeviceCanTurnIptOfforNot: ["
						+ device.getSerialNumber()
						+ "] The device has already disable ipt mode.");
		throw IptAlreadyDisableException();
	}
}

void IptFacade::checkIptEnvironment() {

	if (checkBridgeUtilsAvaliable() == false) {
		IptLog::getInstance()->logError("IptFacade::checkServiceAvailable: No bridge-utilis.");
		throw NoBridgeUtilsException();
	}

	if (checkAdbcommandAvailable() == false) {
		IptLog::getInstance()->logError("IptFacade::checkIptEnvironment: No adb command. ");
		throw NoAdbCommandException();
	}

	ifstream testFile(IPDispatcher::getInstance()->getIPPoolDataFile().c_str(),	ios::in);
	if (testFile == false) {
		IptLog::getInstance()->logError("IptFacade::checkIptEnvironment: No ipt_ip_pool.");
		throw IptEnvironmentDidnotSetupException();
	}

	if (isIptDirectoryCorrect()==false) {
		IptLog::getInstance()->logError("IptFacade::checkIptEnvironment: The directory of ipt is wrong.");
		throw IptDirectoryErrorException();
	}
}

string requestIpAddress(string sn) {
	Semaphore::getInstance()->semaphore_p();
	IPaddress ipAddress = IPDispatcher::getInstance()->getIPAddress(sn);
	Semaphore::getInstance()->semaphore_v();
	return ipAddress.toString();
}

IptFacade* IptFacade::getInstance() {
	if (!m_pInstance) { // only allow one instance of class to be generated.
		m_pInstance = new IptFacade();
		IptLog::getInstance()->setDebug(true);
		IptLog::getInstance()->setInfo(true);
	}
	Semaphore::getInstance()->sem_id
			= semget((key_t) 1234, 1, 0666 | IPC_CREAT);
	return m_pInstance;
}

bool IptFacade::setupIptEnvironment() {
	if (isIptBridgeExist())
		return false;
	vector<string> setupEvnCmd = IptCommand::getInstance()->GetSetupIptEnvironmentCmd();
	ExternalCommander::getInstance()->runExternalCommand(setupEvnCmd);

	IPDispatcher::getInstance()->initIPAddressPool();
	Semaphore::getInstance()->set_semvalue();
	RNDISController::generateConfigFile_AutoBindRndis(AUTO_BIND_RNDIS_CONFIG);
	ExternalCommander::getInstance()->getExternalCommandResult(ADB_COMMAND +
			" devices 2>&1");
	return true;
}

void IptFacade::enableIpt(string deviceSn) {

	checkIptEnvironment();
	XDevice device(deviceSn);
	initialDevice(device);
	device.setIpAddress(requestIpAddress(device.getSerialNumber()));

	// enable ipt
	vector<string> enableIptCmd = IptCommand::getInstance()->GetEnableIptCommand(device.getSerialNumber(), device.getIpAddress().toString());
	ExternalCommander::getInstance()->runExternalCommand(enableIptCmd);

	IptLog::getInstance()->logDebug("IptFacade::enableIpt: wait-for-device -> "
			+ deviceSn);
//	ExternalCommander::getInstance()->waitForDevice(deviceSn);
	string waitForDeviceCmd = IptCommand::getInstance()->GetWaitForDeviceCommand(deviceSn);
	ExternalCommander::getInstance()->runExternalCommand(waitForDeviceCmd);
}

void IptFacade::enablePersistentIpt(string deviceSN, string iptProductId) {

	// #1
	enableIpt(deviceSN);
	RNDISController rndis;
	// #2
	Semaphore::getInstance()->semaphore_p();
	bool isEnablePersistentIpt = rndis.enablePersistent(deviceSN, iptProductId);
	Semaphore::getInstance()->semaphore_v();

	if (isEnablePersistentIpt==false)
		IptLog::getInstance()->logAlert("IptFacade::enablePersistentIpt: enablePersistentIPt fail.");


}

void IptFacade::disableIpt(string deviceSerialNumber) {

	XDevice device(deviceSerialNumber);
	checkDeviceCanTurnIptOfforNot(device);

	RNDISController rndis;
	Semaphore::getInstance()->semaphore_p();
	rndis.disablePersistent(deviceSerialNumber);
	Semaphore::getInstance()->semaphore_v();

	try {
		Semaphore::getInstance()->semaphore_p();
		IPDispatcher::getInstance()->backIPAddress(deviceSerialNumber);
		Semaphore::getInstance()->semaphore_v();
	} catch (IpPoolCrashException &e) {
		IPDispatcher::getInstance()->initIPAddressPool();
		string errormsg(e.what());
		IptLog::getInstance()->logError(
				"IptFacade::disableIpt: backIPaddress\n" + errormsg);
	}
	vector<string> disableIptCmd = IptCommand::getInstance()->GetDisableIptCommand(deviceSerialNumber);
	ExternalCommander::getInstance()->runExternalCommand(disableIptCmd);

	IptLog::getInstance()->logDebug(
			"IptFacade::disableIpt: wait-for-device -> " + deviceSerialNumber);
	string waitForDeviceCmd = IptCommand::getInstance()->GetWaitForDeviceCommand(deviceSerialNumber);
	ExternalCommander::getInstance()->runExternalCommand(waitForDeviceCmd);
//	ExternalCommander::getInstance()->waitForDevice(deviceSerialNumber);

}

void IptFacade::bindRndisOnBridge(string rndisName) {
	vector<string> bindBridgeCmd = IptCommand::getInstance()->GetBindBridgeCommand(rndisName);
	ExternalCommander::getInstance()->runExternalCommand(bindBridgeCmd);
}

IptFacade::~IptFacade() {
	//	delete m_pInstance;
}
