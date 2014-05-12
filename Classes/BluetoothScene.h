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

class Bluetooth : public cocos2d::LayerColor, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
	enum eChild
	{
		eChild_BluetoothLayer,
		eChild_DeviceListView,
		eChild_CheckBluetoothItem,
	};

	static cocos2d::Scene* createScene();

	virtual bool init();
	void BackToMainTitle(cocos2d::Ref* pSender);

	//bluetooth
	void CheckBluetooth(cocos2d::Ref* pSender);
	void AddDevice(std::string name, std::string MAC);

	// implement the "static create()" method manually
	CREATE_FUNC(Bluetooth);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);

private:
	VDeviceMAC m_vDevices;
};

#endif