#include "HelloWorldScene.h"
#include "Dialog.h"

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
	if (!LayerColor::initWithColor(Color4B(184,175,158,255)))
	{
		return false;
	}

	m_nHighScore = UserDefault::getInstance()->getIntegerForKey("Score", 0);

	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// Register Touch Event
	auto listener2 = EventListenerTouchOneByOne::create();
	listener2->setSwallowTouches(true);
	listener2->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener2->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener2->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);

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

	auto labelRestart = LabelTTF::create("New Game", "Arial", 25);
	labelRestart->setColor(Color3B(249,246,242));
	auto restartItem = MenuItemLabel::create(labelRestart, CC_CALLBACK_1(HelloWorld::Restart, this));
	restartItem->setAnchorPoint(Point(1, 0));
	restartItem->setPosition(Point(m_nOffsetX + 4 * m_nRectLength - m_nBorder, m_nOffsetY + 4 * m_nRectLength + m_nBorder));

	auto menu = Menu::create(closeItem, restartItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);

	auto label = LabelTTF::create("", "Arial", m_nCardLength / 4);
	char temp[20];
	sprintf(temp, "Score:");
	label->setString(temp);
	label->setColor(Color3B::WHITE);
	label->setAnchorPoint(cocos2d::Point(0, 0));
	label->setPosition(cocos2d::Point(m_nOffsetX, 4 * m_nRectLength + m_nOffsetY + m_nBorder));
	addChild(label);

	auto labelPt = LabelTTF::create("", "Arial", m_nCardLength / 4);
	sprintf(temp, "%d", m_nPoint);
	labelPt->setString(temp);
	labelPt->setColor(Color3B::WHITE);
	labelPt->setAnchorPoint(cocos2d::Point(0, 0));
	labelPt->setPosition(cocos2d::Point(m_nOffsetX + label->getContentSize().width , 4 * m_nRectLength + m_nOffsetY + m_nBorder));
	addChild(labelPt,1,eChild_Point);

	Director::getInstance()->getTextureCache()->addImage("blank.png");

	//添加底图
	for (int i = 0; i < 16; i++)
	{
		auto x = i / 4;
		auto y = i % 4;
		auto sprite = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("blank.png"));
		sprite->setTextureRect(cocos2d::Rect(0, 0, m_nCardLength, m_nCardLength));
		sprite->setColor(Color3B(204,192,178));
		sprite->setAnchorPoint(cocos2d::Point(0, 0));
		sprite->setPosition(cocos2d::Point(y*m_nRectLength + m_nOffsetX, x*m_nRectLength + m_nOffsetY));
		addChild(sprite,1);

// 		auto label = LabelTTF::create("", "Arial", m_nCardLength / 2);
// 		char temp[20];
// 		sprintf(temp, "%d", i);
// 		label->setString(temp);
// 		label->setColor(Color3B::BLUE);
// 		label->setPosition(cocos2d::Point(y*m_nRectLength + m_nOffsetX + m_nCardLength / 2, x*m_nRectLength + m_nOffsetY + m_nCardLength / 2));
// 		addChild(label);
	}


	//初始化2个卡片
  	AddNewCard();
 	AddNewCard();

// 	auto x = 0, y = 2;
// 	Card* card = Card::create(4, m_nCardLength);
// 	addChild(card);
// 	card->setPosition(cocos2d::Point(y*(m_nBorder + m_nCardLength) + m_nOffsetX, x*(m_nBorder + m_nCardLength) + m_nOffsetY));
// 	card->GetPos().x = x;
// 	card->GetPos().y = y;
// 	m_iCardPark[x][y].m_pCard = card;
// 	m_iCardPark[x][y].m_iMovePos = cocos2d::Point(card->GetPos());
// 	x = 1;
// 	y = 2;
// 	card = Card::create(2, m_nCardLength);
// 	addChild(card);
// 	card->setPosition(cocos2d::Point(y*(m_nBorder + m_nCardLength) + m_nOffsetX, x*(m_nBorder + m_nCardLength) + m_nOffsetY));
// 	card->GetPos().x = x;
// 	card->GetPos().y = y;
// 	m_iCardPark[x][y].m_pCard = card;
// 	m_iCardPark[x][y].m_iMovePos = cocos2d::Point(card->GetPos());
	auto dialog = Dialog::create();
	dialog->SetTitle("Game Over");
	//char temp[255];
	sprintf(temp, "You got %d points!", m_nPoint);
	dialog->SetContent(temp);
	dialog->AddButton("Restart", CC_CALLBACK_1(HelloWorld::Restart, this));
	addChild(dialog,2);
	return true;
}

