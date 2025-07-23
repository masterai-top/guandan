#include "GuanDanAgreeBasePointAlert.h"
#include "GuanDanTableLayer.h"

GuanDanAgreeBasePointClock::GuanDanAgreeBasePointClock( ODEUInt32 u32ClockTime ) : GuanDanGameClock(u32ClockTime)
{
}

GuanDanAgreeBasePointClock::~GuanDanAgreeBasePointClock()
{
}

void GuanDanAgreeBasePointClock::OnTimerEnd()
{
	GuanDanAgreeBasePointAlert* pLayer = (GuanDanAgreeBasePointAlert*)g_pLayerManager->GetLayer("AgreeBasePointAlert");
	pLayer->ClockTimerEnd();
}


GuanDanAgreeBasePointAlert::GuanDanAgreeBasePointAlert() : ODUILayerModalExt("AgreeBasePointAlert"),  MyAgreeBasePointClock(NULL),m_bBaseAlertConfirm(false),m_iTableId(0),m_iBasePoint(0)
{
	UIInitDelegate(GuanDanAgreeBasePointAlert, OnClose);
	UIInitDelegate(GuanDanAgreeBasePointAlert, OnConfirm);
	UIInitDelegate(GuanDanAgreeBasePointAlert, OnCancel);
}

GuanDanAgreeBasePointAlert::~GuanDanAgreeBasePointAlert()
{
	ODE_SAFE_DELETE(MyAgreeBasePointClock);
	term();
}

bool GuanDanAgreeBasePointAlert::init()
{
	setScale(ui_main_scale);

	CCScale9Sprite * Panel = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-01.png");
	Panel->setPreferredSize(CCSize(720, 320));
	Panel->setPosition(ui_ccp(0.5, 0.5));
	Panel->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(Panel);

	ODEFloat32 panel_width = Panel->getContentSize().width;
	ODEFloat32 panel_height = Panel->getContentSize().height;

	CCSprite* pTitle= CCSprite::createWithSpriteFrameName("kessen_base_ensure.png");
	pTitle->setPosition(ccp(panel_width/2,panel_height - 32));
	pTitle->setAnchorPoint(ccp(0.5f, 0.5f));
	Panel->addChild(pTitle);

	CCScale9Sprite* pCenterBg = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pCenterBg->setAnchorPoint(ccp(0.5, 1));
	pCenterBg->setPreferredSize(CCSize(710, 153));
	pCenterBg->setPosition(ccp(panel_width / 2, panel_height - 54));
	Panel->addChild(pCenterBg);

	CCScale9Sprite* pCenterBg2 = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pCenterBg2->setAnchorPoint(ccp(0.5, 1));
	pCenterBg2->setPreferredSize(CCSize(710, 100));
	pCenterBg2->setPosition(ccp(panel_width / 2, pCenterBg->getPositionY() - pCenterBg->getContentSize().height + 3));
	Panel->addChild(pCenterBg2);

	CCLabelTTF* pTipText1= CCLabelTTF::create(TT("UIText_EnsureBasePoint_Text1"), TTF_NAME, 30);
	pTipText1->setPosition(ccp(65, pCenterBg->getPositionY() - pCenterBg->getContentSize().height / 2));
	pTipText1->setAnchorPoint(ccp(0, 0.5f));
	pTipText1->setColor(ccc3(200, 200, 200));
	Panel->addChild(pTipText1);

	base_point = CCLabelTTF::create(" ", TTF_NAME, 30);
	base_point->setPosition(ccp(pTipText1->getPositionX() + pTipText1->getContentSize().width, pCenterBg->getPositionY() - pCenterBg->getContentSize().height / 2 ));
	base_point->setAnchorPoint(ccp(0, 0.5f));
	base_point->setColor(ccc3(225, 150, 0));
	Panel->addChild(base_point);

	m_pTipText2= CCLabelTTF::create(TT("UIText_EnsureBasePoint_Text2"), TTF_NAME, 30);
	m_pTipText2->setPosition(ccp(base_point->getPositionX() + base_point->getContentSize().width,pCenterBg->getPositionY() - pCenterBg->getContentSize().height / 2));
	m_pTipText2->setAnchorPoint(ccp(0, 0.5f));
	m_pTipText2->setColor(ccc3(200, 200, 200));
	Panel->addChild(m_pTipText2);

	CCSprite* pkCloseBtnNormal = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	CCSprite* pkCloseBtnPress = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	pkCloseBtnPress->setColor(g_pkGuanDanGame->GetColorLevelDiv2(pkCloseBtnPress->getColor()));

	ODUIButton* pkCloseBtn = ODE_NEW ODUIButton(pkCloseBtnNormal, pkCloseBtnPress);
	pkCloseBtn->setPosition(ccp(ui_w(0.5) + (panel_width / 2 - 30), ui_h(0.5) + (panel_height / 2 - 30)));
	pkCloseBtn->setContentSize(CCSize(pkCloseBtn->getContentSize().width * 2, pkCloseBtn->getContentSize().height * 2));
	AddWidget("kAgreeCloseBtn", pkCloseBtn);
	UILinkDelegate(pkCloseBtn, OnClose);

	UIBarH* pkCancelNormal = UIBarH::createWithEl("kessen_btn_g", 180);
	UIBarH* pkCancelPress = UIBarH::createWithEl("kessen_btn_g_p", 180);

	ODUIButton* pkCancelBtn = ODE_NEW ODUIButton(pkCancelNormal, pkCancelPress);
	pkCancelBtn->setPosition(ccp(ui_w(0.5) + (-panel_width / 2 + 196), ui_h(0.5) + (-panel_height / 2 + 65)));
	AddWidget("kAgreeCancelBtn", pkCancelBtn);
	UILinkDelegate(pkCancelBtn, OnCancel);

	CCSprite* CancelText = CCSprite::createWithSpriteFrameName("kessen_base_cancel.png");
	CancelText->setPositionY(-2.0f);
	pkCancelBtn->addChild(CancelText, 2);

	UIBarH* EnterMatchBtnNormal = UIBarH::createWithEl("kessen_btn_y", 180);
	UIBarH* EnterMatchBtnPress = UIBarH::createWithEl("kessen_btn_y_p", 180);

	ODUIButton* EnterMatchBtn = ODE_NEW ODUIButton(EnterMatchBtnNormal, EnterMatchBtnPress);
	EnterMatchBtn->setPosition(ccp(ui_w(0.5) + (panel_width / 2 - 196), pkCancelBtn->getPositionY()));
	AddWidget("enter_match_btn", EnterMatchBtn);
	UILinkDelegate(EnterMatchBtn, OnConfirm);

	CCSprite* ConfirmText = CCSprite::createWithSpriteFrameName("kessen_base_confirm.png");
	EnterMatchBtn->addChild(ConfirmText, 2);

	return ODUILayerModalExt<GuanDanAgreeBasePointAlert>::init();
}

