#include "GuanDanAssist.h"
#include "GuanDanTableLayer.h"
#include "LobbyChargeMenu.h"
#include "LobbyPersonalCenter.h"
#include "LobbyBag.h"
#include "LobbyAlipay.h"
#include "LobbyReward.h"
#include "LobbyRewardRecord.h"
#include "LobbySetting.h"
#include "GuanDanLayerHelper.h"


GuanDanRoomChooseButton::~GuanDanRoomChooseButton ()
{
	unschedule (schedule_selector (GuanDanRoomChooseButton::updatePlayerNumbers));
}

GuanDanRoomChooseButton::GuanDanRoomChooseButton(cocos2d::CCNode* pkNormal, cocos2d::CCNode* pkPressed, cocos2d::CCNode* pkDisabled, LobbyKeyControlForSingleLayer * control, int RoomListSize, int TopButtonSize) 
	: ODUIButton (pkNormal, pkPressed, pkDisabled)
{
	m_ContainerLayerControl = control;
	m_RoomListSize = RoomListSize;
	m_TopBtnsSize = TopButtonSize;

	Init ();

	unschedule (schedule_selector (GuanDanRoomChooseButton::updatePlayerNumbers));
	schedule (schedule_selector (GuanDanRoomChooseButton::updatePlayerNumbers), 5.0f);
}


void GuanDanRoomChooseButton::updatePlayerNumbers (ODEFloat32 f32Delta)
{
	//char acBuffer[128] = {0};
	//sprintf (acBuffer, "GD_RoomChoose_ServerId_%d", m_roomType);
	//int numbers = g_pkClientData->getPoepleCountWithRoomId(atoi (GetConfig (acBuffer)));
	//SetPlayerNumber (numbers);
}

void GuanDanRoomChooseButton::Init ()
{
	m_roomType = TP_ROOMTYPE_NONE;
	m_PlayerNumsTTF = NULL;
	m_ScoreLimitedTTF = NULL;
	m_RoomInfoTTF = NULL;
	m_playerNumber = -1;
	ODEZeroMemory (m_baseScore,sizeof (m_baseScore));
}

void GuanDanRoomChooseButton::SetRoomInfo (int playerNumber, char * baseScore, const char * roomInfo)
{
	SetPlayerNumber (playerNumber);
	SetBaseScore (baseScore);
	SetRoomInfo (roomInfo);
}

void GuanDanRoomChooseButton::SetPlayerNumber (int playerNumber)
{
	/*if (m_playerNumber == playerNumber)
	return ;*/
	
	m_playerNumber = playerNumber;

	if (m_PlayerNumsTTF != NULL)
	{
		removeChild (m_PlayerNumsTTF);
		m_PlayerNumsTTF = NULL;
	}

	char playerBuf[64] = {0};

	int randomRate = 0;
	int countBase = 0;

	sprintf (playerBuf, "GD_RoomChoose_PlayerMax_%d", m_roomType);
	int nPlayerMax = atoi (GetConfig (playerBuf));
	sprintf (playerBuf, "GD_RoomChoose_PlayerMin_%d", m_roomType);
	int nPlayerMin = atoi (GetConfig (playerBuf));

	if (strcmp (GetConfig ("GD_PlayerNums_Control"), "1") == 0)
	{
		srand(time(0));
		randomRate = rand() % nPlayerMax;
		countBase = nPlayerMin;
	}
	int realNums = playerNumber;
	int playersNums  =0; //房间人数最终数字
#if (ANDROID_IPTV_CONTROL_USE == 1)
	{
		playersNums = realNums +randomRate  + countBase;
	}
#else
	{
		playersNums = realNums    + countBase + realNums;
	}
#endif //  

	

	if (playersNums > 1000000 || playersNums <= 0)
		return ;

	sprintf (playerBuf, "%d", playersNums);
	m_PlayerNumsTTF = CCLabelAtlas::create(playerBuf,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
	m_PlayerNumsTTF->setAnchorPoint (ccp (0.5f, 0.5f));
	m_PlayerNumsTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMinX () + 165 + m_PlayerNumsTTF->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMaxY () - 345.0f - m_PlayerNumsTTF->getContentSize ().height / 2));
	addChild (m_PlayerNumsTTF, 4);
}

void GuanDanRoomChooseButton::SetBaseScore (char *  baseScore)
{
	if (strcmp (m_baseScore, baseScore) == 0)
		return ;

	strcpy (m_baseScore, baseScore);

	if (m_ScoreLimitedTTF != NULL)
	{
		removeChild (m_ScoreLimitedTTF);
		m_ScoreLimitedTTF = NULL;
	}

	char scoreLimited[128] = {0};
	ODESprintf (scoreLimited, 128 - 1, m_baseScore);
	m_ScoreLimitedTTF = CCLabelAtlas::create(scoreLimited,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');

	m_ScoreLimitedTTF->setAnchorPoint (ccp (0.5f, 0.5f));
	m_ScoreLimitedTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX ()-30, m_pkNormal->boundingBox ().getMidY () - 115.5f - m_ScoreLimitedTTF->getContentSize ().height / 2));
	addChild (m_ScoreLimitedTTF, 4);
}

void GuanDanRoomChooseButton::SetRoomInfo (const char * roomInfo)
{
	if (m_RoomInfoTTF != NULL)
	{
		removeChild (m_RoomInfoTTF);
		m_RoomInfoTTF = NULL;
	}

	char roomInfoBuf[128] = {0};
	sprintf (roomInfoBuf,"%s", roomInfo);


	ODEUInt32 roomLimited = ODEAtoi(roomInfoBuf);
	if (roomLimited>=10000)
	{
		if (roomLimited%10000==0)
		{
			ODEUInt32 limited=roomLimited/10000;
			sprintf (roomInfoBuf,"%d", limited);
		}
		else
		{			
			ODEFloat32  roomLimited_point=ODEFloat32(roomLimited/10000.0-roomLimited/10000);
			int i= ((int)(roomLimited_point * 10)) % 10;
			char TempBuffer[128]={0};
			sprintf(TempBuffer,"%d",roomLimited);

			roomLimited/=10000;
			sprintf (roomInfoBuf,"%d", roomLimited);
			m_RoomInfoTTF = CCLabelAtlas::create(roomInfoBuf,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
			m_RoomInfoTTF->setAnchorPoint (ccp (0.5f, 0.5f));
			m_RoomInfoTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX ()+85, m_pkNormal->boundingBox ().getMidY () - 115.0f - m_RoomInfoTTF->getContentSize ().height / 2));
			addChild (m_RoomInfoTTF, 4);

			CCSprite* point=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_point.png");
			point->setPosition(ccp(m_pkNormal->boundingBox ().getMidX ()+95, m_pkNormal->boundingBox ().getMidY () - 120.0f - m_RoomInfoTTF->getContentSize ().height / 2));
			addChild (point, 4);

			sprintf(TempBuffer,"%d",i);
			CCLabelAtlas*m_RoomInfoTTF1 = CCLabelAtlas::create(TempBuffer,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
			m_RoomInfoTTF1->setAnchorPoint (ccp (0.5f, 0.5f));
			m_RoomInfoTTF1->setPosition (ccp (m_pkNormal->boundingBox ().getMidX ()+105, m_pkNormal->boundingBox ().getMidY () - 115.0f - m_RoomInfoTTF1->getContentSize ().height / 2));
			addChild (m_RoomInfoTTF1, 4);

			CCSprite* pWanText=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_wan_text.png");
			pWanText->setPosition(ccp(m_pkNormal->boundingBox ().getMidX ()+130, m_pkNormal->boundingBox ().getMidY () - 115.0f - m_RoomInfoTTF->getContentSize ().height / 2));
			addChild(pWanText,4);
			return;
		}
	}
	else if (roomLimited>=1000)
	{
		if (roomLimited%1000==0)
		{
			ODEUInt32 limited=roomLimited/1000;
			sprintf (roomInfoBuf,"%d", limited);
		}
		else
		{		
			ODEFloat32  roomLimited_point=ODEFloat32(roomLimited/1000.0-roomLimited/1000);
			int i= ((int)(roomLimited_point * 10)) % 10;
			char TempBuffer[128]={0};
			sprintf(TempBuffer,"%d",roomLimited);

			roomLimited/=1000;
			sprintf (roomInfoBuf,"%d", roomLimited);
			m_RoomInfoTTF = CCLabelAtlas::create(roomInfoBuf,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
			m_RoomInfoTTF->setAnchorPoint (ccp (0.5f, 0.5f));
			m_RoomInfoTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX ()+85, m_pkNormal->boundingBox ().getMidY () - 115.0f - m_RoomInfoTTF->getContentSize ().height / 2));
			addChild (m_RoomInfoTTF, 4);

			CCSprite* point=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_point.png");
			point->setPosition(ccp(m_pkNormal->boundingBox ().getMidX ()+95, m_pkNormal->boundingBox ().getMidY () - 120.0f - m_RoomInfoTTF->getContentSize ().height / 2));
			addChild (point, 4);

			sprintf(TempBuffer,"%d",i);
			CCLabelAtlas*m_RoomInfoTTF1 = CCLabelAtlas::create(TempBuffer,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
			m_RoomInfoTTF1->setAnchorPoint (ccp (0.5f, 0.5f));
			m_RoomInfoTTF1->setPosition (ccp (m_pkNormal->boundingBox ().getMidX ()+105, m_pkNormal->boundingBox ().getMidY () - 115.0f - m_RoomInfoTTF1->getContentSize ().height / 2));
			addChild (m_RoomInfoTTF1, 4);

			CCSprite* pQianText=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_qian_text.png");
			pQianText->setPosition(ccp(m_pkNormal->boundingBox ().getMidX ()+130, m_pkNormal->boundingBox ().getMidY () - 115.0f - m_RoomInfoTTF->getContentSize ().height / 2));
			addChild(pQianText,4);
			return;
		}
	}

	m_RoomInfoTTF = CCLabelAtlas::create(roomInfoBuf,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
	m_RoomInfoTTF->setAnchorPoint (ccp (0.5f, 0.5f));
	if (roomLimited<1000)
	{
		m_RoomInfoTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX ()+100, m_pkNormal->boundingBox ().getMidY () - 115.0f - m_RoomInfoTTF->getContentSize ().height / 2));
	}
	else
	{
		m_RoomInfoTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX ()+95, m_pkNormal->boundingBox ().getMidY () - 115.0f - m_RoomInfoTTF->getContentSize ().height / 2));
	}
	addChild (m_RoomInfoTTF, 4);
	if (roomLimited>=10000)
	{
		CCSprite* pWanText=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_wan_text.png");
		pWanText->setPosition(ccp(m_pkNormal->boundingBox ().getMidX ()+130, m_pkNormal->boundingBox ().getMidY () - 115.0f - m_RoomInfoTTF->getContentSize ().height / 2));
		addChild(pWanText,4);
	}
	else if (roomLimited>=1000)
	{
		CCSprite* pQianText=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_qian_text.png");
		pQianText->setPosition(ccp(m_pkNormal->boundingBox ().getMidX ()+130, m_pkNormal->boundingBox ().getMidY () - 115.0f - m_RoomInfoTTF->getContentSize ().height / 2));
		addChild(pQianText,4);
	}
}

void GuanDanRoomChooseButton::SetRoomChooseType (GuanDanRoomChooseType roomType)
{
	m_roomType = roomType;

#if (ANDROID_IPTV_CONTROL_USE == 1)
	CCScale9Sprite * nameEditTips = CCScale9Sprite::create ("image/keyController.png", CCRect (0, 0, 44, 42), CCRect(21, 20, 2, 2) );
	nameEditTips->setPreferredSize (CCSizeMake(325, 385));
	addChild (nameEditTips, 3);
	nameEditTips->setPosition (ccp (0.0f, 2.5f));
	nameEditTips->setVisible (false);
	KeyControlUnion * controlUnion0 = new KeyControlUnion  ();
	controlUnion0->curSelFlags = nameEditTips;
	controlUnion0->curPos = roomType + m_TopBtnsSize;
	controlUnion0->upPos = 0;
	controlUnion0->leftPos =  (roomType + m_TopBtnsSize - 1)  < TP_ROOMTYPE_NEW + m_TopBtnsSize ? TP_ROOMTYPE_NEW + m_TopBtnsSize : (roomType - 1) + m_TopBtnsSize ;
	controlUnion0->downPos = m_RoomListSize + m_TopBtnsSize;
	controlUnion0->rightPos = (roomType + 1 + m_TopBtnsSize)  >= TP_ROOMTYPE_MAX + m_TopBtnsSize ? TP_ROOMTYPE_MASTER + m_TopBtnsSize : (roomType + 1 + m_TopBtnsSize);
	controlUnion0->setControlCallback (this, keyControl_selector (GuanDanRoomChooseButton::OnEvent), this);
	m_ContainerLayerControl->addKeyControlUnion (controlUnion0);
#endif
}

void GuanDanRoomChooseButton::OnEvent(ODUIWidget& kWidget, ODEUInt32 u32State)
{
	if(u32State == RELEASED)
	{
		if (! IsNetwork())
		{
			g_pkGuanDanGame->Toast(TT("UIText_Net_Disconnect_Connect"));
			return;
		}
		//g_pkClientData->setIsTuanTuan(false);
		//g_pkClientData->setIsMatchGame_GD(false);
 		ODEInt64 userScore = g_pkClientData->GetUserScore ();
		char limitedBuf[128] = {0};
		sprintf (limitedBuf, "GD_RoomChoose_LimitedSocre_%d", m_roomType);
		ODEInt64 limitedScore = atoi (GetConfig (limitedBuf));
		
		sprintf (limitedBuf, "GD_RoomChoose_MaxSocre_%d", m_roomType);
		ODEInt64 maxScore = atoi (GetConfig (limitedBuf));

		if ((limitedScore == maxScore && maxScore == 0) || (userScore >= limitedScore && maxScore == 0 || (userScore >= limitedScore && userScore < maxScore)))
		{
			GuanDanTableLayer* pLayerTable = (GuanDanTableLayer*)g_pLayerManager->GetLayer("GuanDan_TableLayer");
			if (pLayerTable != NULL)
			{
				pLayerTable->OnActiveFromLogin(true, m_roomType);
			}

			char roomLevel[64] = {0};
			sprintf(roomLevel, "GD_RoomChoose_RoomType_%d", m_roomType);
			g_pkClientData->setRoomIndex(atoi(GetConfig(roomLevel)));		

			GuanDanRoomChoice* pLayerRoomChoose = (GuanDanRoomChoice*)g_pLayerManager->GetLayer("GuanDan_RoomChoice");
			if (pLayerRoomChoose != NULL)
			{
				pLayerRoomChoose->EnableAllRoomBtn(false);
				if (m_roomType!=3)
				{				
					pLayerRoomChoose->changeSocketToLobby(false,m_roomType);
				}
				
			}
		}  
		else if (userScore < limitedScore || userScore >= maxScore)
		{
			if ( m_roomType==3)
			{
				return;
			}	


			if (!g_pLayerManager->IsExitInLayer("GuanDan_RechargeGuide"))
			{
				g_pLayerManager->PushLayer("GuanDan_RechargeGuide");
			}

		//	g_pLayerManager->PushLayer("alert", ODE_NEW GuanDanGamePayCallback);
			return;
		}
	}
}

// 大厅游戏列表大图标
GuanDanRoomChooseList::GuanDanRoomChooseList ()
	:RoomList()
{
	
}

GuanDanRoomChooseList::~GuanDanRoomChooseList ()
{
}

ODEUInt32 GuanDanRoomChooseList::AddRoomBtn(GuanDanRoomChooseButton * buttonAdd,int num)
{
	ODEUInt32 u32Res = m_kButtonArray.Size();
	GuanDanRoomChooseButton * pkEvent = buttonAdd;
	pkEvent->setScale(ui_main_scale);
	ODEUInt32 u32Row = u32Res;
	pkEvent->SetIndex(u32Res);
	pkEvent->setPosition(ccp(ODEFloat32(u32Row) * 370.0f * ui_width_scale, 195.0f * ui_height_scale));
	AddChild(pkEvent);
	m_kButtonArray.Resize(m_kButtonArray.Size() + 1);
	Item& kItem = m_kButtonArray.Back();
	kItem.m_pkButton = pkEvent;
	kItem.m_kDelegate.Set(this, &GuanDanRoomChooseList::OnClicked);
	kItem.m_pkButton->RegisterListener(kItem.m_kDelegate);
	if (num >=4)
	{
		pkEvent->setPosition(ccp(ODEFloat32(u32Row) * 300.0f * ui_width_scale, 195.0f * ui_height_scale));
		pkEvent->setScale(0.9*ui_main_scale);
	}
	return u32Res;
}

void GuanDanRoomChooseList::EnableAllRoomBtn (bool enable)
{
	for (ODEVector<Item>::iterator it = m_kButtonArray.Begin(); it != m_kButtonArray.End(); ++it)
	{
		(it)->m_pkButton->SetEnable(enable);
	}
}

void GuanDanRoomChooseList::RemoveAllBtn ()
{
	for (int i = 0; i < m_kButtonArray.Size (); i++)
	{
		removeChild (m_kButtonArray.At (i).m_pkButton);
		CC_SAFE_DELETE(m_kButtonArray.At (i).m_pkButton);
		m_kButtonArray.At (i).m_pkButton = NULL;
	}
	m_kButtonArray.Clear ();
}


ODEUInt32 GuanDanRoomChooseList::GetListSize ()
{
	return m_kButtonArray.Size ();
}

void GuanDanRoomChooseList::ReFreshAllRoomInfo()
{
	//刷新所有房间的人数
	if (m_kButtonArray.Size() >1)
	{
		for (int i = 0; i < m_kButtonArray.Size () -1; i++)
		{
			int iGameID = atoi(TT("GameID"));
			GuanDanRoomChooseButton* l_roombtn = (GuanDanRoomChooseButton*)m_kButtonArray.At (i).m_pkButton;
			int l_realroomnum = g_pkLobbyConnectData->getGamePlayersGD(iGameID, (i+1) );
			l_roombtn->SetPlayerNumber(l_realroomnum);
		}
	}
	
}

//掼蛋团团转场房间选择界面
GD_TuanTuanGameRoomList::GD_TuanTuanGameRoomList()
	: ODUIScrollViewH(1230.0f * ui_width_scale , 400.0f * ui_height_scale,
	"scroll_bean.png", "scroll_bean_ext.png", 3.0f * g_f32ScaleHD * ui_main_scale)
{
	setAnchorPoint(ccp(0.5f,1.0f));
	UpdateBounding();
	m_kButtonArray.Reserve(128);
}

GD_TuanTuanGameRoomList::~GD_TuanTuanGameRoomList()
{

}

