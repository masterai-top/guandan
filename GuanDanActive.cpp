#include "GuanDanActive.h"
#include "msg_GuanDanProto.h"
#include "GuanDanDef.h"

using namespace GuanDan;

GuanDanActiveChooseBtn::GuanDanActiveChooseBtn (cocos2d::CCNode* pkNormal, cocos2d::CCNode* pkPressed, cocos2d::CCNode* ChooseSprite, cocos2d::CCNode * normalSprite, CCNode * chooseIcon) : UIChooseButton(pkNormal, pkPressed)
{
	m_ChooseSprite = (CCSprite*)ChooseSprite;
	m_NormalSprite = (CCSprite*)normalSprite;
	m_ChooseBtnIcon = (CCSprite * )chooseIcon;
	m_ChooseBtnIcon->setPosition (ccp (m_pkNormal->boundingBox ().getMinX () + 25 + m_ChooseBtnIcon->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));
	m_ChooseSprite->setPosition (ccp (m_ChooseBtnIcon->boundingBox ().getMaxX () + 30 + m_ChooseSprite->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));
	m_NormalSprite->setPosition (ccp (m_ChooseBtnIcon->boundingBox ().getMaxX () + 30 + m_NormalSprite->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));

	addChild (m_ChooseBtnIcon, 4);
	addChild (m_ChooseSprite, 4);
	addChild (m_NormalSprite, 4);

	SetChoose (false);
	setContentSize (CCSizeMake (325.0f, 123.0f));
}

void GuanDanActiveChooseBtn::SetActiveShowTips (bool ShowTips, char * tipsPath)
{
	CCNode * tips = getChildByTag (100);
	if (tips != NULL)
		tips->setVisible (ShowTips);
	else
	{
		CCSprite * m_Tips = CCSprite::create (tipsPath);
		m_Tips->setPosition (ccp (m_pkNormal->boundingBox ().getMaxX () - 15 - m_Tips->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));
		addChild (m_Tips, 5, 100);
		m_Tips->setVisible (ShowTips);
	}
}

ODUIWidget* GuanDanActiveChooseBtn::ProcessTouch(ODUILayer::TouchType eType, cocos2d::CCTouch* pkTouch)
{
	ODUIWidget* pkWidget = ODUIWidget::ProcessTouch(eType, pkTouch);
	if(pkWidget) return pkWidget;

	if(m_bEnabled && m_bVisible&&!g_pLayerManager->IsLock())
	{
		switch(eType)
		{
		case ODUILayer::TOUCH_BEGAN:
			if(m_eState != PRESS)
			{
				m_eState = PRESS;
				m_isChoose = true;
				SetChoose(m_isChoose);
				m_kEvent.Callback(*this, PRESSED);
				
			}
			break;
		case ODUILayer::TOUCH_MOVED:
		case ODUILayer::TOUCH_ENDED:
		case ODUILayer::TOUCH_CANCELLED:
			if(m_eState != NORMAL)
			{
				m_eState = NORMAL;
				m_kEvent.Callback(*this, RELEASED);
			}
			break;
		}
		return this;
	}
	else
	{
		return NULL;
	}
}

void GuanDanActiveChooseBtn::setclickStatus (State stats)
{
	m_eState = stats;
}

void GuanDanActiveChooseBtn::UpdateAnimation ()
{
	if(GetEnable())
	{
		if(m_eState == NORMAL)
		{
			if(m_pkNormal)
				m_pkNormal->setVisible(true);
			if(m_pkPressed)
				m_pkPressed->setVisible(false);
		}
		else
		{
			if(m_pkNormal)
				m_pkNormal->setVisible(false);
			if(m_pkPressed)
				m_pkPressed->setVisible(true);
		}

		if(m_pkDisabled)
			m_pkDisabled->setVisible(false);


		if (m_isChoose)
		{
			if (m_ChooseSprite)
				m_ChooseSprite->setVisible (true);

			if (m_NormalSprite)
				m_NormalSprite->setVisible (false);
		}
		else
		{
			if (m_ChooseSprite)
				m_ChooseSprite->setVisible (false);

			if (m_NormalSprite)
				m_NormalSprite->setVisible (true);
		}
	}
	else
	{
		if(m_pkNormal)
			m_pkNormal->setVisible(false);
		if(m_pkPressed)
			m_pkPressed->setVisible(false);
		if(m_pkDisabled)
			m_pkDisabled->setVisible(true);
		if (m_ChooseSprite)
			m_ChooseSprite->setVisible (false);
		if (m_NormalSprite)
			m_NormalSprite->setVisible (true);
	}
}

void GuanDanActiveChooseBtn::SetChoose(bool bChoose)
{
	m_isChoose = bChoose;
	UpdateAnimation ();
}

void GuanDanActiveChooseBtn::SetChooseIndex (int index)
{
	m_index = index;
}

GuanDanActive::GuanDanActive () : ODUILayerModalExt<GuanDanActive>("GuanDan_Active")
	,m_bCreateScene(false)
{

	m_kOnFadeIn.Set(this, &GuanDanActive::OnFadeIn);
	m_kOnFadeOut.Set(this, &GuanDanActive::OnFadeOut);

	UIInitDelegate(GuanDanActive, OnChangeView);
	UIInitDelegate(GuanDanActive, OnChangePayTipsRank);
	UIInitDelegate(GuanDanActive, OnPayTips);
	UIInitDelegate(GuanDanActive, OnCrazyGuanDan);
	UIInitDelegate(GuanDanActive, OnGetLuckTypeAward);
	UIInitDelegate(GuanDanActive, OnGetMedalsAward);
	
	m_GuanDanActiveList = NULL;

	m_PayTipsTableView = NULL;
	m_PayTipsRankList = NULL;
	m_PayTipsTitleNode = NULL;
	m_payTipsRankBtn = NULL;
	m_payTipsButton = NULL;
	m_payTipsTip = NULL;

	m_LuckyTypeNode = NULL;
	m_LuckyTypeButton = NULL;
	m_LuckyTypeTips = NULL;

	m_CrazyGuanDanNode  = NULL;
	m_CrazyPokerButton = NULL;
	m_GetMedalsAwardButton  = NULL;
	m_MedalsTips= NULL;
}

GuanDanActive::~GuanDanActive ()
{
	term ();
	m_bCreateScene = false;
}

void GuanDanActive::OnActive(const ODUIUserDataPtr& spData)
{
	if (!m_bCreateScene)
		createScene ();

	m_PayTipsRankIndex = 0;

	if (m_bottomBtns)
		m_bottomBtns->RefreshPersonalData ();

	//德州活动数据请求
	{
		g_pkNetworkHelper->GetGuanDanPlayerPayTipsRankOnLobby (1);
		g_pkNetworkHelper->GetGuanDanPlayerPayTipsRankOnLobby (2);
		g_pkNetworkHelper->GetGuanDanLuckyTypeProgressOnLobby ();
		g_pkNetworkHelper->GetGuanDanMedalsProgressOnLobby ();
	}

	unschedule (schedule_selector (GuanDanActive::UpdateActiveTips));
	schedule (schedule_selector (GuanDanActive::UpdateActiveTips), 1.0f);


	unschedule (schedule_selector (GuanDanActive::UpdatePayTipsRank));
	scheduleOnce (schedule_selector (GuanDanActive::UpdatePayTipsRank), 1.5f);

	//update TipsShow

	g_pkGuanDanGame->FadeIn(FADE_IN_TIME, &m_kOnFadeIn);
}


void GuanDanActive::UpdateActiveTips (ODEFloat32 u32Delta)
{
	for (int i = GuanDanActiveType_PayTips; i < GuanDanActiveType_Max; i++)
	{
		if (i == GuanDanActiveType_Achievement)
		{
			if (g_pkGuanDanActiveData->isHasMedalsNotGetAward ())
			{
				if (m_chooseButtonArray[i] != NULL)
					m_chooseButtonArray[i]->SetActiveShowTips (true, "TexasPoker/image/TexasPokerActive/activeTips.png");
			}
			else
			{
				if (m_chooseButtonArray[i] != NULL)
					m_chooseButtonArray[i]->SetActiveShowTips (false, "TexasPoker/image/TexasPokerActive/activeTips.png");
			}

			RefreshGuanDanActiveListItems ();
		}
		else if (i == GuanDanActiveType_LuckyType)
		{
			if (g_pkGuanDanActiveData->isHasLuckyTypeNotGetAward ())
			{
				if (m_chooseButtonArray[i] != NULL)
					m_chooseButtonArray[i]->SetActiveShowTips (true, "TexasPoker/image/TexasPokerActive/activeTips.png");
			}
			else
			{
				if (m_chooseButtonArray[i] != NULL)
					m_chooseButtonArray[i]->SetActiveShowTips (false, "TexasPoker/image/TexasPokerActive/activeTips.png");
			}

			RefreshLuckyTypeItems ();
		}
		else
		{
			if (m_chooseButtonArray[i] != NULL)
				m_chooseButtonArray[i]->SetActiveShowTips (false, "TexasPoker/image/TexasPokerActive/activeTips.png");
		}
	}
}

