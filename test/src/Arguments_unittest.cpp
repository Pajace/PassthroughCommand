/*
 * Arguments_unittest.cpp
 *
 *  Created on: Aug 19, 2011
 *      Author: pajace
 */
#include <gtest/gtest.h>
#include "../../ui/Arguments.h"
#include "../../ui/IptCmd.h"
using namespace std;

Arguments arg;

class Arguments_validateIPAddress : public ::testing::TestWithParam<string> {};
TEST_P(Arguments_validateIPAddress, HandleTrueReturn) {
	string ipAddress = GetParam();
	EXPECT_TRUE(arg.validateIpAddress(ipAddress));
}
INSTANTIATE_TEST_CASE_P(HnadleTrueReturn, Arguments_validateIPAddress, testing::Values(
		"1.1.1.1", 	"1.0.0.0",	"1.255.255.255",
		"10.0.0.0", "254.0.0.0","254.255.255.255",
		"255.255.255.255")
);

class Arguments_ValidateErrorIpAddress : public::testing::TestWithParam<string>{};
TEST_P(Arguments_ValidateErrorIpAddress, TestIPNumber_False) {
	string ip = GetParam();
	EXPECT_FALSE(arg.validateIpAddress(ip));
}
INSTANTIATE_TEST_CASE_P(TestWithWrongIP, Arguments_ValidateErrorIpAddress, testing::Values(
		"0.0.0.0",			"100.255.255.256",	"192.168.256.120",
		"192.256.125.123",	"256.159.156.123",	"01.152.153.245",
		"154.01.253.125",	"123.158.025.125",	"255.125.156.025"
		"256.256.257.225")
);

INSTANTIATE_TEST_CASE_P(TestIPFormat, Arguments_ValidateErrorIpAddress, testing::Values(
		"192.168.12.22.1",	"192.168..1.33", "192.168.1.",
		"192..31.3", "192.168.1.3.", "...."));

TEST(Arguments_TestUsbCode, TestUsbCode) {
	EXPECT_TRUE(arg.validateUsbCode("a0f3"));
	EXPECT_TRUE(arg.validateUsbCode("0xfe"));
	EXPECT_TRUE(arg.validateUsbCode("0x9e"));
	EXPECT_FALSE(arg.validateUsbCode(""));
	EXPECT_FALSE(arg.validateUsbCode("1"));
	EXPECT_FALSE(arg.validateUsbCode("12"));
	EXPECT_FALSE(arg.validateUsbCode("112"));
	EXPECT_FALSE(arg.validateUsbCode("112ee"));
}

TEST(Arguments_TestSN, TestDeviceSerialFormat) {
	EXPECT_TRUE(arg.validateSerialNumber("ATSSAGA00001"));
	EXPECT_TRUE(arg.validateSerialNumber("ATSSAGA00001"));
	EXPECT_TRUE(arg.validateSerialNumber("ANDPYMY00301"));
	EXPECT_FALSE(arg.validateSerialNumber(""));
	EXPECT_FALSE(arg.validateSerialNumber("            "));
	EXPECT_FALSE(arg.validateSerialNumber("a"));
	EXPECT_FALSE(arg.validateSerialNumber("aasdfafasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfweqrwer"));
	EXPECT_FALSE(arg.validateSerialNumber("ANDPYMD 03031"));
}

// Test main Controller, before this test, you have to make sure that up side test is correct.
// No. 6
class ArgIptCmd {
public:
	ArgIptCmd(string u, unsigned short e){
		userCommand = u;
		expectCmd = e;
	};
	string userCommand;
	unsigned short expectCmd;
};
class ArgumentsGetIptCommand : public::testing::TestWithParam<ArgIptCmd>{};
TEST_P(ArgumentsGetIptCommand, NormalCase) {
	ArgIptCmd cmd = GetParam();
	IptCmd iptCmd = arg.getIPTCommand(cmd.userCommand);
	EXPECT_EQ(cmd.expectCmd, iptCmd.getIptCommand());
}
INSTANTIATE_TEST_CASE_P(NormalCase, ArgumentsGetIptCommand, testing::Values(
		ArgIptCmd("ipt", (unsigned short)IptCmd::NO_ANY_ARGUMENTS),
		ArgIptCmd("ipt -h", (unsigned short)IptCmd::HELP),
		ArgIptCmd("ipt --help", (unsigned short)IptCmd::HELP),
		ArgIptCmd("ipt s ATSSATA00031 ON", (unsigned short)IptCmd::IPT_ON),
		ArgIptCmd("ipt s ATSATA000031 OFF", (unsigned short)IptCmd::IPT_OFF),
		ArgIptCmd("ipt s ATASATAFFDSS ON -d 192.168.1.1", (unsigned short)IptCmd::IPT_ON_DNS),
		ArgIptCmd("ipt s ATASDFASDFAS ON -p 0ffc", (unsigned short)IptCmd::IPT_ON_USB),
		ArgIptCmd("ipt s ASTASDFASDFA ON -d 10.10.10.2 -p 0fff", (unsigned short)IptCmd::IPT_ON_DNS_USB),
		ArgIptCmd("ipt s ASDFASFSDFFD -d 123.25.22.2 ON -p 0fae", (unsigned short)IptCmd::IPT_ON_DNS_USB),
		ArgIptCmd("ipt s ASDFASFSDFFD -d 123.25.22.2 -p 0fae ON", (unsigned short)IptCmd::IPT_ON_DNS_USB),
		ArgIptCmd("ipt ON s ADDFFEEWWWER", (unsigned short)IptCmd::IPT_ON),
		ArgIptCmd("ipt OFF s FFFDDDDDDDSA", (unsigned short)IptCmd::IPT_OFF),
		ArgIptCmd("ipt ON s ADDFFEEWWWER -d 147.252.33.2", (unsigned short)IptCmd::IPT_ON_DNS),
		ArgIptCmd("ipt ON s ADDFFEEWWWER -p ooxd", (unsigned short)IptCmd::IPT_ON_USB),
		ArgIptCmd("ipt ON s ADDFFEEWWWER -d 155.22.11.14 -p 0ccf", (unsigned short)IptCmd::IPT_ON_DNS_USB),
		ArgIptCmd("ipt ON s ADDFFEEWWWER -p 0ccf -d 155.22.11.114", (unsigned short)IptCmd::IPT_ON_DNS_USB)
		)
);


