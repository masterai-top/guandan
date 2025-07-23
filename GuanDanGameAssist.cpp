#include "GuanDanGameAssist.h"
#include "GuanDanGameView.h"
#include "GuanDanTableLayer.h"
#include "GuanDanHeaderTips.h"
#include "ODUIGraySprite.h"
#include "GuanDanGuide.h"

#define TP_TASK_PATH "TexasPoker/image/task/"
GuanDanRiverChips::GuanDanRiverChips ()
{
	m_currentScore = 0;
	m_ChipsStringTTF = NULL;
	m_ChipsIcon = NULL;
	m_ChipsBackground = NULL;
}

GuanDanRiverChips * GuanDanRiverChips::create (ODEInt64 score, int pos, bool left)
{
	GuanDanRiverChips* pReturn = new GuanDanRiverChips();
	if ( pReturn && pReturn->createTexeasPokerChips(score, pos, left))
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

bool GuanDanRiverChips::createTexeasPokerChips (ODEInt64 score, int pos, bool left)
{
	m_currentScore = score;
	char iconTex [128] = {0};
	sprintf (iconTex, TP_IMAGE_PATH_INGAEM"tp_scoreicon%d.png", pos);
	m_ChipsIcon = CCSprite::create (iconTex);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(iconTex);

	char counterScore [64] = {0};
	sprintf (counterScore, "%lld", score);
	m_ChipsStringTTF = CCLabelTTF::create(counterScore, TTF_NAME, 20);
	m_ChipsStringTTF->setColor (ccc3 (0xFF, 0xD3, 0x02));

	m_ChipsBackground = CCScale9Sprite::create (TP_IMAGE_PATH_INGAEM"tp_betBack.png", CCRect (0, 0, 30, 26), CCRect (14, 12, 2, 2));
	CCSize backgroundSize = CCSize (m_ChipsIcon->getContentSize ().width + m_ChipsStringTTF->getContentSize ().width + 10.0f,  m_ChipsIcon->getContentSize ().height);
	m_ChipsBackground->setPreferredSize (backgroundSize);
	addChild (m_ChipsBackground, 3);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_betBack.png");
	

	if (left)
	{
		m_ChipsIcon->setPosition (ccp (m_ChipsBackground->boundingBox ().getMaxX () - 1.5f - m_ChipsIcon->getContentSize ().width / 2, 0.0f));
		addChild (m_ChipsIcon, 4);

		m_ChipsStringTTF->setPosition (ccp (m_ChipsIcon->boundingBox ().getMinX () - 1.5f - m_ChipsStringTTF->getContentSize ().width / 2, 0.0f));
		addChild (m_ChipsStringTTF, 4);
	}
	else
	{
		m_ChipsIcon->setPosition (ccp (m_ChipsBackground->boundingBox ().getMinX () + 1.5f + m_ChipsIcon->getContentSize ().width / 2, 0.0f));
		addChild (m_ChipsIcon, 4);

		m_ChipsStringTTF->setPosition (ccp (m_ChipsIcon->boundingBox ().getMaxX () + 1.5f + m_ChipsStringTTF->getContentSize ().width / 2, 0.0f));
		addChild (m_ChipsStringTTF, 4);
	}
	return true;
}

GuanDanAddCounterSlider::GuanDanAddCounterSlider(CCNode * background, CCNode * bar, CCSprite * btn, ODEFloat64 f32Ranger)
{
	m_sliderBar = (CCScale9Sprite *)bar;
	m_sliderBtn = (CCSprite *)btn;

	m_addScoreBackground = CCSprite::create (TP_IMAGE_PATH_INGAEM"addCounterBack.png");
	m_addScoreBackground->setPosition (ccp (0.0f, 275.0f - 50.0f + m_addScoreBackground->getContentSize ().height / 2));
	addChild (m_addScoreBackground, 2);
	
	m_maxAddCounterValue = 0;
	m_limitedAddCounterValue = 0;

	addChild (m_sliderBar, 3);
	addChild (m_sliderBtn, 3);

	m_sliderBar->setPosition (ccp (50, 0.0f));
	m_sliderBtn->setPosition (ccp (50, 0.0f));

	m_addCounterAllInSprite = CCSprite::create (TP_IMAGE_PATH_INGAEM"addTipsAllIcon.png");
	m_addCounterAllInSprite->setPosition (ccp (0.0f, 275.0f - 50.0f + m_addScoreBackground->getContentSize ().height / 2));
	addChild (m_addCounterAllInSprite, 4);
	m_addCounterAllInSprite->setVisible (false);

	CCAnimation* m_addCounterAnimation = CCAnimation::create();  

	m_addCounterEffectSprite = CCSprite::create (TP_IMAGE_PATH_CARD_EFFECT"tp_allin_tips_0.png");
	m_addCounterEffectSprite->setPosition (ccp (0.0f, 275.0f - 50.0f + m_addScoreBackground->getContentSize ().height / 2));
	addChild (m_addCounterEffectSprite, 4);
	m_addCounterEffectSprite->setVisible (false);
	char spriteFrameName[100] = {0};
	for (int i = 0; i <= 11; i++)
	{
		sprintf (spriteFrameName, TP_IMAGE_PATH_CARD_EFFECT"tp_allin_tips_%d.png", i);
		m_addCounterAnimation->addSpriteFrameWithFileName(spriteFrameName);
		CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
	}
	m_addCounterAnimation->setDelayPerUnit(1.0f / 15.0f);
	m_addCounterAnimate = CCAnimate::create (m_addCounterAnimation);
	m_addCounterAnimate->retain ();

	m_curSettingTTF = NULL;
	m_maxScoreAddTTF = NULL;
	m_limitedScoreAddTTF = NULL;

	m_f32Range = f32Ranger;

	m_bTarget = false;

	if(m_sliderBar)
	{
		m_f32BarWidth = m_sliderBar->getContentSize().height;
	}

	m_f32Value = 0;

	Update();
	setContentSize(CCSizeMake (120.0f,550));

}

GuanDanAddCounterSlider::~GuanDanAddCounterSlider()
{
	CC_SAFE_DELETE(m_addCounterAnimate);
}

void GuanDanAddCounterSlider::Update()
{
	ODEFloat32 f32Pos = ODEFloorf(ODELerp(m_f32Value, -m_f32Range, m_f32Range));
	m_sliderBtn->setPositionY(f32Pos);

	ODEInt64 curSettingValue = m_f32Value * (m_maxAddCounterValue - m_limitedAddCounterValue) + m_limitedAddCounterValue;

	if (m_curSettingTTF != NULL)
	{
		removeChild (m_curSettingTTF);
	}

	char settingScore[64] = {0};
	sprintf (settingScore, "%lld", curSettingValue);

	m_curSettingTTF = CCLabelAtlas::create (settingScore, TP_IMAGE_PATH_INGAEM"addNumbers.png", 16, 19, '0');
	addChild (m_curSettingTTF, 3);
	m_curSettingTTF->setPosition (ccp (0.0f - m_curSettingTTF->getContentSize ().width / 2, 275.0f - 42.5f + m_curSettingTTF->getContentSize ().height / 2));

	if (m_f32Value == 1.0f)
	{
		m_addCounterAllInSprite->setVisible (true);
		m_addCounterEffectSprite->setVisible (true);
		m_addCounterEffectSprite->runAction (CCRepeatForever::create (m_addCounterAnimate)) ;
	}
	else
	{
		m_addCounterAllInSprite->setVisible (false);
		m_addCounterEffectSprite->setVisible (false);
		m_addCounterEffectSprite->stopAllActions ();
	}
}

void GuanDanAddCounterSlider::SetValue(ODEFloat32 f32Value)
{
	if(f32Value != m_f32Value)
	{
		m_f32Value = f32Value;
		Update();
		m_kEvent.Callback(*this, VALUE_CHANGE);
	}
}

void GuanDanAddCounterSlider::SetValueWithEvent(ODEFloat32 f32Value)
{
	if(f32Value != m_f32Value)
	{
		m_f32Value = f32Value;
		Update();
		m_kEvent.Callback(*this, VALUE_CHANGE);
		m_bChange = true;
	}
}

ODEFloat32 GuanDanAddCounterSlider::GetValue()
{
	return m_f32Value;
}

ODUIWidget* GuanDanAddCounterSlider::ProcessTouch(ODUILayer::TouchType eType, cocos2d::CCTouch* pkTouch)
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
				ODEFloat32 f32WidgetX = getPositionY();
				ODEFloat32 f32Touch = pkTouch->getLocation().y;
				ODEFloat32 f32Min = f32WidgetX - m_f32Range * ui_height_scale;
				ODEFloat32 f32Max = f32WidgetX + m_f32Range * ui_height_scale;
				f32Touch = ODE_MAX(f32Min, f32Touch);
				f32Touch = ODE_MIN(f32Max, f32Touch);
				SetValueWithEvent((f32Touch - f32Min) / (f32Max - f32Min));
			}
			break;
		case ODUILayer::TOUCH_MOVED:
			{
				if (!m_bTarget)
					return NULL;
				ODEFloat32 f32WidgetX = getPositionY();
				ODEFloat32 f32Touch = pkTouch->getLocation().y;
				ODEFloat32 f32Min = f32WidgetX - m_f32Range * ui_height_scale;
				ODEFloat32 f32Max = f32WidgetX + m_f32Range * ui_height_scale;
				f32Touch = ODE_MAX(f32Min, f32Touch);
				f32Touch = ODE_MIN(f32Max, f32Touch);
				SetValueWithEvent((f32Touch - f32Min) / (f32Max - f32Min));
			}
			break;
		case ODUILayer::TOUCH_ENDED:
		case ODUILayer::TOUCH_CANCELLED:
			EventRelease();
			//TouchCancel ();
			break;
		}
		return this;
	}
	else
	{
		return NULL;
	}
}

void GuanDanAddCounterSlider::TouchCancel ()
{
	if(m_bChange)
	{
		m_kEvent.Callback(*this, VALUE_CHANGED);
		m_bChange = false;
	}
	m_bTarget = false;
}

void GuanDanAddCounterSlider::EventRelease()
{
	if(m_bChange)
	{
		m_kEvent.Callback(*this, VALUE_CHANGED);
		m_bChange = false;
	}
	m_bTarget = false;
}

void GuanDanAddCounterSlider::SetSliderContentHeight( ODEFloat32 f32Rate )
{
}

void GuanDanAddCounterSlider::SetMaxAddCounterValue (ODEInt64 maxVal)
{
	m_maxAddCounterValue = maxVal;

	if (m_maxScoreAddTTF != NULL)
		removeChild (m_maxScoreAddTTF);

	char settingScore[64] = {0};
	sprintf (settingScore, "%lld", maxVal);

	m_maxScoreAddTTF = CCLabelAtlas::create (settingScore, TP_IMAGE_PATH_INGAEM"addNumbers.png", 16, 19, '0');
	addChild (m_maxScoreAddTTF, 3);
	m_maxScoreAddTTF->setPosition (ccp ( 50.0f, 275.0f - 15.0f - m_maxScoreAddTTF->getContentSize ().height / 2));

	m_maxScoreAddTTF->setVisible (false);
}

void GuanDanAddCounterSlider::SetLimitedAddCounterValue (ODEInt64 limitedVal)
{
	m_limitedAddCounterValue = limitedVal;

	if (m_limitedScoreAddTTF != NULL)
		removeChild (m_limitedScoreAddTTF);

	char settingScore[64] = {0};
	sprintf (settingScore, "%lld", limitedVal);

	m_limitedScoreAddTTF = CCLabelAtlas::create (settingScore, TP_IMAGE_PATH_INGAEM"addNumbers.png", 16, 19, '0');
	addChild (m_limitedScoreAddTTF, 3);
	m_limitedScoreAddTTF->setPosition (ccp (0.0f - m_limitedScoreAddTTF->getContentSize ().width / 2, 275.0f - 42.5f + m_limitedScoreAddTTF->getContentSize ().height / 2));
	m_limitedScoreAddTTF->setVisible (false);

	if (m_curSettingTTF != NULL)
	{
		removeChild (m_curSettingTTF);
	}

	m_curSettingTTF = CCLabelAtlas::create (settingScore, TP_IMAGE_PATH_INGAEM"addNumbers.png", 16, 19, '0');
	addChild (m_curSettingTTF, 3);
	//m_curSettingTTF->setPosition (ccp (50.0f,  -275.0f + 30.0f + m_curSettingTTF->getContentSize ().height / 2));
	m_curSettingTTF->setPosition (ccp (0.0f - m_curSettingTTF->getContentSize ().width / 2, 275.0f - 42.5f + m_curSettingTTF->getContentSize ().height / 2));
}


GuanDanSendingCard * GuanDanSendingCard::create (GuanDanCardSizeType cardType)
{
	GuanDanSendingCard* pReturn = new GuanDanSendingCard(cardType);
	if (pReturn)
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

GuanDanSendingCard::GuanDanSendingCard (GuanDanCardSizeType cardType)
{
	m_baseNode = CCNode::create ();
	addChild (m_baseNode);
	m_pkCardBack = CCScale9Sprite::create(TP_IMAGE_PATH_INGAEM"tp_card_back2.png");
	m_pkCardBack->setPreferredSize(CCSize(166, 206));
	CCSprite* pCardBackImg = CCSprite::create(TP_IMAGE_PATH_INGAEM"tp_card_back.png");
	pCardBackImg->setPosition(ccp(m_pkCardBack->getContentSize().width / 2, m_pkCardBack->getContentSize().height / 2));
	m_pkCardBack->addChild(pCardBackImg);
	m_baseNode->addChild(m_pkCardBack, 3);

	if (cardType == GuanDanCardSizeType_RiverCard)
	{	
		m_baseNode->setScale (GuanDan_CardScare_RiverCard);
	}
	else if (cardType == GuanDanCardSizeType_ShowCard)
	{
		m_baseNode->setScale (GuanDan_CardScare_ShowCard);
	}
	else if (cardType == GuanDanCardSizeType_HandCard)
	{
		m_baseNode->setScale (GuanDan_CardScare_HandCard);
	}
	else if (cardType == GuanDanCardSizeType_PlayerCard)
	{
		m_baseNode->setScale (GuanDan_CardScare_PlayerCard);
	}else if (cardType == GuanDanCardSizeType_SendingCard)
	{
		m_baseNode->setScale (GuanDan_CardScare_SendingCard);
	}

	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_card_back2.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_card_back.png");

}

GuanDanPlayerCard * GuanDanPlayerCard::create ()
{
	GuanDanPlayerCard* pReturn = new GuanDanPlayerCard();
	if (pReturn)
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

GuanDanPlayerCard::GuanDanPlayerCard ()
{
	m_isGoldCard = false;
}

void GuanDanPlayerCard::createPlayerCardData ()
{
	if (m_isGoldCard)
	{
		CCSprite * m_cardPlayerBack1 = CCSprite::create (TP_IMAGE_PATH_INGAEM"g_tp_player_card1.png");
		addChild (m_cardPlayerBack1, 4);
		CCSprite * m_cardPlayerBack2 = CCSprite::create (TP_IMAGE_PATH_INGAEM"g_tp_player_card2.png");
		addChild (m_cardPlayerBack2, 4);
		m_cardPlayerBack2->setPosition (ccp (m_cardPlayerBack1->boundingBox ().getMaxX () + 7.5f, m_cardPlayerBack1->boundingBox ().getMidY ()));
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"g_tp_player_card1.png");
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"g_tp_player_card2.png");
	}
	else
	{
		CCSprite * m_cardPlayerBack1 = CCSprite::create (TP_IMAGE_PATH_INGAEM"tp_player_card1.png");
		addChild (m_cardPlayerBack1, 4);
		CCSprite * m_cardPlayerBack2 = CCSprite::create (TP_IMAGE_PATH_INGAEM"tp_player_card2.png");
		addChild (m_cardPlayerBack2, 4);
		m_cardPlayerBack2->setPosition (ccp (m_cardPlayerBack1->boundingBox ().getMaxX () + 7.5f, m_cardPlayerBack1->boundingBox ().getMidY ()));
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_player_card1.png");
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_player_card2.png");
	}
}


GuanDanPlayerCard::~GuanDanPlayerCard ()
{
}

