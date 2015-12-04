/*
 * Resource.h
 *
 *  Created on: Sep 19, 2011
 *      Author: pajace
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_
#include <string>
#include <vector>
using namespace std;

static const string IPT_VERSION = "0.8.221938286.00";

#ifdef __x64__
	static const string IPT_BINARY_TYPE = "64bit";
#else
	static const string IPT_BINARY_TYPE = "32bit";
#endif

static const string ADB_COMMAND = "adb";
static const string BRIDGE_IF_NAME = "iptBri0";
static const string AUTO_BIND_RNDIS_CONFIG = "/etc/udev/rules.d/70-persistent-rndis-udev.rules";
static const string PERSISTENCE_IPT_CONFIG = "/etc/udev/rules.d/75-persistent-androidusbnet.rules";
static const string IPT_SUB_NETWORK_PRE		= "192.168.0.";
static const string IPT_SUB_NETWORK_IP  	= IPT_SUB_NETWORK_PRE+"0/24";
static const string IPT_SUB_NETWORK_GATEWAY = IPT_SUB_NETWORK_PRE+"254";
static const string IPT_DNS					= IPT_SUB_NETWORK_GATEWAY;
static const string IPT_BACKUP_DNS_IP		= "168.95.1.1";
static const string IPT_COMMAND_DIRECTORY	= "/usr/local/sbin/";
static const int STRING_NOT_FOUND = -1;

static const string RNDIS_BIND_UDEV_RULE	= "ACTION==\"add\", KERNEL==\"usb*\", SUBSYSTEM==\"net\", DRIVERS==\"rndis_host\",  RUN+=\"/usr/local/sbin/ipt -b usb%n\"";
#endif /* RESOURCE_H_ */