ODEUInt32 GD_TuanTuanGameRoomList::AddRoomBtn(int roomTypeIndex, int maxRoomCount, ODEUInt64 u64Limit, ODEUInt64 u64LimitMax, ODEUInt64 u64BaseBet, int peopleCount)
{
	ODEUInt32 u32Res = m_kButtonArray.Size();

	CCSprite* p_Normal=CCSprite::create("image/InvisibleBtn.png");
	p_Normal->setContentSize(CCSizeMake(328.0f, 397.0f));
	p_Normal->setOpacity(0);
	CCSprite* p_Press=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_room_press.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(GD_IMAGE_PATH_ROOM"gd_room_press.png");

	ODUIButton* pkEvent = NULL;
	pkEvent = ODE_NEW ODUIButton(p_Normal,p_Press);
	pkEvent->setScale(ui_main_scale);
	if (maxRoomCount>3)
	{
		pkEvent->setScale(ui_main_scale * 0.9f);
	}	
	pkEvent->SetIndex(u32Res);
	
	switch (maxRoomCount) {
	case 1:
		pkEvent->setPosition(ccp(610.0f * ui_width_scale, 205.0f * ui_height_scale));
		break;
	case 2:
		pkEvent->setPosition(ccp(u32Res * 502.0f * ui_width_scale + 354.0f * ui_width_scale, 205.0f * ui_height_scale));
		break;
	case 3:
		pkEvent->setPosition(ccp(u32Res * 410.0f * ui_width_scale + 214.0f * ui_width_scale, 205.0f * ui_height_scale));
		break;
	case 4:
	default:
		pkEvent->setPosition(ccp(u32Res * 308.0f * ui_width_scale + 154.0f * ui_width_scale, 205.0f * ui_height_scale));
		break;
	}
	AddChild(pkEvent);
	m_kButtonArray.Resize(m_kButtonArray.Size() + 1);
	Item& kItem = m_kButtonArray.Back();
	kItem.m_pkButton = pkEvent;
	kItem.m_kDelegate.Set(this, &GD_TuanTuanGameRoomList::OnClicked);
	kItem.m_pkButton->RegisterListener(kItem.m_kDelegate);
	kItem.m_LobbyRoomInfo.m_u64LimitScore = u64Limit;
	kItem.m_LobbyRoomInfo.m_u64BaseScore = u64BaseBet;
	kItem.m_LobbyRoomInfo.m_indexInConfig = roomTypeIndex;

	char imageBuffer[60]={0};
	sprintf(imageBuffer,GD_IMAGE_PATH_ROOM"gd_room_%d.png",roomTypeIndex);
	CCSprite* RoomImage=CCSprite::create(imageBuffer);
	pkEvent->addChild(RoomImage,2);
	if (roomTypeIndex==0 || roomTypeIndex==3)
	{
		RoomImage->setPositionY(5);
	}

	//添加底注，积分限制以及在线人数信息
	char limitBuffer[60]={0};
	char BaseScoreBuff[60]={0};
	char peopleBuff[60]={0};
	if (roomTypeIndex!=3)
	{
		sprintf(BaseScoreBuff,"%lld",u64BaseBet);
		m_BaseBetNumber=CCLabelAtlas::create(BaseScoreBuff,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
		m_BaseBetNumber->setPosition(ccp(-30,-127));
		m_BaseBetNumber->setAnchorPoint(ccp(0.5,0.5));
		pkEvent->addChild(m_BaseBetNumber,13);

		sprintf(peopleBuff,"%d",peopleCount);
		m_PeopleNumber=CCLabelAtlas::create(peopleBuff,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
		m_PeopleNumber->setPosition(ccp(40,-157));
		m_PeopleNumber->setAnchorPoint(ccp(0.5,0.5));
		pkEvent->addChild(m_PeopleNumber,13);

		sprintf(limitBuffer,"%lld",u64Limit);
	
		if (u64Limit>=10000)
		{
			if (u64Limit%10000==0)
			{
				ODEUInt32 limited=u64Limit/10000;
				sprintf (limitBuffer,"%d", limited);
			}
			else
			{			
				ODEFloat32  roomLimited_point=ODEFloat32(u64Limit/10000.0-u64Limit/10000);
				int i= ((int)(roomLimited_point * 10)) % 10;
				char TempBuffer[128]={0};
				sprintf(TempBuffer,"%d",u64Limit);

				u64Limit/=10000;
				sprintf (limitBuffer,"%d", u64Limit);
				m_limitNumber = CCLabelAtlas::create(limitBuffer,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
				m_limitNumber->setAnchorPoint (ccp (0.5f, 0.5f));
				m_limitNumber->setPosition (ccp (80,-125));
				pkEvent->addChild (m_limitNumber, 13);

				CCSprite* point=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_point.png");
				point->setPosition(ccp(m_limitNumber->boundingBox ().getMaxX (), m_limitNumber->boundingBox ().getMidY ()-5));
				pkEvent->addChild (point, 13);

				sprintf(TempBuffer,"%d",i);
				CCLabelAtlas*m_RoomInfoTTF1 = CCLabelAtlas::create(TempBuffer,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
				m_RoomInfoTTF1->setAnchorPoint (ccp (0.5f, 0.5f));
				m_RoomInfoTTF1->setPosition (ccp (m_limitNumber->boundingBox ().getMaxX ()+10, m_limitNumber->boundingBox ().getMidY ()));
				pkEvent->addChild (m_RoomInfoTTF1, 13);

				CCSprite* pWanText=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_wan_text.png");
				pWanText->setPosition(ccp(m_limitNumber->boundingBox ().getMaxX ()+30, m_limitNumber->boundingBox ().getMidY () ));
				pkEvent->addChild(pWanText,13);
				return u32Res;
			}
		}
		else if (u64Limit>=1000)
		{
			if (u64Limit%1000==0)
			{
				ODEUInt32 limited=u64Limit/1000;
				sprintf (limitBuffer,"%d", limited);
			}
			else
			{		
				ODEFloat32  roomLimited_point=ODEFloat32(u64Limit/1000.0-u64Limit/1000);
				int i= ((int)(roomLimited_point * 10)) % 10;
				char TempBuffer[128]={0};
				sprintf(TempBuffer,"%d",u64Limit);

				u64Limit/=1000;
				sprintf (limitBuffer,"%d", u64Limit);
				m_limitNumber = CCLabelAtlas::create(limitBuffer,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
				m_limitNumber->setAnchorPoint (ccp (0.5f, 0.5f));
				m_limitNumber->setPosition (ccp (80,-125));
				pkEvent->addChild (m_limitNumber,13);

				CCSprite* point=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_point.png");
				point->setPosition(ccp(m_limitNumber->boundingBox ().getMaxX (), m_limitNumber->boundingBox ().getMidY ()-5));
				pkEvent->addChild (point,13);

				sprintf(TempBuffer,"%d",i);
				CCLabelAtlas*m_RoomInfoTTF1 = CCLabelAtlas::create(TempBuffer,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
				m_RoomInfoTTF1->setAnchorPoint (ccp (0.5f, 0.5f));
				m_RoomInfoTTF1->setPosition (ccp (m_limitNumber->boundingBox ().getMaxX ()+10, m_limitNumber->boundingBox ().getMidY ()));
				pkEvent->addChild (m_RoomInfoTTF1, 13);

				CCSprite* pQianText=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_qian_text.png");
				pQianText->setPosition(ccp(m_limitNumber->boundingBox ().getMaxX ()+30, m_limitNumber->boundingBox ().getMidY ()));
				pkEvent->addChild(pQianText,13);
				return u32Res;
			}
		}

		m_limitNumber = CCLabelAtlas::create(limitBuffer,GD_IMAGE_PATH_ROOM"gd_online_num.png",23,24,'0');
		m_limitNumber->setAnchorPoint (ccp (0.5f, 0.5f));
		if (u64Limit<1000)
		{
			m_limitNumber->setPosition (ccp (100,-125));
		}
		else
		{
			m_limitNumber->setPosition (ccp (85,-125));
		}
		pkEvent->addChild (m_limitNumber,13);

		if (u64Limit>=10000)
		{
			CCSprite* pWanText=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_wan_text.png");
			pWanText->setPosition(ccp(m_limitNumber->boundingBox ().getMaxX ()+20, m_limitNumber->boundingBox ().getMidY ()));
			pkEvent->addChild(pWanText,13);
		}
		else if (u64Limit>=1000)
		{
			CCSprite* pQianText=CCSprite::create(GD_IMAGE_PATH_ROOM"gd_qian_text.png");
			pQianText->setPosition(ccp(m_limitNumber->boundingBox ().getMaxX ()+20, m_limitNumber->boundingBox ().getMidY ()));
			pkEvent->addChild(pQianText,13);
		}
	}
	return u32Res;
}

void GD_TuanTuanGameRoomList::ClearAll()
{
	for (ODEVector<Item>::iterator it = m_kButtonArray.Begin(); it != m_kButtonArray.End(); ++it)
	{
		it->m_pkButton->SetVisible(false);
	}
	m_kButtonArray.Clear();
}

void GD_TuanTuanGameRoomList::OnClicked( ODUIWidget& kWidget, ODEUInt32 u32State )
{
	if(u32State == RELEASED)
	{
		ODEUInt32 u32Index = ((ODUIButton&)kWidget).GetIndex();
		ODEUInt32 u32State = u32Index + 1;
		u32State |= 0x80000000;
		m_kEvent.Callback(*this, u32State);
	}
}

GD_TuanTuanGameRoomList::LobbyRoomInfo GD_TuanTuanGameRoomList::GetRoomInfo( ODEUInt32 u32Index )
{
	return m_kButtonArray[u32Index-1].m_LobbyRoomInfo;
}

void GD_TuanTuanGameRoomList::RefreshList()
{
	ClearAll();
	ODEInt32 i32RoomCount = ODEAtoi(GetConfig("GD_TuanTuanRoomCount"));

	char roomIndexBuffer[50] = {0};
	for (ODEInt32 i(0); i < i32RoomCount; ++i)
	{
		ODEChar8 acBuffer[64];

		memset(roomIndexBuffer, 0, sizeof(roomIndexBuffer));
		sprintf(roomIndexBuffer, "GD_TuanSortRoomListIndex_%d", i);
		int currIndex = atoi(GetConfig(roomIndexBuffer)) - 1;

		ODESprintf(acBuffer, 64, "%s%d", "GD_TuanRoomScoreLimit_", currIndex);
		ODEUInt32 u64Limit = ODEAtoi(GetConfig(acBuffer));
		ODESprintf(acBuffer, 64, "%s%d", "GD_TuanRoomScoreLimitMax_", currIndex);
		ODEUInt32 u64LimitMax = ODEAtoi(GetConfig(acBuffer));
		ODESprintf(acBuffer, 64, "%s%d", "GD_TuanRoomBaseScore_", currIndex);
		ODEUInt32 u64Base = ODEAtoi(GetConfig(acBuffer));
		ODESprintf(acBuffer, 64, "%s%d", "GD_TuanRoomServerId_", currIndex);
		int serverId = ODEAtoi(GetConfig(acBuffer));
		ODESprintf(acBuffer, 64, "%s%d", "GD_TuanRoomBasePeopleCount_", currIndex);
		int basePeopleCount = ODEAtoi(GetConfig(acBuffer));
		ODESprintf(acBuffer, 64, "%s%d", "GD_TuanRoomBasePeopleRandCount_", currIndex);
		int randPeopleCount = ODEAtoi(GetConfig(acBuffer));

		//basePeopleCount += rand() % randPeopleCount;
		if (randPeopleCount != 0)
		{
			basePeopleCount += rand() % randPeopleCount;
		}

		AddRoomBtn(currIndex, i32RoomCount, u64Limit, u64LimitMax, u64Base, basePeopleCount + g_pkClientData->getPoepleCountWithRoomId(serverId));
	}
}

GD_TuanTuanGameRoomList::Item::Item()
{
	m_pkButton = NULL;
}

//掼蛋比赛场房间相关界面
GD_MatchRoomBtn::GD_MatchRoomBtn(long long id, string name, int playerCount, int paramA, int paramC, string firstAward, int sortid)
	:m_roomId(id), m_iParamA(paramA), m_iRealCount(playerCount)
{
	m_iParamC = (paramC == 0) ? 1 : paramC;

	CCSprite* pBtnN = CCSprite::create(GD_IMAGE_PATH_MATCH"gd_match_panelbutton.png");
	CCSprite* pBtnP = CCSprite::create(GD_IMAGE_PATH_MATCH"gd_match_panelbutton.png");
	pBtnP->setColor(ColorLevelDiv2(pBtnP->getColor()));
	CCSprite* pBtnD = CCSprite::create(GD_IMAGE_PATH_MATCH"gd_match_panelbutton.png");

	m_pkTouchItemChairL = CCMenuItemSprite::create(pBtnN, pBtnP, pBtnD, this, menu_selector(GD_MatchRoomBtn::menuCallback));
	ScrollViewMenu* pkMenu = ScrollViewMenu::create(m_pkTouchItemChairL, NULL);
	pkMenu->setPosition(ccp(0, 0));
	addChild(pkMenu);

	CCLabelTTF* pRoomName = CCLabelTTF::create(name.c_str(), TTF_NAME, 36, CCSizeMake(365, 48), kCCTextAlignmentLeft);
	pRoomName->setAnchorPoint(ccp(0, 0.5));
	pRoomName->setPosition(ccp(-240, 67));
	pRoomName->setColor(ccc3(255, 195, 9));
	addChild(pRoomName, 5);	

	char numberBuffer[1024] = {0};
	memset(numberBuffer, 0, 50);
	sprintf(numberBuffer, "%lld", id);

	m_pMatchId = CCLabelAtlas::create(numberBuffer, GD_IMAGE_PATH_ROOM"gd_online_num.png", 23, 24, '0');
	m_pMatchId->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pMatchId->setPosition(ccp(103.0f, -50.0f));
	m_pMatchId->setVisible(false);
	addChild(m_pMatchId, 4);

	memset(numberBuffer, 0, 50);
	sprintf(numberBuffer, "%d", sortid);

	CCLabelAtlas* pSortId = CCLabelAtlas::create(numberBuffer, GD_IMAGE_PATH_ROOM"gd_online_num.png", 23, 24, '0');
	pSortId->setAnchorPoint(ccp(0.0f, 0.0f));
	pSortId->setPosition(ccp(103.0f, -80.0f));
	pSortId->setVisible(false);
	addChild(pSortId, 4);

	CCScale9Sprite* pAwardNameBg = CCScale9Sprite::create("image/match/competitionwordbottom.png");
	pAwardNameBg->setPreferredSize(CCSizeMake(282, 37));
	pAwardNameBg->setAnchorPoint(ccp(0, 0.5));
	pAwardNameBg->setPosition(ccp(-202, 8));
	addChild(pAwardNameBg);

	CCLabelTTF* pAwardNameTitle = CCLabelTTF::create(TT("MatchGame_FirstAward"), TTF_NAME, 24);
	pAwardNameTitle->setAnchorPoint(ccp(0, 0.5));
	pAwardNameTitle->setPosition(ccp(pAwardNameBg->getPositionX() + 13, pAwardNameBg->getPositionY()));
	pAwardNameTitle->setColor(ccc3(254, 89, 24));
	addChild(pAwardNameTitle, 5);

	string strAwardName = "";
	int iNum = 0;

	for (int i = 0; i < strlen(firstAward.c_str());)
	{
		if (firstAward[i] >= 0 && firstAward[i] <= 127) 
		{  
			if (firstAward[i] == 9)
			{
				i ++;
				continue;
			}

			string englishStr = firstAward.substr(i,1).c_str();
			strAwardName += englishStr;
			i += 1;  
		}  
		else
		{  
			string chineseStr = firstAward.substr(i,3).c_str();
			strAwardName += chineseStr;
			i += 3;  
		}  

		if (i >= 12)
		{
			break;
		}
	}

	CCLabelTTF* pAwardName = CCLabelTTF::create(strAwardName.c_str(), TTF_NAME, 24, CCSizeMake(153, 30), kCCTextAlignmentLeft);
	pAwardName->setAnchorPoint(ccp(0, 0.5));
	pAwardName->setPosition(ccp(pAwardNameTitle->boundingBox().getMaxX(), pAwardNameBg->getPositionY()));
	pAwardName->setColor(ccc3(254, 89, 24));
	addChild(pAwardName, 5);	

	CCScale9Sprite* pUserCountBg = CCScale9Sprite::create("image/match/competitionwordbottom.png");
	pUserCountBg->setPreferredSize(CCSizeMake(282, 37));
	pUserCountBg->setAnchorPoint(ccp(0, 0.5));
	pUserCountBg->setPosition(ccp(pAwardNameBg->getPositionX(), pAwardNameBg->getPositionY() - 43));
	addChild(pUserCountBg);

	CCLabelTTF* pUserCountTitle = CCLabelTTF::create(TT("MatchGame_PlayerCount"), TTF_NAME, 24);
	pUserCountTitle->setAnchorPoint(ccp(0, 0.5));
	pUserCountTitle->setPosition(ccp(pUserCountBg->getPositionX() + 13, pUserCountBg->getPositionY()));
	pUserCountTitle->setColor(ccc3(254, 89, 24));
	addChild(pUserCountTitle, 5);

	int iRandParamC = rand() % m_iParamC + 1;
	int iFakePlayerCount = paramA * (1 + iRandParamC / 100.0f) + playerCount * (iRandParamC / 100.0f) + playerCount;
	sprintf(numberBuffer, "%d", iFakePlayerCount);	

	m_pUserCount = CCLabelTTF::create(numberBuffer, TTF_NAME, 24);
	m_pUserCount->setAnchorPoint(ccp(0, 0.5));
	m_pUserCount->setPosition(ccp(pUserCountTitle->boundingBox().getMaxX(), pUserCountTitle->getPositionY()));
	m_pUserCount->setColor(ccc3(254, 89, 24));
	addChild(m_pUserCount, 5);

	ODEFloat32 fDt = rand() % 5 + 5;

	schedule(schedule_selector(GD_MatchRoomBtn::UpdatePlayerCount), fDt);

	CCTextureCache::sharedTextureCache()->removeTextureForKey("image/room_score_num.png");
}

void GD_MatchRoomBtn::UpdatePlayerCount(ODEFloat32 dt)
{
	char numberBuffer[1024] = {0};
	int iRandParamC = rand() % m_iParamC + 1;
	int iFakePlayerCount = m_iParamA * (1 + iRandParamC / 100.0f) + m_iRealCount * (iRandParamC / 100.0f) + m_iRealCount;
	sprintf(numberBuffer, "%d", iFakePlayerCount);

	m_pUserCount->setString(numberBuffer);
}

GD_MatchRoomBtn::~GD_MatchRoomBtn()
{
	unscheduleAllSelectors();
}

void GD_MatchRoomBtn::menuCallback( CCObject* pkObj )
{
	//if (! g_pkNetworkHelper->GetMatchServerDelegate() || !g_pkNetworkHelper->GetMatchServerDelegate()->IsConnect())
	//{
	//	g_pkGame->ShowLoading(false);
	//	g_pkGame->startCheckSocket(SOCKETINDEX_MATCH);
	//	g_pkNetworkHelper->ConnectMatchServer();
	//	return;
	//}

	////请求本比赛房的详细信息
	//CSGetMatchInfoReq req;
	//req.id = m_roomId;

	//if (g_pkNetworkHelper->GetMatchServerDelegate())
	//{
	//	g_pkNetworkHelper->GetMatchServerDelegate()->SendMsgToSocket2(req, CS_GET_MATCH_INFO_REQ);
	//}
}

void GD_MatchRoomBtn::SetTouchEnable( bool bEnable )
{
	m_pkTouchItemChairL->setEnabled(bEnable);
}

void GD_MatchRoomBtn::SetMatchRoomId(long long newId)
{
	m_roomId = newId;

	char numberBuffer[1024] = {0};
	sprintf(numberBuffer, "%lld", newId);

	m_pMatchId->setString(numberBuffer);
}

long long GD_MatchRoomBtn::GetMatchRoomId()
{
	return m_roomId;
}


GD_MatchGameRoomList::GD_MatchGameRoomList(ODEInt32 iRows, ODEInt32 iCols)
	: m_iNumRow(iRows), m_iNumCol(iCols), m_fWidth(1194), m_fHeight(400), m_bIsScrolling(false)
{
	ODEInt32 i32RoomCount = 1;
	m_iPageNodeNum = 0;

	if (i32RoomCount <= (m_iNumRow * m_iNumCol))
	{
		m_iPageNodeNum = 1;
	}
	else if (i32RoomCount % (m_iNumRow * m_iNumCol) == 0)
	{
		m_iPageNodeNum = i32RoomCount / (m_iNumRow * m_iNumCol);
	}
	else
	{
		m_iPageNodeNum = i32RoomCount / (m_iNumRow * m_iNumCol) + 1;
	}

	m_ScrollView = GD_MatchGameRoomScrollView::create();
	m_ScrollView->setDirection(kCCScrollViewDirectionHorizontal);
	m_ScrollView->CreateContainer(this, m_iPageNodeNum, CCSizeMake(m_fWidth * ui_width_scale, m_fHeight * ui_height_scale));
	m_ScrollView->setPosition(ccp(-m_fWidth / 2 * ui_width_scale + 80 * ui_width_scale, -m_fHeight * ui_height_scale));
	m_ScrollView->setViewSize(CCSizeMake(1126 * ui_width_scale, m_fHeight * ui_height_scale));
	addChild(m_ScrollView);
}

GD_MatchGameRoomList::~GD_MatchGameRoomList()
{
}

void GD_MatchGameRoomList::scrollViewDidScroll( cocos2d::extension::CCScrollView *view )
{
	SetAllMatchRoomBtnEnable(false);
}

void GD_MatchGameRoomList::scrollViewDidZoom( cocos2d::extension::CCScrollView *view )
{

}

bool GD_MatchGameRoomList::scrollViewInitPage( cocos2d::CCNode *pScroll, cocos2d::CCNode *pPage, int nPage )
{
	/*std::vector<tagMatchSimpleInfo> vecCurrPageMatchSimpleInfos = g_pkClientData->GetCurrPageMatchSimpleInfos(m_iNumRow, m_iNumCol, nPage);

	int iMatchRoomIndex = 0;

	ODEFloat32 fDisX = 598* ui_width_scale;
	ODEFloat32 fDisY = 185* ui_height_scale;
	ODEFloat32 fBtnWidth = 508 * ui_width_scale;
	ODEFloat32 fDisBorderLeft = (m_fWidth * ui_width_scale - (m_iNumCol - 1) * fDisX - fBtnWidth) / 2 - 350 * ui_width_scale;
	ODEFloat32 fDisBorderTop = -100 * ui_height_scale + 35 * ui_height_scale;

	for (int j(0); j < m_iNumRow; ++j)
	{
		for (int i(0); i < m_iNumCol; ++i)
		{
			if (iMatchRoomIndex >= vecCurrPageMatchSimpleInfos.size())
			{
			return true;
			}

			tagMatchSimpleInfo info = vecCurrPageMatchSimpleInfos.at(iMatchRoomIndex);

			GD_MatchRoomBtn* pMatchRoom = new GD_MatchRoomBtn(info.id, info.name, info.userCount, info.paramA, info.paramC, info.firstAward.awardDesc, info.sortId);			
			pMatchRoom->autorelease();
			pMatchRoom->setPosition((ccp(fDisBorderLeft + i * fDisX, fDisBorderTop - j * fDisY)));
			pMatchRoom->setTag(nPage * (m_iNumCol * m_iNumRow) + iMatchRoomIndex);
			pMatchRoom->setScale(ui_main_scale);
			pPage->addChild(pMatchRoom);

			iMatchRoomIndex++;
		}
	}*/

	return true;
}

void GD_MatchGameRoomList::scrollViewClick( const cocos2d::CCPoint &oOffset, const cocos2d::CCPoint &oPoint , cocos2d::CCNode *pPage, int nPage )
{

}

void GD_MatchGameRoomList::scrollViewScrollEnd( cocos2d::CCNode *pPage, int nPage )
{
	SetAllMatchRoomBtnEnable(true);
	m_bIsScrolling = false;
}

void GD_MatchGameRoomList::SetTouchActive( bool bIsActive )
{
	m_ScrollView->setTouchEnabled(bIsActive);
}

void GD_MatchGameRoomList::RefreshRoomList()
{
	m_ScrollView->removeFromParentAndCleanup(true);

	//std::vector<tagMatchSimpleInfo*>* vecAllMatchSimpleInfos = g_pkClientData->GetAllMatchSimpleInfos();

	//ODEInt32 i32RoomCount = vecAllMatchSimpleInfos->size();

	/*if (i32RoomCount <= (m_iNumRow * m_iNumCol))
	{
		m_iPageNodeNum = 1;
	}
	else if (i32RoomCount % (m_iNumRow * m_iNumCol) == 0)
	{
		m_iPageNodeNum = i32RoomCount / (m_iNumRow * m_iNumCol);
	}
	else
	{
		m_iPageNodeNum = i32RoomCount / (m_iNumRow * m_iNumCol) + 1;
	}*/

	m_ScrollView = GD_MatchGameRoomScrollView::create();
	m_ScrollView->setDirection(kCCScrollViewDirectionHorizontal);
	m_ScrollView->CreateContainer(this, m_iPageNodeNum, CCSizeMake(m_fWidth * ui_width_scale, m_fHeight * ui_height_scale));
	m_ScrollView->setPosition(ccp(-m_fWidth / 2 * ui_width_scale + 30 * ui_width_scale, -m_fHeight * ui_height_scale));
	m_ScrollView->setViewSize(CCSizeMake(1126* ui_width_scale, m_fHeight * ui_height_scale));
	addChild(m_ScrollView);

	SetAllMatchRoomBtnEnable(true);

	if (m_iPageNodeNum == 1)
	{
		SetTouchActive(false);
	}
	else
	{
		SetTouchActive(true);
	}
}

void GD_MatchGameRoomList::SetAllMatchRoomBtnEnable( bool bEnable )
{
	for (int i = 0; i < m_iPageNodeNum; ++i)
	{
		for (int j(0); j < (m_iNumCol * m_iNumRow); ++j)
		{
			CCNode* pkPage = m_ScrollView->GetPage(i);

			if (pkPage)
			{
				GD_MatchRoomBtn* pkRoom = (GD_MatchRoomBtn*)pkPage->getChildByTag(i * (m_iNumCol * m_iNumRow) + j);
				if (pkRoom)
				{
					pkRoom->SetTouchEnable(bEnable);
				}
			}
		}
	}
}

ODEInt32 GD_MatchGameRoomList::GetPageNum()
{
	return m_iPageNodeNum;
}

void GD_MatchGameRoomList::ScrollToPrePage()
{
	if (m_bIsScrolling)
	{
		return;
	}

	m_bIsScrolling = true;

	if (m_ScrollView->GetCurPage() > 0)
	{
		m_ScrollView->ScrollToPrePage();
	}
}

void GD_MatchGameRoomList::ScrollToNextPage()
{
	if (m_bIsScrolling)
	{
		return;
	}

	m_bIsScrolling = true;

	if (m_ScrollView->GetCurPage() < (m_iPageNodeNum - 1))
	{
		m_ScrollView->ScrollToNextPage();
	}
}

ODEInt32 GD_MatchGameRoomList::GetCurrPageIndex()
{
	return m_ScrollView->GetCurPage();
}

void GD_MatchGameRoomList::ScrollToEndPage()
{
	m_ScrollView->ScrollToPage(m_iPageNodeNum - 1);
}

void GD_MatchGameRoomList::scrollViewNoChange()
{
	SetAllMatchRoomBtnEnable(true);
}

void GD_MatchGameRoomList::UpdateRoomId( long long oldId, long long newId )
{
	for (int i = 0; i < m_iPageNodeNum; ++i)
	{
		for (int j(0); j < (m_iNumCol * m_iNumRow); ++j)
		{
			CCNode* pkPage = m_ScrollView->GetPage(i);

			if (pkPage)
			{
				GD_MatchRoomBtn* pkRoom = (GD_MatchRoomBtn*)pkPage->getChildByTag(i * (m_iNumCol * m_iNumRow) + j);
				if (pkRoom)
				{
					if (pkRoom->GetMatchRoomId() == oldId)
					{
						pkRoom->SetMatchRoomId(newId);

						return;
					}
				}
			}
		}
	}
}


//掼蛋房间列表
GD_RoomList::GD_RoomList()
	: ODUIScrollViewH(1280.0f * ui_width_scale , 450.0f * ui_height_scale,
	"scroll_bean.png", "scroll_bean_ext.png", 3.0f * g_f32ScaleHD * ui_main_scale)
{
	setAnchorPoint(ccp(0.5f,1.0f));
	UpdateBounding();
	m_kButtonArray.Reserve(128);
}

GD_RoomList::~GD_RoomList()
{

}

void GD_RoomList::OnClicked( ODUIWidget& kWidget, ODEUInt32 u32State )
{
	if(u32State == RELEASED)
	{
		ODEUInt32 u32State = ((ODUIButton&)kWidget).GetIndex() + 1;
		u32State = (u32State | 0x80000000);
		m_kEvent.Callback(*this, u32State);
	}
}

ODEUInt32 GD_RoomList::AddRoomBtn(int roomType, const ODEChar8* pcRoomImageNormal, const ODEChar8* pcRoomImagePressed,int  Tuijian)
{
	ODEUInt32 u32Res = m_kButtonArray.Size();
	
	CCSprite* pNormal=CCSprite::create(pcRoomImageNormal);
	CCSprite* pPress=CCSprite::create(pcRoomImagePressed);
	pPress->setColor(g_pkGuanDanGame->GetColorLevelDiv2(pNormal->getColor()));

	ODUIButton* pkEvent = ODE_NEW ODUIButton(pNormal,pPress);
	pkEvent->setScale(ui_main_scale * 0.9f);

	ODEUInt32 u32Row = u32Res;
	pkEvent->SetIndex(u32Res);

	if (u32Res==0)
	{
		pkEvent->setPosition(ccp(ODEFloat32(u32Row) * 360.0f * ui_width_scale + 225.0f * ui_width_scale, 220.0f * ui_height_scale));
	}
	else if (u32Res==1)
	{
		pkEvent->setPosition(ccp(ODEFloat32(u32Row) * 360.0f * ui_width_scale + 225.0f * ui_width_scale, 230.0f * ui_height_scale));
	}
	else if (u32Res==2)
	{
		pkEvent->setPosition(ccp(ODEFloat32(u32Row) * 360.0f * ui_width_scale + 225.0f * ui_width_scale, 240.0f * ui_height_scale));
	}
	else 
	{
		pkEvent->setPosition(ccp(ODEFloat32(u32Row) * 360.0f * ui_width_scale + 225.0f * ui_width_scale, 222.0f * ui_height_scale));
	}

	if (Tuijian==0)//最新
	{	
		CCSprite* pkRecommendLogo=pkRecommendLogo = CCSprite::create(GD_IMAGE_PATH_ROOM"gd_topnew.png");
		pkRecommendLogo->setPosition(ccp(-153, 50));
		pkEvent->addChild(pkRecommendLogo, 3);
		CCTextureCache::sharedTextureCache()->removeTextureForKey(GD_IMAGE_PATH_ROOM"gd_topnew.png");

	}
	else if (Tuijian==1)//最热
	{
		CCSprite* pkRecommendLogo=pkRecommendLogo = CCSprite::create(GD_IMAGE_PATH_ROOM"gd_tophot.png");
		pkRecommendLogo->setPosition(ccp(-166, 50));
		pkEvent->addChild(pkRecommendLogo, 3);
		CCTextureCache::sharedTextureCache()->removeTextureForKey(GD_IMAGE_PATH_ROOM"gd_tophot.png");
	}
	else if (Tuijian==2)//公测
	{
		CCSprite* pkRecommendLogo = CCSprite::create(GD_IMAGE_PATH_ROOM"gd_test.png");
		if (u32Res==1)
		{
			pkRecommendLogo->setPosition(ccp(-163, 50));
		}
		else if (u32Res==2)
		{
			pkRecommendLogo->setPosition(ccp(-163, 40));
		}
		else if (u32Res==3)
		{
			pkRecommendLogo->setPosition(ccp(-155, 57));
		}
		pkEvent->addChild(pkRecommendLogo, 3);
		CCTextureCache::sharedTextureCache()->removeTextureForKey(GD_IMAGE_PATH_ROOM"gd_test.png");
	}
	
	
	AddChild(pkEvent);
	m_kButtonArray.Resize(m_kButtonArray.Size() + 1);
	Item& kItem = m_kButtonArray.Back();
	kItem.m_pkButton = pkEvent;
	kItem.m_kDelegate.Set(this, &GD_RoomList::OnClicked);
	kItem.m_pkButton->RegisterListener(kItem.m_kDelegate);

	return u32Res;
}

void GD_RoomList::ClearAll()
{
	for (ODEVector<Item>::iterator it = m_kButtonArray.Begin(); it != m_kButtonArray.End(); ++it)
	{
		it->m_pkButton->SetVisible(false);
	}
	m_kButtonArray.Clear();
	UpdateBounding();
}
int GD_RoomList::getGuanDanRoomType()
{
	return m_RoomType;
}
GD_RoomList::Item::Item()
{
	m_pkButton = NULL;
}




GuanDanSettingSlider::GuanDanSettingSlider( const ODEChar8* pcHandle, ODEFloat32 f32Range ,CCSize spbgsize,CCSize splightsize,const ODEChar8* pcBar /*= NULL*/, const ODEChar8* pcBarLight /*= NULL*/ )
	: m_f32Range(f32Range), m_f32Value(0.0f), m_pkSliderLight(NULL), m_bChange(false)
{
	m_pkHandle = CCSprite::create(pcHandle);
	m_pkHandle->setPositionY(-5);
	addChild(m_pkHandle, 3);
	m_bTarget = false;
	m_pkSliderBar = CCScale9Sprite::create(pcBar/*, CCRect (0, 0, 6, 4), CCRect (2, 2, 2, 2)*/);
	m_pkSliderBar->setPreferredSize(CCSizeMake (m_sliderSize.width, 22));
	m_sliderSize = spbgsize;

	if(pcBar)
	{
		m_pkSliderBar = CCScale9Sprite::create(pcBar/*, CCRect (0, 0, 6, 4), CCRect (2, 2, 2, 2)*/);
		m_pkSliderBar->setPreferredSize(CCSizeMake (m_sliderSize.width, 22));
		addChild(m_pkSliderBar, 0);
		m_f32BarWidth = m_pkSliderBar->getContentSize().width;

		if(pcBarLight)
		{
			m_pkSliderLight = CCScale9Sprite::create(pcBarLight/*, CCRect (0, 0, 6, 4), CCRect (2, 2, 2, 2)*/);
			m_pkSliderLight->setPreferredSize(CCSizeMake (0.01f, 22));
			m_pkSliderLight->setAnchorPoint(ccp (0.0f, 0.5f));
			m_pkSliderLight->setPosition(ccp (m_pkSliderBar->boundingBox ().getMinX (), m_pkSliderBar->boundingBox ().getMidY ()));
			addChild(m_pkSliderLight, 0);
		}
		else
		{
			m_pkSliderLight = NULL;
		}
	}

	Update();
	CCSize kSize = m_pkHandle->getContentSize();
	kSize.width += m_f32Range * 2.0f;
	kSize.height = kSize.height ;
	setContentSize(kSize);
}

GuanDanSettingSlider::~GuanDanSettingSlider()
{
}

void GuanDanSettingSlider::Update()
{
	ODEFloat32 f32Pos = ODEFloorf(ODELerp(m_f32Value, -m_f32Range, m_f32Range));
	m_pkHandle->setPositionX(f32Pos);
	if(m_pkSliderLight)
	{
		ODEFloat32 f32Percentage = (m_f32BarWidth * 0.5f + f32Pos) / m_sliderSize.width;
		f32Percentage = ODE_MIN(f32Percentage, 100.0f);
		f32Percentage = ODE_MAX(f32Percentage, 0.0f);
		CCLog("%f",f32Percentage);
		if (f32Percentage<0.05f)
		{
			m_pkSliderLight->setVisible(false);
			if (f32Percentage <= 0.045f)
			{
				f32Percentage =0;
			}
		}
		else
		{
			m_pkSliderLight->setVisible(true);
			if (f32Percentage<0.1)
			{
				m_pkSliderLight->setPreferredSize(CCSize((f32Percentage + 0.05) * m_sliderSize.width, 22));
			}
			else
			{
				m_pkSliderLight->setPreferredSize(CCSize(f32Percentage * m_sliderSize.width, 22));
			}
		}
	}
}

void GuanDanSettingSlider::SetValue(ODEFloat32 f32Value)
{
	if(f32Value != m_f32Value)
	{
		m_f32Value = f32Value;
		Update();
		m_kEvent.Callback(*this, VALUE_CHANGE);
	}
}

void GuanDanSettingSlider::SetValueWithEvent(ODEFloat32 f32Value)
{
	if(f32Value != m_f32Value)
	{
		m_f32Value = f32Value;
		Update();
		m_kEvent.Callback(*this, VALUE_CHANGE);
		m_bChange = true;
	}
}

ODEFloat32 GuanDanSettingSlider::GetValue()
{
	return m_f32Value;
}

ODUIWidget* GuanDanSettingSlider::ProcessTouch(ODUILayer::TouchType eType, cocos2d::CCTouch* pkTouch)
{
	ODUIWidget* pkWidget = ODUIWidget::ProcessTouch(eType, pkTouch);
	if(pkWidget) return pkWidget;

	if(m_bEnabled && m_bVisible)
	{
		switch(eType)
		{
		case ODUILayer::TOUCH_BEGAN:
			{
				m_bTarget = true;
				ODEFloat32 f32WidgetX = getPositionX();
				ODEFloat32 f32Touch = pkTouch->getLocation().x;
				ODEFloat32 f32Min = f32WidgetX - m_f32Range * ui_main_scale;
				ODEFloat32 f32Max = f32WidgetX + m_f32Range * ui_main_scale;
				f32Touch = ODE_MAX(f32Min, f32Touch);
				f32Touch = ODE_MIN(f32Max, f32Touch);
				SetValueWithEvent((f32Touch - f32Min) / (f32Max - f32Min));
			}
			break;
		case ODUILayer::TOUCH_MOVED:
			{
				if (!m_bTarget)
					return NULL;
				ODEFloat32 f32WidgetX = getPositionX();
				ODEFloat32 f32Touch = pkTouch->getLocation().x;
				ODEFloat32 f32Min = f32WidgetX - m_f32Range * ui_main_scale;
				ODEFloat32 f32Max = f32WidgetX + m_f32Range * ui_main_scale;
				f32Touch = ODE_MAX(f32Min, f32Touch);
				f32Touch = ODE_MIN(f32Max, f32Touch);
				SetValueWithEvent((f32Touch - f32Min) / (f32Max - f32Min));
			}
			break;
		case ODUILayer::TOUCH_ENDED:
		case ODUILayer::TOUCH_CANCELLED:
			EventRelease();
			break;
		}
		return this;
	}
	else
	{
		return NULL;
	}
}

void GuanDanSettingSlider::EventRelease()
{
	if(m_bChange)
	{
		m_kEvent.Callback(*this, VALUE_CHANGED);
		m_bChange = false;
	}
	m_bTarget = false;
}

void GuanDanSettingSlider::SetSliderContentHeight( ODEFloat32 f32Rate )
{
	CCSize kSize = m_pkHandle->getContentSize();
	kSize.width += m_f32Range * 2.0f;
	kSize.height *= f32Rate;
	setContentSize(kSize);
}


GuanDanMusicSlider::GuanDanMusicSlider() : GuanDanSettingSlider (GD_IMAGE_PATH_INGAME"gd_slide_icon.png", 88.0f * g_f32ScaleHD,CCSize(374,22),CCSize(374,22), GD_IMAGE_PATH_INGAME"gd_slide_back.png", GD_IMAGE_PATH_INGAME"gd_slide_light.png")
{
	Update();
}

GuanDanEffectSlider::GuanDanEffectSlider() : GuanDanSettingSlider(GD_IMAGE_PATH_INGAME"gd_slide_icon.png",88.0f * g_f32ScaleHD,CCSize(374,22),CCSize(404,22), GD_IMAGE_PATH_INGAME"gd_slide_back.png", GD_IMAGE_PATH_INGAME"gd_slide_light.png")
{
	Update();
}

GuanDanTableSetPW::GuanDanTableSetPW() : ODUILayerModalExt<GuanDanTableSetPW>("guandan_table_setpw")
{
	m_pkRoot = NULL;
	pkEnterBtn = NULL;
	UIInitDelegate(GuanDanTableSetPW, OnClose);
	UIInitDelegate(GuanDanTableSetPW, OnEnter);
}

GuanDanTableSetPW::~GuanDanTableSetPW()
{
	term();
}

bool GuanDanTableSetPW::init()
{
	m_pkRoot = ODE_NEW ODUIWidget;
	AddWidget("GameBankEnterRoot", m_pkRoot);
	m_pkRoot->setContentSize(CCSizeMake(1280 * ui_width_scale, 720 * ui_height_scale));
	setPosition(ui_ccp(0.5f, 0.5f));

	CCScale9Sprite* pkBg = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-01.png");
	pkBg->setPreferredSize(CCSizeMake(674, 300));
	pkBg->setScale(ui_main_scale);
	addChild(pkBg);

	ODEFloat32 bg_width = pkBg->getPreferredSize().width;
	ODEFloat32 bg_height = pkBg->getPreferredSize().height;

	CCSprite* pkTitleText = CCSprite::createWithSpriteFrameName("set_pw_title.png");
	pkTitleText->setPosition(ccp(bg_width/2,bg_height - 32));
	pkBg->addChild(pkTitleText);

	CCScale9Sprite* pCenterBg = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pCenterBg->setAnchorPoint(ccp(0.5, 1));
	pCenterBg->setPreferredSize(CCSize(664, 140));
	pCenterBg->setPosition(ccp(bg_width / 2, bg_height - 54));
	pkBg->addChild(pCenterBg);

	CCScale9Sprite* pCenterBg2 = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pCenterBg2->setAnchorPoint(ccp(0.5, 1));
	pCenterBg2->setPreferredSize(CCSize(664, 90));
	pCenterBg2->setPosition(ccp(bg_width / 2, pCenterBg->getPositionY() - pCenterBg->getContentSize().height + 3));
	pkBg->addChild(pCenterBg2);

	CCSprite* pkCloseBtnNormal = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	CCSprite* pkCloseBtnPress = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	pkCloseBtnPress->setColor(g_pkGuanDanGame->GetColorLevelDiv2(pkCloseBtnPress->getColor()));

	ODUIButton* pkCloseBtn = ODE_NEW ODUIButton(pkCloseBtnNormal, pkCloseBtnPress);
	pkCloseBtn->setScale(ui_main_scale);
	pkCloseBtn->setPosition(ccp((bg_width / 2 - 30) * ui_width_scale, (bg_height / 2 - 32) * ui_main_scale));
	pkCloseBtn->setContentSize(CCSize(pkCloseBtn->getContentSize().width * 2, pkCloseBtn->getContentSize().height * 2));
	AddWidget("kCloseBtn", pkCloseBtn);
	UILinkDelegate(pkCloseBtn, OnClose);

	CCLabelTTF* pkPasswordTitle = CCLabelTTF::create(TT("UIText_TablePw"), TTF_NAME, 30);
	pkPasswordTitle->setAnchorPoint(ccp(0.5, 0.5));
	pkPasswordTitle->setColor(ccc3(225,150,0));
	pkPasswordTitle->setPosition(ccp(105, bg_height - 135));
	pkBg->addChild(pkPasswordTitle);

	CCScale9Sprite* pkPasswordInputBoxBg =CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pkPasswordInputBox = CCEditBox::create(CCSizeMake(433, 50.0f),pkPasswordInputBoxBg);
	pkPasswordInputBox->setPlaceholderFont(TTF_NAME, 24);
	pkPasswordInputBox->setFont(TTF_NAME, 24);
	pkPasswordInputBox->setPlaceHolder(TT("UIText_PleaseSetTablePw"));
	pkPasswordInputBox->setPlaceholderFontColor(ccc3(225,150,0));
	pkPasswordInputBox->setMaxLength(16);
	pkPasswordInputBox->setReturnType(kKeyboardReturnTypeDone);
	pkPasswordInputBox->setFontColor(ccc3(225,150,0));
	pkPasswordInputBox->setPosition(ccp(pkPasswordTitle->getPositionX() + 306, pkPasswordTitle->getPositionY()));
	pkPasswordInputBox->setInputFlag(kEditBoxInputFlagPassword);
	pkBg->addChild(pkPasswordInputBox, 3);

	CCSprite* pEnterText = CCLabelTTF::createWithSpriteFrameName("kessen_base_confirm.png");
	CCScale9Sprite* pkEnterBtnNormal = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_alert_confirm_normal.png");
	pkEnterBtnNormal->setPreferredSize (CCSizeMake (160.0f, 80.0f));
	CCScale9Sprite* pkEnterBtnPress =CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_alert_confirm_press.png");
	pkEnterBtnPress->setPreferredSize (CCSizeMake (160.0f, 80.0f));
	CCScale9Sprite* pkEnterBtnDisable = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"disable.png");
	pkEnterBtnDisable->setPreferredSize (CCSizeMake (160.0f, 80.0f));
	pkEnterBtn = ODE_NEW ODUIButton(pkEnterBtnNormal, pkEnterBtnPress, pkEnterBtnDisable);
	pkEnterBtn->setScale(ui_main_scale);
	pkEnterBtn->setPosition(ccp(0, (-bg_height / 2 + 63)* ui_main_scale));
	AddWidget("kEnterBtn", pkEnterBtn);
	UILinkDelegate(pkEnterBtn, OnEnter);
	pkEnterBtn->addChild(pEnterText, 2);
	return ODUILayerModalExt<GuanDanTableSetPW>::init();
}

void GuanDanTableSetPW::OnActive( const ODUIUserDataPtr& spData )
{
	

}

void GuanDanTableSetPW::OnDeactive( const ODUIUserDataPtr& spData )
{
	GuanDanSetBasePointLayer* pkTemp = (GuanDanSetBasePointLayer*)g_pLayerManager->GetLayer("GuanDan_SetBasePointLayer");
	pkTemp->getBasePointInputBox()->setEnabled(true);
	if (strlen(pkPasswordInputBox->getText()) == 0)
	{		
	}

	pkPasswordInputBox->detachWithIME();
	pkPasswordInputBox->setText("");
	removeTextureManual ();
}

void GuanDanTableSetPW::removeTextureManual ()
{
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-01.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"bank_enter.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"personal_bank.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"SZ-01.png");

#ifdef  _CACHEDTEXTTUREINFO_
	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
#endif
}

bool GuanDanTableSetPW::OnBack()
{
	return true;
}

void GuanDanTableSetPW::term()
{
	ODUILayer::term();
}

void GuanDanTableSetPW::resetPassword()
{
	pkPasswordInputBox->setText("");
}


bool GuanDanTableSetPW::ProcessTouch( TouchType eType, cocos2d::CCTouch* pkTouch )
{
	if (!ODUILayer::ProcessTouch(eType, pkTouch))
	{
		if(!m_pkRoot->TestInput(eType, pkTouch))
		{
			g_pLayerManager->Back();
		}
		else
		{
			switch(eType)
			{
			case ODUILayer::TOUCH_BEGAN:
				pkPasswordInputBox->ccTouchBegan(pkTouch, NULL);
				break;
			case ODUILayer::TOUCH_MOVED:
				pkPasswordInputBox->ccTouchMoved(pkTouch, NULL);
				break;
			case ODUILayer::TOUCH_ENDED:
				pkPasswordInputBox->ccTouchEnded(pkTouch, NULL);
				break;
			case ODUILayer::TOUCH_CANCELLED:
				pkPasswordInputBox->ccTouchCancelled(pkTouch, NULL);
				break;
			default:
				break;
			}
		}
	}
	return false;
}

UIImplementDelegate(GuanDanTableSetPW, OnClose, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		GuanDanSetBasePointLayer* set_base = (GuanDanSetBasePointLayer*)g_pLayerManager->GetLayer("GuanDan_SetBasePointLayer");
		set_base->getSelectCheck()->SetCheck(false);
		g_pLayerManager->Back();
	}
}

UIImplementDelegate(GuanDanTableSetPW, OnEnter, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		if (strlen(pkPasswordInputBox->getText()) == 0)
		{
			g_pkGuanDanGame->Toast(TT("UIText_TablePw_Null"));
		}
		else if (strlen (pkPasswordInputBox->getText()) < 4 || strlen (pkPasswordInputBox->getText()) > 15)
		{
			g_pkGuanDanGame->Toast(TT("UITextRegisterPasswordError"));
		}
		else if (!ODETools::IsLegalAccount (pkPasswordInputBox->getText()))
		{
			g_pkGuanDanGame->Toast(TT("UITextRegisterPasswordError_Long"));
		}
		else
		{
			GuanDanSetBasePointLayer* baseset = (GuanDanSetBasePointLayer*)g_pLayerManager->GetLayer("GuanDan_SetBasePointLayer");
			char table_pw[128];
			sprintf(table_pw, "%s", pkPasswordInputBox->getText());
			baseset->setUserTableAnteBufferPw(true,table_pw);
			GuanDanSetBasePointLayer* set_base = (GuanDanSetBasePointLayer*)g_pLayerManager->GetLayer("GuanDan_SetBasePointLayer");
			set_base->getSelectCheck()->SetCheck(true);
			g_pLayerManager->Back();
		}
	}
}

CCEditBox * GuanDanTableSetPW::getPwEditBox()
{
	return pkPasswordInputBox;
}

GuanDanPwTableEnter::GuanDanPwTableEnter() : ODUILayerModalExt<GuanDanPwTableEnter>("GuanDan_pwtable_enter")
{
	m_pkRoot = NULL;
	pkEnterBtn = NULL;
	UIInitDelegate(GuanDanPwTableEnter, OnClose);
	UIInitDelegate(GuanDanPwTableEnter, OnCancel);
	UIInitDelegate(GuanDanPwTableEnter, OnEnter);
}

GuanDanPwTableEnter::~GuanDanPwTableEnter()
{
	term();
}

bool GuanDanPwTableEnter::init()
{
	m_pkRoot = ODE_NEW ODUIWidget;
	AddWidget("GameBankEnterRoot", m_pkRoot);
	m_pkRoot->setContentSize(CCSizeMake(1280 * ui_width_scale,720 * ui_height_scale));

	setPosition(ui_ccp(0.5f, 0.5f));

	CCScale9Sprite* pkBg = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-01.png");
	pkBg->setPreferredSize(CCSizeMake(674, 300));
	pkBg->setScale(ui_main_scale);
	addChild(pkBg);

	ODEFloat32 bg_width = pkBg->getPreferredSize().width;
	ODEFloat32 bg_height = pkBg->getPreferredSize().height;

	CCSprite* pkTitleText = CCSprite::createWithSpriteFrameName("input_pw_title.png");
	pkTitleText->setPosition(ccp(bg_width/2,bg_height - 32));
	pkBg->addChild(pkTitleText);

	CCScale9Sprite* pCenterBg = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pCenterBg->setAnchorPoint(ccp(0.5, 1));
	pCenterBg->setPreferredSize(CCSize(664, 140));
	pCenterBg->setPosition(ccp(bg_width / 2, bg_height - 54));
	pkBg->addChild(pCenterBg);

	CCScale9Sprite* pCenterBg2 = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pCenterBg2->setAnchorPoint(ccp(0.5, 1));
	pCenterBg2->setPreferredSize(CCSize(664, 90));
	pCenterBg2->setPosition(ccp(bg_width / 2, pCenterBg->getPositionY() - pCenterBg->getContentSize().height + 3));
	pkBg->addChild(pCenterBg2);

	CCSprite* pkCloseBtnNormal = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	CCSprite* pkCloseBtnPress = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	pkCloseBtnPress->setColor(g_pkGuanDanGame->GetColorLevelDiv2(pkCloseBtnPress->getColor()));

	ODUIButton* pkCloseBtn = ODE_NEW ODUIButton(pkCloseBtnNormal, pkCloseBtnPress);
	pkCloseBtn->setScale(ui_main_scale);
	pkCloseBtn->setPosition(ccp((bg_width / 2 - 30) * ui_width_scale, (bg_height / 2 - 32) * ui_main_scale));
	pkCloseBtn->setContentSize(CCSize(pkCloseBtn->getContentSize().width * 2, pkCloseBtn->getContentSize().height * 2));
	AddWidget("kCloseBtn", pkCloseBtn);
	UILinkDelegate(pkCloseBtn, OnClose);

	CCLabelTTF* pkPasswordTitle = CCLabelTTF::create(TT("UIText_InputTablePw_Tip"), TTF_NAME, 30);
	pkPasswordTitle->setAnchorPoint(ccp(0, 0.5));
	pkPasswordTitle->setColor(ccc3(225,150,0));
	pkPasswordTitle->setPosition(ccp(5, bg_height - 75));
	pkBg->addChild(pkPasswordTitle);

	CCScale9Sprite* pkPasswordInputBoxBg =CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pkPasswordInputBox = CCEditBox::create(CCSizeMake(433, 50.0f),pkPasswordInputBoxBg);
	pkPasswordInputBox->setPlaceholderFont(TTF_NAME, 24);
	pkPasswordInputBox->setFont(TTF_NAME, 24);
	pkPasswordInputBox->setPlaceHolder(TT("UIText_PleaseSetTablePw"));
	pkPasswordInputBox->setPlaceholderFontColor(ccc3(130,130,130));
	pkPasswordInputBox->setMaxLength(16);
	pkPasswordInputBox->setReturnType(kKeyboardReturnTypeDone);
	pkPasswordInputBox->setFontColor(ccc3(225,150,0));
	pkPasswordInputBox->setPosition(ccp(pkPasswordTitle->getPositionX() + 316, pkPasswordTitle->getPositionY()-65));
	pkPasswordInputBox->setInputFlag(kEditBoxInputFlagPassword);
	pkBg->addChild(pkPasswordInputBox, 3);
	CCSprite* pEnterText = CCLabelTTF::createWithSpriteFrameName("kessen_base_confirm.png");
	CCScale9Sprite* pkEnterBtnNormal = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_alert_confirm_normal.png");
	pkEnterBtnNormal->setPreferredSize (CCSizeMake (160.0f, 80.0f));
	CCScale9Sprite* pkEnterBtnPress =CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_alert_confirm_press.png");
	pkEnterBtnPress->setPreferredSize (CCSizeMake (160.0f, 80.0f));
	CCScale9Sprite* pkEnterBtnDisable = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"disable.png");
	pkEnterBtnDisable->setPreferredSize (CCSizeMake (160.0f, 80.0f));
	pkEnterBtn = ODE_NEW ODUIButton(pkEnterBtnNormal, pkEnterBtnPress, pkEnterBtnDisable);
	pkEnterBtn->setScale(ui_main_scale);
	pkEnterBtn->setPosition(ccp(40*ui_main_scale  + pkEnterBtn->getContentSize().width/2*ui_main_scale, (-bg_height / 2 + 63)* ui_main_scale));
	AddWidget("kEnterBtn", pkEnterBtn);
	UILinkDelegate(pkEnterBtn, OnEnter);

	pkEnterBtn->addChild(pEnterText, 2);

	CCSprite* pcancelText = CCSprite::createWithSpriteFrameName("kessen_base_cancel.png");

	CCScale9Sprite* cancelBtnNormal = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_alert_cancel_nromal.png");
	cancelBtnNormal->setPreferredSize (CCSizeMake (160.0f, 80.0f));
	CCScale9Sprite* cancelBtnPress = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_alert_cancel_press.png");
	cancelBtnPress->setPreferredSize (CCSizeMake (160.0f, 80.0f));
	CCScale9Sprite* cancelBtnDisable = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"disable.png");
	cancelBtnDisable->setPreferredSize (CCSizeMake (160.0f, 80.0f));

	pkCancelBtn = ODE_NEW ODUIButton(cancelBtnNormal, cancelBtnPress, cancelBtnDisable);
	pkCancelBtn->setScale(ui_main_scale);
	pkCancelBtn->setPosition(ccp(-40*ui_main_scale - pkCancelBtn->getContentSize().width/2*ui_main_scale, (-bg_height / 2 + 63)* ui_main_scale));
	AddWidget("kEnterBtn", pkCancelBtn);
	UILinkDelegate(pkCancelBtn, OnCancel);

	pkCancelBtn->addChild(pcancelText, 2);
	return ODUILayerModalExt<GuanDanPwTableEnter>::init();
}

