#include "GuanDanCardHelper.h"
#include <algorithm>
/*
 * 去重，将已经排好序的数组用前面一个去重的数组去掉重复的值，前面的数组相当于后面数组去重之后的范围，
 * 该函数的目的是保留 去掉重复数据 之后的数据
 */
void TxPokerUniqueCards(const unsigned char *beg, const unsigned char *end, unsigned char *xbeg, unsigned char *xend)
{
	for (; beg != end; ++beg) {
		for (; xbeg != xend; ++xbeg) {
			if (*beg == *xbeg) {
				*xbeg = 0;
				++xbeg;
				break;
			}
		}
	}
}


bool TexasCardHelp::TxPokerIsTongHua(const unsigned char cards[5])
{
	unsigned char type = cards[0] >> 4;
	if (0 == cards[0])
		return false;
	for (int i = 1; i != 5; ++i) {
		if (type != cards[i] >> 4 || 0 == cards[i])
			return false;
	}
	return true;
}

int TexasCardHelp::TxPokerShunZiType(const unsigned char cards[5])
{
    bool isChange = false;
    unsigned char cardsValue[5];
    for (int i = 0; i != 5; ++i) {
        cardsValue[i] = cards[i] & 0x0f;
        if (cardsValue[i] == 1 || cardsValue[i] == 0x0e)
            { cardsValue[i] = 0x0e; isChange = true;}
		if (0 == cardsValue[i])
			return 0;
    }

	bool isSeq = true;
#define getisSeq(carr)	do { \
							unsigned char value = carr[0]; \
                            isSeq = true; \
							for (int i = 1; i != 5; ++i) { \
								if ((value+i) != (carr[i])) { \
									isSeq = false; \
									break; \
								} \
							} \
						} while (0)
						
	getisSeq(cardsValue);
	if (isChange) {
        if (isSeq) {
			return 2;
		}
	} else if (isSeq) {
		return 1;   
    }

    for (int i = 0; i != 5; ++i)
        if (cardsValue[i] == 0x0e)
            cardsValue[i] = 1;
    std::sort(cardsValue, cardsValue+5);
    getisSeq(cardsValue);
    if (isSeq) return 1;
    return 0;
}

//选出的牌大的在前面
bool TexasCardHelp::TxPokerShunZiIn7Cards(const unsigned char cards[7], unsigned char selectCards[5])
{
	unsigned char cardsValue[7] = {0};
	int type = 0;
	for (int i = 0; i != 7; ++i) {
		cardsValue[i] = cards[i] & 0x0f;
		if (cardsValue[i] == 1) cardsValue[i] = 0x0e;
	}
	
	std::sort(cardsValue, cardsValue+7);
    unsigned char *cend = std::unique(cardsValue, cardsValue+7);

	if (cend - cardsValue < 5)
		return false;
	
    for (unsigned char * ctmp = cend-5; ctmp != cardsValue-1; --ctmp) {
        int t = TxPokerShunZiType(ctmp);
		if (t > type) type = t;
		if (2 == t) {
			if (selectCards) {
				int index = 0;
				for (unsigned char *ch = ctmp+4; ch != ctmp-1; --ch) {
					for (int j = 0; j != 7; ++j) {
						if ((cards[j] & 0x0f) == *ch || (*ch == 0x0e && ((cards[j] & 0x0f) == 0x01))) {
							selectCards[index++] = cards[j]; 
							break;
						}
					}
				}
			}
			return true;
		}
    }
	if (2 == type) return true;

	for (int i = 0; i != 7; ++i) {
		cardsValue[i] = cards[i] & 0x0f;
		if (cardsValue[i] == 0x0e) cardsValue[i] = 1;
	}
	std::sort(cardsValue, cardsValue+7);
	cend = std::unique(cardsValue, cardsValue+7);
	if (cend - cardsValue < 5)
		return false;
	
	for (unsigned char *ctmp = cend-5; ctmp != cardsValue-1; --ctmp) { // big shunzi first
		if (TxPokerShunZiType(ctmp)) {
			if (selectCards) {
				int index = 0;
				for (unsigned char *ch = ctmp+4; ch != ctmp-1; --ch) {
					for (int j = 0; j != 7; ++j) {
						if ((cards[j] & 0x0f) == *ch || (*ch == 1 && (cards[j] & 0x0f) == 0x0e)) {
							selectCards[index++] = cards[j];
							break;
						}
					}
				}
			}
			return true;
		}
	}
	return false;
}

