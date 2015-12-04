/*
 * RNDISController_unittest.cpp
 *
 *  Created on: Aug 30, 2011
 *      Author: pajace
 */
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include "../../ap/RNDISController.h"
using namespace std;

TEST(RNDISController, IsHeader) {
	RNDISController rndis;
	EXPECT_TRUE(isHeader(rndis, "#-///ATSSAGA00031"));
	EXPECT_TRUE(isHeader(rndis, "#-///AASDFASDFAD1"));
	EXPECT_TRUE(isHeader(rndis, "#-///AASDFASDFAE1"));
	EXPECT_TRUE(isHeader(rndis, "#-///ATSSAGA00031"));
	EXPECT_TRUE(isHeader(rndis, "#-///ATSSAGA00031"));

	EXPECT_FALSE(isHeader(rndis, "#--//ATSSAGA00031"));
	EXPECT_FALSE(isHeader(rndis, "#////ATSSAGA00031"));
	EXPECT_FALSE(isHeader(rndis, "#-\\//ATSSAGA00031"));
	EXPECT_FALSE(isHeader(rndis, "#-/// ATSSAGA00031"));
	EXPECT_FALSE(isHeader(rndis, "#-///////GA00031"));
}

//string parsingSN(RNDISController &r, string header) {
//	return r.parsingSN(header);
//}

TEST(RNDISController, ParsingSerialNumber) {


	RNDISController rndis;
	string serialNumber = "ATSSAGA00031";
	string headerTag("#-///");

	string header("");
	header.append(headerTag);
	header.append(serialNumber);
//	EXPECT_EQ(serialNumber, rndis.parsingSerialNumber(header));
	EXPECT_EQ(serialNumber, parsingSN(rndis, header));

	serialNumber = "ATSPARA00032";
	header = headerTag;
	header.append(serialNumber);
//	EXPECT_EQ(serialNumber, rndis.parsingSerialNumber(header));
	EXPECT_EQ(serialNumber, parsingSN(rndis, header));
}

//TEST(RNDISController, GetRNDISName) {
//	string tmpList0 = "/tmp/list0.tmp";
//	string tmpList1 = "/tmp/list1.tmp";
//	string tmpList2 = "/tmp/list2.tmp";
//	string tmpList3 = "/tmp/list3.tmp";
//	string tmpList4 = "/tmp/list4.tmp";
//
//	ofstream outFile0(&tmpList0[0], ios::out);
//	outFile0 << "" << endl;
//	outFile0.close();
//
//	ofstream outFile1(&tmpList1[0], ios::out);
//	outFile1 << "usb0" << endl ;
//	outFile1.close();
//
//	ofstream outFile2(&tmpList2[0], ios::out);
//	outFile2 << "usb0" << endl << "usb1" << endl;
//	outFile2.close();
//
//	ofstream outFile3(&tmpList3[0], ios::out);
//	outFile3 << "usb0" << endl << "usb1" << endl << "usb2" << endl;
//	outFile3.close();
//
//	ofstream outFile4(&tmpList4[0], ios::out);
//	outFile4 << "usb0" << endl << "usb1" << endl<< "usb2" << endl << "usb3" << endl;
//	outFile4.close();
//
//	RNDISController rndis;
//	rndis.updateRNDISName(tmpList0);
//	EXPECT_EQ("usb0", rndis.getRNDISName());
//	rndis.updateRNDISName(tmpList1);
//	EXPECT_EQ("usb1", rndis.getRNDISName());
//	rndis.updateRNDISName(tmpList2);
//	EXPECT_EQ("usb2", rndis.getRNDISName());
//	rndis.updateRNDISName(tmpList3);
//	EXPECT_EQ("usb3", rndis.getRNDISName());
//	rndis.updateRNDISName(tmpList4);
//	EXPECT_EQ("usb4", rndis.getRNDISName());
//
//	remove(&tmpList0[0]);
//	remove(&tmpList1[0]);
//	remove(&tmpList2[0]);
//	remove(&tmpList3[0]);
//	remove(&tmpList4[0]);
//}

