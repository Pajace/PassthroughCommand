/*
 * Arguments.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Pajace
 */
#include <string>
#include <iostream>
#include <sstream>

#include <string.h>
#include <stdio.h>
#include <syslog.h>

#include "Arguments.h"
#include "IptCmd.h"
#include "../Resource.h"
using namespace std;

Arguments::Arguments() {
}

string Arguments::getUsage() {
	string usage;
#ifdef _DEBUG_
	usage = "-- DEBUG Version --\n";
#else
	usage = "";
#endif
	usage.append("ipt v" + IPT_VERSION + " for Ubuntu "+ IPT_BINARY_TYPE + "\n");
	usage.append("Usage: ipt [Command] [OPTIONS] [ON|OFF]\n");
	usage.append("Enable or disable the IPT(Internet Pass Through) on Android phone device.\n\n");

	usage.append("Command:\n");
	usage.append("init                       Initial and setup ipt environment.(only for first boot)\n");
	usage.append("s <DEVICE_SERIAL_NUMBER>   For specific android device.\n");

	usage.append("\nOPTIONS:\n");
//	usage.append("-d <IP_ADDRESS>            Set the domain name server for Android device.(The default\n");
//	usage.append("                           is from network configuration of system.)\n");
	usage.append("-p <ipt_productId>         Enable the IPT state to persistence.(IPT state recovery automatically)\n");
	usage.append("                           (SAGA, PYD, Express, OBOEA, Ruby, Holiday, ShooterU, and Kingdom are 0ffc)\n");
	usage.append("-h                ,--help  Display this help and exit\n");
	usage.append("\n");
	usage.append("Example:\n");
	usage.append(" Turn ATSSAGA10030 IPT ON: \n");
	usage.append("    #ipt s ATSSAGA10030 ON\n");
	usage.append(" Turn ATSSAGA10030 IPT  ON with persistent mode:\n");
	usage.append("    #ipt s ATSSAGA10030 ON -p 0ffc\n");
	usage.append("\n");
	usage.append("How to find the ipt Product ID:\n");
	usage.append("  1. Enable ipt on your device.\n");
	usage.append("  2. Run `lsusb` to identify your device. Example\n");
	usage.append("     root@ubuntuDesktop:/var/log# lsusb\n");
	usage.append("     Bus 002 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub\n");
	usage.append("     Bus 001 Device 039: ID 0bb4:0ffc High Tech Computer Corp.\n");
	usage.append("     Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub\n");
	usage.append("  3. After you doing this you know that\n");
	usage.append("     - the vender ID of your device is 0bb4.\n");
	usage.append("     - the product ID of your device is 0ffc.(If you enable ipt or it will be another.\n");
	usage.append("\n");

	return usage;
}

bool Arguments::validateIpAddress(const string ipAddress) {
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
	// check format

	return false;
}

bool Arguments::validateUsbCode(const string usbNetCode) {

	if (usbNetCode.size()!=4)
		return false;
	return true;
}

bool Arguments::validateSerialNumber(const string serialNumber) {

	if (serialNumber.size()!=12)
		return false;
	else {
		// if contain white space is not allow.
		char testContinerChar[serialNumber.size()];
		strcpy(testContinerChar, serialNumber.c_str());
		for(unsigned int i=0; i<serialNumber.size(); i++) {
			if(testContinerChar[i]==' ')
				return false;
		}
	}
	return true;
}

string Arguments::CharPointerArrayToString(int argc, char* argv[]) {
	string result;
	for (int i=0; i<argc;i++) {
		result+=argv[i];
		if (i!=(argc-1))
			result+=" ";
	}
	return result;
}

class Flag{
public:
	int flagCount;
	string content;
	Flag() {
		flagCount=0;
		content = "";
	}
};

Flag getFlag(string userCmd[], int arrSize, string flag1) {
	Flag result;
	for (int i=1; i<arrSize; i++) {
		if (userCmd[i] == flag1) {
			result.flagCount++;
			if (flag1!="ON" || flag1!="OFF" || flag1!="init")
				i+=1;
			result.content = userCmd[i];
		}
	}
	return result;
}

Flag getHelpFlag(string userCmd[], int arrSize, string flag1, string flag2) {
	Flag result;
	for (int i=1; i<arrSize; i++) {
		if (userCmd[i] == flag1 || userCmd[i] == flag2) {
			result.flagCount++;
		}
	}
	return result;
}