void GuanDanAgreeBasePointAlert::OnActive( const ODUIUserDataPtr& spData )
{
	char strBaseScore[128];
	sprintf(strBaseScore,"%d",g_pkClientData->getJoinTablePoint());
	base_point->setString(strBaseScore);
	m_pTipText2->setPositionX(base_point->getPositionX() + base_point->getContentSize().width + 5);
}

void GuanDanAgreeBasePointAlert::OnDeactive( const ODUIUserDataPtr& spData )
{
}

bool GuanDanAgreeBasePointAlert::OnBack()
{
	if (! m_bBaseAlertConfirm)
	{
	}
	return true;
}

void GuanDanAgreeBasePointAlert::term()
{
	ODUILayer::term();
}

UIImplementDelegate(GuanDanAgreeBasePointAlert, OnClose, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		GuanDanTableLayer* pkTemp = (GuanDanTableLayer*)g_pLayerManager->GetLayer("GuanDan_TableLayer");
		pkTemp->getGameTableList()->SetTouchActive(true);
		g_pLayerManager->Back();
	}
}

UIImplementDelegate(GuanDanAgreeBasePointAlert, OnCancel, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		GuanDanTableLayer* pkTemp = (GuanDanTableLayer*)g_pLayerManager->GetLayer("GuanDan_TableLayer");
		pkTemp->getGameTableList()->SetTouchActive(true);
		g_pkClientData->setAgreeBasePoint(false);
		g_pLayerManager->Back();
	}
}

UIImplementDelegate(GuanDanAgreeBasePointAlert, OnConfirm, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		TcpClientSocket *pGameSocket = g_pkNetworkHelper->GetGameServerDelegate();
		if (pGameSocket->IsConnect())
		{
			PUSRoomExJoinTableReq JoinTableReq={0};
			JoinTableReq.iTableID = g_pkClientData->getJoinTableId();
			pGameSocket->SendMsgToSocket(&JoinTableReq, GAMEFCT_MSG_PUSEX_JOIN_TABLE_REQ, sizeof(JoinTableReq));
		}
	}
}

void GuanDanAgreeBasePointAlert::ClockTimerEnd()
{
}

void GuanDanAgreeBasePointAlert::SetAgreePointClockVisible( bool bVisible )
{
	MyAgreeBasePointClock->SetVisible(bVisible);

	if (bVisible)
	{
		MyAgreeBasePointClock->Start();
	}
	else
	{
		MyAgreeBasePointClock->Stop();
	}
}

void GuanDanAgreeBasePointAlert::SetCurrTableInfo( ODEInt32 iTableId, ODEInt64 iBasePoint )
{
	m_iTableId = iTableId;
	m_iBasePoint = iBasePoint;
}
