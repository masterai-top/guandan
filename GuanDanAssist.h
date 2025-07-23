#pragma once 

#include "ODUILayer.h"
#include "Knickknacks.h"
#include "ClientData.h"
#include "GuanDanDef.h"
#include "C2DXShareSDK.h"
#include "ScrollViewMenu.h"
#include "LobbyKeyControl.h"
#include "LobbyKeyControl.h"

enum GuanDanRoomChooseType
{
	TP_ROOMTYPE_NONE = -1,
	TP_ROOMTYPE_NEW,
	TP_ROOMTYPE_CENTER,
	TP_ROOMTYPE_HIGH,
	TP_ROOMTYPE_MASTER,
	TP_ROOMTYPE_MAX,
};
enum GuanDan_RoomType
{
	gd_normal_room = 0,
	gd_tuantuan_room,
	gd_match_room,
	gd_private_room,
};

typedef enum GD_ROOMState
{
	GD_ROOM_STATE_NULL,
	GD_ROOM_STATE_HALL,
	GD_ROOM_STATE_NORMAL,
	GD_ROOM_STATE_TUANTUAN,
	GD_ROOM_STATE_MATCH,
	GD_ROOM_STATE_PRIVATE,
	GD_ROOM_STATE_MAX
}GD_ROOMStateDef;
//掼蛋经典场房间选择按钮
class GuanDanRoomChooseButton : public ODUIButton
{
public:
	GuanDanRoomChooseButton(cocos2d::CCNode* pkNormal, cocos2d::CCNode* pkPressed, cocos2d::CCNode* pkDisabled = NULL, LobbyKeyControlForSingleLayer * control = NULL, int RoomListSize = 0, int TopButtonSize = 0);
	virtual ~GuanDanRoomChooseButton ();

	GuanDanRoomChooseType GetRoomType () {return m_roomType;}

	void SetRoomChooseType (GuanDanRoomChooseType roomType);

	void Init ();

	void SetRoomInfo (int playerNumber, char * baseScore, const char * roomInfo);

	void SetPlayerNumber (int playerNumber);
	void SetBaseScore (char * baseScore);
	void SetRoomInfo (const char * roomInfo);


	void updatePlayerNumbers (ODEFloat32 f32Delta);

protected:
	virtual void OnEvent(ODUIWidget& kWidget, ODEUInt32 u32State);

private:
	GuanDanRoomChooseType m_roomType;
	CCLabelAtlas * m_PlayerNumsTTF;
	CCLabelAtlas * m_ScoreLimitedTTF;
	CCLabelAtlas * m_RoomInfoTTF;

	int m_playerNumber;
	char m_baseScore [64];
	int	m_RoomListSize;
	int m_TopBtnsSize;

	LobbyKeyControlForSingleLayer * m_ContainerLayerControl;
};


//掼蛋经典场房间选择界面
class GuanDanRoomChooseList : public RoomList
{
public:
	GuanDanRoomChooseList ();

	virtual ~GuanDanRoomChooseList ();

	ODEUInt32 AddRoomBtn (GuanDanRoomChooseButton * buttonAdd,int num); 

	void RemoveAllBtn ();

	void EnableAllRoomBtn (bool enable);

	ODEUInt32 GetListSize ();

	void  ReFreshAllRoomInfo();
};

//掼蛋团团转场房间选择界面
class GD_TuanTuanGameRoomList : public ODUIScrollViewH
{
public:
	typedef ODEMemberDelegate<GD_TuanTuanGameRoomList, ODUIWidget&, ODEUInt32> UIDelegate;

	GD_TuanTuanGameRoomList();

	virtual ~GD_TuanTuanGameRoomList();

	ODEUInt32 AddRoomBtn(int roomTypeIndex, int maxRoomCount, ODEUInt64 u64Limit, ODEUInt64 u64LimitMax, ODEUInt64 u64BaseBet, int peopleCount);

	void ClearAll();

	void RefreshList();

private:
	struct LobbyRoomInfo
	{
		ODEUInt64				m_u64LimitScore;
		ODEUInt64				m_u64BaseScore;
		ODEInt32				m_indexInConfig;
	};

public:
	LobbyRoomInfo GetRoomInfo(ODEUInt32 u32Index);

protected:
	void OnClicked(ODUIWidget& kWidget, ODEUInt32 u32State);

	struct Item
	{
		Item();
		ODUIButton* m_pkButton;
		UIDelegate m_kDelegate;
		LobbyRoomInfo m_LobbyRoomInfo;
	};

