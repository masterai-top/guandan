#include "GuanDanClientData.h"
#include "Knickknacks.h"
#include "GuanDanAssist.h"

void GuanDanGamePlayerData::CreateGuidePlayerGameData ()
{
	Init ();

	m_iTableNum = 0;
	m_iTableNumExtra = 0;
	m_iPlayerNum = 2;
	m_nBaseScore = 20;
	m_bCurrentBanker = 0;
	m_bSmallBetBinder = 1;
	m_bBigBetBinder = 2;
	m_bCurrentUser = 0;
	m_currentGameStatus = GameStatus_River;
	m_bBetVec.push_back (1000);

	g_pkGuanDanGamePlayerData->m_nRiverCard[0] = 0x0D;
	g_pkGuanDanGamePlayerData->m_nRiverCard[1] = 0x1D;
	g_pkGuanDanGamePlayerData->m_nRiverCard[2] = 0x2D;
	g_pkGuanDanGamePlayerData->m_nRiverCard[3] = 0x21;
	g_pkGuanDanGamePlayerData->m_nRiverCard[4] = 0x08;

	ODEInt64 owners = 0;

	int iTablePos = 0;

	for (int i = 0; i < m_iPlayerNum; i++)
	{
		
		m_PlayerNode[i].cIfReady = 1;
		m_PlayerNode[i].iMoney = 1000;
		
		
		m_oCardUser[i].Reset ();
		m_oCardUser[i].m_nGameScore = m_PlayerNode[i].iMoney;
		m_oCardUser[i].m_nUserScore = m_PlayerNode[i].iMoney;

		m_bHasUser[i] = true;

		if (i == 0)
		{
			strcpy (m_PlayerNode[i].szNickName, ODETools::UTF8ToGBK (g_pkClientData->GetNickname ()).c_str ());

			m_PlayerNode[i].cSexType = g_pkClientData->GetUserGender ();
			m_PlayerNode[i].iheadindex = g_pkClientData->GetUserHeadIndex ();

			for (int j = 0; j < GUANDAN_CARD_HAND_NUM; j++)
			{
				if (j == 0)
					m_oCardUser[i].m_bHandCardData[j] = 0x31;
				else
					m_oCardUser[i].m_bHandCardData[j] = 0x18;
			}
		}
		else
		{
			strcpy (m_PlayerNode[i].szNickName, "Ransoul");

			m_PlayerNode[i].cSexType = g_pkClientData->GetUserGender ();
			m_PlayerNode[i].iheadindex = g_pkClientData->GetUserHeadIndex () + i;

			for (int j = 0; j < GUANDAN_CARD_HAND_NUM; j++)
			{
				if (j == 0)
					m_oCardUser[i].m_bHandCardData[j] = 0x07;
				else
					m_oCardUser[i].m_bHandCardData[j] = 0x17;
			}
		}

		owners |= 1 << i;

		iTablePos = GuanDanGameTools::SwitchViewChairID(i);

		m_pTablePlayer[iTablePos] = &m_PlayerNode[i];
	}

	m_bBetOwner.push_back (owners);
}


