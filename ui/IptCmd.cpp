/*
 * IPTCmd.cpp
 *
 *  Created on: Aug 19, 2011
 *      Author: pajace
 */
#include "IptCmd.h"


IptCmd::IptCmd() {
	IptCmd::setIptCommand(0);
}


void IptCmd::setIptCommand(int cmd) {
	iptCommand = cmd;
}


void IptCmd::setIptDescription(string des) {
	ipt_description = des;
}

int IptCmd::getIptCommand() {
	return iptCommand;
}

string IptCmd::getIptDescription() {
	return ipt_description;
}
