#include "Transform.h"
#include "Unity.h"
#include "JNIFunc.h"
#include "BluetoothScene.h"
#include "HelloWorldScene.h"
#include "GameData.h"
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

/*���ַ����п�λ������ַ�'~' ʹ�ַ������ᱻ�ضϣ�������ʵ�������в�����'~'�ַ�
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
	�գ�����ͨ��Э������ָʾ��Ϸ��ʼ
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
	֪ͨ�Է��Լ�����Ϸ����������ɣ��˺���Դ��俨Ƭλ�õ���Ϣ��
*/
void Transform::Send_Scene_Init()
{
	Send_Begin(MSG_SCENE_INIT);
	Send_END();
}

/*
	�գ���ͨ��Э������֪ͨ�Է���Ϸ�����������
*/
void Transform::Msg_Scene_Init(const char* data)
{
	g_Gamedata.setSceneInit(true);
}

/*
	���ͳ�ʼ�������ſ�Ƭ���ݸ��Է�
*/
void Transform::Send_Init_Card(int x1, int y1, int num1, int x2, int y2, int num2)
{
	Send_Begin(MSG_INIT_CARD);
	ADD_INT(x1);
	ADD_INT(y1);
	ADD_INT(num1);
	ADD_INT(x2);
	ADD_INT(y2);
	ADD_INT(num2);
	Send_END();
}

/*
	int : ��һ��Ƭ��x���� 
	int : ��һ��Ƭ��y����
	int : ��һ��Ƭ��ֵ
	int : �ڶ���Ƭ��x����
	int : �ڶ���Ƭ��y����
	int : �ڶ���Ƭ��ֵ
*/
void Transform::Msg_Init_Card(const char* data)
{
	int index = 0;
	int x1 = GET_INT(data, index);
	int y1 = GET_INT(data, index);
	int num1 = GET_INT(data, index);
	int x2 = GET_INT(data, index);
	int y2 = GET_INT(data, index);
	int num2 = GET_INT(data, index);
	unity::Log(TAG, "(%d,%d) num:%d,  (%d,%d) num:%d",x1,y1,num1,x2,y2,num2);
	auto layer = dynamic_cast<HelloWorld*>(cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByTag(HelloWorld::eChild_HelloWorldLayer));
	if (layer)
	{
		auto region = dynamic_cast<CardRegion*>(layer->getChildByTag(HelloWorld::eChild_OtherCardRegion));
		if (region)
		{
			region->AddCard(x1, y1, num1);
			region->AddCard(x2, y2, num2);
		}
	}
}

/*
	int : ����
*/
void Transform::Send_Point(int pt)
{
	Send_Begin(MSG_POINT);
	ADD_INT(pt);
	Send_END();
}

void Transform::Msg_Point(const char* data)
{
	int index = 0;
	int point = GET_INT(data, index);
	unity::Log(TAG, "point : %d", point);

	auto layer = dynamic_cast<HelloWorld*>(cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByTag(HelloWorld::eChild_HelloWorldLayer));
	if(layer)
	{
		layer->AddOtherPoint(point);
	}
}