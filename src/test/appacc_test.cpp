/*
 * appacc_test.cpp
 *
 *  Created on:
 *      Author: ranger.shi
 */


#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>
#include "txdb.h"
#include "database.h"
#include <iostream>
#include  "boost/filesystem/operations.hpp"
#include  "boost/filesystem/path.hpp"
#include  "../vm/appaccount.h"
#include "../vm/vmrunevn.h"
#include "tx.h"
#include "util.h"

using namespace std;

int64_t opValue[8][8] = {
						{100*COIN, 10*COIN, 10*COIN, 30*COIN, 40*COIN, 30*COIN ,30*COIN, 20*COIN},    //false
						{1000*COIN, 200*COIN, 20*COIN, 30*COIN, 40*COIN, 20*COIN, 10*COIN, 20*COIN},  //false
						{500*COIN, 200*COIN, 20*COIN, 100*COIN, 200*COIN, 100*COIN, 300*COIN, 100*COIN}, //false
						{100*COIN, 10*COIN, 20*COIN, 50*COIN, 50*COIN, 10*COIN, 20*COIN, 30*COIN}, //false
						{200*COIN, 20*COIN, 30*COIN, 40*COIN, 30*COIN, 30*COIN, 40*COIN, 40*COIN},  //false
						{1000*COIN, 200*COIN, 20*COIN, 500*COIN, 800*COIN, 400*COIN, 200*COIN, 100*COIN}, //true
						{500*COIN, 200*COIN, 200*COIN, 300*COIN, 200*COIN, 50*COIN, 100*COIN, 50*COIN}, //true
						{600*COIN, 200*COIN, 20*COIN, 30*COIN, 50*COIN, 60*COIN, 70*COIN, 20*COIN}  //false
						};

// appacc_tests/key_test1


bool CheckAppAcct(int64_t opValue[]) {
	CRegID srcRegId(100,1);
	CRegID desRegId(100,2);
	CRegID desUser1RegId(100,3);
	CRegID desUser2RegId(100,4);
	CAccount contractAcct;
	contractAcct.llValues = 100 * COIN;
	contractAcct.regID = desRegId;

	CUserID srcUserId= srcRegId;
	CUserID desUserId = desRegId;
	vector_unsigned_char pContract;
	CTransaction tx(srcUserId, desRegId, 10000, opValue[0], 1, pContract); //100 * COIN

	CVmRunEvn vmRunEvn;
	vector<CVmOperate> vAcctOper;

	vector_unsigned_char vDesUser1RegId = desUser1RegId.GetVec6();
	int64_t temp = opValue[1];  //10 * COIN
	CVmOperate acctAddOper;
	acctAddOper.nacctype = regid;
	acctAddOper.opeatortype = ADD_FREE;
	memcpy(acctAddOper.accountid, &vDesUser1RegId[0], 6);
	memcpy(acctAddOper.money, &temp, sizeof(temp));
	vAcctOper.push_back(acctAddOper);

	vector_unsigned_char vDesUser2RegId = desUser2RegId.GetVec6();
	temp = opValue[2];   //20 * COIN
	acctAddOper.nacctype = regid;
	acctAddOper.opeatortype = ADD_FREE;
	memcpy(acctAddOper.accountid, &vDesUser2RegId[0], 6);
	memcpy(acctAddOper.money, &temp, sizeof(temp));
	vAcctOper.push_back(acctAddOper);

	vector_unsigned_char vDesRegId = desRegId.GetVec6();
	temp = opValue[3];  //30 * COIN
	acctAddOper.nacctype = regid;
	acctAddOper.opeatortype = MINUS_FREE;
	memcpy(acctAddOper.accountid, &vDesRegId[0], 6);
	memcpy(acctAddOper.money, &temp, sizeof(temp));
	vAcctOper.push_back(acctAddOper);
	vmRunEvn.InsertOutputData(vAcctOper);

	CAppFundOperate appFundOper;
	appFundOper.opeatortype = ADD_FREE_OP;
	appFundOper.mMoney = opValue[4];       //20 * COIN
	appFundOper.appuserIDlen = 6;
	memcpy(appFundOper.vAppuser,  &vDesUser1RegId[0], 6);
	appFundOper.FundTaglen = 6;
	memcpy(appFundOper.vFundTag, &vDesUser1RegId[0], 6);
	vmRunEvn.InsertOutAPPOperte(vDesUser1RegId, appFundOper);

	appFundOper.opeatortype = SUB_FREE_OP;
	appFundOper.mMoney = opValue[5];      //90 * COIN
	appFundOper.appuserIDlen = 6;
	memcpy(appFundOper.vAppuser,  &vDesUser2RegId[0], 6);
	appFundOper.FundTaglen = 6;
	memcpy(appFundOper.vFundTag, &vDesUser2RegId[0], 6);
	vmRunEvn.InsertOutAPPOperte(vDesUser2RegId, appFundOper);

	appFundOper.opeatortype = ADD_TAG_OP;
	appFundOper.mMoney = opValue[6];     // 90 * COIN
	appFundOper.appuserIDlen = 6;
	memcpy(appFundOper.vAppuser,  &vDesUser2RegId[0], 6);
	appFundOper.FundTaglen = 6;
	memcpy(appFundOper.vFundTag, &vDesUser2RegId[0], 6);
	vmRunEvn.InsertOutAPPOperte(vDesUser2RegId, appFundOper);

	appFundOper.opeatortype = SUB_TAG_OP;
	appFundOper.mMoney = opValue[7];  // 80 * COIN
	appFundOper.appuserIDlen = 6;
	memcpy(appFundOper.vAppuser,  &vDesUser1RegId[0], 6);
	appFundOper.FundTaglen = 6;
	memcpy(appFundOper.vFundTag, &vDesUser1RegId[0], 6);
	vmRunEvn.InsertOutAPPOperte(vDesUser1RegId, appFundOper);

	return vmRunEvn.CheckAppAcctOperate(&tx);
}

