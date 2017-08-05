#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "systestbase.h"
using namespace std;
using namespace boost;

class CSysAccountTest:public SysTestBase {
public:

	bool RegisterAccount(const string& strAddr, uint64_t nFee,string& strTxHash,bool bSign = false) {
		Value value = RegistAccountTx(strAddr,nFee);
		return GetHashFromCreatedTx(value,strTxHash);
	}

	bool SendMoney(const string& strRegAddr, const string& strDestAddr, uint64_t nMoney, uint64_t nFee = 0) {
		Value value = CreateNormalTx(strRegAddr,strDestAddr,nMoney);
		string hash = "";
		return GetHashFromCreatedTx(value,hash);
	}

};

BOOST_FIXTURE_TEST_SUITE(sysacct_test, CSysAccountTest)
BOOST_FIXTURE_TEST_CASE(transfer_test, CSysAccountTest)
{
	ResetEnv();
	BOOST_CHECK(0==chainActive.Height());

	string strRegAddr = "kcJMQ4GVZRu9ap9tMQx87cp1s6PeYiYo9H";
	string strtmp = "kbMstE4Y1zCHtXXcAfw2Xk1cwVoU9qEPzz";
	CRegID regid;
	GetRegID(strtmp,regid);
	string strSrcRegID = regid.ToString();
	uint64_t nMoney = 10000000;
	BOOST_CHECK(SendMoney(strSrcRegID,strRegAddr,nMoney));
	BOOST_CHECK(GenerateOneBlock());


	uint64_t nFreeMoney = GetBalance(strRegAddr);
	BOOST_CHECK(nFreeMoney == nMoney);


	string strTxHash;
	CRegID regID;
	BOOST_CHECK(RegisterAccount(strRegAddr,10000,strTxHash));
	BOOST_CHECK(GenerateOneBlock());
	BOOST_CHECK(GetRegID(strRegAddr,regID));
	BOOST_CHECK(false == regID.IsEmpty());


	string strResigterd("kbMstE4Y1zCHtXXcAfw2Xk1cwVoU9qEPzz");
	BOOST_CHECK(!RegisterAccount(strRegAddr,10000,strTxHash));


	string strNewKeyID;
	BOOST_CHECK(GetNewAddr(strNewKeyID,true));


	string strUnRegister("kjeEnpZA43M9igfQpgyQ4Ze1DE6bvaapyc");
	vector<unsigned char> vRegID = regID.GetVec6();
	nMoney = nMoney/10;
	BOOST_CHECK(SendMoney(HexStr(vRegID),strUnRegister,nMoney, 10000));
	BOOST_CHECK(GenerateOneBlock());


	nFreeMoney = GetBalance(strUnRegister);
	BOOST_CHECK(nFreeMoney == nMoney);
}

BOOST_FIXTURE_TEST_CASE(register_test,CSysAccountTest)
 {
	ResetEnv();
	BOOST_CHECK(0==chainActive.Height());
	string strTxHash;
	CRegID regID;
	string strRegAddr1("kjeEnpZA43M9igfQpgyQ4Ze1DE6bvaapyc");
	string strRegAddr2 = "";
	BOOST_CHECK(GetNewAddr(strRegAddr2,false));
	uint64_t nFee = 10000;
	vector<string> vFailedTxHash;


	BOOST_CHECK(!RegisterAccount(strRegAddr1, nFee, strTxHash));
	vFailedTxHash.push_back(strTxHash);


	string strInvalidAddr("fjsofeoifdsfdsfdsafafafafafafafa");
	BOOST_CHECK(!RegisterAccount(strInvalidAddr, nFee, strTxHash));
	vFailedTxHash.push_back(strTxHash);

//
//	BOOST_CHECK(!RegisterAccount(strRegAddr1, nFee, strTxHash,nInValidHeight,false));
//	vFailedTxHash.push_back(strTxHash);


	BOOST_CHECK(!RegisterAccount(strRegAddr1, nFee+GetMaxMoney(), strTxHash));
	vFailedTxHash.push_back(strTxHash);


	string strReRegisrerAddr("kbMstE4Y1zCHtXXcAfw2Xk1cwVoU9qEPzz");
	BOOST_CHECK(!RegisterAccount(strReRegisrerAddr, nFee, strTxHash));
	vFailedTxHash.push_back(strTxHash);


	BOOST_CHECK(!RegisterAccount(strRegAddr2, nFee, strTxHash));
	vFailedTxHash.push_back(strTxHash);

//
//	for (const auto& item : vFailedTxHash) {
//		uint256 txHash(item);
//		BOOST_CHECK(!IsTxInMemorypool(txHash));
//		BOOST_CHECK(!IsTxUnConfirmdInWallet(txHash));
//	}


	string strtmp = "kbMstE4Y1zCHtXXcAfw2Xk1cwVoU9qEPzz";
	CRegID regid;
	GetRegID(strtmp,regid);
	string strSrcRegID = regid.ToString();
	uint64_t nMoney = 10000000;
	BOOST_CHECK(SendMoney(strSrcRegID, strRegAddr2, nMoney));
	BOOST_CHECK(GenerateOneBlock());


	uint64_t nFreeMoney = GetBalance(strRegAddr2);
	BOOST_CHECK(nFreeMoney == nMoney);


	for (const auto& item : vFailedTxHash) {
		uint256 txHash(uint256S(item));
		BOOST_CHECK(!IsTxInMemorypool(txHash));
		BOOST_CHECK(!IsTxUnConfirmdInWallet(txHash));
	}


	BOOST_CHECK(SendMoney(strSrcRegID, strRegAddr1, nMoney));
	BOOST_CHECK(GenerateOneBlock());


	nFreeMoney = GetBalance(strRegAddr1);
	BOOST_CHECK(nFreeMoney == nMoney);

	string strSpecial;
	BOOST_CHECK(RegisterAccount(strRegAddr1, nFee, strSpecial,false));
	BOOST_CHECK(IsTxInMemorypool(uint256(uint256S(strSpecial))));
	BOOST_CHECK(IsTxUnConfirmdInWallet(uint256(uint256S(strSpecial))));


	BOOST_CHECK(!RegisterAccount(strRegAddr1, nFee, strTxHash,false));
	BOOST_CHECK(!IsTxInMemorypool(uint256(uint256S(strTxHash))));
	BOOST_CHECK(!IsTxUnConfirmdInWallet(uint256(uint256S(strTxHash))));

	BOOST_CHECK(GenerateOneBlock());


	BOOST_CHECK(IsTxInTipBlock(uint256(uint256S(strSpecial))));

	vector<CAccountLog> vLog;
	BOOST_CHECK(GetTxOperateLog(uint256(uint256S(strSpecial)),vLog));


//	BOOST_CHECK(1 == vLog.size() && 1 == vLog[0].vOperFund.size() && 1 == vLog[0].vOperFund[0].vFund.size());
	BOOST_CHECK(strRegAddr1 == vLog[0].keyID.ToAddress());
//	BOOST_CHECK(vLog[0].vOperFund[0].operType == MINUS_FREE && vLog[0].vOperFund[0].vFund[0].value == nFee);
}

BOOST_AUTO_TEST_SUITE_END()