GuanDanCard * GuanDanCard::create ()
{
	GuanDanCard* pReturn = new GuanDanCard();
	if (pReturn)
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

GuanDanCard::GuanDanCard () : m_isFace (false), m_cardType (GuanDanCardSizeType_ShowCard)
{
	m_baseNode = CCNode::create ();
	addChild (m_baseNode);

	setCardNum (TP_CARD_NONE); //初始化为无牌
	setCardSuit (TP_CARD_DIAMOND); //初始化为方块

	m_pkCardShade = NULL;
	m_pkCardLight = NULL;
	m_pkCardBack = NULL;
	m_pkCardFace = NULL;
	m_pkSuitLarge = NULL;
	m_pkNumber = NULL;
	m_isGoldCard = false;

}

void GuanDanCard::createCardFaceAndBack ()
{
	m_pkCardLight = CCScale9Sprite::create (TP_IMAGE_PATH_INGAEM"tp_card_light.png", CCRect (0, 0, 30, 30), CCRect (14, 14, 2, 2));
	m_pkCardLight->setPreferredSize(CCSize(172.0f, 210.0f));
	m_pkCardLight->setVisible (false);
	m_pkCardLight->setPosition (ccp (0.5f, 0.5f));
	m_baseNode->addChild(m_pkCardLight, 3);

	//是金牌
	if (m_isGoldCard)
	{
		//背面
		m_pkCardBack = CCSprite::createWithSpriteFrameName("g_texaspoker_card_back.png");
		m_baseNode->addChild(m_pkCardBack, 3);

		//牌正面
		m_pkCardFace = CCSprite::createWithSpriteFrameName("g_texaspoker_card_face.png");
		CCSprite* pCardFaceImage = CCSprite::createWithSpriteFrameName("g_texaspoker_card_face_0.png");
		pCardFaceImage->setPosition(ccp(m_pkCardFace->getContentSize().width / 2, m_pkCardFace->getContentSize().height / 2));
		m_pkCardFace->addChild(pCardFaceImage);
		m_baseNode->addChild(m_pkCardFace, 3);
	}
	else //非金牌
	{
		m_pkCardBack = CCSprite::createWithSpriteFrameName("texaspoker_card_back.png");
		m_baseNode->addChild(m_pkCardBack, 3);

		m_pkCardFace = CCSprite::createWithSpriteFrameName("texaspoker_card_face.png");
		m_baseNode->addChild(m_pkCardFace, 3);
	}

	m_pkSuitLarge = CCSprite::create ();
	m_pkSuitLarge->setPosition (ccp (m_pkCardFace->getContentSize ().width / 2, m_pkCardFace->getContentSize ().height / 2));
	m_pkCardFace->addChild (m_pkSuitLarge);

	m_pkNumber = CCSprite::create ();
	m_pkNumber->setPosition (ccp (10 + 20.0f, m_pkCardFace->getContentSize ().height - 10 - 20.0f));
	m_pkCardFace->addChild (m_pkNumber, 4);

	m_pkSuitSmall = CCSprite::create ();
	m_pkSuitSmall->setPosition (ccp (10 + 20.0f, m_pkCardFace->getContentSize ().height - 40.0f - 30.0f));
	m_pkCardFace->addChild (m_pkSuitSmall, 4);

	m_pkCardShade = CCScale9Sprite::create (TP_IMAGE_PATH_INGAEM"tp_card_shade.png", CCRect (0, 0, 22, 22), CCRect (10, 10, 2, 2));
	m_pkCardShade->setPreferredSize(CCSize(162.0f, 199.0f));
	m_pkCardShade->setVisible (false);
	m_pkCardShade->setPosition (ccp (0.5f, 0.5f));
	m_baseNode->addChild(m_pkCardShade, 4);

	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_card_light.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_card_shade.png");
}

void GuanDanCard::setCardMatchLightVisiable (bool isMatch, bool isGiveUp)
{
	if (isGiveUp)
	{
		if (m_pkCardLight)
			m_pkCardLight->setVisible (isMatch);

		if (m_pkCardShade)
			m_pkCardShade->setVisible (isGiveUp);
	}
	else
	{
		if (m_pkCardLight)
			m_pkCardLight->setVisible (isMatch);

		if (m_pkCardShade)
			m_pkCardShade->setVisible (!isMatch);
	}
}

void GuanDanCard::setIsGoldCard (bool isGoldCard)
{
	m_isGoldCard = isGoldCard;
}

void GuanDanCard::setCardShadeShow (bool isShow)
{
	if (m_pkCardShade)
		m_pkCardShade->setVisible (isShow);
}

void GuanDanCard::updateCardShow ()
{
	if (m_isFace)
	{
		if (m_pkCardBack)
			m_pkCardBack->setVisible (false);
	}
	else
	{
		if (m_pkCardFace)
			m_pkCardFace->setVisible (false);
	}
}

void GuanDanCard::showTrunAround (ODEFloat32 delta) 
{
	CCOrbitCamera* orbitFront = CCOrbitCamera::create(0.45f * 0.5, 1, 0, -90, 90, 0, 0);
	CCOrbitCamera* orbitBack = CCOrbitCamera::create(0.45f * 0.5, 1, 0, 0, 90, 0, 0);
	if (m_pkCardFace)
		m_pkCardFace->setVisible(false);
	if (m_pkCardBack)
		m_pkCardBack->runAction(CCSequence::create(CCDelayTime::create (delta), CCShow::create(),orbitBack,CCHide::create(),
    CCTargetedAction::create(m_pkCardFace,CCSequence::create(CCShow::create(),orbitFront,NULL)),NULL));
}

GuanDanCard::~GuanDanCard ()
{
}

void GuanDanCard::setCardValue (unsigned char VALUE)
{
	CCLOG ("CREATE CARD : VALUE : %d", VALUE);
	// 牌型
	ODEUInt32 iCardType = (VALUE >> 4) & GUANDAN_CARD_MASK_VALUE;
	// 牌点
	ODEUInt32 iCardPoint = VALUE & GUANDAN_CARD_MASK_VALUE;

	setCardNum ((GuanDanCardNum)iCardPoint);
	setCardSuit ((GuanDanCardSuit)iCardType);
	createCardData ();
}

void GuanDanCard::setCardNum (GuanDanCardNum num)
{
	m_eNumber = num;
}

void GuanDanCard::setCardSuit (GuanDanCardSuit suit)
{
	m_eSuit = suit;
}

void GuanDanCard::createCardData ()
{
	if (m_eNumber == TP_CARD_NONE)
		return ;

	CCLOG ("CREATE CARD : TYPE : %d, NUMBER: %d", m_eSuit, m_eNumber);

	ODEChar8 acBuffer[64];
	const ODEChar8 cCol = m_eSuit & 0x1 ? 'b' : 'r';
	if (m_eNumber == TP_CARD_LITTLE_KING)
	{
		if (m_isGoldCard)
			ODESprintf(acBuffer, 64, "g_texaspoker_%c%d.png", 'b', m_eNumber - 1);
		else
			ODESprintf(acBuffer, 64, "texaspoker_%c%d.png", 'b', m_eNumber - 1);
	}
	else if (m_eNumber == TP_CARD_BIG_KING)
	{
		if (m_isGoldCard)
			ODESprintf(acBuffer, 64, "g_texaspoker_%c%d.png", 'r', m_eNumber - 2);
		else
			ODESprintf(acBuffer, 64, "texaspoker_%c%d.png", 'r', m_eNumber - 2);

	}
	else
	{
		if (m_isGoldCard)
			ODESprintf(acBuffer, 64, "g_texaspoker_%c%d.png", cCol, m_eNumber);
		else
			ODESprintf(acBuffer, 64, "texaspoker_%c%d.png", cCol, m_eNumber);
	}

	m_pkNumber->initWithSpriteFrameName(acBuffer);

	if(m_eNumber >=  TP_CARD_J)
	{
		ODEChar8 cSuit;
		if(m_eNumber == TP_CARD_J)
			cSuit = 'j';
		else if(m_eNumber == TP_CARD_Q)
			cSuit = 'q';
		else if(m_eNumber == TP_CARD_K)
			cSuit = 'k';
		else if(m_eNumber == TP_CARD_LITTLE_KING)
		{
			cSuit = 'l';
		}
		else if(m_eNumber == TP_CARD_BIG_KING)
		{
			cSuit = 'l';
		}
		else
			cSuit = 'k';

		if (m_eNumber == TP_CARD_LITTLE_KING)
		{
			if (m_isGoldCard)
				ODESprintf(acBuffer, 64, "g_texaspoker_%c%c.png", 'b', cSuit);
			else
				ODESprintf(acBuffer, 64, "texaspoker_%c%c.png", 'b', cSuit);

		}
		else if (m_eNumber == TP_CARD_BIG_KING)
		{
			if (m_isGoldCard)
				ODESprintf(acBuffer, 64, "g_texaspoker_%c%c.png", 'r', cSuit);
			else
				ODESprintf(acBuffer, 64, "texaspoker_%c%c.png", 'r', cSuit);

		}
		else
		{
			if (m_isGoldCard)
				ODESprintf(acBuffer, 64, "g_texaspoker_%c%c.png", 'r', cSuit);
			else
				ODESprintf(acBuffer, 64, "texaspoker_%c%c.png", 'r', cSuit);
		}

		m_pkSuitLarge->initWithSpriteFrameName(acBuffer);
		
		if ((m_eNumber == TP_CARD_LITTLE_KING) || (m_eNumber == TP_CARD_BIG_KING))
		{
			m_pkSuitSmall->setVisible(false);
			m_pkNumber->setVisible(false);
		}
		else
		{
			m_pkSuitSmall->setVisible(true);
			m_pkNumber->setVisible(true);
		}
	}
	else
	{
		m_pkSuitSmall->setVisible(true);
		m_pkNumber->setVisible(true);
	}

	switch(m_eSuit)
	{
	case TP_CARD_SPADE:

		if (m_isGoldCard)
			m_pkSuitSmall->initWithSpriteFrameName("g_texaspoker_spade_s.png");
		else
			m_pkSuitSmall->initWithSpriteFrameName("texaspoker_spade_s.png");

		if(m_eNumber <  TP_CARD_J)
		{
			if (m_isGoldCard)
				m_pkSuitLarge->initWithSpriteFrameName("g_texaspoker_spade_l.png");
			else
				m_pkSuitLarge->initWithSpriteFrameName("texaspoker_spade_l.png");

			m_pkSuitLarge->setPosition (ccp (m_pkCardFace->getContentSize ().width / 2 + 20.0f, m_pkCardFace->getContentSize ().height / 2 - 35.0f));

		}	
		break;
	case TP_CARD_HEART:
		if (m_isGoldCard)
			m_pkSuitSmall->initWithSpriteFrameName("g_texaspoker_heart_s.png");
		else
			m_pkSuitSmall->initWithSpriteFrameName("texaspoker_heart_s.png");

		if(m_eNumber <  TP_CARD_J)
		{
			if (m_isGoldCard)
				m_pkSuitLarge->initWithSpriteFrameName("g_texaspoker_heart_l.png");
			else
				m_pkSuitLarge->initWithSpriteFrameName("texaspoker_heart_l.png");

			m_pkSuitLarge->setPosition (ccp (m_pkCardFace->getContentSize ().width / 2 + 20.0f, m_pkCardFace->getContentSize ().height / 2 - 35.0f));
		}
			
		break;
	case TP_CARD_CLUB:
		if (m_isGoldCard)
			m_pkSuitSmall->initWithSpriteFrameName("g_texaspoker_club_s.png");
		else
			m_pkSuitSmall->initWithSpriteFrameName("texaspoker_club_s.png");

		if(m_eNumber <  TP_CARD_J)
		{
			if (m_isGoldCard)
				m_pkSuitLarge->initWithSpriteFrameName("g_texaspoker_club_l.png");
			else
				m_pkSuitLarge->initWithSpriteFrameName("texaspoker_club_l.png");

			m_pkSuitLarge->setPosition (ccp (m_pkCardFace->getContentSize ().width / 2 + 20.0f, m_pkCardFace->getContentSize ().height / 2 - 35.0f));
		}
			
		break;
	case TP_CARD_DIAMOND:
		if (m_isGoldCard)
			m_pkSuitSmall->initWithSpriteFrameName("g_texaspoker_diamond_s.png");
		else
			m_pkSuitSmall->initWithSpriteFrameName("texaspoker_diamond_s.png");

		if(m_eNumber <  TP_CARD_J)
		{
			if (m_isGoldCard)
				m_pkSuitLarge->initWithSpriteFrameName("g_texaspoker_diamond_l.png");
			else
				m_pkSuitLarge->initWithSpriteFrameName("texaspoker_diamond_l.png");

			m_pkSuitLarge->setPosition (ccp (m_pkCardFace->getContentSize ().width / 2 + 20.0f, m_pkCardFace->getContentSize ().height / 2 - 35.0f));
		}
		break;
	default:
		break;
	}
}

bool GuanDanCard::getFace ()
{
	return m_isFace;
}

void GuanDanCard::setCardType (GuanDanCardSizeType cardType)
{
	if (cardType > GuanDanCardSizeType_PlayerCard || cardType < GuanDanCardSizeType_RiverCard)
		CCAssert (0, "not right cardtype");

	m_cardType = cardType;

	if (m_cardType == GuanDanCardSizeType_RiverCard)
	{	
		m_baseNode->setScale (GuanDan_CardScare_RiverCard);
	}
	else if (m_cardType == GuanDanCardSizeType_ShowCard)
	{
		m_baseNode->setScale (GuanDan_CardScare_ShowCard);
	}
	else if (m_cardType == GuanDanCardSizeType_HandCard)
	{
		m_baseNode->setScale (GuanDan_CardScare_HandCard);
	}
	else if (m_cardType == GuanDanCardSizeType_PlayerCard)
	{
		m_baseNode->setScale (GuanDan_CardScare_PlayerCard);
		m_pkCardBack->setVisible (false);
		m_pkCardFace->setVisible (false);
		m_pkNumber->setVisible (false);
		m_pkSuitLarge->setVisible (false);
		m_pkSuitSmall->setVisible (false);
		m_pkCardLight->setVisible (false);
	}
}

GuanDanCardSizeType GuanDanCard::getCardType ()
{
	return m_cardType;
}

GuanDanHeaderButton::GuanDanHeaderButton (CCNode* pcNormal, CCNode*  pcPressed, CCNode* pcDisabled) : ODUIButton (pcNormal, pcPressed, pcDisabled)
{
	memset (m_userNickName, 0, sizeof (m_userNickName));
	m_PlayerStatus = PlayerStatus_UnKnown;
	m_uPlayerScore = 0;
	m_ImageIndex = 100;
	m_PlayerStatusTTF = NULL;
	m_PlayerNameTTF = NULL;
	m_PlayerHeaderImg = NULL;
	pkChatInfo = NULL;
	pkExpression = NULL;
	m_PlayerUserMedalsTips = NULL;
	m_headerDarkShader = NULL;
	m_playerEffect = NULL;
	m_nickName_bg = NULL;
	m_PlayerScoreTTF = NULL;

	m_optionTime = 0;

	m_PlayerImgBack = CCScale9Sprite::create (TP_IMAGE_PATH_INGAEM"tp_headBackTop.png");
	m_PlayerImgBack->setPreferredSize(CCSize(110, 110));
	addChild(m_PlayerImgBack, 3);
	m_PlayerImgBack->setVisible(false);

	m_waitBackground = CCSprite::create (TP_IMAGE_PATH_INGAEM"tp_game_player_progress.png");
	addChild (m_waitBackground, 3);
	m_waitBackground->setVisible (false);
	m_waitProgressTimer = CCProgressTimer::create(m_waitBackground);
	m_waitProgressTimer->setReverseDirection (true);
	m_waitProgressTimer->setType(kCCProgressTimerTypeRadial);  
	m_waitProgressTimer->setPercentage(100);
	addChild (m_waitProgressTimer, 3);
	m_waitProgressTimer->setVisible (false);

	m_winCardTypeBackground = CCScale9Sprite::create (TP_IMAGE_PATH_INGAEM"tp_player_win.png", CCRect (0, 0, 22, 22), CCRect (10, 10, 2, 2));
	m_winCardTypeBackground->setPreferredSize(CCSize(134, 170));
	addChild (m_winCardTypeBackground, 7);
	m_winCardTypeBackground->setVisible (false);

	m_winCardType = NULL;
	m_handCardFirst = NULL;
	m_handCardSecond = NULL;
	pkExpression = ODE_NEW GuanDanExpressionEffect;
	AddChild(pkExpression,15);
	//liaotian
	pkChatInfo = ODE_NEW GuanDanChatInfoFrame;
	AddChild(pkChatInfo,15);

	m_headerDarkShader = CCScale9Sprite::create (TP_IMAGE_PATH_INGAEM"tp_headBack.png", CCRect (0, 0, 18, 18), CCRect (8, 8, 2, 2));
	m_headerDarkShader->setPreferredSize (CCSize(134, 170));
	addChild (m_headerDarkShader, 10);
	m_headerDarkShader->setVisible (false);

	//setPlayerImgIndex (m_ImageIndex);

	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_headBackTop.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_game_player_progress.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_player_win.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_headBack.png");


}

GuanDanHeaderButton::GuanDanHeaderButton (CCNode* pcNormal, CCNode* pcPressed, char * nickname, int headerIndex, ODEInt64 userScore, CCNode* pcDisabled) : ODUIButton (pcNormal, pcPressed, pcDisabled)
{
	m_PlayerStatus = PlayerStatus_UnKnown;
	m_uPlayerScore = 0;
	m_ImageIndex = 100;
	m_PlayerStatusTTF = NULL;
	m_PlayerNameTTF = NULL;
	m_PlayerHeaderImg = NULL;
	pkChatInfo = NULL;
	pkExpression = NULL;
	m_PlayerUserMedalsTips = NULL;
	m_nickName_bg = NULL;
	m_PlayerScoreTTF = NULL;
	m_optionTime = 0;
	m_playerNodeIndex = 0;
	m_changePlayerIndex = 0;
	m_changePlayerImageIndex = 0;
	pkExpression = ODE_NEW GuanDanExpressionEffect;
	AddChild(pkExpression,15);
	//liaotian
	pkChatInfo = ODE_NEW GuanDanChatInfoFrame;
	AddChild(pkChatInfo,15);

	m_PlayerImgBack = CCScale9Sprite::create (TP_IMAGE_PATH_INGAEM"tp_headBackTop.png");
	m_PlayerImgBack->setPreferredSize(CCSize(110, 110));
	addChild(m_PlayerImgBack, 3);

	m_PlayerImgBack->setVisible(false);
	//m_waitBackground = CCSprite::create (TP_IMAGE_PATH_INGAEM"tp_game_player_progress.png");
	//addChild (m_waitBackground, 3);
	//m_waitBackground->setVisible (false);

	m_waitBackground = CCSprite::create (TP_IMAGE_PATH_INGAEM"tp_game_player_progress.png");
	//addChild (m_waitBackground, 3);
	//m_waitBackground->setVisible (false);
	m_waitProgressTimer = CCProgressTimer::create(m_waitBackground);
	m_waitProgressTimer->setReverseDirection (true);
	m_waitProgressTimer->setType(kCCProgressTimerTypeRadial);  
	m_waitProgressTimer->setPercentage(100);
	addChild (m_waitProgressTimer, 3);
	m_waitProgressTimer->setVisible (false);

	m_winCardTypeBackground = CCScale9Sprite::create (TP_IMAGE_PATH_INGAEM"tp_player_win.png", CCRect (0, 0, 22, 22), CCRect (10, 10, 2, 2));
	m_winCardTypeBackground->setPreferredSize(CCSize(134, 170));
	addChild (m_winCardTypeBackground, 7);
	m_winCardTypeBackground->setVisible (false);

	m_winCardType = NULL;
	m_handCardFirst = NULL;
	m_handCardSecond = NULL;

	setPlayerNickName (nickname);
	//setPlayerImgIndex (headerIndex);
	setPlayerScore (userScore);

	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_headBackTop.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_game_player_progress.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_player_win.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_headBack.png");
}

void GuanDanHeaderButton::UpdatePlayerInfo ( ODEInt32 iIndex, long long iScore, bool bVisitor, int headindex, bool bRefreshLeverl, char * nickname,int glamous)
{
	CCLOG ("GuanDanHeaderButton PlayerIndex[%d], UserScore[%lld]", iIndex, iScore);
	m_playerNodeIndex = iIndex;
	m_changePlayerIndex = iIndex;
	m_changePlayerImageIndex = iIndex;
	//setPlayerImgIndex (g_pkGuanDanGamePlayerData->m_PlayerNode[iIndex].iUserID %6);
	setPlayerScore (iScore);
	setPlayerNickName (nickname);
}

void GuanDanHeaderButton::UpdatePlayerInfo_new ( ODEInt32 iIndex)
{
 	m_playerNodeIndex = iIndex;
// 	m_changePlayerIndex = iIndex;
// 	m_changePlayerImageIndex = iIndex;
	//setPlayerScore_new (iIndex);
	#if(ANDROID_IPTV_CONTROL_USE==1)
	setPlayerNickName_new (iIndex);
	#endif
	setPlayerImgIndex_new(iIndex);
}

void GuanDanHeaderButton::UpdatePlayerInfoForTuanTuanZhuang( ODEInt32 iIndex, int _imageIndex,long long iScore, bool bVisitor, int headindex, bool bRefreshLeverl, char * nickname,int glamous)
{
	CCLOG ("UpdatePlayerInfoForTuanTuanZhuang PlayerIndex[%d], ImageIndex[%lld]", iIndex, _imageIndex);
	m_playerNodeIndex = iIndex;
	m_changePlayerIndex = iIndex;
	m_changePlayerImageIndex = _imageIndex;

	ODEInt32 _vPos1 = GuanDanGameTools::SwitchViewChairID(iIndex);
	ODEInt32 _vPos2 = GuanDanGameTools::SwitchViewChairID(_imageIndex);

	setChangeImgIndex (_vPos1,_vPos2);
	setPlayerScore (iScore);
	setPlayerNickName (nickname);
}

void GuanDanHeaderButton::setUseMedalsInfo (int userMedalsId, int unlockMedals)
{
	m_UnlockMedalsCount = unlockMedals;
	m_MedalsUseId = userMedalsId;

	if (m_PlayerUserMedalsTips != NULL)
		removeChild (m_PlayerUserMedalsTips);

	char strHeader[128] = {0};

	if (unlockMedals == 0){
		return ;
	}
	else if (unlockMedals <= 21){
		sprintf (strHeader, TP_IMAGE_PATH_ACTIVITY"personalTipsT.png");
	}else if (unlockMedals <= 31){
		sprintf (strHeader, TP_IMAGE_PATH_ACTIVITY"personalTipsY.png");
	}else if (unlockMedals <= 41){
		sprintf (strHeader, TP_IMAGE_PATH_ACTIVITY"personalTipsG.png");
	}else if (unlockMedals <= 51){
		sprintf (strHeader, TP_IMAGE_PATH_ACTIVITY"persoanalTipsD.png");
	}else{
		sprintf (strHeader, TP_IMAGE_PATH_ACTIVITY"persoanalTipsD.png");
	}


	if (m_PlayerHeaderImg == NULL)
		return ;

	m_PlayerUserMedalsTips = CCSprite::create (strHeader);
	m_PlayerUserMedalsTips->setAnchorPoint (ccp (0.0, 1.0f));
	m_PlayerUserMedalsTips->setPosition (ccp (m_PlayerHeaderImg->boundingBox ().getMinX (), m_PlayerImgBack->boundingBox ().getMaxY ()));
	addChild (m_PlayerUserMedalsTips, 5);

	CCTextureCache::sharedTextureCache()->removeTextureForKey(strHeader);

}

void GuanDanHeaderButton::playerWaitTimeStart (int timeLeft)
{
	if (m_waitProgressTimer != NULL)
			removeChild (m_waitProgressTimer);
	CCSprite * processSprite = CCSprite::create (TP_IMAGE_PATH_INGAEM"tp_game_player_progress.png");
	m_waitProgressTimer = CCProgressTimer::create(processSprite);
	m_waitProgressTimer->setReverseDirection (true);
	m_waitProgressTimer->setType(kCCProgressTimerTypeRadial); 

	m_optionTime = timeLeft;
	float percent = (m_optionTime / GUANDAN_GAME_TIME_OUT_TIME) * 100.0f;
	m_waitProgressTimer->setPercentage(percent);
	addChild (m_waitProgressTimer, 3);
	m_waitProgressTimer->setVisible (true);

	unschedule (schedule_selector(GuanDanHeaderButton::updatePlayerWaitTime));
	schedule(schedule_selector(GuanDanHeaderButton::updatePlayerWaitTime), 0.01f);

	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_game_player_progress.png");
}

void GuanDanHeaderButton::playerWaitTimeEnd ()
{
	m_waitProgressTimer->setVisible (false);
	m_optionTime = 0;
	unschedule (schedule_selector(GuanDanHeaderButton::updatePlayerWaitTime));
}

void GuanDanHeaderButton::updatePlayerWaitTime (ODEFloat32 f32Delta)
{
	static bool showChange = false;

	static bool playSound = false;
	static bool change = false;

	m_optionTime = m_optionTime - f32Delta;

	if (m_waitProgressTimer)
		m_waitProgressTimer->setPercentage ((m_optionTime / GUANDAN_GAME_TIME_OUT_TIME) * 100.0f);

	float percent = m_waitProgressTimer->getPercentage ();

	CCSprite * processSprite = NULL;
	if (percent > 20.0f && percent <= 50.0f)
	{
		if (!showChange)
			showChange = true;
		processSprite = CCSprite::create (TP_IMAGE_PATH_INGAEM"tp_game_player_progress2.png");
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_game_player_progress2.png");

		if (playSound == false)
			change = true;

	}	
	else if (percent <= 20.0f)
	{
		if (!showChange)
			showChange = true;
		processSprite = CCSprite::create (TP_IMAGE_PATH_INGAEM"tp_game_player_progress3.png");
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_game_player_progress3.png");

		playSound = false;
	}
	else
	{
		if (!showChange)
			showChange = true;

		playSound = false;

		processSprite = CCSprite::create (TP_IMAGE_PATH_INGAEM"tp_game_player_progress.png");
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_game_player_progress.png");
	}
	
	if (showChange)
	{
		if (m_waitProgressTimer != NULL)
			removeChild (m_waitProgressTimer);
		m_waitProgressTimer = CCProgressTimer::create(processSprite);
		m_waitProgressTimer->setReverseDirection (true);
		m_waitProgressTimer->setType(kCCProgressTimerTypeRadial);  
		m_waitProgressTimer->setPercentage(percent);
		addChild (m_waitProgressTimer, 3);
		m_waitProgressTimer->setVisible (true);

		if (change)
		{
			if (g_pkGuanDanGamePlayerData->m_bCurrentUser == g_pkGuanDanGamePlayerData->m_iTableNumExtra)
			{
				g_pSoundSystem->PlayEffect (GUANDAN_SE_CLOCK_FOR_OPTION);
			}
			playSound = true;
			change = false;
		}
	}
}

GuanDanHeaderButton::~GuanDanHeaderButton ()
{
	//if (pkChatInfo != NULL)
	//{
	//	delete pkChatInfo;
	//	removeChild (pkChatInfo);
	//	pkChatInfo = NULL;
	//}

	//if (pkExpression != NULL)
	//{
	//	delete pkExpression;
	//	removeChild (pkExpression);
	//	pkExpression = NULL;
	//}

	m_playerNodeIndex = 0;
	m_changePlayerIndex = 0;
	m_changePlayerImageIndex = 0;
}

void GuanDanHeaderButton::showPlayerStatus (bool bShow)
{
}

void GuanDanHeaderButton::setPlayerNickName_new (int _pos)
{
	if (!m_PlayerNameTTF)
	{
		m_PlayerNameTTF = CCLabelTTF::create(ODETools::getUTF8WithLength(ODETools::GBKToUTF8 (g_pkGuanDanGamePlayerData->m_pTablePlayer[_pos]->szNickName).c_str (),9).c_str(), TTF_NAME, 20);
		m_PlayerNameTTF->setAnchorPoint (ccp (0.5f, 1.0f));
		addChild (m_PlayerNameTTF, 6);
	}
	else
	{
		m_PlayerNameTTF->setString(ODETools::getUTF8WithLength(ODETools::GBKToUTF8 (g_pkGuanDanGamePlayerData->m_pTablePlayer[_pos]->szNickName).c_str (),9).c_str());
	}

	if (_pos == RELATIVE_POSITION_TOP)
	{
		m_PlayerNameTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMaxY ()+160));
	}
	else
	{
		m_PlayerNameTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMaxY ()+180));
	}

	if (!m_nickName_bg)
	{
		m_nickName_bg = CCSprite::create(GD_TV_PATH_IMAGE"gd_NickName_bg.png");
		addChild(m_nickName_bg,5);
	}
	if (_pos==0)
	{
		m_nickName_bg->setPosition(ccp(m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMaxY ()+150));
	}
	else
	{
		m_nickName_bg->setPosition(ccp(m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMaxY ()+170));
	}
}

void GuanDanHeaderButton::setPlayerNickName (char * nickname)
{
	if (strcmp (m_userNickName, nickname) == 0)
		return ;

	if (m_PlayerNameTTF != NULL)
		removeChild (m_PlayerNameTTF);

	m_PlayerNameTTF = NULL;

	strcpy (m_userNickName, nickname);
	
	m_PlayerNameTTF = CCLabelTTF::create(ODETools::getUTF8WithLength(ODETools::GBKToUTF8 (m_userNickName).c_str (),9).c_str(), TTF_NAME, 20);
	m_PlayerNameTTF->setAnchorPoint (ccp (0.5f, 1.0f));
	int ipos=GuanDanGameTools::SwitchViewChairID(m_playerNodeIndex);
	if (ipos==0)
	{
		m_PlayerNameTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMaxY ()+160));
	}
	else
	{
		m_PlayerNameTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMaxY ()+180));
	}
#if(ANDROID_IPTV_CONTROL_USE==1)
	addChild (m_PlayerNameTTF, 6);
	CCSprite*nickName_bg=CCSprite::create(GD_TV_PATH_IMAGE"gd_NickName_bg.png");
	if (ipos==0)
	{
		nickName_bg->setPosition(ccp(m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMaxY ()+150));
	}
	else
	{
		nickName_bg->setPosition(ccp(m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMaxY ()+170));
	}
	
	addChild(nickName_bg,5);
#endif
	m_PlayerNameTTF->setVisible (true);
}

void GuanDanHeaderButton::setPlayerStatus(PlayerStatus status)
{
	if (m_PlayerStatus == status)
		return ;
	if (status > PlayerStatus_Winner || status < PlayerStatus_Visiter)
		return ;

	m_PlayerStatus = status;

	if (m_PlayerStatusTTF != NULL)
		removeChild (m_PlayerStatusTTF);

	m_PlayerStatusTTF = NULL;

	int index = 0;

	setPlayerNickName (m_userNickName);

	if (m_PlayerNameTTF)
		m_PlayerNameTTF->setVisible (false);

	cocos2d::ccColor3B color = ccc3 (0x00, 0x00, 0x00);
	index = (int)m_PlayerStatus;
	switch (m_PlayerStatus)
	{
	case PlayerStatus_Visiter:
		color = ccc3 (0x80, 0x80, 0x80);
		if (m_PlayerNameTTF)
			m_PlayerNameTTF->setVisible (true);
		break;
	case PlayerStatus_UnKnown:
		color = ccc3 (0x80, 0x80, 0x80);
		if (m_PlayerNameTTF)
			m_PlayerNameTTF->setVisible (true);
		break;
	case PlayerStatus_Wait:
		color = ccc3 (0x80, 0x80, 0x80);
		if (m_PlayerNameTTF)
			m_PlayerNameTTF->setVisible (true);
		break;
	case PlayerStatus_Gaming:
		color = ccc3 (0xFF, 0xFF, 0xFF);
		break;
	case PlayerStatus_SmallBeter:
		color = ccc3 (0xFF, 0xFF, 0xFF);
		break;
	case PlayerStatus_BigBeter:
		color = ccc3 (0xFF, 0xFF, 0xFF);
		break;
	case PlayerStatus_Opting:
		color = ccc3 (0xFF, 0xD3, 0x02);
		break;
	case PlayerStatus_Pass:
		color = ccc3 (0x00, 0xFF, 0xFF);
		break;
	case PlayerStatus_Follow:
		color = ccc3 (0x00, 0xFF, 0xFF);
		break;
	case PlayerStatus_AddCounter:
		color = ccc3 (0x00, 0xFF, 0xFF);
		break;
	case PlayerStatus_AllIn:
		color = ccc3 (0x9C, 0x54, 0x0F);
		break;
	case PlayerStatus_GiveUp:
		color = ccc3 (0x80, 0x80, 0x80);
		break;
	case PlayerStatus_Winner:
		m_winCardTypeBackground->setVisible (true);
		break;
	default:
		break;
	}

	if (m_PlayerStatus == PlayerStatus_AllIn)
	{
		m_PlayerStatusTTF = CCSprite::create (TP_IMAGE_PATH_INGAEM"tp_game_status_allin.png");
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_game_status_allin.png");
		m_PlayerStatusTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMaxY () - 0.5f - m_PlayerStatusTTF->getContentSize ().height / 2));
	}
	else
	{
		char titleText[32] = {0};
		sprintf (titleText, "UIText_PT_PlayerStatus_%d", index);
		m_PlayerStatusTTF = CCLabelTTF::create(TT (titleText), TTF_NAME, 22);
		m_PlayerStatusTTF->setAnchorPoint (ccp (0.5f, 1.0f));
		((CCLabelTTF *)m_PlayerStatusTTF)->setColor (color);
		m_PlayerStatusTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMaxY () - 0.5f));
	}

	
	addChild (m_PlayerStatusTTF, 6);
	m_PlayerStatusTTF->setVisible (false);

	if (m_PlayerStatus == PlayerStatus_Winner)
	{
		m_PlayerStatusTTF->setVisible (false);

		if (m_PlayerNameTTF)
			m_PlayerNameTTF->setVisible (false);
		m_winCardTypeBackground->setVisible (true);
	}
	else
	{
		if (m_PlayerStatus != PlayerStatus_Winner && m_PlayerStatus != PlayerStatus_UnKnown && m_PlayerStatus != PlayerStatus_Wait && m_PlayerStatus != PlayerStatus_Visiter)	
		{
			if (m_PlayerNameTTF)
				m_PlayerNameTTF->setVisible (false);
			if (m_PlayerStatusTTF)
				m_PlayerStatusTTF->setVisible (true);
		}
		else
		{
			if (m_PlayerNameTTF)
				m_PlayerNameTTF->setVisible (true);
			if (m_PlayerStatusTTF)
				m_PlayerStatusTTF->setVisible (false);
		}
		m_winCardTypeBackground->setVisible (false);
	}

	if (m_PlayerStatus == PlayerStatus_GiveUp || m_PlayerStatus == PlayerStatus_UnKnown || m_PlayerStatus == PlayerStatus_Visiter)
		m_headerDarkShader->setVisible (true);
	else
		m_headerDarkShader->setVisible (false);
}

void GuanDanHeaderButton::ResetCardType ()
{
	if (m_winCardType != NULL)
	{
		removeChild (m_winCardType);
		m_winCardType = NULL;
	}
	
	if (m_handCardFirst != NULL)
	{
		removeChild (m_handCardFirst);
		m_handCardFirst = NULL;
	}
		
	if (m_handCardSecond != NULL)
	{
		removeChild (m_handCardSecond);
		m_handCardSecond = NULL;
	}
}

void GuanDanHeaderButton::SetCardType (int cardType, unsigned char firstCard, unsigned char secondCard)
{
	if (m_winCardType != NULL)
		removeChild (m_winCardType);

	if (m_handCardFirst != NULL)
		removeChild (m_handCardFirst);

	if (m_handCardSecond != NULL)
		removeChild (m_handCardSecond);

	char cardTypeName[256] = {0};
	sprintf (cardTypeName, TP_IMAGE_PATH_INGAEM"tp_cardType_%d_1.png", cardType);
	if (CCFileUtils::sharedFileUtils ()->isFileExist (cardTypeName))
	{
		m_winCardType = CCSprite::create (cardTypeName);
		CCTextureCache::sharedTextureCache()->removeTextureForKey(cardTypeName);
		m_winCardType->setPosition (ccp (0.0f, m_winCardTypeBackground->boundingBox ().getMaxY () - 2.5f - m_winCardType->getContentSize ().height / 2));
		addChild (m_winCardType, 9);
	}

	m_handCardFirst = GuanDanCard::create ();
	m_handCardFirst->setIsGoldCard (g_pkGuanDanGamePlayerData->m_PlayerNode[m_playerNodeIndex].cIconNum);
	m_handCardFirst->createCardFaceAndBack ();
	m_handCardFirst->setCardValue (firstCard);
	m_handCardFirst->setFace (true);
	m_handCardFirst->setCardType (GuanDanCardSizeType_ShowCard);
	m_handCardFirst->setPosition (ccp (-18.5f,0.0f));
	addChild (m_handCardFirst, 10);

	m_handCardSecond = GuanDanCard::create ();
	m_handCardSecond->setIsGoldCard (g_pkGuanDanGamePlayerData->m_PlayerNode[m_playerNodeIndex].cIconNum);
	m_handCardSecond->createCardFaceAndBack ();
	m_handCardSecond->setCardValue (secondCard);
	m_handCardSecond->setFace (true);
	m_handCardSecond->setPosition (ccp (18.5f,0.0f));
	m_handCardSecond->setCardType (GuanDanCardSizeType_ShowCard);
	addChild (m_handCardSecond, 10);
}