	ODEVector<Item> m_kButtonArray;

	CCLabelAtlas* m_limitNumber;
	CCLabelAtlas* m_BaseBetNumber;
	CCLabelAtlas* m_PeopleNumber;
};
//掼蛋比赛场房间选择界面

class GD_MatchRoomBtn : public CCNode
{
public:
	GD_MatchRoomBtn(long long id, string name, int playerCount, int paramA, int paramC, string firstAward, int sortid);
	~GD_MatchRoomBtn();

	void menuCallback(CCObject* pkObj);

	void SetTouchEnable( bool bEnable );

	void SetMatchRoomId(long long newId);

	long long GetMatchRoomId();

	void UpdatePlayerCount(ODEFloat32 dt);

private:
	long long m_roomId;

	CCMenuItemSprite* m_pkTouchItemChairL;

	CCLabelAtlas* m_pMatchId;

	CCLabelTTF* m_pUserCount;

	int m_iParamA;
	int m_iParamC;
	int m_iRealCount;
};

class GD_MatchGameRoomScrollView : public ODUIPickerScrollView
{
public:
	virtual void registerWithTouchDispatcher(){
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	}


	CREATE_FUNC(GD_MatchGameRoomScrollView);
};

class GD_MatchGameRoomList : public ODUIWidget, public ODUIPickerScrollViewDelegate
{
public:
	typedef ODEMemberDelegate<GD_MatchGameRoomList, ODUIWidget&, ODEUInt32> UIDelegate;

	GD_MatchGameRoomList(ODEInt32 iRows = 2, ODEInt32 iCols = 2);

	virtual ~GD_MatchGameRoomList();

	virtual void scrollViewDidScroll( cocos2d::extension::CCScrollView *view );

	virtual void scrollViewDidZoom( cocos2d::extension::CCScrollView *view );

	virtual bool scrollViewInitPage( cocos2d::CCNode *pScroll, cocos2d::CCNode *pPage, int nPage );

	virtual void scrollViewClick( const cocos2d::CCPoint &oOffset, const cocos2d::CCPoint &oPoint , cocos2d::CCNode *pPage, int nPage );

	virtual void scrollViewScrollEnd( cocos2d::CCNode *pPage, int nPage );

	virtual void scrollViewNoChange();

	void SetTouchActive(bool bIsActive);

	void RefreshRoomList();

	void SetAllMatchRoomBtnEnable(bool bEnable);

	void ScrollToPrePage();

	void ScrollToNextPage();

	void ScrollToEndPage();

	ODEInt32 GetPageNum();

	ODEInt32 GetCurrPageIndex();

	void UpdateRoomId(long long oldId, long long newId);

private:
	bool m_bIsScrolling;

	ODUIPickerScrollView* m_ScrollView;
	CCSprite* m_pPageNodeCurr;
	ODEFloat32 m_fFirstPageNodePos;

	ODEInt32 m_iNumRow;
	ODEInt32 m_iNumCol;
	ODEInt32 m_iPageNodeNum;

	ODEFloat32 m_fWidth;
	ODEFloat32 m_fHeight;
};


//掼蛋房间场次选择界面
class GD_RoomList : public ODUIScrollViewH
{
public:
	typedef ODEMemberDelegate<GD_RoomList, ODUIWidget&, ODEUInt32> UIDelegate;

	GD_RoomList();

	virtual ~GD_RoomList();

	ODEUInt32 AddRoomBtn(int roomType, const ODEChar8* pcRoomImageNormal, const ODEChar8* pcRoomImagePressed, int  Tuijian );

	void ClearAll();

	void setGuanDanRoomType(GuanDan_RoomType _type);
	int getGuanDanRoomType();
	GuanDan_RoomType m_RoomType;
protected:
	void OnClicked(ODUIWidget& kWidget, ODEUInt32 u32State);

	struct Item
	{
		Item();
		ODUIButton* m_pkButton;
		UIDelegate m_kDelegate;
	};

	ODEVector<Item> m_kButtonArray;
};

//掼蛋设置滑动条
class GuanDanSettingSlider : public ODUIWidget
{
public:
	GuanDanSettingSlider(const ODEChar8* pcHandle, ODEFloat32 f32Range ,CCSize spbgsize,CCSize splightsize,const ODEChar8* pcBar = NULL, const ODEChar8* pcBarLight = NULL);

	virtual ~GuanDanSettingSlider();

