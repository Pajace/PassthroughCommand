/*
 * TestIptFacade.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: pajace
 */
#include <gtest/gtest.h>
#include "../../ap/IptFacade.h"
#include "../../ap/IptException.h"
#include <fstream>
using namespace std;

TEST(IptFacade, TestEnable) {
	ASSERT_ANY_THROW(IptFacade::getInstance()->enableIpt("ATSSATA00000"));
	ASSERT_ANY_THROW(IptFacade::getInstance()->enableIpt("SATA00000"));
	ASSERT_ANY_THROW(IptFacade::getInstance()->enableIpt(" "));
}

TEST(IptFacade, TestDisableIpt) {
	ASSERT_ANY_THROW(IptFacade::getInstance()->disableIpt("ANDROIDTESTIPT00"));
	ASSERT_ANY_THROW(IptFacade::getInstance()->disableIpt("ANDROIDTESTIPT0"));
	ASSERT_ANY_THROW(IptFacade::getInstance()->disableIpt("ATSSAGA00040"));
}

TEST(IptFacade, EnablePersistentIpt) {
	ASSERT_ANY_THROW(IptFacade::getInstance()->enablePersistentIpt("000000000000", "0000"));
	ASSERT_ANY_THROW(IptFacade::getInstance()->enablePersistentIpt("00000000000", "0000"));
	ASSERT_ANY_THROW(IptFacade::getInstance()->enablePersistentIpt("000000000000", "000"));
	ASSERT_ANY_THROW(IptFacade::getInstance()->enablePersistentIpt("00000000000", "000"));
}

