
#include <string>
#include <map>
#include <deque>
#include <math.h>
#include "tx.h"
#include "systestbase.h"
#include "miner.h"
#include "../json/json_spirit_value.h"
#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>
using namespace std;

const unsigned int iTxCount = 6000;
vector<std::shared_ptr<CBaseTransaction> > vTransactions;
vector<string> vTransactionHash;
deque<uint64_t> dFee;
deque<uint64_t> dFuel;
uint64_t llTotalFee(0);
static uint64_t registerAppFee(0);

#define MAIN_ADDR "kbMstE4Y1zCHtXXcAfw2Xk1cwVoU9qEPzz"

map<string, string> mapAddress =
        boost::assign::map_list_of
        ("000000000100",	MAIN_ADDR)
        ("000000000200",	"kcJMQ4GVZRu9ap9tMQx87cp1s6PeYiYo9H")
        ("000000000300",	"kjeEnpZA43M9igfQpgyQ4Ze1DE6bvaapyc");

vector<std::tuple<int, uint64_t, string> > vFreezeItem;
vector<pair<string, uint64_t> > vSendFee;

std::string regScriptId("");


map<string, string>::iterator GetRandAddress() {
//	srand(time(NULL));
	unsigned char cType;
	RAND_bytes(&cType, sizeof(cType));
	int iIndex = (cType % 3);
	map<string, string>::iterator iterAddress = mapAddress.begin();
	while(iIndex--) {
		++iterAddress;
	}
	return iterAddress;
}


int GetRandTxType() {
	unsigned char cType;
	RAND_bytes(&cType, sizeof(cType));
	//srand(time(NULL));
	int iIndex = cType % 4;
	return iIndex + 1;
}


class PressureTest: public SysTestBase {
public:
	bool GetContractData(string regId, vector<unsigned char> &vContract) {
		for(auto &addr : mapAddress) {
			if(addr.first == regId)
				continue;

			uint64_t llmoney = GetRandomMoney() * COIN;
			CRegID reg(addr.first);
			vContract.insert(vContract.end(), reg.GetVec6().begin(), reg.GetVec6().end());
			CDataStream ds(SER_DISK, CLIENT_VERSION);
			ds << llmoney;
			vector<unsigned char> temp(ds.begin(), ds.end());
			vContract.insert(vContract.end(), temp.begin(), temp.end());
		}
		return true;
	}

	bool InitRegScript() {
		ResetEnv();

		string hash = "";
		//auto itr = mapAddress.begin();
		//++itr;
		//string addr1 = itr->second;
		//++itr;
		//string addr2 = itr->second;
		BOOST_CHECK(CreateRegScriptTx(false, hash,MAIN_ADDR));
		//BOOST_CHECK(CreateCommonTx(MAIN_ADDR, addr1, 100000000));
		//BOOST_CHECK(CreateCommonTx(MAIN_ADDR, addr2, 100000000));
		BOOST_CHECK(SetBlockGenerte(MAIN_ADDR));
		BOOST_CHECK(GetTxConfirmedRegID(hash,regScriptId));
		//BOOST_CHECK(GetHashFromCreatedTx(RegistAccountTx(addr1, 10000), hash));
		//BOOST_CHECK(GetHashFromCreatedTx(RegistAccountTx(addr2, 10000), hash));
		cout <<regScriptId << endl;
		return true;
	}


	bool CreateCommonTx(string srcAddr, string desAddr, int mon = 1000000) {
		char fee[64] = {0};
		int nfee = GetRandomFee() + mon;
		sprintf(fee, "%d", nfee);
		char money[64] = {0};
		int nmoney = GetRandomMoney();
		sprintf(money, "%d00000000", nmoney);
		const char *argv[] = { "rpctest", "sendtoaddresswithfee", srcAddr.c_str(), desAddr.c_str(), money, fee};
		int argc = sizeof(argv) / sizeof(char*);
		Value value;
		if (!CommandLineRPC_GetValue(argc, argv, value)) {
			return false;
		}
		const Value& result = find_value(value.get_obj(), "hash");
		if(result == null_type) {
			return false;
		}
		string txHash = result.get_str();
		vTransactionHash.push_back(txHash);
		if (mempool.mapTx.count(uint256(uint256S(txHash))) > 0) {
			std::shared_ptr<CBaseTransaction> tx = mempool.mapTx[uint256(uint256S(txHash))].GetTx();
			vTransactions.push_back(tx);
		}
		vSendFee.push_back(make_pair(txHash, nfee));
		return true;
	}