void GuanDanHeaderButton::SetWinCardMatchLight (int cardIndex, bool match)
{
	if (cardIndex == 0)
	{
		if (m_handCardFirst != NULL)
		{
			m_handCardFirst->setCardMatchLightVisiable (match);

			/*if (match)
			{
				if (m_handCardFirst->getZOrder () <= 10)
				{
					m_handCardFirst->setZOrder ( 10 + 3);
				}
			}
			else
			{
				if (m_handCardFirst->getZOrder () < 10)
				{
					m_handCardFirst->setZOrder (10);
				}
			}*/
		}
	}
	else
	{
		if (m_handCardSecond != NULL)
		{
			m_handCardSecond->setCardMatchLightVisiable (match);

			/*if (match)
			{
				if (m_handCardSecond->getZOrder () <= 10)
				{
					m_handCardSecond->setZOrder ( 10 + 3);
				}
			}
			else
			{
				if (m_handCardSecond->getZOrder () < 10)
				{
					m_handCardSecond->setZOrder (10);
				}
			}*/
		}	
	}
}

void GuanDanHeaderButton::OnEvent(ODUIWidget& kWidget, ODEUInt32 u32State)
{
	if(u32State == RELEASED)
	{
		if (g_pkGuanDanData->GetIsNeedGuideInGD())
			return ;

		if (!g_pkGuanDanGamePlayerData->m_pTablePlayer[m_playerNodeIndex])
			return;

		if (g_pLayerManager->GetLayer ("GuanDan_HeaderTips") == NULL)
		{
			g_pLayerManager->AddLayer (GuanDanHeaderTips::create ());
		}

		//int iPos = GuanDanGameTools::SwitchViewChairID (m_playerNodeIndex);

		GuanDanHeaderTips * target = (GuanDanHeaderTips *)g_pLayerManager->GetLayer ("GuanDan_HeaderTips");

		if (target)
			target->setPlayerIndex (m_playerNodeIndex);
		
		//CCUserDefault::sharedUserDefault()->setIntegerForKey("playerIndex",m_playerNodeIndex);
		//更新玩家新信息
		//int iPos = GuanDanGameTools::SwitchViewChairID (m_playerNodeIndex);
		if (m_playerNodeIndex == RELATIVE_POSITION_TOP)
		{
			target->setPosition(ccp(200*ui_main_scale,170*ui_main_scale));
		}
		else if (m_playerNodeIndex == RELATIVE_POSITION_LEFT)
		{
			target->setPosition(ccp(-240*ui_main_scale,30*ui_main_scale));
		}
		else if (m_playerNodeIndex == RELATIVE_POSITION_RIGHT)
		{
			target->setPosition(ccp(240*ui_main_scale,30*ui_main_scale));
		}

		g_pLayerManager->PushLayer ("GuanDan_HeaderTips");
	}
}

void GuanDanHeaderButton::setChangePos (ODEInt32 _pos)
{
	if (-1 == _pos)
	{
		m_changePlayerIndex = m_playerNodeIndex;
		m_changePlayerImageIndex = m_playerNodeIndex;
	}
	else
	{
		m_changePlayerIndex = _pos;
		m_changePlayerImageIndex = _pos;
	}
}

void GuanDanHeaderButton::setChangeImagePos (ODEInt32 _pos)
{
	m_changePlayerImageIndex = _pos;
}

void GuanDanHeaderButton::setHeaderImage()
{
	setPlayerImgIndex(0);
}

void GuanDanHeaderButton::setPlayerScore_new (int _pos)
{
	CCString *_strvalue = CCString::createWithFormat("%lld",g_pkGuanDanGamePlayerData->m_pTablePlayer[_pos]->iMoney);

	if (!m_PlayerScoreTTF)
	{
		m_PlayerScoreTTF = CCLabelTTF::create(_strvalue->getCString(), TTF_NAME, 20);
		m_PlayerScoreTTF->setColor (ccc3 (0xFF, 0xD3, 0x02));
		m_PlayerScoreTTF->setAnchorPoint (ccp (0.5f, 0.0f));
		m_PlayerScoreTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMinY () + 2.5f));
	}
	else
	{
		m_PlayerScoreTTF->setString(_strvalue->getCString());
	}
}

void GuanDanHeaderButton::setPlayerScore (ODEInt64 userScore)
{
	if (m_uPlayerScore == userScore)
		return ;

	if (m_uPlayerScore != 0 && userScore == 0)
	{
		CCLOG ("SetPlayerScore not have Player info");
	}

	if (m_PlayerScoreTTF != NULL)
		removeChild (m_PlayerScoreTTF);

	m_uPlayerScore = userScore;

	char strScore[64]= {0};
	sprintf (strScore, "%lld", userScore);

	m_PlayerScoreTTF = CCLabelTTF::create(strScore, TTF_NAME, 20);
	m_PlayerScoreTTF->setColor (ccc3 (0xFF, 0xD3, 0x02));
	m_PlayerScoreTTF->setAnchorPoint (ccp (0.5f, 0.0f));
	m_PlayerScoreTTF->setPosition (ccp (m_pkNormal->boundingBox ().getMidX (), m_pkNormal->boundingBox ().getMinY () + 2.5f));
	//addChild (m_PlayerScoreTTF, 8);
}


void GuanDanHeaderButton::setPlayerImgIndex_new (int imgIndex)
{
	//int ipos=GuanDanGameTools::SwitchViewChairID(imgIndex);

	int _sexType = g_pkGuanDanGamePlayerData->m_pTablePlayer[imgIndex]->cSexType;

	int _imageIndex = 0;
	bool _isFlipx = false;

	switch (imgIndex)
	{
	case RELATIVE_POSITION_TOP:
		{
			if (_sexType == 0) //男
				_imageIndex = 3;
			else
				_imageIndex = 2;

			//_isFlipx = true;
		}
		break;
	case RELATIVE_POSITION_LEFT:
		{
			if (_sexType == 0) //男
				_imageIndex = 5;
			else
				_imageIndex = 1;
		}
		break;
	case RELATIVE_POSITION_RIGHT:
		{
			if (_sexType == 0) //男
				_imageIndex = 4;
			else
				_imageIndex = 0;

			_isFlipx = true;
		}
		break;
	default:
		break;
	}

	m_ImageIndex = _imageIndex;

	CCString *_strPath = CCString::createWithFormat(GD_IMAGE_PATH_HEADER"gd_headimage_%d.png",_imageIndex);

	if (m_PlayerHeaderImg)
	{
		m_PlayerHeaderImg->removeFromParentAndCleanup(true);
		m_PlayerHeaderImg = NULL;
	}

	m_PlayerHeaderImg = CCSprite::create (_strPath->getCString());
	m_PlayerHeaderImg->setAnchorPoint (ccp (0.5f, 0.0f));
	m_PlayerHeaderImg->setFlipX(_isFlipx);
	if (_sexType == 0)
		m_PlayerHeaderImg->setScale(0.8f);
	else
		m_PlayerHeaderImg->setScale(0.9f);

	if (imgIndex==RELATIVE_POSITION_TOP)
	{
		if (_sexType == 0)
			m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() + 15, m_PlayerImgBack->boundingBox ().getMinY ()));
		else
			m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() - 10, m_PlayerImgBack->boundingBox ().getMinY ()));
	}
	else
	{
		m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX (), m_PlayerImgBack->boundingBox ().getMinY ()));
	}

	addChild (m_PlayerHeaderImg, 1);

	CCTextureCache::sharedTextureCache()->removeTextureForKey(_strPath->getCString());
}

void GuanDanHeaderButton::setPlayerImgIndex (int imgIndex)
{
	if (m_playerNodeIndex < 0) return;
	int ipos=GuanDanGameTools::SwitchViewChairID(m_changePlayerImageIndex/*m_playerNodeIndex*/);
	int sextype=g_pkGuanDanGamePlayerData->m_PlayerNode[m_playerNodeIndex].cSexType;
	if (ipos!=RELATIVE_POSITION_DOWN)
	{
			if (m_PlayerHeaderImg != NULL)
				removeChild (m_PlayerHeaderImg);

			m_ImageIndex = imgIndex;

			char strHeader[128] = {0};

			if (sextype==0 && ipos==RELATIVE_POSITION_TOP)
			{
				m_ImageIndex = 3;
			}
			else if(sextype==0 && ipos==RELATIVE_POSITION_LEFT)
			{
				m_ImageIndex = 5;
			}
			else if(sextype==0 && ipos==RELATIVE_POSITION_RIGHT)
			{
				m_ImageIndex = 4;
			}
			else if(sextype==1 && ipos==RELATIVE_POSITION_TOP)
			{
				m_ImageIndex = 2;
			}
			else if(sextype==1 && ipos==RELATIVE_POSITION_LEFT)
			{
				m_ImageIndex = 1;
			}
			else if(sextype==1 && ipos==RELATIVE_POSITION_RIGHT)
			{
				m_ImageIndex = 0;
			}

			sprintf (strHeader, GD_IMAGE_PATH_HEADER"gd_headimage_%d.png", m_ImageIndex);

			m_PlayerHeaderImg = CCSprite::create (strHeader);
			if (ipos==0)
			{
				m_PlayerHeaderImg->setFlipX(true);
			}			
			else if (ipos==3)
			{
				m_PlayerHeaderImg->setFlipX(true);
			}	
			if (sextype==1)
			{
					m_PlayerHeaderImg->setScale(0.9);
			}
			else
			{
					m_PlayerHeaderImg->setScale(0.8);
			}
			m_PlayerHeaderImg->setAnchorPoint (ccp (0.5f, 0.0f));
			if (sextype==1 && ipos==RELATIVE_POSITION_TOP)
			{
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMaxX (), m_PlayerImgBack->boundingBox ().getMinY ()));				
			}
			else
			{
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX (), m_PlayerImgBack->boundingBox ().getMinY ()));
			}
			
			addChild (m_PlayerHeaderImg, 1);

			m_PlayerHeaderPos = this->convertToWorldSpace(m_PlayerHeaderImg->getPosition());

			CCTextureCache::sharedTextureCache()->removeTextureForKey(strHeader);

	}
}

void GuanDanHeaderButton::setChangeImgIndex_new (int _localCurrIndex,int _localBeforIndex)
{
	if (_localCurrIndex == _localBeforIndex) return;

	if (m_PlayerHeaderImg != NULL)
	{
		m_PlayerHeaderImg->removeFromParentAndCleanup(true);
		m_PlayerHeaderImg = NULL;
	}

	int _sextype=g_pkGuanDanGamePlayerData->m_pTablePlayer[_localCurrIndex]->cSexType;

	char strHeader[128] = {0};

	bool _isFlix = false;

	if (_localCurrIndex == RELATIVE_POSITION_TOP)//2
	{
		if (_localBeforIndex == RELATIVE_POSITION_LEFT)
		{
			if (_sextype == 0) //男
			{
				m_ImageIndex = 5;
			}
			else
			{
				m_ImageIndex = 1;
			}
			
		}
		else if (_localBeforIndex == RELATIVE_POSITION_RIGHT)
		{
			if (_sextype == 0) //男
			{
				m_ImageIndex = 4;
			}
			else
			{
				m_ImageIndex = 0;
			}
		}
		//_isFlix = true;
	}
	else if(_localCurrIndex == RELATIVE_POSITION_LEFT)
	{
		if (_localBeforIndex == RELATIVE_POSITION_TOP)
		{
			if (_sextype == 0) //男
			{
				m_ImageIndex = 3;
			}
			else
			{
				m_ImageIndex = 2;
			}
		}
		else if (_localBeforIndex == RELATIVE_POSITION_RIGHT)
		{
			if (_sextype == 0) //男
			{
				m_ImageIndex = 4;
			}
			else
			{
				m_ImageIndex = 0;
			}
		}
	}
	else if (_localCurrIndex == RELATIVE_POSITION_RIGHT)
	{
		if (_localBeforIndex == RELATIVE_POSITION_TOP)
		{
			if (_sextype == 0) //男
			{
				m_ImageIndex = 3;
			}
			else
			{
				m_ImageIndex = 2;
			}
		}
		else if (_localBeforIndex == RELATIVE_POSITION_LEFT)
		{
			if (_sextype == 0) //男
			{
				m_ImageIndex = 5;
			}
			else
			{
				m_ImageIndex = 1;
			}
		}

		_isFlix = true;
	}

	sprintf (strHeader, GD_IMAGE_PATH_HEADER"gd_headimage_%d.png", m_ImageIndex);

	m_PlayerHeaderImg = CCSprite::create (strHeader);

	if (m_PlayerHeaderImg)
	{
		m_PlayerHeaderImg->setFlipX(_isFlix);

		if (_sextype==1)
		{
			m_PlayerHeaderImg->setScale(0.9);
		}
		else
		{
			m_PlayerHeaderImg->setScale(0.8);
		}

		m_PlayerHeaderImg->setAnchorPoint (ccp (0.5f, 0.0f));

		if (_sextype == 1) //女
		{
			if (_localCurrIndex == RELATIVE_POSITION_LEFT && _localBeforIndex == RELATIVE_POSITION_TOP)
			{
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() - 35, m_PlayerImgBack->boundingBox ().getMinY ()));
			}
			else if (_localCurrIndex == RELATIVE_POSITION_TOP && _localBeforIndex == RELATIVE_POSITION_LEFT)
			{
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() + 15, m_PlayerImgBack->boundingBox ().getMinY ()));
			}
			else if (_localCurrIndex == RELATIVE_POSITION_RIGHT && _localBeforIndex == RELATIVE_POSITION_TOP)
			{
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMaxX(), m_PlayerImgBack->boundingBox ().getMinY ()));
			}
			else if (_localCurrIndex == RELATIVE_POSITION_TOP && _localBeforIndex == RELATIVE_POSITION_RIGHT)
			{
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() + 15, m_PlayerImgBack->boundingBox ().getMinY ()));
			}
			else
			{
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() - 10, m_PlayerImgBack->boundingBox ().getMinY ()));
			}
		}
		else
		{
			if (_localCurrIndex == RELATIVE_POSITION_TOP && _localBeforIndex == RELATIVE_POSITION_RIGHT)
			{
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() + 15, m_PlayerImgBack->boundingBox ().getMinY ()));
			}
			else if (_localCurrIndex == RELATIVE_POSITION_TOP && _localBeforIndex == RELATIVE_POSITION_LEFT)
			{
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() + 15, m_PlayerImgBack->boundingBox ().getMinY ()));
			}
			else
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() - 10, m_PlayerImgBack->boundingBox ().getMinY ()));
		}
		

		addChild (m_PlayerHeaderImg, 1);

		m_PlayerHeaderPos = this->convertToWorldSpace(m_PlayerHeaderImg->getPosition());

		CCTextureCache::sharedTextureCache()->removeTextureForKey(strHeader);
	}
}

void GuanDanHeaderButton::setChangeImgIndex (int _localBeforIndex,int _localAfterIndex, bool _isChange)
{	
	if (_localBeforIndex == _localAfterIndex) return;

	if (_isChange)
	{
		int _tp = _localAfterIndex;
		_localAfterIndex = _localBeforIndex;
		_localBeforIndex = _tp;
	}

	if (m_PlayerHeaderImg != NULL)
	{
		m_PlayerHeaderImg->removeFromParentAndCleanup(true);
		m_PlayerHeaderImg = NULL;
	}

	int sextype1=g_pkGuanDanGamePlayerData->m_pTablePlayer[_localBeforIndex]->cSexType;
	int sextype2=g_pkGuanDanGamePlayerData->m_pTablePlayer[_localAfterIndex]->cSexType;

	char strHeader[128] = {0};

	bool _isFlix = false;

	if (sextype1 == 0)//男
	{
		if (sextype2 == 0)//男
		{
			if (_localBeforIndex == RELATIVE_POSITION_TOP)//2
			{
				if (_localAfterIndex == RELATIVE_POSITION_LEFT)
				{
					m_ImageIndex = 5;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_RIGHT)
				{
					m_ImageIndex = 4;
					_isFlix = true;
					if (_isChange)
						_isFlix = false;
				}
			}
			else if(_localBeforIndex == RELATIVE_POSITION_LEFT)
			{
				if (_localAfterIndex == RELATIVE_POSITION_TOP)
				{
					m_ImageIndex = 3;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_RIGHT)
				{
					m_ImageIndex = 4;
					_isFlix = true;
				}
			}
			else if (_localBeforIndex == RELATIVE_POSITION_RIGHT)
			{
				if (_localAfterIndex == RELATIVE_POSITION_TOP)
				{
					m_ImageIndex = 3;
					if (_isChange)
						_isFlix = true;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_LEFT)
				{
					m_ImageIndex = 5;
				}
			}
		}
		else //女
		{
			if (_localBeforIndex == RELATIVE_POSITION_TOP)//2
			{
				if (_localAfterIndex == RELATIVE_POSITION_LEFT)
				{
					m_ImageIndex = 2;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_RIGHT)
				{
					m_ImageIndex = 2;
					_isFlix = true;
					if (_isChange)
						_isFlix = false;
				}
			}
			else if (_localBeforIndex == RELATIVE_POSITION_LEFT)
			{
				if (_localAfterIndex == RELATIVE_POSITION_TOP)
				{
					m_ImageIndex = 1;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_RIGHT)
				{
					m_ImageIndex = 1;
					_isFlix = true;
				}
			}
			else if (_localBeforIndex == RELATIVE_POSITION_RIGHT)
			{
				if (_localAfterIndex == RELATIVE_POSITION_TOP)
				{
					m_ImageIndex = 0;
					if (_isChange)
						_isFlix = true;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_LEFT)
				{
					m_ImageIndex = 0;
				}
			}
		}
	}
	else//女
	{
		if (sextype2 == 0)//男
		{
			if (_localBeforIndex == RELATIVE_POSITION_TOP)//2
			{
				if (_localAfterIndex == RELATIVE_POSITION_LEFT)
				{
					m_ImageIndex = 3;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_RIGHT)
				{
					m_ImageIndex = 3;
					_isFlix = true;
					if (_isChange)
						_isFlix = false;
				}
			}
			else if (_localBeforIndex == RELATIVE_POSITION_LEFT)
			{
				if (_localAfterIndex == RELATIVE_POSITION_TOP)
				{
					m_ImageIndex = 5;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_RIGHT)
				{
					m_ImageIndex = 5;
					_isFlix = true;
				}
			}
			else if(_localBeforIndex == RELATIVE_POSITION_RIGHT)
			{
				if (_localAfterIndex == RELATIVE_POSITION_TOP)
				{
					m_ImageIndex = 4;
					if (_isChange)
						_isFlix = true;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_LEFT)
				{
					m_ImageIndex = 4;
				}
			}
		}
		else //女
		{
			if (_localBeforIndex == RELATIVE_POSITION_TOP)//2
			{
				if (_localAfterIndex == RELATIVE_POSITION_LEFT)
				{
					m_ImageIndex = 1;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_RIGHT)
				{
					m_ImageIndex = 0;
					_isFlix = true;
					if (_isChange)
						_isFlix = false;
				}
			}
			else if(_localBeforIndex == RELATIVE_POSITION_LEFT)
			{
				if (_localAfterIndex == RELATIVE_POSITION_TOP)
				{
					m_ImageIndex = 2;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_RIGHT)
				{
					m_ImageIndex = 0;
					_isFlix = true;
				}
			}
			else if (_localBeforIndex == RELATIVE_POSITION_RIGHT)
			{
				if (_localAfterIndex == RELATIVE_POSITION_TOP)
				{
					m_ImageIndex = 2;
					if (_isChange)
						_isFlix = true;
				}
				else if (_localAfterIndex == RELATIVE_POSITION_LEFT)
				{
					m_ImageIndex = 1;
				}
			}
		}
	}

	sprintf (strHeader, GD_IMAGE_PATH_HEADER"gd_headimage_%d.png", m_ImageIndex);

	m_PlayerHeaderImg = CCSprite::create (strHeader);

	if (m_PlayerHeaderImg)
	{
		m_PlayerHeaderImg->setFlipX(_isFlix);

		if (sextype2==1)
		{
			m_PlayerHeaderImg->setScale(0.9);
		}
		else
		{
			m_PlayerHeaderImg->setScale(0.8);
		}

		m_PlayerHeaderImg->setAnchorPoint (ccp (0.5f, 0.0f));

		if (_localAfterIndex == RELATIVE_POSITION_LEFT && _localBeforIndex == RELATIVE_POSITION_TOP && !_isFlix)
		{
			if (sextype2 == 0)
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() - 10, m_PlayerImgBack->boundingBox ().getMinY ()));
			else
			{
				if (_isChange)
					m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() + 15, m_PlayerImgBack->boundingBox ().getMinY ()));
				else
					m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMidX() - 30, m_PlayerImgBack->boundingBox ().getMinY ()));
			}
		}		
		else
		{
			if (_localAfterIndex == RELATIVE_POSITION_LEFT)
			{
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMaxX() - 45, m_PlayerImgBack->boundingBox ().getMinY ()));
			}
			else if (_localAfterIndex == RELATIVE_POSITION_RIGHT)
			{
				if (sextype2 == 0)
					m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMaxX() - 30, m_PlayerImgBack->boundingBox ().getMinY ()));
				else
					m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMaxX() - 10, m_PlayerImgBack->boundingBox ().getMinY ()));
			}
			else if (_localAfterIndex == RELATIVE_POSITION_TOP)
			{
				if (sextype2 == 1 && !_isChange)
					m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMaxX() - 55, m_PlayerImgBack->boundingBox ().getMinY ()));
				else
					if (_isChange)
						m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMaxX() - 85, m_PlayerImgBack->boundingBox ().getMinY ()));
					else
						m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMaxX() - 30, m_PlayerImgBack->boundingBox ().getMinY ()));
			}
			else
			{
				m_PlayerHeaderImg->setPosition (ccp (m_PlayerImgBack->boundingBox ().getMaxX(), m_PlayerImgBack->boundingBox ().getMinY ()));
			}
		}

		addChild (m_PlayerHeaderImg, 1);

		m_PlayerHeaderPos = this->convertToWorldSpace(m_PlayerHeaderImg->getPosition());

		CCTextureCache::sharedTextureCache()->removeTextureForKey(strHeader);
	}
}


void GuanDanHeaderButton::SetPlayerEffect (GuanDanPlayerEffect * playerEffect)
{
	if (playerEffect == NULL)
		return ;

	//int ipos = GuanDanGameTools::SwitchViewChairID(m_playerNodeIndex);

	int sextype = g_pkGuanDanGamePlayerData->m_pTablePlayer[m_playerNodeIndex]->cSexType;/* g_pkGuanDanGamePlayerData->m_PlayerNode[m_playerNodeIndex].cSexType;*/

	if (sextype==1)
	{
		playerEffect->setScale(0.9);
	}
	else
	{
		playerEffect->setScale(0.8);
	}

	if (m_playerEffect == NULL)
	{
		m_playerEffect = playerEffect;
		m_playerEffect->setAnchorPoint (ccp (0.5f, 0.0f));
		m_playerEffect->setPosition (ccp (m_PlayerHeaderImg->getPosition ().x, m_PlayerHeaderImg->getPosition ().y + m_PlayerHeaderImg->getContentSize ().height * m_PlayerHeaderImg->getScale () / 2));
		AddChild (m_playerEffect, 2);
	}
	else
	{
		m_playerEffect->setAnchorPoint (ccp (0.5f, 0.0f));
		m_playerEffect->setPosition (ccp (m_PlayerHeaderImg->getPosition ().x, m_PlayerHeaderImg->getPosition ().y + m_PlayerHeaderImg->getContentSize ().height * m_PlayerHeaderImg->getScale () / 2));
	}
}



PlayerStatus GuanDanHeaderButton::getPlayerStatus ()
{
	return m_PlayerStatus;
}

void GuanDanHeaderButton::Chat( const ODEChar8* pcChatMessage )
{
	int relaIndex=GuanDanGameTools::SwitchViewChairID(m_playerNodeIndex);
	if (pkChatInfo)
	{
		if (relaIndex==0)
		{
			pkChatInfo->setPosition(ccp(100,-150));
		}
		else if (relaIndex==1)
		{
			pkChatInfo->setPosition(ccp(100,120));
		}

		else if(relaIndex==2)
		{
			pkChatInfo->setPosition(ccp(100,400));
		}	
		else if (relaIndex==3)
		{
			pkChatInfo->setPosition(ccp(-100,120));
		}
			pkChatInfo->Show(relaIndex, pcChatMessage);
	}
	
}

void GuanDanHeaderButton::PlayExpression( ODEUInt32 u32Index )
{
	int relaIndex=GuanDanGameTools::SwitchViewChairID(m_playerNodeIndex);
	if (pkExpression)
	{
		if (relaIndex==0 || relaIndex==6)
		{
			pkExpression->setPosition(ccp(0,-110));
		}
		else
		{
			pkExpression->setPosition(ccp(0,120));
		}
		
		pkExpression->Play(u32Index);
	}
}

GuanDanGameClock::GuanDanGameClock(ODEUInt32 u32ClockTime): u32ClockTimer(u32ClockTime), IsStart(false), stopTimer(false)
{
	u32BakupClockTime = u32ClockTime;
	m_pkRoot = ODE_NEW ODUIWidget;
	AddChild(m_pkRoot);
	CCSprite* pkClockFrame = CCSprite::createWithSpriteFrameName("kessen_clock.png");
	m_pkRoot->addChild(pkClockFrame);
	ODEChar8 acTimerBuffer[16];
	ODESprintf(acTimerBuffer, 16, "%d", u32ClockTimer);
	pkClockNumber = CCLabelTTF::create(acTimerBuffer, TTF_NAME, 30);
	pkClockNumber->setColor(ccc3(65, 15, 5));
	pkClockNumber->setAnchorPoint(CCPointMake(0.5f, 0.5f));
	pkClockNumber->setPosition(ccp(0, -3 * ui_height_scale));
	m_pkRoot->addChild(pkClockNumber);
}