void HelloWorld::Restart(cocos2d::Ref* pSender)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
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
												   for (auto i = 0; i < 16; i++)
												   {
													   if (m_iCardPark[i / 4][i % 4].m_pCard)
													   {
														   if (m_iCardPark[i / 4][i % 4].m_bMoving)
														   {
															   return;
														   }
													   }
												   }
												   MoveAndMergeCard(keycode);
	}
	default:
		break;
	}



}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	m_iStartPt = touch->getLocation();
	return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{

}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	Point endPt = touch->getLocation();
	if (abs(m_iStartPt.x - endPt.x) > abs(m_iStartPt.y - endPt.y) && abs(m_iStartPt.x - endPt.x) > 50)
	{
		if (m_iStartPt.x > endPt.x)
			MoveAndMergeCard(EventKeyboard::KeyCode::KEY_LEFT_ARROW);
		else 
			MoveAndMergeCard(EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
	}
	else if (abs(m_iStartPt.x - endPt.x) <= abs(m_iStartPt.y - endPt.y) && abs(m_iStartPt.y - endPt.y) > 50)
	{
		if (m_iStartPt.y > endPt.y)
			MoveAndMergeCard(EventKeyboard::KeyCode::KEY_DOWN_ARROW);
		else
			MoveAndMergeCard(EventKeyboard::KeyCode::KEY_UP_ARROW);
	}
	
}

void HelloWorld::MoveAction(int x, int y,cocos2d::EventKeyboard::KeyCode dir)
{
	auto& moveCard = m_iCardPark[x][y];
	if (moveCard.m_pCard)
	{
		//没有位置移动则不会进行move动作
		if ((int)moveCard.m_iMovePos.x == (int)moveCard.m_pCard->GetPos().x && (int)moveCard.m_iMovePos.y == (int)moveCard.m_pCard->GetPos().y)
			return;
		auto doneAction = CallFunc::create(CC_CALLBACK_0(HelloWorld::RemoveMergedCardAndDoubleNum, this, x, y,dir));
		auto moveAction = MoveTo::create(0.15f, cocos2d::Point(m_nRectLength*moveCard.m_iMovePos.y + m_nOffsetX, m_nRectLength*moveCard.m_iMovePos.x + m_nOffsetY));
		auto sequenceAction = Sequence::create(moveAction, doneAction, NULL);
		moveCard.m_pCard->runAction(sequenceAction);
		moveCard.m_bMoving = true;
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
							MoveAction(index2, index1,dir);
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
								MoveAction(index2, index1, dir);
							}
							else
							{
								if ((dir == EventKeyboard::KeyCode::KEY_RIGHT_ARROW && (int)(formerCard.m_iMovePos.y - 1) != (int)card->GetPos().y) ||
									(dir == EventKeyboard::KeyCode::KEY_LEFT_ARROW && (int)(formerCard.m_iMovePos.y + 1) != (int)card->GetPos().y) ||
									(dir == EventKeyboard::KeyCode::KEY_UP_ARROW && (int)(formerCard.m_iMovePos.x - 1) != (int)card->GetPos().x) ||
									(dir == EventKeyboard::KeyCode::KEY_DOWN_ARROW && (int)(formerCard.m_iMovePos.x + 1) != (int)card->GetPos().x))
								{
									moveCard.m_iMovePos = formerCard.m_iMovePos + minusPoint();
									MoveAction(index2, index1, dir);
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
					
					MoveAction(index2, index1, dir);
				}
			}
		}			
	}

}


