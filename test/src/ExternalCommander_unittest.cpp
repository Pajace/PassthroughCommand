/*
 * ExternalCommander_unittest.cpp
 *
 *  Created on: Apr 10, 2012
 *      Author: pajace
 */

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../../ap/ExternalCommander.h"
#include "../../Resource.h"
using namespace std;

//TEST(ExternalCommander, getSetupIptEnvironmentCmdTest) {
//	string bridgeIfName = "TestBridgeIf";
//	string dns = ExternalCommander::getInstance()->getInternalNetworkDNS();
//
//	vector<string> expectedCommand(0);
//	expectedCommand.push_back("brctl addbr " + bridgeIfName );
//	expectedCommand.push_back("ifconfig " + bridgeIfName + " " + IPT_SUB_NETWORK_GATEWAY);
//	expectedCommand.push_back("echo 1 > /proc/sys/net/ipv4/ip_forward");
//	expectedCommand.push_back("iptables -A POSTROUTING -t nat -s " + IPT_SUB_NETWORK_IP +" -j MASQUERADE");
//	expectedCommand.push_back("iptables -I PREROUTING  -t nat -p udp --dport 53 -d " + IPT_SUB_NETWORK_GATEWAY + " -j DNAT --to " + dns);
//	expectedCommand.push_back("sysctl -p");
//	expectedCommand.push_back("modprobe usbnet");
//
//	vector<string> actualCommand = ExternalCommander::getInstance()->getSetupIptEnvironmentCmd(bridgeIfName);
//
//	ASSERT_EQ(expectedCommand.size(), actualCommand.size());
//	for(unsigned int i=0; i<expectedCommand.size(); i++)
//		EXPECT_EQ(expectedCommand.at(i), actualCommand.at(i));
//}
//
//TEST(ExternalCommander, getBindBridgeCommandTest) {
//	string bridgeName 		= "TestedBridge";
//	string bindedInterface	= "usb0";
//
//	vector<string> expectedCommand(0);
//	expectedCommand.push_back("sleep 2");
//	expectedCommand.push_back("brctl addif " + bridgeName + " " + bindedInterface);
//
//	vector<string> actualCommand = ExternalCommander::getInstance()->getBindBridgeCmd(bridgeName,bindedInterface);
//
//	ASSERT_EQ(expectedCommand.size(), actualCommand.size());
//	for(unsigned int i=0; i<expectedCommand.size(); i++)
//		EXPECT_EQ(expectedCommand.at(i), actualCommand.at(i));
//
//}
