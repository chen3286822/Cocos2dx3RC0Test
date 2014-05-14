#include "JNIFunc.h"
#include "cocos2d.h"
#include "BluetoothScene.h"

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
		}
	}

	//搜索蓝牙设备
	void searchBluetooth()
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "connectBluetooth", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
		}
	}

	//初始化蓝牙
	void initBluetooth()
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "initBluetooth", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
		}
	}

	//停止蓝牙连接
	void stopBluetooth()
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "stopBluetooth", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
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
		}
	}


	void Java_org_cocos2dx_cpp_JniHelper_exitApp(JNIEnv *env, jobject thiz)
	{
		CCDirector::getInstance()->end();
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
		const char* data = env->GetStringUTFChars(jdata, &iscopy);
		//处理数据
		env->ReleaseStringUTFChars(jdata, data);
	}

	//接受连接状态
	void Java_org_cocos2dx_cpp_JniHelper_informConnectionState(JNIEnv *env, jobject thiz, jint jstate)
	{
		int state = jstate;
		//根据状态处理
	}
}

#endif

