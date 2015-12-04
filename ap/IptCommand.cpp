/*
 * IptCommand.cpp
 *
 *  Created on: Apr 10, 2012
 *      Author: pajace_chen
 */

#include "IptCommand.h"
#include "ExternalCommander.h"

IptCommand* IptCommand::m_pInstance = NULL;
IptCommand* IptCommand::getInstance() {
	if (!m_pInstance)
		m_pInstance = new IptCommand();
	return m_pInstance;
}

vector<string> IptCommand::GetSetupIptEnvironmentCmd() {
	vector<string> command(0);

	command.push_back("brctl addbr " + BRIDGE_IF_NAME );
	command.push_back("ifconfig " + BRIDGE_IF_NAME + " " + IPT_SUB_NETWORK_GATEWAY);
	command.push_back("echo 1 > /proc/sys/net/ipv4/ip_forward");
	command.push_back("iptables -A POSTROUTING -t nat -s " + IPT_SUB_NETWORK_IP +" -j MASQUERADE");
	command.push_back("iptables -I PREROUTING -t nat -p udp --dport 53 -s " + IPT_SUB_NETWORK_IP + " -d " + IPT_SUB_NETWORK_GATEWAY + " -j DNAT --to " + ExternalCommander::getInstance()->getInternalNetworkDNS());
	command.push_back("sysctl -p");
	command.push_back("modprobe usbnet");
	return command;
}

vector<string> IptCommand::GetEnableIptCommand(string deviceSN, string ipAddress){
	vector<string> command(0);
	const string adbCmd = ADB_COMMAND + " -s " + deviceSN;
	command.push_back(adbCmd + " wait-for-devices ");
	command.push_back(adbCmd + " shell netsharing test 2");
	command.push_back(adbCmd + " shell setprop net.usb0.ps.ip " + ipAddress);
	command.push_back(adbCmd + " shell setprop net.usb0.ps.gw " + IPT_SUB_NETWORK_GATEWAY );
	command.push_back(adbCmd + " shell setprop net.usb0.ps.dns " + IPT_DNS );
	command.push_back(adbCmd + " shell setprop ctl.start netsharing_pass");
	command.push_back("sleep 1");
	command.push_back(adbCmd + " wait-for-device");
	command.push_back(adbCmd + " shell am broadcast -a com.android.net.usbnet.STATE_NOTIFY --ei newNetworkState 2");
	return command;
}

vector<string> IptCommand::GetDisableIptCommand(string deviceSN){
	vector<string> command(0);
	const string adbCmd = ADB_COMMAND + " -s " + deviceSN;
	command.push_back(adbCmd + " shell netsharing net_pass off");
	command.push_back(adbCmd + " wait-for-devices");
	command.push_back(adbCmd + " shell am broadcast -a com.android.net.usbnet.STATE_NOTIFY --ei newNetworkState 0");
	return command;
}

vector<string> IptCommand::GetBindBridgeCommand(string bindedInterface){
	vector<string> command(0);
	command.push_back("sleep 2");
	command.push_back("brctl addif " + BRIDGE_IF_NAME + " " + bindedInterface);
	return command;
}

string IptCommand::GetWaitForDeviceCommand(string deviceSN) {
	string command = ADB_COMMAND + " -s " + deviceSN + " wait-for-device";
	return command;
}

//IptCommand::~IptCommand() {
//	delete IptCommand::m_pInstance;
//}

