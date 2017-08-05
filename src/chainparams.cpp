// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Dacrs developers
// Copyright (c) 2016 The Koala developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "assert.h"
#include "core.h"
#include "protocol.h"
#include "util.h"
#include "key.h"
#include "tx.h"
#include "main.h"

#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string/case_conv.hpp> // for to_lower()
#include <boost/algorithm/string/predicate.hpp> // for startswith() and endswith()
#include <boost/filesystem.hpp>
using namespace boost::assign;
using namespace std;

map<string, string> CBaseParams::m_mapArgs;
map<string, vector<string> > CBaseParams::m_mapMultiArgs;



vector<string> intPubKey_mainNet = {
		"03bcb185cf039c7dee4278c0b6bf57f18811b9e455392ef4dae748e94494ebfc7b",
		"03368764670c2050bce43d198c59f66ddccbdf9e43766b987e771565d1924922ba",
		"03390124787417836ace3e32c2303282e4ed86aec2af539a29a962a4d68d69e901"
};

vector<string> initPubKey_testNet = { //
		"02400a30fb5dfe54a788225fcaa276391c7a5aca08e2f6d377e75b03ad820baf3a",
		"02eedc6ba2961bcd4c9b2acee34f803d8990b2bc968c89dbc1f8b4b88f9e8d325c"
};

vector<string> initPubkey_regTest = {
		"03e7d5bbe9f877d9307c5d0cd820318e79d7a4a05202180c4fa69a682e4a00f2f2",
		"0293fec091a804e4c4a87a5280fb4ede21e53e0f8cd0ec11f4c0948c1076da6736",
		"03489ff18cdfa39d7bcd52d3743689e2ed6383e84d548617c3a400fe4f6266fbd6",
		"029facd7ebef39045ce7a8d293ca39f489fff459c1685d5ff816322d8dc7bcb96d",
		"02b5ca759b3014372905edf0b4cf04e86a378dfb91cdd631412c61b08c9b4f4c21"
};

/*
 *
 * 	27.50.48.26  0x1a30321b
	27.50.48.27  0x1b30321b
	27.50.48.28  0x1c30321b
	27.50.48.30  0x1e30321b
	27.50.48.29  0x1d30321b
 *
 *
 */

unsigned int pnSeed[] = //
		{0x1a30321b, 0x1b30321b, 0x1c30321b, 0x1e30321b, 0x1d30321b};

class CMainParams: public CBaseParams {
public:
	CMainParams() {

// The message start string is designed to be unlikely to occur in normal data.
// The characters are rarely used upper ASCII, not valid as UTF-8, and produce
// a large 4-byte int at any alignment.
		pchMessageStart[0] = 0xff;
		pchMessageStart[1] = 0xf2;
		pchMessageStart[2] = 0xff;
		pchMessageStart[3] = 0xf2;
		vAlertPubKey =	ParseHex("039cf20139ed9985177d8629cca26b1257ea759d98d8813c905db4fc44b8afca5d");
		nDefaultPort = 9666;
		nRPCPort = 18956;
		nUIPort = 4930;
		strDataDir = "main";
		bnProofOfStakeLimit =~arith_uint256(0) >> 10;        //00 3f ff ff
		nSubsidyHalvingInterval = 525600;

		assert(CreateGenesisRewardTx(genesis.vptx, intPubKey_mainNet));
		genesis.SetHashPrevBlock(uint256());
		genesis.SetHashMerkleRoot(genesis.BuildMerkleTree());
		genesis.SetHashPos(uint256());
		genesis.SetVersion(1);
		genesis.SetTime(1501052516);
		genesis.SetBits(0x1f3fffff);        //00 3f ff
		genesis.SetNonce(567);
		genesis.SetFuelRate(INIT_FUEL_RATES);
		genesis.SetHeight(0);
		genesis.ClearSignature();
		hashGenesisBlock = genesis.GetHash();
		publicKey = "039cf20139ed9985177d8629cca26b1257ea759d98d8813c905db4fc44b8afca5d";
//		{
//			cout << "main hashGenesisBlock:\r\n" << hashGenesisBlock.ToString() << endl;
//			cout << "main hashMerkleRoot:\r\n" << genesis.GetHashMerkleRoot().ToString() << endl;
//		}
		assert(hashGenesisBlock == uint256S("0x2a61066903c0c1c2abd99d5eb2a4110025efca7bc1adf1dbe806d51c7a80c6fe"));
		assert(genesis.GetHashMerkleRoot() == uint256S("0x8d0053f29aefb59f02c82532c349753ab6d933c4b34541a98bd77923e2e7580f"));

//      vSeeds.push_back(CDNSSeedData("soypay.org.cn", "seed_cn_0.dspay.org"));
//      vSeeds.push_back(CDNSSeedData("soypay.org.us", "seed_us_0.dspay.org"));

        base58Prefixes[PUBKEY_ADDRESS] = {45};
		base58Prefixes[SCRIPT_ADDRESS] = {10};
		base58Prefixes[SECRET_KEY] = {136};
		base58Prefixes[EXT_PUBLIC_KEY] = {0x05,0x58,0xA1,0x2F};
		base58Prefixes[EXT_SECRET_KEY] = {0x05,0x58,0xE2,0xD4};

		// Convert the pnSeeds array into usable address objects.
		for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++) {
			// It'll only connect to one or two seed nodes because once it connects,
			// it'll get a pile of addresses with newer timestamps.
			// Seed nodes are given a random 'last seen time' of between one and two
			// weeks ago.
			const int64_t nOneWeek = 7 * 24 * 60 * 60;
			struct in_addr ip;
			memcpy(&ip, &pnSeed[i], sizeof(ip));
			CAddress addr(CService(ip, GetDefaultPort()));
			addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
			vFixedSeeds.push_back(addr);
		}
	}

	virtual const CBlock& GenesisBlock() const {
		return genesis;
	}
	virtual Network NetworkID() const {
		return CBaseParams::MAIN;
	}
	virtual bool InitalConfig() {
		return CBaseParams::InitalConfig();
	}
	virtual int GetBlockMaxNonce() const
	{
		return 1000;
	}
	virtual const vector<CAddress>& FixedSeeds() const {
		return vFixedSeeds;
	}
	virtual bool IsInFixedSeeds(CAddress &addr) {
		vector<CAddress>::iterator iterAddr = find(vFixedSeeds.begin(), vFixedSeeds.end(), addr);
		return iterAddr != vFixedSeeds.end();
	}