void GuanDanActive::UpdatePayTipsRank (ODEFloat32 u32Delta)
{
	RefreshPayTipsListItems (m_PayTipsRankIndex);
}

void GuanDanActive::OnDeactive(const ODUIUserDataPtr& spData)
{
	unschedule (schedule_selector (GuanDanActive::UpdateActiveTips));
	unschedule (schedule_selector (GuanDanActive::UpdatePayTipsRank));



	removeTextureManual ();
}

void GuanDanActive::removeTextureManual ()
{
	CCTextureCache::sharedTextureCache()->removeTextureForKey("image/line_2.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey("image/line_4.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey("image/line_5.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey("image/blackframe.png");
#ifdef _CACHEDTEXTTUREINFO_
	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
#endif
}

bool GuanDanActive::init ()
{
	return ODUILayer::init ();
}

void GuanDanActive::createScene ()
{
	m_bCreateScene = true;

	LobbyBackground * m_background = new LobbyBackground (false);
	AddWidget("lobby_back_ground", m_background);

	m_topButtonBar = new SceneLobbyTopBar (TITLE_GUANDAN_ACTIVITY);
	m_topButtonBar->setPosition (ccp(ui_w(0.5f), ui_h(1.0f) - 42 * g_pkGuanDanGame->GetHeightScale ()));
	AddWidget ("chargemenu_tops_bar", m_topButtonBar, 3);

	createGuanDanActiveList ();

	CCSprite * bottomLines = CCSprite::create ("image/bag/bag_bottom_lines.png");
	bottomLines->setScaleX (ui_w (1.0f) / bottomLines->getContentSize ().width );
	bottomLines->setScaleY (ui_main_scale);
	bottomLines->setPosition (ccp (ui_w (0.5f), ui_h (0.5f) - 160 * g_pkGuanDanGame->GetHeightScale () - 69 * g_pkGuanDanGame->GetHeightScale () - 10 * g_pkGuanDanGame->GetHeightScale ()));
	addChild (bottomLines, 10);


#if IS_SINGLE_CHANNEL_PAK
	m_bottomBtns = new SceneBottomBar ();
	m_bottomBtns->setScale (ui_main_scale);
	AddWidget ("bottom_buttons", m_bottomBtns, 10);
#else
	m_bottomBtns = new SceneBottomBarForLobbyStart ();
	m_bottomBtns->setScale (ui_main_scale);
	AddWidget ("bottom_buttons", m_bottomBtns, 10);
#endif

}


void GuanDanActive::createGuanDanActiveList ()
{
	const float h_var = 123.0f;

	ODEList < LobbyGameInfoItemForChannel * > * m_gameList = g_pkLobbyConnectData->getGameChannelList ();

	pkChildTableView = ODE_NEW ODUITableView(5);
	{
		const char* press_filename[4] = {"payTipsPress.png","achievePress.png","LuckyTypePress.png","crazyTexasPress.png"};
		const char* normal_filename[4] = {"payTipsNormal.png", "achieveNormal.png", "LuckyTypeNormal.png","crazyTexasNoraml.png"};
		const char* chooseIcon[4] = {"payTipsIcon.png", "achieveIcon.png", "LuckyTypeIcon.png","crazyTexasIcon.png"};

		int iCounts = 0;

		for (int i =0; i < 4; i++)
		{
			//bool hasGame = false;

			////获取游戏列表信息
			//if (m_gameList)
			//{
			//	ODEList< LobbyGameInfoItemForChannel *>::iterator it = m_gameList->Begin();
			//	for (it;it != m_gameList->End();it++)
			//	{
			//		if ((*it)->nGameID == 601 && i == 0)
			//		{
			//			hasGame  = true;
			//			break;
			//		}
			//		else if ((*it)->nGameID == 602 && i == 1)
			//		{
			//			hasGame  = true;
			//			break;
			//		}
			//		else if ((*it)->nGameID == 616 && i == 2)
			//		{
			//			hasGame  = true;
			//			break;
			//		}
			//		else if ((*it)->nGameID == 627 && i == 3)
			//		{
			//			hasGame  = true;
			//			break;
			//		}
			//		else if ((*it)->nGameID == 606 && i == 4)
			//		{
			//			hasGame  = true;
			//			break;
			//		}
			//		else if ((*it)->nGameID == 690 && i == 5)
			//		{
			//			hasGame  = true;
			//			break;
			//		}
			//		else if ((*it)->nGameID == 680 && i == 6)
			//		{
			//			hasGame = true;
			//			break;
			//		}
			//	}
			//}

			//if (!hasGame)
			//	continue;

			CCSprite* left_split = CCSprite::create("image/exchange/anniu01.png");
			left_split->setScaleX(325.0f*ui_width_scale / left_split->getContentSize ().width);
			left_split->setPosition(ccp(162.5f*ui_main_scale, 636.0f*ui_height_scale - (iCounts + 1)*h_var*ui_height_scale));
			addChild(left_split);

			CCSprite * normal = CCSprite::create ("image/InvisibleBtn.png");
			normal->setScaleX (325.0f / normal->getContentSize ().width);
			normal->setScaleY (123.0f / normal->getContentSize ().height);

			CCSprite * press = CCSprite::create ("TexasPoker/image/TexasPokerActive/itemBack.png");
			press->setScaleX (325.0f / press->getContentSize ().width);
			press->setScaleY (123.0f / press->getContentSize ().height);

			char tempChooseIcon[128];
			sprintf(tempChooseIcon,"%s%s","TexasPoker/image/TexasPokerActive/",chooseIcon[i]);
			CCSprite * chooseIcon = CCSprite::create (tempChooseIcon);

			char temp_pressname[128];
			sprintf(temp_pressname,"%s%s","TexasPoker/image/TexasPokerActive/",press_filename[i]);
			CCSprite * pkRulesBtnPress = CCSprite::create (temp_pressname);

			char temp_normalname[128];
			sprintf(temp_normalname,"%s%s","TexasPoker/image/TexasPokerActive/",normal_filename[i]);
			CCSprite * pkRulesBtnNormal = CCSprite::create(temp_normalname);

			GuanDanActiveChooseBtn* activeChooseBtn = ODE_NEW GuanDanActiveChooseBtn(normal, press,pkRulesBtnPress, pkRulesBtnNormal,chooseIcon);
			activeChooseBtn->setScale(ui_main_scale);
			activeChooseBtn->setPosition(ccp(162.5f*ui_main_scale, (636.0f - iCounts*h_var - h_var * 0.5f)*ui_height_scale));
			pkChildTableView->AddButton(activeChooseBtn);
			m_chooseButtonArray[i] = activeChooseBtn;
			activeChooseBtn->SetChooseIndex (i);
			ODUIPage* pkRulesPage = ODE_NEW ODUIPage;
			pkChildTableView->AddPage(pkRulesPage);
			iCounts ++;
		}
	}

	AddWidget("Prize_tableview",pkChildTableView,3);
	UILinkDelegate(pkChildTableView, OnChangeView);

	CCSprite* buttonleftSprite = CCSprite::create("image/downloads/bianyuantiao.png");
	buttonleftSprite->setScaleY ((ui_h (0.5f) + 160 * ui_height_scale) / buttonleftSprite->getContentSize ().height);
	buttonleftSprite->setScaleX (ui_main_scale);
	buttonleftSprite->setAnchorPoint (ccp (0.0f, 1.0f));
	buttonleftSprite->setPosition(ccp(325.0f * ui_main_scale, ui_h (1.0f) - 80.0f * ui_height_scale));
	addChild(buttonleftSprite, 3);

	pkChildTableView->Change (0);
	m_chooseButtonArray[0]->setclickStatus (ODUIButton::State (1));
	m_chooseButtonArray[0]->SetChoose (true);
}

void GuanDanActive::term ()
{
	if (m_bottomBtns != NULL)
	{
		removeChild (m_bottomBtns);
		delete m_bottomBtns;
		m_bottomBtns = NULL;
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_chooseButtonArray[i] != NULL)
		{
			removeChild (m_chooseButtonArray[i]);
			delete m_chooseButtonArray[i];
			m_chooseButtonArray[i] = NULL;
		}
	}

	if (m_PayTipsRankList != NULL)
	{
		m_PayTipsRankList->removeAllButton ();
		removeChild (m_PayTipsRankList);
		delete m_PayTipsRankList;
		m_PayTipsRankList = NULL;
	}

	if (m_PayTipsTableView != NULL)
	{
		removeChild (m_PayTipsTableView);
		delete m_PayTipsTableView;
		m_PayTipsTableView = NULL;
	}

	if (m_payTipsRankBtn != NULL)
	{
		removeChild (m_payTipsRankBtn);
		delete m_payTipsRankBtn;
		m_payTipsRankBtn = NULL;
	}

	if (m_payTipsButton != NULL)
	{
		removeChild (m_payTipsButton);
		delete m_payTipsButton;
		m_payTipsButton = NULL;
	}

	if (m_GuanDanActiveList != NULL)
	{
		m_GuanDanActiveList->removeAllBtn ();
		removeChild (m_GuanDanActiveList);
		delete m_GuanDanActiveList;
		m_GuanDanActiveList = NULL;
	}

	if (m_GetMedalsAwardButton != NULL)
	{
		removeChild (m_GetMedalsAwardButton);
		delete m_GetMedalsAwardButton;
		m_GetMedalsAwardButton = NULL;
	}

	if (m_LuckyTypeButton != NULL)
	{
		removeChild (m_LuckyTypeButton);
		delete m_LuckyTypeButton;
		m_LuckyTypeButton = NULL;
	}

	if (m_CrazyPokerButton != NULL)
	{
		removeChild (m_CrazyPokerButton);
		delete m_CrazyPokerButton;
		m_CrazyPokerButton = NULL;
	}

	ODUILayer::term ();
}

bool GuanDanActive::OnBack ()
{
	return true;
}

void GuanDanActive::OnMenu ()
{
}

void GuanDanActive::onExit ()
{
	ODUILayer::onExit();
}

void GuanDanActive::OnFadeIn()
{}

void GuanDanActive::OnFadeOut()
{
	g_pkGuanDanGame->addChangeIndexToQuene(LAYER_MANAGER_QUENE_MAIN_LAYER);
	g_pkGuanDanGame->FadeIn(FADE_IN_TIME);
}

UIImplementDelegate(GuanDanActive, OnChangeView, kWidget, u32State)
{
	if (u32State == ODUIWidget::VALUE_CHANGE)
	{
		ODUITableView& kMainView = (ODUITableView&)kWidget;
		ODEUInt32 u32Index = kMainView.GetCurrent();
		ODUIWidget * selectWidget = kMainView.GetCurrentButton ();
		m_currentGuanDanActiveType = (GuanDanActiveType)u32Index;
		RefreshChooseBtnState(u32Index);
		createGuanDanActiveContent ();
	}
}

UIImplementDelegate(GuanDanActive, OnChangePayTipsRank, kWidget, u32State)
{
	if (u32State == ODUIWidget::VALUE_CHANGE)
	{
		ODUITableView& kMainView = (ODUITableView&)kWidget;
		m_PayTipsRankIndex = kMainView.GetCurrent();
		initTableViewStatus (m_PayTipsRankIndex);
		RefreshPayTipsListItems (m_PayTipsRankIndex);
	}
}

UIImplementDelegate(GuanDanActive, OnGetMedalsAward, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		g_pkNetworkHelper->GetGuanDanMedalsGetAwardOnLobby (true);
	}
}
UIImplementDelegate(GuanDanActive, OnGetLuckTypeAward, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		g_pkNetworkHelper->GetGuanDanLuckyTypeGetAwardOnLobby (true);
	}
}
UIImplementDelegate(GuanDanActive, OnPayTips, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		g_pkGuanDanGame->PushCommand (CMD_GUANDAN_NORMAL_ROOM_ENTER);
	}
}

