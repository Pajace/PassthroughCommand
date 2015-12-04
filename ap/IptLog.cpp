/*
 * IptLog.cpp
 *
 *  Created on: Sep 5, 2011
 *      Author: pajace
 */
#include <iostream>
#include <syslog.h>
#include <string>

#include "IptLog.h"
#include "../Resource.h"
using namespace std;

IptLog* IptLog::m_pInstance = NULL;

IptLog* IptLog::getInstance() {
	if (!m_pInstance ) {
		m_pInstance = new IptLog();
		m_pInstance->setDebug(false);
		m_pInstance->setInfo(false);
		m_pInstance->ALERT = "[alert] ";
		m_pInstance->ERROR = "[error] ";
		m_pInstance->INFO  = "[info ] ";
		m_pInstance->DEBUG = "[debug] ";
	}
	return m_pInstance;
}

void IptLog::logMsg(int logLevel, string msg) {
	openlog("ipt",  LOG_PID, logLevel);
	syslog(logLevel, "%s", &msg[0]);
	closelog();
}

void IptLog::logAlert(string msg) {
	logMsg(LOG_ALERT, "["+IPT_VERSION+"]"+ALERT+msg);
}

void IptLog::logError(string msg) {
	logMsg(LOG_ERR, "["+IPT_VERSION+"]"+ERROR+msg);
}

void IptLog::logInfo(string msg) {
	if(isShowInfo)
		logMsg(LOG_INFO, "["+IPT_VERSION+"]"+INFO+msg);
}

#ifdef _DEBUG_
void IptLog::logDebug(string msg) {
	if (isShowDebug)
		logMsg(LOG_DEBUG, "["+IPT_VERSION+"]"+DEBUG+msg);
}
#else
void IptLog::logDebug(string msg) {}
#endif

void IptLog::setDebug(bool isDebug) {
	isShowDebug = isDebug;
}

void IptLog::setInfo(bool isInfo) {
	isShowInfo = isInfo;
}

IptLog::~IptLog() {
	delete IptLog::m_pInstance;
}


