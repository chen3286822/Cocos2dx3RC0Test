package org.cocos2dx.cpp;


import android.os.Handler;
import android.os.Message;

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
	public static native void stopBluetoothCheck();
	public static native void addBluetoothPairedDevice(final String name,final String MAC);
}