UIImplementDelegate(GuanDanActive, OnCrazyGuanDan, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		char tempBuf[256] = {0};
		sprintf (tempBuf, "GD_RoomChoose_ShowEnable_%d", 0);

		if (atoi (GetConfig (tempBuf)) == 0)
		{	
			g_pkGuanDanGame->Toast (TT ("UIText_PT_CrazyTexasPoker_Close"));
		}
		else
		{
	//g_pkGuanDanGame->PushCommand (CMD_GUANDAN_GAME_CRAZY_ENTER);
		}
	}
}

void GuanDanActive::HidePayTipsViews (bool visible)
{
	if (m_PayTipsTableView)
		m_PayTipsTableView->SetVisible (visible);

	if (m_PayTipsRankList)
		m_PayTipsRankList->SetVisible (visible);

	if (m_PayTipsTitleNode)
		m_PayTipsTitleNode->setVisible (visible);

	if (m_payTipsRankBtn)
		m_payTipsRankBtn->SetVisible (visible);

	if (m_payTipsButton)
	{
		m_payTipsButton->SetVisible (visible);
		m_payTipsButton->SetEnable (visible);
	}
	

	if (m_payTipsTip)
		m_payTipsTip->setVisible (visible);

	for (int i = 0 ; i <= 1; i++)
	{
		getChildByTag(GUANDAN_PATTIPS_CHOOSEBTN_TAG +i)->setVisible(visible);
		getChildByTag(GUANDAN_PATTIPS_CHOOSEBTN_BG_TAG +i)->setVisible(visible);
		getChildByTag(GUANDAN_PATTIPS_CHOOSEBTN_TEXT + i)->setVisible (visible);
	}
}


void GuanDanActive::HideMedalsViews (bool visible)
{
	if (m_LuckyTypeNode)
	m_LuckyTypeNode->setVisible (visible);
	if (m_LuckyTypeButton)
	{
		m_LuckyTypeButton->SetEnable (visible);
		m_LuckyTypeButton->SetVisible (visible);
	}
	if (m_LuckyTypeTips)
	m_LuckyTypeTips->setVisible (visible);
}

void GuanDanActive::HideCrazyGuanDanViews (bool visible)
{
	if (m_CrazyGuanDanNode)
	m_CrazyGuanDanNode->setVisible (visible);
	if (m_CrazyPokerButton)
	{
		m_CrazyPokerButton->SetVisible (visible);
		m_CrazyPokerButton->SetEnable (visible);
	}
	
}

void GuanDanActive::HideGuanDanActiveViews (bool visible)
{
	if (m_GuanDanActiveList)
	m_GuanDanActiveList->SetVisible (visible);
	if (m_guanDanActiveBeauty)
	m_guanDanActiveBeauty->setVisible (visible);
	if (m_GetMedalsAwardButton)
	{
		m_GetMedalsAwardButton->SetEnable (visible);
		m_GetMedalsAwardButton->SetVisible (visible);
	}

	if (m_MedalsTips)
		m_MedalsTips->setVisible (visible);
}

void GuanDanActive::initTableViewStatus (int select_index)
{
	for (int i = 0 ; i <= 1; i++)
	{
		if (i!= select_index)
		{
			getChildByTag(GUANDAN_PATTIPS_CHOOSEBTN_TAG +i)->setVisible(false);
			getChildByTag(GUANDAN_PATTIPS_CHOOSEBTN_BG_TAG +i)->setVisible(true);
		}
		else
		{
			getChildByTag(GUANDAN_PATTIPS_CHOOSEBTN_TAG +i)->setVisible(true);
			getChildByTag(GUANDAN_PATTIPS_CHOOSEBTN_BG_TAG +i)->setVisible(false);
		}
	}
}

void GuanDanActive::RefreshPayTipsListItems (int selectIndex)
{
	if (m_PayTipsRankList != NULL)
		m_PayTipsRankList->ResetAllRankItemData  ();
	else
		return ;

	bool isPerDayRank = false;

	if (selectIndex == 0)
		isPerDayRank = true;
	else
		isPerDayRank = false;

	std::vector<GuanDanPaytipRankingItem> * PayTipsRanking = g_pkGuanDanActiveData->GetPayTipsRanking (isPerDayRank);
	std::vector<GuanDanPaytipRankingItem>::iterator it = PayTipsRanking->begin ();

	while (it != PayTipsRanking->end ())
	{
		GuanDanPayTipsButton * selItems = m_PayTipsRankList->GetRankItem (it->nRanking);
		if (selItems != NULL)
		{
			selItems->setHeaderImage (it->nAvatar);
			selItems->setNickName (ODETools::GBKToUTF8 (it->szNickName.c_str ()).c_str ());
			selItems->setRank (it->nRanking);
			selItems->setPlayScore (it->nPaytip);
		}
		it++;
	}

	//m_PayTipsRankList->HideNotHavePayScore ();

	int size = PayTipsRanking->size ();

	if (size <= 0) return ;

	it = PayTipsRanking->begin () + size - 1;

	if (m_payTipsRankBtn != NULL)
	{
		m_payTipsRankBtn->setHeaderImage (it->nAvatar);
		m_payTipsRankBtn->setNickName (ODETools::GBKToUTF8 (it->szNickName.c_str ()).c_str ());
		m_payTipsRankBtn->setRank (it->nRanking);
		m_payTipsRankBtn->setPlayScore (it->nPaytip);
	}
}