void GuanDanPwTableEnter::OnActive( const ODUIUserDataPtr& spData )
{


}

void GuanDanPwTableEnter::OnDeactive( const ODUIUserDataPtr& spData )
{
	pkPasswordInputBox->detachWithIME();
	pkPasswordInputBox->setText("");
	removeTextureManual ();
}

void GuanDanPwTableEnter::removeTextureManual ()
{
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-01.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"bank_enter.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"personal_bank.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"SZ-01.png");

#ifdef  _CACHEDTEXTTUREINFO_
	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
#endif
}

bool GuanDanPwTableEnter::OnBack()
{
	return true;
}

void GuanDanPwTableEnter::term()
{
	ODUILayer::term();
}

void GuanDanPwTableEnter::resetPassword()
{
	pkPasswordInputBox->setText("");
}


bool GuanDanPwTableEnter::ProcessTouch( TouchType eType, cocos2d::CCTouch* pkTouch )
{
	if (!ODUILayer::ProcessTouch(eType, pkTouch))
	{
		if(!m_pkRoot->TestInput(eType, pkTouch))
		{
			g_pLayerManager->Back();
		}
		else
		{
			switch(eType)
			{
			case ODUILayer::TOUCH_BEGAN:
				pkPasswordInputBox->ccTouchBegan(pkTouch, NULL);
				break;
			case ODUILayer::TOUCH_MOVED:
				pkPasswordInputBox->ccTouchMoved(pkTouch, NULL);
				break;
			case ODUILayer::TOUCH_ENDED:
				pkPasswordInputBox->ccTouchEnded(pkTouch, NULL);
				break;
			case ODUILayer::TOUCH_CANCELLED:
				pkPasswordInputBox->ccTouchCancelled(pkTouch, NULL);
				break;
			default:
				break;
			}
		}
	}
	return false;
}

