/*
 * IPDispatcher.h
 *
 *  Created on: Aug 25, 2011
 *      Author: pajace
 */

#ifndef IPDISPATCHER_H_
#define IPDISPATCHER_H_
#include <string>
#include "IPaddress.h"
using std::string;

class IPDispatcher {
public:
	static IPDispatcher* getInstance();
	IPaddress getIPAddress(string);
	bool backIPAddress(string);
	bool initIPAddressPool();
	bool removeIPAddressPool();
	string getIPPoolDataFile() const;
	~IPDispatcher();

private:
	IPDispatcher(){	}; // private so that it can not be called
	IPDispatcher(IPDispatcher const&){}; // copy  constructor is private
//	IPDispatcher operator=(IPDispatcher const&){}; // assignment operator is private
	static IPDispatcher* m_pInstance;
};

#endif /* IPDISPATCHER_H_ */
