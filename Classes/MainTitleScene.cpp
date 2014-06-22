#include "MainTitleScene.h"
#include "HelloWorldScene.h"
#include "BluetoothScene.h"
#include "Unity.h"

USING_NS_CC;

Scene* MainTitle::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainTitle::create();

	// add layer as a child to scene
	scene->addChild(layer, 0, eChild_MainTitleLayer);

	// return the scene
	return scene;
}

bool MainTitle::init()
{
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	//List
	auto labelSingleMode = LabelTTF::create("Single Mode", unity::GetDefaultFontType(), 25);
	auto itemSingleMode = MenuItemLabel::create(labelSingleMode, CC_CALLBACK_1(MainTitle::SingleMode,this));

	auto labelBluetoothMode = LabelTTF::create("Bluetooth Mode", unity::GetDefaultFontType(), 25);
	auto itemBluetoothMode = MenuItemLabel::create(labelBluetoothMode, CC_CALLBACK_1(MainTitle::BluetoothMode,this));

	auto labelExit = LabelTTF::create("Exit", unity::GetDefaultFontType(), 25);
	auto itemExit = MenuItemLabel::create(labelExit, CC_CALLBACK_1(MainTitle::Exit,this));

	auto menu = Menu::create(itemSingleMode, itemBluetoothMode, itemExit, NULL);
	menu->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	menu->alignItemsVerticallyWithPadding(25);
	this->addChild(menu, 1, eChild_Menu);
	return true;
}

void MainTitle::SingleMode(cocos2d::Ref* pSender)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene(HelloWorld::eMode_Single));
}

void MainTitle::BluetoothMode(cocos2d::Ref* pSender)
{
	Director::getInstance()->replaceScene(Bluetooth::createScene());
}

void MainTitle::Exit(cocos2d::Ref* pSender)
{
	Director::getInstance()->end();
}