int TexasCardHelp::TxPokerDuiziIn7Cards(const unsigned char cards[7])
{
	unsigned char cardsValue[7] = {0}, cardsValueBak[7] = {0};
	for (int i = 0; i != 7; ++i)
		cardsValue[i] = cards[i] & 0x0f;
	std::sort(cardsValue, cardsValue+7);
	for (int i = 0; i != 7; ++i)
		cardsValueBak[i] = cardsValue[i];
	
    unsigned char *cend = std::unique(cardsValue, cardsValue+7);

	TxPokerUniqueCards(cardsValue, cend, cardsValueBak, cardsValueBak+7);
	
	
	int duizi = 0, count = 0;
    for (unsigned char *ch = cardsValue; ch != cend; ++ch) {
		if (0 == *ch) continue;
        count = 1;
        for (unsigned char *samech = cardsValueBak; samech != cardsValueBak+7; ++samech)
        {
            if (*samech == *ch) {
				if (count < 2)
                	++count;
				*samech = 0;
            }
        }
        if (2 == count)
            ++duizi;
    }
    return duizi;
}

bool TexasCardHelp::TxPokerBigTongHuaShun(const unsigned char cards[7], unsigned char selectCards[5])
{
    bool ret = false;
    unsigned char allCards[7];
    for (int i = 0; i != 7; ++i) {
        allCards[i] = cards[i];
        if ((allCards[i] & 0x0f) == 1)
            allCards[i] = (allCards[i]&0xf0) + 0x0e;
    }

    std::sort(allCards, allCards+7);
	unsigned char *cend = std::unique(allCards, allCards+7);

	if (cend - allCards < 5)
		return false;

	for (unsigned char *ctmp = allCards; ctmp != cend - 4; ++ctmp)
		if (TxPokerIsTongHua(ctmp) && (2 == TxPokerShunZiType(ctmp))) {
			if (selectCards) {
				for (int i = 0; i != 5; ++i)
					selectCards[i] = *(ctmp+i);
			}
			return true;
		}
	return false;
}


int TexasCardHelp::TxPokerGetCardsTypeIn6(const unsigned char tableCards [ 4 ],const unsigned char handCards [ 2 ],unsigned char selectCards [ 5 ])
{
	unsigned char tableCardsBak[5] = {0};
	for (int i = 0; i != 4; ++i)
		tableCardsBak[i] = tableCards[i];
	return TxPokergetCardsType(tableCardsBak, handCards, selectCards);
}


