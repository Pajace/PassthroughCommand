/*
 * IptCommand.cpp
 *
 *  Created on: Apr 10, 2012
 *      Author: Pajace_Chen
 */
#include <gtest/gtest.h>
#include "../../ap/IptCommand.h"
#include "../../ap/ExternalCommander.h"
#include "../../ap/IptLog.h"
#include "../../Resource.h"
#include <fstream>
using namespace std;

TEST(IptCommand, TestGetSetupIptEnvironmentCmd) {
	vector<string> expectedCmd(0);
	expectedCmd.push_back("brctl addbr " + BRIDGE_IF_NAME );
	expectedCmd.push_back("ifconfig " + BRIDGE_IF_NAME + " " + IPT_SUB_NETWORK_GATEWAY);
	expectedCmd.push_back("echo 1 > /proc/sys/net/ipv4/ip_forward");
	expectedCmd.push_back("iptables -A POSTROUTING -t nat -s " + IPT_SUB_NETWORK_IP +" -j MASQUERADE");
	expectedCmd.push_back("iptables -I PREROUTING  -t nat -p udp --dport 53 -d " + IPT_SUB_NETWORK_GATEWAY + " -j DNAT --to " + ExternalCommander::getInstance()->getInternalNetworkDNS());
	expectedCmd.push_back("sysctl -p");
	expectedCmd.push_back("modprobe usbnet");

	vector<string> actualCmd = IptCommand::getInstance()->GetSetupIptEnvironmentCmd();

	ASSERT_EQ(expectedCmd.size(), actualCmd.size());
	for(unsigned int i=0; i<expectedCmd.size(); i++)
		EXPECT_EQ(expectedCmd.at(i), actualCmd.at(i));
}

TEST(IptCommand, TestGetEnableIptCommand) {
	const string ipAddress = "192.168.0.100";
	const string deviceSN  = "ANDROIDTEST00001";
	const string adbCmd = ADB_COMMAND + " -s " + deviceSN;

	vector<string> expectedCmd(0);
	expectedCmd.push_back(adbCmd + " wait-for-devices ");
	expectedCmd.push_back(adbCmd + " shell netsharing test 2");
	expectedCmd.push_back(adbCmd + " shell setprop net.usb0.ps.ip " + ipAddress);
	expectedCmd.push_back(adbCmd + " shell setprop net.usb0.ps.gw " + IPT_SUB_NETWORK_GATEWAY );
	expectedCmd.push_back(adbCmd + " shell setprop net.usb0.ps.dns " + IPT_DNS );
	expectedCmd.push_back(adbCmd + " shell setprop ctl.start netsharing_pass");
	expectedCmd.push_back("sleep 1");
	expectedCmd.push_back(adbCmd + " wait-for-device");
	expectedCmd.push_back(adbCmd + " shell am broadcast -a com.android.net.usbnet.STATE_NOTIFY --ei newNetworkState 2");

	vector<string> actualCmd = IptCommand::getInstance()->GetEnableIptCommand(deviceSN, ipAddress);

	ASSERT_EQ(expectedCmd.size(), actualCmd.size());
	for(unsigned int i=0; i<expectedCmd.size(); i++)
		EXPECT_EQ(expectedCmd.at(i), actualCmd.at(i));
}

TEST(IptCommand, TestGetDisableIptCommand) {

	const string deviceSN= "ANDROIDTEST00001";
	const string adbCmd  = ADB_COMMAND + " -s " + deviceSN;

	vector<string> expectedCmd(0);
	expectedCmd.push_back(adbCmd + " shell netsharing net_pass off");
	expectedCmd.push_back(adbCmd + " wait-for-devices");
	expectedCmd.push_back(adbCmd + " shell am broadcast -a com.android.net.usbnet.STATE_NOTIFY --ei newNetworkState 0");

	vector<string> actualCmd = IptCommand::getInstance()->GetDisableIptCommand(deviceSN);

	ASSERT_EQ(expectedCmd.size(), actualCmd.size());
	for(unsigned int i=0; i<expectedCmd.size(); i++)
		EXPECT_EQ(expectedCmd.at(i), actualCmd.at(i));
}

TEST(IptCommand, TestGetBindBridgeCommand) {
	string bindedInterface = "usb0";

	vector<string> expectedCmd(0);
	expectedCmd.push_back("sleep 2");
	expectedCmd.push_back("brctl addif " + BRIDGE_IF_NAME + " " + bindedInterface);

	vector<string> actualCmd = IptCommand::getInstance()->GetBindBridgeCommand(bindedInterface);
	ASSERT_EQ(expectedCmd.size(), actualCmd.size());
	for(unsigned int i=0; i<expectedCmd.size(); i++)
		EXPECT_EQ(expectedCmd.at(i), actualCmd.at(i));
}

TEST(IptCommand, TestGetWaitForDeviceCommand) {
	string deviceSN = "ANDROIDTEST00001";

	string expectedCmd = ADB_COMMAND + " -s " + deviceSN + " wait-for-device";
	string actualCmd = IptCommand::getInstance()->GetWaitForDeviceCommand(deviceSN);

	EXPECT_EQ(expectedCmd, actualCmd);
}