UIImplementDelegate(GuanDanPwTableEnter, OnClose, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		g_pkGuanDanGame->setGameTableListEnable (true);
		g_pLayerManager->Back();
	}
}
UIImplementDelegate(GuanDanPwTableEnter, OnCancel, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		g_pkGuanDanGame->setGameTableListEnable (true);
		g_pLayerManager->Back();
	}
}

UIImplementDelegate(GuanDanPwTableEnter, OnEnter, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		if (strlen(pkPasswordInputBox->getText()) == 0)
		{
			g_pkGuanDanGame->Toast(TT("UIText_TablePw_Null"));
		}
		else if (strlen (pkPasswordInputBox->getText()) < 4 || strlen (pkPasswordInputBox->getText()) > 15)
		{
			g_pkGuanDanGame->Toast(TT("UITextRegisterPasswordError"));
		}
		else if (!ODETools::IsLegalAccount (pkPasswordInputBox->getText()))
		{
			g_pkGuanDanGame->Toast(TT("UITextRegisterPasswordError_Long"));
		}
		else
		{
			TcpClientSocket *pGameSocket = g_pkNetworkHelper->GetGameServerDelegate();
			if (pGameSocket->IsConnect())
			{
				g_pkGuanDanGame->ShowLoading();

				PUSRoomExJoinTableReq JoinTableReq={0};
				JoinTableReq.iTableID = g_pkClientData->getJoinPwTableNum();
				sprintf(JoinTableReq.szTablePasswd, "%s", pkPasswordInputBox->getText());
				md5_str(JoinTableReq.szTablePasswd, strlen(JoinTableReq.szTablePasswd), JoinTableReq.szTablePasswd);

				g_pkGuanDanGame->startCheckSocket(SOCKETINDEX_GAME);
				pGameSocket->SendMsgToSocket(&JoinTableReq, GAMEFCT_MSG_PUSEX_JOIN_TABLE_REQ, sizeof(JoinTableReq));
			}
			else
			{
				g_pkGuanDanGame->Toast(TT("UIText_GameUI_KickOut_154"));
			}
		}
	}
}

CCEditBox * GuanDanPwTableEnter::getPwEditBox()
{
	return pkPasswordInputBox;
}

GuanDanGamePay::GuanDanGamePay() : ODUILayerModalExt<GuanDanGamePay>("GuanDanGamePay")
{
	UIInitDelegate(GuanDanGamePay, OnClose);
	UIInitDelegate(GuanDanGamePay, OnConfirm);
	UIInitDelegate(GuanDanGamePay, OnWXPay);
}

GuanDanGamePay::~GuanDanGamePay()
{
	term();
}

bool GuanDanGamePay::init()
{
	setScale(ui_main_scale);
	pkRoot = ODE_NEW ODUIWidget;
	pkRoot->setContentSize(CCSizeMake(765, 525));
	AddWidget("Root", pkRoot);
	pkRoot->setPosition(ui_ccp(0.5f, 0.5f));

	CCScale9Sprite* pkBg = CCScale9Sprite::create( GD_IMAGE_PATH_INGAME"gd_setting_bg.png");
	pkBg->setPreferredSize(CCSizeMake(765.0f, 525.0f));
	pkRoot->addChild(pkBg);

	CCSprite* pkTopLight=CCSprite::create(GD_IMAGE_PATH_INGAME"gd_right_light.png");
	pkTopLight->setAnchorPoint(ccp(0,1));
	pkTopLight->setPosition(ccp(12,pkBg->getContentSize().height-15));
	pkBg->addChild(pkTopLight,1);


	CCScale9Sprite* pkBg_1 = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_headBackTop.png");
	pkBg_1->setAnchorPoint(ccp(0.5, 1));
	pkBg_1->setPreferredSize(CCSizeMake(700.0f, 182.0f));
	pkBg_1->setPosition(ccp(0,pkBg->boundingBox ().getMaxY()-88));
	pkRoot->addChild(pkBg_1);

	CCScale9Sprite* pkBg_2 = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_headBackTop.png");
	pkBg_2->setAnchorPoint(ccp(0.5, 1));
	pkBg_2->setPreferredSize(CCSizeMake(700.0f, 136.0f));
	pkBg_2->setPosition(ccp(0,pkBg_1->getPositionY() - pkBg_1->getContentSize().height + 1));
	pkRoot->addChild(pkBg_2);

	CCSprite* pkTitle = CCSprite::create("GuanDan/quickPay/gd_pay_title.png");
	pkTitle->setPosition(ccp(0,pkBg->boundingBox ().getMaxY()-50));
	pkRoot->addChild(pkTitle);

	CCScale9Sprite* pGoldIconBox = CCScale9Sprite::create("GuanDan/quickPay/gd_icon_bg.png");
	pGoldIconBox->setPreferredSize(CCSize(145, 145));
	pGoldIconBox->setPosition(ccp(pkBg->boundingBox().getMinX() + 118, pkBg_1->boundingBox().getMidY()));
	pkRoot->addChild(pGoldIconBox);

	CCSprite* pGoldIcon = CCSprite::create("GuanDan/quickPay/gd_coin_box.png");
	pGoldIcon->setPosition(pGoldIconBox->getPosition());
	pkRoot->addChild(pGoldIcon);

	CCLabelTTF* pNeedScore = CCLabelTTF::create(TT("UIText_Game_QuickPay_Score"), TTF_NAME, 30);
	pNeedScore->setAnchorPoint(ccp(0, 0.5));
	pNeedScore->setPosition(ccp(pkBg->boundingBox().getMinX() + 216, pkBg_1->boundingBox().getMidY()));
	pkRoot->addChild(pNeedScore, 0, 1000);

	CCSprite*pFirstPay=CCSprite::create("GuanDan/quickPay/gd_firstPay.png");
	pFirstPay->setAnchorPoint(ccp(1,1));
	pFirstPay->setPosition(ccp(pkBg_1->getPositionX()+pkBg_1->getContentSize().width/2,pkBg_1->getPositionY()));
	//pkRoot->addChild(pFirstPay);

	CCLabelTTF* pKeFu = CCLabelTTF::create(TT("UIText_Game_QuickPay_KeFu"), TTF_NAME, 26);
	pKeFu->setPosition(ccp(pkBg->boundingBox().getMidX(), pkBg_2->boundingBox().getMidY()+20));
	pKeFu->setColor(ccc3(222, 178,115));
	pkRoot->addChild(pKeFu);
	//微信充值
	pkWeiXinCheckBox = ODE_NEW ODUICheckBox("GuanDan/quickPay/gd_checkBox1.png","GuanDan/quickPay/gd_checkBox2.png");
	pkWeiXinCheckBox->setPosition(ccp(ui_w(0.5f)+pkBg->boundingBox().getMaxX()-220,ui_h(0.5f)+pkBg->boundingBox().getMidY()-100));
//	AddWidget("weixin_checkBox", pkWeiXinCheckBox);
	UILinkDelegate(pkWeiXinCheckBox, OnWXPay);
	pkWeiXinCheckBox->setContentSize(pkWeiXinCheckBox->getContentSize()*2.5f);

	CCLabelTTF* zhifubaoLabel = CCLabelTTF::create(TT("UIText_GamePayWeiXinText"), TTF_NAME, 28);
	zhifubaoLabel->setAnchorPoint(ccp(0.0f, 0.5f));
	zhifubaoLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
	zhifubaoLabel->setPositionX(40.0f);
	zhifubaoLabel->setColor(ccc3(200, 200, 200));
	pkWeiXinCheckBox->addChild(zhifubaoLabel, 4);
	pkWeiXinCheckBox->SetCheck(false);

	CCSprite* pkCloseBtnNormal = CCSprite::create(GD_IMAGE_PATH_INGAME"gd_setting_close.png");
	CCSprite* pkCloseBtnPress = CCSprite::create(GD_IMAGE_PATH_INGAME"gd_setting_close.png");
	pkCloseBtnPress->setColor(g_pkGuanDanGame->GetColorLevelDiv2(pkCloseBtnPress->getColor()));
	ODUIButton* pkCloseBtn = ODE_NEW ODUIButton(pkCloseBtnNormal, pkCloseBtnPress);
	pkCloseBtn->setPosition(ccp(pkBg->boundingBox ().getMaxX()-32,pkBg->boundingBox ().getMaxY()-34));
	pkCloseBtn->setContentSize(CCSize(pkCloseBtn->getContentSize().width * 2, pkCloseBtn->getContentSize().height * 2));
	pkRoot->AddChild(pkCloseBtn);
	UILinkDelegate(pkCloseBtn, OnClose);

	CCSprite* pkConfirmBtnNormal = CCSprite::create("GuanDan/quickPay/gd_pay_btn.png");
	CCSprite* pkConfirmBtnPress = CCSprite::create("GuanDan/quickPay/gd_pay_btn.png");
	pkConfirmBtnPress->setColor(g_pkGuanDanGame->GetColorLevelDiv2(pkConfirmBtnPress->getColor()));
	
	ODUIButton* pkConfirmBtn = ODE_NEW ODUIButton(pkConfirmBtnNormal, pkConfirmBtnPress);
	pkConfirmBtn->setPosition(ccp(pkBg->boundingBox().getMidX(),pkBg->boundingBox().getMinY()+70));
	pkRoot->AddChild(pkConfirmBtn);
	UILinkDelegate(pkConfirmBtn, OnConfirm);
	CCSprite* pConfirmText = CCSprite::create("GuanDan/quickPay/gd_pay_text.png");
	pkConfirmBtn->addChild(pConfirmText, 4);

	RemoveTexturesManual();

	return ODUILayerModalExt<GuanDanGamePay>::init();
}

void GuanDanGamePay::RemoveTexturesManual()
{
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_QUICKPAY"gamepay_box.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_QUICKPAY"gamepay_btn.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_QUICKPAY"gamepay_confirm_btn.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_QUICKPAY"gamepay_confirm_text.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_QUICKPAY"gamepay_gold_icon2.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_QUICKPAY"gamepay_title.png");
}

void GuanDanGamePay::OnActive( const ODUIUserDataPtr& spData )
{
	int payMoney = 10;
	int getScore = BuyNotify::sharedConfig()->getDefaultGetScoreWithOneYuan(PAY_TAOBAO) * BuyNotify::sharedConfig()->getPayRate() * payMoney;
	char temp[1024] = {0};
	sprintf(temp, TT("UIText_Game_QuickPay_Score"), getScore, payMoney);

	CCLabelTTF* pTip = (CCLabelTTF*)pkRoot->getChildByTag(1000);
	pTip->setString(temp);
}

bool GuanDanGamePay::OnBack()
{
	return true;
}