GuanDanGameClock::~GuanDanGameClock()
{
}

void GuanDanGameClock::UpdateClockTime( ODEFloat32 f32Delta )
{
	if (u32ClockTimer > 0)
	{
		if (stopTimer)
		{
			return;
		}
		--u32ClockTimer;
		ODEChar8 acTimerBuffer[16];
		ODESprintf(acTimerBuffer, 16, "%d", u32ClockTimer);
		pkClockNumber->setString(acTimerBuffer);

		if (u32ClockTimer <= 5)
		{
			g_pSoundSystem->PlayEffect(GUANDAN_SE_CLOCK);
		}

		if (u32ClockTimer==0)
		{
			unschedule(schedule_selector(GuanDanGameClock::UpdateClockTime));
			OnTimerEnd();
		}
	}
}

void GuanDanGameClock::Start()
{
	if (!IsStart)
	{
		IsStart = true;
		ODEChar8 acTimerBuffer[16];
		ODESprintf(acTimerBuffer, 16, "%d", u32BakupClockTime);
		pkClockNumber->setString(acTimerBuffer);
		schedule(schedule_selector(GuanDanGameClock::UpdateClockTime), 1.0f);
	}
}

void GuanDanGameClock::Stop()
{
	unschedule(schedule_selector(GuanDanGameClock::UpdateClockTime));
	u32ClockTimer = u32BakupClockTime;
	IsStart = false;
	stopAllActions();
}

void GuanDanGameClock::OnTimerEnd()
{
}

GuanDanContinueGameClock::GuanDanContinueGameClock( ODEUInt32 u32ClockTime) : GuanDanGameClock(u32ClockTime)
{
}

GuanDanContinueGameClock::~GuanDanContinueGameClock()
{
}

void GuanDanContinueGameClock::OnTimerEnd()
{
	GuanDanGameView* pLayer = (GuanDanGameView*)(g_pLayerManager->GetLayer("GuanDan_GameView"));
	pLayer->ContinueGameTimerEnd();
}

GuanDanAddCounterClock::GuanDanAddCounterClock( ODEUInt32 u32ClockTime ) : GuanDanGameClock(u32ClockTime)
{
}

GuanDanAddCounterClock::~GuanDanAddCounterClock()
{
}

void GuanDanAddCounterClock::OnTimerEnd()
{
}

GuanDanStartGameClock::GuanDanStartGameClock( ODEUInt32 u32ClockTime ) : GuanDanGameClock(u32ClockTime)
{
}

GuanDanStartGameClock::~GuanDanStartGameClock()
{
}

void GuanDanStartGameClock::OnTimerEnd()
{
}

GuanDanGameBankEnter::GuanDanGameBankEnter() : ODUILayerModalExt<GuanDanGameBankEnter>("GuanDan_game_bank_enter")
{
	m_pkRoot = NULL;
	pkEnterBtn = NULL;
	updateButtonTick = 0;
	UIInitDelegate(GuanDanGameBankEnter, OnClose);
	UIInitDelegate(GuanDanGameBankEnter, OnEnter);
}

GuanDanGameBankEnter::~GuanDanGameBankEnter()
{
	term();
}

bool GuanDanGameBankEnter::init()
{
	m_pkRoot = ODE_NEW ODUIWidget;
	AddWidget("GameBankEnterRoot", m_pkRoot);
	m_pkRoot->setContentSize(CCSizeMake(674 * ui_width_scale, 300 * ui_height_scale));

	setPosition(ui_ccp(0.5f, 0.5f));

	CCScale9Sprite* pkBg = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-01.png");
	pkBg->setPreferredSize(CCSizeMake(674, 300));
	pkBg->setScale(ui_main_scale);
	addChild(pkBg);

	ODEFloat32 bg_width = pkBg->getPreferredSize().width;
	ODEFloat32 bg_height = pkBg->getPreferredSize().height;

	CCSprite* pkTitleText = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"personal_bank.png");
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

	CCLabelTTF* pkPasswordTitle = CCLabelTTF::create(TT("UIText_GameBank_Text"), TTF_NAME, 30);
	pkPasswordTitle->setAnchorPoint(ccp(0.5, 0.5));
	pkPasswordTitle->setColor(ccc3(200, 200, 200));
	pkPasswordTitle->setPosition(ccp(105, bg_height - 95));
	pkBg->addChild(pkPasswordTitle);

	CCScale9Sprite* pkPasswordInputBoxBg =CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pkPasswordInputBox = CCEditBox::create(CCSizeMake(433, 50.0f),pkPasswordInputBoxBg);
	pkPasswordInputBox->setPlaceholderFont(TTF_NAME, 24);
	pkPasswordInputBox->setFont(TTF_NAME, 24);
	pkPasswordInputBox->setPlaceHolder(TT("UITextFindRoomPassInputText2"));
	pkPasswordInputBox->setMaxLength(32);
	pkPasswordInputBox->setReturnType(kKeyboardReturnTypeDone);
	pkPasswordInputBox->setPosition(ccp(pkPasswordTitle->getPositionX() + 306, pkPasswordTitle->getPositionY()));
	pkPasswordInputBox->setInputFlag(kEditBoxInputFlagPassword);
	pkBg->addChild(pkPasswordInputBox, 3);

	CCLabelTTF* pkTips = CCLabelTTF::create(TT("MiniGameBankTips0"), TTF_NAME, 25);
	pkTips->setAnchorPoint(ccp(0.5f, 0.5f));
	pkTips->setColor(ccc3(0, 253, 0));
	pkTips->setPosition(ccp(bg_width / 2, bg_height - 160));
	pkBg->addChild(pkTips);


	CCSprite* pkEnterText = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bank_enter.png");

	UIBarH* pkEnterBtnNormal = UIBarH::createWithEl("kessen_btn_g", 180);
	UIBarH* pkEnterBtnPress = UIBarH::createWithEl("kessen_btn_g_p", 180);
	UIBarH* pkEnterBtnDisable = UIBarH::createWithEl("kessen_btn_d", 180);

	pkEnterBtn = ODE_NEW ODUIButton(pkEnterBtnNormal, pkEnterBtnPress, pkEnterBtnDisable);
	pkEnterBtn->setScale(ui_main_scale);
	pkEnterBtn->setPosition(ccp(0, (-bg_height / 2 + 63)* ui_main_scale));
	AddWidget("kEnterBtn", pkEnterBtn);
	UILinkDelegate(pkEnterBtn, OnEnter);

	pkEnterBtn->addChild(pkEnterText, 2);


	m_pkButtonTick = CCLabelAtlas::create("8", TEXASPOKER_IMAGE_PATH_GAMEBANK"SZ-01.png", 18, 26, '-');
	m_pkButtonTick->setAnchorPoint(CCPointMake(1.0, 0.5));
	m_pkButtonTick->setScale(ui_main_scale);
	m_pkButtonTick->setPosition(ccp(10.0f * ui_width_scale, -70.0f * ui_height_scale));
	addChild(m_pkButtonTick, 19);
	m_pkButtonTick->setVisible(false);

	return ODUILayerModalExt<GuanDanGameBankEnter>::init();
}

void GuanDanGameBankEnter::OnActive( const ODUIUserDataPtr& spData )
{
	pkEnterBtn->SetEnable(true);
	m_pkButtonTick->setVisible(false);
}

void GuanDanGameBankEnter::OnDeactive( const ODUIUserDataPtr& spData )
{
	pkPasswordInputBox->detachWithIME();
	removeTextureManual ();
}

void GuanDanGameBankEnter::removeTextureManual ()
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

bool GuanDanGameBankEnter::OnBack()
{
	return true;
}

void GuanDanGameBankEnter::term()
{
	ODUILayer::term();
}

void GuanDanGameBankEnter::resetPassword()
{
	pkPasswordInputBox->setText("");
}

void GuanDanGameBankEnter::setErrorMsg(const ODEChar8 *msg)
{
	m_errorMsg = msg;
}

void GuanDanGameBankEnter::showErrorMsg()
{
	g_pkGuanDanGame->Toast(m_errorMsg);
}

void GuanDanGameBankEnter::startTick()
{
	pkEnterBtn->SetEnable(false);
	unschedule(schedule_selector(GuanDanGameBankEnter::updateForButton));
	schedule(schedule_selector(GuanDanGameBankEnter::updateForButton), 1.0);
	updateButtonTick = 5;
	char tickBuffer[5] = {0};
	sprintf(tickBuffer, "%d", updateButtonTick);
	m_pkButtonTick->setString(tickBuffer);
	m_pkButtonTick->setVisible(true);
}

void GuanDanGameBankEnter::updateForButton(float dt)
{
	if (updateButtonTick > 0)
	{
		updateButtonTick --;
		char tickBuffer[5] = {0};
		sprintf(tickBuffer, "%d", updateButtonTick);
		m_pkButtonTick->setString(tickBuffer);
	}
	else
	{
		m_pkButtonTick->setVisible(false);
		pkEnterBtn->SetEnable(true);
		unschedule(schedule_selector(GuanDanGameBankEnter::updateForButton));
	}
}


bool GuanDanGameBankEnter::ProcessTouch( TouchType eType, cocos2d::CCTouch* pkTouch )
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

UIImplementDelegate(GuanDanGameBankEnter, OnClose, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		g_pLayerManager->Back();
	}
}

UIImplementDelegate(GuanDanGameBankEnter, OnEnter, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		if (strlen(pkPasswordInputBox->getText()) >= 0)
		{
			memset(g_pkClientData->getBankClientAuthenSuccessRsq(), 0, sizeof(LobbyBankClientAuthenSuccessRsp));
			memset(g_pkClientData->getBankerClientAuthenReq(), 0, sizeof(LobbyBankClientAuthenReq));
			g_pkClientData->getBankerClientAuthenReq()->iUserID = g_pkClientData->GetUserID();
			strcpy(g_pkClientData->getBankerClientAuthenReq()->szBankPasswd, pkPasswordInputBox->getText());

			md5_str(g_pkClientData->getBankerClientAuthenReq()->szBankPasswd, strlen(g_pkClientData->getBankerClientAuthenReq()->szBankPasswd), g_pkClientData->getBankerClientAuthenReq()->szBankPasswd);
			UserBankLoginReqDef * tempMsg = (UserBankLoginReqDef*)g_pkClientData->getBankerClientAuthenReq();
			g_pkNetworkHelper->GetGameServerDelegate()->SendMsgToSocket(tempMsg, USER_LOGIN_BANK_REQ, sizeof(UserBankLoginReqDef));
		}
	}
}


CCEditBox * GuanDanGameBankEnter::getPwEditBox()
{
	return pkPasswordInputBox;
}

GuanDanGameBankGetScore::GuanDanGameBankGetScore() : ODUILayerModalExt<GuanDanGameBankGetScore>("GuanDan_game_bank_get_score")
{
	input_box_num = 0;
	pkRoot =NULL;
	pkEnterBtn = NULL;
	updateButtonTick = 0;
	UIInitDelegate(GuanDanGameBankGetScore, OnClose);
	UIInitDelegate(GuanDanGameBankGetScore, OnEnter);
	UIInitDelegate(GuanDanGameBankGetScore, OnSilderVelueChange);
}

GuanDanGameBankGetScore::~GuanDanGameBankGetScore()
{
	term();
}

bool GuanDanGameBankGetScore::init()
{
	setScale(ui_main_scale);
	pkRoot = ODE_NEW ODUIWidget;
	pkRoot->setContentSize(CCSizeMake(725.0f, 558.0f));
	AddWidget("Root", pkRoot);
	pkRoot->setPosition(ui_ccp(0.5f, 0.5f));

	CCTextureCache::sharedTextureCache()->addImage(TEXASPOKER_IMAGE_PATH_GAMEBANK"slider_back.png");
	CCTextureCache::sharedTextureCache()->addImage(TEXASPOKER_IMAGE_PATH_GAMEBANK"slider_bar.png");

	pkBg = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-01.png");
	pkBg->setPreferredSize(CCSizeMake(725.0f, 558.0f));
	pkRoot->addChild(pkBg);

	CCScale9Sprite* pkBg_1 = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pkBg_1->setAnchorPoint(ccp(0.5, 1));
	pkBg_1->setPreferredSize(CCSizeMake(713.0f, 154.0f));
	pkBg_1->setPosition(ccp(0,pkBg->boundingBox ().getMaxY()-56));
	pkRoot->addChild(pkBg_1);

	CCScale9Sprite* pkBg_2 = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pkBg_2->setAnchorPoint(ccp(0.5, 1));
	pkBg_2->setPreferredSize(CCSizeMake(713.0f, 233.0f));
	pkBg_2->setPosition(ccp(0,pkBg_1->getPositionY() - pkBg_1->getContentSize().height + 3));
	pkRoot->addChild(pkBg_2);

	CCScale9Sprite* pkBg_3 = CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pkBg_3->setAnchorPoint(ccp(0.5, 1));
	pkBg_3->setPreferredSize(CCSizeMake(713.0f, 104));
	pkBg_3->setPosition(ccp(0,pkBg_2->getPositionY() - pkBg_2->getContentSize().height + 3));
	pkRoot->addChild(pkBg_3);

	CCSprite* pkTitleText = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"personal_bank.png");
	pkTitleText->setPosition(ccp(0,pkBg->boundingBox ().getMaxY()-33));
	pkRoot->addChild(pkTitleText);

	CCSprite* pkCloseBtnNormal = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	CCSprite* pkCloseBtnPress = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"DZ-04.png");
	pkCloseBtnPress->setColor(g_pkGuanDanGame->GetColorLevelDiv2(pkCloseBtnPress->getColor()));

	ODUIButton* pkCloseBtn = ODE_NEW ODUIButton(pkCloseBtnNormal, pkCloseBtnPress);
	pkCloseBtn->setPosition(ccp(pkBg->boundingBox ().getMaxX()-30,pkBg->boundingBox ().getMaxY()-30));
	pkCloseBtn->setContentSize(CCSize(pkCloseBtn->getContentSize().width * 2, pkCloseBtn->getContentSize().height * 2));
	pkRoot->AddChild(pkCloseBtn);
	UILinkDelegate(pkCloseBtn, OnClose);

	CCSprite* pkEnterBtnNormal = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bank_fetch.png");
	CCSprite* pkEnterBtnPress = CCSprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bank_fetch.png");
	ODGraySprite* pkEnterBtnDisable = ODGraySprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bank_fetch.png");
	pkEnterBtnPress->setColor(g_pkGuanDanGame->GetColorLevelDiv2(pkEnterBtnPress->getColor()));

	pkEnterBtn = ODE_NEW ODUIButton(pkEnterBtnNormal, pkEnterBtnPress, pkEnterBtnDisable);
	pkEnterBtn->setPosition(ccp(0,pkBg->boundingBox ().getMaxY()-492));
	pkRoot->AddChild(pkEnterBtn);
	UILinkDelegate(pkEnterBtn, OnEnter);

	CCLabelTTF* pkSelfTitle = CCLabelTTF::create(TT("UITextbank_score"), TTF_NAME, 30);

	pkSelfTitle->setPosition(ccp(pkBg->boundingBox ().getMinX()+110,pkBg->boundingBox ().getMaxY()-102));
	pkRoot->addChild(pkSelfTitle);

	CCLabelTTF* pkBalanceTitle = CCLabelTTF::create(TT("UITextbank_money"), TTF_NAME, 30);
	pkBalanceTitle->setPosition(ccp(pkBg->boundingBox ().getMinX()+110,pkBg->boundingBox ().getMaxY()-162));
	pkRoot->addChild(pkBalanceTitle);

	m_pkButtonTick = CCLabelAtlas::create("8", TEXASPOKER_IMAGE_PATH_GAMEBANK"SZ-01.png", 18, 26, '-');
	m_pkButtonTick->setAnchorPoint(CCPointMake(1.0, 0.5));
	m_pkButtonTick->setPosition(ccp(10.0f , -160.0f));
	pkRoot->addChild(m_pkButtonTick, 9);
	m_pkButtonTick->setVisible(false);
	
	m_pkBalanceNumber = CCLabelTTF::create("", TTF_NAME, 30);
	m_pkBalanceNumber->setAnchorPoint(CCPointMake(0, 0.5));
	m_pkBalanceNumber->setColor(ccc3(200,150,0));
	m_pkBalanceNumber->setPosition(ccp(pkBg->boundingBox ().getMinX()+190,pkBg->boundingBox ().getMaxY()-167));
	pkRoot->addChild(m_pkBalanceNumber, 9,2001);

	m_pkSelfNumber = CCLabelTTF::create("", TTF_NAME, 30);
	m_pkSelfNumber->setAnchorPoint(CCPointMake(0, 0.5));
	m_pkSelfNumber->setColor(ccc3(200,150,0));
	m_pkSelfNumber->setPosition(ccp(pkBg->boundingBox ().getMinX()+190,pkBg->boundingBox ().getMaxY()-107));
	pkRoot->addChild(m_pkSelfNumber, 9);

	CCLabelTTF* pkGetTitle = CCLabelTTF::create(TT("UITextbank_get_score"), TTF_NAME, 30);
	pkGetTitle->setPosition(ccp(pkBg->boundingBox ().getMinX()+110,pkBg->boundingBox ().getMaxY()-255));
	pkGetTitle->setColor(ccc3(0,255,0));
	pkRoot->addChild(pkGetTitle);
	//输入框
	CCScale9Sprite* pkInputBoxBg =CCScale9Sprite::create(TEXASPOKER_IMAGE_PATH_GAMEBANK"bg_text.png");
	pkInputBox = CCEditBox::create(CCSizeMake(460.0f, 52.0f),pkInputBoxBg);
	pkInputBox->setPlaceholderFont(TTF_NAME, 26);
	pkInputBox->setFont(TTF_NAME, 26);
	pkInputBox->setPlaceHolder("");
	pkInputBox->setMaxLength(16);
	pkInputBox->setInputMode(kEditBoxInputModeNumeric);
	pkInputBox->setReturnType(kKeyboardReturnTypeDone);
	pkInputBox->setPosition(ccp(pkBg->boundingBox ().getMinX()+412,pkBg->boundingBox ().getMaxY()-255));
	pkInputBox->setDelegate(this);
	pkRoot->addChild(pkInputBox, 3);

	pkInputNum = CCLabelAtlas::create("0", TEXASPOKER_IMAGE_PATH_GAMEBANK"input_num.png", 22, 37, '0');
	pkInputNum->setAnchorPoint(ccp(0,0.5));
	pkInputNum->setPosition(ccp(pkBg->boundingBox ().getMinX()+200,pkBg->boundingBox ().getMaxY()-260));
	pkRoot->addChild(pkInputNum, 3);


	m_pkGetNumberNow = CCLabelAtlas::create("0", TEXASPOKER_IMAGE_PATH_GAMEBANK"get_num.png", 18, 29, '0');
	m_pkGetNumberNow->setAnchorPoint(CCPointMake(0.0, 0.5));
	m_pkGetNumberNow->setPosition(ccp(-270.0f , -50.0f ));
	pkRoot->addChild(m_pkGetNumberNow, 9);

	m_pkGetNumberTotal = CCLabelAtlas::create("10/000/000/000/000", TEXASPOKER_IMAGE_PATH_GAMEBANK"get_num.png", 18, 29, '0');
	m_pkGetNumberTotal->setAnchorPoint(CCPointMake(1.0, 0.5));
	m_pkGetNumberTotal->setPosition(ccp(290.0f , -50.0f ));
	pkRoot->addChild(m_pkGetNumberTotal, 9);

	GuanDanScoreSlider* pkGetingSocreSlider = ODE_NEW GuanDanScoreSlider;
	pkGetingSocreSlider->setPosition(ccp(ui_w(0.5f), ui_h(0.5f) - 105.0f ));
	AddWidget("kGetingSocreSlider", pkGetingSocreSlider);
	UILinkDelegate(pkGetingSocreSlider, OnSilderVelueChange);
	pkGetingSocreSlider->SetValue(0.0f);


	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"get_num.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TEXASPOKER_IMAGE_PATH_GAMEBANK"input_num.png");


	return ODUILayerModalExt<GuanDanGameBankGetScore>::init();
}

void GuanDanGameBankGetScore::OnActive( const ODUIUserDataPtr& spData )
{

	m_pkSelfNumber->setString(_GetThousandsNum(g_pkClientData->GetUserScore()));

	if (pkRoot->getChildByTag(2001))
	{
		pkRoot->removeChildByTag(2001);
		m_pkBalanceNumber = CCLabelTTF::create(_GetThousandsNum(g_pkClientData->getBankClientAuthenSuccessRsq()->llBankCoin), TTF_NAME, 30);
		m_pkBalanceNumber->setAnchorPoint(CCPointMake(0, 0.5));
		m_pkBalanceNumber->setColor(ccc3(200,150,0));
		m_pkBalanceNumber->setPosition(ccp(pkBg->boundingBox ().getMinX()+190,pkBg->boundingBox ().getMaxY()-167));
		pkRoot->addChild(m_pkBalanceNumber, 9,2001);
	}
}

void GuanDanGameBankGetScore::OnDeactive( const ODUIUserDataPtr& spData )
{
	removeTextureManual ();
}

bool GuanDanGameBankGetScore::OnBack()
{
	return true;
}

void GuanDanGameBankGetScore::term()
{
	ODUILayer::term();
}

void GuanDanGameBankGetScore::editBoxEditingDidBegin(CCEditBox* editBox)
{
}

void GuanDanGameBankGetScore::editBoxEditingDidEnd(CCEditBox* editBox)
{
	pkInputBox->getText();
}

void GuanDanGameBankGetScore::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{

}

void GuanDanGameBankGetScore::editBoxReturn(CCEditBox* editBox)
{
	if (editBox == pkInputBox)
	{
		WIDGET(GuanDanScoreSlider, kGetingSocreSlider);
		long long u64Score = ODEAtoi64(pkInputBox->getText());
		if (u64Score > totalMoney)
		{
			char buffer[20] = {0};
			sprintf(buffer, "%lld", u64Score);
			pkInputBox->setText("");
			pkInputNum->setString(buffer);
			kGetingSocreSlider.SetValue(1.0f);
			getMoney = totalMoney;
		}
		else
		{
			if (u64Score < 0)
			{
				getMoney = 0;
			}
			else
			{
				getMoney = u64Score;
			}

			if (totalMoney == 0)
			{
				kGetingSocreSlider.SetValue(0);
			}
			else
			{
				kGetingSocreSlider.SetValue((float)getMoney/ (float)totalMoney);
			}

			char buffer[20] = {0};
			sprintf(buffer, "%lld", getMoney);
			pkInputBox->setText("");
			pkInputNum->setString(buffer);
		}
	}
}

void GuanDanGameBankGetScore::resetTotalNum(ODEUInt64 u64Number, bool needTick)
{
	if (needTick)
	{
		pkEnterBtn->SetEnable(false);
		unschedule(schedule_selector(GuanDanGameBankGetScore::updateForButton));
		schedule(schedule_selector(GuanDanGameBankGetScore::updateForButton), 1.0);
		updateButtonTick = 5;
		char tickBuffer[5] = {0};
		sprintf(tickBuffer, "%d", updateButtonTick);
		m_pkButtonTick->setString(tickBuffer);
		m_pkButtonTick->setVisible(true);
	}
	totalMoney = u64Number;
	getMoney = 0;
	SetTotalNumber(totalMoney);
	SetGetingNumber(0);
	WIDGET(GuanDanScoreSlider, kGetingSocreSlider);
	kGetingSocreSlider.SetValue(0.0f);
}

void GuanDanGameBankGetScore::setErrorMsg(const ODEChar8 *msg)
{
	m_errorMsg = msg;
}

void GuanDanGameBankGetScore::showErrorMsg()
{
	g_pkGuanDanGame->Toast(m_errorMsg);
}

void GuanDanGameBankGetScore::startTick()
{
	pkEnterBtn->SetEnable(false);
	unschedule(schedule_selector(GuanDanGameBankGetScore::updateForButton));
	schedule(schedule_selector(GuanDanGameBankGetScore::updateForButton), 1.0);
	updateButtonTick = 5;
	char tickBuffer[5] = {0};
	sprintf(tickBuffer, "%d", updateButtonTick);
	m_pkButtonTick->setString(tickBuffer);
	m_pkButtonTick->setVisible(true);
}


void GuanDanGameBankGetScore::updateForButton(float dt)
{
	if (updateButtonTick > 0)
	{
		updateButtonTick --;
		char tickBuffer[5] = {0};
		sprintf(tickBuffer, "%d", updateButtonTick);
		m_pkButtonTick->setString(tickBuffer);
	}
	else
	{
		m_pkButtonTick->setVisible(false);
		pkEnterBtn->SetEnable(true);
		unschedule(schedule_selector(GuanDanGameBankGetScore::updateForButton));
	}
}

UIImplementDelegate(GuanDanGameBankGetScore, OnClose, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		g_pLayerManager->Back();
	}
}

UIImplementDelegate(GuanDanGameBankGetScore, OnEnter, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		pkInputBox->detachWithIME();
		ODEUInt64 u64Score = ODEAtoi64(pkInputBox->getText());
		if (getMoney == 0 && u64Score == 0)
		{
			g_pkGuanDanGame->Toast(TT("MiniGameBankTips2"));
			return;
		}

		if (getMoney > 0 && getMoney <= totalMoney)
		{
			startTick();

			if (dynamic_cast<GuanDanGameView*>(g_pLayerManager->GetCurrentMainLayer()))
			{
				GetBankCoinReqPCDef tempGetBankCoinReqDef = {0};
				tempGetBankCoinReqDef.iUserID = g_pkClientData->getBankerClientAuthenReq()->iUserID;
				tempGetBankCoinReqDef.llGetNum = getMoney;

				g_pkNetworkHelper->SendDownloadScorePC((ODEChar8*)&tempGetBankCoinReqDef,true);
			}
			else
			{
				GetBankCoinReqDef tempGetBankCoinReqDef = {0};
				tempGetBankCoinReqDef.iUserID = g_pkClientData->getBankerClientAuthenReq()->iUserID;
				tempGetBankCoinReqDef.llGetNum = getMoney;

				g_pkNetworkHelper->SendDownloadScore((ODEChar8*)&tempGetBankCoinReqDef);
			}
		}
	}
}

UIImplementDelegate(GuanDanGameBankGetScore, OnSilderVelueChange, kWidget, u32State)
{
	if (u32State == ODUIWidget::VALUE_CHANGE)
	{
		ODEFloat64 fSliderValue = ((GuanDanSlider&)kWidget).GetValue();
		ODEInt64 numMoney = totalMoney * fSliderValue;

		SetGetingNumber(numMoney);
		getMoney = numMoney;
	}
}

