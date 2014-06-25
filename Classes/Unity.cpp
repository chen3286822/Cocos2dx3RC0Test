#include "Unity.h"
#include "JNIFunc.h"

namespace unity
{
	const char* GetDefaultFontType()
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		return "fonts/American Typewriter.ttf";
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return "fonts/arial.ttf";
#endif
	}

	unsigned long GetTickCountX()
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		return getTickCount();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return GetTickCount();
#endif
	}

	void Log(const char* tag, const char* format,...)
	{
		char temp[250] = {0};
		va_list pArgs;
		va_start (pArgs, format);
		vsprintf(temp, format, pArgs);
		va_end(pArgs);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		debugLog(tag,temp);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
// 		wchar_t temp2[250] = {0};
// 		LPCWSTR LPformat = LPCWSTR("%s : %s");
// 		wsprintf(temp2, LPformat, tag, temp);
// 		OutputDebugString(temp2);
#endif
		

	}

	int GetDigits(int a)
	{
		int digits = 1;
		while (a >= 10)
		{
			a = a / 10;
			digits++;
		}
		return digits;
	}

	int GetDataInt(const char* data, int& index)
	{
		int length = (int)(data[index] - '0');
		index += 1;
		std::string dataStr = data;
		dataStr = dataStr.substr(index, length);
		index += length;
		return atoi(dataStr.c_str());
	}

	void CleanAndExit()
	{
		UserData::destroyInstance();

		cocos2d::CCDirector::getInstance()->end();
	}

	//////////////////////////////////////////////////////////////////////////
	UserData* UserData::m_sUserData = nullptr;
	UserData::Record UserData::m_sRecords[RECORDSNUM] = { 
		UserData::Record(10,"Akam",2000), 
		UserData::Record(9, "Bob", 4000),
		UserData::Record(8, "Steve", 6000),
		UserData::Record(7, "Leonard", 8000),
		UserData::Record(6, "Kobe", 10000),
		UserData::Record(5, "Sam", 15000),
		UserData::Record(4, "Naruto", 20000),
		UserData::Record(3, "Doom", 30000),
		UserData::Record(2, "Acfun", 50000),
		UserData::Record(1, "Bilibili", 100000),
	};

	UserData::UserData()
	{
		m_mRecords.clear();

		char str[50];
		char str2[100];
		for (int i = 1; i <= RECORDSNUM; i++)
		{
			sprintf(str, "Record%d", RECORDSNUM-i+1);
			sprintf(str2, "%s /%d", m_sRecords[i-1].m_strName.c_str(), m_sRecords[i-1].m_nScore);
			std::string value = cocos2d::UserDefault::getInstance()->getStringForKey(str, str2);
			Record record;
			record.m_nRank = m_sRecords[i-1].m_nRank;
			std::stringstream ssteam(value);
			char cTemp;
			ssteam >> record.m_strName >> cTemp >> record.m_nScore;
			m_mRecords[i] = record;
		}
	}

	UserData* UserData::getInstance()
	{
		if (!m_sUserData)
		{
			m_sUserData = new UserData();
		}

		return m_sUserData;
	}

	void UserData::destroyInstance()
	{
		CC_SAFE_DELETE(m_sUserData);
	}

	bool UserData::HasBreakRecord(int score)
	{
		for (auto mit = m_mRecords.begin(); mit != m_mRecords.end(); ++mit)
		{
			if (score > mit->second.m_nScore)
			{
				return true;
			}
		}
		return false;
	}

	int UserData::GetCurrentHighScore(int currentScore)
	{
		for (auto mit = m_mRecords.begin(); mit != m_mRecords.end(); ++mit)
		{
			if (currentScore <= mit->second.m_nScore)
			{
				return mit->second.m_nScore;
			}
		}

		return 0;
	}

	void UserData::InsertRecord(const char* name, int score)
	{		
		for (auto rit = m_mRecords.begin(); rit != m_mRecords.end(); ++rit)
		{
			if (score <= (rit->second.m_nScore))
			{
				//小于等于第10名
				if (rit == m_mRecords.begin())
					break;
				//插入其中，后面的依次后移一名
				for (auto mit2 = m_mRecords.begin(); mit2 != rit; ++mit2)
				{
					auto mBack = mit2;
					mBack++;
					if (mBack != rit)
					{
						mit2->second.m_strName = mBack->second.m_strName;
						mit2->second.m_nScore = mBack->second.m_nScore;
					}
					else
					{
						mit2->second.m_strName = name;
						mit2->second.m_nScore = score;
					}
				}
				break;
			}
		}

		char str[50];
		char str2[100];
		for (auto mit = m_mRecords.begin(); mit != m_mRecords.end(); mit++)
		{		
			sprintf(str, "Record%d", mit->second.m_nRank);			
			sprintf(str2, "%s /%d", mit->second.m_strName.c_str(), mit->second.m_nScore);
			cocos2d::UserDefault::getInstance()->setStringForKey(str, str2);
		}
	}

	UserData::Record* UserData::GetRecord(int rank)
	{
		if (rank < 1 || rank > RECORDSNUM)
			return nullptr;

		for (auto mit = m_mRecords.begin(); mit != m_mRecords.end();mit++)
		{
			if (rank == mit->second.m_nRank)
				return &(mit->second);;
		}

		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
}