	bool CreateRegAcctTx() {

		const char *argv[] = {"rpctest", "getnewaddress"};
		int argc = sizeof(argv) /sizeof(char *);
		Value value;
		if(!CommandLineRPC_GetValue(argc, argv, value))
		{
			return false;
		}
		const Value& retNewAddr = find_value(value.get_obj(), "addr");
		if(retNewAddr.type() == null_type) {
			return false;
		}
		string newAddress = retNewAddr.get_str();
		map<string, string>::iterator iterSrcAddr = GetRandAddress();

		if(!CreateCommonTx(iterSrcAddr->second, newAddress))
			return false;

		int nfee = GetRandomFee() + 100000000;

		Value result = RegistAccountTx(newAddress, nfee);
		string txHash = "";
		BOOST_CHECK(GetHashFromCreatedTx(value, txHash));

		vTransactionHash.push_back(txHash);
		if (mempool.mapTx.count(uint256(uint256S(txHash))) > 0) {
			std::shared_ptr<CBaseTransaction> tx = mempool.mapTx[uint256(uint256S(txHash))].GetTx();
			vTransactions.push_back(tx);
		}
		vSendFee.push_back(make_pair(txHash, nfee));
		return true;
	}


	bool CreateContractTx(int seq) {
		//map<string, string>::iterator iterSrcAddr = GetRandAddress();
		string srcAddr(MAIN_ADDR);

		unsigned char cType;
		RAND_bytes(&cType, sizeof(cType));
		int iIndex = cType % 2;
		iIndex +=20;
		string contact =strprintf("%02x",iIndex);

		int nfee = GetRandomFee() + COIN + seq;
		uint64_t llmoney = GetRandomMoney() * COIN;
		int height;
		SysTestBase::GetBlockHeight(height);
		Value value = SysTestBase::CreateContractTx(regScriptId,srcAddr, contact, height, nfee, llmoney);
		string txHash = "";
		BOOST_CHECK(GetHashFromCreatedTx(value,txHash));

		vTransactionHash.push_back(txHash);
		if (mempool.mapTx.count(uint256(uint256S(txHash))) > 0) {
			std::shared_ptr<CBaseTransaction> tx = mempool.mapTx[uint256(uint256S(txHash))].GetTx();
			vTransactions.push_back(tx);
		}
		vSendFee.push_back(make_pair(txHash, nfee));
		return true;
	}

	bool CreateRegScriptTx(bool fFlag, string &hash,string regAddress="") {
		if(regAddress=="") {
			map<string, string>::iterator iterSrcAddr = GetRandAddress();
			regAddress = iterSrcAddr->second;
		}
		uint64_t nFee = GetRandomFee() + 1 * COIN;
		registerAppFee = nFee;
		Value ret = RegisterAppTx(regAddress, "unit_test.bin", 100, nFee);
		BOOST_CHECK(GetHashFromCreatedTx(ret,hash));

		if(fFlag) {
			vTransactionHash.push_back(hash);
			if (mempool.mapTx.count(uint256(uint256S(hash))) > 0) {
				std::shared_ptr<CBaseTransaction> tx = mempool.mapTx[uint256(uint256S(hash))].GetTx();
				vTransactions.push_back(tx);
			}
			vSendFee.push_back(make_pair(hash, nFee));
		}

		return true;
	}


	void CreateRandTx(int txCount) {
		for (int i = 0; i < txCount; ++i) {
			//int nTxType = GetRandTxType();
			switch(3) {
			case 1:
//				{
//					BOOST_CHECK(CreateRegAcctTx());
//					++i;
//				}
				--i;
				break;
			case 2:
				{
					map<string, string>::iterator iterSrcAddr = GetRandAddress();
					map<string, string>::iterator iterDesAddr = GetRandAddress();
					while(iterDesAddr->first == iterSrcAddr->first) {
						iterDesAddr = GetRandAddress();
					}
					BOOST_CHECK(CreateCommonTx(iterSrcAddr->second, iterDesAddr->second));
				}
				break;
			case 3:
				{
					BOOST_CHECK(CreateContractTx(i));
				}
				break;
			case 4:
//				{
//					string hash ="";
//					BOOST_CHECK(CreateRegScriptTx(true,hash));
//				}
				--i;
				break;
			default:
				assert(0);
			}
//			cout << "create tx order:" << i << "type:" <<nTxType << endl;
			//putchar('\b');
			if(0 != i) {
				ShowProgress("create tx progress: ",(int)(((i+1)/(float)txCount) * 100));
			}
		}
	}

