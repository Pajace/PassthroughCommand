/*
 * IptFacade.h
 *
 *  Created on: Aug 24, 2011
 *      Author: pajace
 */

#ifndef IPTFACADE_H_
#define IPTFACADE_H_
#include <iostream>
#include "RNDISController.h"
#include "IptLog.h"
#include <unistd.h>
#include <sys/sem.h>
using namespace std;

class IptFacade {
public:
	static IptFacade* getInstance();
	bool setupIptEnvironment();
	void enableIpt(string deviceSn);
	void enablePersistentIpt(string deviceSN, string usbCode);
	void disableIpt(string deviceSerialNumber);
	void bindRndisOnBridge(string rndisName);
	~IptFacade();

private:
	IptFacade(){}; // private so that it can not be called
	IptFacade(IptFacade const&){}; // copy  constructor is private
//	IptFacade& operator=(IptFacade const&){}; // assignment operator is private
	IptFacade operator= (IptFacade const &); // assignment operator is private
	static IptFacade* m_pInstance;
	void checkIptEnvironment();
};

#endif /* IPTFACADE_H_ */