protected:
	CBlock genesis;
	vector<CAddress> vFixedSeeds;
};
//static CMainParams mainParams;

//
// Testnet (v3)
//
class CTestNetParams: public CMainParams {
public:
	CTestNetParams() {
		// The message start string is designed to be unlikely to occur in normal data.
		// The characters are rarely used upper ASCII, not valid as UTF-8, and produce
		// a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0x2a;
        pchMessageStart[2] = 0x2d;
        pchMessageStart[3] = 0x3d;
		vAlertPubKey =	ParseHex("0237599d64799941009dd6673cca01c103c6b65ac4246fd74a86a288d1f317c38d");
		nDefaultPort = 19666;
		nRPCPort = 18959;
		nUIPort = 4948;
		strDataDir = "testnet";
		publicKey = "0237599d64799941009dd6673cca01c103c6b65ac4246fd74a86a288d1f317c38d";
		// Modify the testnet genesis block so the timestamp is valid for a later start.
		genesis.SetTime(1501052645);
		genesis.SetNonce(356);
		genesis.vptx.clear();
		assert(CreateGenesisRewardTx(genesis.vptx, initPubKey_testNet));
		genesis.SetHashMerkleRoot(genesis.BuildMerkleTree());
		hashGenesisBlock = genesis.GetHash();
		for(auto & item : vFixedSeeds)
			item.SetPort(GetDefaultPort());

//		{
//			cout << "testnet hashGenesisBlock:\r\n" << hashGenesisBlock.ToString() << endl;
//		}
		assert(hashGenesisBlock == uint256S("0x09b5a130c3f623865043f8321697e5ac7cf774c7090e62bb745f3ad761cbe181"));
//		vSeeds.clear();
//		vSeeds.push_back(CDNSSeedData("Dacrs.petertodd.org", "testnet-seed.Dacrs.petertodd.org"));
//		vSeeds.push_back(CDNSSeedData("bluematt.me", "testnet-seed.bluematt.me"));

		base58Prefixes[PUBKEY_ADDRESS] = {108};
		base58Prefixes[SCRIPT_ADDRESS] = {16};
		base58Prefixes[SECRET_KEY]     = {168};
		base58Prefixes[EXT_PUBLIC_KEY] = {0x05,0x56,0x68,0x1C};
		base58Prefixes[EXT_SECRET_KEY] = {0x05,0x56,0x1A,0x32};
	}
	virtual Network NetworkID() const {return CBaseParams::TESTNET;}
	virtual bool InitalConfig()
	{
		CMainParams::InitalConfig();
		fServer = true;
		return true;
	}
	virtual int GetBlockMaxNonce() const
	{
		return 1000;
	}
};
//static CTestNetParams testNetParams;

