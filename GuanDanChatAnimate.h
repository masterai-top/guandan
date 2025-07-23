#pragma once

#include "cocos2d.h"
USING_NS_CC;

typedef enum{
	GUANDAN_CHAT_ANIMATE_BU_DONG,					//不懂
	GUANDAN_CHAT_ANIMATE_CHAO_FENG_1,				//嘲讽1
	GUANDAN_CHAT_ANIMATE_CHAO_FENG_2,				//嘲讽2
	GUANDAN_CHAT_ANIMATE_DA_JING,					//大惊
	GUANDAN_CHAT_ANIMATE_DA_KU,						//大哭
	GUANDAN_CHAT_ANIMATE_DE_YI_1,					//得意1
	GUANDAN_CHAT_ANIMATE_DE_YI_2,					//得意2
	GUANDAN_CHAT_ANIMATE_HAN,						//汗
	GUANDAN_CHAT_ANIMATE_KAI_XIN,					//开心
	GUANDAN_CHAT_ANIMATE_KU,						//哭
	GUANDAN_CHAT_ANIMATE_MENG,						//萌
	GUANDAN_CHAT_ANIMATE_NI_XI,						//逆袭
	GUANDAN_CHAT_ANIMATE_NU,						//怒
	GUANDAN_CHAT_ANIMATE_PAO_XIAO,					//咆哮
	GUANDAN_CHAT_ANIMATE_QIN_1,						//亲1
	GUANDAN_CHAT_ANIMATE_QIN_2,						//亲2
	GUANDAN_CHAT_ANIMATE_SE,						//色
	GUANDAN_CHAT_ANIMATE_SHANG_XIN,					//伤心
	GUANDAN_CHAT_ANIMATE_TU,						//吐
	GUANDAN_CHAT_ANIMATE_XIAO_GONG,					//小攻
	GUANDAN_CHAT_ANIMATE_XIAO_JING,					//小惊
	GUANDAN_CHAT_ANIMATE_XIAO_SHOU,					//小受
	GUANDAN_CHAT_ANIMATE_YUN,						//晕
	GUANDAN_CHAT_ANIMATE_MAX						//最大动画个数
} GuanDanChatAnimateEnum;

typedef struct tagGuanDanChatAnimiStruct
{
	CCSprite*  animiSprite;
	CCAnimate* animate;
}GuanDanChatAnimiStruct;


class GuanDanChatAnimate
{
public:

	GuanDanChatAnimate();
	~GuanDanChatAnimate();
	
	//加载动画资源
	static void loadPlist();
	
	//释放动画资源
	static void unLoadPlist();

	//根据枚举值获取对应的动画
	static GuanDanChatAnimiStruct* getAnimateByIndex(GuanDanChatAnimateEnum animiIndex);

private:

	//不懂
	static GuanDanChatAnimiStruct* getBuDongAnimi();

	//嘲讽1
	static GuanDanChatAnimiStruct* getChaoFeng1Animi();

	//嘲讽2
	static GuanDanChatAnimiStruct* getChaoFeng2Animi();

	//大惊
	static GuanDanChatAnimiStruct* getDaJingAnimi();

	//大哭
	static GuanDanChatAnimiStruct* getDaKuAnimi();

	//得意1
	static GuanDanChatAnimiStruct* getDeYi1Animi();

	//得意2
	static GuanDanChatAnimiStruct* getDeYi2Animi();
	
	//汗
	static GuanDanChatAnimiStruct* getHanAnimi();

	//开心
	static GuanDanChatAnimiStruct* getKaiXinAnimi();

	//哭
	static GuanDanChatAnimiStruct* getKuAnimi();

	//萌
	static GuanDanChatAnimiStruct* getMengAnimi();

	//逆袭
	static GuanDanChatAnimiStruct* getNiXiAnimi();

	//怒
	static GuanDanChatAnimiStruct* getNuAnimi();

	//咆哮
	static GuanDanChatAnimiStruct* getPaoXiaoAnimi();

	//亲1
	static GuanDanChatAnimiStruct* getQin1Animi();

	//亲2
	static GuanDanChatAnimiStruct* getQin2Animi();

	//色
	static GuanDanChatAnimiStruct* getSeAnimi();

	//伤心
	static GuanDanChatAnimiStruct* getShangXinAnimi();

	//吐
	static GuanDanChatAnimiStruct* getTuAnimi();

	//小攻
	static GuanDanChatAnimiStruct* getXiaoGongAnimi();

	//小惊
	static GuanDanChatAnimiStruct* getXiaoJingAnimi();

	//小受
	static GuanDanChatAnimiStruct* getXiaoShouAnimi();

	//晕
	static GuanDanChatAnimiStruct* getYunAnimi();

};