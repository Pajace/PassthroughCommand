/*
 * RNDISController.h
 *
 *  Created on: Aug 29, 2011
 *      Author: pajace
 */

#ifndef RNDISCONTROLLER_H_
#define RNDISCONTROLLER_H_
#include <string>
#include <vector>
using namespace std;

class RNDISController {

	friend string parsingSN( RNDISController &r, string h) {
		return r.parsingSerialNumber(h);
	}
	friend bool isHeader( RNDISController &r, string h) {
		return r.isHeader(h);
	}

public:
	static void generateConfigFile_AutoBindRndis(string);
	RNDISController();
	virtual ~RNDISController();
	bool enablePersistent(string serialNumber, string usbNetCode);
	bool disablePersistent(string serialNumber);


	string getUdevConfigFile() const;
	string getUdevRuleOfRNDIS(string serialNumber, string usbNetCode) const;
	string getUdevHeaderOfRNDIS(string serialNumber) const;
	void setUdevConfigFile(string udevConfigFile);

private:
	void generateConfigFile();
	bool checkSerialNumber(string header);
	bool isSuccessToRemove(string serialNumber);

	string parsingSerialNumber(string header) const;
	bool isHeader(string header);

	bool isDoubleEnable(const vector<string> &udevRulecontent, string serialNumber);
    void writeConfigBack2SystemFile(const vector<string> &udevRuleContent, int enableOrDisable);
    vector<string> getUdevConfigContent();
    bool isSuccessToAdd(string serialNumber);

	string udevConfigFile;
};

#endif /* RNDISCONTROLLER_H_ */
