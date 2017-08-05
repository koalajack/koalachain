/*
 * CAnony_tests.cpp
 *
 *  Created on: 2015-04-24
 *      Author: frank
 */

#include "CKaola_tests.h"
#include "CycleTestManger.h"
#include <boost/assign/list_of.hpp>
#include "json/json_spirit_utils.h"
#include "json/json_spirit_value.h"
#include "json/json_spirit_reader.h"
using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace json_spirit;

#define ADDR_COMPANY_A        "dcmCbKbAfKrofNz35MSFupxrx4Uwn3vgjL"
// 1826-1437  129692871376

#define ADDR_USER_B      "dcmWdcfxEjRXUHk8LpygtgDHTpixoo3kbd"
// 1826-1285  128999717658

#define ADDR_ADMINISTRATOR_C "dcnGLkGud6c5bZJSUghzxvCqV45SJEwRcH"
// 1826-1081  126691618233


//#define ID_strAppRegId  "47067-1"    //
#define ID_strAppRegId  "47039-1"    //
CKaolaTest::CKaolaTest():nNum(0), nStep(0), strTxHash(""), strAppRegId(ID_strAppRegId) {

}

TEST_STATE CKaolaTest::Run(){
	cout<<"CKaolaTest run start"<<endl;
#if 0

	if(!RegistScript()){
		cout<<"CKaolaTest RegistScript err"<<endl;
		return end_state;
	}


	while(true)
	{
		if(WaitComfirmed(strTxHash, strAppRegId)) {
					break;
				}
	}
#endif

//	Config();
//	Modify();
//	Recharge();
	Withdraw();

	cout<<"CKaolaTest run end"<<endl;
	return end_state;
}


bool CKaolaTest::RegistScript(){

	const char* pKey[] = { "cNcJkU44oG3etbWoEvY46i5qWPeE8jVb7K44keXxEQxsXUZ85MKU",
			"cNcJkU44oG3etbWoEvY46i5qWPeE8jVb7K44keXxEQxsXUZ85MKU"};
	int nCount = sizeof(pKey) / sizeof(char*);
	basetest.ImportWalletKey(pKey, nCount);

	string strFileName("kaola.bin");
	int nFee = basetest.GetRandomFee();
	int nCurHight;
	basetest.GetBlockHeight(nCurHight);
	string regAddr="dk2NNjraSvquD9b4SQbysVRQeFikA55HLi";

	//reg anony app
	Value regscript = basetest.RegisterAppTx(regAddr, strFileName, nCurHight, nFee + 1 *COIN);// + 20 *COIN
	if(basetest.GetHashFromCreatedTx(regscript, strTxHash)){

		return true;
	}
	return false;
}

bool CKaolaTest::Config(void)
{
   cout<<"Config start"<<endl;

   COMPANY_CONFIG senddata;

	memset(&senddata,0,sizeof(senddata));
	senddata.type = TX_CONFIG;
	senddata.maxMoneyByTime = 0;
	senddata.maxMoneyByDay = 0;
	strcpy(senddata.address,ADDR_COMPANY_A);

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;//20 * COIN;

	cout<<"Config data:"<<sendcontract<<endl;
	cout<<"Config strAppRegId:"<<strAppRegId<<endl;
	Value  retValue= basetest.CreateContractTx(strAppRegId,ADDR_ADMINISTRATOR_C,sendcontract,0,0,nTempSend);
	if (basetest.GetHashFromCreatedTx(retValue, strTxHash)) {
		nStep++;
	}else{
		cout<<"Config err end"<<endl;
		return false;
	}

#if 1

	while(true)
	{
		if(WaitComfirmed(strTxHash, strAppRegId)) {
					break;
				}
	}
#endif
	cout<<"Config success end"<<endl;
	return false;
}
bool CKaolaTest::Modify(void)
{
   cout<<"Modify start"<<endl;

   COMPANY_CONFIG_MODIFY senddata;

	memset(&senddata,0,sizeof(senddata));
	senddata.type = TX_CONFIG;
	senddata.maxMoneyByTime = 0.1 * COIN;
	senddata.maxMoneyByDay = 0.2 * COIN;

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;

	cout<<"Modify data:"<<sendcontract<<endl;
	cout<<"Modify strAppRegId:"<<strAppRegId<<endl;
	Value  retValue= basetest.CreateContractTx(strAppRegId,ADDR_ADMINISTRATOR_C,sendcontract,0,0,nTempSend);
	if (basetest.GetHashFromCreatedTx(retValue, strTxHash)) {
		nStep++;
	}else{
		cout<<"Modify err end"<<endl;
		return false;
	}

#if 1

	while(true)
	{
		if(WaitComfirmed(strTxHash, strAppRegId)) {
					break;
				}
	}
#endif
	cout<<"Modify success end"<<endl;
	return false;
}


bool CKaolaTest::Recharge()
{
   cout<<"Recharge start"<<endl;

   COMPANY_RECHARGE senddata;
	memset(&senddata,0,sizeof(senddata));
	senddata.type = TX_RECHARGE;
//	senddata.moneyM = 1 * COIN;

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 1 * COIN;

    cout<<"Recharge data:"<<sendcontract<<endl;
	Value  retValue= basetest.CreateContractTx(strAppRegId,ADDR_ADMINISTRATOR_C,sendcontract,0,0,nTempSend);
	if (basetest.GetHashFromCreatedTx(retValue, strTxHash)) {
		nStep++;

	}else{
	    cout<<"Recharge err end"<<endl;
	    return false;
    }

#if 1

	while(true)
	{
		if(WaitComfirmed(strTxHash, strAppRegId)) {
				break;
			}
	}
#endif
	 cout<<"Recharge success end"<<endl;
	return false;
}

bool CKaolaTest::Withdraw()
{
   cout<<"Withdraw start"<<endl;

   COMPANY_WITHDRAW senddata;
	memset(&senddata,0,sizeof(senddata));
	senddata.type = TX_WITHDRAW;
	strcpy(senddata.address,ADDR_USER_B);
	senddata.moneyM = 0.1 * COIN;

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;
    cout<<"Withdraw data:"<<sendcontract<<endl;
	Value  retValue= basetest.CreateContractTx(strAppRegId,ADDR_COMPANY_A,sendcontract,0,0,nTempSend);
	if (basetest.GetHashFromCreatedTx(retValue, strTxHash)) {
		nStep++;

	}else{
	    cout<<"Withdraw err end"<<endl;
	    return false;
    }

#if 1

	while(true)
	{
		if(WaitComfirmed(strTxHash, strAppRegId)) {
					break;
		}
	}
#endif

	 cout<<"Withdraw success end"<<endl;
	return false;
}


BOOST_FIXTURE_TEST_SUITE(CKaolaTxTest,CKaolaTest)

BOOST_FIXTURE_TEST_CASE(Test,CKaolaTest)
{
	Run();
}
BOOST_AUTO_TEST_SUITE_END()

