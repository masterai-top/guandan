#include "GuanDanChatAnimate.h"

GuanDanChatAnimate::GuanDanChatAnimate()
{

}

GuanDanChatAnimate::~GuanDanChatAnimate()
{

}

void GuanDanChatAnimate::loadPlist()
{
	if (CCTextureCache::sharedTextureCache()->textureForKey("TexasPoker/image/chat_animi.pvr.ccz") == NULL)
	{
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("TexasPoker/image/chat_animi.plist", "TexasPoker/image/chat_animi.pvr.ccz");
	}
}

void GuanDanChatAnimate::unLoadPlist()
{
	if (CCTextureCache::sharedTextureCache()->textureForKey("TexasPoker/image/chat_animi.pvr.ccz") != NULL)
	{
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("TexasPoker/image/chat_animi.plist");
		CCTextureCache::sharedTextureCache()->removeTextureForKey("TexasPoker/image/chat_animi.pvr.ccz");
	}
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getAnimateByIndex(GuanDanChatAnimateEnum animiIndex)
{
	switch (animiIndex)
	{
	case GUANDAN_CHAT_ANIMATE_BU_DONG://不懂
		{
			return getBuDongAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_CHAO_FENG_1://嘲讽1
		{
			return getChaoFeng1Animi();
		}
	case GUANDAN_CHAT_ANIMATE_CHAO_FENG_2://嘲讽2
		{
			return getChaoFeng2Animi();
		}
	case GUANDAN_CHAT_ANIMATE_DA_JING://大惊
		{
			return getDaJingAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_DA_KU://大哭
		{
			return getDaKuAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_DE_YI_1://得意1
		{
			return getDeYi1Animi();
		}
	case GUANDAN_CHAT_ANIMATE_DE_YI_2://得意2
		{
			return getDeYi2Animi();
		}
	case GUANDAN_CHAT_ANIMATE_HAN://汗
		{
			return getHanAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_KAI_XIN://开心
		{
			return getKaiXinAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_KU://哭
		{
			return getKuAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_MENG://萌
		{
			return getMengAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_NI_XI://逆袭
		{
			return getNiXiAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_NU://怒
		{
			return getNuAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_PAO_XIAO://咆哮
		{
			return getPaoXiaoAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_QIN_1://亲1
		{
			return getQin1Animi();
		}
	case GUANDAN_CHAT_ANIMATE_QIN_2://亲2
		{
			return getQin2Animi();
		}
	case GUANDAN_CHAT_ANIMATE_SE://色
		{
			return getSeAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_SHANG_XIN://伤心
		{
			return getShangXinAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_TU://吐
		{
			return getTuAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_XIAO_GONG://小攻
		{
			return getXiaoGongAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_XIAO_JING://小惊
		{
			return getXiaoJingAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_XIAO_SHOU://小受
		{
			return getXiaoShouAnimi();
		}
	case GUANDAN_CHAT_ANIMATE_YUN://晕
		{
			return getYunAnimi();
		}
	default:
		{
			return NULL;
		}
		break;
	}
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getBuDongAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("budong_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("budong_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("budong_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("budong_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("budong_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("budong_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("budong_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("budong_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("budong_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("budong_6.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getChaoFeng1Animi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_9.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_10.png"));

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_9.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_10.png"));

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_11.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_12.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_13.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_14.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_15.png"));

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_16.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_16.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_16.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_16.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_16.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_16.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_16.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_16.png"));

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_15.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_14.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_13.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_12.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_1_11.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 25);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getChaoFeng2Animi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_7.png"));

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_8.png"));

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("chaofeng_2_4.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 25);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getDaJingAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_9.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_10.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_11.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dajing_12.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getDaKuAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_9.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_10.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_11.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_12.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_13.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("daku_14.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getDeYi1Animi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_1_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_1_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_1_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_1_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_1_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_1_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_1_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_1_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_1_9.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_1_10.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getDeYi2Animi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_2_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("deyi_2_2.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getHanAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("han_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("han_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("han_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("han_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("han_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("han_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("han_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("han_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("han_9.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("han_10.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getKaiXinAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("kaixin_3.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getKuAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ku_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ku_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ku_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ku_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ku_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ku_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ku_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ku_6.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getMengAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meng_8.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getNiXiAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nixi_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nixi_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nixi_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nixi_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nixi_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nixi_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nixi_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nixi_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nixi_9.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nixi_10.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getNuAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nu_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nu_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nu_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nu_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nu_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nu_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nu_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nu_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("nu_9.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getPaoXiaoAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("paoxiao_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("paoxiao_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("paoxiao_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("paoxiao_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("paoxiao_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("paoxiao_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("paoxiao_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("paoxiao_8.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getQin1Animi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_1_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_1_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_1_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_1_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_1_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_1_6.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getQin2Animi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qin_2_7.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getSeAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("se_6.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getShangXinAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shangxin_6.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getTuAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_7.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_9.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_9.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_8.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tu_9.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getXiaoGongAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaogong_6.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getXiaoJingAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaojing_6.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getXiaoShouAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xiaoshou_6.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}

GuanDanChatAnimiStruct* GuanDanChatAnimate::getYunAnimi()
{
	GuanDanChatAnimiStruct* animiStruct = new GuanDanChatAnimiStruct;

	CCArray* frameArray = CCArray::create();

	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("yun_1.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("yun_2.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("yun_3.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("yun_4.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("yun_5.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("yun_6.png"));
	frameArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("yun_7.png"));

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, 1.0f / 15);
	CCAnimate* animate = CCAnimate::create(animation);

	animiStruct->animiSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frameArray->objectAtIndex(0));
	animiStruct->animate = animate;
	return animiStruct;
}