	bool DetectionAccount(uint64_t llFuelValue, uint64_t llFees) {
		uint64_t freeValue(0);
		uint64_t totalValue(0);
		uint64_t scriptaccValue(0);
		for(auto & item : mapAddress) {
			CRegID regId(item.first);
			CUserID userId = regId;
			{
				LOCK(cs_main);
				CAccount account;
				CAccountViewCache accView(*pAccountViewTip, true);
				if (!accView.GetAccount(userId, account)) {
					return false;
				}
				freeValue += account.GetRawBalance();
			}

		}

		if(regScriptId != ""){
			CRegID regId(regScriptId);
			CUserID userId = regId;
			{
				LOCK(cs_main);
				CAccount account;
				CAccountViewCache accView(*pAccountViewTip, true);
				if (!accView.GetAccount(userId, account)) {
					return false;
				}
				scriptaccValue += account.GetRawBalance();
			}

		}
		totalValue += freeValue;
		totalValue += scriptaccValue;

		uint64_t uTotalRewardValue(0);
		if (chainActive.Tip()->nHeight - 1 > COINBASE_MATURITY)  //height 1 is generate by another account
			uTotalRewardValue = 10 * COIN * (chainActive.Tip()->nHeight - 101);
		dFee.push_back(llFees);
		dFuel.push_back(llFuelValue);
		llTotalFee += llFees;

		if (dFee.size() > 100) {
			uint64_t feeTemp = dFee.front();
			uint64_t fuelTemp = dFuel.front();
			llTotalFee -= feeTemp;
			llTotalFee += fuelTemp;
			dFee.pop_front();
			dFuel.pop_front();
		}

		BOOST_CHECK_EQUAL(totalValue + llTotalFee + registerAppFee, (3000000000 * COIN + uTotalRewardValue));
		return true;
	}

	bool SetBlockGenerte(const char *addr)
	{
		return SetAddrGenerteBlock(addr);

	}
};

BOOST_FIXTURE_TEST_SUITE(pressure_tests, PressureTest)
BOOST_FIXTURE_TEST_CASE(tests, PressureTest)
{

	InitRegScript();
//	BOOST_CHECK(DetectionAccount(1*COIN));
	for(int i=0; i<1; ++i) {


		CreateRandTx(iTxCount);

		BOOST_CHECK_EQUAL(vTransactions.size(), iTxCount);
		{
			//LOCK(pwalletMain->cs_wallet);

			BOOST_CHECK_EQUAL(pwalletMain->UnConfirmTx.size(), iTxCount);

			//for(auto &item : vTransactionHash) {
				//BOOST_CHECK(pwalletMain->UnConfirmTx.count(uint256(uint256S(item))) > 0);
			//}

		}

		unsigned int nSize = mempool.mapTx.size();
		int nConfirmTxCount(0);
		uint64_t llRegAcctFee(0);
		uint64_t llSendValue(0);
		uint64_t llFuelValue(0);
		while (nSize) {

			map<string, string>::const_iterator iterAddr = GetRandAddress();
			BOOST_CHECK(SetBlockGenerte(MAIN_ADDR));
			CBlock block;
			{
				LOCK(cs_main);
				CBlockIndex *pindex = chainActive.Tip();
				llFuelValue += pindex->nFuel;
				BOOST_CHECK(ReadBlockFromDisk(block, pindex));
			}

			for(auto &item : block.vptx) {
				{
					LOCK2(cs_main, pwalletMain->cs_wallet);

					BOOST_CHECK(!pwalletMain->UnConfirmTx.count(item->GetHash()) > 0);

					BOOST_CHECK(pwalletMain->mapInBlockTx[block.GetHash()].mapAccountTx.count(item->GetHash())>0);

					BOOST_CHECK(!mempool.mapTx.count(item->GetHash()) > 0);
				}

			}
			{
				LOCK2(cs_main, pwalletMain->cs_wallet);

				BOOST_CHECK(pwalletMain->mapInBlockTx[block.GetHash()].mapAccountTx.size() == block.vptx.size());
				nConfirmTxCount += block.vptx.size() - 1;

				nSize = mempool.mapTx.size();
				BOOST_CHECK_EQUAL((nSize + nConfirmTxCount), vTransactions.size());

				BOOST_CHECK_EQUAL(nSize, pwalletMain->UnConfirmTx.size());
			}

			BOOST_CHECK(block.GetSerializeSize(SER_DISK, CLIENT_VERSION) <= MAX_BLOCK_SIZE);
			for(auto & ptx : block.vptx) {
				if(ptx->IsCoinBase()) {
					continue;
				}
				if(REG_ACCT_TX == ptx->nTxType) {
					llRegAcctFee += ptx->GetFee();
				}
				if(COMMON_TX == ptx->nTxType) {
					std::shared_ptr<CTransaction> pTransaction(dynamic_pointer_cast<CTransaction>(ptx));
					if(typeid(pTransaction->desUserId) == typeid(CKeyID)) {
						llSendValue += pTransaction->llValues;				}
				}
			}
			llSendValue -= llRegAcctFee;
			llSendValue += block.GetFee();

			BOOST_CHECK(DetectionAccount(llFuelValue, block.GetFee()));
		}
		uint64_t totalFee(0);
		for(auto &item : vSendFee) {
			totalFee += item.second;
		}

		BOOST_CHECK_EQUAL(totalFee, llSendValue);

		vTransactions.clear();
		vTransactionHash.clear();
		vSendFee.clear();
	}
}
BOOST_AUTO_TEST_SUITE_END()
