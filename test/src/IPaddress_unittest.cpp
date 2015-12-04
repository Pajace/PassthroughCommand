/*
 * IPaddress_unittest.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: pajace
 */
#include <gtest/gtest.h>
#include "../../ap/IPaddress.h"

IPaddress ipAddress;
string expectIP;
bool isSetIPOK;
string defaultIP = "1.0.0.0";

TEST(IPAddressUnitTest, Normal_IP) {

	EXPECT_EQ("1.0.0.0", ipAddress.toString() );

	expectIP = "192.168.0.1";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	ASSERT_TRUE(isSetIPOK);
	ASSERT_EQ(expectIP, ipAddress.toString());

	expectIP = "1.1.1.1";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	ASSERT_TRUE(isSetIPOK);
	ASSERT_EQ(expectIP, ipAddress.toString());

	expectIP = "1.0.0.0";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	ASSERT_TRUE(isSetIPOK);
	ASSERT_EQ(expectIP, ipAddress.toString());

	expectIP = "1.255.255.255";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	ASSERT_TRUE(isSetIPOK);
	ASSERT_EQ(expectIP, ipAddress.toString());

	expectIP = "10.0.0.0";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	ASSERT_TRUE(isSetIPOK);
	ASSERT_EQ(expectIP, ipAddress.toString());

	expectIP = "254.0.0.0";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	ASSERT_TRUE(isSetIPOK);
	ASSERT_EQ(expectIP, ipAddress.toString());

	expectIP = "254.255.255.255";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	ASSERT_TRUE(isSetIPOK);
	ASSERT_EQ(expectIP, ipAddress.toString());

	expectIP = "255.255.255.255";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	ASSERT_TRUE(isSetIPOK);
	ASSERT_EQ(expectIP, ipAddress.toString());
}

TEST(IPAddressUnitTest, AbnormalIP) {
	// initial
	ipAddress.setIPAddress("1.0.0.0");

	expectIP = "0.0.0.0";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "100.255.255.256";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "192.168.256.120";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "192.256.125.123";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "256.159.156.123";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "01.152.153.245";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "154.01.253.125";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "123.158.025.125";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "255.125.156.025";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "256.256.257.255";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "1568516484.1846846.151654564.112";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

}

TEST(IPAddressUnitTest, TestWrongFormat) {

	// initial
	ipAddress.setIPAddress("1.0.0.0");

	expectIP = "192.168.12.22.1";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "192.168..1.33";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "192.168.1.";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "192..31.3";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "192.168.1.3.";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "....";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "...";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());

	expectIP = "1 . 1 . 1 . 1";
	isSetIPOK = ipAddress.setIPAddress(expectIP);
	EXPECT_FALSE(isSetIPOK);
	EXPECT_EQ(defaultIP, ipAddress.toString());
}
