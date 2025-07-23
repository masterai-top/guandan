#ifndef KessenConfig_h__
#define KessenConfig_h__
#include "cocos2d.h"
#include <vector>
#include "ODEPCH.h"
#include "ODUILayerManager.h"
#include <map>

#define MOBILE_INFO_LENGTH  50

typedef struct MobileIonfo
{
	char         PhoneConfigVersion[MOBILE_INFO_LENGTH];//配置文件版本号  
	char		 PhoneDouziVersion[MOBILE_INFO_LENGTH];//手机豆子版本  getPackageVersion
	char		 PhoneSdkVersion[MOBILE_INFO_LENGTH];//手机sdk版本  getSdkVersion()
	char		 PhoneDeviceName[MOBILE_INFO_LENGTH];//手机型号 如：设备名称  iPhone OS getDeviceName()
	char		 PhoneFactory[MOBILE_INFO_LENGTH];//手机厂家生产厂家 getFactory()
	char	     PhoneModel[MOBILE_INFO_LENGTH];//手机品牌 getPhoneModel()
	char		 PhoneChannelId[MOBILE_INFO_LENGTH];//手机渠道信息
	char		 PhoneIpAddress[MOBILE_INFO_LENGTH];//本地IPgetLocalIpAddress
	char		 PhoneMacAddress[MOBILE_INFO_LENGTH];//本地PhoneMacAddress

	char		 PhoneDeviceID[MOBILE_INFO_LENGTH];//本地GetDeviceID
	char		 Phonecpu_pinlv[MOBILE_INFO_LENGTH];//本地getCpuInfo
	char         PhoneProvince[MOBILE_INFO_LENGTH]; //省
	char         PhoneLocCity[MOBILE_INFO_LENGTH]; //市
	char         PhoneLocDistrict[MOBILE_INFO_LENGTH];  //区
	char         PhoneLocAddr[4*MOBILE_INFO_LENGTH]; //详细地址

	double       PhoneLocLatitude; //纬度
	double       PhoneLocLongitude; //经度
	int          PhoneSDCardFreeSize;//获取SD卡剩余空间大小多少MB
	int          PhoneCardFreeSize;// //获取手机内存剩余空间大小多少MB
	int		     PhoneHeight;//手机屏幕高度 getHeight()
	int		     PhoneWidth;//手机屏幕高度 getWidth()
	int          PhoneYunYingShang; //运营商信息（1移动，2联通。3电信）
	int          PhoneSignalStrength;  //小于-100 的WIFI网络认为最差，-55 - 0之间认为极好
	int          PhoneSignalType;//网络连接类型  0为无网络 ，1为wifi ，3为3g，4为4g，2为2g（GPRS）
	int	         m_data_year;//时间
	int	         m_data_month;//时间
	int	         m_data_day;//时间
	int	         m_data_hour;//时间
	int	         m_data_min;//时间
	int	         m_data_sec;//时间

	//iii.所有游戏版本号
	//vii.机器码
}MobileIonfo;

 extern MobileIonfo m_MobileIonfo;

class GameConfig
{
private:

public:
	GameConfig();
	~GameConfig();

	static GameConfig* getInstance();

	//加载 string.xml 中的字符串
	void loadString();

	//加载 region.xml 中的字符串
	void LoadLocalString();

	//记载 config.xml 中的内容
	bool loadConfig(const ODEChar8* configFileName);
	//根据key获取string.xml中的对应的value
	const char* GetString( const char* pcName );

	//根据key获取regoin.xml中对应的省份和地区
	const char* GetLocalString(const char* pcName);

	//根据单位获取数字的整数部分
	const char* GetNumberLimit( int u64Number, int u32Limit );

	//由于时间限制，现在只支持房间列表限制积分显示
	const ODEChar8* GetDetalNumber(ODEUInt64 u64Number);

	//根据key获取config.xml中对应的value
	const char* GetConfigValue( const char* pcName );

	//获取当前scene的UILayerManager
	ODUILayerManager* getCurrentLayerManager();

	//获取某个文件的大小
	ODEInt32 GetFileSize(const char* pcFileName); 

	//AppStore支付解析Json
	ODEString GetJson(const char* srcStr,const char* startStr,const char* endStr);
	//支付完毕是否消息传递给可以连接Http服务器验证（）
	//bool getIsPayConnectHttpSever();
	//void setPayConnectHttpSever(bool isConnectHttp);
	//bool m_isConnectHttp;

//	bool  isUnipayConnect;
//	void setUnipayConnect(bool isConnect);
//	bool getUnipayConnect();
	void setMobileInfo();

	//设置玩家充值的地方
	void setPayState(int state);
	int  getPayState();
	int  pay_Entrance;//玩家是在哪里充值了
	MobileIonfo getMobileInfo();
private:

	int GetDigitNum(int u64Number);
	
	//保存字符串
	ODEStringMap<ODEFixedStringA> m_kStringMap;
	//保存配置文件
	ODEStringMap<ODEFixedStringA> m_kConfigMap;
    //保存地区字符串
	ODEStringMap<ODEFixedStringA> m_kregoinMap;
};

#endif // KessenConfig_h__