TEST(Arguments_getIPTCommand, ERROR_CASE) {

	IptCmd iptCmd;

	iptCmd = arg.getIPTCommand("ipt s jjffdkskeiea");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_ONOFF, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s asdfsadjfkas on");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_ONOFF+(unsigned short)IptCmd::UNKOWN_PARAMETER, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s FFDDJEQJDJFA off");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_ONOFF+(unsigned short)IptCmd::UNKOWN_PARAMETER, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s AFFFFJDDDSKF on ");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_ONOFF+(unsigned short)IptCmd::UNKOWN_PARAMETER, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s PPPPOOIUYTRU ON s JJFKFKFKDKEI");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_SERIAL, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt sJKKIIIJKKKLL ON");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_SERIAL+(unsigned short)IptCmd::UNKOWN_PARAMETER, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt JKKKLIIIJJFK s ON");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_SERIAL+(unsigned short)IptCmd::UNKOWN_PARAMETER, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s ATSSAGA00099 ON on");
	EXPECT_EQ((unsigned short)IptCmd::UNKOWN_PARAMETER, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s ATA ATA OFF");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_SERIAL+(unsigned short)IptCmd::UNKOWN_PARAMETER, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s ATA ATA");
	EXPECT_EQ(
			(unsigned short)IptCmd::ERROR_SERIAL+
			(unsigned short)IptCmd::UNKOWN_PARAMETER+
			(unsigned short)IptCmd::ERROR_ONOFF,
			iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s fjskdfon on");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_SERIAL+
			(unsigned short)IptCmd::UNKOWN_PARAMETER+
			(unsigned short)IptCmd::ERROR_ONOFF,
			iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s ATSSAGA00031 OFF -d 10.9.32.1");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_ONOFF, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s ATSSAGA00031 OFF -p 0ffc");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_ONOFF, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s ATSSAGA00031 OFF -d 10.9.32.1 -p 0ffc");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_ONOFF, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s ATSSAGA00031 OFF -d 10.9.32.1111");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_ONOFF+
			(unsigned short)IptCmd::INVALID_IP, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s ATSSAGA00031 OFF -p 0ffca");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_ONOFF+
			(unsigned short)IptCmd::INVALID_USBCODE, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s ATSSAGA00031 OFF -d ");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_ONOFF+
			(unsigned short)IptCmd::INVALID_IP, iptCmd.getIptCommand());

	iptCmd = arg.getIPTCommand("ipt s ATSSAGA00031 OFF -p ");
	EXPECT_EQ((unsigned short)IptCmd::ERROR_ONOFF+
			(unsigned short)IptCmd::INVALID_USBCODE, iptCmd.getIptCommand());
}

TEST(Arguments, TestGetSet) {
	arg.getIPTCommand("./ipt s ATSSAGA00031 ON -p 0ffc -d 10.0.0.1");
	EXPECT_EQ(arg.getSerialNumber(), "ATSSAGA00031");
	EXPECT_EQ(arg.getDnsIP(), "10.0.0.1");
	EXPECT_EQ(arg.getIptProductID(), "0ffc");

	arg.getIPTCommand("./ipt s XXXXXXXX0001 ON -p 0cb7");
	EXPECT_EQ(arg.getSerialNumber(), "XXXXXXXX0001");
	EXPECT_EQ(arg.getIptProductID(), "0cb7");

	arg.getIPTCommand("./ipt s xxxxddddeeee -d 192.192.192.1 ON");
	EXPECT_EQ(arg.getSerialNumber(), "xxxxddddeeee");
	EXPECT_EQ(arg.getDnsIP(), "192.192.192.1");

}

TEST(Arguments, TestBindBridge) {
	IptCmd iptCmd = arg.getIPTCommand("ipt -b usb0");
	ASSERT_EQ((unsigned short)IptCmd::BIND_RNDIS_ON_BRIDGE, iptCmd.getIptCommand());
}

TEST(Arguments, TestBindBridgeFalse) {
	IptCmd iptCmd = arg.getIPTCommand("ipt -b");
	ASSERT_NE((unsigned short)IptCmd::BIND_RNDIS_ON_BRIDGE, iptCmd.getIptCommand());
}

TEST(Arguments, TestInitializeIpt) {
	IptCmd iptCmd = arg.getIPTCommand("ipt init");
	ASSERT_EQ((unsigned short)IptCmd::INIT_IPT, iptCmd.getIptCommand());
}
