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
	m_nRectLength = m_nBorder + m_nCardLength;
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
		sprite->setPosition(cocos2d::Point(y*m_nRectLength + m_nOffsetX, x*m_nRectLength + m_nOffsetY));
		addChild(sprite);

		auto label = LabelTTF::create("", "Arial", m_nCardLength / 2);
		char temp[20];
		sprintf(temp, "%d", i);
		label->setString(temp);
		label->setColor(Color3B::BLUE);
		label->setPosition(cocos2d::Point(y*m_nRectLength + m_nOffsetX + m_nCardLength / 2, x*m_nRectLength + m_nOffsetY + m_nCardLength / 2));
		addChild(label);
	}


	//初始化2个卡片
  	//AddNewCard();
 	//AddNewCard();

	auto x = 3, y = 0;
	Card* card = Card::create(4, m_nCardLength);
	addChild(card);
	card->setPosition(cocos2d::Point(y*(m_nBorder + m_nCardLength) + m_nOffsetX, x*(m_nBorder + m_nCardLength) + m_nOffsetY));
	card->GetPos().x = x;
	card->GetPos().y = y;
	m_iCardPark[x][y].m_pCard = card;
	m_iCardPark[x][y].m_iMovePos = cocos2d::Point(card->GetPos());
	y = 1;
	card = Card::create(2, m_nCardLength);
	addChild(card);
	card->setPosition(cocos2d::Point(y*(m_nBorder + m_nCardLength) + m_nOffsetX, x*(m_nBorder + m_nCardLength) + m_nOffsetY));
	card->GetPos().x = x;
	card->GetPos().y = y;
	m_iCardPark[x][y].m_pCard = card;
	m_iCardPark[x][y].m_iMovePos = cocos2d::Point(card->GetPos());
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

	auto condition = [=](int Lformer1, int Lindex1, int Lformer2, int Lindex2) -> bool
	{
		if (dir == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			return (Lindex1 + Lformer1 <= 3);
		else if (dir == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			return (Lindex1 + Lformer1 >= 0);
		else if (dir == EventKeyboard::KeyCode::KEY_UP_ARROW)
			return (Lindex2 + Lformer2 <= 3);
		else if (dir == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
			return (Lindex2 + Lformer2 >= 0);
		return false;
	};
	auto minusPoint = [=]() -> cocos2d::Point
	{
		if (dir == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			return cocos2d::Point(0, -1);
		else if (dir == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			return cocos2d::Point(0, 1);
		else if (dir == EventKeyboard::KeyCode::KEY_UP_ARROW)
			return cocos2d::Point(-1, 0);
		else if (dir == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
			return cocos2d::Point(1, 0);
		return cocos2d::Point(0, 0);
	};

	std::vector<int> first;
	std::vector<int> second;
	if (dir == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		int firstCycle[] = { 2, 1, 0 };
		int secondCycle[] = { 0, 1, 2, 3 };
		first.assign(firstCycle, firstCycle + 3);
		second.assign(secondCycle, secondCycle + 4);
	}
	else if (dir == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		int firstCycle[] = { 1, 2, 3 };
		int secondCycle[] = { 0, 1, 2, 3 };
		first.assign(firstCycle, firstCycle + 3);
		second.assign(secondCycle, secondCycle + 4);
	}
	else if (dir == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		int firstCycle[] = { 0, 1, 2, 3 };
		int secondCycle[] = { 2, 1, 0 };
		first.assign(firstCycle, firstCycle + 4);
		second.assign(secondCycle, secondCycle + 3);
	}
	else if (dir == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		int firstCycle[] = { 0, 1, 2, 3 };
		int secondCycle[] = { 1, 2, 3, };
		first.assign(firstCycle, firstCycle + 4);
		second.assign(secondCycle, secondCycle + 3);
	}
	for (auto index1 : first)
	{
		for (auto index2 : second)
		{			
			auto& moveCard = m_iCardPark[index2][index1];
			auto card = moveCard.m_pCard;
			if (card)
			{
				//移动合并算法
				auto former1 = 1, former2 = 0;
				if (dir == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
				{
					former1 = -1;
				}
				else if (dir == EventKeyboard::KeyCode::KEY_UP_ARROW)
				{
					former1 = 0;
					former2 = 1;
				}
				else if (dir == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
				{
					former1 = 0;
					former2 = -1;
				}


				while (condition(former1,index1,former2,index2))
				{
					//对于card之前的每个位置
					//有卡则进入情况1，无卡继续下一次循环
					//1、该卡是否合并中，是则进入a，否则进入b
					//a、观察其m_iMovePos，将card移动到m_iMovePos之前的一个点
					//b、观察其Num是否和card一致，是则将card移动到该位置，并且设置合并中，否则移动到之前的一个空位
					auto& formerCard = m_iCardPark[index2 + former2][index1 + former1];
					if (formerCard.m_pCard)
					{
						if (formerCard.m_bGoingMerge)
						{
							moveCard.m_iMovePos = formerCard.m_iMovePos + minusPoint();
							auto doneAction = CallFunc::create(CC_CALLBACK_0(HelloWorld::RemoveMergedCardAndDoubleNum, this,index2,index1));
							auto moveAction = MoveTo::create(0.5, cocos2d::Point(m_nRectLength*moveCard.m_iMovePos.y + m_nOffsetX, m_nRectLength*moveCard.m_iMovePos.x + m_nOffsetY));
							auto sequenceAction = Sequence::create(moveAction, doneAction, NULL);
							card->runAction(sequenceAction);
						}
						else
						{
							if (card->getNum() == formerCard.m_pCard->getNum())
							{
								moveCard.m_iMovePos = formerCard.m_iMovePos;
								moveCard.m_bGoingMerge = true;
								moveCard.m_nTag = 2;
								formerCard.m_bGoingMerge = true;
								formerCard.m_nTag = 1;
								auto doneAction = CallFunc::create(CC_CALLBACK_0(HelloWorld::RemoveMergedCardAndDoubleNum, this, index2, index1));
								auto moveAction = MoveTo::create(0.5, cocos2d::Point(m_nRectLength*moveCard.m_iMovePos.y + m_nOffsetX, m_nRectLength*moveCard.m_iMovePos.x + m_nOffsetY));
								auto sequenceAction = Sequence::create(moveAction, doneAction, NULL);
								card->runAction(sequenceAction);
							}
							else
							{
								if ((dir == EventKeyboard::KeyCode::KEY_RIGHT_ARROW && (int)(formerCard.m_iMovePos.y - 1) != (int)card->GetPos().y) ||
									(dir == EventKeyboard::KeyCode::KEY_LEFT_ARROW && (int)(formerCard.m_iMovePos.y + 1) != (int)card->GetPos().y) ||
									(dir == EventKeyboard::KeyCode::KEY_UP_ARROW && (int)(formerCard.m_iMovePos.x - 1) != (int)card->GetPos().x) ||
									(dir == EventKeyboard::KeyCode::KEY_DOWN_ARROW && (int)(formerCard.m_iMovePos.x + 1) != (int)card->GetPos().x))
								{
									moveCard.m_iMovePos = formerCard.m_iMovePos + minusPoint();
									auto doneAction = CallFunc::create(CC_CALLBACK_0(HelloWorld::RemoveMergedCardAndDoubleNum, this, index2, index1));
									auto moveAction = MoveTo::create(0.5, cocos2d::Point(m_nRectLength*moveCard.m_iMovePos.y + m_nOffsetX, m_nRectLength*moveCard.m_iMovePos.x + m_nOffsetY));
									auto sequenceAction = Sequence::create(moveAction, doneAction, NULL);
									card->runAction(sequenceAction);
								}
							}
						}
						break;
					}
					else
					{
						if (dir == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
							former1++;
						else if (dir == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
							former1--;
						else if (dir == EventKeyboard::KeyCode::KEY_UP_ARROW)
							former2++;
						else if (dir == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
							former2--;
					}
				}
				//card前一直没有卡，则直接移动到最前位置
				if (!condition(former1,index1,former2,index2))
				{
					if (dir == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
						moveCard.m_iMovePos = cocos2d::Point(index2, 3);
					else if (dir == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
						moveCard.m_iMovePos = cocos2d::Point(index2, 0);
					else if (dir == EventKeyboard::KeyCode::KEY_UP_ARROW)
						moveCard.m_iMovePos = cocos2d::Point(3, index1);
					else if (dir == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
						moveCard.m_iMovePos = cocos2d::Point(0, index1);
					
					auto doneAction = CallFunc::create(CC_CALLBACK_0(HelloWorld::RemoveMergedCardAndDoubleNum, this, index2, index1));
					auto moveAction = MoveTo::create(0.5, cocos2d::Point(m_nRectLength*moveCard.m_iMovePos.y + m_nOffsetX, m_nRectLength*moveCard.m_iMovePos.x + m_nOffsetY));
					auto sequenceAction = Sequence::create(moveAction, doneAction, NULL);
					card->runAction(sequenceAction);
				}
			}
		}			
	}

}


void HelloWorld::RemoveMergedCardAndDoubleNum(int x, int y)
{
	auto& moveCard = m_iCardPark[x][y];
	if (moveCard.m_nTag == 1)
	{
		moveCard.m_bGoingMerge = false;
		moveCard.m_nTag = 0;
		moveCard.m_iMovePos = cocos2d::Point(0, 0);
		this->removeChild(moveCard.m_pCard);
		moveCard.m_pCard = nullptr;
	}
	else if (moveCard.m_nTag == 2 || moveCard.m_nTag == 0)
	{
		auto& movedCard = m_iCardPark[(int)moveCard.m_iMovePos.x][(int)moveCard.m_iMovePos.y];
		movedCard.m_bGoingMerge = false;
		movedCard.m_nTag = 0;
		movedCard.m_iMovePos = moveCard.m_iMovePos;
		movedCard.m_pCard = moveCard.m_pCard;

		if (moveCard.m_nTag == 2)
			movedCard.m_pCard->setNum(movedCard.m_pCard->getNum() * 2);

		moveCard.m_bGoingMerge = false;
		moveCard.m_nTag = 0;
		moveCard.m_iMovePos = cocos2d::Point(0, 0);
		moveCard.m_pCard = nullptr;
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
			m_iCardPark[x][y].m_iMovePos = cocos2d::Point(card->GetPos());
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
