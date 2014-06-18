#ifndef __BLUETOOTH_SCENE_H__
#define __BLUETOOTH_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

struct DeviceMAC
{
	std::string strName{ "" };
	std::string strMAC{ "" };
};
typedef std::vector<DeviceMAC> VDeviceMAC;

class Bluetooth : public cocos2d::LayerColor
{
public:
	enum eChild
	{
		eChild_BluetoothLayer,
		eChild_Menu,
		eChild_CheckBluetoothItem,
		eChild_StartGameItem,
		eChild_ShowLabel,
	};

	static cocos2d::Scene* createScene();

	virtual bool init();
	void BackToMainTitle(cocos2d::Ref* pSender);

	//½ÓÊÜ·µ»Ø¼ü
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	//bluetooth
	void CheckBluetooth(cocos2d::Ref* pSender);
	void AddDevice(std::string name, std::string MAC);
	void CheckConnectionState(int state);
	void StartGame(cocos2d::Ref* pSender);
	void GetMessage(const char* data);

	

	// implement the "static create()" method manually
	CREATE_FUNC(Bluetooth);

private:
	VDeviceMAC m_vDevices;
};

#endif