#ifndef __JNI_FUNC_H__
#define __JNI_FUNC_H__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

#define CLASS_NAME "org/cocos2dx/cpp/JniHelper"
#define EXIT_DIALOG 0x01
#define NO_BLUETOOTH_DIALOG 0x02

#define STATE_NONE 0					//初始化及断开连接时的状态
#define STATE_LISTEN 1					//监听状态
#define STATE_CONNECTING 2		//主动连接中状态
#define STATE_CONNECTED 3		//已经连接状态，可以收发数据了

extern "C"
{
	void showTipDialog(const char* title, const char* msg, int msgID);
	void searchBluetooth();
	void initBluetooth();
	void stopBluetooth();
	void sendMessage(const char* data);
	unsigned long getTickCount();
	void debugLog(const char* tag,const char* data);
}
#endif

#endif