void HelloWorld::RemoveMergedCardAndDoubleNum(int x, int y, EventKeyboard::KeyCode dir)
{
	if (m_iCardPark[x][y].m_pCard)
	{
		m_iCardPark[x][y].m_bMoving = false;
	}

	//当所有卡片都结束移动后再修正卡片位置
	for (auto i = 0; i < 16; i++)
	{
		if (m_iCardPark[i/4][i%4].m_pCard)
		{
			if (m_iCardPark[i / 4][i % 4].m_bMoving)
			{
				return;
			}
		}	
	}
	
	int bChecked[4][4];
	memset(bChecked, 0, sizeof(int)* 4 * 4);
	std::vector<int> first;
	std::vector<int> second;
	if (dir == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		int firstCycle[] = { 3, 2, 1, 0 };
		int secondCycle[] = { 0, 1, 2, 3 };
		first.assign(firstCycle, firstCycle + 4);
		second.assign(secondCycle, secondCycle + 4);
	}
	else if (dir == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		int firstCycle[] = { 0, 1, 2, 3 };
		int secondCycle[] = { 0, 1, 2, 3 };
		first.assign(firstCycle, firstCycle + 4);
		second.assign(secondCycle, secondCycle + 4);
	}
	else if (dir == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		int firstCycle[] = { 0, 1, 2, 3 };
		int secondCycle[] = { 3, 2, 1, 0 };
		first.assign(firstCycle, firstCycle + 4);
		second.assign(secondCycle, secondCycle + 4);
	}
	else if (dir == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		int firstCycle[] = { 0, 1, 2, 3 };
		int secondCycle[] = { 0, 1, 2, 3, };
		first.assign(firstCycle, firstCycle + 4);
		second.assign(secondCycle, secondCycle + 4);
	}
	for (auto index1 : first)
	{
		for (auto index2 : second)
		{
			auto& moveCard = m_iCardPark[index2][index1];
			if (!moveCard.m_pCard || bChecked[index2][index1] == 1 ||
				((int)moveCard.m_iMovePos.x == (int)moveCard.m_pCard->GetPos().x && (int)moveCard.m_iMovePos.y == (int)moveCard.m_pCard->GetPos().y && !moveCard.m_bGoingMerge)	//当卡片不会合并且目标位置不变时，也不用检查
				)
				continue;

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
				movedCard.m_pCard->GetPos() = movedCard.m_iMovePos;

				if (moveCard.m_nTag == 2)
				{				
					movedCard.m_pCard->setNum(movedCard.m_pCard->getNum() * 2,false);		
					AddPoint(movedCard.m_pCard->getNum());
				}

				moveCard.m_bGoingMerge = false;
				moveCard.m_nTag = 0;
				moveCard.m_iMovePos = cocos2d::Point(0, 0);
				moveCard.m_pCard = nullptr;

				//标记该卡片已经被检查过了，防止卡片移动到另一个位置，在for循环中又被检查到
				bChecked[(int)movedCard.m_iMovePos.x][(int)movedCard.m_iMovePos.y] = 1;
			}		
		}
		
	}
	
	AddNewCard();
}

void HelloWorld::AddPoint(int pt)
{
	m_nPoint += pt;
	auto label = dynamic_cast<LabelTTF*>(getChildByTag(eChild_Point));
	if (label)
	{
		char temp[20];
		sprintf(temp, "%d", m_nPoint);
		label->setString(temp);

		//先恢复默认scale，否则快速加分，会导致之前scale没有恢复，但是又执行放大动作，导致分数越来越大
		label->stopAllActions();
		label->setScale(1.0f);

		float _originalScale = label->getScale();
		auto zoomAction1 = ScaleTo::create(0.2f, _originalScale * 1.4f);
		auto zoomAction2 = ScaleTo::create(0.2f, _originalScale);
		auto sequenceAction = Sequence::create(zoomAction1, zoomAction2, NULL);
		label->runAction(sequenceAction);
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
		return;
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
			addChild(card,2);
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

	//检测是否无路可走
	//没有空位时才会检测
	if (emptyCard.size() -1 <= 0)
		CheckFailure();
}

void HelloWorld::CheckFailure()
{
	//检查所有格子的上下左右，如果存在数字和自己一样的，则不会失败
	for (auto i = 0; i < 4; i ++)
	{
		for (auto j = 0; j < 4; j ++)
		{
			MoveCard& moveCard = m_iCardPark[i][j];
			int x[4] = { -1, 1, 0, 0 };
			int y[4] = { 0, 0, 1, -1 };
			for (auto index = 0; index < 4; index++)
			{
				auto x1 = x[index] + i;
				auto y1 = y[index] + j;
				if (x1 < 0 || x1 > 3 || y1 < 0 || y1 > 3)
					continue;
				MoveCard& nerghbourCard = m_iCardPark[x1][y1];
				if (moveCard.m_pCard->getNum() == nerghbourCard.m_pCard->getNum())
					return;
			}		
		}		
	}
	//游戏失败
	CCLOG("Game Over!");
	auto dialog = Dialog::create();
	dialog->SetTitle("Game Over");
	char temp[255];
	if (m_nPoint > m_nHighScore)
	{
		sprintf(temp, "A new record! You got %d points!", m_nPoint);
		m_nHighScore = m_nPoint;
		UserDefault::getInstance()->setIntegerForKey("Score", m_nHighScore);
	}
	else
	{
		sprintf(temp, "You got %d points!\nBest record is %dpts.", m_nPoint,m_nHighScore);
	}
	
	dialog->SetContent(temp);
	dialog->AddButton("Restart", CC_CALLBACK_1(HelloWorld::Restart, this));
	addChild(dialog,3);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
