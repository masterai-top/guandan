#pragma once 

#include "ODUILayer.h"
#include "Knickknacks.h"
#include "ClientData.h"
#include "SoundSystem.h"
#include "GuanDanGameAssist.h"
#include "GuanDanAssist.h"

class GuanDanAgreeBasePointClock : public GuanDanGameClock
{
public:
	GuanDanAgreeBasePointClock(ODEUInt32 u32ClockTime);
	virtual ~GuanDanAgreeBasePointClock();
	virtual void OnTimerEnd();
};

class GuanDanAgreeBasePointAlert : public ODUILayerModalExt<GuanDanAgreeBasePointAlert>
{
public:
	typedef ODEMemberDelegate<GuanDanAgreeBasePointAlert, ODUIWidget&, ODEUInt32> UIDelegate;

	GuanDanAgreeBasePointAlert();

	virtual ~GuanDanAgreeBasePointAlert();

	virtual bool init();

	virtual void OnActive(const ODUIUserDataPtr& spData);

	virtual void OnDeactive(const ODUIUserDataPtr& spData);

	virtual bool OnBack();

	virtual void term();

	void ClockTimerEnd();

	void SetAgreePointClockVisible( bool bVisible );

	void SetCurrTableInfo(ODEInt32 iTableId, ODEInt64 iBasePoint);

	ODE_CREATE_FUNC(GuanDanAgreeBasePointAlert);

	ODE_SYNTHESIZE(ODEUInt32, m_u32Index, NowIndex);

private:
	ODUIWebView* m_pkWebView;
	GuanDanAgreeBasePointClock* MyAgreeBasePointClock;
	CCLabelTTF *base_point;
	CCLabelTTF* m_pTipText2;

	bool m_bBaseAlertConfirm;

	ODEInt32 m_iTableId;
	ODEInt64 m_iBasePoint;
protected:
	UIDeclareDelegate(OnClose);
	UIDeclareDelegate(OnConfirm);
	UIDeclareDelegate(OnCancel);
};