void GuanDanGameBankGetScore::SetTotalNumber( ODEUInt64 u64Number )
{
	if (pkRoot->getChildByTag(2001))
	{
		pkRoot->removeChildByTag(2001);
		m_pkBalanceNumber = CCLabelTTF::create(_GetThousandsNum(u64Number), TTF_NAME, 30);
		m_pkBalanceNumber->setAnchorPoint(CCPointMake(0, 0.5));
		m_pkBalanceNumber->setColor(ccc3(200,150,0));
		m_pkBalanceNumber->setPosition(ccp(pkBg->boundingBox ().getMinX()+190,pkBg->boundingBox ().getMaxY()-167));
		pkRoot->addChild(m_pkBalanceNumber, 9,2001);
	}

	m_pkGetNumberTotal->setString(_GetThousandsNum(u64Number));
}

void GuanDanGameBankGetScore::SetGetingNumber( ODEUInt64 u64Number )
{
	ODEChar8 acBuffer[128];
	ODESprintf(acBuffer, 128, "%lld", u64Number);
	if (u64Number==0)
	{
		pkInputBox->setText("");
		pkInputNum->setString("0");
	}
	else
	{
		pkInputBox->setText("");
		pkInputNum->setString(acBuffer);
	}
	m_pkGetNumberNow->setString(_GetThousandsNum(u64Number));
}


const ODEChar8* GuanDanGameBankGetScore::_GetThousandsNum( ODEUInt64 u64Score )
{
	static ODEChar8 commades[128];
	ODEChar8 commasrc[128];

	ODESprintf(commasrc, 128, "%lld", u64Score);
	int L=ODEStrlen(commasrc);
	register ODEInt32 j=0;
	register ODEInt32 k=0;
	for (ODEInt32 i=L-1;i>=0;i--) {
		commades[j]=commasrc[i];
		k++;
		j++;
		if ((k%4)==0 && i>0) {
			commades[j]=' ';
			j++;
		}
	}
	commades[j]=0;
	ODEStrcpy(commades, 128, Strrev(commades));

	return commades;
}

bool GuanDanGameBankGetScore::ProcessTouch( TouchType eType, cocos2d::CCTouch* pkTouch )
{
	if (!ODUILayer::ProcessTouch(eType, pkTouch))
	{
		switch(eType)
		{
		case ODUILayer::TOUCH_BEGAN:
			{
				pkInputBox->ccTouchBegan(pkTouch, NULL);

				if(!pkRoot->TestInput(eType, pkTouch))
				{
					g_pLayerManager->Back();
				}
			}
			break;
		case ODUILayer::TOUCH_MOVED:
			pkInputBox->ccTouchMoved(pkTouch, NULL);
			break;
		case ODUILayer::TOUCH_ENDED:
			pkInputBox->ccTouchEnded(pkTouch, NULL);
			break;
		case ODUILayer::TOUCH_CANCELLED:
			pkInputBox->ccTouchCancelled(pkTouch, NULL);
			break;
		default:
			break;
		}		
	}
	return false;
}

void GuanDanGameBankGetScore::SetSelfNumber( ODEUInt64 u64Number )
{
	m_pkSelfNumber->setString(_GetThousandsNum(u64Number));
}

void GuanDanGameBankGetScore::removeTextureManual ()
{
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_GAMEBANK"handle.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_GAMEBANK"slider_back.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_GAMEBANK"slider_bar.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_GAMEBANK"handle_r.png");
#ifdef  _CACHEDTEXTTUREINFO_
	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
#endif
}

GuanDanScoreSlider::GuanDanScoreSlider() : GuanDanSlider(TP_IMAGE_PATH_GAMEBANK"handle.png",TP_IMAGE_PATH_GAMEBANK"handle_l.png",TP_IMAGE_PATH_GAMEBANK"handle_r.png", 
	140.0f * g_f32ScaleHD,CCSize(613,10),CCSize(643,42), TP_IMAGE_PATH_GAMEBANK"slider_back.png", TP_IMAGE_PATH_GAMEBANK"slider_bar.png")
{
	Update();
}

void GuanDanScoreSlider::Update()
{
	GuanDanSlider::Update();
}

GDCardTypeEffect::GDCardTypeEffect(GuanDanCardType cardType) : m_cardType (GD_CARDS_TYPE_NONE)
{
	if (m_cardType <= GD_CARDS_TYPE_NONE && m_cardType > GD_CARDS_TYPE_BIGTONGHUASHUN)
		return ;

	setScale (ui_main_scale);

	m_cardTypeSprite = NULL;
	m_cardTypeSpriteTop = NULL;
	m_cardTypeSpriteTopL = NULL;
	m_cardTypeSpriteBomttom = NULL;
	m_cardTypeSpriteBomttomL = NULL;
	m_cardTypeEffectCircle = NULL;

	memset (m_cardTypeLight, 0, sizeof (m_cardTypeLight));

	m_cardType = GD_CARDS_TYPE_DUIZI;

	createCardTypeEffect (cardType);
}

void GDCardTypeEffect::removeCreateCardType ()
{
	if (m_cardTypeSprite != NULL)
	{
		removeChild (m_cardTypeSprite);
		m_cardTypeSprite = NULL;
	}

	if (m_cardTypeSpriteTop != NULL)
	{
		removeChild (m_cardTypeSpriteTop);
		m_cardTypeSpriteTop = NULL;
	}

	if (m_cardTypeSpriteTopL != NULL)
	{
		removeChild (m_cardTypeSpriteTopL);
		m_cardTypeSpriteTopL = NULL;
	}

	if (m_cardTypeSpriteBomttom)
	{
		removeChild (m_cardTypeSpriteBomttom);
		m_cardTypeSpriteBomttom = NULL;
	}

	if (m_cardTypeSpriteBomttomL)
	{
		removeChild (m_cardTypeSpriteBomttomL);
		m_cardTypeSpriteBomttomL = NULL;
	}

	for (int i = 0; i < MAX_LIGHT_COUNT_MAX; i ++)
	{
		if (m_cardTypeLight[i])
		{
			removeChild (m_cardTypeLight[i]);
			m_cardTypeLight[i] = NULL;
		}
	}

	if (m_cardTypeEffectCircle != NULL)
	{
		removeChild (m_cardTypeEffectCircle);
		m_cardTypeEffectCircle = NULL;
	}

	CC_SAFE_RELEASE(m_cardTypeBomttomAnimate);
	CC_SAFE_RELEASE(m_cardTypeBomttomAnimateL);
	CC_SAFE_RELEASE(m_cardTypeTopAnimate);
	CC_SAFE_RELEASE(m_cardTypeTopAnimateL);
	for (int i = 0; i < MAX_LIGHT_COUNT_MAX; i++)
	{
		CC_SAFE_RELEASE(m_cardTypeLight[i]);
	}
}

void GDCardTypeEffect::createCardTypeEffect (GuanDanCardType cardType)
{
	//下花纹
	char cardTypeName[128] = {0};
	int m_bottomAnimateFrame = 0;
	int m_topAnimateFrame = 0;

	m_cardType = cardType;

	switch (m_cardType)
	{
	case GD_CARDS_TYPE_NONE:
		break;
	case GD_CARDS_TYPE_SANPAI:
			m_topAnimateFrame = 1;
			m_bottomAnimateFrame = 1;
		break;
	case GD_CARDS_TYPE_DUIZI:
			m_topAnimateFrame = 1;
			m_bottomAnimateFrame = 14;
		break;
	case GD_CARDS_TYPE_ERDUI:
			m_topAnimateFrame = 1;
			m_bottomAnimateFrame = 14;
		break;
	case GD_CARDS_TYPE_SANTIAO:
			m_topAnimateFrame = 1;
			m_bottomAnimateFrame = 14;
		break;
	case GD_CARDS_TYPE_SHUNZI:
			m_topAnimateFrame = 13;
			m_bottomAnimateFrame = 14;
		break;
	case GD_CARDS_TYPE_TONGHUA:
			m_topAnimateFrame = 1;
			m_bottomAnimateFrame = 14;
		break;
	case GD_CARDS_TYPE_HULU:
			m_topAnimateFrame = 8;
			m_bottomAnimateFrame = 10;
		break;
	case GD_CARDS_TYPE_TIEZHI:
			m_topAnimateFrame = 1;
			m_bottomAnimateFrame = 10;
		break;
	case GD_CARDS_TYPE_TONGHUASHUN:
			m_topAnimateFrame = 9;
			m_bottomAnimateFrame = 10;
		break;
	case GD_CARDS_TYPE_BIGTONGHUASHUN:
			m_topAnimateFrame = 12;
			m_bottomAnimateFrame = 10;
		break;
	default:
		break;
	}

	m_cardTypeEffectCircle = CCSprite::create (TP_IMAGE_PATH_INGAEM"tp_card_effect_circle.png");
	m_cardTypeEffectCircle->setScale (2.2f);
	addChild (m_cardTypeEffectCircle);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"tp_card_effect_circle.png");

	CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(m_cardTypeEffectCircle);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setOpacity((GLubyte)(255 * 0.25f));
    }

	m_cardTypeEffectCircle->setVisible (false);

	if (m_cardType == GD_CARDS_TYPE_BIGTONGHUASHUN || m_cardType == GD_CARDS_TYPE_TONGHUASHUN)
		sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_d_0.png", GD_CARDS_TYPE_HULU);
	else
		sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_d_0.png", m_cardType);

	m_cardTypeSpriteBomttom = CCSprite::create(cardTypeName);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(cardTypeName);
	addChild(m_cardTypeSpriteBomttom);
	m_cardTypeSpriteBomttom->setVisible (false);
	m_cardTypeSpriteBomttom->setPosition (ccp (m_cardTypeSpriteBomttom->getContentSize ().width / 2, -m_cardTypeSpriteBomttom->getContentSize ().height / 2));

	m_cardTypeBomttomAnimation = CCAnimation::create();  
	for (int i = 0; i < m_bottomAnimateFrame; i++)
	{
		if (m_cardType == GD_CARDS_TYPE_BIGTONGHUASHUN || m_cardType == GD_CARDS_TYPE_TONGHUASHUN)
			sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_d_%d.png", GD_CARDS_TYPE_HULU, i);
		else
			sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_d_%d.png", m_cardType, i);
		m_cardTypeBomttomAnimation->addSpriteFrameWithFileName(cardTypeName);
		CCTextureCache::sharedTextureCache()->removeTextureForKey(cardTypeName);
	}
	m_cardTypeBomttomAnimation->setDelayPerUnit(1.0f / 15.0f);
	m_cardTypeBomttomAnimate = CCAnimate::create (m_cardTypeBomttomAnimation);
	m_cardTypeBomttomAnimate->retain ();

	if (m_cardType == GD_CARDS_TYPE_BIGTONGHUASHUN || m_cardType == GD_CARDS_TYPE_TONGHUASHUN)
		sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_d_0.png", GD_CARDS_TYPE_HULU);
	else
		sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_d_0.png", m_cardType);
	m_cardTypeSpriteBomttomL = CCSprite::create(cardTypeName);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(cardTypeName);
	m_cardTypeSpriteBomttomL->setFlipX (true);
	addChild(m_cardTypeSpriteBomttomL);
	m_cardTypeSpriteBomttomL->setVisible (false);
	m_cardTypeSpriteBomttomL->setPosition (ccp (-m_cardTypeSpriteBomttomL->getContentSize ().width / 2, -m_cardTypeSpriteBomttomL->getContentSize ().height / 2));
	m_cardTypeBomttomAnimationL = CCAnimation::create();  
	for (int i = 0; i < m_bottomAnimateFrame; i++)
	{
		if (m_cardType == GD_CARDS_TYPE_BIGTONGHUASHUN || m_cardType == GD_CARDS_TYPE_TONGHUASHUN)
			sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_d_%d.png", GD_CARDS_TYPE_HULU, i);
		else
			sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_d_%d.png", m_cardType, i);

		CCSprite * effectFrame = CCSprite::create (cardTypeName);
		effectFrame->setFlipX (true);
		m_cardTypeBomttomAnimationL->addSpriteFrameWithTexture(effectFrame->getTexture (), CCRect (0, 0, effectFrame->getTexture ()->getPixelsWide (), effectFrame->getTexture ()->getPixelsHigh ()));
		CCTextureCache::sharedTextureCache()->removeTextureForKey(cardTypeName);
	}
	m_cardTypeBomttomAnimationL->setDelayPerUnit(1.0f / 15.0f);
	m_cardTypeBomttomAnimateL = CCAnimate::create (m_cardTypeBomttomAnimationL);
	m_cardTypeBomttomAnimateL->retain ();

	//上花纹
	sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_top_0.png", m_cardType);
	m_cardTypeSpriteTop = CCSprite::create(cardTypeName);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(cardTypeName);
	m_cardTypeSpriteTop->setVisible (false);
	if (m_cardType != GD_CARDS_TYPE_TONGHUA && m_cardType != GD_CARDS_TYPE_TIEZHI)
	{
		if (m_cardType != GD_CARDS_TYPE_SHUNZI && m_cardType != GD_CARDS_TYPE_BIGTONGHUASHUN)
			m_cardTypeSpriteTop->setPosition (ccp (-m_cardTypeSpriteTop->getContentSize ().width / 2, m_cardTypeSpriteTop->getContentSize ().height / 2));
		else
			m_cardTypeSpriteTop->setPosition (ccp (m_cardTypeSpriteTop->getContentSize ().width / 2, m_cardTypeSpriteTop->getContentSize ().height / 2));
	}
	else
	{
		m_cardTypeSpriteTop->setAnchorPoint (ccp (0.5f, 0.0f));
		m_cardTypeSpriteTop->setPosition (ccp (0.0f, 0.0f));
	}
	addChild(m_cardTypeSpriteTop);

	m_cardTypeTopAnimation = CCAnimation::create();  
	for (int i = 0; i < m_topAnimateFrame; i++)
	{
		sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_top_%d.png", m_cardType, i);
		if (CCFileUtils::sharedFileUtils ()->isFileExist (cardTypeName))
		{
			CCSprite * effectFrame = CCSprite::create (cardTypeName);
			m_cardTypeTopAnimation->addSpriteFrameWithTexture(effectFrame->getTexture (), CCRect (0, 0, effectFrame->getTexture ()->getPixelsWide (), effectFrame->getTexture ()->getPixelsHigh ()));
			CCTextureCache::sharedTextureCache()->removeTextureForKey(cardTypeName);
		}
	}
	m_cardTypeTopAnimation->setDelayPerUnit(1.0f / 15.0f);
	m_cardTypeTopAnimate = CCAnimate::create (m_cardTypeTopAnimation);
	m_cardTypeTopAnimate->retain ();


	if (m_cardType != GD_CARDS_TYPE_TONGHUA && m_cardType != GD_CARDS_TYPE_TIEZHI)
	{
		sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_top_0.png", m_cardType);
		m_cardTypeSpriteTopL = CCSprite::create(cardTypeName);
		m_cardTypeSpriteTopL->setVisible (false);
		m_cardTypeSpriteTopL->setFlipX (true);

		if (m_cardType != GD_CARDS_TYPE_SHUNZI && m_cardType != GD_CARDS_TYPE_BIGTONGHUASHUN)
			m_cardTypeSpriteTopL->setPosition (ccp (m_cardTypeSpriteTopL->getContentSize ().width / 2, m_cardTypeSpriteTopL->getContentSize ().height / 2));
		else
			m_cardTypeSpriteTopL->setPosition (ccp (-m_cardTypeSpriteTopL->getContentSize ().width / 2, m_cardTypeSpriteTopL->getContentSize ().height / 2));

		addChild(m_cardTypeSpriteTopL);

		m_cardTypeTopAnimationL = CCAnimation::create();  
		for (int i = 0; i < m_topAnimateFrame; i++)
		{
			sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d_top_%d.png", m_cardType, i);

			if (CCFileUtils::sharedFileUtils ()->isFileExist (cardTypeName))
			{
				CCSprite * effectFrame = CCSprite::create (cardTypeName);
				effectFrame->setFlipX (true);
				m_cardTypeTopAnimationL->addSpriteFrameWithTexture(effectFrame->getTexture (), CCRect (0, 0, effectFrame->getTexture ()->getPixelsWide (), effectFrame->getTexture ()->getPixelsHigh ()));
				CCTextureCache::sharedTextureCache()->removeTextureForKey(cardTypeName);
			}
		}
		m_cardTypeTopAnimationL->setDelayPerUnit(1.0f / 15.0f);
		m_cardTypeTopAnimateL = CCAnimate::create (m_cardTypeTopAnimationL);
		m_cardTypeTopAnimateL->retain ();
	}

	//牌型
	sprintf (cardTypeName, TP_IMAGE_PATH_CARDTYPE_EFFECT"tp_cardType_%d.png", m_cardType);
	m_cardTypeSprite = CCSprite::create (cardTypeName);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(cardTypeName);
	addChild (m_cardTypeSprite);
	m_cardTypeSprite->setVisible (false);

	sprintf (cardTypeName, TP_IMAGE_PATH_CARD_EFFECT"tp_star_0.png");
	
	for (int i = 0; i < MAX_LIGHT_COUNT_MAX; i++)
	{
		m_cardTypeLight[i] = CCSprite::create(cardTypeName);
		CCTextureCache::sharedTextureCache()->removeTextureForKey(cardTypeName);
		m_cardTypeLight[i]->setVisible (false);

		int xPer = m_cardTypeSprite->getContentSize ().width / 2;
		int yPer = m_cardTypeSprite->getContentSize ().height / 2;

		int x = rand () % xPer;
		int y = rand () % yPer;
		int a = rand () % 2;
		int b = rand () % 2;
		CCPoint pt;
		if (a == 0)
		{
			if (b == 0)
				pt = CCPoint (x, y);
			else 
				pt = CCPoint (x, -y);
		}
		else
		{
			if (b == 0)
				pt = CCPoint (-x, y);
			else 
				pt = CCPoint (-x, -y);
		}

		CCLOG ("Stars Pos: CardType[%d], %.2f , %.2f", (int)m_cardType, pt.x, pt.y);

		m_cardTypeLight[i]->setPosition (pt);
		addChild(m_cardTypeLight[i]);

		m_cardTypeLightAnimation[i] = CCAnimation::create();  
		for (int j = 0; j < 11; j++)
		{
			sprintf (cardTypeName, TP_IMAGE_PATH_CARD_EFFECT"tp_star_%d.png", j);
			if (CCFileUtils::sharedFileUtils ()->isFileExist (cardTypeName))
			{
				CCSprite * effectFrame = CCSprite::create (cardTypeName);
				m_cardTypeLightAnimation[i]->addSpriteFrameWithTexture(effectFrame->getTexture (), CCRect (0, 0, effectFrame->getTexture ()->getPixelsWide (), effectFrame->getTexture ()->getPixelsHigh ()));
				CCTextureCache::sharedTextureCache()->removeTextureForKey(cardTypeName);
			}
		}
		m_cardTypeLightAnimation[i]->setDelayPerUnit(1.0f / 30.0f);
		m_cardTypeLightAnimate[i] = CCAnimate::create (m_cardTypeLightAnimation[i]);
		m_cardTypeLightAnimate[i]->retain ();
	}
}

GDCardTypeEffect::~GDCardTypeEffect()
{
	removeCreateCardType ();
}

void GDCardTypeEffect::Play()
{
	//CCTintBy  * tink =  CCTintBy::create (0.45f, 255, 255, 255);
	//CCActionInterval  * tink2 =  tink->reverse ();

	CCShow * tink = CCShow::create ();
	CCShow * tink2 = CCShow::create ();

	if (m_cardType == GD_CARDS_TYPE_SANPAI)
	{
		m_cardTypeSpriteTop->runAction (CCSequence::create (CCShow::create (),  CCFadeIn::create (0.2f), NULL));
		m_cardTypeSpriteTopL->runAction (CCSequence::create (CCShow::create (), CCFadeIn::create (0.2f), NULL));
		m_cardTypeSpriteBomttom->runAction (CCSequence::create (CCShow::create (), CCFadeIn::create (0.2f), NULL));
		m_cardTypeSpriteBomttomL->runAction (CCSequence::create ( CCShow::create (), CCFadeIn::create (0.2f), NULL));
		m_cardTypeSprite->runAction (CCSequence::create (CCDelayTime::create (0.15f), CCShow::create (), CCFadeIn::create (0.2f), NULL));
	}
	else if (m_cardType >= GD_CARDS_TYPE_DUIZI && m_cardType <= GD_CARDS_TYPE_SHUNZI)
	{
		m_cardTypeSpriteBomttom->runAction (CCSequence::create (CCShow::create (),  m_cardTypeBomttomAnimate, NULL));
		m_cardTypeSpriteBomttomL->runAction (CCSequence::create (CCShow::create (),  m_cardTypeBomttomAnimateL, NULL));
		m_cardTypeSpriteTop->runAction (CCSequence::create (CCDelayTime::create (0.5f), CCShow::create (), CCFadeIn::create (0.35f), m_cardTypeTopAnimate, NULL));
		m_cardTypeSpriteTopL->runAction (CCSequence::create (CCDelayTime::create (0.5f), CCShow::create (), CCFadeIn::create (0.35f), m_cardTypeTopAnimateL, NULL));
		CCAction  * typeSpriteaction = CCSequence::create (CCDelayTime::create (0.85f), CCShow::create (), tink, tink2, NULL);
		m_cardTypeSprite->runAction (typeSpriteaction);
		for (int i = 0; i < MAX_LIGHT_COUNT_MAX; i++)
		{
			m_cardTypeLight[i]->runAction (CCSequence::create (CCDelayTime::create (0.85f), CCShow::create (), m_cardTypeLightAnimate[i], NULL));
		}

	}else if (m_cardType == GD_CARDS_TYPE_TONGHUA)
	{
		m_cardTypeSpriteBomttom->runAction (CCSequence::create (CCShow::create (), m_cardTypeBomttomAnimate, NULL));
		m_cardTypeSpriteBomttomL->runAction (CCSequence::create (CCShow::create (), m_cardTypeBomttomAnimateL, NULL) );
		m_cardTypeSpriteTop->setScaleY (0.0f);
		m_cardTypeSpriteTop->runAction (CCSequence::create (CCDelayTime::create (0.5f),CCShow::create (), CCScaleTo::create (0.6f, 1.0, 1.2f), CCScaleTo::create (0.5F, 1.0f, 1.0f), NULL));
		CCAction  * typeSpriteaction = CCSequence::create (CCDelayTime::create (0.5f), CCShow::create (), tink, tink2, NULL);
		m_cardTypeSprite->runAction (typeSpriteaction);
		for (int i = 0; i < MAX_LIGHT_COUNT_MAX; i++)
		{
			m_cardTypeLight[i]->runAction (CCSequence::create (CCDelayTime::create (0.5f), CCShow::create (), m_cardTypeLightAnimate[i], NULL));
		}
	}
	else if (m_cardType == GD_CARDS_TYPE_HULU)
	{
		m_cardTypeSpriteBomttom->runAction (CCSequence::create (CCShow::create (), m_cardTypeBomttomAnimate, NULL));
		m_cardTypeSpriteBomttomL->runAction (CCSequence::create (CCShow::create (), m_cardTypeBomttomAnimateL, NULL));
		m_cardTypeSpriteTop->runAction (CCSequence::create (CCShow::create (), m_cardTypeTopAnimate, NULL));
		m_cardTypeSpriteTopL->runAction (CCSequence::create (CCShow::create (), m_cardTypeTopAnimateL, NULL));
		CCAction  * typeSpriteaction = CCSequence::create (CCDelayTime::create (0.35f), CCShow::create (), tink, tink2, NULL);
		m_cardTypeSprite->runAction (typeSpriteaction);
		for (int i = 0; i < MAX_LIGHT_COUNT_MAX; i++)
		{
			m_cardTypeLight[i]->runAction (CCSequence::create (CCDelayTime::create (0.35f), CCShow::create (), m_cardTypeLightAnimate[i], NULL));
		}
	}
	else if (m_cardType == GD_CARDS_TYPE_TIEZHI)
	{
		m_cardTypeSpriteBomttom->runAction (CCSequence::create (CCShow::create (), m_cardTypeBomttomAnimate, NULL));
		m_cardTypeSpriteBomttomL->runAction (CCSequence::create (CCShow::create (), m_cardTypeBomttomAnimateL, NULL) );
		m_cardTypeSpriteTop->setScaleY (0.0f);
		m_cardTypeSpriteTop->runAction (CCSequence::create (CCDelayTime::create (0.5f),CCShow::create (), CCScaleTo::create (0.6f, 1.0, 1.2f), CCScaleTo::create (0.5F, 1.0f, 1.0f), NULL));
		CCAction  * typeSpriteaction = CCSequence::create (CCDelayTime::create (0.5f), CCShow::create (), tink, tink2, NULL);
		m_cardTypeSprite->runAction (typeSpriteaction);
		m_cardTypeEffectCircle->runAction (CCRepeatForever::create(CCSequence::create (CCShow::create (), CCRotateBy::create(0.5f, 30.0f), NULL)));
		for (int i = 0; i < MAX_LIGHT_COUNT_MAX; i++)
		{
			m_cardTypeLight[i]->runAction (CCSequence::create (CCDelayTime::create (0.5f), CCShow::create (), m_cardTypeLightAnimate[i], NULL));
		}
	}
	else if (m_cardType == GD_CARDS_TYPE_TONGHUASHUN)
	{
		m_cardTypeSpriteBomttom->runAction (CCSequence::create (CCShow::create (), m_cardTypeBomttomAnimate, NULL));
		m_cardTypeSpriteBomttomL->runAction (CCSequence::create (CCShow::create (), m_cardTypeBomttomAnimateL, NULL));
		m_cardTypeSpriteTop->runAction (CCSequence::create (CCShow::create (), m_cardTypeTopAnimate, NULL));
		m_cardTypeSpriteTopL->runAction (CCSequence::create (CCShow::create (), m_cardTypeTopAnimateL, NULL));
		CCAction  * typeSpriteaction = CCSequence::create (CCDelayTime::create (0.35f), CCShow::create (), tink, tink2, NULL);
		m_cardTypeSprite->runAction (typeSpriteaction);
		m_cardTypeEffectCircle->runAction (CCRepeatForever::create(CCSequence::create (CCShow::create (), CCRotateBy::create(0.5f, 30.0f), NULL)));
		for (int i = 0; i < MAX_LIGHT_COUNT_MAX; i++)
		{
			m_cardTypeLight[i]->runAction (CCSequence::create (CCDelayTime::create (0.35f), CCShow::create (), m_cardTypeLightAnimate[i], NULL));
		}
	}
	else if (m_cardType ==GD_CARDS_TYPE_BIGTONGHUASHUN)
	{
		m_cardTypeSpriteBomttom->runAction (CCSequence::create (CCShow::create (), m_cardTypeBomttomAnimate, NULL));
		m_cardTypeSpriteBomttomL->runAction (CCSequence::create (CCShow::create (), m_cardTypeBomttomAnimateL, NULL));
		m_cardTypeSpriteTop->runAction (CCSequence::create (CCShow::create (), m_cardTypeTopAnimate, NULL));
		m_cardTypeSpriteTopL->runAction (CCSequence::create (CCShow::create (), m_cardTypeTopAnimateL, NULL));
		CCAction  * typeSpriteaction = CCSequence::create (CCDelayTime::create (0.35f), CCShow::create (), tink, tink2, NULL);
		m_cardTypeSprite->runAction (typeSpriteaction);
		m_cardTypeEffectCircle->runAction (CCRepeatForever::create(CCSequence::create (CCShow::create (), CCRotateBy::create(0.5f, 30.0f), NULL)));
		for (int i = 0; i < MAX_LIGHT_COUNT_MAX; i++)
		{
			m_cardTypeLight[i]->runAction (CCSequence::create (CCDelayTime::create (0.35f), CCShow::create (), m_cardTypeLightAnimate[i], NULL));
		}
	}

	CCCallFunc* pkCall = CCCallFunc::create(this, callfunc_selector(GDCardTypeEffect::PlayEffectEnd));
	runAction (CCSequence::create (CCDelayTime::create (2.5F), pkCall, NULL));
}