void GuanDanActive::RefreshGuanDanActiveListItems ()
{
	if (m_GuanDanActiveList == NULL)
		return ;

	std::vector<GuanDanMedalProgressItem> * MedalsProgress = g_pkGuanDanActiveData->GetMedalsProgress ();
	std::vector<GuanDanMedalProgressItem>::iterator it = MedalsProgress->begin ();

	while (it != MedalsProgress->end ())
	{
		GuanDanMedalsButton * medalsBtn = m_GuanDanActiveList->GetMedalsButton (it->nMedalID);
		if (medalsBtn != NULL)
		{
			{
				medalsBtn->SetMedalsStatus (it->nStatus);
				medalsBtn->SetMedalsDesc (ODETools::GBKToUTF8 (it->szDes.c_str ()).c_str ());
				medalsBtn->SetMedalsProgress (it->doneProgress, it->allProgress);
				medalsBtn->SetMedalsTitle (ODETools::GBKToUTF8 (it->szTitle.c_str ()).c_str ());
			}
		}
		it++;
	}
}


void GuanDanActive::RefreshLuckyTypeItems ()
{
	if (m_LuckyTypeNode == NULL)
	{
		return ;
	}
	std::vector<GuanDanCardtypeProgressItem> * cardTypeProgress = g_pkGuanDanActiveData->GetLuckyTypeCardProgress ();
	std::vector<GuanDanCardtypeProgressItem>::iterator it = cardTypeProgress->begin ();
	int iPos = 0;
	while (it != cardTypeProgress->end ())
	{
		CCNode * target = m_LuckyTypeNode->getChildByTag (100 + iPos);
		if (target != NULL)
		{
			GuanDanLuckyCardTypeNode * luckyCardNode =  (GuanDanLuckyCardTypeNode*)target;
			luckyCardNode->setStatus (it->nStatus);
		}
		iPos ++;
		it++;
	}
}

void GuanDanActive::CreateGuanDanActiveListItems ()
{
	m_GuanDanActiveList->removeAllBtn ();

	std::vector<GuanDanMedalProgressItem> * MedalsProgress = g_pkGuanDanActiveData->GetMedalsProgress ();
	std::vector<GuanDanMedalProgressItem>::iterator it = MedalsProgress->begin ();

	int iPos = 0;

	while (it != MedalsProgress->end ())
	{
		char medalsSprite [256] = {0};
		switch ((it->nMedalID - 1) % 3)
		{
		case 0:
			sprintf (medalsSprite, "%s", "TexasPoker/image/TexasPokerActive/Copper.png");
			break;
		case 1:
			sprintf (medalsSprite, "%s", "TexasPoker/image/TexasPokerActive/Silver.png");
			break;
		case 2:
			sprintf (medalsSprite, "%s", "TexasPoker/image/TexasPokerActive/Kim.png");
			break;
		default:
			sprintf (medalsSprite, "%s", "TexasPoker/image/TexasPokerActive/Copper.png");
			break;
		}


		CCSprite * pkNormal = CCSprite::create (medalsSprite);
		CCSprite * pkPress = CCSprite::create (medalsSprite);
		pkPress->setColor (g_pkGuanDanGame->GetColorLevelDiv2 (pkPress->getColor ()));
		ODGraySprite * pkDisable = ODGraySprite::create (medalsSprite);
		pkDisable->initProgram () ;
		GuanDanMedalsButton * m_MedalsBtns = new  GuanDanMedalsButton (pkNormal, pkPress, pkDisable);
		m_MedalsBtns->SetMedalsID (it->nMedalID);
		m_MedalsBtns->SetMedalsActivityID (it->nActivityID);
		m_MedalsBtns->SetMedalsStatus (it->nStatus);
		m_MedalsBtns->SetMedalsDesc (ODETools::GBKToUTF8 (it->szDes.c_str ()).c_str ());
		m_MedalsBtns->SetMedalsProgress (it->doneProgress, it->allProgress);
		m_MedalsBtns->SetMedalsTitle (ODETools::GBKToUTF8 (it->szTitle.c_str ()).c_str ());
		m_MedalsBtns->setScale (ui_main_scale);
		m_GuanDanActiveList->AddAccountBtn(m_MedalsBtns, iPos);
		m_GuanDanActiveList->ScrollToTop ();
		iPos ++;
		it++;
	}
}

void GuanDanActive::RefreshChooseBtnState(int select_index)
{
	for (int i =0 ; i < 4; i++)
	{
		if (i != select_index)
		{
			if (m_chooseButtonArray[i] != NULL)
				m_chooseButtonArray[i]->setclickStatus (ODUIButton::State (0));
				m_chooseButtonArray[i]->SetChoose (false);
		}
		else
		{
			if (m_chooseButtonArray[i] != NULL)
				m_chooseButtonArray[i]->SetChoose (true);
		}
	}
}

