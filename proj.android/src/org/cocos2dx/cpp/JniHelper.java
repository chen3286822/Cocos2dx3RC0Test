package org.cocos2dx.cpp;


import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;

public class JniHelper {
	private static Handler mHandler;
	
	public static void init(Handler handler)
	{
		JniHelper.mHandler = handler;
	}
	
	public static native void exitApp();
	
	public static void showTipDialog(final String title,final String text,final int msgID)
	{
		 Message msg = mHandler.obtainMessage();
		 msg.what = msgID;

		 DialogMessage dm = new DialogMessage();
		 dm.title = title;
		 dm.msg = text;
		 msg.obj = dm;
		 msg.sendToTarget();
	}
	
	public static void connectBluetooth()
	{
		 Message msg = mHandler.obtainMessage();
		 msg.what = Cocos2dxActivity.CONNECT_BLUETOOTH;
		 msg.sendToTarget();
	}
	
	public static void initBluetooth()
	{
		Message msg = mHandler.obtainMessage();
		msg.what = Cocos2dxActivity.INIT_BLUETOOTH;
		msg.sendToTarget();
	}
	
	public static void stopBluetooth()
	{
		Message msg = mHandler.obtainMessage();
		msg.what = Cocos2dxActivity.STOP_BLUETOOTH;
		msg.sendToTarget();
	}
	
	public static void toastMsg(final String data)
	{
		Message msg = mHandler.obtainMessage();
		msg.what = Cocos2dxActivity.MESSAGE_TOAST;
		Bundle bundle = new Bundle();
        bundle.putString(Cocos2dxActivity.TOAST, data);
        msg.setData(bundle);
		msg.sendToTarget();
	}
	
	public static void sendMessage(final String data)
	{
		Message msg = mHandler.obtainMessage();
		msg.what = Cocos2dxActivity.SEND_MESSAGE;
		msg.obj = data;
		msg.sendToTarget();
		//getMessage(data);
	}
	
	public static long getTickCount()
	{
		return SystemClock.uptimeMillis();
	}
	
	public static void debugLog(final String tag,final String data)
	{
		Log.d(tag,data);
	}
	
	public static native void stopBluetoothCheck();
	public static native void addBluetoothPairedDevice(final String name,final String MAC);
	public static native void getMessage(final String data);
	public static native void informConnectionState(int state);
}