void GDCardTypeEffect::PlayEffectEnd ()
{
	removeCreateCardType ();

	if (g_pkGuanDanData->GetIsNeedGuideInGD())
	{
		if (g_pLayerManager->GetLayer ("GuanDan_GuideManager") == NULL)
		{
			g_pLayerManager->AddLayer (GuanDanGuideManager::create ());
		}
		GuanDanGuideManager * target = (GuanDanGuideManager *) g_pLayerManager->GetLayer ("GuanDan_GuideManager");
		if (target)
			target->setCurrentStep (14);

		g_pLayerManager->PushLayer ("GuanDan_GuideManager");

		GuanDanGameView * gameview = (GuanDanGameView *)g_pLayerManager->GetLayer ("GuanDan_GameView");
		
		if (gameview != NULL)
			gameview->createGuidModeSceneStep1 ();
	}
	else
		g_pkGuanDanGame->PushCommand (CMD_GAME_CARDTYPE_PLAY_END);
}

void GDCardTypeEffect::Reset(ODEFloat32 f32Delta)
{
	unschedule (schedule_selector (GDCardTypeEffect::Reset));
	setVisible(false);
}

GuanDanHeGuanEffect::GuanDanHeGuanEffect(HEGUAN_ACTION_TYPE actionType)
{
	m_heguanBodySprite = NULL;

	isInMotion = false;

	m_upAnimationSprite.clear ();
	m_downAnimationSprite.clear ();
	m_leftEyesSprite.clear ();
	m_rightEyesSprite.clear ();
	m_tipsSprite.clear ();

	setScale(ui_main_scale);
	createHeGuanEffect (actionType);
}

GuanDanHeGuanEffect::~GuanDanHeGuanEffect()
{
}

void GuanDanHeGuanEffect::Play()
{
	unschedule (schedule_selector (GuanDanHeGuanEffect::updateHeGuanEffect));
	schedule (schedule_selector (GuanDanHeGuanEffect::updateHeGuanEffect), 0.07f);
}

