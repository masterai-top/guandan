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
	char         PhoneConfigVersion[MOBILE_INFO_LENGTH];//�����ļ��汾��  
	char		 PhoneDouziVersion[MOBILE_INFO_LENGTH];//�ֻ����Ӱ汾  getPackageVersion
	char		 PhoneSdkVersion[MOBILE_INFO_LENGTH];//�ֻ�sdk�汾  getSdkVersion()
	char		 PhoneDeviceName[MOBILE_INFO_LENGTH];//�ֻ��ͺ� �磺�豸����  iPhone OS getDeviceName()
	char		 PhoneFactory[MOBILE_INFO_LENGTH];//�ֻ������������� getFactory()
	char	     PhoneModel[MOBILE_INFO_LENGTH];//�ֻ�Ʒ�� getPhoneModel()
	char		 PhoneChannelId[MOBILE_INFO_LENGTH];//�ֻ�������Ϣ
	char		 PhoneIpAddress[MOBILE_INFO_LENGTH];//����IPgetLocalIpAddress
	char		 PhoneMacAddress[MOBILE_INFO_LENGTH];//����PhoneMacAddress

	char		 PhoneDeviceID[MOBILE_INFO_LENGTH];//����GetDeviceID
	char		 Phonecpu_pinlv[MOBILE_INFO_LENGTH];//����getCpuInfo
	char         PhoneProvince[MOBILE_INFO_LENGTH]; //ʡ
	char         PhoneLocCity[MOBILE_INFO_LENGTH]; //��
	char         PhoneLocDistrict[MOBILE_INFO_LENGTH];  //��
	char         PhoneLocAddr[4*MOBILE_INFO_LENGTH]; //��ϸ��ַ

	double       PhoneLocLatitude; //γ��
	double       PhoneLocLongitude; //����
	int          PhoneSDCardFreeSize;//��ȡSD��ʣ��ռ��С����MB
	int          PhoneCardFreeSize;// //��ȡ�ֻ��ڴ�ʣ��ռ��С����MB
	int		     PhoneHeight;//�ֻ���Ļ�߶� getHeight()
	int		     PhoneWidth;//�ֻ���Ļ�߶� getWidth()
	int          PhoneYunYingShang; //��Ӫ����Ϣ��1�ƶ���2��ͨ��3���ţ�
	int          PhoneSignalStrength;  //С��-100 ��WIFI������Ϊ��-55 - 0֮����Ϊ����
	int          PhoneSignalType;//������������  0Ϊ������ ��1Ϊwifi ��3Ϊ3g��4Ϊ4g��2Ϊ2g��GPRS��
	int	         m_data_year;//ʱ��
	int	         m_data_month;//ʱ��
	int	         m_data_day;//ʱ��
	int	         m_data_hour;//ʱ��
	int	         m_data_min;//ʱ��
	int	         m_data_sec;//ʱ��

	//iii.������Ϸ�汾��
	//vii.������
}MobileIonfo;

 extern MobileIonfo m_MobileIonfo;

class GameConfig
{
private:

public:
	GameConfig();
	~GameConfig();

	static GameConfig* getInstance();

	//���� string.xml �е��ַ���
	void loadString();

	//���� region.xml �е��ַ���
	void LoadLocalString();

	//���� config.xml �е�����
	bool loadConfig(const ODEChar8* configFileName);
	//����key��ȡstring.xml�еĶ�Ӧ��value
	const char* GetString( const char* pcName );

	//����key��ȡregoin.xml�ж�Ӧ��ʡ�ݺ͵���
	const char* GetLocalString(const char* pcName);

	//���ݵ�λ��ȡ���ֵ���������
	const char* GetNumberLimit( int u64Number, int u32Limit );

	//����ʱ�����ƣ�����ֻ֧�ַ����б����ƻ�����ʾ
	const ODEChar8* GetDetalNumber(ODEUInt64 u64Number);

	//����key��ȡconfig.xml�ж�Ӧ��value
	const char* GetConfigValue( const char* pcName );

	//��ȡ��ǰscene��UILayerManager
	ODUILayerManager* getCurrentLayerManager();

	//��ȡĳ���ļ��Ĵ�С
	ODEInt32 GetFileSize(const char* pcFileName); 

	//AppStore֧������Json
	ODEString GetJson(const char* srcStr,const char* startStr,const char* endStr);
	//֧������Ƿ���Ϣ���ݸ���������Http��������֤����
	//bool getIsPayConnectHttpSever();
	//void setPayConnectHttpSever(bool isConnectHttp);
	//bool m_isConnectHttp;

//	bool  isUnipayConnect;
//	void setUnipayConnect(bool isConnect);
//	bool getUnipayConnect();
	void setMobileInfo();

	//������ҳ�ֵ�ĵط�
	void setPayState(int state);
	int  getPayState();
	int  pay_Entrance;//������������ֵ��
	MobileIonfo getMobileInfo();
private:

	int GetDigitNum(int u64Number);
	
	//�����ַ���
	ODEStringMap<ODEFixedStringA> m_kStringMap;
	//���������ļ�
	ODEStringMap<ODEFixedStringA> m_kConfigMap;
    //��������ַ���
	ODEStringMap<ODEFixedStringA> m_kregoinMap;
};

#endif // KessenConfig_h__
