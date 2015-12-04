/*
 * IptCommand.h
 *
 *  Created on: Apr 10, 2012
 *      Author: pajace_chen
 */

#ifndef IPTCOMMAND_H_
#define IPTCOMMAND_H_
#include <string>
#include <vector>
#include "../Resource.h"
using namespace std;

class IptCommand {
public:
	static IptCommand* getInstance();

	vector<string> GetSetupIptEnvironmentCmd();
	vector<string> GetEnableIptCommand(string devicesSN, string ipAddress);
	vector<string> GetDisableIptCommand(string deviceSN);
	vector<string> GetBindBridgeCommand(string bindedInterface);
	string GetWaitForDeviceCommand(string deviceSN);

private:
	IptCommand(){}; // private so that it can not be called
	IptCommand(IptCommand const&){}; // copy constructor is private
	~IptCommand(){
		delete m_pInstance;
	};

	static IptCommand* m_pInstance;
};

#endif /* IPTCOMMAND_H_ */