void GuanDanGamePlayerData::CreateGuidePlayerGameData2 ()
{
	Init ();

	m_iTableNum = 0;
	m_iTableNumExtra = 0;
	m_iPlayerNum = 3;
	m_nBaseScore = 20;
	m_bCurrentBanker = 0;
	m_bSmallBetBinder = 1;
	m_bBigBetBinder = 2;
	m_bCurrentUser = 0;
	m_currentGameStatus = GameStatus_Flop;
	m_bBetVec.push_back (0);

	m_GameIsEnding = false;

	ODEInt64 owners = 0;

	int iTablePos = 0;

	for (int i = 0; i < m_iPlayerNum; i++)
	{
		m_PlayerNode[i].cIfReady = 1;
		m_PlayerNode[i].iMoney = 100;
		
		m_oCardUser[i].Reset ();

		m_bHasUser[i] = true;

		if (i == 0)
		{
			strcpy (m_PlayerNode[i].szNickName, ODETools::UTF8ToGBK (g_pkClientData->GetNickname ()).c_str ());

			m_PlayerNode[i].cSexType = g_pkClientData->GetUserGender ();
			m_PlayerNode[i].iheadindex = g_pkClientData->GetUserHeadIndex ();

			for (int j = 0; j < GUANDAN_CARD_HAND_NUM; j++)
			{
				if (j == 0)
					m_oCardUser[i].m_bHandCardData[j] = 0x31;
				else
					m_oCardUser[i].m_bHandCardData[j] = 0x18;
			}

			m_oCardUser[i].m_playerStatus = PlayerStatus_Opting;

			m_nTurnAddCounter[i] = 0;

			m_PlayerNode[i].iMoney = 100 - m_nTurnAddCounter[i];
		}
		else if (i == 1)
		{
			strcpy (m_PlayerNode[i].szNickName, "Ransoul");

			m_PlayerNode[i].cSexType = 0;
			m_PlayerNode[i].iheadindex = 103;

			for (int j = 0; j < GUANDAN_CARD_HAND_NUM; j++)
			{
				if (j == 0)
					m_oCardUser[i].m_bHandCardData[j] = 0x07;
				else
					m_oCardUser[i].m_bHandCardData[j] = 0x17;
			}

			m_nTurnAddCounter[i] = 20;

			m_PlayerNode[i].iMoney = 100 - m_nTurnAddCounter[i];

			m_oCardUser[i].m_playerStatus = PlayerStatus_SmallBeter;
		}
		else if (i == 2)
		{
			strcpy (m_PlayerNode[i].szNickName, "Norda");

			m_PlayerNode[i].cSexType = 1;
			m_PlayerNode[i].iheadindex = 109;

			for (int j = 0; j < GUANDAN_CARD_HAND_NUM; j++)
			{
				if (j == 0)
					m_oCardUser[i].m_bHandCardData[j] = 0x1A;
				else
					m_oCardUser[i].m_bHandCardData[j] = 0x23;
			}

			m_nTurnAddCounter[i] = 40;
			m_PlayerNode[i].iMoney = 100 - m_nTurnAddCounter[i];
			m_oCardUser[i].m_playerStatus = PlayerStatus_BigBeter;
		}

		m_oCardUser[i].m_nGameScore = m_PlayerNode[i].iMoney;
		m_oCardUser[i].m_nUserScore = m_PlayerNode[i].iMoney;
		

		iTablePos = GuanDanGameTools::SwitchViewChairID(i);
		m_pTablePlayer[iTablePos] = &m_PlayerNode[i];

		owners |= 1 << i;
	}

	m_bBetOwner.push_back (owners);
}

void GuanDanGamePlayerData::addWinUser (unsigned char winUser)
{
	m_WinnerUser.push_back (winUser);
}

void GuanDanGamePlayerData::addBetValues (ODEInt64 value)
{
	m_bBetVec.push_back (value);
}

void GuanDanGamePlayerData::addBetOwner (ODEInt64 owner)
{
	m_bBetOwner.push_back (owner);
}

void GuanDanGamePlayerData::setUserAddCounter (unsigned char user, ODEInt64 counter)
{
	m_nTurnAddCounter[user] += m_nTurnAddCounter[user] + counter;
}

void GuanDanGamePlayerData::updateUserAddCounterAll ()
{
	for (int i = 0; i < GUANDAN_TABLE_PLAYER_NUM; i++)
	{
		m_nTurnAddCounterAll[i] += m_nTurnAddCounter[i];
		m_nTurnAddCounter[i] = 0;
	}
}