void GuanDanHeGuanEffect::createHeGuanEffect (HEGUAN_ACTION_TYPE actionType)
{
	Reset (0.0f);

	m_HeguanActionType = actionType;

	isInMotion = true;

	char spriteFrameName[100] = {0};

	switch (m_HeguanActionType)
	{
	case HEGUAN_ACTION_TYPE_CLICK:
		{
			CCSprite * upSprite = CCSprite::create(TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_click_up.png");
			upSprite->setPosition (ccp (193.9, -154.85));
			CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_click_up.png");
			m_upAnimationSprite.push_back (upSprite);
			addChild(upSprite);
			upSprite->setVisible (false);

			for (int i = 0; i <= 1; i++)
			{
				sprintf (spriteFrameName, TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_click_down_%d.png", i);
				CCSprite * downSprite = CCSprite::create(spriteFrameName);
				downSprite->setPosition (ccp (179.5f, -202.50f));
				CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
				addChild(downSprite);
				downSprite->setVisible (false);
				m_downAnimationSprite.push_back(downSprite);
			}

			for (int i = 0; i <= 1; i++)
			{
				sprintf (spriteFrameName, TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_blink_l_%d.png", i);
				CCSprite * leftEyesSprite = CCSprite::create(spriteFrameName);
				if (i == 0)
					leftEyesSprite->setPosition (ccp (105.0f, -52.5f));
				else
					leftEyesSprite->setPosition (ccp (106.0f, -54.5f));

				CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
				addChild(leftEyesSprite);
				leftEyesSprite->setVisible (false);
				m_leftEyesSprite.push_back(leftEyesSprite);
			}

			for (int i = 0; i <= 1; i++)
			{
				sprintf (spriteFrameName, TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_blink_r_%d.png", i);
				CCSprite * rightEyes = CCSprite::create(spriteFrameName);
				if (i == 0)
					rightEyes->setPosition (ccp (132.5f, -52.5f));
				else
					rightEyes->setPosition (ccp (131.5f, -54.5f));
				CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
				addChild(rightEyes);
				rightEyes->setVisible (false);
				m_rightEyesSprite.push_back(rightEyes);
			}

			m_heguanBodySprite = CCSprite::create(TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_start_lh.png");
			m_heguanBodySprite->setPosition (ccp (174.50f, -168.45f));
			CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_start_lh.png");
			addChild(m_heguanBodySprite);
			m_heguanBodySprite->setVisible (true);
		}
		break;
	case HEGUAN_ACTION_TYPE_WAVE:
		/*m_UpAnimationSprite = CCSprite::create(TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_wave_up_0.png");
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_wave_up_0.png");
		addChild(m_UpAnimationSprite);
		for (int i = 0; i <= 3; i++)
		{
			sprintf (spriteFrameName, TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_wave_up_%d.png", i);
			m_UpAnimation->addSpriteFrameWithFileName(spriteFrameName);
			CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
		}

		m_DownAnimationSprite = CCSprite::create(TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_wave_down_0.png");
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_wave_down_0.png");
		addChild(m_DownAnimationSprite);
		for (int i = 0; i <= 4; i++)
		{
			sprintf (spriteFrameName, TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_wave_down_%d.png", i);
			m_DownAnimation->addSpriteFrameWithFileName(spriteFrameName);
			CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
		}*/
		break;
	case HEGUAN_ACTION_TYPE_LOVE:
		{
			for (int i = 0; i <= 2; i++)
			{
				//捂嘴和招手
				if (i >= 1)
				{
					sprintf (spriteFrameName, TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_love_up_1.png");
					CCSprite * upSprite = CCSprite::create(spriteFrameName);
					upSprite->setPosition (ccp (168.2, -155.6));
					CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
					m_upAnimationSprite.push_back (upSprite);
					addChild(upSprite);
					upSprite->setVisible (false);
				}
				else
				{
					//过渡
					sprintf (spriteFrameName, TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_love_up_%d.png", i);
					CCSprite * upSprite = CCSprite::create(spriteFrameName);
					upSprite->setPosition (ccp (182.5, -159.95));
					CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
					m_upAnimationSprite.push_back (upSprite);
					addChild(upSprite);
					upSprite->setVisible (false);
				}
			}

			for (int i = 0; i <= 2; i++)
			{
				sprintf (spriteFrameName, TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_love_down_%d.png", i);
				CCSprite * downSprite = CCSprite::create(spriteFrameName);

				if (i == 0)
				{
					downSprite->setPosition (ccp (166.5f, -183.40f));
				}
				else if (i == 1)
				{
					downSprite->setPosition (ccp (151.75f, -144.95f));
				}
				else if (i == 2)
				{
					downSprite->setPosition (ccp (160.25f, -156.95f));
				}

				CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
				addChild(downSprite);
				downSprite->setVisible (false);
				m_downAnimationSprite.push_back(downSprite);
			}

			for (int i = 0; i <= 1; i++)
			{
				sprintf (spriteFrameName, TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_love_tip_%d.png", i);
				CCSprite * leftEyesSprite = CCSprite::create(spriteFrameName);
				leftEyesSprite->setPosition (ccp (106.75f, -51.95f));
				CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
				addChild(leftEyesSprite);
				leftEyesSprite->setVisible (false);
				m_leftEyesSprite.push_back(leftEyesSprite);
			}

			for (int i = 0; i < 1; i++)
			{
				sprintf (spriteFrameName, TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_love_tip_2.png");
				CCSprite * tipsSprite = CCSprite::create(spriteFrameName);
				tipsSprite->setPosition (ccp (119.0f, -76.95f));
				CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
				addChild(tipsSprite);
				tipsSprite->setVisible (false);
				m_tipsSprite.push_back(tipsSprite);
			}

			m_heguanBodySprite = CCSprite::create(TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_start_lh.png");
			m_heguanBodySprite->setPosition (ccp (174.50f, -168.45f));
			CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_HEGUAN_EFFECT"tp_hg_start_lh.png");
			addChild(m_heguanBodySprite);
			m_heguanBodySprite->setVisible (true);
		}
		break;
	default:
		break;
	}
}



void GuanDanHeGuanEffect::setUpAnimationSpriteVisibleAll (bool bShow)
{
	for (int i = 0; i < m_upAnimationSprite.size (); i++)
	{
		if (m_upAnimationSprite[i] != NULL)
		{
			m_upAnimationSprite[i]->setVisible (bShow);
		}
	}
}

void GuanDanHeGuanEffect::setDownAnimationSpriteVisibleAll (bool bShow)
{
	for (int i = 0; i < m_downAnimationSprite.size (); i++)
	{
		if (m_downAnimationSprite[i] != NULL)
		{
			m_downAnimationSprite[i]->setVisible (bShow);
		}
	}
}

void GuanDanHeGuanEffect::setLeftEyesAnimationSpriteVisibleAll (bool bShow)
{
	for (int i = 0; i < m_leftEyesSprite.size (); i++)
	{
		if (m_leftEyesSprite[i] != NULL)
		{
			m_leftEyesSprite[i]->setVisible (bShow);
		}
	}
}

void GuanDanHeGuanEffect::setRightEyesAnimationSpriteVisibleAll (bool bShow)
{
	for (int i = 0; i < m_rightEyesSprite.size (); i++)
	{
		if (m_rightEyesSprite[i] != NULL)
		{
			m_rightEyesSprite[i]->setVisible (bShow);
		}
	}
}

void GuanDanHeGuanEffect::setTipsAnimationSpriteVisibleAll (bool bShow)
{
	for (int i = 0; i < m_tipsSprite.size (); i++)
	{
		if (m_tipsSprite[i] != NULL)
		{
			m_tipsSprite[i]->setVisible (bShow);
		}
	}
}

void GuanDanHeGuanEffect::updateHeGuanEffect (ODEFloat32 f32Delta)
{
	if (m_HeguanActionType == HEGUAN_ACTION_TYPE_CLICK)
	{
		switch (m_currentSteps)
		{
		case 0:
			m_heguanBodySprite->setVisible (true);
			setUpAnimationSpriteVisibleAll (false);
			m_upAnimationSprite[0]->setVisible (true);
			setDownAnimationSpriteVisibleAll (false);
			setLeftEyesAnimationSpriteVisibleAll (false);
			setRightEyesAnimationSpriteVisibleAll (false);
			break;
		case 1:
			setLeftEyesAnimationSpriteVisibleAll (false);
			setRightEyesAnimationSpriteVisibleAll (false);
			m_heguanBodySprite->setVisible (false);
			m_upAnimationSprite[0]->setVisible (true);
			m_downAnimationSprite[1]->setVisible (true);
			//m_leftEyesSprite[0]->setVisible (true);
			//m_rightEyesSprite[0]->setVisible (true);
			break;
		case 2:
			setDownAnimationSpriteVisibleAll (false);
			setLeftEyesAnimationSpriteVisibleAll (false);
			setRightEyesAnimationSpriteVisibleAll (false);
			m_heguanBodySprite->setVisible (false);
			m_upAnimationSprite[0]->setVisible (true);
			m_downAnimationSprite[0]->setVisible (true);
			g_pSoundSystem->PlayEffect (GUANDAN_SE_HEGUAN_KNOCK);
			//m_leftEyesSprite[1]->setVisible (true);
			//m_rightEyesSprite[1]->setVisible (true);
			break;
		case 3:
			setDownAnimationSpriteVisibleAll (false);
			setLeftEyesAnimationSpriteVisibleAll (false);
			setRightEyesAnimationSpriteVisibleAll (false);
			m_heguanBodySprite->setVisible (false);
			m_upAnimationSprite[0]->setVisible (true);
			m_downAnimationSprite[1]->setVisible (true);
			//m_leftEyesSprite[0]->setVisible (true);
			//m_rightEyesSprite[0]->setVisible (true);
			break;
		case 4:
			setDownAnimationSpriteVisibleAll (false);
			setLeftEyesAnimationSpriteVisibleAll (false);
			setRightEyesAnimationSpriteVisibleAll (false);
			m_heguanBodySprite->setVisible (false);
			m_upAnimationSprite[0]->setVisible (true);
			m_downAnimationSprite[0]->setVisible (true);
			break;
		case 5:
			setLeftEyesAnimationSpriteVisibleAll (false);
			setRightEyesAnimationSpriteVisibleAll (false);
			m_heguanBodySprite->setVisible (true);
			setUpAnimationSpriteVisibleAll (false);
			setDownAnimationSpriteVisibleAll (false);
			break;
		default:
			unschedule (schedule_selector (GuanDanHeGuanEffect::updateHeGuanEffect));
			EffectPlayEnd ();
			break;
		}
		m_currentSteps ++;
	}
	else if (m_HeguanActionType == HEGUAN_ACTION_TYPE_LOVE)
	{
		switch (m_currentSteps)
		{
		case 0:
			m_heguanBodySprite->setVisible (true);
			setUpAnimationSpriteVisibleAll (false);
			setDownAnimationSpriteVisibleAll (false);	
			setLeftEyesAnimationSpriteVisibleAll (false);
			setTipsAnimationSpriteVisibleAll (false);
			break;
		case 1:
			m_heguanBodySprite->setVisible (false);
			setUpAnimationSpriteVisibleAll (false);
			setDownAnimationSpriteVisibleAll (false);	
			m_upAnimationSprite[0]->setVisible (true);
			m_downAnimationSprite[0]->setVisible (true);
			break;
		case 2:
			m_heguanBodySprite->setVisible (false);
			setUpAnimationSpriteVisibleAll (false);
			setDownAnimationSpriteVisibleAll (false);	
			setLeftEyesAnimationSpriteVisibleAll (false);
			m_upAnimationSprite[1]->setVisible (true);
			m_downAnimationSprite[1]->setVisible (true);
			m_leftEyesSprite[0]->setVisible (true);
			m_tipsSprite[0]->setVisible (true);
			break;
		case 3:
			m_heguanBodySprite->setVisible (false);
			setUpAnimationSpriteVisibleAll (false);
			setDownAnimationSpriteVisibleAll (false);	
			setLeftEyesAnimationSpriteVisibleAll (false);
			m_upAnimationSprite[2]->setVisible (true);
			m_downAnimationSprite[2]->setVisible (true);
			m_leftEyesSprite[1]->setVisible (true);
			m_tipsSprite[0]->setVisible (true);
			g_pSoundSystem->PlayEffect (GUANDAN_SE_HEGUAN_LOVE);
			break;
		case 4:
			m_heguanBodySprite->setVisible (false);
			setUpAnimationSpriteVisibleAll (false);
			setDownAnimationSpriteVisibleAll (false);
			setLeftEyesAnimationSpriteVisibleAll (false);
			m_upAnimationSprite[0]->setVisible (true);
			m_downAnimationSprite[0]->setVisible (true);
			m_leftEyesSprite[0]->setVisible (true);
			m_tipsSprite[0]->setVisible (true);
			break;
		case 5:
			m_heguanBodySprite->setVisible (true);
			setUpAnimationSpriteVisibleAll (false);
			setDownAnimationSpriteVisibleAll (false);
			setLeftEyesAnimationSpriteVisibleAll (false);
			setTipsAnimationSpriteVisibleAll (false);
			break;
		default:
			unschedule (schedule_selector (GuanDanHeGuanEffect::updateHeGuanEffect));
			EffectPlayEnd ();
			break;
		}
		m_currentSteps ++;
	}
}

void GuanDanHeGuanEffect::EffectPlayEnd ()
{
	m_currentSteps = 0;

	isInMotion = false;

	if (m_HeguanActionType == HEGUAN_ACTION_TYPE_LOVE)
		g_pkGuanDanGame->PushCommand (CMD_HEGUAN_MOTION_LOVE_PLAY_END);
	else
		g_pkGuanDanGame->PushCommand (CMD_HEGUAN_MOTION_PLAY_END);
}

void GuanDanHeGuanEffect::Reset( ODEFloat32 f32Delta )
{
	m_currentSteps = 0;

	if (m_heguanBodySprite)
	{
		m_heguanBodySprite->setVisible (true);
		removeChild (m_heguanBodySprite);
		m_heguanBodySprite = NULL;
	}
	
	for (int i = 0; i < m_leftEyesSprite.size (); i++)
	{
		m_leftEyesSprite[i]->setVisible (false);
		removeChild (m_leftEyesSprite[i]);
		m_leftEyesSprite[i] = NULL;
	}

	for (int i = 0; i < m_rightEyesSprite.size (); i++)
	{
		m_rightEyesSprite[i]->setVisible (false);
		removeChild (m_rightEyesSprite[i]);
		m_rightEyesSprite[i] = NULL;
	}
	
	for (int i = 0; i < m_tipsSprite.size (); i++)
	{
		m_tipsSprite[i]->setVisible (false);
		removeChild (m_tipsSprite[i]);
		m_tipsSprite[i] = NULL;
	}

	for (int i = 0; i < m_upAnimationSprite.size (); i++)
	{
		m_upAnimationSprite[i]->setVisible (false);
		removeChild (m_upAnimationSprite[i]);
		m_upAnimationSprite[i] = NULL;
	}

	for (int i = 0; i < m_downAnimationSprite.size (); i++)
	{
		m_downAnimationSprite[i]->setVisible (false);
		removeChild (m_downAnimationSprite[i]);
		m_downAnimationSprite[i] = NULL;
	}

	m_upAnimationSprite.clear ();
	m_downAnimationSprite.clear ();
	m_leftEyesSprite.clear ();
	m_rightEyesSprite.clear ();
	m_tipsSprite.clear ();
}

GDPlayerEffect::GDPlayerEffect(PlayerNodeEffect effectType)
{
	m_playerNodeSprite = NULL;
	setScale(ui_main_scale);
	createPlayerNodeEffect (effectType);
}

void GDPlayerEffect::createPlayerNodeEffect (PlayerNodeEffect effectType)
{
	Reset (0.0f);

	m_PlayerNodeType = effectType;

	CCAnimation* m_playerNodeAnimation = CCAnimation::create();  
	char spriteFrameName[100] = {0};
	switch (m_PlayerNodeType)
	{
	case PlayerNodeEffect_Light :
		m_playerNodeSprite = CCSprite::create(TP_IMAGE_PATH_CARD_EFFECT"tp_card_effect_0.png");
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_CARD_EFFECT"tp_card_effect_0.png");
		addChild(m_playerNodeSprite);
		for (int i = 0; i <= 10; i++)
		{
			sprintf (spriteFrameName, TP_IMAGE_PATH_CARD_EFFECT"tp_card_effect_%d.png", i);
			m_playerNodeAnimation->addSpriteFrameWithFileName(spriteFrameName);
			CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
		}
		break;
	case PlayerNodeEffect_Heart:
		m_playerNodeSprite = CCSprite::create(TP_IMAGE_PATH_CARD_EFFECT"tp_heart_effect_0.png");
		CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_CARD_EFFECT"tp_heart_effect_0.png");
		addChild(m_playerNodeSprite);
		for (int i = 0; i <= 15; i++)
		{
			sprintf (spriteFrameName, TP_IMAGE_PATH_CARD_EFFECT"tp_heart_effect_%d.png", i);
			m_playerNodeAnimation->addSpriteFrameWithFileName(spriteFrameName);
			CCTextureCache::sharedTextureCache()->removeTextureForKey(spriteFrameName);
		}
		break;
	default:
		break;
	}
	m_playerNodeAnimation->setDelayPerUnit(1.0f / 15.0f);
	m_playerNodeAnimate = CCAnimate::create (m_playerNodeAnimation);
	m_playerNodeAnimate->retain ();
	m_playerNodeSprite->setVisible (false);
}

GDPlayerEffect::~GDPlayerEffect()
{
	Reset (0.0f);
	CC_SAFE_RELEASE(m_playerNodeAnimate);
}

void GDPlayerEffect::Play()
{
	m_playerNodeSprite->setVisible (true);

	CCCallFunc* pkCall = CCCallFunc::create(this, callfunc_selector(GDPlayerEffect::EffectPlayEnd));

	if (m_PlayerNodeType == PlayerNodeEffect_Light)
		m_playerNodeSprite->runAction (CCSequence::create (CCRepeat::create(m_playerNodeAnimate, 3), pkCall, NULL));
	else
		m_playerNodeSprite->runAction (CCSequence::create(m_playerNodeAnimate, pkCall, NULL));
}

void GDPlayerEffect::EffectPlayEnd ()
{
	if (m_playerNodeSprite)
		m_playerNodeSprite->setVisible (false);
	if (m_playerNodeSprite != NULL)
	{
		removeChild (m_playerNodeSprite);
		m_playerNodeSprite = NULL;
	}
}

void GDPlayerEffect::Reset( ODEFloat32 f32Delta )
{
	if (m_playerNodeSprite)
		m_playerNodeSprite->setVisible (false);

	if (m_playerNodeSprite != NULL)
	{
		removeChild (m_playerNodeSprite);
		m_playerNodeSprite = NULL;
	}
}

//----------------------------------------------///
//  聊天信息显示
//---------------------------------------------///

ChatMessageDisplay::ChatMessageDisplay()
{

}

ChatMessageDisplay::~ChatMessageDisplay()
{

}

void ChatMessageDisplay::OnEnd()
{
	ODUIScrollDisplay::OnEnd();
	this->runAction(CCSequence::create(CCDelayTime::create(1.0f), CCCallFunc::create(this,callfunc_selector(ChatMessageDisplay::DelayExc)), NULL));
}

void ChatMessageDisplay::DelayExc()
{
	GetParent()->SetVisible(false);
}


//----------------------------------------------///
//  聊天表情动画帧
//---------------------------------------------///
GuanDanChatInfoFrame::GuanDanChatInfoFrame()
{
	setScale(ui_main_scale);
	m_pkPanel = CCScale9Sprite::create(GD_IMAGE_PATH_INGAME"sound_chat.png");//, CCRect (0, 0, 88, 50), CCRect (70, 16, 2, 2)
    //m_pkPanel = ODUIPanel::createWithCltBlBt("alert_frame4", 120 * ui_main_scale * g_f32ScaleHD, 30 * ui_main_scale * g_f32ScaleHD);
	m_pkPanel->setPreferredSize(CCSizeMake(120* g_f32ScaleHD,90* g_f32ScaleHD));
	addChild(m_pkPanel);

	m_pkContentText = CCLabelTTF::create("test message", TTF_NAME, ODEFloorf(12 * g_f32ScaleHD));
	m_pkPanel->addChild(m_pkContentText, 1);
	m_pkContentText->setVisible(false);
	m_pkContentText->setColor(ccc3(255,255,0));

	m_pkTempText = CCLabelTTF::create("", TTF_NAME, 12* g_f32ScaleHD);
	m_pkPanel->addChild(m_pkTempText, 2);
	m_pkTempText->setVisible(false);
	m_pkTempText->setColor(ccc3(255,255,0));

	CCSize kSize;
	kSize = m_pkContentText->getContentSize();
	m_pkContentText->setAnchorPoint(ccp(0.5,0.5));
	m_pkPanel->setPreferredSize(CCSizeMake(20* g_f32ScaleHD + kSize.width, 35 * g_f32ScaleHD));

	m_f32Blend = 0.0f;
	UpdateOpacity();

	m_pkChatMessageDisplay = ODE_NEW ChatMessageDisplay;
	AddChild(m_pkChatMessageDisplay);

	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_INGAEM"sound_chat.png");
}

GuanDanChatInfoFrame::~GuanDanChatInfoFrame()
{

}

void GuanDanChatInfoFrame::Show( ODEUInt32 u32Index, const ODEChar8* pcContext )
{
	stopAllActions();
	unscheduleAllSelectors();
	SetVisible(true);
	CCPoint kPoint;
	kPoint.x = 0.0f;
	kPoint.y = 130.0f*ui_main_scale;
	if (u32Index==0)
	{
		kPoint.x=750.0f*ui_main_scale;
	}
	 else if (u32Index==1)
	{
		kPoint.x = 300.0f*ui_main_scale;
	} 
	 else if (u32Index==2)
	 {
		 kPoint.x=650*ui_main_scale;
	 }
	else if (u32Index==3)
	{
		kPoint.x =1000*ui_main_scale;
	
	}
	
	
	CCSize kSize;
	m_f32ShowTime = 1.5f;
	m_f32Blend = 0.0f;
	CCLog("%d",ODEStrlen(pcContext));
	m_pkContentText->setString(pcContext);
	if (/*ODEStrlen(pcContext) <= CHAR_MSG_MAX_LEN*/m_pkContentText->getContentSize().width<=264)
	{
		m_pkContentText->setVisible(true);
		m_pkPanel->setOpacity(0);
		m_pkContentText->setOpacity(0);
		m_pkTempText->setVisible(false);

	//	m_pkContentText->setString(pcContext);
		kSize = m_pkContentText->getContentSize();
		kSize.width = m_pkContentText->getContentSize().width/* * (ODEStrlen(pcContext) / 3)*/;

		ODEInt32 i32OldLen =ODEStrlen(pcContext);
		ODEFloat32 f32FixedX = abs((int)((i32OldLen - CHAR_MSG_MAX_LEN)  / 2 * 5.0f));
		kPoint.x += f32FixedX;
		this->setPositionX(kPoint.x);
		if (m_pkContentText->getContentSize().width<=48)
		{
			m_pkPanel->setPreferredSize(CCSizeMake(20* g_f32ScaleHD + 50, 35* g_f32ScaleHD));
			m_pkContentText->setPosition(ccp(m_pkContentText->getContentSize().width,m_pkContentText->getContentSize().height*1.4));
			m_pkTempText->setPosition(ccp(m_pkTempText->getContentSize().width,m_pkTempText->getContentSize().height*1.4));
			this->setPositionX(kPoint.x-50);
		}
		else
		{
			m_pkPanel->setPreferredSize(CCSizeMake(20* g_f32ScaleHD + kSize.width, 35* g_f32ScaleHD));
			m_pkContentText->setPosition(ccp(m_pkContentText->getContentSize().width*0.6,m_pkContentText->getContentSize().height*1.4));
			m_pkTempText->setPosition(ccp(m_pkTempText->getContentSize().width*0.6,m_pkTempText->getContentSize().height*1.4));
		}		

		schedule(schedule_selector(GuanDanChatInfoFrame::Update), 0.0f);
		
	}
 	else
	{
		m_pkContentText->setString(pcContext);
		m_pkTempText->setString(pcContext);
		this->setPositionX(kPoint.x);

		if (ODEStrlen(pcContext) < CHAR_MSG_MAX_LEN)
		{
			if (m_pkContentText->getContentSize().width <= MASSAGE_MAX_WIDTH)
			{
				m_pkContentText->setVisible(true);
				m_pkPanel->setOpacity(0);
				m_pkTempText->setVisible(false);

				m_pkPanel->setPreferredSize(CCSizeMake(20* g_f32ScaleHD + MASSAGE_MAX_WIDTH, 35* g_f32ScaleHD));

				schedule(schedule_selector(GuanDanChatInfoFrame::Update), 0.0f);
			}
			else
			{
				m_pkContentText->setVisible(false);
				m_pkPanel->setOpacity(255);
				m_pkTempText->setVisible(true);

				m_pkPanel->setPreferredSize(CCSizeMake(20* g_f32ScaleHD + MASSAGE_MAX_WIDTH,35* g_f32ScaleHD));

				m_pkChatMessageDisplay->SetDisplayObject(m_pkTempText, CCSizeMake(MASSAGE_MAX_WIDTH+10* g_f32ScaleHD , MASSAGE_MAX_HEIGHT), 2.3f, ktLeft, false);
				m_pkChatMessageDisplay->Start(1.0f);
			}
		}
		else
		{
			m_pkContentText->setVisible(false);
			m_pkPanel->setOpacity(255);
			m_pkTempText->setVisible(true);

			m_pkPanel->setPreferredSize(CCSizeMake(20* g_f32ScaleHD + MASSAGE_MAX_WIDTH, 35* g_f32ScaleHD));

			m_pkChatMessageDisplay->SetDisplayObject(m_pkTempText, CCSizeMake(MASSAGE_MAX_WIDTH+10* g_f32ScaleHD, MASSAGE_MAX_HEIGHT), 2.3f, ktLeft, false);
			m_pkChatMessageDisplay->Start(1.0f);
		}
		m_pkContentText->setPosition(ccp(m_pkContentText->getContentSize().width*3/5-10* g_f32ScaleHD,m_pkContentText->getContentSize().height*1.2));
		m_pkTempText->setPosition(ccp(m_pkTempText->getContentSize().width*3/5-10* g_f32ScaleHD,m_pkTempText->getContentSize().height*1.2));
	}	
}

void GuanDanChatInfoFrame::UpdateOpacity()
{
	ODEUInt8 u8Blend = m_f32Blend * 255.0f;
	m_pkContentText->setOpacity(u8Blend);
	m_pkPanel->setOpacity(u8Blend);
}

void GuanDanChatInfoFrame::Update( ODEFloat32 f32Delta )
{
	if(m_f32ShowTime > 0.0f)
	{
		if(m_f32Blend < 1.0f)
		{
			m_f32Blend += f32Delta * 5.0f;
			if(m_f32Blend >= 1.0f)
			{
				m_f32Blend = 1.0f;
			}
		}
		if(m_f32Blend >= 1.0f)
		{
			m_f32ShowTime -= f32Delta;
		}
	}
	if(m_f32ShowTime <= 0.0f)
	{
		m_f32ShowTime = 0.0f;
		if(m_f32Blend > 0.0f)
		{
			m_f32Blend -= f32Delta * 5.0f;
			if(m_f32Blend <= 0.0f)
			{
				m_f32Blend = 0.0f;
				unscheduleAllSelectors();
			}
		}
	}
	UpdateOpacity();
}

void GuanDanChatInfoFrame::FadeOut()
{
	m_f32Blend = 255.0f;
	schedule(schedule_selector(GuanDanChatInfoFrame::Update), 0.0f);
}
//表情 
GuanDanExpressionEffect::GuanDanExpressionEffect()
{
	//setScale(ui_main_scale);
}

GuanDanExpressionEffect::~GuanDanExpressionEffect()
{

}

void GuanDanExpressionEffect::Play( ODEUInt32 u32Index )
{
	ODEChar8 acBufferFaceName[128]={0};

	removeAllChildrenWithCleanup(true);

	GuanDanChatAnimate::loadPlist();

	GuanDanChatAnimiStruct* animiStruct = GuanDanChatAnimate::getAnimateByIndex((GuanDanChatAnimateEnum)u32Index);

	if (animiStruct != NULL)
	{
		CCSprite* animiSprite = animiStruct->animiSprite;
		CCAnimate* animate = animiStruct->animate;

		if (animiSprite != NULL)
		{
			animiSprite->setOpacity(0);
			addChild(animiSprite, 2);
			animiSprite->runAction(CCSequence::create(CCFadeIn::create(0.5f), CCRepeat::create(animate, 3), CCFadeOut::create(0.5f), NULL));
		}

		CC_SAFE_DELETE(animiStruct);
	}

//	GuanDanChatAnimate::unLoadPlist();
}

//道具滚动列表
GuanDanToolList::Item::Item()
{
	m_pkButton = NULL;
}

GuanDanToolList::GuanDanToolList(ODEFloat32 w, ODEFloat32 h)
	: ODUIScrollViewH(w * ui_width_scale , h * ui_main_scale,
	"scroll_bean.png", "scroll_bean_ext.png", 3.0f * g_f32ScaleHD)
	,m_fWidth(w)
	,m_fHeight(h)
{
	//setScale(ui_main_scale);
	setAnchorPoint(ccp(0.5f,1.0f));	
	m_vecToolButtonArray.Reserve(128);
	ODEUInt32 u32Res = m_vecToolButtonArray.Size();
	ODEUInt32 u32Col = u32Res;

	isFirst=CCUserDefault::sharedUserDefault()->getBoolForKey("first",true);
	User_ID=CCUserDefault::sharedUserDefault()->getIntegerForKey("ID");


	ODEChar8 Buffer[256] = {0};
	char* label_money[10]={" 50"," 50"," 50"," 50","100","100","100","200","200","200"};
	for (int i=0;i<10;i++)
	{	
		sprintf(Buffer,"TexasPoker/image/tool/tool_%d.png",i);
		CCSprite* pNormalTool=CCSprite::create(Buffer);
		CCSprite* pPressTool=CCSprite::create(Buffer);
		ODUIButton* pToolBtn = ODE_NEW ODUIButton(pNormalTool, pPressTool);
		pToolBtn->setPosition(ccp(500*ui_width_scale+110*i*ui_width_scale,90*ui_main_scale));
		pToolBtn->setScale(ui_main_scale);
		pToolBtn->setTag(i);
		CCTextureCache::sharedTextureCache()->removeTextureForKey(Buffer);

		m_vecToolButtonArray.Resize(m_vecToolButtonArray.Size() + 1);
		Item& kToolBtnItem = m_vecToolButtonArray.Back();
		kToolBtnItem.m_pkButton = pToolBtn;
		kToolBtnItem.m_kDelegate.Set(this, &GuanDanToolList::OnToolBtnClicked);
		kToolBtnItem.m_pkButton->RegisterListener(kToolBtnItem.m_kDelegate);

		pToolBtn->setContentSize(CCSize (90, 60));

		AddChild(pToolBtn,3);

		CCSprite*pBackGround=CCSprite::create("TexasPoker/image/tool/Round.png");
		pToolBtn->addChild(pBackGround,0);

		CCSprite*pFrame=CCSprite::create("TexasPoker/image/tool/Frame.png");
		pFrame->setAnchorPoint(ccp(0.4,2.3));
		pToolBtn->addChild(pFrame,0);

		CCSprite* pGold=CCSprite::create("TexasPoker/image/tool/Gold.png");
		pGold->setAnchorPoint(ccp(0.1,0.14));
		pFrame->addChild(pGold,0);

		CCLabelTTF* Pmoney=CCLabelTTF::create(label_money[i],TTF_NAME,23);
		Pmoney->setColor(ccc3(255,255,0));
		Pmoney->setAnchorPoint(ccp(-1.2,0.1));
		pFrame->addChild(Pmoney,0);
	}
	CCTextureCache::sharedTextureCache()->removeTextureForKey("TexasPoker/image/tool/Round.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey("TexasPokerimage/tool/Frame.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey("TexasPoker/image/tool/Gold.png");
}

GuanDanToolList::~GuanDanToolList()
{

}

bool GuanDanToolList::init()
{

	return true;
}

void GuanDanToolList::OnToolBtnClicked( ODUIWidget& kWidget, ODEUInt32 u32State )
{
	if (u32State==RELEASED)
	{
		CCLog("%s","Click");
		ODUIButton* pButton=(ODUIButton*)&kWidget;

		GuanDanHeaderTips * target = (GuanDanHeaderTips *)g_pLayerManager->GetLayer ("GuanDan_HeaderTips");
		int PlayerIndex = 0;
		if (target)
		{
			PlayerIndex = target->getPlayerIndex ();
		}

		int myIndex = g_pkGuanDanGamePlayerData->m_iTableNumExtra;
		if (myIndex != PlayerIndex)
		{
				long long money=g_pkGuanDanGamePlayerData->m_PlayerNode[g_pkGuanDanGamePlayerData->m_iTableNumExtra].iMoney;
				int ToolId=0;	
				int itemsCost = 0;
				int tag = pButton->getTag ();

				if (tag==0){
					itemsCost = 50;
					ToolId=11004;
				}
				else if (tag == 1){
					itemsCost = 50;
					ToolId=11011;
				}
				else if (tag == 2){
					itemsCost = 50;
					ToolId=11003;
				}
				else if (tag == 3){
					itemsCost = 50;
					ToolId=11012;
				}
				else if (tag == 4){
					itemsCost = 100;
					ToolId=11006;
				}
				else if (tag == 5){
					itemsCost = 100;
					ToolId=11013;
				}
				else if (tag == 6){
					itemsCost = 100;
					ToolId=11007;
				}
				else if (tag == 7){
					itemsCost = 200;
					ToolId=11015;
				}
				else if (tag == 8){
					itemsCost = 200;
					ToolId=11008;
				}
				else if (tag == 9){
					itemsCost = 200;
					ToolId=11016;
				}

				if (money<itemsCost){
					g_pkGuanDanGame->Toast(TT("LowScoreContent"));
					return ;
				}

				bool isFirstUserItems = CCUserDefault::sharedUserDefault()->getBoolForKey("first_use_items_in_app", true);
				if (isFirstUserItems)
				{
					CCUserDefault::sharedUserDefault()->setBoolForKey("first_use_items_in_app", false);
					CCUserDefault::sharedUserDefault()->setIntegerForKey("toolid",ToolId);
					CCUserDefault::sharedUserDefault()->setIntegerForKey("index",PlayerIndex);
					CCUserDefault::sharedUserDefault()->flush();
					g_pLayerManager->PushLayer("alert", ODE_NEW GuanDanFirstUseToolCallback);
					return;
				}				
				SendUseToolReq(ToolId,PlayerIndex);
				g_pLayerManager->PopAllLayers();
		}
		else
		{
			g_pkGuanDanGame->Toast(TT("UITextUserToolResult"));
			g_pLayerManager->PopAllLayers();
		}
	}

}
//使用道具请求
void GuanDanToolList::SendUseToolReq( unsigned int toolId, unsigned int chairNum )
{
	CSInteractPropsReq msg={0};
	msg.propsID=toolId;
	msg.toChair=chairNum;
	if (g_pkNetworkHelper->GetGameServerDelegate())
	{
		g_pkNetworkHelper->GetGameServerDelegate()->SendMsgToSocket(&msg, CS_MSG_INTERACTPROPS_REQ, sizeof(CSInteractPropsReq));
	}		
}

//游戏内任务
GuanDanTaskAndActivityWidget::GuanDanTaskAndActivityWidget()
{
	setContentSize(CCSizeMake(248.0f, 400.0f));

	UIInitDelegate(GuanDanTaskAndActivityWidget, OnChangeTask);
	UIInitDelegate(GuanDanTaskAndActivityWidget, OnGetGift);
	UIInitDelegate(GuanDanTaskAndActivityWidget, OnShowDetailInfo);

	CCSprite* pWidgetBg = CCSprite::create(TP_TASK_PATH"KessenTaskWidgetBg.png");
	addChild(pWidgetBg);

	m_taskCloseLabel = CCLabelTTF::create(TT("UIText_Task_Not_Open"), TTF_NAME, 30);
	m_taskCloseLabel->setAnchorPoint(ccp(0.5f, 0.5f));
	m_taskCloseLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
	m_taskCloseLabel->setColor(ccc3(203, 168, 47));
	addChild(m_taskCloseLabel);

	CCSprite * normalShow = CCSprite::create();
	CCSprite * pressShow = CCSprite::create();
	m_ShowDetailInfoBtn = ODE_NEW ODUIButton(normalShow, pressShow);
	m_ShowDetailInfoBtn->setContentSize(CCSizeMake(232.0f, 245.0f));
	m_ShowDetailInfoBtn->setPositionY(pWidgetBg->getPositionY() + 40.0f);
	AddChild(m_ShowDetailInfoBtn, 3);
	UILinkDelegate(m_ShowDetailInfoBtn, OnShowDetailInfo);
	m_ShowDetailInfoBtn->SetVisible(false);

	m_ActivityTextBg = CCScale9Sprite::create(TP_TASK_PATH"KessenTaskActivitytextBG.png");//, CCRectMake(7, 7, 2, 2));
	m_ActivityTextBg->setPreferredSize(CCSizeMake(234, 41));
	m_ActivityTextBg->setPositionY(185.0f);
	m_ActivityTextBg->setVisible(false);
	addChild(m_ActivityTextBg);

	CCSprite* pActtivityTitle = CCSprite::create(TP_TASK_PATH"KessenTaskActivitytext.png"); 
	pActtivityTitle->setPosition(ccp(m_ActivityTextBg->getContentSize().width / 2, m_ActivityTextBg->getContentSize().height / 2));
	m_ActivityTextBg->addChild(pActtivityTitle);

	CCScale9Sprite* pChangeTaskNormal = CCScale9Sprite::create(TP_TASK_PATH"kessen_task_and_activity_change_task_btn.png", CCRectMake(7, 20, 2, 1));
	pChangeTaskNormal->setPreferredSize(CCSizeMake(234, 41));
	CCScale9Sprite* pChangeTaskSelected = CCScale9Sprite::create(TP_TASK_PATH"kessen_task_and_activity_change_task_btn.png", CCRectMake(7, 20, 2, 1));
	pChangeTaskSelected->setPreferredSize(CCSizeMake(234, 41));
	pChangeTaskSelected->setColor(ColorLevelDiv2(pChangeTaskSelected->getColor()));

	m_ChangeTaskBtn = ODE_NEW ODUIButton(pChangeTaskNormal, pChangeTaskSelected);
	m_ChangeTaskBtn->setPositionY(185.0f);
	AddChild(m_ChangeTaskBtn, 3);
	UILinkDelegate(m_ChangeTaskBtn, OnChangeTask);
	m_ChangeTaskBtn->SetVisible(false);

	CCSprite* pChangeTaskIcon = CCSprite::create(TP_TASK_PATH"KessenGameTaskGetGiftIcon_0.png");
	pChangeTaskIcon->setPositionX(-90.0f);
	m_ChangeTaskBtn->addChild(pChangeTaskIcon, 3);

	CCLabelTTF* pChangeTaskCoin = CCLabelTTF::create("500", TTF_NAME, 24);
	pChangeTaskCoin->setAnchorPoint(ccp(0, 0.5f));
	pChangeTaskCoin->setHorizontalAlignment(kCCTextAlignmentLeft);
	pChangeTaskCoin->setPositionX(-60.0f);
	m_ChangeTaskBtn->addChild(pChangeTaskCoin, 3);

	CCLabelTTF* pChangeTaskText = CCLabelTTF::create(TT("UIText_Task_Change_Task"), TTF_NAME, 24);
	pChangeTaskText->setAnchorPoint(ccp(0.5f, 0.5f));
	pChangeTaskText->setHorizontalAlignment(kCCTextAlignmentCenter);
	pChangeTaskText->setPositionY(2.0f);
	pChangeTaskText->setPositionX(50.0f);
	m_ChangeTaskBtn->addChild(pChangeTaskText, 3);

	m_TaskName = CCLabelTTF::create("", TTF_NAME, 26);
	m_TaskName->setAnchorPoint(ccp(0.5f, 0.5f));
	m_TaskName->setHorizontalAlignment(kCCTextAlignmentCenter);
	m_TaskName->setColor(ccc3(195, 161, 123));
	//m_TaskName->setPosition(ccp(-110.0f, 130.0f));
	m_TaskName->setPosition(ccp(0, 135.0f));
	addChild(m_TaskName);

	m_TaskGetGift = CCLabelTTF::create("", TTF_NAME, 26, CCSizeMake(230, 140), kCCTextAlignmentLeft);
	m_TaskGetGift->setColor(ccc3(203, 168, 47));
	m_TaskGetGift->setPositionY(35);
	addChild(m_TaskGetGift);

	m_ProgressLabel = CCLabelTTF::create(TT("UIText_Task_Progress"), TTF_NAME, 24);
	m_ProgressLabel->setColor(ccc3(203, 168, 47));
	m_ProgressLabel->setAnchorPoint(ccp(0.0f, 0.5f));
	m_ProgressLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
	m_ProgressLabel->setPosition(ccp(-110.0f, -60.0f));
	addChild(m_ProgressLabel);

	CCLabelTTF* pRoundLabel = CCLabelTTF::create("", TTF_NAME, 24);
	pRoundLabel->setColor(ccc3(203, 168, 47));
	pRoundLabel->setAnchorPoint(ccp(0.0f, 0.5f));
	pRoundLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
	pRoundLabel->setPosition(ccp(70.0f, 14));
	m_ProgressLabel->addChild(pRoundLabel, 1, 10);

	m_ProgressBg = CCScale9Sprite::create(TP_TASK_PATH"kessen_task_and_activity_progress_bg.png");//, CCRectMake(8, 33, 2, 2));
	m_ProgressBg->setPreferredSize(CCSizeMake(200, 68));
	m_ProgressBg->setPositionY(-120.0f);
	addChild(m_ProgressBg);

	CCProgressTimer* pProgressFront = CCProgressTimer::create(CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage(TP_TASK_PATH"kessenGameTaskActivityProgressFront.png")));
	pProgressFront->setAnchorPoint(ccp(0.0f, 0.5f));
	pProgressFront->setPosition(ccp(0, m_ProgressBg->getContentSize().height / 2));
	pProgressFront->setType(kCCProgressTimerTypeBar);
	pProgressFront->setPercentage(100);
	pProgressFront->setMidpoint(ccp(0, 0));
	pProgressFront->setBarChangeRate(ccp(1, 0));
	m_ProgressBg->addChild(pProgressFront, 1, 10);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"kessenGameTaskActivityProgressFront.png");
	pProgressFront->runAction(CCProgressTo::create(0.0f, 0.0f));

	CCLabelTTF* pProgressLabel = CCLabelTTF::create("", TTF_NAME, 30);
	pProgressLabel->setAnchorPoint(ccp(0.5f, 0.5f));
	pProgressLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
	pProgressLabel->setPosition(ccp(m_ProgressBg->getContentSize().width / 2, m_ProgressBg->getContentSize().height / 2));
	m_ProgressBg->addChild(pProgressLabel, 2, 20);

	CCScale9Sprite* pGetGiftBtnNormal = CCScale9Sprite::create(TP_TASK_PATH"kessen_task_and_activity_get_gift_btn.png");//, CCRectMake(8, 33, 2, 2));
	pGetGiftBtnNormal->setPreferredSize(CCSizeMake(200, 68));
	CCScale9Sprite* pGetGiftBtnSelected = CCScale9Sprite::create(TP_TASK_PATH"kessen_task_and_activity_get_gift_btn.png");//, CCRectMake(8, 33, 2, 2));
	pGetGiftBtnSelected->setPreferredSize(CCSizeMake(200, 68));
	pGetGiftBtnSelected->setColor(ColorLevelDiv2(pGetGiftBtnSelected->getColor()));

	m_GetGiftBtn = ODE_NEW ODUIButton(pGetGiftBtnNormal, pGetGiftBtnSelected);
	m_GetGiftBtn->setPositionY(-120.0f);
	AddChild(m_GetGiftBtn, 3);
	UILinkDelegate(m_GetGiftBtn, OnGetGift);
	m_GetGiftBtn->SetVisible(false);

	CCLabelTTF* pGetGiftText = CCLabelTTF::create(TT("UIText_Task_Get_Gift"), TTF_NAME, 30);
	pGetGiftText->setAnchorPoint(ccp(0.5f, 0.5f));
	pGetGiftText->setHorizontalAlignment(kCCTextAlignmentCenter);
	pGetGiftText->setPositionY(3);
	m_GetGiftBtn->addChild(pGetGiftText, 3);

	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"KessenTaskActivitytextBG.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"KessenTaskWidgetBg.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"KessenTaskActivitytext.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"kessen_task_and_activity_change_task_btn.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"KessenGameTaskGetGiftIcon_0.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"kessen_task_and_activity_progress_bg.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"kessenGameTaskActivityProgressFront.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"kessen_task_and_activity_get_gift_btn.png");
}

GuanDanTaskAndActivityWidget::~GuanDanTaskAndActivityWidget()
{

}

void GuanDanTaskAndActivityWidget::initAllTaskAndActivity()
{
	m_enumType = TP_TASK_OR_ACTIVTTY_MAX;
	m_activityEnumType = TP_ACTVIITY_ENUM_MAX;
	m_ShowDetailInfoBtn->SetVisible(false);
	m_ChangeTaskBtn->SetVisible(false);
	m_ProgressBg->setVisible(false);
	m_GetGiftBtn->SetVisible(false);
	m_ActivityTextBg->setVisible(false);
	m_ProgressLabel->setVisible(false);
	setTaskName("");
	setTaskGetGift("");
	m_currentProgress = 0;
	m_totalProgress = 0;
	m_taskCloseLabel->setVisible(true);
}

void GuanDanTaskAndActivityWidget::setEnumType(GuanDanTaskOrActivityEnum type)
{
	m_enumType = type;

	m_ShowDetailInfoBtn->SetVisible(true);
	switch (m_enumType)
	{
	case TP_TASK_OR_ACTIVITY_FOR_TASK:
		{
			m_ChangeTaskBtn->SetVisible(true);
		}
		break;
	case TP_TASK_OR_ACTIVTTY_FOR_ACTIVITY:
		{
			m_ActivityTextBg->setVisible(true);
		}
		break;
	}
}

GuanDanTaskOrActivityEnum GuanDanTaskAndActivityWidget::getEnumType()
{
	return m_enumType;
}

void GuanDanTaskAndActivityWidget::setActivityEnumType(GuanDanActivityEnum type)
{
	m_taskCloseLabel->setVisible(false);
	m_activityEnumType = type;
}

GuanDanActivityEnum GuanDanTaskAndActivityWidget::getActivityEnumType()
{
	return m_activityEnumType;
}

void GuanDanTaskAndActivityWidget::setTaskName(const char* taskName)
{
	if (taskName != NULL)
	{
		m_TaskName->setString(taskName);
	}
}

void GuanDanTaskAndActivityWidget::setTaskGetGift(const char* getGift)
{
	if (m_TaskGetGift != NULL)
	{
		m_TaskGetGift->setString(getGift);
	}
}

void GuanDanTaskAndActivityWidget::setUnitProgress(int progress)
{
	m_unitProgress = progress;
}

void GuanDanTaskAndActivityWidget::setProgress(int currentProgress, int totalProgress)
{
	m_ProgressLabel->setVisible(true);
	m_ProgressBg->setVisible(true);
	CCLabelTTF* pRoundLabel = (CCLabelTTF*)m_ProgressLabel->getChildByTag(10);

	m_currentProgress = currentProgress;
	m_totalProgress = totalProgress;
	if (m_unitProgress == m_totalProgress)
	{
		pRoundLabel->setString("");
	}
	else
	{
		char roundBuffer[10] = {0};
		int totalRound = m_totalProgress / m_unitProgress;
		int currRound = m_currentProgress / m_unitProgress;
		sprintf(roundBuffer, "(%d/%d)", currRound, totalRound);
		pRoundLabel->setString(roundBuffer);

		switch (m_enumType)
		{
		case TP_TASK_OR_ACTIVITY_FOR_TASK:
			{

			}
			break;
		case TP_TASK_OR_ACTIVTTY_FOR_ACTIVITY:
			{
				switch (m_activityEnumType)
				{
				case TP_ACTVIITY_ENUM_WIN_5_IN_10:
					{

					}
					break;
				case TP_ACTVIITY_ENUM_PLAY_10_MINUTE:
					{
						//LandlordScoreExchangeGiftInfoLayer* infolayer = (LandlordScoreExchangeGiftInfoLayer*)g_pLayerManager->GetLayer("landlord_score_exchange_gift_info_Layer");
						//if (infolayer != NULL)
						//{
						//	infolayer->updateInfo(totalRound, totalRound);
						//}
					}
					break;
				}
			}
			break;
		}
	}

	updateProgress(m_currentProgress % m_unitProgress, m_unitProgress);
}

void GuanDanTaskAndActivityWidget::setProgress(int currentProgress)
{
	setProgress(currentProgress, m_totalProgress);
}

void GuanDanTaskAndActivityWidget::updateProgress(int currentProgress, int totalProgress)
{
	CCProgressTimer* pProgressFront = (CCProgressTimer*)m_ProgressBg->getChildByTag(10);
	CCLabelTTF* pProgressLabel = (CCLabelTTF*)m_ProgressBg->getChildByTag(20);
	if (pProgressFront != NULL)
	{
		if (m_currentProgress >= m_totalProgress)
		{
			pProgressFront->runAction(CCProgressTo::create(0.1f, 100.0f));
			char progressBuffer[20] = {0};
			sprintf(progressBuffer, "%d/%d", totalProgress, totalProgress);
			pProgressLabel->setString(progressBuffer);
		}
		else
		{
			float preProgress = pProgressFront->getPercentage();
			float currProgress = (float)currentProgress / totalProgress;

			pProgressFront->stopAllActions();
			if (currProgress * 100 < preProgress)
			{
				CCAction * action = CCSequence::createWithTwoActions(CCProgressTo::create(0.05f, 100.0f), CCProgressFromTo::create(0.05f, 0.0f, currProgress * 100.0f));
				pProgressFront->runAction(action);
			}
			else if (currProgress * 100 >= preProgress)
			{
				pProgressFront->runAction(CCProgressTo::create(0.1f, currProgress * 100.0f));
			}

			if (pProgressLabel != NULL)
			{
				char progressBuffer[20] = {0};
				sprintf(progressBuffer, "%d/%d", currentProgress, totalProgress);
				pProgressLabel->setString(progressBuffer);
			}
		}	
	}
}

UIImplementDelegate(GuanDanTaskAndActivityWidget, OnChangeTask, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		CCLog("FUNCTION [%s] LINE [%d]", __FUNCTION__, __LINE__);
		//m_currentProgress += 10;

		//if (m_currentProgress <= m_totalProgress)
		//{
		//	updateProgress(m_currentProgress, m_totalProgress);
		//}

		//if (m_currentProgress >= m_totalProgress)
		//{
		//	m_GetGiftBtn->SetVisible(true);
		//	m_ProgressBg->setVisible(false);
		//}
	} 
}

UIImplementDelegate(GuanDanTaskAndActivityWidget, OnGetGift, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		CCLog("FUNCTION [%s] LINE [%d]", __FUNCTION__, __LINE__);
		//m_currentProgress = 0;
		//m_ProgressBg->setVisible(true);

		//m_GetGiftBtn->SetVisible(false);

		//updateProgress(m_currentProgress, m_totalProgress);
	} 
}

UIImplementDelegate(GuanDanTaskAndActivityWidget, OnShowDetailInfo, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		CCLog("FUNCTION [%s] LINE [%d]", __FUNCTION__, __LINE__);
		switch (m_enumType)
		{
		case TP_TASK_OR_ACTIVITY_FOR_TASK:
			{

			}
			break;
		case TP_TASK_OR_ACTIVTTY_FOR_ACTIVITY:
			{
				switch (m_activityEnumType)
				{
				case TP_ACTVIITY_ENUM_WIN_5_IN_10:
					{

					}
					break;
				case TP_ACTVIITY_ENUM_PLAY_10_MINUTE:
					{
						if (g_pLayerManager->GetLayer("sg_score_exchange_gift_info_Layer") == NULL)
						{
							g_pLayerManager->AddLayer(GuanDanScoreExchangeGiftInfoLayer::create());
						}
						g_pLayerManager->PushLayer("sg_score_exchange_gift_info_Layer");
					}
					break;
				}
			}
			break;
		}
	} 
}

GuanDanTaskAndActivityLayer::GuanDanTaskAndActivityLayer() : ODUILayerModalExt("guandan_task")
{

}

GuanDanTaskAndActivityLayer::~GuanDanTaskAndActivityLayer()
{
	term();
}

bool GuanDanTaskAndActivityLayer::init()
{

	UIInitDelegate(GuanDanTaskAndActivityLayer, OnClose);
	m_Panel = CCScale9Sprite::create(TP_TASK_PATH"taskBg.png");
	m_Panel->setPreferredSize (CCSizeMake (1080.0f, 490.0f));
	m_Panel->setScale(ui_main_scale);
	addChild(m_Panel);
	m_Panel->setPosition(ui_ccp(0.5f, 0.5f));

	CCSprite* pTitle = CCSprite::create(TP_TASK_PATH"KessenGameTaskActivityTitle.png");
	pTitle->setPosition(ccp(m_Panel->getContentSize().width / 2, m_Panel->getContentSize().height - pTitle->getContentSize().height));
	m_Panel->addChild(pTitle);
	/*
	CCSprite* pkCloseBtnFrame = CCSprite::create("closebtn_frame.png");
	pkCloseBtnFrame->setPosition(ccp(532.0f, 233.5f));
	m_Panel->addChild(pkCloseBtnFrame, 1);*/

	CCSprite* pkCloseBtnNormal = CCSprite::create(TP_IMAGE_PATH_GAMEBANK"DZ-04.png");
	CCSprite* pkCloseBtnPress = CCSprite::create(TP_IMAGE_PATH_GAMEBANK"DZ-04.png");
	pkCloseBtnPress->setColor(ColorLevelDiv2(pkCloseBtnPress->getColor()));

	ODUIButton* pkClose = ODE_NEW ODUIButton(pkCloseBtnNormal, pkCloseBtnPress);
	pkClose->setPosition(ccp(ODEFloorf(m_Panel->getPositionX() + 512 * ui_main_scale), ODEFloorf(m_Panel->getPositionY() + 213.5 * ui_main_scale)));
	pkClose->setContentSize(CCSize(pkClose->getContentSize().width * 2, pkClose->getContentSize().height * 2));
	pkClose->setScale(ui_main_scale);
	AddWidget("btn_close", pkClose);
	UILinkDelegate(pkClose, OnClose);

	CCScale9Sprite* m_pkBgFrame = CCScale9Sprite::create(TP_IMAGE_PATH_GAMEBANK"bg_text.png");//,CCRect(0,0,6,6),CCRect(2,2,2,2));

	m_pkBgFrame->setPreferredSize (CCSizeMake (1058.0f, 422.0f));
	m_pkBgFrame->setPosition(m_Panel->getContentSize().width / 2, 12 + m_pkBgFrame->getContentSize().height / 2);
	m_Panel->addChild(m_pkBgFrame, 2);

	GuanDanTaskAndActivityWidget* pTaskWidget_0 = ODE_NEW GuanDanTaskAndActivityWidget;
	pTaskWidget_0->setPosition(CCSizeMake(m_Panel->getPositionX() - 375 * ui_width_scale, m_Panel->getPositionY() - 45 * ui_width_scale));
	pTaskWidget_0->setScale(ui_main_scale);
	AddWidget("task_widget_0", pTaskWidget_0);

	GuanDanTaskAndActivityWidget* pTaskWidget_1 = ODE_NEW GuanDanTaskAndActivityWidget;
	pTaskWidget_1->setPosition(CCSizeMake(m_Panel->getPositionX() - 125 * ui_width_scale, m_Panel->getPositionY() - 45 * ui_width_scale));
	pTaskWidget_1->setScale(ui_main_scale);
	AddWidget("task_widget_1", pTaskWidget_1);
	
	CCSprite* pCenterLine = CCSprite::create(TP_TASK_PATH"kessenchartscutoffrule.png");
	pCenterLine->setScaleY(108 * ui_main_scale);
	pCenterLine->setPosition(CCPointMake(m_Panel->getPositionX(), m_Panel->getPositionY() - 45 * ui_width_scale));
	addChild(pCenterLine, 10);
	
	
	GuanDanTaskAndActivityWidget* pActivityWidget_0 = ODE_NEW GuanDanTaskAndActivityWidget;
	pActivityWidget_0->setPosition(CCSizeMake(m_Panel->getPositionX() + 125 * ui_width_scale, m_Panel->getPositionY() - 45 * ui_width_scale));
	pActivityWidget_0->setScale(ui_main_scale);
	AddWidget("activity_widget_0", pActivityWidget_0);

	GuanDanTaskAndActivityWidget* pActivityWidget_1 = ODE_NEW GuanDanTaskAndActivityWidget;
	pActivityWidget_1->setPosition(CCSizeMake(m_Panel->getPositionX() + 375 * ui_width_scale, m_Panel->getPositionY() - 45 * ui_width_scale));
	pActivityWidget_1->setScale(ui_main_scale);
	AddWidget("activity_widget_1", pActivityWidget_1);

	removeTextureManual();

	return ODUILayerModalExt<GuanDanTaskAndActivityLayer>::init();
}

void GuanDanTaskAndActivityLayer::removeTextureManual()
{
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"taskBg.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"kessenchartscutoffrule.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"KessenGameTaskActivityTitle.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_GAMEBANK"DZ-01.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_GAMEBANK"DZ-04.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_IMAGE_PATH_GAMEBANK"bg_text.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"kessen_task_and_activity_get_gift_btn.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"kessen_task_and_activity_progress_bg.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"KessenGameTaskGetGiftIcon_0.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"kessen_task_and_activity_change_task_btn.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"KessenTaskActivitytext.png.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"KessenTaskActivitytextBG.png");
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"KessenTaskWidgetBg.png");

}

void GuanDanTaskAndActivityLayer::OnActive( const ODUIUserDataPtr& spData )
{
	stopAllActions();
	setScale(0.5f);
	this->runAction(CCEaseBackInOut::create(CCScaleTo::create(0.3f, 1.0f, 1.0f)));
}

void GuanDanTaskAndActivityLayer::OnDeactive( const ODUIUserDataPtr& spData )
{
	setScale(0.5f);
}

bool GuanDanTaskAndActivityLayer::OnBack()
{
	return true;
}

void GuanDanTaskAndActivityLayer::term()
{
	ODUILayer::term();
}

void GuanDanTaskAndActivityLayer::initAllTaskAndActivity()
{
	WIDGET(GuanDanTaskAndActivityWidget, task_widget_0);
	task_widget_0.initAllTaskAndActivity();
	//task_widget_0.setEnumType(TP_TASK_OR_ACTIVITY_FOR_TASK);
	WIDGET(GuanDanTaskAndActivityWidget, task_widget_1);
	task_widget_1.initAllTaskAndActivity();
	//task_widget_1.setEnumType(TP_TASK_OR_ACTIVITY_FOR_TASK);

	WIDGET(GuanDanTaskAndActivityWidget, activity_widget_0);
	activity_widget_0.initAllTaskAndActivity();
	activity_widget_0.setEnumType(TP_TASK_OR_ACTIVTTY_FOR_ACTIVITY);
	WIDGET(GuanDanTaskAndActivityWidget, activity_widget_1);
	activity_widget_1.initAllTaskAndActivity();
	activity_widget_1.setEnumType(TP_TASK_OR_ACTIVTTY_FOR_ACTIVITY);
}

void GuanDanTaskAndActivityLayer::addTask(GuanDanTaskOrActivityEnum enumType)
{
	WIDGET(GuanDanTaskAndActivityWidget, task_widget_0);
	WIDGET(GuanDanTaskAndActivityWidget, task_widget_1);

}

void GuanDanTaskAndActivityLayer::addActivity(GuanDanTaskOrActivityEnum enumType, GuanDanActivityEnum activityType)
{
	WIDGET(GuanDanTaskAndActivityWidget, activity_widget_0);
	WIDGET(GuanDanTaskAndActivityWidget, activity_widget_1);
	if (enumType == TP_TASK_OR_ACTIVTTY_FOR_ACTIVITY)
	{
		if (activity_widget_0.getActivityEnumType() == TP_ACTVIITY_ENUM_MAX)
		{
			activity_widget_0.setActivityEnumType(activityType);
		}
		else if (activity_widget_0.getActivityEnumType() == activityType)
		{
			CCLog("FUNCTION [%s] LINE [%d] ACTIVITY [%d] IS ALREADY EXISED !", __FUNCTION__, __LINE__, activityType);
		}
		else if (activity_widget_1.getActivityEnumType() == TP_ACTVIITY_ENUM_MAX)
		{
			activity_widget_1.setActivityEnumType(activityType);
		}
		else if (activity_widget_1.getActivityEnumType() == activityType)
		{
			CCLog("FUNCTION [%s] LINE [%d] ACTIVITY [%d] IS ALREADY EXISED !", __FUNCTION__, __LINE__, activityType);
		}
		else
		{
			CCLog("FUNCTION [%s] LINE [%d] ACTIVITIES R FULL !", __FUNCTION__, __LINE__);
		}
	}
	else
	{
		CCLog("FUNCTION [%s] LINE [%d] ACTIVITY TYPE [%d] ERROR ", __FUNCTION__, __LINE__, activityType);
	}
}

GuanDanTaskAndActivityWidget* GuanDanTaskAndActivityLayer::getActivity(GuanDanActivityEnum activityType)
{
	WIDGET(GuanDanTaskAndActivityWidget, activity_widget_0);
	WIDGET(GuanDanTaskAndActivityWidget, activity_widget_1);

	if (activity_widget_0.getActivityEnumType() == activityType)
	{
		return &activity_widget_0;
	}
	else if (activity_widget_1.getActivityEnumType() == activityType)
	{
		return &activity_widget_1;
	}
	else
	{
		return NULL;
	}
}

UIImplementDelegate(GuanDanTaskAndActivityLayer, OnClose, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		if (g_pkGuanDanData->GetIsNeedGuideInGD())
			g_pLayerManager->PopLayer ();
		else
			g_pLayerManager->PopAllLayers ();
	}
}


#ifdef SCORE_EXCHANGE_GIFT_MACRO

GuanDanSgScoreExchangeGiftWidget::GuanDanSgScoreExchangeGiftWidget() :
	m_baseTimeUnit(600)
	, m_totalTime(0)
{
	//setScale(ui_main_scale);
	CCSize clickSize = CCSizeMake(186, 70);
	setContentSize(clickSize);
	UIInitDelegate(GuanDanSgScoreExchangeGiftWidget, OnShowTip);


	//CCRect rect = CCRectMake(0, 0, 26, 22);
	//CCRect rectInserts = CCRectMake(10, 9, 6, 1);

	CCSprite * icon = CCSprite::create(TP_TASK_PATH"scoreexchangeIcon.png");
	icon->setAnchorPoint(ccp(0.5f, 0.5f));
	icon->setPosition(ccp(-getContentSize().width / 2 + icon->getContentSize().width / 2 + 6, 0));
	addChild(icon);

	CCScale9Sprite* jindubg = CCScale9Sprite::create(TP_TASK_PATH"scoreexchangegift_2.png");//CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey("image/scoreexchangegift_2.png"));
	jindubg->setContentSize(CCSizeMake(125.0f, 32.0f));
	jindubg->setAnchorPoint(ccp(0.5f, 0.5f));
	jindubg->setPosition(ccp(getContentSize().width / 2 - jindubg->getContentSize().width / 2 -8, icon->getPositionY()));
	addChild(jindubg);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"scoreexchangegift_2.png");

	CCTextureCache::sharedTextureCache()->addImage(TP_TASK_PATH"scoreexchangegift_3.png");
	m_jinduFront = CCProgressTimer::create(CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(TP_TASK_PATH"scoreexchangegift_3.png")));
	m_jinduFront->setAnchorPoint(ccp(0.5f, 0.5f));
	m_jinduFront->setPosition(jindubg->getPosition());
	m_jinduFront->setType(kCCProgressTimerTypeBar);
	m_jinduFront->setPercentage(100);
	m_jinduFront->setMidpoint(ccp(0, 0));
	m_jinduFront->setBarChangeRate(ccp(1, 0));
	addChild(m_jinduFront);
	m_jinduFront->runAction(CCProgressTo::create(0.0f, 0.0f));
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"scoreexchangegift_3.png");

	CCSprite* pkScoreExchangeGiftBtnNormal = CCSprite::create();
	CCSprite* pkScoreExchangeGiftBtnPressed = CCSprite::create();
	ODUIButton* pkScoreExchangeGift = ODE_NEW ODUIButton(pkScoreExchangeGiftBtnNormal, pkScoreExchangeGiftBtnPressed);
	pkScoreExchangeGift->setContentSize(CCSize(clickSize.width * 2, clickSize.height * 2));
	pkScoreExchangeGift->setPosition(ccp(0,0));
	pkScoreExchangeGift->setScale(ui_main_scale * 1.2f);
	AddChild(pkScoreExchangeGift, 3);
	UILinkDelegate(pkScoreExchangeGift, OnShowTip);

	//setContentSize(clickSize);
}