	void SetValue(ODEFloat32 f32Value);

	ODEFloat32 GetValue();

	virtual ODUIWidget* ProcessTouch(ODUILayer::TouchType eType, cocos2d::CCTouch* pkTouch);

	virtual void EventRelease();

	void SetSliderContentHeight(ODEFloat32 f32Rate);

protected:
	virtual void Update();

	void SetValueWithEvent(ODEFloat32 f32Value);

	cocos2d::CCSprite* m_pkHandle;
	CCScale9Sprite * m_pkSliderBar;
	CCScale9Sprite* m_pkSliderLight;

	CCSize m_sliderSize;
	ODEFloat32 m_f32BarWidth;
	const ODEFloat32 m_f32Range;
	ODEFloat32 m_f32Value;
	bool m_bChange;
	bool m_bTarget;
};


//掼蛋声音滑动条
class GuanDanMusicSlider : public GuanDanSettingSlider
{
public:
	GuanDanMusicSlider();
};

class GuanDanEffectSlider : public GuanDanSettingSlider
{
public:
	GuanDanEffectSlider();
};

/*
	TexasPoker 房间列表
*/
class GuanDanGameRoomList : public ODUIScrollView
{
public:
	typedef ODEMemberDelegate<GuanDanGameRoomList, ODUIWidget&, ODEUInt32> UIDelegate;

	GuanDanGameRoomList();

	virtual ~GuanDanGameRoomList();

	ODEUInt32 AddRoomBtn(const ODEChar8* pcRoomName, const ODEChar8* pcServerIP, ODEUInt32 u32ServerPort, ODEUInt64 u64Limit,ODEUInt32 roomid);

	void ClearAll();

	void RefreshList();

	void EnableAllRoomBtn(bool bEnable);

	void UpdatePlayerNum();

private:
	struct LobbyRoomInfo
	{
		ODEChar8				m_szServerIP[128];
		ODEUInt32				m_nServerPort;
		ODEUInt64				m_u64LimitScore;
		ODEUInt64				m_u64BaseScore;
	};
public:
	LobbyRoomInfo GetRoomInfo(ODEUInt32 u32Index);

protected:
	void OnClicked(ODUIWidget& kWidget, ODEUInt32 u32State);

	struct Item
	{
		Item();
		ODUIButton* m_pkButton;
		UIDelegate m_kDelegate;
		LobbyRoomInfo m_LobbyRoomInfo;
	};
	ODEVector<Item> m_kButtonArray;
	CCLabelTTF* pkLimitNumber;
};



//桌子锁
class GuanDanTableSetPW : public ODUILayerModalExt<GuanDanTableSetPW>
{
public:
	GuanDanTableSetPW();

	virtual ~GuanDanTableSetPW();

	virtual bool init();

	virtual void OnActive(const ODUIUserDataPtr& spData);

	virtual void OnDeactive(const ODUIUserDataPtr& spData);

	virtual bool OnBack();

	virtual void term();

	void removeTextureManual ();

	ODE_CREATE_FUNC(GuanDanTableSetPW);

	CCEditBox *getPwEditBox();
	void resetPassword();

protected:
	virtual bool ProcessTouch(TouchType eType, cocos2d::CCTouch* pkTouch);

	UIDeclareDelegate(OnClose);
	UIDeclareDelegate(OnEnter);

private:
	ODUIWidget* m_pkRoot;

	ODUIButton* pkEnterBtn;

	CCEditBox* pkPasswordInputBox;

	ODEStringA m_errorMsg;

};

//有锁桌入桌
class GuanDanPwTableEnter : public ODUILayerModalExt<GuanDanPwTableEnter>
{
public:
	GuanDanPwTableEnter();
	virtual ~GuanDanPwTableEnter();
	virtual bool init();
	virtual void OnActive(const ODUIUserDataPtr& spData);
	virtual void OnDeactive(const ODUIUserDataPtr& spData);
	virtual bool OnBack();
	virtual void term();
	void removeTextureManual ();

	ODE_CREATE_FUNC(GuanDanPwTableEnter);

	CCEditBox *getPwEditBox();
	void resetPassword();

protected:
	virtual bool ProcessTouch(TouchType eType, cocos2d::CCTouch* pkTouch);
	UIDeclareDelegate(OnClose);
	UIDeclareDelegate(OnCancel);
	UIDeclareDelegate(OnEnter);

private:
	ODUIWidget* m_pkRoot;
	ODUIButton* pkEnterBtn;
	ODUIButton* pkCancelBtn;
	CCEditBox* pkPasswordInputBox;
	ODEStringA m_errorMsg;
};