int TexasCardHelp::TxPokergetCardsType(const unsigned char tableCards[5], const unsigned char handCards[2], unsigned char selectCards[5])
{
	unsigned char allCards[7] = {0};
	int valueCount = 0;
	
	for (int i = 0; i != 5; ++i) {
		allCards[i] = tableCards[i];
		if (allCards[i])
			++valueCount;
	}
	allCards[5] = handCards[0];
	if (allCards[5])
		++valueCount;
	allCards[6] = handCards[1];
	if (allCards[6])
		++valueCount;

	if (valueCount < 5)
		return TXPK_CARDS_TYPE_NONE;
	
	std::sort(allCards, allCards+sizeof(allCards));
	
	if (TxPokerBigTongHuaShun(allCards, selectCards)) {
		return TXPK_CARDS_TYPE_BIGTONGHUASHUN;   //皇家同花顺
	}
	
	{ //同花顺
		unsigned char tongHuaCards[7] = {0};
		for (int i = 0; i != 7; ++i)
			tongHuaCards[i] = allCards[i];
		std::sort(tongHuaCards, tongHuaCards+7);
		unsigned char *cend = std::unique(tongHuaCards, tongHuaCards+7);
		if (cend - tongHuaCards >= 5) {
			for (unsigned char *ctmp = cend - 5; ctmp != tongHuaCards - 1; --ctmp) { //大的优先
				if (0 == *ctmp) continue;
				if (TxPokerIsTongHua(ctmp) && (1 == TxPokerShunZiType(ctmp))) {
					if (selectCards) {
						int index = 0;
						for (int i = 0; i != 5; ++i)
							selectCards[i] = *(ctmp+4-i);
					}
					return TXPK_CARDS_TYPE_TONGHUASHUN;
				}	
			}
		}
	}
	
	// should do this    1 to A(0x0e)
	for (int i = 0 ; i != 7; ++i)
		if ((allCards[i] & 0x0f) == 0x01)
			allCards[i] = (allCards[i] & 0xf0) + 0x0e;

	unsigned char allCardsBak[7] = {0}; 
	unsigned char allCardsBakValue[7] = {0};
	for (int i = 0; i != 7; ++i)
		allCardsBak[i] = allCards[i] & 0x0f;

	std::sort(allCardsBak, allCardsBak+7);
	for (int i = 0; i != 7; ++i)
		allCardsBakValue[i] = allCardsBak[i];
	unsigned char *cend = std::unique(allCardsBak, allCardsBak+7);

	TxPokerUniqueCards(allCardsBak, cend, allCardsBakValue, allCardsBakValue+7);

	int nMaxSameCard = 0, tmpSameCard = 0;
	unsigned char nMaxSameValue = 0;

	for (unsigned char *ctmp = allCardsBak; ctmp != cend; ++ctmp) {
		if (0 == *ctmp)
			continue;
		tmpSameCard = 1;
		for (unsigned char *ch = allCardsBakValue; ch != allCardsBakValue+7; ++ch) {
			if (*ch == *ctmp) {
				++tmpSameCard;
			}
		}
		if (tmpSameCard >= nMaxSameCard && *ctmp > nMaxSameValue) {
			nMaxSameCard = tmpSameCard;
			nMaxSameValue = *ctmp;
		}
	}
	
	
	if (4 == nMaxSameCard) { // copy cards    1 to A(0x0e)
		if (selectCards) {
			int sindex = 0, bindex = 0;
			for (int i = 0; i != 7; ++i) {
				if ((allCards[i] & 0x0f) == nMaxSameValue)
					selectCards[sindex++] = allCards[i];
			}
			unsigned char *xtmp = cend-1;
			while (*xtmp == nMaxSameValue) --xtmp;
			
			for (int i = 0; i != 7; ++i)
				if (*xtmp == (allCards[i] & 0x0f)) {
					selectCards[4] = allCards[i];
					break;
				}
		}
		return TXPK_CARDS_TYPE_TIEZHI;
	}
	
	if (3 == nMaxSameCard) { // note AAA KKK Q ** AAA  10 10 J J ** AAA 10 9 8 8 
		int nSecondMaxCard = 0, tmp = 0;
		unsigned char secondValue = 0;
		for (int i = 0; i != 6; ++i) {
			unsigned char value = allCards[i] & 0x0f;
			if (value == nMaxSameValue || 0 == value) continue;
            tmp = 1;
			for (int j = i+1; j != 7; ++j)
				if (value == (allCards[j] & 0x0f))
					++tmp;
			#if 0
			if (tmp >= nSecondMaxCard && value > secondValue) {
				nSecondMaxCard = tmp;
				secondValue = value;
			}
			#endif
			
			if (tmp >= nSecondMaxCard) { //取张数最多的，牌值最大的，但是大于等于2的时候是牌值最大的
				if (tmp >= 2 ) {
					if (nSecondMaxCard <= 1) {
						nSecondMaxCard = tmp;
						secondValue = value;
					} else {
						if (value > secondValue) {
							nSecondMaxCard = tmp;
							secondValue = value;
						}
					}
				} else {
					if (value > secondValue) {
						nSecondMaxCard = tmp;
						secondValue = value;

					}
				}
			}
			
		}
		
		if (2 == nSecondMaxCard || 3 == nSecondMaxCard) { // copy cards 1 to A(0x0e)
			if (selectCards) {
				int count = 0, index = 0;
				for (int i = 0; i != 7; ++i)
					if (nMaxSameValue == (allCards[i] & 0x0f))
						selectCards[index++] = allCards[i];
				for (int i = 0; i != 7; ++i)
					if ((secondValue == (allCards[i] & 0x0f)) && (count < 2)) {
						selectCards[index++] = allCards[i];
						if (++count == 2)
							break;
					}
				
			}
			return TXPK_CARDS_TYPE_HULU;
		}
	}

	std::sort(allCards, allCards+7);
	int t = 2;
	if (TxPokerIsTongHua(allCards+t) || TxPokerIsTongHua(allCards+ (--t)) || TxPokerIsTongHua(allCards+(--t))) {
		if (selectCards) { // copy cards  1 to A(0x0e)
			for (int i = 0; i != 5; ++i)
				selectCards[i] = *(allCards+t+4-i);
		}
		return TXPK_CARDS_TYPE_TONGHUA;
	}

	if (TxPokerShunZiIn7Cards(allCards, selectCards)) { // above has change 1 to A(0x0e), selectCards include A not 1
		return TXPK_CARDS_TYPE_SHUNZI;
	}

	if (3 == nMaxSameCard) {
		if (selectCards) { // copy cards, 1 to A(0x0e)
			unsigned char bak[4] = {0};
			int index =0, bindex = 0;
			for (int i = 0; i != 7; ++i) {
				if (nMaxSameValue == (allCards[i] & 0x0f))
					selectCards[index++] = allCards[i];
				else
					bak[bindex++] = allCards[i] & 0x0f;
			}
			std::sort(bak, bak+4);
			for (int i = 0; i != 7; ++i) { //大的优先
				if ((allCards[i] & 0x0f) == bak[3]) {
					selectCards[3] = allCards[i];
					break;
				}
			}

			for (int i = 0; i != 7; ++i) {
				if ((allCards[i] & 0x0f) == bak[2]) {
					selectCards[4] = allCards[i];
					break;
				}
			}
			
		}
		return TXPK_CARDS_TYPE_SANTIAO;
	}

	int duizi = TxPokerDuiziIn7Cards(allCards); // 11 22 33 4  or 11 22 3 4 5
	if (2 == duizi || 3 == duizi) {
		if (selectCards) {
			unsigned char dcards[7] = {0}, dcardsBak[7] = {0};
			int index = 0;
			for (int i = 0; i != 7; ++i) {
				dcards[i] = allCards[i] & 0x0f; // in allCards  1 is A(0x0e)
			}
			std::sort(dcards, dcards+7);
			for (int i = 0; i != 7; ++i)
				dcardsBak[i] = dcards[i];
			
			unsigned char *dend = std::unique(dcards, dcards+7);
			TxPokerUniqueCards(dcards, dend, dcardsBak, dcardsBak+7);

			char v1 = 0, v2 = 0;
			for (unsigned char *cctmp = dcardsBak+6; cctmp != dcardsBak-1; --cctmp) {
				if (*cctmp) {
					if (0 == v1)
						v1 = *cctmp;
					else if (0 == v2) {
						v2 = *cctmp;
						break;
					}
					
				}
			}

			index = 0;
			for (int i = 0; i != 7; ++i) {
				if ((allCards[i] & 0x0f) == v1) {
					selectCards[index++] = allCards[i];
					if (2 == index)
						break;
				}
			}
			
			for (int i = 0; i != 7; ++i) {
				if ((allCards[i] & 0x0f) == v2) {
					selectCards[index++] = allCards[i];
					if (4 == index)
						break;
				}
			}

			char v3 = 0;
			for (unsigned char *xc = dend-1; xc != dcards-1; --xc) {
				if (*xc != v1 && *xc != v2) {
					v3 = *xc;
					break;
				}
			}

			for (int i = 0; i != 7; ++i) {
				if ((allCards[i] & 0x0f) == v3) {
					selectCards[4] = allCards[i];
					break;
				}
			}	
		}
		return TXPK_CARDS_TYPE_ERDUI;
	}
	
	if (1 == duizi) {
		if (selectCards) { // copy cards 1 to A(0x0e)
			unsigned char dcards[7] = {0};

			for (int i = 0; i != 7; ++i) {
				dcards[i] = allCards[i] & 0x0f;
			}
			std::sort(dcards, dcards+7);
			int dIndex = 0;
			for (dIndex = 0; dIndex != 6; ++dIndex) {
				if (dcards[dIndex] && dcards[dIndex] == dcards[dIndex+1])
					break;
			}
			int sIndex = 0;
			for (int i = 0; i != 7; ++i) {
				if ((allCards[i] & 0x0f) == dcards[dIndex]) {
					selectCards[sIndex++] = allCards[i];
					if (2 == sIndex)
						break;
				}
			}

			for (unsigned char *ctmp = dcards+6; ctmp != dcards-1; --ctmp) {
				if (*ctmp != dcards[dIndex]) {
					for (int i = 0; i != 7; ++i) {
						if ((allCards[i] & 0x0f) == *ctmp) {
							selectCards[sIndex++] = allCards[i];
							break;
						}
					}
				}
				if (sIndex >= 5)
					break;
			}
		}
		return TXPK_CARDS_TYPE_DUIZI;
	}
	
	if (selectCards) {
		unsigned char dcards[7] = {0};
		for (int i = 0; i != 7; ++i) {
			dcards[i] = allCards[i] & 0x0f;
		}
		std::sort(dcards, dcards+7);
		int index = 0;
		for (int i = 6; i != 1; --i) {
			for (int j = 0; j != 7; ++j) {
				if (dcards[i] == (allCards[j] & 0x0f)) {
					selectCards[index++] = allCards[j];
					break;
				}
			}
		}
	}
	return TXPK_CARDS_TYPE_SANPAI;
}

