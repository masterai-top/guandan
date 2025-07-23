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
/* *****************************��ս���齫��������**********************/



//���״̬����Ϸ״̬
enum PlayerStatus
{
	PlayerStatus_Visiter = -1,			//�Թ���
	PlayerStatus_UnKnown = 0,			//δ֪״̬
	PlayerStatus_Wait,					//�ȴ�
	PlayerStatus_Gaming,				//��Ϸ�еȴ���ע״̬
	PlayerStatus_SmallBeter,			//Сäע
	PlayerStatus_BigBeter,				//��äע
	PlayerStatus_Opting,				//��ע������
	PlayerStatus_Pass,					//����
	PlayerStatus_Follow,				//��ע
	PlayerStatus_AddCounter,			//��ע
	PlayerStatus_AllIn,					//ȫ��
	PlayerStatus_GiveUp,				//����
	PlayerStatus_Winner,				//ʤ��״̬
	PlayerStatus_Max,					//���״̬
};

enum GameStatus
{
	GameStatus_Idle = 0,				//���� �ȴ��������׼�����
	GameStatus_CardSending,				//����
	GameStatus_Perflop,					//��Сäע��һ����ע ��ʼ��ע��ҴӴ�äע����ʱ�뿪ʼ
	GameStatus_Flop,					//���ź��Ƶڶ�����ע״̬ ��ʼ��ע��Ҵ�Сäע��ʼ
	GameStatus_Turn,					//�����ź��Ƶ�������ע״̬ ��ʼ��ע��Ҵ�Сäע��ʼ
	GameStatus_River,					//�����ź��Ƶ�������ע״̬ ��ʼ��ע��Ҵ�Сäע��ʼ
	GameStatus_GameEnd,					//��Ϸ����״̬
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


//��Ӧ�߼����������
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
	ODEChar8	m_bHandCardData[GUANDAN_CARD_HAND_NUM];				//�����˿�
	unsigned char		m_bHandCardCount;										//�����������
	unsigned char		m_bRiverCardCount;										//��������
	unsigned char		m_bCurrentAddRound;										//��ǰ��ע�ִ�
	unsigned char		m_bCompareCardData[GUANDAN_CARD_RIVER_NUM];			//�����б��Ƶ���
	unsigned char		m_bCardType;											//����
	unsigned char		m_bTimeOutCount;										//��Ϸ��ʱ��Ҽ���

	PlayerStatus m_playerStatus;										//���״̬

	ODEInt64	m_nUserScore;											//�û����ϵĻ���
	ODEInt64	m_nGameScore;											//��Ϸ����
	ODEInt64	m_nWinScoreTotal;										//��Ӯ����
	ODEInt64	m_nWinScore;											//������Ӯ
	ODEInt64	m_nAddCounterTotal;										//��ע�ܼ�¼
	ODEInt64	m_nAddCounter;											//��ǰ�ִμ�ע���
	ODEInt64	m_nTax;													//˰��
	bool		m_bIsPass;												//�Ƿ�����

	ODEInt64	m_nCurrentTurnNeedAddScore;								//��ǰ��ע��С���
	ODEInt64	m_nCurrentTurnMaxAddScore;								//��ǰ�����ע���

	int							iMedalsUse;				//��ǰʹ�õ�medalsID
	int							iMedalsUnLockCount;		//��ǰ������medals����
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
	ODEInt32						m_iTableNum;										//���Ӻ�
	ODEInt32						m_iTableNumExtra;									//����Ҷ�Ӧ�ľ�����λ��
	ODEInt32						m_iPlayerNum;										//�������
	PlayerNodeDef					m_PlayerNode[GUANDAN_TABLE_PLAYER_NUM];			//��ҽ�㣬����λ����λ������
	PlayerNodeDef*					m_pTablePlayer[GUANDAN_TABLE_PLAYER_NUM];		//���λ������	
	bool							m_bLookOnMode;										//�Ƿ��ǹۿ�ģʽ����������λ���Ѿ������������
	bool							m_bHasUser[GUANDAN_TABLE_PLAYER_NUM];			//�Ƿ�����ң����λ������
	GuanDanPlayerCardData		m_oCardUser[GUANDAN_TABLE_PLAYER_NUM];			//�������״̬,���λ������
	ODEInt64						m_glamorous[GUANDAN_TABLE_PLAYER_NUM];
	ODEInt64						m_nBaseScore;										//��ע
	unsigned char					m_bCurrentBanker;									//��ǰׯ��
	unsigned char					m_bSmallBetBinder;									//Сäע���
	unsigned char					m_bBigBetBinder;									//��äע���
	unsigned char					m_bCurrentUser;										//��ǰ�������
	GameStatus						m_currentGameStatus;								//��ǰ��Ϸ״̬
	ODEInt64						m_nMaxAddCounter;									//���Ѻע
	ODEInt64						m_nTurnAddCounter[GUANDAN_TABLE_PLAYER_NUM];		//��ǰ�ִ���ע���
	ODEInt64						m_nTurnAddCounterAll[GUANDAN_TABLE_PLAYER_NUM];	//������ע����
	ODEInt64						m_nBetMacCounter;									//��Ϸ�������Ѻע���
	unsigned char					m_nRiverCardCounter;								//��������
	unsigned char					m_nRiverCard[GUANDAN_TABLE_PLAYER_NUM];			//��������
	std::vector <unsigned char>		m_WinnerUser;										//��Ϸ�Ծ�������ʹ�С
	std::vector <ODEInt64>			m_bBetVec;											//������߳�
	std::vector <ODEInt64>			m_bBetOwner;										//������߳ع���
	bool							m_winCardShowStatus[GUANDAN_TABLE_PLAYER_NUM];	//�Ƿ����������ʾ����
	bool							m_GameIsEnding;										//��Ϸ������ʶ

public:
	std::vector<tagPUSTableInfo*>	m_vPUSTableInfo;								//������Ϣ
	int								m_nBaseMultiple;								//��������
	int								m_nBaseAnte;									//������ע
	int								m_nMaxAnte;										//����ע
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
	//�����������˳���С���������Ļ��ּ��㣬�����ÿ���߳������Ӯ�Ļ���
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