//
// Regression test
//
class CRegTestParams: public CTestNetParams {
public:
	CRegTestParams() {
		pchMessageStart[0] = 0xfb;
		pchMessageStart[1] = 0xdc;
		pchMessageStart[2] = 0x6d;
		pchMessageStart[3] = 0x3f;
		nSubsidyHalvingInterval = 150;
		bnProofOfStakeLimit = ~arith_uint256(0) >> 6;     //target:00000011 11111111 11111111
		genesis.SetTime(1501053287);
		genesis.SetBits(0x2003ffff);
		genesis.SetNonce(925);
		genesis.vptx.clear();
		assert(CreateGenesisRewardTx(genesis.vptx, initPubkey_regTest));
		genesis.SetHashMerkleRoot(genesis.BuildMerkleTree());
		hashGenesisBlock = genesis.GetHash();
		nDefaultPort = 13666;
		nTargetSpacing = 20;
		nTargetTimespan = 30 * 20;
		strDataDir = "regtest";
//		{
//			cout << "regtest hashGenesisBlock:\r\n" << hashGenesisBlock.ToString() << endl;
//			cout << "regtest hashMerkleRoot:\r\n" << genesis.GetHashMerkleRoot().ToString() << endl;
//		}
		assert(hashGenesisBlock == uint256S("0x23e5568cef4ba1c04ac22c259c73b16c44107fea60f4da05a1a140a7329a8b74"));

		vFixedSeeds.clear();
		vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
	}

	virtual bool RequireRPCPassword() const {
		return false;
	}
	virtual Network NetworkID() const {
		return CBaseParams::REGTEST;
	}
	virtual bool InitalConfig() {
		CTestNetParams::InitalConfig();
		fServer = true;
		return true;
	}
};


/********************************************************************************/
const vector<string> &CBaseParams::GetMultiArgs(const string& strArg) {
	return m_mapMultiArgs[strArg];
}
int CBaseParams::GetArgsSize() {
	return m_mapArgs.size();
}
int CBaseParams::GetMultiArgsSize() {
	return m_mapMultiArgs.size();
}

string CBaseParams::GetArg(const string& strArg, const string& strDefault) {
	if (m_mapArgs.count(strArg))
		return m_mapArgs[strArg];
	return strDefault;
}

int64_t CBaseParams::GetArg(const string& strArg, int64_t nDefault) {
	if (m_mapArgs.count(strArg))
		return atoi64(m_mapArgs[strArg]);
	return nDefault;
}

bool CBaseParams::GetBoolArg(const string& strArg, bool fDefault) {
	if (m_mapArgs.count(strArg)) {
		if (m_mapArgs[strArg].empty())
			return true;
		return (atoi(m_mapArgs[strArg]) != 0);
	}
	return fDefault;
}

bool CBaseParams::SoftSetArg(const string& strArg, const string& strValue) {
	if (m_mapArgs.count(strArg))
		return false;
	m_mapArgs[strArg] = strValue;
	return true;
}

bool CBaseParams::SoftSetArgCover(const string& strArg, const string& strValue) {
	m_mapArgs[strArg] = strValue;
	return true;
}

void CBaseParams::EraseArg(const string& strArgKey) {
	m_mapArgs.erase(strArgKey);
}

bool CBaseParams::SoftSetBoolArg(const string& strArg, bool fValue) {
	if (fValue)
		return SoftSetArg(strArg, string("1"));
	else
		return SoftSetArg(strArg, string("0"));
}

bool CBaseParams::IsArgCount(const string& strArg) {
	if (m_mapArgs.count(strArg)) {
		return true;
	}
	return false;
}

CBaseParams &SysCfg() {
	static shared_ptr<CBaseParams> pParams;

	if (pParams.get() == NULL) {
		bool fRegTest = CBaseParams::GetBoolArg("-regtest", false);
		bool fTestNet = CBaseParams::GetBoolArg("-testnet", false);
		if (fTestNet && fRegTest) {
			fprintf(stderr, "Error: Invalid combination of -regtest and -testnet.\n");
//			assert(0);
		}

		if (fRegTest) {
			//LogPrint("spark", "In Reg Test Net\n");
			pParams = std::make_shared<CRegTestParams>();
		} else if (fTestNet) {
			//LogPrint("spark", "In Test Net\n");
			pParams = std::make_shared<CTestNetParams>();
		} else {
			//LogPrint("spark", "In Main Net\n");
			pParams = std::make_shared<CMainParams>();
		}

	}


	assert(pParams != NULL);
	return *pParams.get();
}

//write for test code
const CBaseParams &SysParamsMain() {
	static std::shared_ptr<CBaseParams> pParams;
	pParams = std::make_shared<CMainParams>();
	assert(pParams != NULL);
	return *pParams.get();
}

//write for test code
const CBaseParams &SysParamsTest() {
	static std::shared_ptr<CBaseParams> pParams;
	pParams = std::make_shared<CTestNetParams>();
	assert(pParams != NULL);
	return *pParams.get();
}

