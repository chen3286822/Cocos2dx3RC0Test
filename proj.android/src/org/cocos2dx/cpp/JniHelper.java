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
	
	private static void showTipDialog(final String title,final String text)
	{
		 Message msg = mHandler.obtainMessage();
		 msg.what = Cocos2dxActivity.SHOW_DIALOG;

		 DialogMessage dm = new DialogMessage();
		 dm.title = title;
		 dm.msg = text;
		 msg.obj = dm;
		 msg.sendToTarget();
	}
}