BOOST_AUTO_TEST_SUITE(appacc_tests)

BOOST_AUTO_TEST_CASE(key_test1)
 {
	auto StrTVector = [&](string tag)
	{
		return vector<unsigned char>(tag.begin(),tag.end());
	};

	srand((int) time(NULL));

	vector<unsigned char> AppuserId = StrTVector("test1");
	vector<unsigned char> fundtag = StrTVector("foundtag");
	vector<unsigned char> fundtag2 = StrTVector("foundtag2");

	CAppFundOperate opTe(AppuserId,fundtag, ADD_TAG_OP, 500, 800000);
	BOOST_CHECK(opTe.GetFundTagV() == fundtag);
	BOOST_CHECK(opTe.GetUint64Value()== 800000);
	BOOST_CHECK(opTe.getopeatortype()== ADD_TAG_OP);


	vector<CAppFundOperate> OpArry;
	uint64_t allmony = 0;
	int timeout = (rand() % 15000) + 51;
	int loop = 500;
	int maxtimeout = timeout + loop+1;
	for (int i = 0; i < loop; i++) {
		int64_t temp = ((rand() * rand()) % 15000000) + 20;
		allmony += temp;
		CAppFundOperate op(AppuserId,fundtag, ADD_TAG_OP, timeout + i, temp);
		OpArry.insert(OpArry.end(), op);
	}

	CAppUserAccout AccCount(AppuserId);
	BOOST_CHECK(AccCount.getaccUserId() == AppuserId);
	BOOST_CHECK(AccCount.Operate(OpArry));
	BOOST_CHECK(AccCount.getllValues() == 0);


	{
		CAppCFund tep;
		BOOST_CHECK(AccCount.GetAppCFund(tep, fundtag, timeout));
		BOOST_CHECK(tep.getvalue() == OpArry[0].GetUint64Value());
		CAppCFund tep2;
		BOOST_CHECK(AccCount.GetAppCFund(tep2, fundtag, maxtimeout + 5) == false);

		AccCount.AutoMergeFreezeToFree(timeout - 1);
		BOOST_CHECK(AccCount.GetAppCFund(tep, fundtag, timeout));
		BOOST_CHECK(tep.getvalue() == OpArry[0].GetUint64Value());
	}

	{
		vector<CAppFundOperate> OpArry2;
		CAppFundOperate subfreexeop(AppuserId,fundtag, SUB_TAG_OP, timeout, 8);
		OpArry2.insert(OpArry2.end(), subfreexeop);
		BOOST_CHECK(AccCount.Operate(OpArry2));
	}

	{
		CAppCFund subtemptep;
		BOOST_CHECK(AccCount.GetAppCFund(subtemptep, fundtag, timeout));
		BOOST_CHECK(subtemptep.getvalue() == (OpArry[0].GetUint64Value() - 8));
	}

	{
		vector<CAppFundOperate> OpArry2;
		CAppFundOperate revertfreexeop(AppuserId,fundtag, ADD_TAG_OP, timeout, 8);
		OpArry2.clear();
		OpArry2.insert(OpArry2.end(), revertfreexeop);
		BOOST_CHECK(AccCount.Operate(OpArry2));
	}

	{
		CAppCFund reverttemptep;
		BOOST_CHECK(AccCount.GetAppCFund(reverttemptep, fundtag, timeout));
		BOOST_CHECK(reverttemptep.getvalue() == OpArry[0].GetUint64Value());
	}

	{
		CAppCFund tep;
		AccCount.AutoMergeFreezeToFree(timeout);
		BOOST_CHECK(AccCount.GetAppCFund(tep, fundtag, timeout) == false);
		BOOST_CHECK(AccCount.getllValues() == OpArry[0].GetUint64Value());;
	}

	{
		CAppFundOperate subfreeop(AppuserId,fundtag, SUB_FREE_OP, timeout, OpArry[0].GetUint64Value());
		vector<CAppFundOperate> OpArry2;
		OpArry2.insert(OpArry2.end(), subfreeop);
		BOOST_CHECK(AccCount.Operate(OpArry2));
		BOOST_CHECK(AccCount.getllValues() == 0);;
	}

	{
		vector<CAppFundOperate> OpArry2;
		CAppFundOperate addfreeop(AppuserId,fundtag, ADD_FREE_OP, timeout, OpArry[0].GetUint64Value());
		OpArry2.clear();
		OpArry2.insert(OpArry2.end(), addfreeop);
		BOOST_CHECK(AccCount.Operate(OpArry2));
		BOOST_CHECK(AccCount.getllValues() == OpArry[0].GetUint64Value());
	}

	AccCount.AutoMergeFreezeToFree(maxtimeout);
	//BOOST_CHECK(AccCount.getllValues() == allmony);

}

BOOST_AUTO_TEST_CASE(checkappacct_test) {
	for(int j=0; j <8; ++j) {
		for(int i=0; i<8; ++i) {
			cout << opValue[j][i] <<" ";
		}
		cout << endl;
		int64_t txValue = opValue[j][0];
		int64_t acctMinusValue = opValue[j][3];
		int64_t acctSum = txValue - acctMinusValue;
		int64_t appAcctSum = opValue[j][4] - opValue[j][5] + opValue[j][6] - opValue[j][7];
		bool isCheck = (acctSum == appAcctSum);
		cout << "ischeck:" << isCheck << endl;
		BOOST_CHECK(CheckAppAcct(opValue[j]) == isCheck);
	}

}
BOOST_AUTO_TEST_SUITE_END()



