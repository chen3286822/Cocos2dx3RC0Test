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
	void Java_org_cocos2dx_cpp_JniHelper_exitApp(JNIEnv *env, jobject thiz)
	{
		CCDirector::getInstance()->end();
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

	void initBluetooth()
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "initBluetooth", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
		}
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
}

#endif

