package org.cocos2dx.cpp;

import android.app.AlertDialog;
import android.app.NativeActivity;
import android.bluetooth.BluetoothAdapter;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

// The name of .so is specified in AndroidMenifest.xml. NativityActivity will load it automatically for you.
// You can use "System.loadLibrary()" to load other .so files.

public class Cocos2dxActivity extends NativeActivity{

	private Handler handler = new Handler()
	{
		@Override
		public void handleMessage(Message msg)
		{
			switch(msg.what)
			{
			case EXIT_DIALOG:
			{
				DialogMessage dm = (DialogMessage)msg.obj;
				new AlertDialog.Builder(Cocos2dxActivity.this)
				.setTitle(dm.title)
				.setMessage(dm.msg).setNegativeButton("cancle",
						new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
						dialog.dismiss();
					}
				})
				.setPositiveButton("Ok",
						new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
						dialog.dismiss();
						JniHelper.exitApp();
					}
				})
				.create().show();
			}
				break;
			case NO_BLUETOOTH_DIALOG:
			{
				DialogMessage dm = (DialogMessage)msg.obj;
				new AlertDialog.Builder(Cocos2dxActivity.this)
				.setTitle(dm.title)
				.setMessage(dm.msg)
				.setPositiveButton("Ok",
						new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
						dialog.dismiss();
						JniHelper.stopBluetoothCheck();
					}
				})
				.create().show();
			}
				break;
			case ASK_ENABLE_BLUETOOTH:
			{
			    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			    startActivityForResult(enableBtIntent, REQUEST_ENABLE_BLUETOOTH);
			}
			break;
			}
		}
	};
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		JniHelper.init(handler);
		
		//For supports translucency
		
		//1.change "attribs" in cocos\2d\platform\android\nativeactivity.cpp
		/*const EGLint attribs[] = {
	            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,  
	            //EGL_BLUE_SIZE, 5,   -->delete 
	            //EGL_GREEN_SIZE, 6,  -->delete 
	            //EGL_RED_SIZE, 5,    -->delete 
	            EGL_BUFFER_SIZE, 32,  //-->new field
	            EGL_DEPTH_SIZE, 16,
	            EGL_STENCIL_SIZE, 8,
	            EGL_NONE
	    };*/
		
		//2.Set the format of window
		// getWindow().setFormat(PixelFormat.TRANSLUCENT);
		
	}
	
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch (requestCode) {
		case REQUEST_ENABLE_BLUETOOTH:
		{
			if(resultCode == RESULT_OK)
			{
				//connect again
				JniHelper.connectBluetooth();
			}
			else if(resultCode == RESULT_CANCELED)
			{
				//finish connect
				JniHelper.showTipDialog("Error","No bluetooth available!",NO_BLUETOOTH_DIALOG);
			}
		}
			break;
		}
	}
	
	//msg for communicate from c++ to java
	public static final int EXIT_DIALOG = 0x001;
	public static final int NO_BLUETOOTH_DIALOG = 0x002;
	public static final int ASK_ENABLE_BLUETOOTH = 0x003;
	
	//msg for bluetooth state
	public static final int REQUEST_ENABLE_BLUETOOTH = 0x01;
}