TEST(RNDISController, getUdevRuleOfRNDIS) {
	string serialNumber = "ATSSAGA00031";
	string iptProductId = "0ffc";
	RNDISController rndis;

	string rule("ACTION==\"add\", SUBSYSTEM==\"usb\", DRIVERS==\"usb\", ATTRS{idProduct}!=\"");
	rule.append(iptProductId);
	rule.append("\", ATTRS{serial}==\"");
	rule.append(serialNumber);
	rule.append("\", RUN+=\"/usr/local/sbin/ipt s ");
	rule.append(serialNumber);
	rule.append(" ON\"");
	EXPECT_EQ(rule, rndis.getUdevRuleOfRNDIS(serialNumber, iptProductId));
}

TEST(RNDISController, getUdevHeaderOfRNDIS) {
	string tag="#-///";
	string serialNumber("ATSSAGA00031");
	string header("");
	header.append(tag);
	header.append(serialNumber);
	RNDISController rndis;

	EXPECT_EQ(header, rndis.getUdevHeaderOfRNDIS(serialNumber));
}

TEST(RNDISController, Set_Get_ConfigFile) {
	RNDISController rndis;
	string defaultUdevConfigFile = "/etc/udev/rules.d/75-persistent-androidusbnet.rules";
	string udevConfigFile = "/etc/udev/rules.d/76-testing-udev-config.rules";

	EXPECT_EQ(defaultUdevConfigFile, rndis.getUdevConfigFile());

	rndis.setUdevConfigFile(udevConfigFile);
	EXPECT_EQ(udevConfigFile, rndis.getUdevConfigFile());
}

// Comprehensive Testing
TEST(Comprehensive, PersistentIpt) {
	string udevRule = "/tmp/testing";
	RNDISController rndis;
	rndis.setUdevConfigFile(udevRule);
	ASSERT_TRUE(rndis.enablePersistent("ATSSAGA00031", "0ffc"));
	EXPECT_FALSE(rndis.enablePersistent("ATSSAGA00031", "0ffc"));
	EXPECT_FALSE(rndis.enablePersistent("ATSSAGA00031", "0ffc"));
	EXPECT_FALSE(rndis.enablePersistent("ATSSAGA00031", "0ffc"));
	EXPECT_FALSE(rndis.enablePersistent("ATSSAGA00031", "0ffc"));

	EXPECT_TRUE(rndis.disablePersistent("ATSSAGA00031"));
	EXPECT_TRUE(rndis.disablePersistent("ATSSAGA00032"));
	EXPECT_TRUE(rndis.disablePersistent("ATSSAGA00031"));

	string serialNumber;
	stringstream ss;

	for (int i=100; i<1000; i++) {
		ss.str("");
		ss.clear();
		ss << i;
		serialNumber = "ATSSAGA00";
		serialNumber.append(ss.str());
		EXPECT_TRUE(rndis.enablePersistent(serialNumber, "0ffc"));
	}

	for (int i=100; i<1000; i++) {
		ss.str("");
		ss.clear();
		ss << i;
		serialNumber = "ATSSAGA00";
		serialNumber.append(ss.str());
		EXPECT_TRUE(rndis.disablePersistent(serialNumber));
	}

	for (int i=100; i<1000; i++) {
		ss.str("");
		ss.clear();
		ss << i;
		serialNumber = "ATSSAGA00";
		serialNumber.append(ss.str());
		EXPECT_TRUE(rndis.enablePersistent(serialNumber, "0ffc"));
//		system("clear && cat /tmp/testing ");
		EXPECT_TRUE(rndis.disablePersistent(serialNumber));
//		system("clear && cat /tmp/testing ");
	}

	remove(&udevRule[0]);
}
