#include "MainTitleScene.h"
#include "HelloWorldScene.h"
#include "BluetoothScene.h"
#include "RankScene.h"
#include "Unity.h"

USING_NS_CC;

bool BgLayer::init()
{
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	for (auto i = 0; i < 4;i++)
	{
		m_pBg[i] = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("bg.png"), Rect(0, 0, visibleSize.width, visibleSize.height));
 		Texture2D::TexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
 		m_pBg[i]->getTexture()->setTexParameters(params);
		m_pBg[i]->setAnchorPoint(Point(0, 0));
		this->addChild(m_pBg[i], 0, eChild_Bg1 + i);
		m_nPos[i] = i;
	}

	m_pBg[0]->setPosition(Point(0, 0));
	m_pBg[1]->setPosition(Point(-visibleSize.width, 0));
	m_pBg[2]->setPosition(Point(0, -visibleSize.height));
	m_pBg[3]->setPosition(Point(-visibleSize.width, -visibleSize.height));


	unscheduleUpdate();
	scheduleUpdate();
	return true;
}

void BgLayer::update(float fDelta)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto xSpeed = fDelta * 20;
	auto ySpeed = fDelta * 30;
	
	auto yRightUp = m_pBg[m_nPos[0]]->getPositionY();
	yRightUp += ySpeed; 

	if (yRightUp >= visibleSize.height)
	{
		m_pBg[m_nPos[0]]->setPositionY(-visibleSize.height);
		m_pBg[m_nPos[1]]->setPositionY(-visibleSize.height);
		m_pBg[m_nPos[2]]->setPositionY(0);
		m_pBg[m_nPos[3]]->setPositionY(0);

		//ÐÞ¸Ä±àºÅÎ»ÖÃ
		auto temp = m_nPos[0];
		m_nPos[0] = m_nPos[2];
		m_nPos[2] = temp;
		temp = m_nPos[1];
		m_nPos[1] = m_nPos[3];
		m_nPos[3] = temp;
	}
	else
	{
		for (auto bg : m_pBg)
		{
			bg->setPositionY(bg->getPositionY() + ySpeed);
		}
	}

	auto xRightUp = m_pBg[m_nPos[0]]->getPositionX();
	xRightUp += xSpeed;

	if (xRightUp >= visibleSize.width)
	{
		m_pBg[m_nPos[0]]->setPositionX(-visibleSize.width);
		m_pBg[m_nPos[2]]->setPositionX(-visibleSize.width);
		m_pBg[m_nPos[1]]->setPositionX(0);
		m_pBg[m_nPos[3]]->setPositionX(0);

		//ÐÞ¸Ä±àºÅÎ»ÖÃ
		auto temp = m_nPos[0];
		m_nPos[0] = m_nPos[1];
		m_nPos[1] = temp;
		temp = m_nPos[2];
		m_nPos[2] = m_nPos[3];
		m_nPos[3] = temp;
	}
	else
	{
		for (auto bg : m_pBg)
		{
			bg->setPositionX(bg->getPositionX() + xSpeed);
		}
	}

}

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

	Director::getInstance()->getTextureCache()->addImage("bg.png");


	//background
	m_pBgLayer = BgLayer::create();
	m_pBgLayer->setPosition(origin.x, origin.y);
	this->addChild(m_pBgLayer, 0, eChild_Background);

	//List
	auto labelSingleMode = LabelTTF::create("Single Mode", unity::GetDefaultFontType(), 25);
	auto itemSingleMode = MenuItemLabel::create(labelSingleMode, CC_CALLBACK_1(MainTitle::SingleMode,this));

	auto labelBluetoothMode = LabelTTF::create("Bluetooth Mode", unity::GetDefaultFontType(), 25);
	auto itemBluetoothMode = MenuItemLabel::create(labelBluetoothMode, CC_CALLBACK_1(MainTitle::BluetoothMode,this));

	auto labelExit = LabelTTF::create("Exit", unity::GetDefaultFontType(), 25);
	auto itemExit = MenuItemLabel::create(labelExit, CC_CALLBACK_1(MainTitle::Exit,this));

	auto labelRank = LabelTTF::create("Rank", unity::GetDefaultFontType(), 25);
	auto itemRank = MenuItemLabel::create(labelRank, CC_CALLBACK_1(MainTitle::Rank, this));

	auto menu = Menu::create(itemSingleMode, itemBluetoothMode, itemRank, itemExit, NULL);
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
	unity::CleanAndExit();
}

void MainTitle::Rank(cocos2d::Ref* pSender)
{
	Director::getInstance()->replaceScene(RankLayer::createScene());
}