#pragma once

//just for NULL
#include <cstddef>


//定义5张牌的类型 
#define TXPK_CARDS_TYPE_BIGTONGHUASHUN	0x0A	//皇家同花顺
#define TXPK_CARDS_TYPE_TONGHUASHUN		0x09	//同花顺
#define TXPK_CARDS_TYPE_TIEZHI			0x08	//四张
#define TXPK_CARDS_TYPE_HULU			0x07	//三张加一对
#define TXPK_CARDS_TYPE_TONGHUA			0x06	//同花
#define TXPK_CARDS_TYPE_SHUNZI			0x05	//顺子
#define TXPK_CARDS_TYPE_SANTIAO			0x04	//三张
#define TXPK_CARDS_TYPE_ERDUI			0x03	//两对
#define TXPK_CARDS_TYPE_DUIZI			0x02	//一对
#define TXPK_CARDS_TYPE_SANPAI			0x01	//散牌
#define TXPK_CARDS_TYPE_NONE			0x00	//不进行计算



/*
 * note: handCards[2] must not 0 0
 */
class TexasCardHelp 
{
public:
	/*
	* note: 1 may change to A(0x0e)
	* big first: eg 2 2 2 3 3 eg 10 9 8 7 6 eg 88 77 A
	*/
	static int TxPokergetCardsType(const unsigned char tableCards[5], const unsigned char handCards[2], unsigned char selectCards[5] = NULL);

	static int TxPokerGetCardsTypeIn6(const unsigned char tableCards[4], const unsigned char handCards[2], unsigned char selectCards[5] = NULL);

	/*
	*return: 1>2 >0  1==2 =0  1<2 < 0
	*/
	static int TxPokerCompareCards(const unsigned char hcards1[2], const unsigned char hcards2[2], const unsigned char tcards[5]);

private:
	// sorted
	static bool TxPokerIsTongHua(const unsigned char cards[5]);

	/* sorted
	* return: 0 不是顺子 1 普通顺子 2 大顺子 10 11 12 13 A --big First: 10 9 8 7 6
	*/
	static int TxPokerShunZiType(const unsigned char cards[5]);

	//sorted cards 1 may or not change to A(0x0e)
	static bool TxPokerShunZiIn7Cards(const unsigned char cards[7], unsigned char selectCards[5] = NULL);

	static int TxPokerDuiziIn7Cards(const unsigned char cards[7]);

	static bool TxPokerBigTongHuaShun(const unsigned char cards[7], unsigned char selectCards[5]);

};


