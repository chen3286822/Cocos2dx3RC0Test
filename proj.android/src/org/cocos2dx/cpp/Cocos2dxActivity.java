package org.cocos2dx.cpp;


import org.cocos2dx.cpp.DeviceListActivity;
import org.cocos2dx.cpp.BluetoothConnectionService;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.NativeActivity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
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
			}
				break;
			case CONNECT_BLUETOOTH:
			{
				if (!mBluetoothAdapter.isEnabled())
				{
					ToastMsg("Bluetooth is disabled now!",Toast.LENGTH_SHORT);
					
					Message msgEnableBT = handler.obtainMessage();
					msgEnableBT.what = Cocos2dxActivity.ASK_ENABLE_BLUETOOTH;
					msgEnableBT.sendToTarget();
					return;
				}
				
				if (mConnecttService == null)
				{
					mConnecttService = new BluetoothConnectionService(Cocos2dxActivity.this, handler);
		            // Only if the state is STATE_NONE, do we know that we haven't started already
		            if (mConnecttService.getState() == BluetoothConnectionService.STATE_NONE) {
		              // Start the Bluetooth chat services
		            	mConnecttService.start();
		            }
				}
				
				// Launch the DeviceListActivity to see devices and do scan
				Intent serverIntent = new Intent(Cocos2dxActivity.this,DeviceListActivity.class);				
	            startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE_SECURE);
			}
				break;
			case STOP_BLUETOOTH:
			{
				if (mConnecttService != null) 
				{
					mConnecttService.stop();
					mConnecttService = null;
				}
				
			}
				break;
			case MESSAGE_STATE_CHANGE:
                if(D) Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
                
                //inform the connection state
                JniHelper.informConnectionState(BluetoothConnectionService.STATE_CONNECTED);
                
                switch (msg.arg1) {
                case BluetoothConnectionService.STATE_CONNECTED:           	
                    break;
                case BluetoothConnectionService.STATE_CONNECTING:
                    //setStatus(R.string.title_connecting);
                    break;
                case BluetoothConnectionService.STATE_LISTEN:
                case BluetoothConnectionService.STATE_NONE:
                    //setStatus(R.string.title_not_connected);
                    break;
                }
                break;
            case MESSAGE_WRITE:
                byte[] writeBuf = (byte[]) msg.obj;
                // construct a string from the buffer
                String writeMessage = new String(writeBuf);
                //mConversationArrayAdapter.add("Me:  " + writeMessage);
                break;
            case MESSAGE_READ:
                byte[] readBuf = (byte[]) msg.obj;
                // construct a string from the valid bytes in the buffer
                String readMessage = new String(readBuf, 0, msg.arg1);
                JniHelper.getMessage(readMessage);
                break;
            case MESSAGE_DEVICE_NAME:
                // save the connected device's name
                mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
                Toast.makeText(getApplicationContext(), "Connected to "
                               + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                break;
            case MESSAGE_TOAST:
                Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                               Toast.LENGTH_SHORT).show();
                break;
            case SEND_MESSAGE:
            	byte[] sendBuf = (byte[]) msg.obj;
            	sendMsg(sendBuf);
            	break;
			}
		}
	};
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		
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
        mConnecttService.connect(device, secure);
	}
	
	private void sendMsg(String message)
	{
        // Check that we're actually connected before trying anything
        if (mConnecttService.getState() != BluetoothConnectionService.STATE_CONNECTED) {
            Toast.makeText(this, "Not connect yet", Toast.LENGTH_SHORT).show();
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the BluetoothChatService to write
            byte[] send = message.getBytes();
            mConnecttService.write(send);
        }
	}
	
	private void sendMsg(byte[] message)
	{
        // Check that we're actually connected before trying anything
        if (mConnecttService.getState() != BluetoothConnectionService.STATE_CONNECTED) {
            Toast.makeText(this, "Not connect yet", Toast.LENGTH_SHORT).show();
            return;
        }
        
        mConnecttService.write(message);
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
    // Message types sent from the BluetoothConnectionService Handler
    public static final int MESSAGE_STATE_CHANGE = 0x006;
    public static final int MESSAGE_READ = 0x007;
    public static final int MESSAGE_WRITE = 0x008;
    public static final int MESSAGE_DEVICE_NAME = 0x009;
    public static final int MESSAGE_TOAST = 0x0010;
	//more msg
    public static final int STOP_BLUETOOTH = 0x0011;
    public static final int SEND_MESSAGE = 0x0012;
    
	//msg for bluetooth state
	public static final int REQUEST_ENABLE_BLUETOOTH = 0x01;
	private static final int REQUEST_CONNECT_DEVICE_SECURE = 0x02;
	   
    // Key names received from the BluetoothConnectionService Handler
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";
	
	//bluetooth
	private boolean mSearching = false;
	private BluetoothAdapter mBluetoothAdapter = null;
	// Member object for the connection services
    private BluetoothConnectionService mConnecttService = null;
    
    // Name of the connected device
    private String mConnectedDeviceName = null;
    
    // Debugging
    private static final String TAG = "2048";
    private static final boolean D = true;
}
