package org.cocos2dx.cpp;

import java.util.Set;

import org.cocos2dx.cpp.DeviceListActivity;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.NativeActivity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.Toast;

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
			case INIT_BLUETOOTH:
			{
				if(mSearching)
				{
					ToastMsg("Bluetooth is searching now!",Toast.LENGTH_SHORT);
					return;
				}
				//check bluetooth
				mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
				//test
				//mBluetoothAdapter = null;
				if(mBluetoothAdapter == null)
				{
					// Device does not support Bluetooth
					JniHelper.showTipDialog("Error","No bluetooth available!",Cocos2dxActivity.NO_BLUETOOTH_DIALOG);
					return;
				}
				if (!mBluetoothAdapter.isEnabled()) {
					ToastMsg("Bluetooth is disabled now!",Toast.LENGTH_SHORT);
					
					Message msgEnableBT = handler.obtainMessage();
					msgEnableBT.what = Cocos2dxActivity.ASK_ENABLE_BLUETOOTH;
					msgEnableBT.sendToTarget();
					return;
				}
				//cancel searching first
				//mBluetoothAdapter.cancelDiscovery();
				//get devices already paired first
//				Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
//				// If there are paired devices
//				if (pairedDevices.size() > 0) {
//				    // Loop through paired devices
//				    for (BluetoothDevice device : pairedDevices) {
//				    	JniHelper.addBluetoothPairedDevice(device.getName(),device.getAddress());
//				    }
//				}
//				else
//				{
//					ToastMsg("No bonded devices!",Toast.LENGTH_SHORT);
//				}
//				mBluetoothAdapter.startDiscovery();
			}
				break;
			case CONNECT_BLUETOOTH:
			{
				// Launch the DeviceListActivity to see devices and do scan
				Intent serverIntent = new Intent(Cocos2dxActivity.this,DeviceListActivity.class);				
	            startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE_SECURE);
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
		
		// Register for broadcasts when a device is discovered and discovery has finished
//		IntentFilter filter = new IntentFilter();
//		filter.addAction(BluetoothDevice.ACTION_FOUND);
//		filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
//		filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
//		this.registerReceiver(mReceiver, filter);
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
	
	private void connectDevice(Intent data, boolean secure)
	{
        // Get the device MAC address
        String address = data.getExtras()
            .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
        // Get the BluetoothDevice object
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        // Attempt to connect to the device
        //mChatService.connect(device, secure);
	}
	
	@Override
    protected void onDestroy() {
        super.onDestroy();
        
        // Unregister broadcast listeners
       // this.unregisterReceiver(mReceiver);
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
        case REQUEST_CONNECT_DEVICE_SECURE:
            // When DeviceListActivity returns with a device to connect
            if (resultCode == Activity.RESULT_OK) {
                connectDevice(data, true);
            }
            break;
		}
	}
	
	protected void ToastMsg(final String text,final int during)
	{
		Context context = getApplicationContext();
		CharSequence ctext = text;
		int duration = Toast.LENGTH_SHORT;

		Toast toast = Toast.makeText(context, ctext, duration);
		toast.show();
	}
	
	//msg for communicate from c++ to java
	public static final int EXIT_DIALOG = 0x001;
	public static final int NO_BLUETOOTH_DIALOG = 0x002;
	public static final int ASK_ENABLE_BLUETOOTH = 0x003;
	public static final int INIT_BLUETOOTH = 0x004;
	public static final int CONNECT_BLUETOOTH = 0x005;
	
	//msg for bluetooth state
	public static final int REQUEST_ENABLE_BLUETOOTH = 0x01;
	private static final int REQUEST_CONNECT_DEVICE_SECURE = 0x02;
	
	//bluetooth
	private boolean mSearching = false;
	private BluetoothAdapter mBluetoothAdapter = null;

}
