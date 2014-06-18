#include "Transform.h"
#include "Unity.h"
#include "JNIFunc.h"
#include "BluetoothScene.h"
#include "HelloWorldScene.h"
#include <string>

Transform g_Transform;

Transform::Transform()
{
	memset(m_strMsg, 0, MSG_LENGTH);

	m_mFunc.clear();
	m_mFunc[MSG_START] = &Transform::Msg_Start;
	m_mFunc[MSG_INIT_CARD] = &Transform::Msg_Init_Card;
	m_mFunc[MSG_POINT] = &Transform::Msg_Point;
}

Transform::~Transform()
{

}

/*将字符串中空位填充上字符'~' 使字符串不会被截断，限制是实际数据中不能有'~'字符
*/
char* Transform::Encode(const char* data)
{
	memcpy(m_strMsg, data, MSG_LENGTH);
	for (int i = 0; i < MSG_LENGTH; i++)
	{
		if (m_strMsg[i] == 0)
			m_strMsg[i] = 126;		
	}
	m_strMsg[MSG_LENGTH - 1] = '\0';
	return m_strMsg;
}

char* Transform::Decode(const char* data)
{
	//unity::Log(TAG, "data length:%d",strlen(data));
	memcpy(m_strMsg, data, MSG_LENGTH);
	//unity::Log(TAG, "memcpy over");
	for (int i = 0; i < MSG_LENGTH - 1; i++)
	{
		if (m_strMsg[i] == '~')
			m_strMsg[i] = 0;
	}
	return m_strMsg;
}

void Transform::Parse(const char* data)
{
	//unity::Log(TAG, "begin decode");
	//CCLOG("data is : %s", data);
	Decode(data);
	//unity::Log(TAG, "decode over");

	//parse the protocol header
	int header = 0;
	memcpy(&header, m_strMsg, sizeof(header));
	unity::Log(TAG, "header:%d",header);

	MFunc::iterator it = m_mFunc.find(header);
	if (it != m_mFunc.end())
	{
		(this->*(it->second))(m_strMsg + sizeof(header));
	}
}

void Transform::Send_Start()
{
	Send_Begin(MSG_START);
	Send_END();
}

/*
	空，仅仅通过协议类型指示游戏开始
*/
void Transform::Msg_Start(const char* data)
{
	auto layer = dynamic_cast<HelloWorld*>(cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByTag(HelloWorld::eChild_HelloWorldLayer));
	if (layer)
	{
		layer->InformToStart();
		unity::Log(TAG, "I was informed!");
	}
}

/*
	int : 第一卡片的x坐标 
	int : 第一卡片的y坐标
	int : 第一卡片的值
	int : 第二卡片的x坐标
	int : 第二卡片的y坐标
	int : 第二卡片的值
*/
void Transform::Msg_Init_Card(const char* data)
{

}

/*
	int : 分数
*/
void Transform::Send_Point(int pt)
{
	Send_Begin(MSG_POINT);
	ADD_INT(pt);
	Send_END();
}

void Transform::Msg_Point(const char* data)
{
	int ptLength = (int)(data[0] - '0');
	CCLOG("data is : %s", data);
	std::string ptStr = data;
	ptStr = ptStr.substr(1, ptLength);
	int point = atoi(ptStr.c_str());
	unity::Log(TAG, "point : %d", point);

	auto layer = dynamic_cast<HelloWorld*>(cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByTag(HelloWorld::eChild_HelloWorldLayer));
	if(layer)
	{
		layer->AddOtherPoint(point);
	}
}