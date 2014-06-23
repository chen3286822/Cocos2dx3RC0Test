#include "BluetoothScene.h"
#include "Dialog.h"
#include "MainTitleScene.h"
#include "HelloWorldScene.h"
#include "JNIFunc.h"
#include "Unity.h"

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

	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(Bluetooth::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	m_vDevices.clear();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();


	//bluetooth check
	auto labelBluetooth = LabelTTF::create("Check Bluetooth", unity::GetDefaultFontType(), 25);
	labelBluetooth->setColor(Color3B(249, 246, 242));
	auto bluetoothItem = MenuItemLabel::create(labelBluetooth, CC_CALLBACK_1(Bluetooth::CheckBluetooth, this));
	bluetoothItem->setAnchorPoint(Point(0.5, 0));
	bluetoothItem->setPosition(Point(origin.x + visibleSize.width/2, origin.y));

	//begin game menu item
 	auto labelStart = LabelTTF::create("Restart Game", unity::GetDefaultFontType(), 25);
	labelStart->setColor(Color3B(249, 246, 242));
	auto startItem = MenuItemLabel::create(labelStart, CC_CALLBACK_1(Bluetooth::StartGame, this));
	startItem->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	startItem->setVisible(false);
	 
	auto menuBluetooth = Menu::create();
	menuBluetooth->addChild(bluetoothItem, 1, eChild_CheckBluetoothItem);
	menuBluetooth->addChild(startItem, 1, eChild_StartGameItem);
	menuBluetooth->setPosition(Point::ZERO);
	this->addChild(menuBluetooth, 1, eChild_Menu);

	auto showLabel = LabelTTF::create("Nothing", unity::GetDefaultFontType(), 25);
	showLabel->setAnchorPoint(Point(0.5, 1));
	showLabel->setPosition(Point(origin.x + visibleSize.width/2,origin.y + visibleSize.height - 40));
	this->addChild(showLabel, 1, eChild_ShowLabel);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	initBluetooth();
#endif

	return true;
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

void Bluetooth::StartGame(cocos2d::Ref* pSender)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//sendMessage("Hello!");
#endif
}

void Bluetooth::GetMessage(const char* data)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

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

	
#endif
}

void Bluetooth::CheckConnectionState(int state)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	switch (state)
	{
	case STATE_CONNECTED:
	{
							auto label = dynamic_cast<LabelTTF*>(getChildByTag(eChild_ShowLabel));
							if (label)
							{
								label->setString("Connected");
							}
							auto menu = dynamic_cast<Menu*>(getChildByTag(eChild_Menu));
							if (menu)
							{
								auto item = dynamic_cast<MenuItem*>(menu->getChildByTag(eChild_CheckBluetoothItem));
								if (item)
								{
									item->setVisible(false);
								}
							}
							//start game
							Director::getInstance()->replaceScene(HelloWorld::createScene(HelloWorld::eMode_Bluetooth));
	}
		break;
	case STATE_CONNECTING:
	{
							auto label = dynamic_cast<LabelTTF*>(getChildByTag(eChild_ShowLabel));
							if (label)
							{
								label->setString("Connecting");
							}
	}
		break;
	case STATE_NONE:
	{
							auto label = dynamic_cast<LabelTTF*>(getChildByTag(eChild_ShowLabel));
							if (label)
							{
								label->setString("Not connected");
							}
	}
		break;
	case STATE_LISTEN:
	{
					   auto label = dynamic_cast<LabelTTF*>(getChildByTag(eChild_ShowLabel));
					   if (label)
					   {
						   label->setString("Listening");
					   }
					   auto menu = dynamic_cast<Menu*>(getChildByTag(eChild_Menu));
					   if (menu)
					   {
						   auto item = dynamic_cast<MenuItem*>(menu->getChildByTag(eChild_CheckBluetoothItem));
						   if (item)
						   {
							   item->setVisible(true);
						   }
					   }
	}
		break;
	default:
		break;
	}
#endif
}

void Bluetooth::BackToMainTitle(cocos2d::Ref* pSender)
{
	Director::getInstance()->replaceScene(MainTitle::createScene());
}

//½ÓÊÜ·µ»Ø¼ü
void Bluetooth::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_BACKSPACE:
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		{
			//Director::getInstance()->end();
			showTipDialog("Exit", "Do you want to end such a wonderful game?", EXIT_DIALOG);
		}
#endif
	}
		break;
	case EventKeyboard::KeyCode::KEY_ESCAPE:
	{
											   unity::CleanAndExit();
	}
		break;
	default:
		break;
	}
}