bool GuanDanGamePay::ProcessTouch( TouchType eType, CCTouch* pkTouch )
{
	if (!ODUILayer::ProcessTouch(eType, pkTouch))
	{
		if(!pkRoot->TestInput(eType, pkTouch))
		{
			if (g_pkGuanDanData->GetIsNeedGuideInGD())
				g_pLayerManager->PopLayer ();
			else
				g_pLayerManager->PopAllLayers ();
		}
	}
	return true;
}

UIImplementDelegate(GuanDanGamePay, OnClose, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		if (g_pkGuanDanData->GetIsNeedGuideInGD())
			g_pLayerManager->PopLayer ();
		else
			g_pLayerManager->PopAllLayers ();
	}
}

UIImplementDelegate(GuanDanGamePay, OnConfirm, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
			//玩家在哪里充值了
			int payEntrance = BUY_STATE_GUANDAN_LEVEL_ROOM1_PLAY + g_pkClientData->getRoomIndex();
			BuyNotify::sharedConfig()->setPayState(payEntrance);

			if (g_pLayerManager->GetLayer("lobby_alipay") == NULL)
			{
				g_pLayerManager->AddLayer(LobbyAlipay::create());
			}

			LobbyAlipay* pAlipay = (LobbyAlipay*)g_pLayerManager->GetLayer("lobby_alipay");
			
			if (pAlipay)
			{
				pAlipay->sendPayMsg (g_pkClientData->GetUserName(), (10));
			}

			g_pLayerManager->PopAllLayers();
	
	}
}

UIImplementDelegate(GuanDanGamePay, OnWXPay, kWidget, u32State)
{
	if (u32State == ODUIWidget::VALUE_CHANGE)
	{

	}
}

GuanDanGameMusicSlider::GuanDanGameMusicSlider() : GuanDanSlider(TEXASPOKER_IMAGE_PATH_GAMEBANK"handle.png",TEXASPOKER_IMAGE_PATH_GAMEBANK"handle_l.png",TEXASPOKER_IMAGE_PATH_GAMEBANK"handle_r.png",  88.0f * g_f32ScaleHD,CCSize(386,42),CCSize(416,42), TEXASPOKER_IMAGE_PATH_GAMEBANK"slider_back.png", TEXASPOKER_IMAGE_PATH_GAMEBANK"slider_bar.png")
{

	Update();
}

void GuanDanGameMusicSlider::Update()
{
	GuanDanSlider::Update();
}

GuanDanGameEffectSlider::GuanDanGameEffectSlider() :GuanDanSlider(TEXASPOKER_IMAGE_PATH_GAMEBANK"handle.png",TEXASPOKER_IMAGE_PATH_GAMEBANK"handle_l.png",TEXASPOKER_IMAGE_PATH_GAMEBANK"handle_r.png",  88.0f * g_f32ScaleHD,CCSize(386,42),CCSize(416,42), TEXASPOKER_IMAGE_PATH_GAMEBANK"slider_back.png", TEXASPOKER_IMAGE_PATH_GAMEBANK"slider_bar.png")
{

	Update();
}

void GuanDanGameEffectSlider::Update()
{
	GuanDanSlider::Update();
}

GuanDanGameLightSlider::GuanDanGameLightSlider() : GuanDanSlider(TEXASPOKER_IMAGE_PATH_GAMEBANK"handle.png",TEXASPOKER_IMAGE_PATH_GAMEBANK"handle_l.png",TEXASPOKER_IMAGE_PATH_GAMEBANK"handle_r.png", 88.0f * g_f32ScaleHD,CCSize(386,42),CCSize(416,42), TEXASPOKER_IMAGE_PATH_GAMEBANK"slider_back.png", TEXASPOKER_IMAGE_PATH_GAMEBANK"slider_bar.png")
{
	
	Update();
}
//end
void GuanDanGameLightSlider::Update()
{
	GuanDanSlider::Update();
}

GuanDanGameOption::GuanDanGameOption() : ODUILayerModalExt<GuanDanGameOption>("guandan_game_option"),
	//m_iPreLightValue(0),
	m_iPreMusicValue(0),
	m_iPreEffectValue(0),
	m_bPreEffect(true),
	m_bPreShark(false),
	m_bPreSilence(false),
	//m_bPreLight(true),
	m_bPreMusic(true)
{
	UIInitDelegate(GuanDanGameOption, OnClose);
	UIInitDelegate(GuanDanGameOption, OnMusicSliderVelueChange);
	UIInitDelegate(GuanDanGameOption, OnEffectSliderVelueChange);
	UIInitDelegate(GuanDanGameOption, OnSilenceCheck);
	UIInitDelegate(GuanDanGameOption, OnSpecialEffectCheck);
	UIInitDelegate(GuanDanGameOption, OnBeijingyinCheck);
	UIInitDelegate(GuanDanGameOption, OnExitGame);
}

GuanDanGameOption::~GuanDanGameOption()
{
	term();
}

bool GuanDanGameOption::init()
{
	setScale(ui_main_scale);
	pkRoot = ODE_NEW ODUIWidget;
	pkRoot->setContentSize(CCSizeMake(725.0f, 514.0f));
	AddWidget("Root", pkRoot);
	pkRoot->setPosition(ui_ccp(0.5f, 0.5f));

	CCScale9Sprite* pkBg = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-01.png");
	pkBg->setPreferredSize(CCSizeMake(725.0f, 322.0f));
	pkRoot->addChild(pkBg);

	CCScale9Sprite* pkBg_1 = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pkBg_1->setPreferredSize(CCSizeMake(715.0f, 170.0f));
	pkBg_1->setPosition(ccp(0,pkBg->boundingBox ().getMaxY()-152));
	pkRoot->addChild(pkBg_1);

	CCScale9Sprite* pkBg_2 = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pkBg_2->setPreferredSize(CCSizeMake(715.0f, 80.0f));
	pkBg_2->setPosition(ccp(0,pkBg->boundingBox ().getMaxY()-274));
	pkRoot->addChild(pkBg_2);

	ODEFloat32 panel_width = pkBg->getContentSize().width;
	ODEFloat32 panel_height = pkBg->getContentSize().height;

	CCSprite* pkTitle = CCSprite::create(TP_IMAGE_PATH_INGAEM"setting_title.png");
	pkTitle->setPosition(ccp(0,pkBg->boundingBox ().getMaxY()-38));
	pkRoot->addChild(pkTitle);

	CCSprite* pkCloseBtnNormal = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	CCSprite* pkCloseBtnPress = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	pkCloseBtnPress->setColor(g_pkGuanDanGame->GetColorLevelDiv2(pkCloseBtnPress->getColor()));
	ODUIButton* pkCloseBtn = ODE_NEW ODUIButton(pkCloseBtnNormal, pkCloseBtnPress);
	pkCloseBtn->setPosition(ccp(pkBg->boundingBox ().getMaxX()-32,pkBg->boundingBox ().getMaxY()-38));
	pkCloseBtn->setContentSize(CCSize(pkCloseBtn->getContentSize().width * 2, pkCloseBtn->getContentSize().height * 2));
	pkRoot->AddChild(pkCloseBtn);
	UILinkDelegate(pkCloseBtn, OnClose);

	ccColor3B kColor;
	kColor.r = 255;
	kColor.g = 200;
	kColor.b = 155;

	CCLabelTTF* pkMusicText = CCLabelTTF::create(TT("UITextGameOptionMusic"), TTF_NAME, 30);
	pkMusicText->setPosition(ccp(pkBg->boundingBox ().getMinX()+102,pkBg->boundingBox ().getMaxY()-108));
	pkRoot->addChild(pkMusicText);

	CCLabelTTF* pkEffectText = CCLabelTTF::create(TT("UITextGameOptionEffect"), TTF_NAME, 30);
	pkEffectText->setPosition(ccp(pkBg->boundingBox ().getMinX()+102,pkBg->boundingBox ().getMaxY()-104 -83));
	pkRoot->addChild(pkEffectText);

	GuanDanGameMusicSlider* pkMusicSlider = ODE_NEW GuanDanGameMusicSlider;
	pkMusicSlider->setPosition(ccpAdd(ui_ccp(0.5,0.5),ccp(pkBg->boundingBox ().getMinX()+360,pkBg->boundingBox ().getMaxY()-108)));
	AddWidget("music_slider", pkMusicSlider);

 	pkMusicSlider->SetValue(0.5f);
	UILinkDelegate(pkMusicSlider,OnMusicSliderVelueChange);

	GuanDanGameEffectSlider* pkEffectSlider = ODE_NEW GuanDanGameEffectSlider;
	pkEffectSlider->setPosition(ccpAdd(ui_ccp(0.5,0.5),ccp(pkBg->boundingBox ().getMinX()+360,pkBg->boundingBox ().getMaxY()-104 -83)));
	AddWidget("effect_slider", pkEffectSlider);

 	pkEffectSlider->SetValue(0.5f);
	UILinkDelegate(pkEffectSlider,OnEffectSliderVelueChange);

	pkSilenceCheckBox = ODE_NEW ODUICheckBox("image/setting/lobby_checkBox1.png", "image/setting/lobby_checkBox2.png");
	pkSilenceCheckBox->setPosition(ccpAdd(ui_ccp(0.5,0.5),ccp(pkBg->boundingBox ().getMaxX()-80,pkBg->boundingBox ().getMaxY()-275)));
	AddWidget("silence_checkBox", pkSilenceCheckBox);
	UILinkDelegate(pkSilenceCheckBox,OnSilenceCheck);

	CCLabelTTF* pkSilenceText = CCLabelTTF::create(TT("UITextGameOptionSilence"), TTF_NAME, 30);
	pkSilenceText->setPosition(ccp(pkBg->boundingBox ().getMinX()+119,pkBg->boundingBox().getMaxY()-275));

	pkRoot->addChild(pkSilenceText);

	pkSpecialEffetCheckBox = ODE_NEW ODUICheckBox("image/setting/lobby_checkBox1.png", "image/setting/lobby_checkBox2.png");
	pkSpecialEffetCheckBox->setPosition(ccpAdd(ui_ccp(0.5,0.5),ccp(pkBg->boundingBox ().getMaxX()-80,pkBg->boundingBox ().getMaxY()-104 -83)));
	AddWidget("voice_checkBox", pkSpecialEffetCheckBox);
	UILinkDelegate(pkSpecialEffetCheckBox,OnSpecialEffectCheck);

	pkBeijingyinCheckBox = ODE_NEW ODUICheckBox("image/setting/lobby_checkBox1.png", "image/setting/lobby_checkBox2.png");
	pkBeijingyinCheckBox->setPosition(ccpAdd(ui_ccp(0.5,0.5),ccp(pkBg->boundingBox ().getMaxX()-80,pkBg->boundingBox ().getMaxY()-108)));
	AddWidget("voice_checkBox", pkBeijingyinCheckBox);
	UILinkDelegate(pkBeijingyinCheckBox,OnBeijingyinCheck);

	return ODUILayerModalExt<GuanDanGameOption>::init();
}

void GuanDanGameOption::OnActive( const ODUIUserDataPtr& spData )
{
	WIDGET(GuanDanGameEffectSlider,effect_slider);
	WIDGET(GuanDanGameMusicSlider, music_slider);

	bool bCheckMusic = g_pSaveData->ReadParam("bg_yin", ODEUInt32(1)) ? true : false;
	pkBeijingyinCheckBox->SetCheck(bCheckMusic);
	music_slider.SetValue( bCheckMusic ? g_pSaveData->ReadParam("music_volume", 1.0f) : 0);

	bool bCheckEffectSound = g_pSaveData->ReadParam("special_effect", ODEUInt32(1)) ? true : false;
	pkSpecialEffetCheckBox->SetCheck(bCheckEffectSound);	
	effect_slider.SetValue( bCheckEffectSound ? g_pSaveData->ReadParam("sound_volume", 1.0f) : 0);

	pkSilenceCheckBox->SetCheck(g_pSaveData->ReadParam("sound_switch", ODEUInt32(0)) ? true : false);
}

bool GuanDanGameOption::OnBack()
{
	return true;
}

bool GuanDanGameOption::ProcessTouch( TouchType eType, CCTouch* pkTouch )
{
	if (!ODUILayer::ProcessTouch(eType, pkTouch))
	{
		if(!pkRoot->TestInput(eType, pkTouch))
		{
			if (eType == TOUCH_BEGAN)
			{
				bool bCheckMusic = g_pSaveData->ReadParam("bg_yin", ODEUInt32(1)) ? true : false;

				if (! bCheckMusic)
				{
					g_pSaveData->WriteParam("music_volume", 0.0f);
				}

				bool bCheckEffectSound = g_pSaveData->ReadParam("special_effect", ODEUInt32(1)) ? true : false;
				if (! bCheckEffectSound)
				{
					g_pSaveData->WriteParam("sound_volume", 0.0f);
				}

				g_pLayerManager->Back();
			}
		}
	}
	return true;
}

UIImplementDelegate(GuanDanGameOption, OnMusicSliderVelueChange, kWidget, u32State)
{
	if (u32State == ODUIWidget::VALUE_CHANGE)
	{
		g_pSoundSystem->SetMusicVolume(((GuanDanSlider&)kWidget).GetValue());
		CCLOG("Music volum change to %f", ((GuanDanSlider&)kWidget).GetValue());
	}
	if (u32State == ODUIWidget::VALUE_CHANGED)
	{
		g_pSoundSystem->SetMusicVolume(((GuanDanSlider&)kWidget).GetValue());
		g_pSaveData->WriteParam("music_volume", ((GuanDanSlider&)kWidget).GetValue());
		g_pSaveData->Save();
		m_iPreMusicValue = (ODEFloat32)(g_pSaveData->ReadParam("music_volume", 1.0f));
		if (m_iPreMusicValue == 0)
		{
			pkBeijingyinCheckBox->SetCheck(false);
		}
		else
		{
			pkBeijingyinCheckBox->SetCheck(true);
		}

	}
}

UIImplementDelegate(GuanDanGameOption, OnEffectSliderVelueChange, kWidget, u32State)
{
	if (u32State == ODUIWidget::VALUE_CHANGE)
	{
		g_pSoundSystem->SetSoundVolume(((GuanDanSlider&)kWidget).GetValue());
		CCLOG("Effict volum change to %f", ((GuanDanSlider&)kWidget).GetValue());
	}
	if (u32State == ODUIWidget::VALUE_CHANGED)
	{
		g_pSoundSystem->SetSoundVolume(((GuanDanSlider&)kWidget).GetValue());
		g_pSaveData->WriteParam("sound_volume", ((GuanDanSlider&)kWidget).GetValue());
		g_pSaveData->Save();
		m_iPreEffectValue = (ODEFloat32)(g_pSaveData->ReadParam("sound_volume", 1.0f));
		if (m_iPreEffectValue == 0)
		{
			pkSpecialEffetCheckBox->SetCheck(false);
		}
		else
		{
			pkSpecialEffetCheckBox->SetCheck(true);
		}

	}
}

UIImplementDelegate(GuanDanGameOption, OnSilenceCheck, kWidget, u32State)
{
	if (u32State == ODUIWidget::VALUE_CHANGE)
	{
		if (pkSilenceCheckBox->IsCheck())
		{
			g_pSoundSystem->SetSwitch(false);
			pkBeijingyinCheckBox->SetCheck(false);
			pkSpecialEffetCheckBox->SetCheck(false);
			g_pSaveData->WriteParam("sound_switch", ODEUInt32(1));
			g_pSaveData->Save();
		}
		else
		{
			pkBeijingyinCheckBox->SetCheck(true);
			pkSpecialEffetCheckBox->SetCheck(true);
			g_pSoundSystem->SetSwitch(true);
			g_pSaveData->WriteParam("sound_switch", ODEUInt32(0));
			g_pSaveData->Save();
		}
	}
}

UIImplementDelegate(GuanDanGameOption, OnSpecialEffectCheck, kWidget, u32State)
{
	if (u32State == ODUIWidget::VALUE_CHANGE)
	{
		m_iPreEffectValue = (ODEFloat32)(g_pSaveData->ReadParam("sound_volume", 1.0f));
		if (pkSpecialEffetCheckBox->IsCheck())
		{
			g_pSaveData->WriteParam("special_effect", ODEUInt32(1));
			g_pSaveData->Save();

			WIDGET(GuanDanGameEffectSlider,effect_slider);
			effect_slider.SetValue(m_iPreEffectValue);
		}
		else
		{
			g_pSaveData->WriteParam("special_effect", ODEUInt32(0));
			g_pSaveData->Save();
			WIDGET(GuanDanGameEffectSlider, effect_slider);
			effect_slider.SetValue(0.0f);
		}
	}
}

UIImplementDelegate(GuanDanGameOption, OnBeijingyinCheck, kWidget, u32State)
{
	if (u32State == ODUIWidget::VALUE_CHANGE)
	{
		m_iPreMusicValue = (ODEFloat32)(g_pSaveData->ReadParam("music_volume", 1.0f));
		if (pkBeijingyinCheckBox->IsCheck())
		{
			g_pSaveData->WriteParam("bg_yin", ODEUInt32(1));
			g_pSaveData->Save();

			WIDGET(GuanDanGameMusicSlider, music_slider);
			music_slider.SetValue(m_iPreMusicValue);
		
		}
		else
		{
			g_pSaveData->WriteParam("bg_yin", ODEUInt32(0));
			g_pSaveData->Save();
			WIDGET(GuanDanGameMusicSlider, music_slider);
			music_slider.SetValue(0.0f);
		}
	}
}

UIImplementDelegate(GuanDanGameOption, OnClose, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		bool bCheckMusic = g_pSaveData->ReadParam("bg_yin", ODEUInt32(1)) ? true : false;

		if (! bCheckMusic)
		{
			g_pSaveData->WriteParam("music_volume", 0.0f);
		}

		bool bCheckEffectSound = g_pSaveData->ReadParam("special_effect", ODEUInt32(1)) ? true : false;
		if (! bCheckEffectSound)
		{
			g_pSaveData->WriteParam("sound_volume", 0.0f);
		}

		g_pLayerManager->Back();
	}
}

UIImplementDelegate(GuanDanGameOption, OnExitGame, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		g_pLayerManager->PopAllLayers();
		g_pLayerManager->PopAllMiddleLayers();
		g_pLayerManager->Back();
	}
}


void GuanDanRechargeGuide::OnActive(const ODUIUserDataPtr& spData)
{
#if (ANDROID_IPTV_CONTROL_USE == 1)
	m_LobbyKeyControlForSingleLayer.initCurSelTips (0);
	schedule (schedule_selector (GuanDanRechargeGuide::LayerControlScheduleUpdate), 0.25f);
	CCNotificationCenter::sharedNotificationCenter()->postNotification("notification_key_control_disable");
#else
#endif
}

void GuanDanRechargeGuide::OnDeactive(const ODUIUserDataPtr& spData)
{
#if (ANDROID_IPTV_CONTROL_USE == 1)
	unschedule (schedule_selector (GuanDanRechargeGuide::LayerControlScheduleUpdate));
	CCNotificationCenter::sharedNotificationCenter()->postNotification("notification_key_control_active");
#endif
}


void GuanDanRechargeGuide::LayerControlScheduleUpdate (ODEFloat32 f32Delta)
{
	m_LobbyKeyControlForSingleLayer.update ();
}

GuanDanRechargeGuide::GuanDanRechargeGuide() : ODUILayerModalExt<GuanDanRechargeGuide>("GuanDan_RechargeGuide")
{
	UIInitDelegate(GuanDanRechargeGuide, OnRecharge);
	UIInitDelegate(GuanDanRechargeGuide, OnCancel);
}

GuanDanRechargeGuide::~GuanDanRechargeGuide()
{
	term();
}

