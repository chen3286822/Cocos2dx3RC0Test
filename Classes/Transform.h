#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "cocos2d.h"
#include <map>

#define MSG_LENGTH 128
/*
��Ϣǰ4�ֽ�ΪЭ��ͷ��int����
����ΪЭ�����ݣ������int�ͣ���ֱ����䣬ռ4�ֽ�
�����string���������4�ֽڵ��ַ������ȣ�������ַ�������
������Ϣ���������'\0'(�ǽ�β��)�����滻Ϊ'~'����ֹ�ַ����ض�
*/
#define Send_Begin(header) char tempSend[MSG_LENGTH] = {0}; \
	int tempIndex = 0; \
	int tempInter = header; \
	const char* tempChar = nullptr; \
	memcpy(tempSend, &tempInter, sizeof(tempInter)); \
	tempIndex += sizeof(tempInter); \
	
#define ADD_INT(a) tempInter = a; \
	memcpy(tempSend + tempIndex, &tempInter, sizeof(tempInter)); \
	tempIndex += sizeof(tempInter);
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

#define MSG_START 1
#define MSG_INIT_CARD 2
#define MSG_POINT 3

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
	void Msg_Init_Card(const char* data);
	void Send_Point(int pt);
	void Msg_Point(const char* data);
private:
	char m_strMsg[MSG_LENGTH];
	MFunc m_mFunc;
};

extern Transform g_Transform;
#endif