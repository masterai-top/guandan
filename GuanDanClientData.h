#pragma once 

#include "ODEEngine.h"
#include "ClientDataDef.h"
#include <string>
#include <map>
#include "lobbyConnectData.h"
#include "msg_GuanDanProto.h"
#include "GuanDanGameParam.h"

using namespace std;
using namespace GuanDan;

#define  KESSEN_All_TABLE_COUNT  75


struct tagPUSTableInfo
{
	int					iTableID;
	int					iTableAnte;
	bool				bIsInGame;
	bool				bIsSetPassword;
	int					iTableUserCount;
	int					iTableUserID[GUANDAN_TABLE_PLAYER_NUM];
	unsigned short		usUserStatus[GUANDAN_TABLE_PLAYER_NUM];
};

struct tagPUSSetTableAnte
{
	int					iTableID;
	int					iTableAnte;
	bool				bSetTablePassword;
	char				szTablePassword[33];
};
/* *****************************决战类麻将桌子数据**********************/



//玩家状态与游戏状态
enum PlayerStatus
{
	PlayerStatus_Visiter = -1,			//旁观者
	PlayerStatus_UnKnown = 0,			//未知状态
	PlayerStatus_Wait,					//等待
	PlayerStatus_Gaming,				//游戏中等待下注状态
	PlayerStatus_SmallBeter,			//小盲注
	PlayerStatus_BigBeter,				//大盲注
	PlayerStatus_Opting,				//下注操作中
	PlayerStatus_Pass,					//看牌
	PlayerStatus_Follow,				//跟注
	PlayerStatus_AddCounter,			//加注
	PlayerStatus_AllIn,					//全下
	PlayerStatus_GiveUp,				//放弃
	PlayerStatus_Winner,				//胜利状态
	PlayerStatus_Max,					//最大状态
};

enum GameStatus
{
	GameStatus_Idle = 0,				//空闲 等待所有玩家准备完成
	GameStatus_CardSending,				//发牌
	GameStatus_Perflop,					//大小盲注第一轮下注 开始下注玩家从大盲注后逆时针开始
	GameStatus_Flop,					//三张河牌第二轮下注状态 开始下注玩家从小盲注开始
	GameStatus_Turn,					//第四张河牌第三轮下注状态 开始下注玩家从小盲注开始
	GameStatus_River,					//第五张河牌第四轮下注状态 开始下注玩家从小盲注开始
	GameStatus_GameEnd,					//游戏结束状态
};

enum GuanDanCardType
{
	GD_CARDS_TYPE_NONE = 0,
	GD_CARDS_TYPE_SANPAI = 1,
	GD_CARDS_TYPE_DUIZI,
	GD_CARDS_TYPE_ERDUI,
	GD_CARDS_TYPE_SANTIAO,
	GD_CARDS_TYPE_SHUNZI,
	GD_CARDS_TYPE_TONGHUA,
	GD_CARDS_TYPE_HULU,
	GD_CARDS_TYPE_TIEZHI,
	GD_CARDS_TYPE_TONGHUASHUN,
	GD_CARDS_TYPE_BIGTONGHUASHUN,
};


//对应逻辑上玩家数据
class GuanDanPlayerCardData
{
public:
	GuanDanPlayerCardData()
	{
		m_nWinScoreTotal = 0;
		m_bTimeOutCount = 0;
		iMedalsUse = -1;
		iMedalsUnLockCount = 0;
		Reset ();
	}

	void Reset()
	{
		ODEZeroMemory (&m_bHandCardData, sizeof(m_bHandCardData));
		ODEZeroMemory (&m_bCompareCardData, sizeof (m_bCompareCardData));
		m_bCurrentAddRound = 0;
		m_bCardType = GD_CARDS_TYPE_NONE;
		m_playerStatus = PlayerStatus_Wait;
		m_nTax = 0;
		m_nWinScore = 0;
		m_nAddCounter = 0;
		m_nWinScoreTotal = 0;
		m_nAddCounterTotal = 0;
		m_nCurrentTurnNeedAddScore = 0;
		m_nCurrentTurnMaxAddScore = 0;
		m_bIsPass = false;
	}