bool GuanDanGamePlayerData::isPlayerIsPass (unsigned char userIndex)
{
	if (m_oCardUser[userIndex].m_playerStatus  != PlayerStatus_GiveUp || 
		m_oCardUser[userIndex].m_playerStatus != PlayerStatus_UnKnown || 
		m_oCardUser[userIndex].m_playerStatus != PlayerStatus_Wait ||
		m_oCardUser[userIndex].m_playerStatus != PlayerStatus_Visiter)
	{
		return false;
	}
	else
	{
		return true;
	}
}

ODEInt64 GuanDanGamePlayerData::GetPlayerWinScoreEnd (unsigned char userorder [GUANDAN_TABLE_PLAYER_NUM], int userIndex, int chipsIndex)
{
	if (chipsIndex > m_bBetOwner.size () - 1)
		return 0;

	ODEInt64  winScore = 0;
	int cardOrder = userorder[userIndex];

	std::vector <int> m_biggerUserVec;
	std::vector <int> m_equalUserVec;

	if (cardOrder == GUANDAN_INVALID_USER_CHAIR_INDEX)
		return  0;
	else
	{
		m_biggerUserVec.clear ();
		m_equalUserVec.clear ();

		for (int i = 0 ; i < GUANDAN_TABLE_PLAYER_NUM; i++)
		{
			if (i == userIndex)
				continue;

			if (userorder[i] > cardOrder && userorder[i] != GUANDAN_INVALID_USER_CHAIR_INDEX)
				m_biggerUserVec.push_back (i);

			if (userorder[i] == cardOrder)
				m_equalUserVec.push_back (i);
		}
	}

	for (int i = 0; i < m_bBetOwner.size (); i++)
	{
		if (i != chipsIndex)
			continue;

		bool hasBiggerOwnerWin = false;
		int equalWinnerOwner = 0;
		int addCounterWinner = 0;

		for (int j = 0; j < m_biggerUserVec.size (); j ++)
		{
			if ((m_bBetOwner[i] & (1 << m_biggerUserVec[j])) > 0)
			{
				hasBiggerOwnerWin = true;
			}
		}

		if (hasBiggerOwnerWin)
		{
			return 0;
		}
		else
		{
			if ((m_bBetOwner[i] & (1 << userIndex)) > 0)
			{
				if (0 == m_equalUserVec.size())
					winScore += m_bBetVec[i];
				else 
					winScore += m_bBetVec[i]/(m_equalUserVec.size()+1);
			}
		}
	}
	return winScore;
}

GuanDanGameActiveData::GuanDanGameActiveData ()
{
	init ();
}

GuanDanGameActiveData::~GuanDanGameActiveData ()
{
}

void GuanDanGameActiveData::init ()
{
	clearPlayerPayTipsPerDay ();
	clearPlayerPayTipsAll ();
	clearMedalsProgress ();
	clearLuckyTypeCardProgress ();
}

void GuanDanGameActiveData::addPlayerPayTipsItem (GuanDanPaytipRankingItem item, bool isPerDay)
{
	if (isPerDay)
		m_PayTipsRankingPerDay.push_back (item);
	else
		m_PayTipsRankingAll.push_back (item);
}


void GuanDanGameActiveData::updateLuckTypeCardProgress ()
{
	for (int i = 0; i < m_LuckyTypeCardProgress.size (); i++)
	{
		if (m_LuckyTypeCardProgress[i].nStatus == 1)
		{
			m_LuckyTypeCardProgress[i].nStatus = 2;
		}
	}
}

void GuanDanGameActiveData::updateMedalsProgress ()
{
	for (int i = 0; i < m_MedalsProgress.size (); i++)
	{
		if (m_MedalsProgress[i].nStatus == 1)
		{
			m_MedalsProgress[i].nStatus = 2;
		}
	}
}