//游戏内充值
class GuanDanGamePay : public ODUILayerModalExt<GuanDanGamePay>
{
public:
	GuanDanGamePay();

	virtual ~GuanDanGamePay();

	virtual bool init();

	virtual void OnActive(const ODUIUserDataPtr& spData);

	virtual bool ProcessTouch(TouchType eType, CCTouch* pkTouch);

	virtual bool OnBack();

	void RemoveTexturesManual();

	ODE_CREATE_FUNC(GuanDanGamePay);

private:

	UIDeclareDelegate(OnClose);
	UIDeclareDelegate(OnConfirm);
	UIDeclareDelegate(OnWXPay);
private:
	ODUICheckBox* pkWeiXinCheckBox;
	ODUIWidget* pkRoot;
	ODUIPanel* pkPanel;
};


/*
	滑动条
*/
class GuanDanSlider : public ODUIWidget
{
public:
	GuanDanSlider(const ODEChar8* pcHandle,const ODEChar8*pcHandleCenterL,const ODEChar8*pcHandleCenterR, ODEFloat32 f32Range ,CCSize spbgsize,CCSize splightsize,const ODEChar8* pcBar = NULL, const ODEChar8* pcBarLight = NULL);

	virtual ~GuanDanSlider();

	void SetValue(ODEFloat32 f32Value);

	ODEFloat32 GetValue();

	virtual ODUIWidget* ProcessTouch(ODUILayer::TouchType eType, cocos2d::CCTouch* pkTouch);

	virtual void EventRelease();

	void SetSliderContentHeight(ODEFloat32 f32Rate);

protected:
	virtual void Update();

	void SetValueWithEvent(ODEFloat32 f32Value);

	cocos2d::CCSprite* m_pkHandle;
	cocos2d::CCSprite* m_pkHandle_center;
	const ODEFloat32 m_f32Range;
	ODEFloat32 m_f32Value;
	CCScale9Sprite* m_pkBarLight;
	ODEFloat32 m_f32BarWidth;
	bool m_bChange;
	bool m_bTarget;
	ODEFloat32 bar_length ;
	ODEFloat32 bar_wight ;
	const ODEChar8 *m_pkHandle_center_l;
	const ODEChar8 *m_pkHandle_center_r;
};


class GuanDanGameMusicSlider : public GuanDanSlider
{
public:
	GuanDanGameMusicSlider();

	virtual void Update();
};

class GuanDanGameEffectSlider : public GuanDanSlider
{
public:
	GuanDanGameEffectSlider();

	virtual void Update();
};

class GuanDanGameLightSlider : public GuanDanSlider
{
public:
	GuanDanGameLightSlider();

	virtual void Update();
};

//游戏内设置
class GuanDanGameOption : public ODUILayerModalExt<GuanDanGameOption>
{
public:
	GuanDanGameOption();

	virtual ~GuanDanGameOption();

	virtual bool init();

	virtual void OnActive(const ODUIUserDataPtr& spData);

	virtual bool OnBack();

	ODE_CREATE_FUNC(GuanDanGameOption);

private:
	virtual bool ProcessTouch(TouchType eType, CCTouch* pkTouch);

	UIDeclareDelegate(OnClose);

	UIDeclareDelegate(OnMusicSliderVelueChange);

	UIDeclareDelegate(OnEffectSliderVelueChange);

	//静音
	UIDeclareDelegate(OnSilenceCheck);
	//特效
	UIDeclareDelegate(OnSpecialEffectCheck);
	//背景音
	UIDeclareDelegate( OnBeijingyinCheck);

	UIDeclareDelegate(OnExitGame);

private:
	ODUIWidget* pkRoot;
	ODUIPanel* pkPanel;

	//静音checkBox
	ODUICheckBox * pkSilenceCheckBox;
	//游戏特效checkBox
	ODUICheckBox* pkSpecialEffetCheckBox;
	//背景音
	ODUICheckBox* pkBeijingyinCheckBox;
	//亮度
	ODUICheckBox* pkLiangduCheckBox;
	//游戏震动
	//ODUICheckBox* pkSharkCheckBox;
	//省电模式
	ODUICheckBox* pkPowerSavingCheckBox;

	ODEFloat32 m_iPreLightValue;
	ODEFloat32 m_iPreMusicValue;
	ODEFloat32 m_iPreEffectValue;

