/*
 * TestIptFacade.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: pajace
 */
#include <gtest/gtest.h>

#include <fstream>
#include <stdlib.h>

#include "../../ap/IptFacade.h"
#include "../../ap/IptException.h"
#include "../../ap/ExternalCommander.h"
#include "../../ap/Semaphore.h"
#include "../../Resource.h"

using namespace std;

#define GREEN	"\033[22;32m"
#define RED		"\033[22;31m"
#define GRAY	"\033[22;37m"

// hard code
#define SERIAL_NUMBER  "HT21CS400238"

string downloadfileSite = "http://ftp.speed.hinet.net/test_060m.dat";
string internalDownloadSite = "http://hiandroid.android.com.tw/hiandroid/resources/theme/default/images/androidlogo.png";

string commandResult;
string command;
int findResult=0;

string enableString;
string disableString;

class IptComprehansiveTest: public testing::Test {
protected:
	static void SetUpTestCase() {
		IptFacade::getInstance()->setupIptEnvironment();
		enableString = " Enable [";
		enableString.append(SERIAL_NUMBER);
		enableString.append("] - [OK]");
		disableString = " Disable [";
		disableString.append(SERIAL_NUMBER);
		disableString.append("] - [OK]");
	}

	static void TearDownTestCase() {
		string resetIptEnvironment("ifconfig "+BRIDGE_IF_NAME+" down ");
		resetIptEnvironment.append("&& brctl delbr "+BRIDGE_IF_NAME+" ");
		resetIptEnvironment.append("&& rm /tmp/ipt* ");
		resetIptEnvironment.append("&& rm /etc/udev/rules.d/70-persistent-rndis-udev.rules ");
		resetIptEnvironment.append("&& rm /etc/udev/rules.d/75-persistent-androidusbnet.rules ");
		system(resetIptEnvironment.c_str());
		Semaphore::getInstance()->del_semvalue();
	}
};

TEST_F(IptComprehansiveTest, TestEnableIpt)
{
	// enable ipt
	command = "ipt ON s ";
	command.append(SERIAL_NUMBER);
	commandResult = ExternalCommander::getInstance()->
			getExternalCommandResult(command);
	findResult = commandResult.find(enableString);
	EXPECT_NE(-1, findResult);
}

TEST_F(IptComprehansiveTest, TestDownload1) {
	// download
	command = "adb -s ";
	command.append(SERIAL_NUMBER);
	command.append(" shell /data/bin/wget "+downloadfileSite+" -O /data/IPT_TEST");
	commandResult = ExternalCommander::getInstance()->getExternalCommandResult(command);
	findResult = commandResult.find("`/data/IPT_TEST' saved");
	EXPECT_NE(-1, findResult);
}

TEST_F(IptComprehansiveTest, TestDisable1) {
	// disable ipt
	command = "ipt OFF s ";
	command.append(SERIAL_NUMBER);
	commandResult = ExternalCommander::getInstance()->getExternalCommandResult(command);
	findResult = commandResult.find(disableString);
	EXPECT_NE(-1, findResult);
}

TEST_F(IptComprehansiveTest, TestEnablePersistentIpt)
{
	// enable ipt
	command = "ipt -p 0ffc ON s ";
	command.append(SERIAL_NUMBER);
	commandResult = ExternalCommander::getInstance()->getExternalCommandResult(
			command);
	findResult = commandResult.find(enableString);
	EXPECT_NE(-1, findResult);
}

TEST_F(IptComprehansiveTest, TestDownload2) {
	// download
	command = "adb -s ";
	command.append(SERIAL_NUMBER);
	command.append(" shell /data/bin/wget "+downloadfileSite+" -O /data/IPT_TEST");
	commandResult = ExternalCommander::getInstance()->getExternalCommandResult(command);
	findResult = commandResult.find("`/data/IPT_TEST' saved");
	EXPECT_NE(-1, findResult);
}

TEST_F(IptComprehansiveTest, TestDisable2) {
	// disable ipt
	command = "ipt OFF s ";
	command.append(SERIAL_NUMBER);
	commandResult = ExternalCommander::getInstance()->getExternalCommandResult(command);
	findResult = commandResult.find(disableString);
	EXPECT_NE(-1, findResult);
}

/////////////////////////////////////////////////////////////
TEST_F(IptComprehansiveTest, TestEnableIptWithDNS)
{
	// enable ipt
	command = "ipt ON s ";
	command.append(SERIAL_NUMBER);
	commandResult = ExternalCommander::getInstance()->getExternalCommandResult(
			command);
	findResult = commandResult.find(enableString);
	EXPECT_NE(-1, findResult);
}

TEST_F(IptComprehansiveTest, TestDownload3) {
	// download
	command = "adb -s ";
	command.append(SERIAL_NUMBER);
	command.append(" shell /data/bin/wget "+internalDownloadSite+" -O /data/IPT_TEST");
	commandResult = ExternalCommander::getInstance()->getExternalCommandResult(command);
	findResult = commandResult.find("`/data/IPT_TEST' saved");
	EXPECT_NE(-1, findResult);
}

TEST_F(IptComprehansiveTest, TestDisable3) {
	// disable ipt
	command = "ipt OFF s ";
	command.append(SERIAL_NUMBER);
	commandResult = ExternalCommander::getInstance()->getExternalCommandResult(command);
	findResult = commandResult.find(disableString);
	EXPECT_NE(-1, findResult);
}
/////////////////////////////////////////////////////////////
TEST_F(IptComprehansiveTest, TestEnablePersistentIpt2)
{
	// enable ipt
	command = "ipt -p 0ffc ON s ";
	command.append(SERIAL_NUMBER);
	commandResult = ExternalCommander::getInstance()->getExternalCommandResult(
			command);
	findResult = commandResult.find(enableString);
	EXPECT_NE(-1, findResult);
}

TEST_F(IptComprehansiveTest, TestDownload4) {
	// download
	command = "adb -s ";
	command.append(SERIAL_NUMBER);
	command.append(" shell /data/bin/wget "+internalDownloadSite+" -O /data/IPT_TEST");
	commandResult = ExternalCommander::getInstance()->getExternalCommandResult(command);
	findResult = commandResult.find("`/data/IPT_TEST' saved");
	EXPECT_NE(-1, findResult);
}

TEST_F(IptComprehansiveTest, TestDisable4) {
	// disable ipt
	command = "ipt OFF s ";
	command.append(SERIAL_NUMBER);
	commandResult = ExternalCommander::getInstance()->getExternalCommandResult(command);
	findResult = commandResult.find(disableString);
	EXPECT_NE(-1, findResult);
}
