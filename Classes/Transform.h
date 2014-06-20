#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "cocos2d.h"
#include <map>

#define MSG_LENGTH 128
/*
��Ϣǰ4�ֽ�ΪЭ��ͷ��int���ͣ�����ΪЭ������
�����int�ͣ����������1�ֽڵ��ַ���ʾ��int���ַ����ȣ�������int���ַ�����ʽ��������int���Ȳ��ܳ���9
�����string���������4�ֽڵ��ַ������ȣ�������ַ�������
������Ϣ���������'\0'(�ǽ�β��)�����滻Ϊ'~'����ֹ�ַ����ض�
*/
#define Send_Begin(header) char tempSend[MSG_LENGTH] = {0}; \
	int tempIndex = 0; \
	int tempInter = header; \
	const char* tempChar = nullptr; \
	memcpy(tempSend, &tempInter, sizeof(tempInter)); \
	tempIndex += sizeof(tempInter); \
	
#define ADD_INT(a) tempSend[tempIndex] = '0' + unity::GetDigits(a); \
	tempIndex++; \
	sprintf(tempSend+tempIndex,"%d",a); \
	tempIndex += (a/10 + 1);

#define ADD_STR(a) tempInter = strlen(a); \
	tempChar = a; \
	memcpy(tempSend + tempIndex, &tempInter, sizeof(tempInter)); \
	tempIndex += sizeof(tempInter); \
	memcpy(tempSend + tempIndex, tempChar, tempInter); \
	tempIndex += tempInter;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define  Send_END() sendMessage(g_Transform.Encode(tempSend));
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define  Send_END()
#endif

#define GET_INT(data,index) unity::GetDataInt(data,index);


#define MSG_START 1
#define MSG_INIT_CARD 2
#define MSG_POINT 3
#define MSG_SCENE_INIT 4
#define MSG_MOVE 5
#define MSG_ADD_CARD 6

class Transform;
typedef void(Transform::*MsgFunc)(const char*);
typedef std::map<int, MsgFunc> MFunc;
//process and transform data between devices
class Transform
{
public:
	Transform();
	~Transform();

	void Parse(const char* data);
	char* Encode(const char* data);
	char* Decode(const char* data);

	void Send_Start();
	void Msg_Start(const char* data);
	void Send_Scene_Init();
	void Msg_Scene_Init(const char* data);
	void Send_Init_Card(int x1,int y1,int num1,int x2,int y2,int num2);
	void Msg_Init_Card(const char* data);
	void Send_Point(int pt);
	void Msg_Point(const char* data);
	void Send_Move(int dir);
	void Msg_Move(const char* data);
	void Send_Add_Card(int x, int y, int num);
	void Msg_Add_Card(const char* data);
private:
	char m_strMsg[MSG_LENGTH];
	MFunc m_mFunc;
};

extern Transform g_Transform;
#endif