bool GuanDanRechargeGuide::init()
{
	setPosition(ccp(ui_w(0.5), ui_h(0.5)));

	CCScale9Sprite* pkBg = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-01.png");
	pkBg->setPreferredSize(CCSizeMake(500, 300));
	pkBg->setScale(ui_main_scale);
	addChild(pkBg);

	ODEFloat32 bg_width = pkBg->getPreferredSize().width;
	ODEFloat32 bg_height = pkBg->getPreferredSize().height;

	CCScale9Sprite* pCenterBg = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pCenterBg->setAnchorPoint(ccp(0.5, 1));
	pCenterBg->setPreferredSize(CCSize(492, 140));
	pCenterBg->setPosition(ccp(bg_width / 2, bg_height - 54));
	pkBg->addChild(pCenterBg);

	CCScale9Sprite* pCenterBg2 = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pCenterBg2->setAnchorPoint(ccp(0.5, 1));
	pCenterBg2->setPreferredSize(CCSize(492, 90));
	pCenterBg2->setPosition(ccp(bg_width / 2, pCenterBg->getPositionY() - pCenterBg->getContentSize().height + 3));
	pkBg->addChild(pCenterBg2);

	CCLabelTTF* pTipText = CCLabelTTF::create(TT("UIText_PayGuide_Tip1"), TTF_NAME, 30, CCSizeMake(400, 46), kCCTextAlignmentCenter);
	pTipText->setAnchorPoint(ccp(0.5, 0.5));
	pTipText->setPosition(ccp(pCenterBg->getPositionX(), pCenterBg->getPositionY() - pCenterBg->getContentSize().height / 2));
	pkBg->addChild(pTipText);

	CCScale9Sprite* pkCancelNoraml = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_alert_cancel_nromal.png");
	pkCancelNoraml->setPreferredSize (CCSizeMake (160, 80.0f));
	CCScale9Sprite* pkCancelPress = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_alert_cancel_press.png");
	pkCancelPress->setPreferredSize (CCSizeMake (160, 80.0f));

	ODUIButton* pkCancelBtn = ODE_NEW ODUIButton(pkCancelNoraml, pkCancelPress);
	pkCancelBtn->setPosition(ccp(-110 * ui_main_scale, (-bg_height / 2 + 63) * ui_main_scale));
	pkCancelBtn->setScale(ui_main_scale);
	AddWidget("kCancelBtn", pkCancelBtn);
	UILinkDelegate(pkCancelBtn, OnCancel);

#if (ANDROID_IPTV_CONTROL_USE == 1)
	CCScale9Sprite * cancelTips = CCScale9Sprite::create ("image/keyController.png", CCRect (0, 0, 44, 42), CCRect(21, 20, 2, 2) );
	cancelTips->setPreferredSize (CCSizeMake(165.0f, 85.0f));
	cancelTips->setPosition (ccp (0.0f, 1.5f));
	pkCancelBtn->addChild (cancelTips);
	cancelTips->setVisible (false);
	KeyControlUnion * controlUnion0 = new KeyControlUnion  ();
	controlUnion0->curSelFlags = cancelTips;
	controlUnion0->curPos = 0;
	controlUnion0->upPos = 0;
	controlUnion0->leftPos = 1;
	controlUnion0->downPos = 0;
	controlUnion0->rightPos = 1;
	controlUnion0->setControlCallback (this,keyControl_selector (GuanDanRechargeGuide::OnCancel),pkCancelBtn);
	m_LobbyKeyControlForSingleLayer.addKeyControlUnion (controlUnion0);
#endif

	CCLabelTTF* pkCancelText = CCLabelTTF::create(TT("Cancel"), TTF_NAME, 36);
	pkCancelText->setAnchorPoint(ccp(0.5, 0.5));
	pkCancelBtn->addChild(pkCancelText, 2);

	CCScale9Sprite* pkEnterNoraml = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_alert_confirm_normal.png");
	pkEnterNoraml->setPreferredSize (CCSizeMake (175, 80.0f));
	CCScale9Sprite* pkEnterPress = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_alert_confirm_press.png");
	pkEnterPress->setPreferredSize (CCSizeMake (175, 80.0f));

	ODUIButton* pkEnterBtn = ODE_NEW ODUIButton(pkEnterNoraml, pkEnterPress);
	pkEnterBtn->setPosition(ccp(110 * ui_main_scale, (-bg_height / 2 + 63) * ui_main_scale));
	pkEnterBtn->setScale(ui_main_scale);
	AddWidget("kEnterBtn", pkEnterBtn);
	UILinkDelegate(pkEnterBtn, OnRecharge);

#if (ANDROID_IPTV_CONTROL_USE == 1)
	CCScale9Sprite * rechargeTips = CCScale9Sprite::create ("image/keyController.png", CCRect (0, 0, 44, 42), CCRect(21, 20, 2, 2) );
	rechargeTips->setPreferredSize (CCSizeMake(180.0f, 85.0f));
	rechargeTips->setPosition (ccp (0.0f, 1.5f));
	pkEnterBtn->addChild (rechargeTips);
	rechargeTips->setVisible (false);
	KeyControlUnion * controlUnion1 = new KeyControlUnion  ();
	controlUnion1->curSelFlags = rechargeTips;
	controlUnion1->curPos = 1;
	controlUnion1->upPos = 1;
	controlUnion1->leftPos = 0;
	controlUnion1->downPos = 1;
	controlUnion1->rightPos = 0;
	controlUnion1->setControlCallback (this,keyControl_selector (GuanDanRechargeGuide::OnRecharge),pkEnterBtn);
	m_LobbyKeyControlForSingleLayer.addKeyControlUnion (controlUnion1);
#endif


	CCLabelTTF* pkEnterText = CCLabelTTF::create(TT("UIText_PayGuide_Btn_Text"), TTF_NAME, 36);
	pkEnterText->setAnchorPoint(ccp(0.5, 0.5));
	pkEnterBtn->addChild(pkEnterText, 2);

	return ODUILayerModalExt<GuanDanRechargeGuide>::init();
}

UIImplementDelegate(GuanDanRechargeGuide, OnRecharge, kWidget, u32State)
{
	if(u32State == ODUIWidget::RELEASED)
	{
		g_pkGuanDanGame->addChangeIndexToQuene (LAYER_MANAGER_QUENE_CHARGE_MENU);
	}
}

UIImplementDelegate(GuanDanRechargeGuide, OnCancel, kWidget, u32State)
{
	if(u32State == ODUIWidget::RELEASED)
	{
		//g_pkGuanDanGame->addChangeIndexToQuene (LAYER_MANAGER_QUENE_MAIN_LAYER);
		g_pLayerManager->PopAllLayers ();
	}
}

ODEInt32 GuanDanGameTools::SwitchViewChairID(ODEInt32 nChair)
{

	int iPersoalPos = g_pkGuanDanGamePlayerData->m_iTableNumExtra;

	int iDis = g_pkGuanDanGamePlayerData->m_iTableNumExtra - nChair;

	// 本玩家始终在 POSITION_BOTTOM 的相对位置
	int iPos = GUANDAN_POSITION_BOTTOM - iDis;

	if (iDis > 0 && iPos < 0)
	{
		iPos = iPos + GUANDAN_TABLE_PLAYER_NUM;
	}

	if (iDis < 0 && iPos > (GUANDAN_TABLE_PLAYER_NUM - 1))
	{
		iPos = iPos - GUANDAN_TABLE_PLAYER_NUM;
	}

	return iPos;
}

ODEInt32 GuanDanGameTools::GetUserCurLevel( ODEInt32 nExperice )
{
	for (int i = 0;i<TP_ALL_LEVEL_NUM;i++)
	{
		if (nExperice < GUANDAN_ALLEXPERICE[i])
		{
			return i+1;
		}
	}

	if(nExperice >= GUANDAN_ALLEXPERICE[TP_ALL_LEVEL_NUM - 1])
		return TP_ALL_LEVEL_NUM + 1;

	return 1;
}

std::string GuanDanGameTools::GetFormatNumberString( long long iNumber )
{
	long long absNum = labs(iNumber);

	char szBuffer[ 256 ] = { 0 };
	sprintf( szBuffer, "%d", absNum );

	std::string strResult = szBuffer;

	if (iNumber != 0)
	{
		strResult.insert( 0, 1, '.' );
	}

	//从后往前，每3位添加一个','
	//除了前面的符号（+，-）
	for( int i = strResult.length() - 3; i > 1; i -= 3 )
	{
		strResult.insert( i, 1, '/' );
	}

	return strResult;
}

GuanDanOPTButton::GuanDanOPTButton (CCNode * normal, CCNode * press, CCNode * icon, CCNode * disable, CCNode * icon2) : ODUIButton (normal, press, disable), m_pkIcon (NULL), m_pkIcon2 (NULL)
{
	m_pkIcon = icon;
	m_pkIcon2 = icon2;

	if (m_pkIcon2 == NULL)
	{
		addChild (m_pkIcon, 4, 100);
	}
	else
	{
		addChild (m_pkIcon, 4, 100);
		addChild (m_pkIcon2, 4, 101);
		m_pkIcon->setPosition (ccp (m_pkNormal->boundingBox ().getMinX () + 59 + m_pkIcon->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));
		m_pkIcon2->setPosition (ccp (m_pkIcon->boundingBox ().getMaxX () + 6 + m_pkIcon2->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));
	}
}

void GuanDanOPTButton::resetIcon (CCNode *  icon)
{
	if (m_pkIcon != NULL)
	{
		removeChild (m_pkIcon);
	}

	m_pkIcon = icon;

	if (m_pkIcon2 == NULL)
	{
		addChild (m_pkIcon, 4);
	}
	else
	{
		addChild (m_pkIcon, 4);
		m_pkIcon->setPosition (ccp (m_pkNormal->boundingBox ().getMinX () + 59 + m_pkIcon->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));
	}
}

void GuanDanOPTButton::setOPTButtonIcon (CCNode * icon)
{
	CCPoint pos ;
	if (m_pkIcon != NULL)
	{
		pos = m_pkIcon->getPosition ();
		removeChild (m_pkIcon);
	}
	m_pkIcon = icon;
	addChild (m_pkIcon);
	m_pkIcon->setPosition (pos);
}

GuanDanChatButton::GuanDanChatButton (CCNode * normal, CCNode * press, CCNode * icon, CCNode * icon2, CCNode * disable ) : ODUIButton (normal, press, disable), m_pkIcon (NULL), m_pkIcon2 (NULL)
{
	m_pkIcon = icon;
	m_pkIcon2 = icon2;
	addChild (m_pkIcon, 4);
	addChild (m_pkIcon2, 4);
	m_pkIcon->setPosition (ccp (m_pkNormal->boundingBox ().getMinX () + 42 + m_pkIcon->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));
	m_pkIcon2->setPosition (ccp (m_pkIcon->boundingBox ().getMaxX () + 32 + m_pkIcon2->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));
}

GuanDanOPTWaitButton::GuanDanOPTWaitButton (CCNode * normal, CCNode * press, CCNode * icon, CCNode * disable) : ODUIButton (normal, press, disable), m_pkIcon (NULL)
{
	m_pkIcon = icon;
	m_pkIcon->setPosition (ccp (m_pkNormal->boundingBox ().getMaxX () - 35 - m_pkIcon->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));
	addChild (m_pkIcon, 4);
	m_noCheckIcon = CCSprite::create (TP_IMAGE_PATH_INGAEM"auto_opt.png");
	m_checkIcon = CCSprite::create (TP_IMAGE_PATH_INGAEM"auto_opt_sel.png");
	m_noCheckIcon->setPosition (ccp (m_pkNormal->boundingBox ().getMinX () + 15 + m_noCheckIcon->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));
	m_checkIcon->setPosition (ccp (m_pkNormal->boundingBox ().getMinX () + 15 + m_checkIcon->getContentSize ().width / 2, m_pkNormal->boundingBox ().getMidY ()));
	addChild (m_noCheckIcon, 4);
	addChild (m_checkIcon, 4);
	setCheck (false);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"auto_opt.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"auto_opt_sel.png");
}

void GuanDanOPTWaitButton::setOPTWaitButton (CCNode * icon)
{
	if (m_pkIcon != NULL)
	{
		removeChild (m_pkIcon);
	}
	m_pkIcon = icon;
	addChild (m_pkIcon, 4);
}

void GuanDanOPTWaitButton::setCheck (bool isCheck)
{
	isChecked = isCheck;
	if (isChecked)
	{
		m_checkIcon->setVisible (true);
	}
	else
	{
		m_checkIcon->setVisible (false);
	}
}

void GuanDanOPTWaitButton::setClick ()
{
	isChecked = !isChecked;
	setCheck (isChecked);
}

static float convertDistanceFromPointToInchGuanDan(float pointDis)
{
	float factor = ( CCEGLView::sharedOpenGLView()->getScaleX() + CCEGLView::sharedOpenGLView()->getScaleY() ) / 2;
	return pointDis * factor / CCDevice::getDPI();
}

//房间列表
GuanDanGameRoomList::GuanDanGameRoomList()
	:  ODUIScrollView(1240 * ui_width_scale, 472 * ui_height_scale,
	"scroll_bean.png", "scroll_bean_ext.png", 3.0f)
{
	setAnchorPoint(ccp(0.5f,1.0f));
	SetScrollBarColor(ccc3(191,194,225));
	SetScrollBarOpacity(250);
	UpdateBounding();
	m_kButtonArray.Reserve(250);
}

GuanDanGameRoomList::~GuanDanGameRoomList()
{
}

