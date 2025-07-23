#include "GuanDanConfig.h"
#include "PayNotify.h"
#include "GameJNIPlatform.h"
#include "ClientData.h"


static GameConfig* m_gameConfig = NULL;

MobileIonfo m_MobileIonfo;

GameConfig::GameConfig()
{
//   m_isConnectHttp=false;
   m_gameConfig=NULL;
 //  isUnipayConnect=false;
   pay_Entrance=0;
}

GameConfig::~GameConfig()
{

}

GameConfig* GameConfig::getInstance()
{
	if (!m_gameConfig )
	{
		m_gameConfig = new GameConfig();
	}
	return m_gameConfig;
}

void GameConfig::loadString()
{
	std::string pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename("text/string.xml");
	unsigned char* pbyBuffer = NULL;
	unsigned long ulSize = 0;
	pbyBuffer = CCFileUtils::sharedFileUtils()->getFileData(pathKey.c_str(), "r", &ulSize);
	if (ulSize)
	{
		ODEChar8* pcMem = (ODEChar8*)ODEMalloc(ulSize + 1);
		ODEMemcpy(pcMem, pbyBuffer, ulSize);
		pcMem[ulSize] = 0;
		ODEXMLDocument kDoc;
		kDoc.Parse(pcMem);
		ODEFree(pcMem);
		ODE_ASSERT(!kDoc.Error());
		ODEXMLElement* pkRoot = kDoc.RootElement();

		pkRoot = pkRoot->FirstChildElement("zh_CN");
		ODE_ASSERT(pkRoot);

		ODEXMLElement* pkEntry = pkRoot->FirstChildElement();
		while(pkEntry)
		{
			const ODEChar8* pcName = pkEntry->Value();
			const ODEChar8* pcValue = pkEntry->GetAttribute("value");
			if (pcName && pcValue)
			{
				m_kStringMap[pcName] = pcValue;
			}
			pkEntry = pkEntry->NextSiblingElement();
		}
		if (pbyBuffer) delete [] pbyBuffer;
	}
}

void GameConfig::LoadLocalString()
{

	std::string pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename("regoin.xml");
	unsigned char* pbyBuffer = NULL;
	unsigned long ulSize = 0;
	pbyBuffer = CCFileUtils::sharedFileUtils()->getFileData(pathKey.c_str(), "r", &ulSize);
	if (ulSize)
	{
		ODEChar8* pcMem = (ODEChar8*)ODEMalloc(ulSize + 1);
		ODEMemcpy(pcMem, pbyBuffer, ulSize);
		pcMem[ulSize] = 0;
		ODEXMLDocument kDoc;
		kDoc.Parse(pcMem);
		ODEFree(pcMem);
		ODE_ASSERT(!kDoc.Error());
		ODEXMLElement* pkRoot = kDoc.RootElement();

		pkRoot = pkRoot->FirstChildElement("zh_CN");
		ODE_ASSERT(pkRoot);

		ODEXMLElement* pkEntry = pkRoot->FirstChildElement();
		while(pkEntry)
		{
			const ODEChar8* pcName = pkEntry->Value();
			const ODEChar8* pcValue = pkEntry->GetAttribute("value");
			if (pcName && pcValue)
			{
				m_kregoinMap[pcName] = pcValue;
			}
			pkEntry = pkEntry->NextSiblingElement();
		}
		if (pbyBuffer) delete [] pbyBuffer;
	}
}