	void setCurrentTurnAddInfo (ODEInt64 needScore, ODEInt64 maxScore)
	{
		m_nCurrentTurnNeedAddScore = needScore;
		m_nCurrentTurnMaxAddScore = maxScore;
	}

	void resetCurrentTurnAddInfo ()
	{
		m_nCurrentTurnNeedAddScore = 0;
		m_nCurrentTurnMaxAddScore = 0;
	}

public:
	ODEChar8	m_bHandCardData[GUANDAN_CARD_HAND_NUM];				//手上扑克
	unsigned char		m_bHandCardCount;										//玩家手上排数
	unsigned char		m_bRiverCardCount;										//河牌数量
	unsigned char		m_bCurrentAddRound;										//当前下注轮次
	unsigned char		m_bCompareCardData[GUANDAN_CARD_RIVER_NUM];			//最后进行比牌的牌
	unsigned char		m_bCardType;											//牌型
	unsigned char		m_bTimeOutCount;										//游戏超时玩家计数

	PlayerStatus m_playerStatus;										//玩家状态

	ODEInt64	m_nUserScore;											//用户身上的积分
	ODEInt64	m_nGameScore;											//游戏积分
	ODEInt64	m_nWinScoreTotal;										//输赢总数
	ODEInt64	m_nWinScore;											//单局输赢
	ODEInt64	m_nAddCounterTotal;										//加注总记录
	ODEInt64	m_nAddCounter;											//当前轮次加注情况
	ODEInt64	m_nTax;													//税收
	bool		m_bIsPass;												//是否弃牌

	ODEInt64	m_nCurrentTurnNeedAddScore;								//当前下注最小额度
	ODEInt64	m_nCurrentTurnMaxAddScore;								//当前最大下注额度

	int							iMedalsUse;				//当前使用的medalsID
	int							iMedalsUnLockCount;		//当前解锁的medals数量
};

class GuanDanGamePlayerData : public ODESingleton<GuanDanGamePlayerData>
{
public:

	GuanDanGamePlayerData ()
	{
		Init ();
	}

	void Init()
	{
		m_iTableNum = 0;
		m_iTableNumExtra = 0;
		m_iPlayerNum = 0;
		m_bLookOnMode = false;
		m_GameIsEnding = false;
		ODEZeroMemory(&m_PlayerBasicInformation,sizeof(LobbyAuthenRes));
		ODEZeroMemory(m_PlayerNode, sizeof(m_PlayerNode));
		ODEZeroMemory(m_pTablePlayer, sizeof(m_pTablePlayer));
		ODEZeroMemory(m_bHasUser, sizeof(m_bHasUser));

		m_nBaseMultiple = 1;
		m_nBaseAnte = 1;	
		m_nMaxAnte = 10000000;

		m_nBaseScore = 0;
		m_bCurrentBanker = GUANDAN_INVALID_USER_CHAIR_INDEX;
		m_bSmallBetBinder = GUANDAN_INVALID_USER_CHAIR_INDEX;
		m_bCurrentUser = GUANDAN_INVALID_USER_CHAIR_INDEX;
		m_currentGameStatus = GameStatus_Idle;
		m_nMaxAddCounter = 0;
		m_nBetMacCounter = 0;
		m_nRiverCardCounter = 0;
		m_WinnerUser.clear ();
		m_bBetVec.clear ();
		m_bBetOwner.clear ();

		ODEZeroMemory(m_nTurnAddCounter, sizeof(m_nTurnAddCounter));
		ODEZeroMemory(m_nTurnAddCounterAll, sizeof(m_nTurnAddCounterAll));
		ODEZeroMemory(m_glamorous, sizeof(m_glamorous));
		//ClearPUSRoomTableInfo ();

		Reset ();
	}