ODEUInt32 GuanDanGameRoomList::AddRoomBtn(const ODEChar8* pcRoomName, const ODEChar8* pcServerIP, ODEUInt32 u32ServerPort, ODEUInt64 u64Limit,ODEUInt32 roomid)
{

	ODEUInt32 u32Res = m_kButtonArray.Size();
	CCSprite* room_btn =NULL;
	CCSprite* room_btn_press =NULL;
	CCSprite* room_btn_dis = NULL;
	ODUIButton* pkEvent =NULL;
	switch (roomid % 2)
	{
	case 0:
			room_btn = CCSprite::create(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_0_0.png");
			room_btn_press = CCSprite::create(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_0_1.png");
			room_btn_dis = CCSprite::create(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_0_0.png");
			pkEvent = ODE_NEW ODUIButton(room_btn, room_btn_press, room_btn_dis);
		break;
	case 1:
			room_btn = CCSprite::create(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_1_0.png");
			room_btn_press = CCSprite::create(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_1_1.png");
			room_btn_dis = CCSprite::create(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_1_0.png");
			pkEvent = ODE_NEW ODUIButton(room_btn, room_btn_press, room_btn_dis);
			break;
	default:
			room_btn = CCSprite::create(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_0_0.png");
			room_btn_press = CCSprite::create(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_0_1.png");
			room_btn_dis = CCSprite::create(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_0_0.png");
			pkEvent = ODE_NEW ODUIButton(room_btn, room_btn_press, room_btn_dis);
		break;
	}
		
	pkEvent->setScale(0.8*ui_main_scale);
	
	ODEFloat32 f32Width = getContentSize().width;
	ODEUInt32 u32Flag = u32Res & 1;
	ODEUInt32 u32Row = (u32Res >> 1) + 1;
	pkEvent->SetIndex(u32Res);

	if (u32Flag)
	{
		pkEvent->setPosition(ccp(ODEFloorf(f32Width * 0.5f + 290.0f * ui_width_scale),
			-ODEFloorf(ODEFloat32(u32Row) * 110.0f * g_f32ScaleHD * ui_height_scale)));
	}
	else
	{
		ODEInt32 room_num = 1;
		room_num = atoi(GetConfig("GD_AllRoomCount"));

		if (room_num == 1)
		{
			pkEvent->setPosition(ccp(ODEFloorf(f32Width * 0.5f), 0));
		}
		else
		{
			pkEvent->setPosition(ccp(ODEFloorf(f32Width * 0.5f - 290.0f * ui_width_scale),
				-ODEFloorf(ODEFloat32(u32Row) * 110.0f * g_f32ScaleHD * ui_height_scale)));
		}
	}
	
	CCLabelBMFont* pkRoomTitle = CCLabelBMFont::create(pcRoomName, "font/room.fnt");
	pkRoomTitle->setPosition(ccp(100.0f,40));
	pkEvent->addChild(pkRoomTitle, 2);

	//"推荐"
	//CCLabelBMFont* pkPlayerNumText = CCLabelBMFont::create(TT("RoomPeopleNum"), "font/room.fnt");
	//pkPlayerNumText->setPosition(ccp(140.0f, -40.0f));
	//pkPlayerNumText->setColor(ccc3(242, 188, 0));
	//pkPlayerNumText->setScale(0.7);
	//pkEvent->addChild(pkPlayerNumText, 2, 1000);

	//"人数"
	CCLabelTTF* pkPlayerNum = CCLabelTTF::create(TT("RoomPeopleNum"), TTF_NAME, 33);
	pkPlayerNum->setPosition(ccp(140.0f, -40.0f));
	pkPlayerNum->setColor(ccc3(242, 188, 0));
	pkEvent->addChild(pkPlayerNum, 2, 1000);

	AddChild(pkEvent,9);
	m_kButtonArray.Resize(m_kButtonArray.Size() + 1);
	Item& kItem = m_kButtonArray.Back();
	kItem.m_pkButton = pkEvent;
	kItem.m_kDelegate.Set(this, &GuanDanGameRoomList::OnClicked);
	kItem.m_pkButton->RegisterListener(kItem.m_kDelegate);
	kItem.m_LobbyRoomInfo.m_nServerPort = u32ServerPort;
	kItem.m_LobbyRoomInfo.m_u64LimitScore = u64Limit;
	ODESprintf(kItem.m_LobbyRoomInfo.m_szServerIP, 128, "%s", pcServerIP);

	return u32Res;
}

void GuanDanGameRoomList::ClearAll()
{
	for (ODEVector<Item>::iterator it = m_kButtonArray.Begin(); it != m_kButtonArray.End(); ++it)
	{
		(it)->m_pkButton->SetVisible(false);
	}
	m_kButtonArray.Clear();
	UpdateBounding();
}

void GuanDanGameRoomList::OnClicked( ODUIWidget& kWidget, ODEUInt32 u32State )
{
	if(u32State == RELEASED)
	{
		ODEUInt32 u32Index = ((ODUIButton&)kWidget).GetIndex();
		ODEUInt32 u32State = u32Index + 1;
		u32State |= 0x80000000;
		m_kEvent.Callback(*this, u32State);
	}
}

GuanDanGameRoomList::LobbyRoomInfo GuanDanGameRoomList::GetRoomInfo( ODEUInt32 u32Index )
{
	return m_kButtonArray[u32Index].m_LobbyRoomInfo;
}

void GuanDanGameRoomList::RefreshList()
{
	ClearAll();
	ODEInt32 i32RoomCount = ODEAtoi(GetConfig("GD_AllRoomCount"));
	for (ODEInt32 i(0); i < i32RoomCount; ++i)
	{
		ODEChar8 acBuffer[64];
		ODESprintf(acBuffer, 64, "%s%d", "GD_NormalRoomIp_", i);
		const ODEChar8* pcIp = GetConfig(acBuffer);
		ODESprintf(acBuffer, 64, "%s%d", "GD_NormalRoomPort_", i);
		ODEUInt32 i32Port = ODEAtoi(GetConfig(acBuffer));
		ODESprintf(acBuffer, 64, "%s%d", "GD_NormalRoomTitleImage_", i);
		const ODEChar8* pcTitleImageName = GetConfig(acBuffer);
		ODESprintf(acBuffer, 64, "%s%d", "GD_NormalRoomScoreLimit_", i);
		ODEUInt32 u64Limit = ODEAtoi(GetConfig(acBuffer));
		ODESprintf(acBuffer, 64, "%s%d", "GD_NormalRoomBaseScore_", i);
		ODEUInt32 u64Base = ODEAtoi(GetConfig(acBuffer));

		AddRoomBtn(pcTitleImageName, pcIp, i32Port, u64Limit,i);
	}
	
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_0_0.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_0_1.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_1_0.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_ROOMCHOICE"room_1_1.png");
}

void GuanDanGameRoomList::EnableAllRoomBtn( bool bEnable )
{
	for (ODEVector<Item>::iterator it = m_kButtonArray.Begin(); it != m_kButtonArray.End(); ++it)
	{
		(it)->m_pkButton->SetEnable(bEnable);
	}
}

void GuanDanGameRoomList::UpdatePlayerNum()
{
	ODEInt32 i32RoomCount = ODEAtoi(GetConfig("GD_AllRoomCount"));
	int iServerID = 0;
	int playersNums = 0;
	char temp[50] = {0};

	for (int i = 0; i < i32RoomCount; ++i)
	{
		sprintf(temp, "GD_NormalRoomID_%d", i);

		iServerID = atoi(GetConfig(temp));

		//真实人数
		playersNums = g_pkClientData->getPoepleCountWithRoomId(iServerID);

		CCLabelTTF* pPlayerNumLabel = (CCLabelTTF*)m_kButtonArray[i].m_pkButton->getChildByTag(1000);

		if (pPlayerNumLabel && playersNums != 0)
		{
			if (strcmp (GetConfig ("Game_PlayerNums_Control"), "1") == 0)
			{
				//加上基数控制
				sprintf(temp, "GD_NormalRoomBasePeopleCount_%d", i);
				playersNums = playersNums + atoi(GetConfig(temp));
			}

			sprintf(temp, "%d", playersNums);

			CCLabelTTF* pPlayerNum = (CCLabelTTF*)m_kButtonArray[i].m_pkButton->getChildByTag(1000);
			if (pPlayerNum)
			{
				pPlayerNum->setString(temp);
			}
		}
		else if (pPlayerNumLabel)
		{
			pPlayerNumLabel->setString(TT("RoomPeopleNum"));
		}
	}
}

GuanDanGameRoomList::Item::Item()
{
	m_pkButton = NULL;
}


/*
	TexasPoker 桌子列表
*/
GuanDanGameTableList::GuanDanGameTableList()
{
	m_iTableNum = atoi(GetConfig("GD_AllTableCount"));

	if (0 == m_iTableNum)
	{
		m_iTableNum = KESSEN_All_TABLE_COUNT;
	}

	scrollViewSize = CCSizeMake(1280 * ui_width_scale, 470 * ui_height_scale);

	if (m_iTableNum % 4 == 0)
	{
		scrollViewContentSize = CCSizeMake(1280 * ui_width_scale, ( m_iTableNum / 4 + 1) * 290 * ui_height_scale);
	}
	else
	{
		scrollViewContentSize = CCSizeMake(1280 * ui_width_scale, ( m_iTableNum / 4 + 2) * 290 * ui_height_scale);
	}

	scrollView = GuanDanTableScrollView::create();  

	CCNode* containLayer = CCNode::create();

	for (int i(0); i < m_iTableNum; ++i)
	{
		GuanDanGameTableCell* gameTableCell = GuanDanGameTableCell::create();
		gameTableCell->setPosition(ccp(scrollViewSize.width / 2 -160* ui_width_scale + (i % 4 - 1)* (scrollViewSize.width / 4 + 1 * ui_width_scale), scrollViewContentSize.height - 5* ui_height_scale - 140 * ui_height_scale - (i / 4) * 295 * ui_height_scale));
		gameTableCell->setTableIndex(i + 1);
		gameTableCell->setDelegate(this);
		gameTableCell->updateVisualRect(CCRectMake((1280 * ui_width_scale - scrollViewSize.width) / 2, (720 * ui_height_scale - scrollViewSize.height) / 2,
			scrollViewSize.width, scrollViewSize.height));
		containLayer->addChild(gameTableCell, 1, 1000+i);
	}

	scrollView->setContainer(containLayer);
	scrollView->setContentSize(CCSizeMake(scrollViewSize.width, scrollViewContentSize.height));
	scrollView->setContentOffset(ccp(0.0f, scrollViewSize.height - scrollViewContentSize.height));

	scrollView->setViewSize(scrollViewSize);
	scrollView->setPosition(ccp(-scrollViewSize.width / 2, 0));
	scrollView->setDirection(kCCScrollViewDirectionVertical);
	scrollView->setBounceable(false); 
	addChild(scrollView);
	setContentSize(CCSizeMake((1280 - 200) * ui_width_scale, (720 - 100) * ui_height_scale));
}

GuanDanGameTableList::~GuanDanGameTableList()
{
}

void GuanDanGameTableList::selectCallback(unsigned short tableNum, unsigned short usTableNumExtra)
{
	if (g_pkClientData->GetUserScore() < GUANDAN_ENTER_GAME_MIN_SCORE)
	{
		/*GuanDanTableLayer * pkTemp = (GuanDanTableLayer*)g_pLayerManager->GetLayer("GuanDan_TableLayer");
		pkTemp->getGameTableList()->SetTouchActive(false);*/

		if (!g_pLayerManager->IsExitInLayer("RechargeGuide"))
		{
			g_pLayerManager->PushLayer("RechargeGuide");
		}
		return;
	}

	TcpClientSocket *ptempGameSocket = g_pkNetworkHelper->GetGameServerDelegate();
	if (ptempGameSocket && ptempGameSocket->IsConnect() == false)
	{
		g_pkGuanDanGame->Toast(TT("UIText_GameUI_KickOut_154"));
		return;
	}

	unsigned long roomBasePoint = getBasePointWithTableSeat(tableNum, usTableNumExtra);
	SendSitTableMsg(tableNum, usTableNumExtra);
}

void GuanDanGameTableList::SendSitTableMsg( unsigned short tableNum, unsigned short usTableNumExtra )
{

	tagPUSTableInfo *pVectorTableInfo = g_pkGuanDanGamePlayerData->m_vPUSTableInfo.at(tableNum);
	if (pVectorTableInfo)
	{
		if (pVectorTableInfo->iTableUserCount >= GUANDAN_TABLE_PLAYER_NUM)
		{
			g_pkGuanDanGame->Toast(TT("UIToast_TableFullOfPlayer"));
		}
		else if (pVectorTableInfo->iTableUserCount>0)
		{

			if (pVectorTableInfo->bIsSetPassword)
			{
				g_pkGuanDanGame->setGameTableListEnable (false);

				//记录当前桌位号
				g_pkClientData->setJoinPwTableNum(tableNum);
				//push 输入密码框
				if (g_pLayerManager->GetLayer ("GuanDan_pwtable_enter") != NULL)
				{
					g_pLayerManager->PushLayer("GuanDan_pwtable_enter");
				}
				else
				{
					g_pLayerManager->AddLayer(GuanDanPwTableEnter::create());	
					g_pLayerManager->PushLayer("GuanDan_pwtable_enter");
				}
				return;
			}
			TcpClientSocket *pGameSocket = g_pkNetworkHelper->GetGameServerDelegate();
			if (pGameSocket->IsConnect())
			{
				PUSRoomExJoinTableReq JoinTableReq={0};
				JoinTableReq.iTableID = tableNum;

				GuanDanTableLayer* pkTemp = (GuanDanTableLayer*)g_pLayerManager->GetLayer("GuanDan_TableLayer");
				pkTemp->getGameTableList()->SetTouchActive(false);
				g_pkGuanDanGame->ShowLoading();

				g_pkGuanDanGame->startCheckSocket(SOCKETINDEX_GAME);
				pGameSocket->SendMsgToSocket(&JoinTableReq, GAMEFCT_MSG_PUSEX_JOIN_TABLE_REQ, sizeof(JoinTableReq));
			}
		} 
		else
		{
			ODEInt64 score=g_pkClientData->GetUserScore();			
			if (score/10>=10)
			{
				g_pkGuanDanGame->setGameTableListEnable (false);

				//设置底注

				if (g_pLayerManager->GetLayer("GuanDan_SetBasePointLayer") == NULL)
				{
					g_pLayerManager->AddLayer (GuanDanSetBasePointLayer::create ());
				}

				GuanDanSetBasePointLayer * baseset = (GuanDanSetBasePointLayer*)g_pLayerManager->GetLayer("GuanDan_SetBasePointLayer");
				baseset->setUserTableAnteBufferTableId(tableNum);
				g_pLayerManager->PopAllLayers();
				g_pLayerManager->PushLayer("GuanDan_SetBasePointLayer");
			}
			else
			{
				if (!g_pLayerManager->IsExitInLayer("RechargeGuide"))
				{
					g_pLayerManager->PushLayer("RechargeGuide");
				}
				return;
			}
		}
	}
}


GuanDanGameTableCell *GuanDanGameTableList::GetGameTable( int iTableID )
{
	CCNode* containLayer = dynamic_cast<CCNode*>(scrollView->getContainer());
	if (containLayer)
	{
		int tag = 1000 + iTableID;
		GuanDanGameTableCell* gameTableCell = (GuanDanGameTableCell*)containLayer->getChildByTag(tag);
	return gameTableCell;
	}
}

void GuanDanGameTableCell::SetTableInfo( bool bInGame, bool bSetPassword, int iTableAnte, int iTableUserCount )
{
	is_setPw = bSetPassword;
		
	CCLabelAtlas* tableIndexLabel = (CCLabelAtlas*)getChildByTag(20);
	char buffer[100] = {0};
	sprintf(buffer, "%d", iTableAnte);
	tableIndexLabel->setString(buffer);

	CCSprite* dizhuText = (CCSprite*)getChildByTag(19);
	CCSprite* leftbgLabel = (CCSprite*)getChildByTag(17);
	CCSprite* rightbgLabel = (CCSprite*)getChildByTag(18);
	CCSprite* table_lock = (CCSprite*)getChildByTag(2009);

	if (bSetPassword)
	{
		table_lock->setVisible(true);
	} 
	else
	{
		table_lock->setVisible(false);
	}

	if (iTableAnte == 0)
	{
		tableIndexLabel->setVisible(false);
		dizhuText->setVisible(false);
		leftbgLabel->setVisible(false);
		rightbgLabel->setVisible(false);
		table_lock->setVisible(false);
	}
	else
	{
		tableIndexLabel->setVisible(true);
		if (!bSetPassword)
		{
		dizhuText->setVisible(true);
		}
		leftbgLabel->setVisible(true);
		rightbgLabel->setVisible(true);
	}
}
void GuanDanGameTableList::updateTableBasePoint(unsigned short tableNum, ODEInt32 basePoint)
{
	CCNode* containLayer = dynamic_cast<CCNode*>(scrollView->getContainer());
	if (containLayer)
	{
		int tag = 1000 + tableNum - 1;
		GuanDanGameTableCell* gameTableCell = (GuanDanGameTableCell*)containLayer->getChildByTag(tag);
		if (gameTableCell)
		{
			gameTableCell->updateBase(basePoint);
		}
	}
}

void GuanDanGameTableList::updateHeadImg(unsigned short tableNum, unsigned short usTableNumExtra, int id, int index)
{
	CCNode* containLayer = dynamic_cast<CCNode*>(scrollView->getContainer());
	if (containLayer)
	{
		int tag = 1000 + tableNum - 1;
		GuanDanGameTableCell* gameTableCell = (GuanDanGameTableCell*)containLayer->getChildByTag(tag);
		if (gameTableCell)
		{
			gameTableCell->updateHeadImg(usTableNumExtra, index);
		}
	}
}

bool GuanDanGameTableList::HasPlayerInSeat(unsigned short tableNum, unsigned short usTableNumExtra)
{
	CCNode* containLayer = dynamic_cast<CCNode*>(scrollView->getContainer());
	if (containLayer)
	{
		int tag = 1000 + tableNum - 1;
		GuanDanGameTableCell* gameTableCell = (GuanDanGameTableCell*)containLayer->getChildByTag(tag);
		if (gameTableCell)
		{
			return gameTableCell->HasPlayerInSeat(usTableNumExtra);
		}
	}
	return false;
}

int GuanDanGameTableList::getCurrentTableCount()
{
	return m_iTableNum;
}

int GuanDanGameTableList::getCurrentSeatCount()
{
	return 4;
}

void GuanDanGameTableList::setViewSize(ODEFloat64 viewH, ODEFloat64 viewW)
{
	scrollView->setViewSize(CCSizeMake(viewH,viewW));
}

ODEInt32 GuanDanGameTableList::getBasePointWithTableSeat(unsigned short tableNum, unsigned short seatNum)
{
	CCNode* containLayer = dynamic_cast<CCNode*>(scrollView->getContainer());
	if (containLayer)
	{
		int tag = 1000 + tableNum - 1;
		GuanDanGameTableCell* gameTableCell = (GuanDanGameTableCell*)containLayer->getChildByTag(tag);
		if (gameTableCell)
		{
			return gameTableCell->getBasePoint();
		}
	}
	return 0;
}

void GuanDanGameTableList::SetTouchActive( bool bIsActive )
{
	scrollView->setTouchEnabled(bIsActive);
	CCNode* containLayer = dynamic_cast<CCNode*>(scrollView->getContainer());

	for(int i =1000 ;i<1000 + m_iTableNum;i++)
	{
		GuanDanGameTableCell* gameTableCell = (GuanDanGameTableCell*)containLayer->getChildByTag(i);
		gameTableCell->SetMenuEnable(bIsActive);
	}
}

void GuanDanGameTableList::initTableList()
{
	for (int i(0); i < m_iTableNum; ++i)
	{
		updateTableBasePoint(i + 1, 0);
		for (int j(0); j < GUANDAN_TABLE_PLAYER_NUM; ++j)
		{
			updateHeadImg(i + 1, j, 0, -1);
		}	
	}
}

void GuanDanGameTableList::ResetTableView()
{
	scrollView->setContentOffset(ccp(0.0f, scrollViewSize.height - scrollViewContentSize.height));
}

//桌子
GuanDanGameTableCell::GuanDanGameTableCell() : ODUILayerExt<GuanDanGameTableCell>("TexasPoker_GameTableCell"),
	basePoint(0),is_setPw(false)
{
	ODEZeroMemory(m_bPlayerInSeat, sizeof(m_bPlayerInSeat));
	setContentSize(CCSizeMake(208, 128));

	CCSprite* pTableBg = CCSprite::create(TP_IMAGE_PATH_TABLE"tp_table_single_table.png");
	pTableBg->setScale(ui_main_scale);
	pTableBg->setPosition(ccp(1 * ui_width_scale, 10 * ui_height_scale));
	addChild(pTableBg);

	CCSprite * pHeGuan = CCSprite::create (TP_IMAGE_PATH_TABLE"tp_table_heguan.png");
	pHeGuan->setScale(ui_main_scale);
	pHeGuan->setAnchorPoint (ccp (0.5f, 0.0f));
	pHeGuan->setPosition(ccp (pTableBg->boundingBox () .getMidX (), pTableBg->boundingBox ().getMaxY () - 35.0f * ui_main_scale));
	addChild(pHeGuan);

	CCSprite*leftLabelBg = CCSprite::createWithSpriteFrameName("kessen_table_half_bg_label.png");
	leftLabelBg->setAnchorPoint(ccp(1.0f, 0.5f));
	leftLabelBg->setScaleX(ui_width_scale);
	leftLabelBg->setScaleY(ui_height_scale);
	leftLabelBg->setPosition(ccp(0, -5*ui_height_scale));
	leftLabelBg->setTag(17);
	leftLabelBg->setVisible(false);
	addChild(leftLabelBg);

	CCSprite*rightLabelBg = CCSprite::createWithSpriteFrameName("kessen_table_half_bg_label.png");
	rightLabelBg->setAnchorPoint(ccp(0, 0.5f));
	rightLabelBg->setScaleX(ui_width_scale);
	rightLabelBg->setScaleY(ui_height_scale);
	rightLabelBg->setFlipX(true);
	rightLabelBg->setPosition(ccp(-0.5*ui_width_scale,-5*ui_height_scale));
	rightLabelBg->setTag(18);
	rightLabelBg->setVisible(false);
	addChild(rightLabelBg);

	CCSprite* normalImage= CCSprite::create("image/InvisibleBtn.png");
	CCSprite* selectImage= CCSprite::create("image/InvisibleBtn.png");
	CCSprite* disableImage= CCSprite::create("image/InvisibleBtn.png");
	CCMenuItemSprite* centerMenuItemSprite = CCMenuItemSprite::create(normalImage, selectImage, disableImage, this, menu_selector(GuanDanGameTableCell::menuCloseCallback));
	centerMenuItemSprite->setContentSize(CCSizeMake(pTableBg->getContentSize().width , pTableBg->getContentSize().height));
	centerMenuItemSprite->setPosition(ccp(119 * ui_width_scale, 89 * ui_height_scale));

	CCLabelTTF* tableIndexLabel = CCLabelTTF::create("", TTF_NAME, ODEFloorf(20));
	tableIndexLabel->setColor(ccc3(50, 30, 15));
	tableIndexLabel->setPosition(ccp(0, -120 * ui_height_scale));
	tableIndexLabel->setScale(ui_main_scale);
	tableIndexLabel->setTag(10);
	addChild(tableIndexLabel);

	CCSprite* lock = CCSprite::createWithSpriteFrameName("table_lock.png");
	lock->setPositionY(30 * ui_height_scale);
	lock->setScale(ui_main_scale);
	lock->setTag(2009);
	lock->setZOrder(9);
	lock->setVisible(false);
	addChild(lock);

	CCSprite* dizhuText = CCSprite::createWithSpriteFrameName("kessen_table_dizhu_text.png");
	dizhuText->setPositionY(25 * ui_height_scale);
	dizhuText->setScale(ui_main_scale);
	dizhuText->setTag(19);
	dizhuText->setVisible(false);
	addChild(dizhuText);

	CCLabelAtlas* dizhuLabel = CCLabelAtlas::create("00000000000", TP_TABLE_DIZHU_NUM, 20, 24, '0');
	dizhuLabel->setAnchorPoint(ccp(0.5f, 0.5f));
	dizhuLabel->setPositionY(-5 * ui_height_scale);
	dizhuLabel->setString("0");
	dizhuLabel->setScale(ui_main_scale);
	dizhuLabel->setTag(20);
	dizhuLabel->setVisible(false);
	addChild(dizhuLabel);

	ScrollViewMenu* pkmenu = ScrollViewMenu::create();

	for (int i(0); i < GUANDAN_TABLE_PLAYER_NUM; ++i)
	{
		CCSprite* normalImage= CCSprite::create(TP_IMAGE_PATH_TABLE"tp_table_chair.png");
		CCSprite* selectImage= CCSprite::create(TP_IMAGE_PATH_TABLE"tp_table_chair.png");
 		CCSprite* disableImage= CCSprite::create(TP_IMAGE_PATH_TABLE"tp_table_chair.png");
		CCMenuItemSprite* menuItemSprite = CCMenuItemSprite::create(normalImage, selectImage, disableImage, this, menu_selector(GuanDanGameTableCell::menuCloseCallback));
		menuItemSprite->setScale(ui_main_scale);
		switch (i)
		{
		case TP_CellPos_L1:
			menuItemSprite->setPosition(ccp((119 + 60 + 25) * ui_main_scale, (99 + 65) * ui_main_scale));
			menuItemSprite->setRotation(-135);
			break;
		case TP_CellPos_L2:
			menuItemSprite->setPosition(ccp((119 + 60 + 60) * ui_main_scale, (99) * ui_main_scale));
			menuItemSprite->setRotation(-90);
			break;
		case TP_CellPos_L3:
			menuItemSprite->setPosition(ccp((119 + 60 + 25) * ui_main_scale, (99 - 65) * ui_main_scale));
			menuItemSprite->setRotation(-45);
			break;
		case TP_CellPos_Center:
			menuItemSprite->setPosition(ccp((119) * ui_main_scale, (19) * ui_main_scale));
			menuItemSprite->setRotation(0);
			break;
		case TP_CellPos_R1:
			menuItemSprite->setPosition(ccp((-0 + 35) * ui_main_scale, (99 - 65) * ui_main_scale));
			menuItemSprite->setRotation(45);
			break;
		case TP_CellPos_R2:
			menuItemSprite->setPosition(ccp((-0) * ui_main_scale, (99) * ui_main_scale));
			menuItemSprite->setRotation(90);
			break;
		case TP_CellPos_R3:
			menuItemSprite->setPosition(ccp((-0 + 35) * ui_main_scale, (99 + 65) * ui_main_scale));
			menuItemSprite->setRotation(135);
			break;
		}
		pkmenu->addChild(menuItemSprite, 1, 1000+i);
	}

	for (int i(0); i < GUANDAN_TABLE_PLAYER_NUM; ++i)
	{
		CCSprite* headImg = CCSprite::createWithSpriteFrameName("kessen_blank_head.png");
		headImg->setColor(ccc3(0,255,0));
		switch (i)
		{
		case TP_CellPos_L1:
			headImg->setPosition(ccp((95.0f) * ui_main_scale, (80) * ui_main_scale));
			headImg->setRotation(45.0f);
			break;
		case TP_CellPos_L2:
			headImg->setPosition(ccp((130.0f) * ui_main_scale, (10) * ui_main_scale));
			break;
		case TP_CellPos_L3:
			headImg->setPosition(ccp((95.0f) * ui_main_scale, -60 * ui_main_scale));
			headImg->setRotation(-45.0f);
			break;
		case TP_CellPos_Center:
			headImg->setPosition(ccp(0, -80.0f  * ui_main_scale));
			break;
		case TP_CellPos_R1:
			headImg->setPosition(ccp((-95.0f) * ui_main_scale, -60 * ui_main_scale));
			headImg->setRotation(45.0f);
			break;
		case TP_CellPos_R2:
			headImg->setPosition(ccp((-130.0f) * ui_main_scale, (10) * ui_main_scale));
			break;
		case TP_CellPos_R3:
			headImg->setPosition(ccp((-90.0f) * ui_main_scale, (75.0f) * ui_main_scale));
			headImg->setRotation(-45.0f);
			break;
		}
		headImg->setScale(ui_main_scale);
		addChild(headImg, 2, 1050 + i);
	}

	pkmenu->addChild(centerMenuItemSprite, 1, 1000+4);
	pkmenu->setAnchorPoint(ccp(0.5f, 0.5f));
	pkmenu->setPosition(CCPointMake(-(238 / 2) * ui_main_scale, -(178 / 2) * ui_main_scale));
	addChild(pkmenu, 1, 999);
}

GuanDanGameTableCell::~GuanDanGameTableCell()
{
}

GuanDanGameTableCell* GuanDanGameTableCell::create()
{
	GuanDanGameTableCell *pRet = new GuanDanGameTableCell();
	if (pRet)
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

ODEInt32 GuanDanGameTableCell::GetPlayerCount()
{
	ODEInt32 iCount = 0;

	for (int i = 0; i < GUANDAN_TABLE_PLAYER_NUM; ++i)
	{
		if (m_bPlayerInSeat[i])
		{
			++iCount;
		}
	}

	return iCount;
}

bool GuanDanGameTableCell::HasPlayerInSeat(ODEInt32 iIndex)
{
	return m_bPlayerInSeat[iIndex];
}

void GuanDanGameTableCell::updateVisualRect(CCRect rect)
{
	ScrollViewMenu* menu = (ScrollViewMenu*)getChildByTag(999);
	menu->setVisualRect(rect);
}

void GuanDanGameTableCell::setTableIndex(unsigned short index)
{
	this->tableIndex = index - 1;

	CCLabelTTF* tableIndexLabel = (CCLabelTTF*)getChildByTag(10);
	char buffer[10] = {0};
	sprintf(buffer, "- %d -", index);
	tableIndexLabel->setString(buffer);
}

void GuanDanGameTableCell::setDelegate(GuanDanGameTableList* tableList)
{
	this->gameTableList = tableList;
}

void GuanDanGameTableCell::updateBase(ODEInt32 point)
{
	basePoint = point;

	CCLabelAtlas* tableIndexLabel = (CCLabelAtlas*)getChildByTag(20);
	char buffer[100] = {0};
	sprintf(buffer, "%d", point);
	tableIndexLabel->setString(buffer);

	CCSprite* dizhuText = (CCSprite*)getChildByTag(19);
	CCSprite* leftbgLabel = (CCSprite*)getChildByTag(17);
	CCSprite* rightbgLabel = (CCSprite*)getChildByTag(18);
	CCSprite* table_lock = (CCSprite*)getChildByTag(2009);
	if (point == 0)
	{
		tableIndexLabel->setVisible(false);
		dizhuText->setVisible(false);
		leftbgLabel->setVisible(false);
		rightbgLabel->setVisible(false);
		table_lock->setVisible(false);

		for (int i = 0; i < GUANDAN_TABLE_PLAYER_NUM; i++)
		{
			updateHeadImg(i, TP_HEAD_IMA_NULL);
		}
	}
	else
	{
		tableIndexLabel->setVisible(true);
		if (!is_setPw)
		{
		dizhuText->setVisible(true);
		}
		leftbgLabel->setVisible(true);
		rightbgLabel->setVisible(true);
	}
}

void GuanDanGameTableCell::menuCloseCallback(CCObject* pSender)
{
	CCMenuItemSprite* menuItem = (CCMenuItemSprite*)pSender;
	int tag = menuItem->getTag();
	if (tag >= 1000 && tag < 1000 + GUANDAN_TABLE_PLAYER_NUM)
	{
		if (gameTableList)
		{
			gameTableList->selectCallback(tableIndex, tag - 1000);
		}
	}
	else if (tag == 1000 + GUANDAN_TABLE_PLAYER_NUM)
	{
		ODEInt32 iIndex = rand() % 4;

		if (GetPlayerCount() == 4)
		{
			if (gameTableList)
			{
				gameTableList->selectCallback(tableIndex, iIndex);
			}
		}
		else
		{
			while (HasPlayerInSeat(iIndex))
			{
				iIndex = rand() % 4;
			}
			if (gameTableList)
			{
				gameTableList->selectCallback(tableIndex, iIndex);
			}
		}		
	}
}
//新的入座头像更新
void GuanDanGameTableCell::setHeadImg( unsigned short usTableNumExtra, int id, int index)
{
	TPtableHeadImg imgIndex = TP_HEAD_IMA_NULL;
	CCSprite* headimgLabel = (CCSprite*)getChildByTag(1050 + usTableNumExtra);
	if (headimgLabel)
	{
		char filename[100];
		if (id == 0)
		{
			sprintf (filename, "kessen_blank_head.png");
		}
		else
		{
			sprintf (filename, "mini_head_man_%d.png", index);
			CCSpriteFrame * spriteFrame =  CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName (filename);
			if (spriteFrame == NULL)
			{
				sprintf (filename, "kessen_blank_head.png");
			}
		}
		headimgLabel->initWithSpriteFrameName(filename);
	}
}

void GuanDanGameTableCell::updateHeadImg(unsigned short usTableNumExtra, int imgIndex)
{
	CCSprite* headimgLabel = (CCSprite*)getChildByTag(1050 + usTableNumExtra);
	if (headimgLabel)
	{
		char filename[100];
		sprintf (filename, "mini_head_man_%d.png", imgIndex);
		CCSpriteFrame * spriteFrame =  CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName (filename);
		if (spriteFrame == NULL)
		{
			m_bPlayerInSeat[usTableNumExtra] = false;
			sprintf (filename, "kessen_blank_head.png");
		}
		else
		{
			m_bPlayerInSeat[usTableNumExtra] = true;
		}
		headimgLabel->initWithSpriteFrameName(filename);
	}
}

ODEInt32 GuanDanGameTableCell::getBasePoint()
{
	return basePoint;
}

void GuanDanGameTableCell::SetMenuEnable( bool isEnable)
{
	ScrollViewMenu* pkMenu = (ScrollViewMenu*)getChildByTag(999);
	pkMenu->setEnabled(isEnable);
}

GuanDanSlider::GuanDanSlider( const ODEChar8* pcHandle, const ODEChar8*pcHandleCenterL,const ODEChar8*pcHandleCenterR, ODEFloat32 f32Range ,CCSize spbgsize,CCSize splightsize,const ODEChar8* pcBar /*= NULL*/, const ODEChar8* pcBarLight /*= NULL*/ )
	: m_f32Range(f32Range), m_f32Value(0.0f), m_pkBarLight(NULL), m_bChange(false)
{
	m_pkHandle = CCSprite::create(pcHandle);
	addChild(m_pkHandle, 3);
	m_bTarget = false;
	m_pkHandle_center = CCSprite::create();
	m_pkHandle_center->setPosition(ccp(m_pkHandle->getContentSize().width/2,m_pkHandle->getContentSize().height/2));
	m_pkHandle->addChild(m_pkHandle_center);
	m_pkHandle_center_l = pcHandleCenterL;
	m_pkHandle_center_r =pcHandleCenterR;
	if(pcBar)
	{
		CCScale9Sprite* pkBar = CCScale9Sprite::create(pcBar);
		pkBar->setPreferredSize(spbgsize);
		addChild(pkBar, 0);
		m_f32BarWidth = pkBar->getContentSize().width;
		if(pcBarLight)
		{
			m_pkBarLight = CCScale9Sprite::create(pcBarLight);
			m_pkBarLight->setPreferredSize(CCSizeZero);
			m_pkBarLight->setAnchorPoint(ccp(0,0.5));
			m_pkBarLight->setPositionX(-splightsize.width/2);
			addChild(m_pkBarLight, 0);

			bar_wight = splightsize.width;
			bar_length = splightsize.height;
		}
		else
		{
			m_pkBarLight = NULL;
		}
	}


	Update();
	CCSize kSize = m_pkHandle->getContentSize();
	kSize.width += m_f32Range * 2.0f;
	kSize.height = kSize.height ;
	setContentSize(kSize);
}

GuanDanSlider::~GuanDanSlider()
{
}

void GuanDanSlider::Update()
{
	ODEFloat32 f32Pos = ODEFloorf(ODELerp(m_f32Value, -m_f32Range, m_f32Range));
	m_pkHandle->setPositionX(f32Pos);
	if(m_pkBarLight)
	{
		ODEFloat32 f32Percentage = (m_f32BarWidth * 0.5f + f32Pos) / m_f32BarWidth;
		f32Percentage = ODE_MIN(f32Percentage, 100.0f);
		f32Percentage = ODE_MAX(f32Percentage, 0.0f);
		if (f32Percentage<0.05f)
		{
			m_pkBarLight->setVisible(false);
			if (f32Percentage <= 0.045f)
			{
				f32Percentage =0;
				m_pkHandle_center->initWithFile(m_pkHandle_center_l);
			}
		}
		else
		{
			m_pkBarLight->setVisible(true);
			m_pkHandle_center->initWithFile(m_pkHandle_center_r);
			if (f32Percentage<0.1)
			{
				m_pkBarLight->setPreferredSize(CCSize((f32Percentage +0.05)*bar_wight,bar_length));
			}
			else
			{
				m_pkBarLight->setPreferredSize(CCSize(f32Percentage*bar_wight,bar_length));
			}
		}
	}
}

void GuanDanSlider::SetValue(ODEFloat32 f32Value)
{
	if(f32Value != m_f32Value)
	{
		m_f32Value = f32Value;
		Update();
		m_kEvent.Callback(*this, VALUE_CHANGE);
	}
}

void GuanDanSlider::SetValueWithEvent(ODEFloat32 f32Value)
{
	if(f32Value != m_f32Value)
	{
		m_f32Value = f32Value;
		Update();
		m_kEvent.Callback(*this, VALUE_CHANGE);
		m_bChange = true;
	}
}

ODEFloat32 GuanDanSlider::GetValue()
{
	return m_f32Value;
}

ODUIWidget* GuanDanSlider::ProcessTouch(ODUILayer::TouchType eType, cocos2d::CCTouch* pkTouch)
{
	ODUIWidget* pkWidget = ODUIWidget::ProcessTouch(eType, pkTouch);
	if(pkWidget) return pkWidget;

	if(m_bEnabled && m_bVisible)
	{
		switch(eType)
		{
		case ODUILayer::TOUCH_BEGAN:
			{
				m_bTarget = true;
				ODEFloat32 f32WidgetX = getPositionX();
				ODEFloat32 f32Touch = pkTouch->getLocation().x;
				ODEFloat32 f32Min = f32WidgetX - m_f32Range * ui_main_scale;
				ODEFloat32 f32Max = f32WidgetX + m_f32Range * ui_main_scale;
				f32Touch = ODE_MAX(f32Min, f32Touch);
				f32Touch = ODE_MIN(f32Max, f32Touch);
				SetValueWithEvent((f32Touch - f32Min) / (f32Max - f32Min));
			}
			break;
		case ODUILayer::TOUCH_MOVED:
			{
				if (!m_bTarget)
					return NULL;
				ODEFloat32 f32WidgetX = getPositionX();
				ODEFloat32 f32Touch = pkTouch->getLocation().x;
				ODEFloat32 f32Min = f32WidgetX - m_f32Range * ui_main_scale;
				ODEFloat32 f32Max = f32WidgetX + m_f32Range * ui_main_scale;
				f32Touch = ODE_MAX(f32Min, f32Touch);
				f32Touch = ODE_MIN(f32Max, f32Touch);
				SetValueWithEvent((f32Touch - f32Min) / (f32Max - f32Min));
			}
			break;
		case ODUILayer::TOUCH_ENDED:
		case ODUILayer::TOUCH_CANCELLED:
			EventRelease();
			break;
		}
		return this;
	}
	else
	{
		return NULL;
	}
}

void GuanDanSlider::EventRelease()
{
	if(m_bChange)
	{
		m_kEvent.Callback(*this, VALUE_CHANGED);
		m_bChange = false;
	}
	m_bTarget = false;
}

void GuanDanSlider::SetSliderContentHeight( ODEFloat32 f32Rate )
{
	CCSize kSize = m_pkHandle->getContentSize();
	kSize.width += m_f32Range * 2.0f;
	kSize.height *= f32Rate;
	setContentSize(kSize);
}

/*
	滚动条
*/
void GuanDanTableScrollView::ccTouchMoved( CCTouch* touch, CCEvent* event )
{
	if (!this->isVisible())
	{
		return;
	}

	if (m_pTouches->containsObject(touch))
	{
		if (m_pTouches->count() == 1 && m_bDragging)
		{ 
			CCPoint moveDistance, newPoint, maxInset, minInset;
			CCRect  frame;
			float newX, newY;

			frame = getViewRect();

			newPoint     = this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0));
			moveDistance = ccpSub(newPoint, m_tTouchPoint);

			float dis = 0.0f;
			if (m_eDirection == kCCScrollViewDirectionVertical)
			{
				dis = 1.8*moveDistance.y;
			}
			else if (m_eDirection == kCCScrollViewDirectionHorizontal)
			{
				dis = moveDistance.x;
			}
			else
			{
				dis = sqrtf(moveDistance.x*moveDistance.x + moveDistance.y*moveDistance.y);
			}

			if (!m_bTouchMoved && fabs(convertDistanceFromPointToInchGuanDan(dis)) < MOVE_INCH_GUANDAN )
			{
				return;
			}

			if (!m_bTouchMoved)
			{
				moveDistance = CCPointZero;
			}

			m_tTouchPoint = newPoint;
			m_bTouchMoved = true;

			if (frame.containsPoint(this->convertToWorldSpace(newPoint)))
			{
				switch (m_eDirection)
				{
				case kCCScrollViewDirectionVertical:
					moveDistance = ccp(0.0f, 1.8*moveDistance.y);
					break;
				case kCCScrollViewDirectionHorizontal:
					moveDistance = ccp(moveDistance.x, 0.0f);
					break;
				default:
					break;
				}

				maxInset = m_fMaxInset;
				minInset = m_fMinInset;

				newX     = m_pContainer->getPosition().x + moveDistance.x;
				newY     = m_pContainer->getPosition().y + 1.8*moveDistance.y;

				m_tScrollDistance = moveDistance;
				this->setContentOffset(ccp(newX, newY));
			}
		}
		else if (m_pTouches->count() == 2 && !m_bDragging)
		{
			const float len = ccpDistance(m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
				m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
			this->setZoomScale(this->getZoomScale()*len/m_fTouchLength);
		}
	}
}

//玩家信息栏
GameBottomBar::GameBottomBar()
{
	CCScale9Sprite* personInfoBg=CCScale9Sprite::create(GD_IMAGE_PATH_INGAME"gd_bottomBg.png");
	personInfoBg->setPreferredSize(CCSizeMake(1280,70));
	//personInfoBg->setPosition(ccp(ui_w(0.5f),personInfoBg->getContentSize().height/2*ui_main_scale));
	personInfoBg->setScale(ui_main_scale);
	addChild(personInfoBg,0);
	//玩家昵称图标
	CCSprite* NickName_Icon=CCSprite::create(GD_IMAGE_PATH_INGAME"gd_nameIcon.png");
	NickName_Icon->setPosition(ccp(NickName_Icon->getContentSize().width/2+130,NickName_Icon->getContentSize().height-8));
	personInfoBg->addChild(NickName_Icon,1);

	CCScale9Sprite* pName_Bg=CCScale9Sprite::create(GD_IMAGE_PATH_INGAME"gd_myInfo_bg.png");
	pName_Bg->setPreferredSize(CCSizeMake(200,52));
	pName_Bg->setPosition(ccp(NickName_Icon->getPositionX()+pName_Bg->getContentSize().width/2+ 40,NickName_Icon->getPositionY()));
	personInfoBg->addChild(pName_Bg,1);

	//玩家积分图标
	CCSprite* Score_Icon=CCSprite::create(GD_IMAGE_PATH_INGAME"gd_scoreIcon.png");
	Score_Icon->setPosition(ccp(pName_Bg->getPositionX()+ Score_Icon->getContentSize().width/2+120,NickName_Icon->getPositionY()));
	personInfoBg->addChild(Score_Icon,1);

	CCScale9Sprite* pScore_Bg=CCScale9Sprite::create(GD_IMAGE_PATH_INGAME"gd_myInfo_bg.png");
	pScore_Bg->setPreferredSize(CCSizeMake(200,52));
	pScore_Bg->setPosition(ccp(Score_Icon->getPositionX()+pScore_Bg->getContentSize().width/2+ 40,NickName_Icon->getPositionY()));
	personInfoBg->addChild(pScore_Bg,1);

	//玩家惯蛋点图标
	CCSprite* gdPoint_Icon=CCSprite::create(GD_IMAGE_PATH_INGAME"gd_pointIcon.png");
	gdPoint_Icon->setPosition(ccp(pScore_Bg->getPositionX()+ gdPoint_Icon->getContentSize().width/2+ 120,NickName_Icon->getPositionY()));
	personInfoBg->addChild(gdPoint_Icon,1);

	CCScale9Sprite* pGdPoint_Bg=CCScale9Sprite::create(GD_IMAGE_PATH_INGAME"gd_myInfo_bg.png");
	pGdPoint_Bg->setPreferredSize(CCSizeMake(200,52));
	pGdPoint_Bg->setPosition(ccp(gdPoint_Icon->getPositionX()+pGdPoint_Bg->getContentSize().width/2+ 40,NickName_Icon->getPositionY()));
	personInfoBg->addChild(pGdPoint_Bg,1);

	//玩家徽章图标
	int gdpopint=g_pkGuanDanGamePlayerData->m_PlayerNode[g_pkGuanDanGamePlayerData->m_iTableNumExtra].iSingalGameExperience;
	CCSprite* Badge_Icon=NULL;
	if (gdpopint<=10)
	{
		Badge_Icon=CCSprite::create(GD_IMAGE_PATH_BADAGE"gd_badge0.png");
	}
	else if (gdpopint<=200)
	{
		Badge_Icon=CCSprite::create(GD_IMAGE_PATH_BADAGE"gd_badge1.png");

	}
	else if (gdpopint<=1000)
	{
		Badge_Icon=CCSprite::create(GD_IMAGE_PATH_BADAGE"gd_badge2.png");

	}
	else if (gdpopint<=2000)
	{
		Badge_Icon=CCSprite::create(GD_IMAGE_PATH_BADAGE"gd_badge3.png");

	}
	else if (gdpopint<=5000)
	{
		Badge_Icon=CCSprite::create(GD_IMAGE_PATH_BADAGE"gd_badge4.png");

	}
	else
	{
		Badge_Icon=CCSprite::create(GD_IMAGE_PATH_BADAGE"gd_badge5.png");
	}
	Badge_Icon->setPosition(ccp(pGdPoint_Bg->getPositionX()+Badge_Icon->getContentSize().width/2+120,NickName_Icon->getPositionY()));
	personInfoBg->addChild(Badge_Icon,1);

	CCScale9Sprite* pBadge_Bg=CCScale9Sprite::create(GD_IMAGE_PATH_INGAME"gd_myInfo_bg.png");
	pBadge_Bg->setPreferredSize(CCSizeMake(100,52));
	pBadge_Bg->setPosition(ccp(Badge_Icon->getPositionX()+pBadge_Bg->getContentSize().width/2+ 40,NickName_Icon->getPositionY()));
	personInfoBg->addChild(pBadge_Bg,1);


	//玩家昵称
	m_pkName = CCLabelTTF::create("", TTF_NAME, 26);

	m_pkName->setPosition(ccp(100,25));
	pName_Bg->addChild(m_pkName);

	//玩家积分值
	//m_pkScore = CCLabelAtlas::create("0",GD_IMAGE_PATH_INGAME"gd_score_num.png",22,28,'0');
	m_pkScore=CCLabelTTF::create("",TTF_NAME,26);
	m_pkScore->setPosition(ccp(80,25));
	m_pkScore->setColor(ccc3(255,255,0));
	pScore_Bg->addChild(m_pkScore);

	//玩家惯蛋点值
	//m_GuanDanPoint=CCLabelAtlas::create("0",GD_IMAGE_PATH_INGAME"gd_score_num.png",22,28,'0');
	m_GuanDanPoint=CCLabelTTF::create("",TTF_NAME,26);
	m_GuanDanPoint->setPosition(ccp(100,25));
	m_GuanDanPoint->setColor(ccc3(255,255,0));
	pGdPoint_Bg->addChild(m_GuanDanPoint);

	//玩家称号
	m_chengHao=CCSprite::create(GD_IMAGE_PATH_BADAGE"gd_badge_value0.png");
	m_chengHao->setPosition(ccp(50,25));
	pBadge_Bg->addChild(m_chengHao);
}

GameBottomBar::~GameBottomBar()
{

}



void GameBottomBar::SetUserInfo( const ODEChar8* pcName, ODEUInt64 u64Score,ODEInt64 gdPopint,const ODEChar8* pChenghao)
{
	string tempName = pcName;
	if (strcmp (pcName, " ") == 0 || strcmp (pcName, "") == 0)
		m_pkName->setString (ODETools::getUTF8WithLength(g_pkClientData->GetUserName (), 9).c_str());
	else
		m_pkName->setString (ODETools::getUTF8WithLength(tempName.c_str(), 9).c_str());
	m_pkScore->setString(g_pkGuanDanGame->GetThousandsNum(u64Score));
	m_GuanDanPoint->setString(g_pkGuanDanGame->GetThousandsNum(gdPopint));
	m_chengHao->initWithFile(pChenghao);

}

void GameBottomBar::UpdateHeadImg()
{
	
}

void GameBottomBar::UpdateUserScore(ODEUInt64 u64Score)
{
	m_pkScore->setString(g_pkGuanDanGame->GetThousandsNum(u64Score));
}

//  机器人托管
//---------------------------------------------///

GDRobotAgentLable::GDRobotAgentLable() : ODUILayerModalExt<GDRobotAgentLable>("robot_agent_label")
	,m_bIsAgent(false)
{

}

GDRobotAgentLable::~GDRobotAgentLable()
{
	term();
}

bool GDRobotAgentLable::init()
{
	//setScale(ui_main_scale);

	m_pkRoot = ODE_NEW ODUIWidget;
	//m_pkRoot->setPosition(ccp(ui_w(0.5f),ODEFloor(55.0f * ui_main_scale)));
	addChild(m_pkRoot);

	pkShade = CCScale9Sprite::create(GD_IMAGE_PATH_INGAME"gd_TuoGuan_bg.png");	
	pkShade->setPreferredSize(CCSizeMake(420,60));
	if (ui_main_scale > 1)
	{
		pkShade->setScale(1);
	}
	else
	{
		pkShade->setScale(ui_main_scale);
	}
	
	//pkShade->setPosition(ccp(ui_w(0.5f),ui_h(0.25f)));
	pkShade->setPosition(ccp(ui_w(0.5f),80 * ui_height_scale));
	/*m_pkRoot->*/addChild(pkShade);


	CCSprite* pkLogo = CCSprite::create(GD_IMAGE_PATH_INGAME"gd_cancle_tuoguan.png");
	pkLogo->setPosition(ccp(200,30));
	if (ui_main_scale > 1)
	{
		pkLogo->setScale(1);
	}
	else
	{
		pkLogo->setScale(ui_main_scale);
	}
	pkShade->addChild(pkLogo);

	CCSprite* pRobot=CCSprite::create(GD_IMAGE_PATH_INGAME"gd_robot.png");
	pRobot->setPosition(ccp(430,40));
	if (ui_main_scale > 1)
	{
		pRobot->setScale(1);
	}
	else
	{
		pRobot->setScale(0.7 * ui_main_scale);
	}
	pkShade->addChild(pRobot);
	RobotAnimation();


	return ODUILayerModalExt<GDRobotAgentLable>::init();
}

void GDRobotAgentLable::OnActive( const ODUIUserDataPtr& spData )
{
	//m_pkPanel->PanelFadeIn(FADE_IN_TIME, 0.0f);
	m_bIsAgent = true;
}

void GDRobotAgentLable::OnDeactive( const ODUIUserDataPtr& spData )
{
	m_bIsAgent = false;
	if (m_playerNodeSprite != NULL)
	{
		removeChild (m_playerNodeSprite);
		m_playerNodeSprite = NULL;
	}
}

bool GDRobotAgentLable::OnBack()
{
	return false;
}

void GDRobotAgentLable::term()
{
	ODE_DELETE(m_pkRoot);
	ODUILayer::term();
}

bool GDRobotAgentLable::GetIsRobotAgent()
{
	return m_bIsAgent;
}

void GDRobotAgentLable::RobotAnimation()
{
	CCAnimation* m_playerNodeAnimation = CCAnimation::create();  
	char spriteFrameName[100] = {0};
	m_playerNodeSprite = CCSprite::create(GD_ROBOT_EFFECT"self_robot_0.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(GD_ROBOT_EFFECT"self_robot_0.png");
	pkShade->addChild(m_playerNodeSprite);
	//m_playerNodeSprite->setPosition(ccp(431,60));

	if (ui_main_scale >1)
	{
		m_playerNodeSprite->setScale(1);
		m_playerNodeSprite->setPosition(ccp(431,64));
	}
	else
	{
		m_playerNodeSprite->setScale(0.7 * ui_main_scale);
		if(ui_main_scale==1)
		{
			m_playerNodeSprite->setPosition(ccp(431,60));
		}

		else
		{
			m_playerNodeSprite->setPosition(ccp(431,53));
		}
	}
	
	for (int i = 0; i <4; i++)
	{
		sprintf (spriteFrameName, GD_ROBOT_EFFECT"self_robot_%d.png", i);
		m_playerNodeAnimation->addSpriteFrameWithFileName(spriteFrameName);
		CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
	}

	m_playerNodeAnimation->setDelayPerUnit(0.2f);
	CCAnimate* m_playerNodeAnimate = CCAnimate::create (m_playerNodeAnimation);
	m_playerNodeSprite->runAction (CCRepeatForever::create(m_playerNodeAnimate));
}

bool GDRobotAgentLable::ProcessTouch( TouchType eType, cocos2d::CCTouch* pkTouch )
{
	if (!ODUILayer::ProcessTouch(eType, pkTouch))
	{
		
			switch(eType)
			{
			case ODUILayer::TOUCH_BEGAN:
				//由托管状态变为非托管状态
				CUserTrust msgReq = {0};
				msgReq.nTrustUser = g_pkGuanDanGamePlayerData->m_iTableNumExtra;
				msgReq.bTrust = false; //0 非托管  1托管
				g_pkNetworkHelper->SendTrusteeshipReq((ODEChar8*)&msgReq);
				break;
			}
	}
	return false;
}
