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




#define	TX_REGISTER   0x01
#define TX_MODIFYREGISTER  0x02
#define TX_ARBIT_ON     0x03
#define TX_ARBIT_OFF    0x04
#define	TX_UNREGISTER  0x05
#define	TX_SEND  0x06
#define	TX_CANCEL  0x07
#define	TX_ACCEPT  0x08
#define TX_DELIVERY 0x09
#define	TX_BUYERCONFIRM  0x0a
#define	TX_ARBITRATION  0x0b
#define	TX_FINALRESULT  0x0c ¹û






#define	SEND_TYPE_BUY   0x00
#define	SEND_TYPE_SELL  0x01


typedef struct {
	unsigned char systype;               //0xff
	unsigned char type;
	unsigned char typeaddr;            // 0x01 regid 0x02 base58
	uint64_t     money;

	IMPLEMENT_SERIALIZE
	(
		READWRITE(systype);
		READWRITE(type);
		READWRITE(typeaddr);
		READWRITE(money);
	)
} APPACC_money;

typedef struct {
	unsigned char systype;               //0xff
	unsigned char type;
	unsigned char typeaddr;            // 0x01 regid 0x02 base58
//	uint64_t     money;

	IMPLEMENT_SERIALIZE
	(
		READWRITE(systype);
		READWRITE(type);
		READWRITE(typeaddr);
//		READWRITE(money);
	)
} APPACC;

enum GETDAWEL{
	TX_REGID = 0x01,
	TX_BASE58 = 0x02,
};



typedef struct {
	unsigned char type;
	uint64_t arbiterMoneyX;
	uint64_t overtimeMoneyYmax;
	uint64_t configMoneyZ;
	unsigned int  overtimeheightT;
	char  comment[220];
	IMPLEMENT_SERIALIZE
	(
			READWRITE(type);
			READWRITE(arbiterMoneyX);
			READWRITE(overtimeMoneyYmax);
			READWRITE(configMoneyZ);
			READWRITE(overtimeheightT);
			for(int i = 0; i < 220; i++)
			{
				READWRITE(comment[i]);
			}
	)

}TX_REGISTER_CONTRACT;

typedef struct {
	unsigned char type;
	unsigned char sendType;
	char arbitationID[6];
	uint64_t moneyM;
	unsigned int height;

	char goods[20];
	char  comment[200];
	IMPLEMENT_SERIALIZE
	(
			READWRITE(type);
			READWRITE(sendType);
			for(int i = 0; i < 6; i++)
			{
				READWRITE(arbitationID[i]);
			}
			READWRITE(moneyM);
			READWRITE(height);
			for(int i = 0; i < 20; i++)
			{
				READWRITE(goods[i]);
			}
			for(int i = 0; i < 200; i++)
			{
				READWRITE(comment[i]);
			}
	)
}TX_SNED_CONTRACT;

typedef struct {
	unsigned char type;
	unsigned char txhash[32];
	unsigned int height;
	IMPLEMENT_SERIALIZE
	(
			READWRITE(type);
			for(int i = 0; i < 32; i++)
			{
				READWRITE(txhash[i]);
			}
			READWRITE(height);
	)
} TX_CONTRACT;
typedef struct {
	unsigned char type;
	unsigned char txhash[32];
	unsigned int height;
	char  arbitationID[6];
	IMPLEMENT_SERIALIZE
	(
			READWRITE(type);
			for(int i = 0; i < 32; i++)
			{
				READWRITE(txhash[i]);
			}
			READWRITE(height);
			for(int i = 0; i < 6; i++)
			{
				READWRITE(arbitationID[i]);
			}
	)
} TX_Arbitration;

typedef struct {
	unsigned char type;
	unsigned char arbitHash[32];
	unsigned int overtimeheightT;
	char 	winner[6];
	uint64_t winnerMoney;
	char  loser[6];
	uint64_t loserMoney;
	IMPLEMENT_SERIALIZE
	(
		READWRITE(type);
		for(int i = 0; i < 32; i++)
		{
			READWRITE(arbitHash[i]);
		}
		READWRITE(overtimeheightT);
		for(int i = 0; i < 6; i++)
		{
			READWRITE(winner[i]);
		}
		READWRITE(winnerMoney);
		for(int i = 0; i < 6; i++)
		{
			READWRITE(loser[i]);
		}
		READWRITE(loserMoney);
	)
}TX_FINALRESULT_CONTRACT;


class CGuaranteeTest: public CycleTestBase {
	int nNum;
	int nStep;
	string strTxHash;
	string strAppRegId;
public:
	CGuaranteeTest();
	~CGuaranteeTest(){};
	virtual TEST_STATE Run() ;
	bool RegistScript();

	bool Recharge(void);
	bool Withdraw(void);
	bool WithdrawSomemoney(void);

	bool Register(unsigned char type);
	bool ArbitONOrOFF(unsigned char type);
	bool UnRegister(void);
	bool SendStartTrade(void);
	bool SendCancelTrade(void);
	bool AcceptTrade(void);
	bool DeliveryTrade(void);
	bool BuyerConfirm(void);
    bool Arbitration(void);
    bool RunFinalResult(void);

};

#endif /* CANONY_TESTS_H */