void GuanDanGameActiveData::setMedalsStatus (int medalsId, int status)
{
	for (int i = 0; i < m_MedalsProgress.size (); i++)
	{
		if (status == 3)
		{
			if (m_MedalsProgress[i].nStatus == 3)
				m_MedalsProgress[i].nStatus = 2;
		}

		if (m_MedalsProgress[i].nMedalID == medalsId)
		{
			m_MedalsProgress[i].nStatus = status;
		}
	}
}

void GuanDanGameActiveData::addLuckyTypeCardProgress (GuanDanCardtypeProgressItem item)
{
	bool hasItemAlready = false;

	for (int i = 0; i < m_LuckyTypeCardProgress.size (); i++)
	{
		if (strcmp ( m_LuckyTypeCardProgress[i].szTitle.c_str (), item.szTitle.c_str ()) == 0)
		{
			m_LuckyTypeCardProgress[i].nStatus = item.nStatus;
			m_LuckyTypeCardProgress[i].szDes = item.szDes;
			hasItemAlready = true;
			return ;
		}
	}

	if (!hasItemAlready)
		m_LuckyTypeCardProgress.push_back (item);
}

void GuanDanGameActiveData::addMedalsProgress (GuanDanMedalProgressItem item)
{
	bool hasItemAlready = false;

	for (int i = 0; i < m_MedalsProgress.size (); i++)
	{
		if (m_MedalsProgress[i].nMedalID == item.nMedalID)
		{
			m_MedalsProgress[i].nStatus = item.nStatus;
			m_MedalsProgress[i].doneProgress = item.doneProgress;
			m_MedalsProgress[i].allProgress = item.allProgress;
			m_MedalsProgress[i].nActivityID = item.nActivityID;
			m_MedalsProgress[i].szDes = item.szDes;
			m_MedalsProgress[i].szTitle = item.szTitle;
			hasItemAlready = true;
			return ;
		}
	}

	if (!hasItemAlready)
		m_MedalsProgress.push_back (item);
}

const char * GuanDanGameActiveData::GetMedalsTitle (int medalsID)
{
	for (int i = 0; i < m_MedalsProgress.size (); i++)
	{
		if (m_MedalsProgress[i].nMedalID == medalsID)
		{
			return m_MedalsProgress[i].szTitle.c_str ();
		}
	}
	return NULL;
}

void GuanDanGameActiveData::clearPlayerPayTipsPerDay ()
{
	m_PayTipsRankingPerDay.clear ();
}

void GuanDanGameActiveData::clearPlayerPayTipsAll ()
{
	m_PayTipsRankingAll.clear ();
}

void GuanDanGameActiveData::clearLuckyTypeCardProgress ()
{
	m_LuckyTypeCardProgress.clear ();
}

void GuanDanGameActiveData::clearMedalsProgress ()
{
	m_MedalsProgress.clear ();
}

std::vector<GuanDanPaytipRankingItem> * GuanDanGameActiveData::GetPayTipsRanking (bool isPerDay)
{
	if (isPerDay)
		return &m_PayTipsRankingPerDay;
	else
		return &m_PayTipsRankingAll;
}

std::vector<GuanDanCardtypeProgressItem> *  GuanDanGameActiveData::GetLuckyTypeCardProgress ()
{
	return &m_LuckyTypeCardProgress;
}

std::vector<GuanDanMedalProgressItem> * GuanDanGameActiveData::GetMedalsProgress ()
{
	return &m_MedalsProgress;
}

bool GuanDanGameActiveData::isHasMedalsNotGetAward ()
{
	for (int i = 0; i < m_MedalsProgress.size (); i++)
	{
		if (m_MedalsProgress[i].nStatus == 1)
		{
			return true;
		}
	}
	return false;
}

bool GuanDanGameActiveData::isHasLuckyTypeNotGetAward ()
{
	for (int i = 0; i < m_LuckyTypeCardProgress.size (); i++)
	{
		if (m_LuckyTypeCardProgress[i].nStatus == 1)
		{
			return true;
		}
	}
	return false;
}