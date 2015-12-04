/*
 * IptCommand.h
 *
 *  Created on: Aug 19, 2011
 *      Author: pajace
 */

#ifndef IPTCOMMAND_H_
#define IPTCOMMAND_H_
#include <string>
using namespace std;

class IptCmd {
public:
	static const unsigned short HELP				= 0;
	static const unsigned short IPT_ON				= 1;
	static const unsigned short IPT_SET_DNS			= 2;
	static const unsigned short IPT_SET_USBCODE		= 4;
	static const unsigned short IPT_OFF				= 8;

	static const unsigned short ERROR_ONOFF			=   16;
	static const unsigned short ERROR_SERIAL		=   32;
	static const unsigned short INVALID_IP			=   64;
	static const unsigned short INVALID_USBCODE		=  128;
	static const unsigned short UNKOWN_PARAMETER	=  512;
	static const unsigned short NO_ANY_ARGUMENTS	= 1024;
	static const unsigned short MISSING_ERROR		= 1025;

	static const unsigned short BIND_RNDIS_ON_BRIDGE= 2048;
	static const unsigned short OTHER_ERROR			= 2058;

	static const unsigned short INIT_IPT			= 4096;

	static const unsigned short IPT_ON_DNS_USB		= (IPT_ON+IPT_SET_DNS+IPT_SET_USBCODE);
	static const unsigned short IPT_ON_DNS			= (IPT_ON+IPT_SET_DNS);
	static const unsigned short IPT_ON_USB			= (IPT_ON+IPT_SET_USBCODE);

	IptCmd();
	void setIptCommand(int);
	void setIptDescription(string);
	int getIptCommand();
	string getIptDescription();

private:
	string ipt_description;
	int iptCommand;
};



#endif /* IPTCOMMAND_H_ */
