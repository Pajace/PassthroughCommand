/*
 * XDevice_unittest.cpp
 *
 *  Created on: Aug 26, 2011
 *      Author: pajace
 */
#include <gtest/gtest.h>
#include "../../ap/XDevice.h"
#include "../../ap/IPaddress.h"
#include <fstream>

using namespace std;

TEST(XDevice, NormalTest) {
	XDevice device;
	string ipAddress = "192.168.0.101";
	string serialNumber = "ATSSAGA00040";
	string usbNetCode = "0ffc";

	bool isSettingIP = device.setIpAddress(ipAddress);
	bool isSettingSerial = device.setSerialNumber(serialNumber);
	bool isSettingUsbNetCode = device.setIptProductId(usbNetCode);

	ASSERT_TRUE(isSettingIP);
	ASSERT_TRUE(isSettingSerial);
	ASSERT_TRUE(isSettingUsbNetCode);
	ASSERT_EQ(ipAddress, device.getIpAddress().toString());
	ASSERT_EQ(serialNumber, device.getSerialNumber());
	ASSERT_EQ(usbNetCode, device.getIptProductId());
}

TEST(androidDevice, AbnormalTest) {
	XDevice device;
	string ipAddress = "192.168.112.2222";
	string serialNumber = "ATASAGA000000";
	string usbNetCode = "f fff";

	bool isSettingIP = device.setIpAddress(ipAddress);
	bool isSettingSerial = device.setSerialNumber(serialNumber);
	bool isSettingUsbNetCode = device.setIptProductId(usbNetCode);

	ASSERT_FALSE(isSettingIP);
	ASSERT_FALSE(isSettingSerial);
	ASSERT_FALSE(isSettingUsbNetCode);
	ASSERT_NE(ipAddress, device.getIpAddress().toString());
	ASSERT_NE(serialNumber, device.getSerialNumber());
	ASSERT_NE(usbNetCode, device.getIptProductId());


	ipAddress = "192.168.112.222.";
	serialNumber = "A ASAGA000000";
	usbNetCode = "0cfff";

	isSettingIP = device.setIpAddress(ipAddress);
	isSettingSerial = device.setSerialNumber(serialNumber);
	isSettingUsbNetCode = device.setIptProductId(usbNetCode);

	ASSERT_FALSE(isSettingIP);
	ASSERT_FALSE(isSettingSerial);
	ASSERT_FALSE(isSettingUsbNetCode);
	ASSERT_NE(ipAddress, device.getIpAddress().toString());
	ASSERT_NE(serialNumber, device.getSerialNumber());
	ASSERT_NE(usbNetCode, device.getIptProductId());
}

TEST(androidDevice, TestisDevicePlugged) {
	XDevice devices;

	string device1("ATSPYRAM0028");
	string device2("ATSPYRAM0027");
	string device3("HT0CETL00021");
	string device4("ATSSAGA00031");

	string pluggedDeviceList("");
	pluggedDeviceList+=device1+"\n";
	pluggedDeviceList+=device2;
	pluggedDeviceList+=device3;
	pluggedDeviceList+=device4;


	bool isPlugged = false;

	devices.setSerialNumber(device1);
	isPlugged = devices.isDevicePlugged(pluggedDeviceList);
	EXPECT_TRUE(isPlugged);

	devices.setSerialNumber(device2);
	isPlugged = devices.isDevicePlugged(pluggedDeviceList);
	EXPECT_TRUE(isPlugged);

	devices.setSerialNumber("POPOPOPOPOPO");
	isPlugged = devices.isDevicePlugged(pluggedDeviceList);
	EXPECT_FALSE(isPlugged);

	devices.setSerialNumber(device3);
	isPlugged = devices.isDevicePlugged(pluggedDeviceList);
	EXPECT_TRUE(isPlugged);

	devices.setSerialNumber("FFFFFFFFFFFF");
	isPlugged = devices.isDevicePlugged(pluggedDeviceList);
	EXPECT_FALSE(isPlugged);

	devices.setSerialNumber(device4);
	isPlugged = devices.isDevicePlugged(pluggedDeviceList);
	EXPECT_TRUE(isPlugged);

	devices.setSerialNumber("ASASASASASAS");
	isPlugged = devices.isDevicePlugged(pluggedDeviceList);
	EXPECT_FALSE(isPlugged);
}


TEST(XDevice, Test_isIptOn) {
	XDevice device;

	EXPECT_TRUE(device.isDeviceEnableIptMode("usb0     UP    192.168.1.1     255.255.255.0   0x00001043"));
	EXPECT_FALSE(device.isDeviceEnableIptMode("usb0     DOWN    0.0.0.0    0.0.0.0   0x00001043"));;
}
