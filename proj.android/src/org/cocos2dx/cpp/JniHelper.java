package org.cocos2dx.cpp;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;

public class JniHelper {
	private static final int REQUEST_ENABLE_BT = 1;
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
		//check bluetooth
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		//test
		//mBluetoothAdapter = null;
		if(mBluetoothAdapter == null)
		{
			// Device does not support Bluetooth
			showTipDialog("Error","No bluetooth available!",Cocos2dxActivity.NO_BLUETOOTH_DIALOG);
			return;
		}
		if (!mBluetoothAdapter.isEnabled()) {
			Message msg = mHandler.obtainMessage();
			msg.what = Cocos2dxActivity.ASK_ENABLE_BLUETOOTH;
			msg.sendToTarget();
			return;
		}
	}
	public static native void stopBluetoothCheck();
	
	private static BluetoothAdapter mBluetoothAdapter;
}
