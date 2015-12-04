/*
 * IPaddress.h
 *
 *  Created on: Aug 25, 2011
 *      Author: pajace
 */

#ifndef IPADDRESS_H_
#define IPADDRESS_H_
#include <string>
using std::string;

class IPaddress {
public:
	IPaddress(string="1.0.0.0");
	bool setIPAddress(string);
	string toString() const;

private:
	string ipaddress;
	bool validateIpAddress(const string ipAddress);
};

#endif /* IPADDRESS_H_ */
