#include "BluetoothScene.h"
#include "Dialog.h"
#include "MainTitleScene.h"
#include "JNIFunc.h"

USING_NS_CC;
USING_NS_CC_EXT;

Scene* Bluetooth::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Bluetooth::create();

	// add layer as a child to scene
	scene->addChild(layer, 0, eChild_BluetoothLayer);

	// return the scene
	return scene;
}

bool Bluetooth::init()
{
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}

	m_vDevices.clear();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	TableView* tableView = TableView::create(this, Size(visibleSize.width, visibleSize.height*7/8));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(Point(origin.x, origin.y + visibleSize.height/8));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tableView, 1, eChild_DeviceListView);
	tableView->reloadData();

	//bluetooth check
	auto labelBluetooth = LabelTTF::create("Check Bluetooth", "Arial", 25);
	labelBluetooth->setColor(Color3B(249, 246, 242));
	auto bluetoothItem = MenuItemLabel::create(labelBluetooth, CC_CALLBACK_1(Bluetooth::CheckBluetooth, this));
	bluetoothItem->setAnchorPoint(Point(0.5, 0));
	bluetoothItem->setPosition(Point(origin.x + visibleSize.width/2, origin.y));
	 
	auto menuBluetooth = Menu::create(bluetoothItem, NULL);
	menuBluetooth->setPosition(Point::ZERO);
	this->addChild(menuBluetooth, 1, eChild_CheckBluetoothItem);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	initBluetooth();
#endif
	return true;
}


void Bluetooth::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size Bluetooth::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height/8);
}

TableViewCell* Bluetooth::tableCellAtIndex(TableView *table, ssize_t idx)
{
 	if (idx >= m_vDevices.size())
 	{
 		return nullptr;
 	}
	
	auto string = String::createWithFormat("%s : %s", m_vDevices[idx].strName.c_str(),m_vDevices[idx].strMAC.c_str());
	TableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = new TableViewCell();
		cell->autorelease();
		auto sprite = Sprite::create("CloseSelected.png");
		sprite->setAnchorPoint(Point::ZERO);
		sprite->setPosition(Point(0, 0));
		cell->addChild(sprite);

		auto label = LabelTTF::create(string->getCString(), "Helvetica", 20.0);
		label->setPosition(Point::ZERO);
		label->setAnchorPoint(Point::ZERO);
		label->setTag(123);
		cell->addChild(label);
	}
	else
	{
		auto label = (LabelTTF*)cell->getChildByTag(123);
		label->setString(string->getCString());
	}


	return cell;
}

ssize_t Bluetooth::numberOfCellsInTableView(TableView *table)
{
	return m_vDevices.size();
}

void Bluetooth::CheckBluetooth(Ref* pSender)
{
	m_vDevices.clear();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	searchBluetooth();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto dialog = Dialog::create();
	dialog->SetTitle("Error");
	dialog->SetContent("Windows doesn't support Bluetooth!");
	dialog->AddButton("Restart", CC_CALLBACK_1(Bluetooth::BackToMainTitle, this));
	addChild(dialog, 2);
#endif
}


void Bluetooth::AddDevice(std::string name, std::string MAC)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (MAC.compare("FINISH") == 0)
	{
		showTipDialog("FINISH", "Bluetooth discovery finished!", NO_BLUETOOTH_DIALOG);
		return;
	}

	DeviceMAC device;
	device.strName = name;
	device.strMAC = MAC;
	m_vDevices.push_back(device);

	auto tableView = dynamic_cast<TableView*>(this->getChildByTag(eChild_DeviceListView));
	if (tableView)
	{
		tableView->reloadData();
	}
	
#endif
}

void Bluetooth::BackToMainTitle(cocos2d::Ref* pSender)
{
	Director::getInstance()->replaceScene(MainTitle::createScene());
}