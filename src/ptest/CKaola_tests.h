/*
 * CBlackHalo_tests.h
 *
 *  Created on: 2015-04-24
 *      Author: frank.shi
 */

#ifndef CANONY_TESTS_H
#define CANONY_TESTS_H

#include "CycleTestBase.h"
#include "../test/systestbase.h"
#include "../rpc/rpcclient.h"
#include "tx.h"

using namespace std;
using namespace boost;
using namespace json_spirit;


#define TX_CONFIG     0x01
#define TX_RECHARGE   0x02
#define TX_WITHDRAW   0x03


enum GETDAWEL{
	TX_REGID = 0x01,
	TX_BASE58 = 0x02,
};

typedef struct {
	unsigned char type;
	uint64_t maxMoneyByTime;
	uint64_t maxMoneyByDay;
	char  address[35];
	IMPLEMENT_SERIALIZE
	(
			READWRITE(type);
			READWRITE(maxMoneyByTime);
			READWRITE(maxMoneyByDay);
			for(int i = 0; i < 35; i++)
			{
				READWRITE(address[i]);
			}
	)
}COMPANY_CONFIG;
typedef struct {
	unsigned char type;
	uint64_t maxMoneyByTime;
	uint64_t maxMoneyByDay;
//	char  address[35];
	IMPLEMENT_SERIALIZE
	(
			READWRITE(type);
			READWRITE(maxMoneyByTime);
			READWRITE(maxMoneyByDay);
//			for(int i = 0; i < 220; i++)
//			{
//				READWRITE(address[i]);
//			}
	)
}COMPANY_CONFIG_MODIFY;

typedef struct {
	unsigned char type;
//	uint64_t moneyM;

	IMPLEMENT_SERIALIZE
	(
			READWRITE(type);
//			READWRITE(moneyM);
	)
}COMPANY_RECHARGE;


typedef struct {
	unsigned char type;
	char  address[35];
	uint64_t moneyM;

	IMPLEMENT_SERIALIZE
	(
			READWRITE(type);
			for(int i = 0; i < 35; i++)
			{
				READWRITE(address[i]);
			}
			READWRITE(moneyM);
	)
} COMPANY_WITHDRAW;


class CKaolaTest: public CycleTestBase {
	int nNum;
	int nStep;
	string strTxHash;
	string strAppRegId;
public:
	CKaolaTest();
	~CKaolaTest(){};
	virtual TEST_STATE Run() ;
	bool RegistScript();

	bool Config(void);
	bool Modify(void);
	bool Recharge(void);
	bool Withdraw(void);
};

#endif /* CANONY_TESTS_H */
