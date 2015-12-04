/*
 * IPDispatcher_unittest.cpp
 *
 *  Created on: Aug 29, 2011
 *      Author: pajace
 */
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "../../ap/XDevice.h"
#include "../../ap/IPaddress.h"
#include "../../ap/IPDispatcher.h"
#include "../../Resource.h"
using namespace std;


TEST(IPDispatcher, TestInitialIPPool) {

	IPDispatcher::getInstance()->removeIPAddressPool();

	ifstream inFile(&IPDispatcher::getInstance()->getIPPoolDataFile()[0], ios::in);
	EXPECT_FALSE(inFile);

	bool isInitial = IPDispatcher::getInstance()->initIPAddressPool();
	EXPECT_TRUE(isInitial);

	inFile.open(&IPDispatcher::getInstance()->getIPPoolDataFile()[0], ios::in);
	EXPECT_TRUE(inFile);
	string ipAddressString;
	string expectedIPaddress;
	stringstream ss;
	string deviceSerialString;
	for (int i=0; i<254; i++) {
		ss.str("");
		ss.clear();
		ss << (i+1);
		expectedIPaddress = IPT_SUB_NETWORK_PRE;
		expectedIPaddress.append(ss.str());

		inFile >> ipAddressString >> deviceSerialString;
		EXPECT_EQ(expectedIPaddress, ipAddressString);
		if (i==253) {
			EXPECT_EQ("111111111111", deviceSerialString);
		} else {
			EXPECT_EQ("000000000000", deviceSerialString);
		}
	}
	IPDispatcher::getInstance()->removeIPAddressPool();
}

TEST(IPDispatcher, TestInitialAndRemove) {
	bool isRemove, isInitial;
	IPDispatcher::getInstance()->removeIPAddressPool();
	isRemove = IPDispatcher::getInstance()->removeIPAddressPool();
	EXPECT_FALSE(isRemove);
	isInitial = IPDispatcher::getInstance()->initIPAddressPool();
	EXPECT_TRUE(isInitial);
	isRemove = IPDispatcher::getInstance()->removeIPAddressPool();
	EXPECT_TRUE(isRemove);
}

TEST(IPDispatcher, TestGetIPAddress) {
	bool isInitIPPool = IPDispatcher::getInstance()->initIPAddressPool();
	EXPECT_TRUE(isInitIPPool);

	IPaddress ipaddress;
	string expectedIP;
	stringstream ss;
	string sn;
	for (int i=1; i<254; i++) {
		ss.str("");
		ss.clear();
		ss << i;
		expectedIP = IPT_SUB_NETWORK_PRE;
		expectedIP.append(ss.str());

		if (i<10) {
			sn = "ATSTESTIPT0";
		} else if (i>=10 && i<100) {
			sn = "ATSTESTIPT";
		} else {
			sn = "ATSTESTIP";
		}
		sn.append(ss.str());

		ipaddress = IPDispatcher::getInstance()->getIPAddress(sn);
		EXPECT_EQ(expectedIP, ipaddress.toString());
	}
	EXPECT_TRUE(IPDispatcher::getInstance()->removeIPAddressPool());
}

TEST(IPDispatcher, TestBackIPAddress) {
	bool isInitIPPool = IPDispatcher::getInstance()->initIPAddressPool();
	EXPECT_TRUE(isInitIPPool);

	// Consum all ip address first
	IPaddress ipaddress;
	string expectedIP;
	stringstream ss;
	string sn;
	for (int i=1; i<300; i++) {
		ss.str("");
		ss.clear();
		ss << i;
		expectedIP = IPT_SUB_NETWORK_PRE;
		expectedIP.append(ss.str());

		if (i<10) {
			sn = "ATSTES00000";
		} else if (i>=10 && i<100) {
			sn = "ATSTEST000";
		} else {
			sn = "ATSTEST00";
		}
		sn.append(ss.str());

		if (i<254) {
			ipaddress = IPDispatcher::getInstance()->getIPAddress(sn);
			EXPECT_EQ(expectedIP, ipaddress.toString());
		} else { // It had exhausted IP address of IP pool
			ipaddress = IPDispatcher::getInstance()->getIPAddress(sn);
			EXPECT_EQ("1.0.0.0", ipaddress.toString());
		}

	}

	IPDispatcher::getInstance()->backIPAddress("ATSTEST00020");
	IPDispatcher::getInstance()->backIPAddress("ATSTEST00030");
	IPDispatcher::getInstance()->backIPAddress("ATSTEST00040");
	IPDispatcher::getInstance()->backIPAddress("ATSTEST00050");
	IPDispatcher::getInstance()->backIPAddress("ATSTEST00060");
	IPDispatcher::getInstance()->backIPAddress("ATSTEST00070");

	ipaddress = IPDispatcher::getInstance()->getIPAddress("ATSTEST00011");
	EXPECT_EQ(IPT_SUB_NETWORK_PRE+"11", ipaddress.toString());

	ipaddress = IPDispatcher::getInstance()->getIPAddress("ATSTEST00021");
	EXPECT_EQ(IPT_SUB_NETWORK_PRE+"21", ipaddress.toString());

	ipaddress = IPDispatcher::getInstance()->getIPAddress("ATSTEST00031");
	EXPECT_EQ(IPT_SUB_NETWORK_PRE+"31", ipaddress.toString());

	ipaddress = IPDispatcher::getInstance()->getIPAddress("ATSTEST00041");
	EXPECT_EQ(IPT_SUB_NETWORK_PRE+"41", ipaddress.toString());

	ipaddress = IPDispatcher::getInstance()->getIPAddress("ATSTEST00051");
	EXPECT_EQ(IPT_SUB_NETWORK_PRE+"51", ipaddress.toString());

	// get ip with not use serial
	ipaddress = IPDispatcher::getInstance()->getIPAddress("XXXXXXXXXXXX");
	EXPECT_EQ(IPT_SUB_NETWORK_PRE+"20", ipaddress.toString());

	ipaddress = IPDispatcher::getInstance()->getIPAddress("XOXOXOXOXOXO");
	EXPECT_EQ(IPT_SUB_NETWORK_PRE+"30", ipaddress.toString());

	ipaddress = IPDispatcher::getInstance()->getIPAddress("------------");
	EXPECT_EQ(IPT_SUB_NETWORK_PRE+"40", ipaddress.toString());

	EXPECT_TRUE(IPDispatcher::getInstance()->removeIPAddressPool());
}

