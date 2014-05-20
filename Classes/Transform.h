#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "cocos2d.h"
#include <map>

#define MSG_LENGTH 128
/*
消息前4字节为协议头，int类型
后面为协议数据，如果是int型，则直接填充，占4字节
如果是string，则先填充4字节的字符串长度，再填充字符串本身
整个消息中如果含有'\0'(非结尾处)，则替换为'~'，防止字符串截断
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