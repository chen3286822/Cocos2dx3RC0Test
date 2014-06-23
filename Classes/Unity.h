#ifndef __UNITY_H__
#define __UNITY_H__

#include "cocos2d.h"
#include "Transform.h"

#define TAG "2048Debug"
namespace unity
{
	const char* GetDefaultFontType();
	unsigned long GetTickCountX();
	void Log(const char* tag,const char* format,...);		//CCLog 可以替代，只不过无法自定义标签，但是尽量少用该函数，减少jni传递
	int GetDigits(int a);
	int GetDataInt(const char* data,int& index);
	void CleanAndExit();

	class UserData
	{
	public:
		static const int RECORDSNUM{ 10 };
		struct Record
		{
			int m_nRank;
			std::string m_strName;
			int m_nScore;
			Record()
			{
				m_nRank = 0;
				m_strName = "";
				m_nScore = 0;
			}
			Record(int rank, const char* name, int score)
			{
				m_nRank = rank;
				m_strName = name;
				m_nScore = score;
			}
		};

		static UserData* getInstance();
		static void destroyInstance();

		bool HasBreakRecord(int score);
		int GetCurrentHighScore(int currentScore);
		void InsertRecord(const char* name,int score);
		std::map<int, Record>& GetRecords(){ return m_mRecords; }
		Record* GetRecord(int rank);

	private:
		UserData();
		~UserData(){};

		static UserData* m_sUserData;
		static Record m_sRecords[RECORDSNUM];

		std::map<int, Record> m_mRecords;
	};
}


#endif