	void Reset ()
	{
		m_GameIsEnding = false;
		m_currentGameStatus = GameStatus_Idle;
		m_nMaxAddCounter = 0;
		m_nBetMacCounter = 0;
		m_nRiverCardCounter = 0;
		m_WinnerUser.clear ();
		m_bBetVec.clear ();
		m_bBetOwner.clear ();
		ODEZeroMemory(m_nTurnAddCounter, sizeof(m_nTurnAddCounter));
		ODEZeroMemory(m_nTurnAddCounterAll, sizeof(m_nTurnAddCounterAll));
		ODEZeroMemory (m_nRiverCard,sizeof (m_nRiverCard));

		for (int i = 0; i < GUANDAN_TABLE_PLAYER_NUM; i ++)
		{
			m_oCardUser[i].Reset ();
		}
	}
	
	void ResetGameData ()
	{
		m_GameIsEnding = false;
		m_currentGameStatus = GameStatus_Idle;
		m_nMaxAddCounter = 0;
		m_nBetMacCounter = 0;
		m_nRiverCardCounter = 0;
		m_WinnerUser.clear ();
		m_bBetVec.clear ();
		m_bBetOwner.clear ();
		ODEZeroMemory(m_nTurnAddCounter, sizeof(m_nTurnAddCounter));
		ODEZeroMemory(m_nTurnAddCounterAll, sizeof(m_nTurnAddCounterAll));
		ODEZeroMemory (m_nRiverCard,sizeof (m_nRiverCard));
		memset (m_winCardShowStatus, false, sizeof (m_winCardShowStatus));
	}

	void ClearPUSRoomTableInfo();
	void CreateGuidePlayerGameData ();
	void CreateGuidePlayerGameData2 ();
	ODEInt32						m_iTableNum;										//桌子号
	ODEInt32						m_iTableNumExtra;									//本玩家对应的绝对座位号
	ODEInt32						m_iPlayerNum;										//玩家数量
	PlayerNodeDef					m_PlayerNode[GUANDAN_TABLE_PLAYER_NUM];			//玩家结点，按座位绝对位置排列
	PlayerNodeDef*					m_pTablePlayer[GUANDAN_TABLE_PLAYER_NUM];		//相对位置排列	
	bool							m_bLookOnMode;										//是否是观看模式，即入座的位置已经有玩家在玩了
	bool							m_bHasUser[GUANDAN_TABLE_PLAYER_NUM];			//是否有玩家，相对位置排列
	GuanDanPlayerCardData		m_oCardUser[GUANDAN_TABLE_PLAYER_NUM];			//所有玩家状态,相对位置排列
	ODEInt64						m_glamorous[GUANDAN_TABLE_PLAYER_NUM];
	ODEInt64						m_nBaseScore;										//底注
	unsigned char					m_bCurrentBanker;									//当前庄家
	unsigned char					m_bSmallBetBinder;									//小盲注玩家
	unsigned char					m_bBigBetBinder;									//大盲注玩家
	unsigned char					m_bCurrentUser;										//当前操作玩家
	GameStatus						m_currentGameStatus;								//当前游戏状态
	ODEInt64						m_nMaxAddCounter;									//最大押注
	ODEInt64						m_nTurnAddCounter[GUANDAN_TABLE_PLAYER_NUM];		//当前轮次下注情况
	ODEInt64						m_nTurnAddCounterAll[GUANDAN_TABLE_PLAYER_NUM];	//所有下注汇总
	ODEInt64						m_nBetMacCounter;									//游戏配置最大押注额度
	unsigned char					m_nRiverCardCounter;								//河牌数量
	unsigned char					m_nRiverCard[GUANDAN_TABLE_PLAYER_NUM];			//河牌牌型
	std::vector <unsigned char>		m_WinnerUser;										//游戏对局玩家牌型大小
	std::vector <ODEInt64>			m_bBetVec;											//主池与边池
	std::vector <ODEInt64>			m_bBetOwner;										//主池与边池归属
	bool							m_winCardShowStatus[GUANDAN_TABLE_PLAYER_NUM];	//是否进行牌型显示内容
	bool							m_GameIsEnding;										//游戏结束标识

public:
	std::vector<tagPUSTableInfo*>	m_vPUSTableInfo;								//桌子信息
	int								m_nBaseMultiple;								//基础倍数
	int								m_nBaseAnte;									//基础底注
	int								m_nMaxAnte;										//最大底注
	LobbyAuthenRes					m_PlayerBasicInformation;

public:

	void setCurrentBanker (unsigned char charIndex) {m_bCurrentBanker = charIndex;}
	void setSamllBetBinder (unsigned char charIndex) {m_bSmallBetBinder = charIndex;}
	void setBigBetBinder (unsigned char charIndex)  {m_bBigBetBinder = charIndex;}
	void setBaseScore (ODEInt64 baseScore) {m_nBaseScore = baseScore;}
	void setCurrentUser (unsigned char charIndex) {m_bCurrentUser = charIndex;}
	void setRiverCard (unsigned char cardIndex, unsigned char cardValue) {m_nRiverCard[cardIndex] = cardValue;}
	void setGlamorous (unsigned char chairIndex, ODEInt64 Value) {m_glamorous[chairIndex] = Value;}
	ODEInt64 setGlamorous (unsigned char chairIndex) {return m_glamorous[chairIndex];}
	void setGameStatus (GameStatus status) {m_currentGameStatus = status;}
	GameStatus getGameStatus () {return m_currentGameStatus;}
	void setMaxAddCounter (ODEInt64 addCounter) {m_nMaxAddCounter = addCounter;}

	void setRiverCardCounter (unsigned char riverCard) 
	{
		m_nRiverCardCounter = riverCard;

		if (m_nRiverCardCounter == 3)
		{
			setGameStatus (GameStatus_Perflop);
		}
		else if (m_nRiverCardCounter == 4)
		{
			setGameStatus (GameStatus_Turn);
		}
		else if (m_nRiverCardCounter == 5)
		{
			setGameStatus (GameStatus_River);
		}
	}

	void addWinUser (unsigned char winUser);
	void addBetValues (ODEInt64 value);
	void addBetOwner (ODEInt64 owner);
	void setUserAddCounter (unsigned char user, ODEInt64 counter);
	void updateUserAddCounterAll ();
	bool isPlayerIsPass (unsigned char userIndex);
	//根据玩家牌型顺序大小来进行最后的积分计算，计算出每个边池中玩家赢的积分
	ODEInt64 GetPlayerWinScoreEnd (unsigned char userorder [GUANDAN_TABLE_PLAYER_NUM], int userIndex, int chipsIndex);
};

class GuanDanGameActiveData : public ODESingleton<GuanDanGameActiveData>
{
public:

	GuanDanGameActiveData ();
	virtual ~GuanDanGameActiveData ();

	void init ();
	void addPlayerPayTipsItem (GuanDanPaytipRankingItem item, bool isPerDay);

	void addLuckyTypeCardProgress (GuanDanCardtypeProgressItem item);

	void updateLuckTypeCardProgress ();
	void updateMedalsProgress ();

	void setMedalsStatus (int medalsId, int status);

	void addMedalsProgress (GuanDanMedalProgressItem item);

	void clearPlayerPayTipsPerDay ();
	void clearPlayerPayTipsAll ();

	void clearLuckyTypeCardProgress ();
	void clearMedalsProgress ();

	bool isHasMedalsNotGetAward ();
	bool isHasLuckyTypeNotGetAward ();

	const char * GetMedalsTitle (int medalsID);

	std::vector<GuanDanPaytipRankingItem> * GetPayTipsRanking (bool isPerDay);
	std::vector<GuanDanCardtypeProgressItem> * GetLuckyTypeCardProgress ();
	std::vector<GuanDanMedalProgressItem> * GetMedalsProgress ();

private:
	std::vector<GuanDanPaytipRankingItem>	m_PayTipsRankingPerDay;
	std::vector<GuanDanPaytipRankingItem>	m_PayTipsRankingAll;
	std::vector<GuanDanCardtypeProgressItem> m_LuckyTypeCardProgress;
	std::vector<GuanDanMedalProgressItem> m_MedalsProgress;
};