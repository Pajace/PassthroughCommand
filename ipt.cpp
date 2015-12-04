//============================================================================
// Name        : ipt.cpp
// Author      : Pajace
// Version     :
// Copyright   : Your copyright notice
// Description : ipt command for Ubuntu 64bit
//============================================================================

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include "ui/Arguments.h"
#include "ui/IptCmd.h"
#include "ap/IptException.h"
#include "ap/IptFacade.h"

using namespace std;

Arguments args;
enum SWITCH{ON, OFF};

void displayMessage(string message) {
	cout << message ;
	IptLog::getInstance()->logInfo("ipt::main: " + message);
}

void displaySuccessMsg(int iptSwitch) {
	switch (iptSwitch) {
	case ON:
		displayMessage(" - Enable [" +args.getSerialNumber()+"] - [OK]\n");
		break;
	case OFF:
		displayMessage(" - Disable [" +args.getSerialNumber()+"] - [OK]\n");
		break;
	}

}

bool runIptCommand(unsigned short iptCmd) {
	try {
		switch (iptCmd) {
			case IptCmd::IPT_ON:
				IptFacade::getInstance()->enableIpt(args.getSerialNumber());
				break;

			case IptCmd::IPT_ON_USB:
				IptFacade::getInstance()->enablePersistentIpt(args.getSerialNumber(), args.getIptProductID());
				break;

			case IptCmd::IPT_OFF:
				IptFacade::getInstance()->disableIpt(args.getSerialNumber());
				sleep(5);
				break;
		}
	} catch (DeviceWasnotPluggedException &e) {
		displayMessage(" [Fail] : " + args.getSerialNumber() + " - This device was not plugged.\n");
		return false;
	} catch (IptAlreadyEnableException &e1 ) {
		displayMessage(" [Fail] : " + args.getSerialNumber() + " - This device has already turn ipt on.\n");
		return false;
	} catch (IptAlreadyDisableException &e3) {
		displayMessage(" [Fail] : " + args.getSerialNumber() + " - This device has already disable ipt mode.\n");
		return false;
	} catch (IptEnvironmentDidnotSetupException &e4) {
		displayMessage(" [Fail] : You have to initial ipt environment first (typing `ipt init` or add ipt initialize command in /etc/rc.local\n");
		return false;
	} catch(NoAdbCommandException &e) {
		string msg(e.what());
		displayMessage(" [Fail] : "+msg+"\n");
		return false;
	} catch(NoBridgeUtilsException &e) {
		string msg(e.what());
		displayMessage(" [Fail] : "+msg+"\n");
		return false;
	} catch(IptDirectoryErrorException &e ){
		string msg(e.what());
		displayMessage(" [Fail] : "+msg+"\n");
		return false;
	} catch(runtime_error &e) {
		string msg(e.what());
		displayMessage(" [Fail] : "+msg+"\n");
		return false;
	}

	return true;
}

int main(int argc, char *argv[]) {

	string userInput = args.CharPointerArrayToString(argc, argv);
	IptCmd userCommand = args.getIPTCommand(userInput);

	unsigned short cmd = userCommand.getIptCommand();

	switch (cmd) {
		case IptCmd::HELP:
			cout << args.getUsage();
			break;

		case IptCmd::IPT_ON:
			displayMessage("[" + args.getSerialNumber() + "] Enable IPT \n");
			if (runIptCommand(cmd)==true)
				displaySuccessMsg(ON);
			break;

		case IptCmd::IPT_ON_DNS:
			displayMessage("[" + args.getSerialNumber() + "] Enable IPT with DNS " + args.getDnsIP() + " \n");
			if (runIptCommand(cmd)==true)
				displaySuccessMsg(ON);
			break;

		case IptCmd::IPT_ON_USB:
			displayMessage("[" + args.getSerialNumber() + "] Enable Persistent IPT \n");
			if (runIptCommand(cmd)==true)
				displaySuccessMsg(ON);
			break;

		case IptCmd::IPT_ON_DNS_USB:
			displayMessage("[" + args.getSerialNumber() + "] Enable persistent IPT with DNS " + args.getDnsIP() + "\n");
			if(runIptCommand(cmd)==true)
				displaySuccessMsg(ON);
			break;

		case IptCmd::IPT_OFF:
			displayMessage("[" + args.getSerialNumber() + "] Disable IPT Mode \n");
			if(runIptCommand(cmd)==true)
				displaySuccessMsg(OFF);
			break;

		case IptCmd::BIND_RNDIS_ON_BRIDGE:
			IptFacade::getInstance()->bindRndisOnBridge(args.getRndisName());
			cout << "Bind Bridge with " << args.getRndisName() << endl;
			break;

		case IptCmd::INIT_IPT:

			if(IptFacade::getInstance()->setupIptEnvironment())
				displayMessage("ipt: set up ipt environment [OK].\n");
			else
				displayMessage("ipt: set up ipt environment [Fail].\n     You has already initialize the ipt environment before.\n");
			break;

		default:
			if (userCommand.getIptCommand()>=16 && userCommand.getIptCommand()!=IptCmd::BIND_RNDIS_ON_BRIDGE) {
				cerr << userCommand.getIptDescription();
				IptLog::getInstance()->logInfo(userCommand.getIptDescription());
			} else
				IptLog::getInstance()->logAlert(userCommand.getIptDescription());
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