	bool m_bPreSilence;
	bool m_bPreShark;
	bool m_bPreEffect;
	bool m_bPreMusic;
	bool m_bPreLight;
};

class GuanDanGameTools 
{
public:
	static ODEInt32 SwitchViewChairID(ODEInt32 nChair);
	static ODEInt32 GetUserCurLevel(ODEInt32 nExperice);
	static std::string GetFormatNumberString(long long iNumber);
};	

// 充值引导
class GuanDanRechargeGuide : public ODUILayerModalExt<GuanDanRechargeGuide>
{
public:
	GuanDanRechargeGuide();

	virtual void OnActive(const ODUIUserDataPtr& spData);
	virtual void OnDeactive(const ODUIUserDataPtr& spData);

	virtual ~GuanDanRechargeGuide();

	virtual bool init();

	ODE_CREATE_FUNC(GuanDanRechargeGuide);

protected:

	UIDeclareDelegate(OnRecharge);
	UIDeclareDelegate(OnCancel);

	LobbyKeyControlForSingleLayer m_LobbyKeyControlForSingleLayer;
	void LayerControlScheduleUpdate (ODEFloat32 f32Delta);
};


class GoToLobbyPayBitScoreCallback : public ODUIAlert::Callback
{
public :
	GoToLobbyPayBitScoreCallback()
	{
		m_kTitle = TT("GoLobbyPayTitle");
		m_kContent = TT("GoLobbyPayBitScoreContent");
		m_kEnter = TT("GoLobbyPayOk");
		m_kCancel = TT("GoLobbyPayCancel");
	}

	virtual void OnConfirm()
	{
		g_pkDirector->end();
	}

	virtual void OnCancel()
	{
		
	}
};

ODESmartPointer(GoToLobbyPayBitScoreCallback);

class GoToLobbyPayCallback : public ODUIAlert::Callback
{
public :
	GoToLobbyPayCallback()
	{
		m_kTitle = TT("GoLobbyPayTitle");
		m_kContent = TT("GoLobbyPayForcePayContent");
		m_kEnter = TT("GoLobbyPayOk");
		m_kCancel = TT("GoLobbyPayCancel");
	}

	virtual void OnConfirm()
	{
		g_pkDirector->end();
	}

	virtual void OnCancel()
	{

	}
};

ODESmartPointer(GoToLobbyPayCallback);


class GuanDanOPTButton : public ODUIButton
{
public:
	GuanDanOPTButton (CCNode * normal, CCNode * press, CCNode * icon, CCNode * disable = NULL, CCNode * icon2 = NULL);

	void resetIcon (CCNode *  icon);

	void setOPTButtonIcon (CCNode * icon);

private:
	CCNode * m_pkIcon;
	CCNode * m_pkIcon2;
};

class GuanDanChatButton : public ODUIButton
{
public:
	GuanDanChatButton (CCNode * normal, CCNode * press, CCNode * icon, CCNode * icon2, CCNode * disable = NULL);
private:
	CCNode * m_pkIcon;
	CCNode * m_pkIcon2;
};

class GuanDanOPTWaitButton : public ODUIButton
{
public:
	GuanDanOPTWaitButton (CCNode * normal, CCNode * press, CCNode * icon, CCNode * disable = NULL);
	void setCheck (bool isCheck);
	bool getCheck () {return isChecked;}
	void setClick ();
	void setOPTWaitButton (CCNode * icon);
private:
	bool isChecked;
	CCNode * m_pkIcon;
	CCSprite * m_checkIcon;
	CCSprite * m_noCheckIcon;
};

typedef enum __TPTableCellPosition
{
	TP_CellPos_L1 = 0,
	TP_CellPos_L2,
	TP_CellPos_L3,
	TP_CellPos_Center,
	TP_CellPos_R1,
	TP_CellPos_R2,
	TP_CellPos_R3,
}TPTableCellPosition;


class GuanDanGameTableList;
/*
	桌子列表子节点
*/
class GuanDanGameTableCell : public ODUILayerExt<GuanDanGameTableCell>
{
public:
	
	typedef enum _TP_TableHeadImg
	{
		TP_HEAD_IMG_MAN_0,
		TP_HEAD_IMG_MAN_1,
		TP_HEAD_IMG_MAN_2,
		TP_HEAD_IMG_WOMAN_0,
		TP_HEAD_IMG_WOMAN_1,
		TP_HEAD_IMG_WOMAN_2,
		TP_HEAD_IMA_NULL
	}TPtableHeadImg;

