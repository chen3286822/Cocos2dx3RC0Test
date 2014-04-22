#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B::GRAY))
	{
		return false;
	}

	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	srand((unsigned int)time(NULL));

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	m_nShorter = std::min(visibleSize.width, visibleSize.height);
	m_nCardLength = (m_nShorter - m_nBorder * 5) / 4;
	m_nOffsetX = m_nBorder + origin.x;
	m_nOffsetY = (std::max(visibleSize.width, visibleSize.height) - m_nShorter) / 2 + origin.y;

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...
	Director::getInstance()->getTextureCache()->addImage("blank.png");

	//添加底图
	for (int i = 0; i < 16; i++)
	{
		auto x = i / 4;
		auto y = i % 4;
		auto sprite = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("blank.png"));
		sprite->setTextureRect(cocos2d::Rect(0, 0, m_nCardLength, m_nCardLength));
		sprite->setColor(Color3B::WHITE);
		sprite->setAnchorPoint(cocos2d::Point(0, 0));
		sprite->setPosition(cocos2d::Point(y*(m_nBorder + m_nCardLength) + m_nOffsetX, x*(m_nBorder + m_nCardLength) + m_nOffsetY));
		addChild(sprite);
	}


	//初始化2个卡片
	//m_lCards.clear();
	AddNewCard();
	AddNewCard();
	//AddNewNum();
	return true;
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_BACKSPACE:
	{
												  if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
												  {
													  Director::getInstance()->end();
												  }
	}
		break;
	case EventKeyboard::KeyCode::KEY_ESCAPE:
	{
											   Director::getInstance()->end();
	}
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	{
												  // AddNewCard();
												   MoveAndMergeCard(keycode);
	}
	default:
		break;
	}



}

void HelloWorld::MoveAndMergeCard(EventKeyboard::KeyCode dir)
{
	if (dir == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		int first[] = { 2, 1, 0 };
		for (auto index : first)
		{
			for (auto i = 0; i < 4;i++)
			{
				Card* card = m_iCardPark[index][i].m_pCard;
				if (card)
				{
					//移动合并算法
				}
			}			
		}
		
	}
}

Card* HelloWorld::FindCard(int x, int y)
{
	if (m_iCardPark[x][y].m_pCard)
		return m_iCardPark[x][y].m_pCard;

// 	for (auto card : m_lCards)
// 	{
// 		cocos2d::Point& pt = card->GetPos();
// 		if ((int)pt.x == x && (int)pt.y == y)
// 			return card;
// 	}
	return nullptr;
}

void HelloWorld::AddNewCard()
{
	//计算剩余空卡
	std::list<int> emptyCard;
	static int allNum[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	for (auto num : allNum)
	{
		emptyCard.push_back(num);
	}

	for (auto i = 0; i < 4; i ++)
	{
		for (auto j = 0; j < 4; j ++)
		{
			if (m_iCardPark[i][j].m_pCard)
			{
				cocos2d::Point& pt = m_iCardPark[i][j].m_pCard->GetPos();
				emptyCard.remove(int(pt.x * 4 + pt.y));
			}
		}	
	}
	
// 	for (auto card : m_lCards)
// 	{
// 		cocos2d::Point& pt = card->GetPos();
// 		emptyCard.remove(int(pt.x * 4 + pt.y));
// 	}

	if (emptyCard.size() < 1)
	{
		//没有空间，失败
	}
	//随机选2个放置数字
	auto index = rand() % emptyCard.size();

	auto is2 = (rand() % 4) >= 1;
	auto start = 0;
	for (auto newCard : emptyCard)
	{
		if (start == index)
		{
			//计算卡片长宽以及位置
			int x = newCard / 4;
			int y = newCard % 4;
			Card* card = Card::create(is2 ? 2 : 4, m_nCardLength);
			addChild(card);
			card->setPosition(cocos2d::Point(y*(m_nBorder + m_nCardLength) + m_nOffsetX, x*(m_nBorder + m_nCardLength) + m_nOffsetY));
			card->GetPos().x = x;
			card->GetPos().y = y;
			m_iCardPark[x][y].m_pCard = card;
			//m_lCards.push_back(card);
			break;
		}
		start++;
	}


}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