//write for test code
const CBaseParams &SysParamsReg() {
	static std::shared_ptr<CBaseParams> pParams;
	pParams = std::make_shared<CRegTestParams>();
	assert(pParams != NULL);
	return *pParams.get();
}

static void InterpretNegativeSetting(string name, map<string, string>& mapSettingsRet) {
	// interpret -nofoo as -foo=0 (and -nofoo=0 as -foo=1) as long as -foo not set
	if (name.find("-no") == 0) {
		string positive("-");
		positive.append(name.begin() + 3, name.end());
		if (mapSettingsRet.count(positive) == 0) {
			bool value = !SysCfg().GetBoolArg(name, false);
			mapSettingsRet[positive] = (value ? "1" : "0");
		}
	}
}

void CBaseParams::ParseParameters(int argc, const char* const argv[]) {
	m_mapArgs.clear();
	m_mapMultiArgs.clear();
	for (int i = 1; i < argc; i++) {
		string str(argv[i]);
		string strValue;
		size_t is_index = str.find('=');
		if (is_index != string::npos) {
			strValue = str.substr(is_index + 1);
			str = str.substr(0, is_index);
		}
#ifdef WIN32
		boost::to_lower(str);
		if (boost::algorithm::starts_with(str, "/"))
			str = "-" + str.substr(1);
#endif
		if (str[0] != '-')
			break;

		m_mapArgs[str] = strValue;
		m_mapMultiArgs[str].push_back(strValue);
	}

	// New 0.6 features:
//	BOOST_FOREACH(const PAIRTYPE(string,string)& entry, m_mapArgs) {
	for (auto& entry : m_mapArgs) {
		string name = entry.first;

		//  interpret --foo as -foo (as long as both are not set)
		if (name.find("--") == 0) {
			string singleDash(name.begin() + 1, name.end());
			if (m_mapArgs.count(singleDash) == 0)
				m_mapArgs[singleDash] = entry.second;
			name = singleDash;
		}

		// interpret -nofoo as -foo=0 (and -nofoo=0 as -foo=1) as long as -foo not set
		InterpretNegativeSetting(name, m_mapArgs);
	}
#if 0
	for(const auto& tmp:m_mapArgs) {
		printf("key:%s - value:%s\n", tmp.first.c_str(), tmp.second.c_str());
	}
#endif
}

bool CBaseParams::CreateGenesisRewardTx(vector<std::shared_ptr<CBaseTransaction> > &vRewardTx, const vector<string> &vInitPubKey) {
	int length = vInitPubKey.size();
	for (int i = 0; i < length; ++i) {
		int64_t money(0);
		if( i > 0) {
			money = 360000000 * COIN;
		}
		shared_ptr<CRewardTransaction> pRewardTx = std::make_shared<CRewardTransaction>(ParseHex(vInitPubKey[i].c_str()), money, 0);
		pRewardTx->nVersion = nTxVersion1;
		if (pRewardTx.get())
			vRewardTx.push_back(pRewardTx);
		else
			return false;
	}
	return true;

};

bool CBaseParams::IntialParams(int argc, const char* const argv[]) {
	ParseParameters(argc, argv);
	if (!boost::filesystem::is_directory(GetDataDir(false))) {
		fprintf(stderr, "Error: Specified data directory \"%s\" does not exist.\n", CBaseParams::m_mapArgs["-datadir"].c_str());
		return false;
	}
	try {
		ReadConfigFile(CBaseParams::m_mapArgs, CBaseParams::m_mapMultiArgs);
	} catch (exception &e) {
		fprintf(stderr, "Error reading configuration file: %s\n", e.what());
		return false;
	}
	return true;
}

int64_t CBaseParams::GetTxFee() const{
     return paytxfee;
}
int64_t CBaseParams::SetDeflautTxFee(int64_t fee)const{
	paytxfee = fee;

	return fee;
}

CBaseParams::CBaseParams() {
	fImporting = false;
	fReindex = false;
	fBenchmark = false;
	fTxIndex = false;
	nIntervalPos = 1;
	nLogmaxsize = 100 * 1024 * 1024;//100M
	nTxCacheHeight = 500;
	nTimeBestReceived = 0;
	nScriptCheckThreads = 0;
	nViewCacheSize = 2000000;
	nTargetSpacing = 60;
	nTargetTimespan = 30 * 60;
	nSubsidyHalvingInterval = 0;
	paytxfee = 10000;
	nDefaultPort = 0;
	fPrintToConsole= 0;
	fPrintToToFile = 0;
	fLogTimestamps = 0;
	fLogPrintFileLine = 0;
	fDebug = 0;
	fDebugAll= 0 ;
	fServer = 0 ;
	fServer = 0;
	nRPCPort = 0;
	bOutPut = false;
	nUIPort = 0;


}
/********************************************************************************/

