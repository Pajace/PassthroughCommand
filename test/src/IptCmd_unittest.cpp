#include <gtest/gtest.h>
#include "../../ui/IptCmd.h"

IptCmd iptCmd;
int cmdArrSize = 11;
unsigned short iptCmdArr[] = {
		IptCmd::ERROR_ONOFF,
		IptCmd::ERROR_SERIAL,
		IptCmd::HELP,
		IptCmd::INVALID_IP,
		IptCmd::INVALID_USBCODE,
		IptCmd::IPT_OFF,
		IptCmd::IPT_ON,
		IptCmd::IPT_SET_DNS,
		IptCmd::IPT_SET_USBCODE,
		IptCmd::MISSING_ERROR,
		IptCmd::UNKOWN_PARAMETER
};

TEST(IptCmd, TestCmd_GetAndSet) {
	for (int i=0; i<cmdArrSize; i++) {
		iptCmd.setIptCommand(iptCmdArr[i]);
		EXPECT_EQ(iptCmdArr[i], iptCmd.getIptCommand());
	}
}

TEST(IptCmd, TestCmd_Normal) {
	iptCmd.setIptCommand(IptCmd::IPT_ON);
	EXPECT_EQ((unsigned short)IptCmd::IPT_ON, iptCmd.getIptCommand());

	iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::IPT_SET_DNS);
	EXPECT_EQ((unsigned short)IptCmd::IPT_ON_DNS, iptCmd.getIptCommand());

	iptCmd.setIptCommand(iptCmd.getIptCommand()+IptCmd::IPT_SET_USBCODE);
	EXPECT_EQ((unsigned short)IptCmd::IPT_ON_DNS_USB, iptCmd.getIptCommand());
}

TEST(IptCmd, TestCmd_Abnormal) {
	for (int i=0; i<cmdArrSize; i++) {
		for (int j=0; j<cmdArrSize; j++) {
			if (i==j) continue;
			EXPECT_NE(iptCmdArr[i], iptCmdArr[j]);
		}
	}
}

TEST(IptCmd, TestDescription) {
	string testString;

	testString="`1234567890-=";
	iptCmd.setIptDescription(testString);
	EXPECT_EQ(testString, iptCmd.getIptDescription());

	testString="~!@#$%^&*()_+";
	iptCmd.setIptDescription(testString);
	EXPECT_EQ(testString, iptCmd.getIptDescription());

	testString="qwertyuiopasdfghjklzxcvbnm";
	iptCmd.setIptDescription(testString);
	EXPECT_EQ(testString, iptCmd.getIptDescription());

	testString="QAZXSWEDCVFRTGBNHYUJMKILOP";
	iptCmd.setIptDescription(testString);
	EXPECT_EQ(testString, iptCmd.getIptDescription());

	testString="[]\{}|;':\",./<>?\"";
	iptCmd.setIptDescription(testString);
	EXPECT_EQ(testString, iptCmd.getIptDescription());

}