int TexasCardHelp::TxPokerCompareCards(const unsigned char hcards1[2], const unsigned char hcards2[2], const unsigned char tcards[5])
{
	unsigned char selectCards1[5] = {0}, selectCards2[5] = {0};
	int htype1 = TxPokergetCardsType(tcards, hcards1, selectCards1);
	int htype2 = TxPokergetCardsType(tcards, hcards2, selectCards2);

	if (htype1 != htype2)
		return htype1 - htype2;

	// htype1 == htype2
	if (TXPK_CARDS_TYPE_BIGTONGHUASHUN == htype1 && TXPK_CARDS_TYPE_BIGTONGHUASHUN == htype2)
		return 0;
	unsigned char  comp_card1 = 0, comp_card2 = 0;
	
	// compare value
	if (TXPK_CARDS_TYPE_TONGHUASHUN == htype1 && TXPK_CARDS_TYPE_TONGHUASHUN == htype2) {
		return ((selectCards1[0] & 0x0f) - (selectCards2[0] & 0x0f));
	}

	if (TXPK_CARDS_TYPE_TIEZHI == htype1 && TXPK_CARDS_TYPE_TIEZHI == htype2) {
		if ((selectCards1[0] & 0x0f) != (selectCards2[0] & 0x0f))
			return (selectCards1[0] & 0x0f) - (selectCards2[0] & 0x0f);
		else {
			//__log(_ERROR, "TxPokerCompareCards", "TIEZHI two 4 cards same");
			return (selectCards1[4] & 0x0f) - (selectCards2[4] & 0x0f);
		}
	}

	if (TXPK_CARDS_TYPE_HULU == htype1 && TXPK_CARDS_TYPE_HULU == htype2) {
		if ((selectCards1[0] & 0x0f) != (selectCards2[0] & 0x0f))
			return (selectCards1[0] & 0x0f) - (selectCards2[0] & 0x0f);
		else {
			//__log(_ERROR, "TxPokerCompareCards", "HULU two 3 cards same");
			return (selectCards1[3] & 0x0f) - (selectCards2[3] & 0x0f);
		}
	}

	if (TXPK_CARDS_TYPE_TONGHUA == htype1 && TXPK_CARDS_TYPE_TONGHUA == htype2) {
		for (int i = 0; i != 5; ++i) {
			if ((selectCards1[i] & 0x0f) != (selectCards2[i] & 0x0f))
				return (selectCards1[i] & 0x0f) - (selectCards2[i] & 0x0f);
		}
		return 0;
	}

	if (TXPK_CARDS_TYPE_SHUNZI == htype1 && TXPK_CARDS_TYPE_SHUNZI == htype2) {
		// 1 2 3 4 5 or 10 j Q k A    two type
		for (int i = 4; i != -1; --i) {
			if ((selectCards1[i] & 0x0f) != (selectCards2[i] & 0x0f))
				return (selectCards1[i] & 0x0f) - (selectCards2[i] & 0x0f);
		}
		return 0;
	}

	if (TXPK_CARDS_TYPE_SANTIAO == htype1 && TXPK_CARDS_TYPE_SANTIAO == htype2) {
		if ((selectCards1[0] & 0x0f) != (selectCards2[0] & 0x0f))
			return (selectCards1[0] & 0x0f) - (selectCards2[0] & 0x0f);
		else {
			//__log(_ERROR, "TxPokerCompareCards", "SANTIAO two 3 cards same");
			if ((selectCards1[3] & 0x0f) != (selectCards2[3] & 0x0f))
				return (selectCards1[3] & 0x0f) - (selectCards2[3] & 0x0f);
			return (selectCards1[4] & 0x0f) - (selectCards2[4] & 0x0f);
		}
	}

	if (TXPK_CARDS_TYPE_ERDUI == htype1 && TXPK_CARDS_TYPE_ERDUI == htype2) {
		if ((selectCards1[0] & 0x0f) != (selectCards2[0] & 0x0f))
			return (selectCards1[0] & 0x0f) - (selectCards2[0] & 0x0f);
		if ((selectCards1[2] & 0x0f) != (selectCards2[2] & 0x0f))
			return (selectCards1[2] & 0x0f) - (selectCards2[2] & 0x0f);
		return (selectCards1[4] & 0x0f) - (selectCards2[4] & 0x0f);
	}

	if (TXPK_CARDS_TYPE_DUIZI == htype1 && TXPK_CARDS_TYPE_DUIZI == htype2) {
		if ((selectCards1[0] & 0x0f) != (selectCards2[0] & 0x0f))
			return (selectCards1[0] & 0x0f) - (selectCards2[0] & 0x0f);
		for (int i = 2; i != 5; ++i)
			if ((selectCards1[i] & 0x0f) != (selectCards2[i] & 0x0f))
				return (selectCards1[i] & 0x0f) - (selectCards2[i] & 0x0f);
		return 0;
	}

	if (TXPK_CARDS_TYPE_SANPAI == htype1 && TXPK_CARDS_TYPE_SANPAI == htype2) {
		for (int i = 0; i != 5; ++i) {
			if ((selectCards1[i] & 0x0f) != (selectCards2[i] & 0x0f))
				return (selectCards1[i] & 0x0f) - (selectCards2[i] & 0x0f);
		}
		return 0;
	}
	return 0;
}