GuanDanSgScoreExchangeGiftWidget::~GuanDanSgScoreExchangeGiftWidget()
{

}

void GuanDanSgScoreExchangeGiftWidget::setBaseTimeUnit(int time)
{
	m_baseTimeUnit = time;
}

void GuanDanSgScoreExchangeGiftWidget::updateInfo(int currTime)
{
	int totalRound = m_totalTime / m_baseTimeUnit;
	float currProgress = 0.0f;

	GuanDanScoreExchangeGiftInfoLayer* infolayer = (GuanDanScoreExchangeGiftInfoLayer*)g_pLayerManager->GetLayer("sg_score_exchange_gift_info_Layer");
	if (currTime < m_totalTime)
	{
		currProgress = currTime % m_baseTimeUnit / (float)m_baseTimeUnit;
		int currRound = currTime / m_baseTimeUnit;
		infolayer->updateInfo(currRound, totalRound);
	}
	else
	{
		currProgress = 1.0f;
		infolayer->updateInfo(totalRound, totalRound);
	}

	if (m_jinduFront->numberOfRunningActions() > 0)
	{
		m_jinduFront->stopAllActions();
	}
	float preProgress = m_jinduFront->getPercentage();
	if (preProgress < currProgress * 100.0f)
	{
		m_jinduFront->runAction(CCProgressTo::create(0.1f, currProgress * 100.0f));
	}
	else if (preProgress > currProgress * 100.0f)
	{
		CCAction * action = CCSequence::createWithTwoActions(CCProgressTo::create(0.05f, 100.0f), CCProgressFromTo::create(0.05f, 0.0f, currProgress * 100.0f));
		m_jinduFront->runAction(action);
	}
}

void GuanDanSgScoreExchangeGiftWidget::updateInfo(int currTime, int totalTime)
{
	m_totalTime = totalTime;

	updateInfo(currTime);
}

void GuanDanSgScoreExchangeGiftWidget::setTitle(const char* title)
{
	//m_titleLabel->setString(ODETools::GBKToUTF8(title).c_str());
}

UIImplementDelegate(GuanDanSgScoreExchangeGiftWidget, OnShowTip, kWidget, u32State)
{
	if (u32State == ODUIWidget::RELEASED)
	{
		//g_pLayerManager->AddLayer(GuanDanScoreExchangeGiftInfoLayer::create());
		//g_pLayerManager->PushLayer("sg_score_exchange_gift_info_Layer");
		if (g_pLayerManager->GetLayer("guandan_task") == NULL)
		{
			g_pLayerManager->AddLayer(GuanDanTaskAndActivityLayer::create());
		}
		g_pLayerManager->PushLayer("guandan_task");
	}
}

GuanDanScoreExchangeGiftInfoLayer::GuanDanScoreExchangeGiftInfoLayer() : ODUILayerModalExt("sg_score_exchange_gift_info_Layer")
	, m_currRound(0)
	, m_totalRound(0)
{

}

GuanDanScoreExchangeGiftInfoLayer::~GuanDanScoreExchangeGiftInfoLayer()
{
	term();
}

bool GuanDanScoreExchangeGiftInfoLayer::init()
{
	setScale(ui_main_scale);

	//CCRect rect = CCRectMake(0, 0, 42, 42);
	//CCRect rectInserts = CCRectMake(20, 20, 2, 2);
	CCScale9Sprite* layerbg = CCScale9Sprite::create(TP_TASK_PATH"scoreexchangegift_1.png");
	layerbg->setContentSize(CCSizeMake(522.0f, 160));
	layerbg->setPosition(ui_ccp(0.5f, 0.55f));
	layerbg->setTag(100);
	addChild(layerbg);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"scoreexchangegift_1.png");

	CCScale9Sprite* pScoreExchangeGiftBg = CCScale9Sprite::create(TP_TASK_PATH"scoreexchangegift_4.png");
	pScoreExchangeGiftBg->setPreferredSize(CCSize(500, 136));
	pScoreExchangeGiftBg->setPosition(ccp(layerbg->getContentSize().width / 2, layerbg->getContentSize().height / 2 - 2));
	layerbg->addChild(pScoreExchangeGiftBg);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TP_TASK_PATH"scoreexchangegift_4.png");

	info1 = CCLabelTTF::create("", TTF_NAME, 25.0f, CCSizeMake(522.0f - 30.0f, 160 - 30.0f), kCCTextAlignmentLeft);
	info1->setColor(ccc3(222, 222, 222));
	info1->setAnchorPoint(ccp(0.0f, 1.0f));
	info1->setPosition(ccp(15.0f, 142.0f));
	layerbg->addChild(info1);

	info2 = CCLabelTTF::create("", TTF_NAME, 25.0f, CCSizeMake(522.0f - 30.0f, 160 - 30.0f), kCCTextAlignmentLeft);
	info2->setColor(ccc3(222, 222, 222));
	info2->setAnchorPoint(ccp(0.0f, 1.0f));
	info2->setPosition(ccp(15.0f, 82.0f));
	layerbg->addChild(info2);

	info3 = CCLabelTTF::create("", TTF_NAME, 25.0f, CCSizeMake(522.0f - 30.0f, 160 - 30.0f), kCCTextAlignmentLeft);
	info3->setColor(ccc3(234, 184, 3));
	info3->setAnchorPoint(ccp(0.0f, 1.0f));
	info3->setPosition(ccp(15.0f, 52.0f));
	layerbg->addChild(info3);

	return ODUILayerModalExt<GuanDanScoreExchangeGiftInfoLayer>::init();
}

void GuanDanScoreExchangeGiftInfoLayer::OnActive( const ODUIUserDataPtr& spData )
{
	info1->setString(GetConfig("ScoreExchangeGiftInfo0"));

	char buffer[256] = {0};
	sprintf(buffer, GetConfig("ScoreExchangeGiftInfo1"), m_currRound, m_totalRound);
	info2->setString(buffer);

	info3->setString(GetConfig("ScoreExchangeGiftInfo2"));

	CCScale9Sprite* layerbg = (CCScale9Sprite*)getChildByTag(100);
	if (layerbg != NULL)
	{
		layerbg->stopAllActions();
		layerbg->setOpacity(0);
		layerbg->setScale(2.0f);

		layerbg->runAction(CCSpawn::create(CCFadeTo::create(0.5f, 255), CCScaleTo::create(0.5f, 1.0f), NULL));
	}

}

void GuanDanScoreExchangeGiftInfoLayer::OnDeactive( const ODUIUserDataPtr& spData )
{

}

bool GuanDanScoreExchangeGiftInfoLayer::OnBack()
{
	return true;
}

void GuanDanScoreExchangeGiftInfoLayer::term()
{
	ODUILayer::term();
}

bool GuanDanScoreExchangeGiftInfoLayer::ProcessTouch( TouchType eType, CCTouch* pkTouch )
{
	if (!ODUILayer::ProcessTouch(eType, pkTouch))
	{
		CCPoint point = pkTouch->getLocation();

		if (eType == TOUCH_ENDED)
		{
			g_pLayerManager->Back();
		}
	}

	return true;
}

void GuanDanScoreExchangeGiftInfoLayer::updateInfo(int curr, int total)
{
	m_currRound = curr;
	m_totalRound = total;

	char buffer[256] = {0};
	sprintf(buffer, GetConfig("ScoreExchangeGiftInfo1"), m_currRound, m_totalRound);
	info2->setString(buffer);
}
#endif

GuanDanGuideTipsTool* GuanDanGuideTipsTool::create (int lineDir, int circleDir, const char * str)
{
	GuanDanGuideTipsTool* pReturn = new GuanDanGuideTipsTool(lineDir, circleDir, str);
	if ( pReturn)
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

GuanDanGuideTipsTool::GuanDanGuideTipsTool (int lineDir, int circleDir, const  char * str)
{
	m_lineDirection = lineDir;
	m_circleDirection = circleDir;
	
	m_guidTTF = CCLabelTTF::create (str, TTF_NAME, 24);
	m_guidTTF->setColor (ccc3 (0x00, 0x00, 0x00));

	m_circleNode = CCScale9Sprite::create (TP_IMAGE_PATH_GUIDE"tipsBack.png", CCRect (0, 0, 18, 18), CCRect (8, 8, 2,2));
	m_circleNode->setPreferredSize (CCSizeMake (m_guidTTF->getContentSize ().width + 30, m_guidTTF->getContentSize ().height + 8));
	m_guidTTF->setPosition (ccp (m_circleNode->getContentSize ().width / 2, m_circleNode->getContentSize ().height / 2));
	m_circleNode->addChild (m_guidTTF);

	if (m_lineDirection == 0){
		m_lineNode = CCSprite::create (TP_IMAGE_PATH_GUIDE"tipsLines.png");
		m_circleNode->setAnchorPoint (ccp (0.5f, 1.0f));
		m_circleNode->setPosition (ccp (m_lineNode->boundingBox ().getMidX (), m_lineNode->boundingBox ().getMinY ()));
	}else if (m_lineDirection == 1){
		m_lineNode = CCSprite::create (TP_IMAGE_PATH_GUIDE"tipsLines1.png");
		m_circleNode->setAnchorPoint (ccp (0.5f, 0.0));
		m_circleNode->setPosition (ccp (m_lineNode->boundingBox ().getMidX (), m_lineNode->boundingBox ().getMaxY ()));
	}else if (m_lineDirection == 2){
		m_lineNode = CCSprite::create (TP_IMAGE_PATH_GUIDE"tipsLines2.png");
		m_circleNode->setAnchorPoint (ccp (1.0f, 0.5f));
		m_circleNode->setPosition (ccp (m_lineNode->boundingBox ().getMinX (), m_lineNode->boundingBox ().getMidY ()));
	}else if (m_lineDirection == 3){
		m_lineNode = CCSprite::create (TP_IMAGE_PATH_GUIDE"tipsLines3.png");
		m_circleNode->setAnchorPoint (ccp (0.0f, 0.5f));
		m_circleNode->setPosition (ccp (m_lineNode->boundingBox ().getMaxX (), m_lineNode->boundingBox ().getMidY ()));
	}else{
		assert (0);
	}
	addChild (m_circleNode);
	addChild (m_lineNode);
}

void GuanDanGuideTipsTool::setLineDirection (int dir)
{
	m_lineDirection= dir;
}

void GuanDanGuideTipsTool::setCircleDirection (int dir)
{
	m_circleDirection = dir;
}
