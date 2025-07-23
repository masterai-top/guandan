#pragma once 

#include "GameCommon.h"
#include "Knickknacks.h"
#include "GameJNIPlatform.h"
#include "LobbyAssistScene.h"

#define GUANDAN_SELECT_CHOOSEBTN_TAG 500
#define GUANDAN_SELECT_CHOOSEBTN_TEXT 600

#define GUANDAN_PATTIPS_CHOOSEBTN_TAG 1127
#define GUANDAN_PATTIPS_CHOOSEBTN_BG_TAG 1137
#define GUANDAN_PATTIPS_CHOOSEBTN_TEXT 1147

class GuanDanActiveChooseBtn : public UIChooseButton
{
public:
	GuanDanActiveChooseBtn(cocos2d::CCNode* pkNormal, cocos2d::CCNode* pkPressed, cocos2d::CCNode* ChooseSprite, cocos2d::CCNode * normalSprite, CCNode * chooseIcon);
	virtual ODUIWidget* ProcessTouch(ODUILayer::TouchType eType, cocos2d::CCTouch* pkTouch);
	virtual void EventRelease() {}
	void SetChoose(bool bChoose);
	bool GetChooseStats () {return m_isChoose;}
	void SetChooseIndex (int index);
	int GetChooseIndex () {return m_index;}

	void SetActiveShowTips (bool ShowTips, char * tipsPath);

	void setclickStatus (State stats);

private:
	virtual void UpdateAnimation ();
	CCSprite * m_ChooseBtnIcon;
	CCSprite * m_ChooseSprite;
	CCSprite * m_NormalSprite;
	bool m_isChoose;
	int m_index;
};


enum GuanDanActiveType
{
	GuanDanActiveType_PayTips,
	GuanDanActiveType_Achievement,
	GuanDanActiveType_LuckyType,
	GuanDanActiveType_CrazyGuanDan,
	GuanDanActiveType_Max,
};

class GuanDanActive : public ODUILayerModalExt<GuanDanActive>
{
public:

	GuanDanActive ();

	virtual ~GuanDanActive ();

	virtual bool init ();

	virtual void term ();

	virtual void OnActive(const ODUIUserDataPtr& spData);

	virtual void OnDeactive(const ODUIUserDataPtr& spData);

	virtual bool OnBack();

	virtual void OnMenu();

	virtual void onExit();

	void removeTextureManual ();

	void createGuanDanActiveList ();

	void createScene ();

	void RefreshChooseBtnState(int select_index);
	void initTableViewStatus (int select_index);
	void HidePayTipsViews (bool visible);
	void RefreshPayTipsListItems (int selectIndex);

	void HideMedalsViews (bool visible);

	void HideCrazyGuanDanViews (bool visible);

	void HideGuanDanActiveViews (bool visible);
	void RefreshGuanDanActiveListItems ();
	void CreateGuanDanActiveListItems ();

	void createGuanDanActiveContent ();
	void RefreshLuckyTypeItems ();

	void UpdateActiveTips (ODEFloat32 u32Delta);

	void UpdatePayTipsRank (ODEFloat32 u32Delta);

	ODE_CREATE_FUNC(GuanDanActive);

	typedef ODEMemberDelegate<GuanDanActive> FadeDelegate;

protected:

	UIDeclareDelegate(OnChangeView);
	UIDeclareDelegate(OnChangePayTipsRank);
	UIDeclareDelegate(OnPayTips);
	UIDeclareDelegate(OnCrazyGuanDan);
	UIDeclareDelegate(OnGetLuckTypeAward);
	UIDeclareDelegate(OnGetMedalsAward);

	ODUITableView * pkChildTableView;
	SceneLobbyTopBar * m_topButtonBar;


#if IS_SINGLE_CHANNEL_PAK
	SceneBottomBar * m_bottomBtns;
#else
	SceneBottomBarForLobbyStart * m_bottomBtns;
#endif

	void OnFadeIn();
	FadeDelegate m_kOnFadeIn;

	void OnFadeOut();
	FadeDelegate m_kOnFadeOut;

	bool m_bCreateScene ;

	GuanDanActiveChooseBtn * m_chooseButtonArray[4];

	GuanDanActiveType m_currentGuanDanActiveType;

	int m_PayTipsRankIndex;
	
	//打赏荷官活动
	GuanDanPayTipsRankList * m_PayTipsRankList;
	GuanDanPayTipsButton * m_payTipsRankBtn;
	ODUITableView * m_PayTipsTableView;
	CCNode * m_PayTipsTitleNode;
	ODUIButton * m_payTipsButton;
	CCLabelTTF * m_payTipsTip;

	//成就勋章
	GuanDanActiveMedalsList * m_GuanDanActiveList;
	CCSprite * m_guanDanActiveBeauty;
	ODUIButton * m_GetMedalsAwardButton;
	CCLabelTTF * m_MedalsTips;

	//幸运牌型
	CCNode * m_LuckyTypeNode;
	ODUIButton * m_LuckyTypeButton;
	CCLabelTTF * m_LuckyTypeTips;


	//德州也疯狂
	CCNode * m_CrazyGuanDanNode;
	ODUIButton * m_CrazyPokerButton;

private:

};