	static GuanDanGameTableCell* create();

	GuanDanGameTableCell();

	virtual ~GuanDanGameTableCell();

	void updateVisualRect(CCRect rect);
	void setTableIndex(unsigned short index);
	void setDelegate(GuanDanGameTableList* tableList);

	bool HasPlayerInSeat(ODEInt32 iIndex);
	ODEInt32 GetPlayerCount();

	void updateBase(ODEInt32 point);

	void menuCloseCallback(CCObject* pSender);

	void updateHeadImg(unsigned short usTableNumExtra, int imgIndex);
	ODEInt32 getBasePoint();
	void SetMenuEnable(bool isEnable);
	void SetTableInfo( bool bInGame, bool bSetPassword, int iTableAnte, int iTableUserCount );
	void setHeadImg( unsigned short usTableNumExtra, int id, int index);
private:
	GuanDanGameTableList* gameTableList;
	unsigned short tableIndex;
	ODEInt32 basePoint;
	bool m_bPlayerInSeat[GUANDAN_TABLE_PLAYER_NUM];
	bool   is_setPw;
};



//房间列表
class GuanDanTableScrollView : public CCScrollView
{
public:
	virtual void registerWithTouchDispatcher(){
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
	}
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);

	CREATE_FUNC(GuanDanTableScrollView);
	void updatePosTouchEnd(float t);
private:
	float newX, newY;
};


/*
	TexasPoker 桌子列表
*/
class GuanDanGameTableList : public ODUIWidget
{
public:
	typedef ODEMemberDelegate<GuanDanGameTableList, ODUIWidget&, ODEUInt32> UIDelegate;

	GuanDanGameTableList();

	virtual ~GuanDanGameTableList();

	void selectCallback(unsigned short tableNum, unsigned short usTableNumExtra);

	void SendSitTableMsg(unsigned short tableNum, unsigned short usTableNumExtra);

	void updateTableBasePoint(unsigned short tableNum, ODEInt32 basePoint);

	void updateHeadImg(unsigned short tableNum, unsigned short usTableNumExtra, int id, int index);

	bool HasPlayerInSeat(unsigned short tableNum, unsigned short usTableNumExtra);

	int getCurrentTableCount();
	int getCurrentSeatCount();

	ODEInt32 getBasePointWithTableSeat(unsigned short tableNum, unsigned short seatNum);
	void setViewSize(ODEFloat64 viewH, ODEFloat64 viewW);
	void SetTouchActive( bool bIsActive );

	void initTableList();
	GuanDanGameTableCell *GetGameTable( int iTableID );

	void ResetTableView();
private:

	GuanDanTableScrollView* scrollView;

	CCSize scrollViewSize;

	CCSize scrollViewContentSize;

	ODEInt32 m_iTableNum;
};

//玩家信息栏
class GameBottomBar : public ODUIWidget
{
public:
	typedef ODEMemberDelegate<GameBottomBar, ODUIWidget&, ODEUInt32> UIDelegate;

	GameBottomBar();

	virtual ~GameBottomBar();

	void SetUserInfo(const ODEChar8* pcName, ODEUInt64 u64Score,ODEInt64 gdPopint,const ODEChar8* pChenghao);

	void UpdateUserScore(ODEUInt64 u64Score);

	void UpdateHeadImg();
private:
	CCLabelTTF* m_pkName;
	CCLabelTTF* m_pkScore;
	CCLabelTTF* m_GuanDanPoint;
	CCSprite* m_chengHao;

	int m_headID;
protected:

};

//機器人託管
class GDRobotAgentLable : public ODUILayerModalExt<GDRobotAgentLable>
{
public:
	GDRobotAgentLable();

	virtual ~GDRobotAgentLable();

	virtual bool init();

	virtual void OnActive(const ODUIUserDataPtr& spData);

	virtual void OnDeactive(const ODUIUserDataPtr& spData);

	virtual bool OnBack();

	virtual void term();

	bool GetIsRobotAgent();

	void RobotAnimation();

	ODE_CREATE_FUNC(GDRobotAgentLable);
	virtual bool ProcessTouch(TouchType eType, cocos2d::CCTouch* pkTouch);

private:
	ODUIWidget* m_pkRoot;
	CCScale9Sprite* pkShade;
	bool m_bIsAgent;
	CCSprite* m_playerNodeSprite;
};