bool isErrorCatched(IptCmd iptCmd) {
	if (iptCmd.getIptCommand()==0)
		return false;
	return true;
}

string getAnyUnkowArgs(string userCmd[], int arrSize, string args[], int argsSize) {
	string result = "";
	for (int i=1; i<arrSize; i++) {
		result = userCmd[i];
		for (int j=0; j<argsSize; j++) {
			if (result==args[j]) {
				result = "";
				if (userCmd[i]==args[0] || userCmd[i]==args[1] || userCmd[i]==args[2] || userCmd[i]==args[3])
					i++;
				break;
			}
			else if (result!=""&&j==(argsSize-1))
				return result;
		}
	}
	return result;
}
string Arguments::getSerialNumber() const {
	return serialNumber;
}
string Arguments::getDnsIP() const {
	return dnsIP;
}
string Arguments::getIptProductID() const {
	return iptProductId;
}
string Arguments::getRndisName() const {
	return rndisName;
}

IptCmd Arguments::getIPTCommand(string cmdString2) {


	string argv[100];

	// separate string by white space
	int whitespace = 0;
	for (string::size_type ix =0, i=0; ix !=cmdString2.size(); ix++) {
		bool isContinueWhiteSpace = false;
		if (cmdString2[ix]==' ' && isContinueWhiteSpace==false) {
			whitespace++;
			i++;
			isContinueWhiteSpace = true;
		} else {
			isContinueWhiteSpace = false;
			argv[i] += cmdString2[ix];
		}
	}
	int argc = (whitespace+1);

	// main flow
	IptCmd iptCmd;
	iptCmd.setIptCommand(0);
	iptCmd.setIptDescription("");

	// If no arguments
	if (argc==1) {
		iptCmd.setIptCommand(IptCmd::NO_ANY_ARGUMENTS);
#ifdef _DEBUG_
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("-- DEBUG Version --\n"));
#endif
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt for Ubuntu " + IPT_BINARY_TYPE + " version " + IPT_VERSION + "\n"));
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("You have to specify Android device's serial number and ON|OFF.\n"));
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("Try `ipt --help` or `ipt -h` for more information.\n"));
		return iptCmd;
	}

	// Has arguments
	static const string SERIAL_ARG = "s";
	static const string DNS_ARG = "-d";
	static const string PERSISTENT_ARG = "-p";

	static const string BIND_RNDIS = "-b";
	static const string ON_ARG = "ON";
	static const string OFF_ARG = "OFF";

	static const string HELP_ARG1 = "-h";
	static const string HELP_ARG2 = "--help";

	static const string INIT_IPT  = "init";

	Flag sFlag = getFlag(argv, argc, SERIAL_ARG);
	Flag dFlag = getFlag(argv, argc, DNS_ARG);
	Flag pFlag = getFlag(argv, argc, PERSISTENT_ARG);

	Flag bFlag = getFlag(argv, argc, BIND_RNDIS);
	Flag onFlag = getFlag(argv, argc, ON_ARG);
	Flag offFlag = getFlag(argv, argc, OFF_ARG);

	Flag helpFlag = getHelpFlag(argv, argc, HELP_ARG1, HELP_ARG2);

	Flag initFlag = getFlag(argv, argc, INIT_IPT);

	const int argsSize = 9;
	string args[argsSize] = {
			SERIAL_ARG, DNS_ARG,   PERSISTENT_ARG,
			BIND_RNDIS, ON_ARG,    OFF_ARG,
			HELP_ARG1,	HELP_ARG2, INIT_IPT
	};


	// 1. display help or not
	if (helpFlag.flagCount==1) {
		iptCmd.setIptCommand(IptCmd::HELP);
		return iptCmd;
	}

	// 2. catch error
	string unKnowargs = getAnyUnkowArgs(argv, argc, args, argsSize);
	if (unKnowargs!="") {
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt: Unknown argument. -> "+ unKnowargs + "\n"));
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::UNKOWN_PARAMETER);
	}

	// serial number
	if ( sFlag.flagCount == 0 && bFlag.flagCount == 0 && initFlag.flagCount==0) {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::ERROR_SERIAL);
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt: Missing s argument.\n"));
	} else if (sFlag.flagCount > 1) {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::ERROR_SERIAL);
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt: It cannot assign more than one device at time.\n"));
	} else if ( sFlag.flagCount==1 && validateSerialNumber(sFlag.content)==false) {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::ERROR_SERIAL);
		iptCmd.setIptDescription(
				iptCmd.getIptDescription().append(
						"ipt: [" + sFlag.content + "] The format of device's serial number is not valid. It must have 12 digit.\n")
		);
	} else {} // No error occur

	// dns server
	if ( dFlag.flagCount > 1 ) {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::INVALID_IP);
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt: It cannot assign more than one dns server ip at time. \n"));
	} else if ( dFlag.flagCount==1 && validateIpAddress(dFlag.content)==false ) {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::INVALID_IP);
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt: [" + dFlag.content + "] : The IP address is invalid. - \n"));
	} else {}

	// usb code
	if ( pFlag.flagCount > 1) {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::INVALID_USBCODE);
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt: It cannot assign more then one USB code at time.\n"));
	} else if ( pFlag.flagCount==1 &&validateUsbCode(pFlag.content)==false) {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::INVALID_USBCODE);
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt: ["+ pFlag.content +"] is not a valid product ID.\n"));
	}

	// switch flag
	if (onFlag.flagCount>1 || offFlag.flagCount>1) {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::ERROR_ONOFF);
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt: ON or OFF cannot assign more than once.\n"));
	} else if (onFlag.flagCount==1 && offFlag.flagCount==1) {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::ERROR_ONOFF);
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt: On and OFF cannot assign at the same time.\n"));
	} else if (onFlag.flagCount==0 && offFlag.flagCount==0 && bFlag.flagCount == 0 && initFlag.flagCount==0) {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::ERROR_ONOFF);
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt: Missing ON or OFF arguments.\n"));
	} else if (offFlag.flagCount==1 && (pFlag.flagCount>=1 || dFlag.flagCount>=1 || helpFlag.flagCount>=1))  {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::ERROR_ONOFF);
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("ipt: If you assign the ipt \"OFF\", then you don't have to \n"));
		iptCmd.setIptDescription(iptCmd.getIptDescription().append("     assign any arguments.\n"));
	}

	// BindRNDIS flag
	if (bFlag.flagCount == 1 && bFlag.content[0]!='u' && bFlag.content[1]!='s' && bFlag.content[2]!='b') {
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::OTHER_ERROR);
		iptCmd.setIptDescription("Ipt: no RNDIS name after -b.");
	}


	if (isErrorCatched(iptCmd)) {
		return iptCmd;
	}

	// ==============Decide Command==========================================================

	if ( sFlag.flagCount==1 && onFlag.flagCount==1 ) {
		serialNumber = sFlag.content;
		iptCmd.setIptCommand(IptCmd::IPT_ON);
		iptCmd.setIptDescription("ipt: Device: " + sFlag.content + ", set ipt on.\n");
	} else if (sFlag.flagCount==1 && offFlag.flagCount==1) {
		serialNumber = sFlag.content;
		iptCmd.setIptCommand(IptCmd::IPT_OFF);
		iptCmd.setIptDescription("ipt: Device: " + sFlag.content + ", set ipt off.\n");
	} else if (bFlag.flagCount == 1 && pFlag.flagCount!=1 && dFlag.flagCount!=1) {
		rndisName = bFlag.content;
		iptCmd.setIptCommand(IptCmd::BIND_RNDIS_ON_BRIDGE);
		iptCmd.setIptDescription("ipt: Bind RNIDS on Bridge\n");
	} else if (initFlag.flagCount==1 ) {
		iptCmd.setIptCommand(IptCmd::INIT_IPT);
		iptCmd.setIptDescription("ipt: initialize ipt environment.\n");
	} else {
		iptCmd.setIptCommand(IptCmd::MISSING_ERROR);
		iptCmd.setIptDescription("ipt: Missing error on decide command.\n");
	}

	if ( pFlag.flagCount==1 ) {
		iptProductId = pFlag.content;
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::IPT_SET_USBCODE);
		iptCmd.setIptDescription("ipt: with usb code.\n");
	}
	if ( dFlag.flagCount==1 ) {
		dnsIP = dFlag.content;
		iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::IPT_SET_DNS);
		iptCmd.setIptDescription("ipt: with DNS IP.\n");
	}


	return iptCmd;
}