bool GameConfig::loadConfig(const ODEChar8* configFileName)
{
		bool loadLocal = true;
		//bool loadLocal = false;

	if (strcmp(TT("UpdateConfigOnInstall"), "1") != 0)
	{
		bool isFirstInstall = CCUserDefault::sharedUserDefault()->getBoolForKey("first_install_in_app", true);

		if (isFirstInstall)
		{
			CCUserDefault::sharedUserDefault()->setBoolForKey("first_install_in_app", false);
			CCUserDefault::sharedUserDefault()->flush();

			loadLocal = true;
		}
	}

	std::string pathKey = "";
	if (loadLocal)
	{
		pathKey = ""; // 内网
		pathKey += TT(configFileName);
	}
	else
	{
		pathKey = CCFileUtils::sharedFileUtils()->getWritablePath();  //外网
	//std::string pathKey = "";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	pathKey += TT ("ConfigZipPathTemp");
	pathKey += + "/";
	pathKey += TT(configFileName);
#else
	pathKey += TT(configFileName);
#endif
	}

	CCLOG ("config file %s", pathKey.c_str ());

	unsigned char* pbyBuffer = NULL;
	unsigned long ulSize = 0;
	pbyBuffer = CCFileUtils::sharedFileUtils()->getFileData(pathKey.c_str(), "r", &ulSize);
	if (ulSize)
	{
		ODEChar8* pcMem = (ODEChar8*)ODEMalloc(ulSize + 1);
		ODEMemcpy(pcMem, pbyBuffer, ulSize);
		pcMem[ulSize] = 0;
		ODEXMLDocument kDoc;
		kDoc.Parse(pcMem);
		ODEFree(pcMem);
		if (kDoc.Error())
		{
			return false;
		}
		ODEXMLElement* pkRoot = kDoc.RootElement();

		pkRoot = pkRoot->FirstChildElement("Client");
		if (pkRoot == NULL)
		{
			return false;
		}

		m_kConfigMap.Clear();
		ODEXMLElement* pkEntry = pkRoot->FirstChildElement();
		while(pkEntry)
		{
			const ODEChar8* pcName = pkEntry->Value();
			const ODEChar8* pcValue = pkEntry->GetAttribute("value");

			if (pcName && pcValue)
			{
				m_kConfigMap[pcName] = pcValue;
			}
			pkEntry = pkEntry->NextSiblingElement();
		}

		if (pbyBuffer) delete [] pbyBuffer;

		//CCLOG(GetConfig("OdaoCheck"));

		if (ODEStrcmp(GetConfig("OdaoCheck"), "2E7DFD0F65BD8C28C568F44102E23BDC") != 0)
		{
			return false;
		}
		//if (ODEStrcmp(GetConfig("OdaoCheck"), "2E7DFD0F65BD8C28C568F44102E23BDC") != 0) return false;

		std::string configVersion = GetConfig("ConfigVersion");
		g_pkGuanDanGame->SaveStringByCCFile("GameConfigVersionClient", configVersion);

		if (g_pkGuanDanGame->m_isAppStore)
		{
			string clientVersion = TT("ClientVersion");
			string updateClientVersion = GetConfig("UpdateClientVersion");
			string serverLimitClientVersion = GetConfig("ServerLimitClientVersion");
            CCLOG("FUNCTION [%s] LINE [%d] CLIENT VERSION [%s] UPDATE CLIENT VERSION [%s] SERVER LIMIT CLIENT VERSION [%s]", __FUNCTION__,
                  __LINE__, clientVersion.c_str(), updateClientVersion.c_str(), serverLimitClientVersion.c_str());
			if (strcmp(clientVersion.c_str(), updateClientVersion.c_str()) < 0)
			{
				g_pkGuanDanGame->m_isAppStoreAuditVersion = false;
				if (strcmp(clientVersion.c_str(), serverLimitClientVersion.c_str()) < 0)
				{
                    CCLOG("FUNCTION [%s] LINE [%d] NEED FORCE UPDATE CLIENT", __FUNCTION__, __LINE__);
				}
				else
				{
                    CCLOG("FUNCTION [%s] LINE [%d] THERE IS A NEW VERSION EXISTED", __FUNCTION__, __LINE__);
				}
			}
			else if (strcmp(clientVersion.c_str(), updateClientVersion.c_str()) == 0)
			{
				if (strcmp(clientVersion.c_str(), serverLimitClientVersion.c_str()) > 0)
				{
#ifdef __FORCE_NO_AUDIT_VERSION__
                    g_pkGuanDanGame->m_isAppStoreAuditVersion = false;
#else
                    g_pkGuanDanGame->m_isAppStoreAuditVersion = true;
#endif //__FORCE_NO_AUDIT_VERSION__
					
                    CCLOG("FUNCTION [%s] LINE [%d] THIS IS THE AUDIT VERSION", __FUNCTION__, __LINE__);
				}
				else
				{
					g_pkGuanDanGame->m_isAppStoreAuditVersion = false;
                    CCLOG("FUNCTION [%s] LINE [%d] THIS IS THE NORMAL VERSION", __FUNCTION__, __LINE__);
				}
			}
			else
			{
				g_pkGuanDanGame->m_isAppStoreAuditVersion = false;
                CCLOG("FUNCTION [%s] LINE [%d] CLIENT VERSION IS TO BIG", __FUNCTION__, __LINE__);
			}
		}
        
		CCLOG("FUNCTION [%s] LINE [%d] IS APPSTORE [%d] IS APPSTORE AUDIT [%d]", __FUNCTION__, __LINE__, g_pkGuanDanGame->m_isAppStore, g_pkGuanDanGame->m_isAppStoreAuditVersion);
		g_pkClientData->initRandomLogin();

	}
	else 
	{
		return false;
	}

	return true;
}
int GameConfig::GetDigitNum(int u64Number)
{
	int u32Res(0);
	do 
	{
		++u32Res;
		u64Number /= 10;
	} while (u64Number);
	return u32Res;
}

const char* GameConfig::GetString( const char* pcName )
{
	ODEStringMap<ODEFixedStringA>::iterator iter = m_kStringMap.Find(pcName);
	if (iter)
	{
		return *iter;
	}
	else
	{
		return pcName;
	}
	//return iter ? *iter : "none";
}

const char* GameConfig::GetLocalString(const char* pcName)
{

	ODEStringMap<ODEFixedStringA>::iterator iter = m_kregoinMap.Find(pcName);
	if (iter)
	{
		return *iter;
	}
	else
	{
		return pcName;
	}
}

