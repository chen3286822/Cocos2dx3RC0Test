#include "JNIFunc.h"
#include "cocos2d.h"
#include "BluetoothScene.h"
#include "Transform.h"
#include "Unity.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)


extern "C"
{
	void showTipDialog(const char* title, const char* msg, int msgID)
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showTipDialog", "(Ljava/lang/String;Ljava/lang/String;I)V"))
		{
			jstring jTitle = t.env->NewStringUTF(title);
			jstring jMsg = t.env->NewStringUTF(msg);
			jint jMsgID = msgID;
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jTitle, jMsg, jMsgID);
			t.env->DeleteLocalRef(jTitle);
			t.env->DeleteLocalRef(jMsg);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	//搜索蓝牙设备
	void searchBluetooth()
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "connectBluetooth", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	//初始化蓝牙
	void initBluetooth()
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "initBluetooth", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	//停止蓝牙连接
	void stopBluetooth()
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "stopBluetooth", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	//使用toast显示信息
	void toastMsg(const char* data)
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "toastMsg", "(Ljava/lang/String;)V"))
		{
			jstring jData = t.env->NewStringUTF(data);
			t.env->CallStaticVoidMethod(t.classID, t.methodID,jData);
			t.env->DeleteLocalRef(jData);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	//向连接设备发送数据
	void sendMessage(const char* data)
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "sendMessage", "(Ljava/lang/String;)V"))
		{
			jstring jData = t.env->NewStringUTF(data);
			t.env->CallStaticVoidMethod(t.classID, t.methodID,jData);
			t.env->DeleteLocalRef(jData);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	unsigned long getTickCount()
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getTickCount", "()J"))
		{
			jlong jData = 	t.env->CallStaticLongMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return (long)jData;
		}
		return 0;
	}

	void debugLog(const char* tag,const char* data)
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "debugLog", "(Ljava/lang/String;Ljava/lang/String;)V"))
		{
			jstring jTag = t.env->NewStringUTF(tag);
			jstring jData = t.env->NewStringUTF(data);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jTag, jData);
			t.env->DeleteLocalRef(jTag);
			t.env->DeleteLocalRef(jData);
			t.env->DeleteLocalRef(t.classID);
		}
	}


	void Java_org_cocos2dx_cpp_JniHelper_exitApp(JNIEnv *env, jobject thiz)
	{
		unity::CleanAndExit();
	}

	void Java_org_cocos2dx_cpp_JniHelper_stopBluetoothCheck(JNIEnv *env, jobject thiz)
	{
		//showTipDialog("test", "checkbluetoothtest", EXIT_DIALOG);
	}

	//添加蓝牙设备
	void Java_org_cocos2dx_cpp_JniHelper_addBluetoothPairedDevice(JNIEnv *env, jobject thiz, jstring name, jstring MAC)
	{
		auto layer = dynamic_cast<Bluetooth*>(CCDirector::getInstance()->getRunningScene()->getChildByTag(Bluetooth::eChild_BluetoothLayer));
		if (layer)
		{
			jboolean iscopy;
			const char* deviceName = env->GetStringUTFChars(name, &iscopy);
			const char* deviceMAC = env->GetStringUTFChars(MAC, &iscopy);
			layer->AddDevice(deviceName, deviceMAC);
			env->ReleaseStringUTFChars(name, deviceName);
			env->ReleaseStringUTFChars(MAC, deviceMAC);
		}
		else
		{
			showTipDialog("error", "not found layer", NO_BLUETOOTH_DIALOG);
		}
	}

	//接受蓝牙数据
	void Java_org_cocos2dx_cpp_JniHelper_getMessage(JNIEnv *env, jobject thiz, jstring jdata)
	{
		jboolean iscopy;
		//unity::Log(TAG, "begin conv");
		const char* data = env->GetStringUTFChars(jdata, &iscopy);
		//unity::Log(TAG, "data length:%d",strlen(data));
		char temp[MSG_LENGTH] = {0};
		memcpy(temp,data,MSG_LENGTH-1);
		temp[MSG_LENGTH-1] = '\0';
		//unity::Log(TAG, "temp : %s", temp);
		//处理数据
		g_Transform.Parse(temp);
		//unity::Log(TAG, "parse over");
// 		auto layer = dynamic_cast<Bluetooth*>(CCDirector::getInstance()->getRunningScene()->getChildByTag(Bluetooth::eChild_BluetoothLayer));
// 		if(layer)
// 		{
// 			layer->GetMessage(data);
// 		}
		env->ReleaseStringUTFChars(jdata, data);
	}

	//接受连接状态
	void Java_org_cocos2dx_cpp_JniHelper_informConnectionState(JNIEnv *env, jobject thiz, jint jstate)
	{
		int state = jstate;
		//根据状态处理
		auto layer = dynamic_cast<Bluetooth*>(CCDirector::getInstance()->getRunningScene()->getChildByTag(Bluetooth::eChild_BluetoothLayer));
		if(layer)
		{
			layer->CheckConnectionState(state);
		}
	}
}

#endif

