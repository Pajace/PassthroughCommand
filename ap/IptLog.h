/*
 * IptLog.h
 *
 *  Created on: Sep 5, 2011
 *      Author: pajace
 */

#ifndef IPTLOG_H_
#define IPTLOG_H_
#include <string>
using namespace std;

class IptLog {
public:
	static IptLog* getInstance();
	void logAlert(string);
	void logError(string);
	void logInfo(string);
	void logDebug(string);

	void setDebug(bool);
	void setInfo(bool);

	string ALERT;// = "alert";
	string ERROR;// = "error";
	string INFO;//= "info";
	string DEBUG;// = "debug";

private:
	IptLog(){};
	IptLog(IptLog const&){};
	IptLog operator=(IptLog const&); // assignment operator is private
	static IptLog *m_pInstance;
	void logMsg(int logLevel, string msg);

	bool isShowInfo;
	bool isShowDebug;
	~IptLog();
};

#endif /* IPTLOG_H_ */