const ODEChar8* GameConfig::GetDetalNumber(ODEUInt64 u64Number)
{
	static ODEChar8 s_acBuffer[1024];
	memset(s_acBuffer, 0, 1024);
	//单位 个
	if (u64Number % 10000 == 0)
	{
		//单位万
		ODEUInt64 subU64Number = u64Number / 10000;
		ODESprintf(s_acBuffer, 1024, "%lld%s", (ODEUInt64)subU64Number, TT("W"));
	}
	else
	{
		//单位个
		ODEUInt64 subU64Number = u64Number / 10000;
		if (subU64Number == 0)
		{
			//单位千
			ODEUInt64 qianU64Number = u64Number / 1000;
			//百
			ODEUInt64 baiU64Number = u64Number / 100 % 10;
			//十
			ODEUInt64 shiU64Number = u64Number / 10 % 10;
			//个
			ODEUInt64 geU64Number = u64Number % 10;
			if (qianU64Number == 0)
			{
				if (baiU64Number == 0)
				{
					if (shiU64Number == 0)
					{
						ODESprintf(s_acBuffer, 1024, "%lld", (ODEUInt64)geU64Number);
					}
					else
					{
						if (geU64Number == 0)
						{
							ODESprintf(s_acBuffer, 1024, "%lld%s", (ODEUInt64)shiU64Number, TT("S"));
						}
						else
						{
							ODESprintf(s_acBuffer, 1024, "%lld%s%lld", (ODEUInt64)shiU64Number, TT("S"), geU64Number);
						}
					}
				}
				else
				{
					if (shiU64Number == 0)
					{
						ODESprintf(s_acBuffer, 1024, "%lld%s", (ODEUInt64)baiU64Number, TT("B"));
					}
					else
					{
						if (geU64Number == 0)
						{
							ODESprintf(s_acBuffer, 1024, "%lld%s%lld%s", (ODEUInt64)baiU64Number, TT("B"), (ODEUInt64)shiU64Number, TT("S"));
						}
						else
						{
							ODESprintf(s_acBuffer, 1024, "%lld", (ODEUInt64)u64Number);
						}
					}
				}
			}
			else
			{
				if (baiU64Number == 0)
				{
					ODESprintf(s_acBuffer, 1024, "%lld%s", (ODEUInt64)qianU64Number, TT("Q"));
				}
				else
				{
					if (shiU64Number == 0)
					{
						ODESprintf(s_acBuffer, 1024, "%lld%s%lld%s", (ODEUInt64)qianU64Number, TT("Q"), (ODEUInt64)baiU64Number, TT("B"));
					}
					else
					{
						if (geU64Number == 0)
						{
							ODESprintf(s_acBuffer, 1024, "%lld%s%lld%s%lld%s", (ODEUInt64)qianU64Number, TT("Q"), (ODEUInt64)baiU64Number, TT("B"), (ODEUInt64)shiU64Number, TT("S"));
						}
						else
						{
							ODESprintf(s_acBuffer, 1024, "%lld", (ODEUInt64)u64Number);
						}
					}
				}
			}
		}
		else
		{
			ODEUInt64 subSubU64Number = u64Number % 10000;
			while (subSubU64Number % 10 == 0)
			{
				subSubU64Number = subSubU64Number / 10;
			}
			ODESprintf(s_acBuffer, 1024, "%lld.%lld%s", (ODEUInt64)subU64Number, (ODEUInt64)subSubU64Number, TT("W"));
		}
	}
	return s_acBuffer;
}

const char* GameConfig::GetNumberLimit( int u64Number, int u32Limit )
{
	static char s_acBuffer[1024];
	ODE_ASSERT(u32Limit >= 4);
	int u32DigitNum = GetDigitNum(u64Number);

	if(u32DigitNum <= u32Limit)
	{
		sprintf(s_acBuffer, "%llu", u64Number);
	}
	else if(u32DigitNum <= (u32Limit + 2))
	{
		sprintf(s_acBuffer, "%lluk", u64Number / 1000ull);
	}
	else if(u32DigitNum <= (u32Limit + 5))
	{
		sprintf(s_acBuffer,"%llum", u64Number / 1000000ull);
	}
	else if(u32DigitNum <= (u32Limit + 8))
	{
		sprintf(s_acBuffer,"%llub", u64Number / 1000000000ull);
	}
	else
	{
		sprintf(s_acBuffer, "nan");
	}
	return s_acBuffer;
}

const char* GameConfig::GetConfigValue( const char* pcName )
{
	ODEStringMap<ODEFixedStringA>::iterator iter = m_kConfigMap.Find(pcName);
	return iter ? *iter : "none";
}

ODUILayerManager* GameConfig::getCurrentLayerManager()
{
	return g_pkGuanDanGame->GetLayerManager();
}

int GameConfig::GetFileSize(const char* pcFileName)
{
	FILE * fp = fopen(pcFileName, "r");   
	fseek(fp, 0L, SEEK_END);   
	int size = ftell(fp);   
	fclose(fp);   
	return size;   
}

ODEString GameConfig::GetJson(const char* srcStr,const char* startStr,const char* endStr)
{
	ODEString res="";
	if(!startStr || !endStr || !srcStr)
		return res;
	ODEString src = srcStr;
	int start = src.Find(startStr);
	if(start!=int(-1))
	{
		res = src.GetSubstring(start+strlen(startStr),endStr);
		res.Replace("\\r\\n", "\r\n");
	}
	return res;
}