void GuanDanActive::createGuanDanActiveContent ()
{
	CCSprite * titleImage = NULL;
	char titleImageName[128] = {0};

	switch (m_currentGuanDanActiveType)
	{
	case GuanDanActiveType_PayTips:
		sprintf (titleImageName, "TexasPoker/image/TexasPokerActive/payTipsTitle.png");
		break;
	case GuanDanActiveType_LuckyType:
		sprintf (titleImageName, "TexasPoker/image/TexasPokerActive/LuckyTypeTitle.png");
		break;
	case GuanDanActiveType_Achievement:
		sprintf (titleImageName, "TexasPoker/image/TexasPokerActive/achieveTitle.png");
		break;
	case GuanDanActiveType_CrazyGuanDan:
		sprintf (titleImageName, "TexasPoker/image/TexasPokerActive/crazyTexasTitle.png");
		break;
	case GuanDanActiveType_Max:
		break;
	default:
		break;
	}

	if (strcmp (titleImageName, "") != 0)
	{
		titleImage = CCSprite::create (titleImageName);
		if (titleImage != NULL)
			m_topButtonBar->ResetTitleImage (titleImage);
	}

	if (m_PayTipsTableView != NULL)
		HidePayTipsViews (false);

	if (m_LuckyTypeNode != NULL)
		HideMedalsViews (false);

	if (m_CrazyGuanDanNode != NULL)
		HideCrazyGuanDanViews (false);

	if (m_GuanDanActiveList != NULL)
		HideGuanDanActiveViews (false);

	if (m_currentGuanDanActiveType == GuanDanActiveType_PayTips)
	{
		if (m_PayTipsTableView == NULL)
		{
			m_PayTipsTableView = ODE_NEW ODUITableView(2);
			{
				const float w_var = 475.0f;
				const float w_btn = 475.0f;
				const float h_btn = 61.0f;
				const float btn_const_h = 636.0f - 30.5f;
				const char* normal_filename[2] = {TT("UIText_PT_PayTips_Rank_0"),TT("UIText_PT_PayTips_Rank_1")};
				const char* press_filename[2] = {TT("UIText_PT_PayTips_Rank_0"),TT("UIText_PT_PayTips_Rank_1")};

				for (int i = 0;i <= 1; i++)
				{
					CCScale9Sprite* m_bg_normalsp_temp = CCScale9Sprite::create (IMAGE_PATH_LOBBYHELP"choosebtn_normal.png");
					m_bg_normalsp_temp->setPreferredSize(CCSizeMake(w_btn,h_btn));
					m_bg_normalsp_temp->setScale(ui_main_scale);
					m_bg_normalsp_temp->setPosition(ccp(w_btn / 2.0f * ui_main_scale +i*w_var*ui_main_scale + 330.0f * ui_width_scale,(btn_const_h)*ui_height_scale));
					addChild(m_bg_normalsp_temp,5,GUANDAN_PATTIPS_CHOOSEBTN_BG_TAG+i);

					CCScale9Sprite* m_bg_sp_temp = CCScale9Sprite::create (IMAGE_PATH_LOBBYHELP"choosebtn_press.png");
					m_bg_sp_temp->setPreferredSize(CCSizeMake(w_btn,h_btn));
					m_bg_sp_temp->setScale(ui_main_scale);
					m_bg_sp_temp->setPosition(ccp(w_btn / 2.0f * ui_main_scale +i*w_var*ui_main_scale + 330.0f * ui_width_scale,(btn_const_h)*ui_height_scale));
					addChild(m_bg_sp_temp,5,GUANDAN_PATTIPS_CHOOSEBTN_TAG+i);
					m_bg_sp_temp->setVisible(false);

					char temp_pressname[128];
					sprintf(temp_pressname,"%s",press_filename[i]);
					CCLabelTTF* pkmianzeText_press = CCLabelTTF::create(temp_pressname, TTF_NAME, ODEFloorf(30 ));
					pkmianzeText_press->setPosition(ccp(m_bg_sp_temp->getContentSize().width/2,m_bg_sp_temp->getContentSize().height/2));
					pkmianzeText_press->setColor(ccc3(255,225,115));
					m_bg_sp_temp->addChild(pkmianzeText_press,5);

					CCSprite* choose_btn_slider = CCSprite::create(IMAGE_PATH_LOBBYHELP"choose_btn_slider.png");
					choose_btn_slider->setPosition(ccp(m_bg_sp_temp->getContentSize().width/2, 3.5f));
					choose_btn_slider->setScaleX(1.67f);
					choose_btn_slider->setScaleY(1.1f);
					m_bg_sp_temp->addChild(choose_btn_slider,2);

					CCScale9Sprite *m_kTempBtnNormal_1 = CCScale9Sprite::create (IMAGE_PATH_LOBBYHELP"btn_bg.png");
					m_kTempBtnNormal_1->setPreferredSize(CCSizeMake(w_btn,h_btn));
					CCScale9Sprite *m_kTempBtnNormal_2 = CCScale9Sprite::create (IMAGE_PATH_LOBBYHELP"btn_bg.png");
					m_kTempBtnNormal_2->setPreferredSize(CCSizeMake(w_btn,h_btn));
		
					char temp_normalname[128];
					sprintf(temp_normalname,"%s",normal_filename[i]);
					CCLabelTTF* pkmianzeText_normal = CCLabelTTF::create(temp_normalname, TTF_NAME, ODEFloorf(30 ));
					addChild(pkmianzeText_normal, 0, GUANDAN_PATTIPS_CHOOSEBTN_TEXT + i);
					pkmianzeText_normal->setScale(ui_main_scale);
					pkmianzeText_normal->setPosition(ccp(w_btn / 2.0f * ui_main_scale +i*w_var*ui_main_scale + 330.0f * ui_width_scale,(btn_const_h)*ui_height_scale));

					UIChooseButton* pkRulesBtn = ODE_NEW UIChooseButton(m_kTempBtnNormal_1, m_kTempBtnNormal_2);
					pkRulesBtn->setScale(ui_main_scale);
					pkRulesBtn->setPosition(ccp(w_btn / 2.0f * ui_main_scale +i*w_var*ui_main_scale + 330.0f * ui_width_scale,(btn_const_h)*ui_height_scale));
					m_PayTipsTableView->AddButton(pkRulesBtn);
					ODUIPage* pkRulesPage = ODE_NEW ODUIPage;
					m_PayTipsTableView->AddPage(pkRulesPage);
				}
			}
			AddWidget("pay_tips_rank_tableview",m_PayTipsTableView,5);
			UILinkDelegate(m_PayTipsTableView, OnChangePayTipsRank);
		}

		if (m_PayTipsRankList == NULL && g_pkGuanDanActiveData->GetPayTipsRanking (true)->size () >= 0)
		{
			m_PayTipsRankList = ODE_NEW GuanDanPayTipsRankList(930.0f, 225.0f);
			m_PayTipsRankList->setPosition(ccp(475.0f * ui_main_scale + 330.0f * ui_main_scale, 605.0f * ui_height_scale - 85.0f * ui_height_scale));
			m_PayTipsRankList->ScrollToTop();
			AddWidget ("download_content_list", m_PayTipsRankList);

			for (int i = 1; i <= 10; i++)
			{
				ODEChar8 background[256] = {0};

				if (i == 10)
					sprintf (background, "%s", TP_IMAGE_PATH_ACTIVITY"rankingLine.png");
				else
					sprintf (background, "%s", TP_IMAGE_PATH_ACTIVITY"rankingLine2.png");

				CCScale9Sprite * pkNormal = CCScale9Sprite::create (background);
				pkNormal->setScaleX (900.0f / pkNormal->getContentSize ().width);
				CCScale9Sprite * pkPress = CCScale9Sprite::create (background);
				pkPress->setScaleX (900.0f / pkPress->getContentSize ().width);

				GuanDanPayTipsButton * m_rankItems = new  GuanDanPayTipsButton (pkNormal, pkPress, i, 0, "", 0, false);
				m_PayTipsRankList->AddRankItem(m_rankItems);
				m_PayTipsRankList->ScrollToTop ();
			}

			CCScale9Sprite * pkNormal = CCScale9Sprite::create (TP_IMAGE_PATH_ACTIVITY"rankingLine.png");
			pkNormal->setScaleX (900.0f / pkNormal->getContentSize ().width);
			CCScale9Sprite * pkPress = CCScale9Sprite::create (TP_IMAGE_PATH_ACTIVITY"rankingLine.png");
			pkPress->setScaleX (900.0f / pkPress->getContentSize ().width);
			m_payTipsRankBtn = new  GuanDanPayTipsButton (pkNormal, pkPress, 11, 0, "", 0, true);
			m_payTipsRankBtn->setScale (ui_main_scale);
			m_payTipsRankBtn->setPosition (m_PayTipsRankList->boundingBox ().getMidX () - 15 * ui_main_scale, m_PayTipsRankList->boundingBox ().getMinY () - m_payTipsRankBtn->getContentSize ().height / 2 * ui_main_scale);
			AddWidget ("pay_tips_my_rank", m_payTipsRankBtn);

			/*std::vector<TexasPokerPaytipRankingItem> * PayTipsRanking = g_pkGuanDanActiveData->GetPayTipsRanking (false);
			std::vector<TexasPokerPaytipRankingItem>::iterator it = PayTipsRanking->begin ();
			while (it != PayTipsRanking->end ())
			{
				ODEChar8 background[256] = {0};

				if (it == PayTipsRanking->end () -1)
				{
					it++;
					continue;
				}
				else if (it == PayTipsRanking->end () -2)
					sprintf (background, "%s", TP_IMAGE_PATH_ACTIVITY"rankingLine.png");
				else
					sprintf (background, "%s", TP_IMAGE_PATH_ACTIVITY"rankingLine2.png");

				CCScale9Sprite * pkNormal = CCScale9Sprite::create (background);
				pkNormal->setScaleX (900.0f / pkNormal->getContentSize ().width);
				CCScale9Sprite * pkPress = CCScale9Sprite::create (background);
				pkPress->setScaleX (900.0f / pkPress->getContentSize ().width);

				GuanDanPayTipsButton * m_rankItems = new  GuanDanPayTipsButton (pkNormal, pkPress, it->nRanking, it->nAvatar, ODETools::GBKToUTF8 (it->szNickName.c_str ()).c_str (), it->nPaytip, false);
				m_PayTipsRankList->AddRankItem(m_rankItems);
				m_PayTipsRankList->ScrollToTop ();
				it++;
			}

			int size = PayTipsRanking->size ();

			if (size > 0) 
			{
				it = PayTipsRanking->begin () + size - 1;
				CCScale9Sprite * pkNormal = CCScale9Sprite::create (TP_IMAGE_PATH_ACTIVITY"rankingLine.png");
				pkNormal->setScaleX (900.0f / pkNormal->getContentSize ().width);
				CCScale9Sprite * pkPress = CCScale9Sprite::create (TP_IMAGE_PATH_ACTIVITY"rankingLine.png");
				pkPress->setScaleX (900.0f / pkPress->getContentSize ().width);
				m_payTipsRankBtn = new  GuanDanPayTipsButton (pkNormal, pkPress, it->nRanking, it->nAvatar, ODETools::GBKToUTF8 (it->szNickName.c_str ()).c_str (), it->nPaytip, true);
				m_payTipsRankBtn->setScale (ui_main_scale);
				m_payTipsRankBtn->setPosition (m_PayTipsRankList->boundingBox ().getMidX () - 15 * ui_main_scale, m_PayTipsRankList->boundingBox ().getMinY () - m_payTipsRankBtn->getContentSize ().height / 2 * ui_main_scale);
				AddWidget ("pay_tips_my_rank", m_payTipsRankBtn);
			}*/

			RefreshPayTipsListItems (0);
		}
		else
		{
			if (m_PayTipsRankList != NULL)
				RefreshPayTipsListItems (0);
		}

		if (m_PayTipsTitleNode == NULL)
		{
			m_PayTipsTitleNode = CCNode::create ();
			m_PayTipsTitleNode->setContentSize (CCSizeMake (900.0f, 55.0f));
			m_PayTipsTitleNode->setScale (ui_main_scale);

			CCScale9Sprite * pkNormal = CCScale9Sprite::create ("image/mail/mail_delete_box_p.png");
			pkNormal->setPreferredSize (CCSizeMake (900.0f, 55.0f));
			pkNormal->setVisible (false);
			m_PayTipsTitleNode->addChild (pkNormal);

			CCLabelTTF * rankTTF = CCLabelTTF::create (TT ("UIText_PT_PayTips_Rank"), TTF_NAME, 30);
			rankTTF->setPosition (ccp  (pkNormal->boundingBox ().getMinX () + 55, pkNormal->boundingBox ().getMidY ()));
			rankTTF->setColor (ccc3 (235, 184, 0));
			m_PayTipsTitleNode->addChild (rankTTF);

			CCSprite * m_Line1 = CCSprite::create (TP_IMAGE_PATH_ACTIVITY"line.png");
			m_Line1->setPosition (ccp (rankTTF->boundingBox ().getMaxX () + 62.5f, pkNormal->boundingBox ().getMidY ()));
			m_PayTipsTitleNode->addChild(m_Line1);

			CCLabelTTF * headTTF = CCLabelTTF::create (TT ("UIText_PT_PayTips_HeadIndex"), TTF_NAME, 30);
			headTTF->setPosition (ccp  (rankTTF->boundingBox ().getMaxX () + 125.0f + headTTF->getContentSize ().width / 2, pkNormal->boundingBox ().getMidY ()));
			headTTF->setColor (ccc3 (235, 184, 0));
			m_PayTipsTitleNode->addChild (headTTF);

			CCSprite * m_Line2 = CCSprite::create (TP_IMAGE_PATH_ACTIVITY"line.png");
			m_Line2->setPosition (ccp (pkNormal->boundingBox ().getMidX () - 75.0f, pkNormal->boundingBox ().getMidY ()));
			m_PayTipsTitleNode->addChild(m_Line2);

			CCLabelTTF * nickNameTTF = CCLabelTTF::create (TT ("UIText_PT_PayTips_NickName"), TTF_NAME, 30);
			nickNameTTF->setPosition (ccp  (pkNormal->boundingBox ().getMidX () + 45.0f, pkNormal->boundingBox ().getMidY ()));
			nickNameTTF->setColor (ccc3 (235, 184, 0));
			m_PayTipsTitleNode->addChild (nickNameTTF);

			CCLabelTTF * payTipsTTF = CCLabelTTF::create (TT ("UIText_PT_PayTips_PayScore"), TTF_NAME, 30);
			payTipsTTF->setPosition (ccp  (pkNormal->boundingBox ().getMaxX () - 75, pkNormal->boundingBox ().getMidY ()));
			payTipsTTF->setColor (ccc3 (235, 184, 0));
			m_PayTipsTitleNode->addChild (payTipsTTF);

			CCSprite * m_Line3 = CCSprite::create (TP_IMAGE_PATH_ACTIVITY"line.png");
			m_Line3->setPosition (ccp (payTipsTTF->boundingBox ().getMinX () - 75, pkNormal->boundingBox ().getMidY ()));
			m_PayTipsTitleNode->addChild(m_Line3);

			m_PayTipsTitleNode->setPosition (ccp (m_PayTipsRankList->boundingBox ().getMidX () - 15 * ui_main_scale, m_PayTipsRankList->boundingBox ().getMaxY () + m_PayTipsTitleNode->getContentSize ().height / 2 * ui_main_scale));
			addChild (m_PayTipsTitleNode, 4);
		}

		if (m_payTipsButton == NULL)
		{
			CCScale9Sprite * payTipsNormal = CCScale9Sprite::create ("image/mail/mail_confirm.png");
			payTipsNormal->setPreferredSize (CCSizeMake (220.0f, 66.0f));
			CCScale9Sprite * payTipsPress = CCScale9Sprite::create ("image/mail/mail_confirm.png");
			payTipsPress->setPreferredSize (CCSizeMake (220.0f, 66.0f));
			payTipsPress->setColor (g_pkGuanDanGame->GetColorLevelDiv2 (payTipsPress->getColor ()));
			m_payTipsButton = new ODUIButton (payTipsNormal, payTipsPress);
			m_payTipsButton->setScale (ui_main_scale);
			m_payTipsButton->setPosition (ccp (m_PayTipsRankList->boundingBox ().getMidX (), m_PayTipsRankList->boundingBox ().getMinY () - 85 * ui_height_scale - m_payTipsButton->getContentSize ().height / 2 * ui_main_scale));
			UILinkDelegate(m_payTipsButton, OnPayTips);
			m_payTipsButton->SetText (TT ("UIText_PT_PayTips_GoPayTips"), false, 16);
			AddWidget ("pay_tips_btn", m_payTipsButton, 4);
		}

		if (m_payTipsTip == NULL)
		{
			m_payTipsTip = CCLabelTTF::create (GetConfig ("UIText_PT_PayTips_PayTipsTip"), TTF_NAME, 22 * ui_main_scale);
			m_payTipsTip->setColor (ccc3 (208, 22, 33));
			m_payTipsTip->setPosition (ccp (m_PayTipsRankList->boundingBox ().getMinX () + 15 * ui_main_scale + m_payTipsTip->getContentSize ().width / 2,
				m_payTipsButton->boundingBox ().getMinY () - 30 * ui_height_scale - m_payTipsTip->getContentSize ().height / 2));
			addChild (m_payTipsTip, 4);
		}

		HidePayTipsViews (true);
		m_PayTipsTableView->Change (0);
		initTableViewStatus (0);
	}
	else if (m_currentGuanDanActiveType == GuanDanActiveType_Achievement)
	{
		if (m_GuanDanActiveList == NULL)
		{
			m_GuanDanActiveList = ODE_NEW GuanDanActiveMedalsList(950.0f * ui_width_scale, 400.0f *ui_height_scale);
			m_GuanDanActiveList->setPosition(ccp(325.0f * ui_width_scale + 950.0f * ui_width_scale / 2, ui_h(0.5f) + 280.0f * ui_height_scale));
			m_GuanDanActiveList->RefreshList();
			m_GuanDanActiveList->createListBackground(17, 950.0f);
			m_GuanDanActiveList->ScrollToTop();
			AddWidget("active_medals_list", m_GuanDanActiveList);

			m_guanDanActiveBeauty = CCSprite::create ("TexasPoker/image/TexasPokerActive/Beauty.png");
			m_guanDanActiveBeauty->setScale ((1- 0.1) * ui_main_scale);
			m_guanDanActiveBeauty->setAnchorPoint (ccp (0.5f, 0.0f));
			m_guanDanActiveBeauty->setPosition (ccp (m_GuanDanActiveList->boundingBox ().getMaxX () - m_guanDanActiveBeauty->getContentSize ().width / 2 * m_guanDanActiveBeauty->getScale (),
			ui_h (0.5f) - 160 * g_pkGuanDanGame->GetHeightScale () - 69 * g_pkGuanDanGame->GetHeightScale () - 9 * g_pkGuanDanGame->GetHeightScale ()));
			addChild (m_guanDanActiveBeauty, 4);

			CreateGuanDanActiveListItems ();

			if (m_GetMedalsAwardButton == NULL)
			{
				CCScale9Sprite * payTipsNormal = CCScale9Sprite::create ("image/mail/mail_confirm.png");
				payTipsNormal->setPreferredSize (CCSizeMake (220.0f, 66.0f));
				CCScale9Sprite * payTipsPress = CCScale9Sprite::create ("image/mail/mail_confirm.png");
				payTipsPress->setPreferredSize (CCSizeMake (220.0f, 66.0f));
				payTipsPress->setColor (g_pkGuanDanGame->GetColorLevelDiv2 (payTipsPress->getColor ()));
				m_GetMedalsAwardButton = new ODUIButton (payTipsNormal, payTipsPress);
				m_GetMedalsAwardButton->setScale (ui_main_scale);
				m_GetMedalsAwardButton->setPosition (ccp (335.0f * ui_width_scale + 935.0f * ui_width_scale / 2, 
				ui_h (1.0f) - 84.0f * ui_height_scale - m_GuanDanActiveList->getContentSize ().height - 10.0f * ui_height_scale - m_GetMedalsAwardButton->getContentSize ().height / 2 * ui_main_scale));
				UILinkDelegate(m_GetMedalsAwardButton, OnGetMedalsAward);
				m_GetMedalsAwardButton->SetText (TT ("UIText_PT_LuckyCardType_GetAward"), false, 16);
				AddWidget ("get_award_btn", m_GetMedalsAwardButton, 4);
			}

			if (m_MedalsTips == NULL)
			{
				m_MedalsTips = CCLabelTTF::create (TT ("UIText_PT_Medals_Tips"), TTF_NAME, 22 * ui_main_scale);
				m_MedalsTips->setColor (ccc3 (208, 22, 33));
				m_MedalsTips->setPosition (ccp (335.0f * ui_width_scale + 15 * ui_main_scale + m_MedalsTips->getContentSize ().width / 2,
					m_GetMedalsAwardButton->boundingBox ().getMinY () - 35 * ui_height_scale - m_MedalsTips->getContentSize ().height / 2));
				addChild (m_MedalsTips, 4);
			}
		}
		else
		{
			RefreshGuanDanActiveListItems ();
		}

		HideGuanDanActiveViews (true);
	}
	else if (m_currentGuanDanActiveType == GuanDanActiveType_LuckyType)
	{
		if (m_LuckyTypeNode == NULL)
		{
			m_LuckyTypeNode = CCNode::create ();
			m_LuckyTypeNode->setContentSize (CCSizeMake (935.0f * ui_width_scale, 415.0f * ui_height_scale));
			m_LuckyTypeNode->setPosition (ccp (335.0f * ui_width_scale + 935.0f * ui_width_scale / 2, ui_h (1.0f) - 84.0f * ui_height_scale - m_LuckyTypeNode->getContentSize ().height / 2));
			addChild (m_LuckyTypeNode, 4);

			CCScale9Sprite * pkNormal = CCScale9Sprite::create ("image/mail/mail_delete_box_p.png");
			pkNormal->setPreferredSize (CCSizeMake (935.0f * ui_width_scale, 415.0f * ui_height_scale));
			pkNormal->setVisible (false);
			m_LuckyTypeNode->addChild (pkNormal);

			CCLabelTTF * luckyTypeRule = CCLabelTTF::create (TT ("UIText_PT_PayTips_LuckyTypeRule"), TTF_NAME, 26 * ui_main_scale);
			luckyTypeRule->setAnchorPoint (ccp (0.0f, 0.5f));
			luckyTypeRule->setColor (ALERT_CANCEL_TEXT_COLOR);
			luckyTypeRule->setVisible (false);
			luckyTypeRule->setPosition (ccp (pkNormal->boundingBox ().getMinX () + 20 * ui_width_scale, pkNormal->boundingBox ().getMaxY () - 10 * ui_height_scale - luckyTypeRule->getContentSize ().height /2));
			m_LuckyTypeNode->addChild (luckyTypeRule, 4);

			CCLabelTTF * luckyTypeTime = CCLabelTTF::create (TT ("UIText_PT_PayTips_LuckyTypeTime"), TTF_NAME, 26 * ui_main_scale);
			luckyTypeTime->setAnchorPoint (ccp (0.0f, 0.5f));
			luckyTypeTime->setColor (ALERT_CANCEL_TEXT_COLOR);
			luckyTypeTime->setPosition (ccp (pkNormal->boundingBox ().getMinX () + 20 * ui_width_scale, luckyTypeRule->boundingBox ().getMidY ()));
			m_LuckyTypeNode->addChild (luckyTypeTime, 4);

			CCLabelTTF * luckyTypeTimeReally = CCLabelTTF::create (GetConfig ("TP_PayTips_LuckyTypeTime"), TTF_NAME, 26 * ui_main_scale);
			luckyTypeTimeReally->setAnchorPoint (ccp (0.0f, 0.5f));
			luckyTypeTimeReally->setColor (ccc3 (54, 238, 46));
			luckyTypeTimeReally->setPosition (ccp (luckyTypeTime->boundingBox ().getMaxX () + 10 * ui_width_scale, luckyTypeTime->boundingBox ().getMidY ()));
			m_LuckyTypeNode->addChild (luckyTypeTimeReally, 4);

			CCLabelTTF * JoinGame = CCLabelTTF::create (TT ("UIText_PT_PayTips_LuckyTypeJoinGame"), TTF_NAME, 26 * ui_main_scale);
			JoinGame->setAnchorPoint (ccp (0.0f, 0.5f));
			JoinGame->setColor (ALERT_CANCEL_TEXT_COLOR);
			JoinGame->setPosition (ccp (pkNormal->boundingBox ().getMinX () + 20 * ui_width_scale, luckyTypeTime->boundingBox ().getMinY () - 5 * ui_height_scale - JoinGame->getContentSize ().height / 2));
			m_LuckyTypeNode->addChild (JoinGame, 4);

			CCLabelTTF * LuckyTypeContent = CCLabelTTF::create (GetConfig ("UIText_PT_PayTips_LuckyTypeContent"), TTF_NAME, 26 * ui_main_scale);
			LuckyTypeContent->setAnchorPoint (ccp (0.0f, 0.5f));
			LuckyTypeContent->setColor (ALERT_CANCEL_TEXT_COLOR);
			LuckyTypeContent->setPosition (ccp (pkNormal->boundingBox ().getMinX () + 20 * ui_width_scale, JoinGame->boundingBox ().getMinY () - 5 * ui_height_scale - LuckyTypeContent->getContentSize ().height / 2));
			m_LuckyTypeNode->addChild (LuckyTypeContent, 4);

			//创建幸运牌型
			
			std::vector<GuanDanCardtypeProgressItem> * cardTypeProgress = g_pkGuanDanActiveData->GetLuckyTypeCardProgress ();
			std::vector<GuanDanCardtypeProgressItem>::iterator it = cardTypeProgress->begin ();

			int iPos = 0;
			while (it != cardTypeProgress->end ())
			{
				GuanDanLuckyCardTypeNode * luckyCardNode = GuanDanLuckyCardTypeNode::create(ODETools::GBKToUTF8 (it->szTitle.c_str ()).c_str (), ODETools::GBKToUTF8 (it->szDes.c_str ()).c_str (), it->nStatus);
				luckyCardNode->setAnchorPoint (ccp (0.0f, 0.5f));
				luckyCardNode->setScale (ui_main_scale);

				if (iPos % 2 == 0)
				{
					luckyCardNode->setPosition (ccp (pkNormal->boundingBox ().getMinX () + 20 * ui_width_scale, LuckyTypeContent->boundingBox ().getMinY () - 25 * ui_height_scale - (iPos / 2) * (luckyCardNode->getContentSize ().height + 12.5f) * ui_main_scale));

					CCSprite * lines = CCSprite::create ("image/mail/mail_split_v1.png");
					lines->setScaleX (900.0f *ui_main_scale / lines->getContentSize  ().width);
					lines->setPosition (ccp (pkNormal->boundingBox ().getMidX (), LuckyTypeContent->boundingBox ().getMinY () - 25 * ui_height_scale - (iPos / 2) * (luckyCardNode->getContentSize ().height + 12.5f) * ui_main_scale));
					m_LuckyTypeNode->addChild (lines);
				}
				else
				{
					luckyCardNode->setPosition (ccp (pkNormal->boundingBox ().getMidX (), LuckyTypeContent->boundingBox ().getMinY () - 25 * ui_height_scale - (iPos / 2) * (luckyCardNode->getContentSize ().height + 12.5f)  * ui_main_scale));
				}

				if (it == cardTypeProgress->end () - 1)
				{
					CCSprite * lines = CCSprite::create ("image/mail/mail_split_v1.png");
					lines->setScaleX (900.0f * ui_main_scale / lines->getContentSize  ().width);
					lines->setPosition (ccp (pkNormal->boundingBox ().getMidX (), LuckyTypeContent->boundingBox ().getMinY () - 25 * ui_height_scale - ((iPos + 1) / 2) * (luckyCardNode->getContentSize ().height + 12.5f)  * ui_main_scale));
					m_LuckyTypeNode->addChild (lines);
				}

				m_LuckyTypeNode->addChild (luckyCardNode, 0, 100 + iPos);

				iPos ++;
				it++;
			}
		}

		if (m_LuckyTypeButton == NULL)
		{
			CCScale9Sprite * payTipsNormal = CCScale9Sprite::create ("image/mail/mail_confirm.png");
			payTipsNormal->setPreferredSize (CCSizeMake (220.0f, 66.0f));
			CCScale9Sprite * payTipsPress = CCScale9Sprite::create ("image/mail/mail_confirm.png");
			payTipsPress->setPreferredSize (CCSizeMake (220.0f, 66.0f));
			payTipsPress->setColor (g_pkGuanDanGame->GetColorLevelDiv2 (payTipsPress->getColor ()));
			m_LuckyTypeButton = new ODUIButton (payTipsNormal, payTipsPress);
			m_LuckyTypeButton->setScale (ui_main_scale);
			m_LuckyTypeButton->setPosition (ccp (335.0f * ui_width_scale + 935.0f * ui_width_scale / 2, ui_h (1.0f) - 84.0f * ui_height_scale - m_LuckyTypeNode->getContentSize ().height  - 10 * ui_height_scale - m_LuckyTypeButton->getContentSize ().height / 2 * ui_main_scale));
			UILinkDelegate(m_LuckyTypeButton, OnGetLuckTypeAward);
			m_LuckyTypeButton->SetText (TT ("UIText_PT_LuckyCardType_GetAward"), false, 16);
			AddWidget ("lucky_type_btn", m_LuckyTypeButton, 4);
		}

		if (m_LuckyTypeTips == NULL)
		{
			m_LuckyTypeTips = CCLabelTTF::create (TT ("UIText_PT_LuckyCardType_LuckyTip"), TTF_NAME, 22 * ui_main_scale);
			m_LuckyTypeTips->setColor (ccc3 (208, 22, 33));
			m_LuckyTypeTips->setPosition (ccp (335.0f * ui_width_scale + 15 * ui_main_scale + m_LuckyTypeTips->getContentSize ().width / 2,
				m_LuckyTypeButton->boundingBox ().getMinY () - 30 * ui_height_scale - m_LuckyTypeTips->getContentSize ().height / 2));
			addChild (m_LuckyTypeTips, 4);
		}

		HideMedalsViews (true);
	}
	else if (m_currentGuanDanActiveType == GuanDanActiveType_CrazyGuanDan)
	{
		if (m_CrazyGuanDanNode == NULL)
		{
			m_CrazyGuanDanNode = CCNode::create ();
			m_CrazyGuanDanNode->setContentSize (CCSizeMake (935.0f * ui_main_scale, 400.0f * ui_height_scale));
			m_CrazyGuanDanNode->setPosition (ccp (335.0f * ui_width_scale + 935.0f * ui_width_scale / 2, ui_h (1.0f) - 84.0f * ui_height_scale - m_CrazyGuanDanNode->getContentSize ().height / 2));
			addChild (m_CrazyGuanDanNode, 2);

			CCScale9Sprite * pkNormal = CCScale9Sprite::create ("image/mail/mail_delete_box_p.png");
			pkNormal->setPreferredSize (CCSizeMake (935.0f * ui_main_scale, 400.0f * ui_height_scale));
			pkNormal->setVisible (false);
			m_CrazyGuanDanNode->addChild (pkNormal);

			CCSprite* m_crazyGuanDanBack = CCSprite::create ("TexasPoker/image/TexasPokerActive/crazyTexasPokerBack.png");
			m_crazyGuanDanBack->setScale (ui_main_scale);
			m_crazyGuanDanBack->setPosition (ccp (pkNormal->boundingBox ().getMidX (), pkNormal->boundingBox ().getMidY ()));
			m_CrazyGuanDanNode->addChild (m_crazyGuanDanBack, 4);

			CCSprite* m_crazyTexasTextBack = CCSprite::create ("TexasPoker/image/TexasPokerActive/crazyTexasPokerTextBack.png");
			m_crazyTexasTextBack->setScaleX (ui_main_scale);
			m_crazyTexasTextBack->setScaleY (ui_height_scale);
			m_crazyTexasTextBack->setPosition (ccp (pkNormal->boundingBox ().getMidX (), pkNormal->boundingBox ().getMinY () + m_crazyTexasTextBack->getContentSize ().height / 2 * ui_height_scale));
			m_CrazyGuanDanNode->addChild (m_crazyTexasTextBack, 4);

			CCSprite* m_crazyTexasTableBack = CCSprite::create ("TexasPoker/image/TexasPokerActive/crazyTexasPokerTable.png");
			m_crazyTexasTableBack->setScaleX (ui_main_scale);
			m_crazyTexasTableBack->setScaleY (ui_height_scale);
			m_crazyTexasTableBack->setPosition (ccp (pkNormal->boundingBox ().getMidX (), pkNormal->boundingBox ().getMinY () - m_crazyTexasTableBack->getContentSize ().height / 2 * ui_height_scale));
			m_CrazyGuanDanNode->addChild (m_crazyTexasTableBack, 4);

			CCLabelTTF * crazyGuanDanTime = CCLabelTTF::create (TT ("UIText_PT_PayTips_CrazyTexasPoker_Title"), TTF_NAME, 26 * ui_main_scale);
			crazyGuanDanTime->setColor (ALERT_CANCEL_TEXT_COLOR);
			crazyGuanDanTime->setAnchorPoint (ccp (0.0f, 0.5f));
			crazyGuanDanTime->setPosition (ccp (pkNormal->boundingBox ().getMinX () + 20 * ui_main_scale, pkNormal->boundingBox ().getMaxY () - 10 - crazyGuanDanTime->getContentSize ().height /2));
			m_CrazyGuanDanNode->addChild (crazyGuanDanTime, 4);

			CCLabelTTF * crazyGuanDanRealTime = CCLabelTTF::create (GetConfig ("TP_CrazyTexasPoker_Time"), TTF_NAME, 26 * ui_main_scale);
			crazyGuanDanRealTime->setColor (ccc3 (54, 238, 46));
			crazyGuanDanRealTime->setAnchorPoint (ccp (0.0f, 0.5f));
			crazyGuanDanRealTime->setPosition (ccp (crazyGuanDanTime->boundingBox ().getMaxX () + 20 * ui_main_scale, pkNormal->boundingBox ().getMaxY () - 10 - crazyGuanDanRealTime->getContentSize ().height /2));
			m_CrazyGuanDanNode->addChild (crazyGuanDanRealTime, 4);

			CCLabelTTF * JoinGame = CCLabelTTF::create (TT ("UIText_PT_PayTips_CrazyTexasPoker_JoinGame"), TTF_NAME, 26 * ui_main_scale);
			JoinGame->setColor (ALERT_CANCEL_TEXT_COLOR);
			JoinGame->setAnchorPoint (ccp (0.0f, 0.5f));
			JoinGame->setPosition (ccp (pkNormal->boundingBox ().getMinX () + 20 * ui_main_scale, crazyGuanDanTime->boundingBox ().getMinY () - 7.5f - JoinGame->getContentSize ().height / 2));
			m_CrazyGuanDanNode->addChild (JoinGame, 4);

			CCLabelTTF * ContentTitle = CCLabelTTF::create (TT ("UIText_PT_PayTips_CrazyTexasPoker_ContentTitle"), TTF_NAME, 26 * ui_main_scale);
			ContentTitle->setColor (ALERT_CANCEL_TEXT_COLOR);
			ContentTitle->setAnchorPoint (ccp (0.0f, 0.5f));
			ContentTitle->setPosition (ccp (pkNormal->boundingBox ().getMinX () + 20 * ui_main_scale, JoinGame->boundingBox ().getMinY () - 7.5f - JoinGame->getContentSize ().height / 2));
			m_CrazyGuanDanNode->addChild (ContentTitle, 4);

			CCLabelTTF * CrazyTexasContent = CCLabelTTF::create(TT("UIText_PT_PayTips_CrazyTexasPoker_Content"), TTF_NAME, 26 * ui_main_scale, CCSizeMake(550.0f * ui_main_scale, 0), kCCTextAlignmentLeft);
			CrazyTexasContent->setColor (ALERT_CANCEL_TEXT_COLOR);
			CrazyTexasContent->setAnchorPoint (ccp (0.0f, 1.0f));
			CrazyTexasContent->setPosition (ccp (ContentTitle->boundingBox ().getMaxX (), ContentTitle->boundingBox ().getMaxY ()));
			m_CrazyGuanDanNode->addChild (CrazyTexasContent, 4);

			CCSprite * m_carzyTitleImage = CCSprite::create ("TexasPoker/image/TexasPokerActive/crazy_texaspoker_actor.png");
			m_carzyTitleImage->setScale (ui_main_scale);
			m_carzyTitleImage->setPosition (ccp (pkNormal->boundingBox ().getMaxX () - 25 * ui_main_scale - m_carzyTitleImage->getContentSize ().width / 2 * ui_main_scale, pkNormal->boundingBox ().getMidY () - 55 * ui_height_scale));
			m_CrazyGuanDanNode->addChild (m_carzyTitleImage, 4);
		}

		if (m_CrazyPokerButton == NULL)
		{
			CCScale9Sprite * payTipsNormal = CCScale9Sprite::create ("image/mail/mail_confirm.png");
			payTipsNormal->setPreferredSize (CCSizeMake (220.0f, 66.0f));
			CCScale9Sprite * payTipsPress = CCScale9Sprite::create ("image/mail/mail_confirm.png");
			payTipsPress->setPreferredSize (CCSizeMake (220.0f, 66.0f));
			payTipsPress->setColor (g_pkGuanDanGame->GetColorLevelDiv2 (payTipsPress->getColor ()));
			m_CrazyPokerButton = new ODUIButton (payTipsNormal, payTipsPress);
			m_CrazyPokerButton->setScale (ui_main_scale);
			m_CrazyPokerButton->setPosition (ccp (335.0f * ui_width_scale + 935.0f * ui_width_scale / 2, ui_h (1.0f) - 84.0f * ui_height_scale - m_CrazyGuanDanNode->getContentSize ().height - 25 * ui_height_scale - m_CrazyPokerButton->getContentSize ().height / 2 * ui_main_scale));
			UILinkDelegate(m_CrazyPokerButton, OnCrazyGuanDan);
			m_CrazyPokerButton->SetText (TT ("UIText_PT_PayTips_CrazyTexasPoker_GoCrazy"), false, 16);
			AddWidget ("crazy_texaspoker_type_btn", m_CrazyPokerButton, 4);
		}

		HideCrazyGuanDanViews (true);
	}
}