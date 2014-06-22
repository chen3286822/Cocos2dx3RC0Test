#include "HelloWorldScene.h"
#include "Dialog.h"
#include "JNIFunc.h"
#include "Unity.h"
#include "Transform.h"
#include "MainTitleScene.h"
#include "GameData.h"

USING_NS_CC;

CardRegion* CardRegion::create(int cardLength, bool bOther)
{
	CardRegion* cardRegion = new CardRegion();
	if (cardRegion && cardRegion->init(cardLength,bOther))
	{
		cardRegion->autorelease();
		return cardRegion;
	}

	CC_SAFE_DELETE(cardRegion);
	return nullptr;
}

bool CardRegion::init(int cardLength, bool bOther)
{
	m_bOther = bOther;
	m_vInitCards.clear();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	m_nCardLength = cardLength;
	m_nRectLength = m_nBorder + m_nCardLength;
	m_nShorter = std::min(visibleSize.width, visibleSize.height);

	//board
	auto spriteboard = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("roundedrectangleboard.png"));
	spriteboard->setScale((m_nShorter - 4) / spriteboard->getTextureRect().size.width, (m_nShorter - 4) / spriteboard->getTextureRect().size.height);
	spriteboard->setColor(Color3B(187, 173, 160));
	spriteboard->setAnchorPoint(cocos2d::Point(0, 0));
	spriteboard->setPosition(cocos2d::Point(0, 0));
	addChild(spriteboard, 0);

	//添加底图
	for (int i = 0; i < 16; i++)
	{
		auto x = i / 4;
		auto y = i % 4;
		auto sprite = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("roundedrectangle.png"));
		//sprite->setTextureRect(cocos2d::Rect(0, 0, m_nCardLength, m_nCardLength));
		sprite->setScale(m_nCardLength / sprite->getTextureRect().size.width, m_nCardLength / sprite->getTextureRect().size.height);
		sprite->setColor(Color3B(204, 192, 178));
		sprite->setAnchorPoint(cocos2d::Point(0, 0));
		sprite->setPosition(cocos2d::Point(m_nBorder + y*m_nRectLength, x*m_nRectLength + m_nBorder));
		addChild(sprite, 1);	
	}

	if (!m_bOther)
	{
		AddCard(true);
		AddCard(true);

		unscheduleUpdate();
		scheduleUpdate();
	}

	return true;
}

void CardRegion::update(float fDelta)
{
	if (g_Gamedata.getSceneInit())
	{
		//send init card information
		if (m_vInitCards.size() >= 2)
		{
			SimpleCard& card1 = m_vInitCards[0];
			SimpleCard& card2 = m_vInitCards[1];
			g_Transform.Send_Init_Card((int)card1.m_iPos.x,(int)card1.m_iPos.y,card1.m_nNum,(int)card2.m_iPos.x,(int)card2.m_iPos.y,card2.m_nNum);
		}

		g_Gamedata.setSceneInit(false);
		unscheduleUpdate();
	}
}

void CardRegion::AddCard(bool bInit)
{
	//计算剩余空卡
	std::list<int> emptyCard;
	static int allNum[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	for (auto num : allNum)
	{
		emptyCard.push_back(num);
	}

	for (auto i = 0; i < 4; i++)
	{
		for (auto j = 0; j < 4; j++)
		{
			if (m_iCardPark[i][j].m_pCard)
			{
				cocos2d::Point& pt = m_iCardPark[i][j].m_pCard->GetPos();
				emptyCard.remove(int(pt.x * 4 + pt.y));
			}
		}
	}

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
			addChild(card, 2);
			card->setPosition(cocos2d::Point(m_nBorder + y*(m_nBorder + m_nCardLength), x*(m_nBorder + m_nCardLength) + m_nBorder));
			card->GetPos().x = x;
			card->GetPos().y = y;
			m_iCardPark[x][y].m_pCard = card;
			m_iCardPark[x][y].m_iMovePos = cocos2d::Point(card->GetPos());
			
			if (bInit)
			{
				SimpleCard simpleCard;
				simpleCard.m_iPos = card->GetPos();
				simpleCard.m_nNum = card->getNum();
				m_vInitCards.push_back(simpleCard);
			}
			else if (!m_bOther)
			{
				//send card info to other
				g_Transform.Send_Add_Card((int)card->GetPos().x, (int)card->GetPos().y, card->getNum());
			}
			break;
		}
		start++;
	}

	//检测是否无路可走
	//没有空位时才会检测
	if (emptyCard.size() - 1 <= 0)
		CheckFailure();
}

void CardRegion::AddCard(int x, int y, int num)
{
	if (x < 0 || x >= 4 || y < 0 || y >= 4)
		return;
	if (m_iCardPark[x][y].m_pCard)
		return;
	Card* card = Card::create(num, m_nCardLength);
	addChild(card, 2);
	card->setPosition(cocos2d::Point(m_nBorder + y*(m_nBorder + m_nCardLength), x*(m_nBorder + m_nCardLength) + m_nBorder));
	card->GetPos().x = x;
	card->GetPos().y = y;
	m_iCardPark[x][y].m_pCard = card;
	m_iCardPark[x][y].m_iMovePos = cocos2d::Point(card->GetPos());
}

void CardRegion::CheckFailure()
{
	//检查所有格子的上下左右，如果存在数字和自己一样的，则不会失败
	for (auto i = 0; i < 4; i++)
	{
		for (auto j = 0; j < 4; j++)
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

	auto helloWorld = dynamic_cast<HelloWorld*>(getParent());
	if (helloWorld)
	{
		auto dialog = Dialog::create();
		dialog->SetTitle("Game Over");
		char temp[255];
		if (helloWorld->GetPoint() > helloWorld->GetHighPoint())
		{
			dialog->setNewRecord(true);
			sprintf(temp, "A new record!\nYou got %d points!", helloWorld->GetPoint());
			helloWorld->SetHighPoint(helloWorld->GetPoint());
			UserDefault::getInstance()->setIntegerForKey("Score", helloWorld->GetHighPoint());
		}
		else
		{
			sprintf(temp, "You got %d points!\nBest record is %dpts.", helloWorld->GetPoint(), helloWorld->GetHighPoint());
		}

		dialog->SetContent(temp);
		dialog->AddButton("Restart", CC_CALLBACK_1(HelloWorld::Restart, dynamic_cast<HelloWorld*>(getParent())));
		helloWorld->addChild(dialog, 3);
	}
}

void CardRegion::MoveAndMergeCard(EventKeyboard::KeyCode dir)
{
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

	if (!m_bOther)
		g_Transform.Send_Move((int)dir);

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


				while (condition(former1, index1, former2, index2))
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
							MoveAction(index2, index1, dir);
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
				if (!condition(former1, index1, former2, index2))
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

void CardRegion::MoveAction(int x, int y, cocos2d::EventKeyboard::KeyCode dir)
{
	auto& moveCard = m_iCardPark[x][y];
	if (moveCard.m_pCard)
	{
		//没有位置移动则不会进行move动作
		if ((int)moveCard.m_iMovePos.x == (int)moveCard.m_pCard->GetPos().x && (int)moveCard.m_iMovePos.y == (int)moveCard.m_pCard->GetPos().y)
			return;
		auto doneAction = CallFunc::create(CC_CALLBACK_0(CardRegion::RemoveMergedCardAndDoubleNum, this, x, y, dir));
		auto moveAction = MoveTo::create(0.15f, cocos2d::Point(m_nBorder + m_nRectLength*moveCard.m_iMovePos.y, m_nRectLength*moveCard.m_iMovePos.x + m_nBorder));
		auto sequenceAction = Sequence::create(moveAction, doneAction, NULL);
		moveCard.m_pCard->runAction(sequenceAction);
		moveCard.m_bMoving = true;
	}
}

void CardRegion::RemoveMergedCardAndDoubleNum(int x, int y, EventKeyboard::KeyCode dir)
{
	if (m_iCardPark[x][y].m_pCard)
	{
		m_iCardPark[x][y].m_bMoving = false;
	}

	//当所有卡片都结束移动后再修正卡片位置
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

	int bChecked[4][4];
	memset(bChecked, 0, sizeof(int) * 4 * 4);
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
					movedCard.m_pCard->setNum(movedCard.m_pCard->getNum() * 2, false);
					auto helloWorld = dynamic_cast<HelloWorld*>(getParent());
					if (helloWorld && !m_bOther)
						helloWorld->AddPoint(movedCard.m_pCard->getNum());
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

	if (!m_bOther)
		AddCard();
}

Card* CardRegion::FindCard(int x, int y)
{
	if (m_iCardPark[x][y].m_pCard)
		return m_iCardPark[x][y].m_pCard;
	return nullptr;
}

HelloWorld* HelloWorld::create(eMode mode)
{
	auto pRet = new HelloWorld();
	if (pRet && pRet->init(mode))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

Scene* HelloWorld::createScene(eMode mode)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create(mode);

	// add layer as a child to scene
	scene->addChild(layer, 0, eChild_HelloWorldLayer);

	// return the scene
	return scene;
}

HelloWorld::~HelloWorld()
{
	m_pKeyboardListener->release();
	_eventDispatcher->removeEventListener(m_pKeyboardListener);
	m_pTouchListener->release();
	_eventDispatcher->removeEventListener(m_pTouchListener);
}

// on "init" you need to initialize your instance
bool HelloWorld::init(eMode mode)
{
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(250, 248, 239, 255)))
	{
		return false;
	}

	m_eGameMode = mode;
	m_bGetStartInformed = false;
	m_dwStartTime = 0;
	m_nStartSeconds = 0;
	m_pKeyboardListener = nullptr;
	m_pTouchListener = nullptr;

	m_nHighScore = UserDefault::getInstance()->getIntegerForKey("Score", 0);

	m_pKeyboardListener = EventListenerKeyboard::create();
	m_pKeyboardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	m_pKeyboardListener->retain();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pKeyboardListener, this);

	// Register Touch Event
	m_pTouchListener = EventListenerTouchOneByOne::create();
	m_pTouchListener->setSwallowTouches(true);
	m_pTouchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	m_pTouchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	m_pTouchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	//if not retain, the listener will be autoreleased in bluetooth mode, because it has not been added to the dispathcher yet
	m_pTouchListener->retain();
	if (m_eGameMode == eMode_Single)
		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pTouchListener, this);


	srand((unsigned int)time(NULL));

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	m_nShorter = std::min(visibleSize.width, visibleSize.height);
	m_nLonger = std::max(visibleSize.width, visibleSize.height);
	m_nCardLength = (m_nShorter - m_nBorder * 5 - m_nCardRegionOffset * 2) / 4;
	m_nRectLength = m_nBorder*5 + m_nCardLength*4;
	
	
	int startX = 0, startY = 0;
	if (m_eGameMode == eMode_Single)
	{
		m_fScale = 1.0f;
		m_nOffsetX = m_nCardRegionOffset + origin.x;
		m_nOffsetY = (m_nLonger - m_nShorter) / 2 + origin.y;
		
	}
	else if (m_eGameMode == eMode_Bluetooth)
	{
		int nWidth = (m_nLonger - m_nStatusHeight - 2 * m_nCardRegionOffset) / 2;
		m_fScale = (float)nWidth/(float)m_nRectLength;
		m_nOffsetX = origin.x + (visibleSize.width - m_nRectLength*m_fScale)/2;
		m_nOffsetY = m_nCardRegionOffset + origin.y;
	}

	/////////////////////////////
	Director::getInstance()->getTextureCache()->addImage("roundedrectangle.png");
	Director::getInstance()->getTextureCache()->addImage("roundedrectangle2.png");
	Director::getInstance()->getTextureCache()->addImage("roundedrectangleboard.png");


	startX = m_nOffsetX;
	startY = m_nOffsetY + m_nRectLength*m_fScale + m_nBorder;
 	if (m_eGameMode == eMode_Single)
 	{
		//new game
		auto spriteNewGame = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("roundedrectangle2.png"));
		spriteNewGame->setScale(1.2f, 1);
		spriteNewGame->setColor(Color3B(143, 122, 102));
		spriteNewGame->setAnchorPoint(cocos2d::Point(1, 0));
		spriteNewGame->setPosition(cocos2d::Point(origin.x + visibleSize.width - m_nCardRegionOffset, startY));
		addChild(spriteNewGame, 1);

		auto labelRestart = LabelTTF::create("New Game", unity::GetDefaultFontType(), 20);
		labelRestart->setColor(Color3B(249, 246, 242));
		auto restartItem = MenuItemLabel::create(labelRestart, CC_CALLBACK_1(HelloWorld::Restart, this));
		restartItem->setAnchorPoint(Point(0.5, 0.5));
		restartItem->setPosition(Point(spriteNewGame->getPosition().x - spriteNewGame->getTextureRect().size.width * spriteNewGame->getScaleX() / 2, spriteNewGame->getPosition().y + spriteNewGame->getTextureRect().size.height * spriteNewGame->getScaleY() / 2));

		auto menu = Menu::create(restartItem, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu, 2);
	}


	//score
	auto spriteScore = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("roundedrectangle2.png"));
	spriteScore->setScale(1.2f, 1);
	spriteScore->setColor(Color3B(187, 173, 160));
	spriteScore->setAnchorPoint(cocos2d::Point(0, 0));
	spriteScore->setPosition(cocos2d::Point(startX, startY));
	addChild(spriteScore, 1);

	auto label = LabelTTF::create("SCORE", unity::GetDefaultFontType(), 18);
	label->setColor(Color3B(238, 228, 218));
	label->setPosition(cocos2d::Point(startX + spriteScore->getTextureRect().size.width*spriteScore->getScaleX() / 2, startY + spriteScore->getTextureRect().size.height*spriteScore->getScaleY() * 3 / 4));
	addChild(label, 2);

	auto labelPt = LabelTTF::create("", unity::GetDefaultFontType(), m_nCardLength / 4);
	char temp[20];
	sprintf(temp, "%d", m_nPoint);
	labelPt->setString(temp);
	labelPt->setColor(Color3B::WHITE);
	labelPt->setAnchorPoint(cocos2d::Point(0.5, 0));
	labelPt->setPosition(cocos2d::Point(startX + spriteScore->getTextureRect().size.width*spriteScore->getScaleX() / 2, startY));
	addChild(labelPt, 2, eChild_Point);

	if (m_eGameMode == eMode_Single)
	{
		//best
		startY += (spriteScore->getTextureRect().size.height * spriteScore->getScaleY() + 5);
		auto spriteBest = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("roundedrectangle2.png"));
		spriteBest->setScale(1.2f, 1);
		spriteBest->setColor(Color3B(187, 173, 160));
		spriteBest->setAnchorPoint(cocos2d::Point(0, 0));
		spriteBest->setPosition(cocos2d::Point(startX, startY));
		addChild(spriteBest, 1);

		label = LabelTTF::create("BEST", unity::GetDefaultFontType(), 18);
		label->setColor(Color3B(238, 228, 218));
		label->setPosition(cocos2d::Point(startX + spriteBest->getTextureRect().size.width*spriteBest->getScaleX() / 2, startY + spriteBest->getTextureRect().size.height*spriteBest->getScaleY() * 3 / 4));
		addChild(label, 2);

		auto labelHighPt = LabelTTF::create("", unity::GetDefaultFontType(), m_nCardLength / 4);
		sprintf(temp, "%d", m_nHighScore);
		labelHighPt->setString(temp);
		labelHighPt->setColor(Color3B::WHITE);
		labelHighPt->setAnchorPoint(cocos2d::Point(0.5, 0));
		labelHighPt->setPosition(cocos2d::Point(startX + spriteBest->getTextureRect().size.width*spriteBest->getScaleX() / 2, startY));
		addChild(labelHighPt, 2, eChild_HighPoint);
	}

	if (m_eGameMode == eMode_Bluetooth)
	{
		//other point
		startX += m_nRectLength*m_fScale;
		auto spriteOtherScore = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("roundedrectangle2.png"));
		spriteOtherScore->setScale(1.2f, 1);
		spriteOtherScore->setColor(Color3B(187, 173, 160));
		spriteOtherScore->setAnchorPoint(cocos2d::Point(1, 0));
		spriteOtherScore->setPosition(cocos2d::Point(startX, startY));
		addChild(spriteOtherScore, 1);

		auto labelOtherScore = LabelTTF::create("Other SCORE", unity::GetDefaultFontType(), 18);
		labelOtherScore->setColor(Color3B(238, 228, 218));
		labelOtherScore->setPosition(cocos2d::Point(startX - spriteOtherScore->getTextureRect().size.width*spriteOtherScore->getScaleX() / 2, startY + spriteOtherScore->getTextureRect().size.height*spriteOtherScore->getScaleY() * 3 / 4));
		addChild(labelOtherScore, 2);

		auto labelOtherPt = LabelTTF::create("", unity::GetDefaultFontType(), m_nCardLength / 6);
		labelOtherPt->setColor(Color3B::WHITE);
		labelOtherPt->setAnchorPoint(Point(0.5, 0));
		labelOtherPt->setPosition(cocos2d::Point(startX - spriteOtherScore->getTextureRect().size.width*spriteOtherScore->getScaleX() / 2, startY));
		addChild(labelOtherPt, 2, eChild_OtherPoint);
	}

	//card region
	m_pCardRegion = CardRegion::create(m_nCardLength,false);
	addChild(m_pCardRegion, 2, eChild_CardRegion);
	m_pCardRegion->setAnchorPoint(Point(0, 0));
	m_pCardRegion->setPosition(Point(m_nOffsetX, m_nOffsetY));
	m_pCardRegion->setScale(m_fScale);

	if (m_eGameMode == eMode_Bluetooth)
	{
		auto showLabel = LabelTTF::create("", unity::GetDefaultFontType(), 25);
		showLabel->setAnchorPoint(Point(0.5, 1));
		showLabel->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 40));
		addChild(showLabel, 3, eChild_ShowLabel);

		//other card region
		m_pOtherCardRegion = CardRegion::create(m_nCardLength,true);
		addChild(m_pOtherCardRegion, 2, eChild_OtherCardRegion);
		m_pOtherCardRegion->setAnchorPoint(Point(0, 0));
		m_pOtherCardRegion->setPosition(Point(m_nOffsetX, m_nOffsetY + m_nStatusHeight + m_nRectLength*m_fScale));
		m_pOtherCardRegion->setScale(m_fScale);

		//add a update scheduler
		unscheduleUpdate();
		scheduleUpdate();
		m_dwStartTime = unity::GetTickCountX();

		//inform game scene create finish
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

		//g_Transform.Send_Scene_Init();
		unity::Log(TAG, "game scene init!");
#endif
	}
	return true;
}

void HelloWorld::update(float fDelta)
{
	if (m_bGetStartInformed && m_nStartSeconds >= 3)
	{
		//inform other player to start
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

		g_Transform.Send_Start();
		unity::Log(TAG, "go to the game scene!");
#endif
		//touch control
		if (m_pTouchListener)
			_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pTouchListener, this);

		auto label = dynamic_cast<LabelTTF*>(getChildByTag(eChild_ShowLabel));
		if (label)
		{
			label->setVisible(false);
		}

		unscheduleUpdate();
		return;
	}
	unsigned long curTime = unity::GetTickCountX();
	if (m_dwStartTime + 1000 <= curTime && m_nStartSeconds < 3)
	{
		m_dwStartTime = curTime;
		auto label = dynamic_cast<LabelTTF*>(getChildByTag(eChild_ShowLabel));
		if (label)
		{
			//unity::Log("2048debug", "after %d the game will start", 3 - m_nStartSeconds);
			label->setString(StringUtils::format("The game will start in %d...", 3 - m_nStartSeconds));
		}
		m_nStartSeconds++;
		if (m_nStartSeconds >= 3)
		{
			//inform other player to start
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			unity::Log(TAG, "inform game start");
			//g_Transform.Send_Start();
#endif
		}
	}
}


void HelloWorld::Restart(cocos2d::Ref* pSender)
{
	if (m_eGameMode == eMode_Single)
		Director::getInstance()->replaceScene(HelloWorld::createScene(eMode_Single));
	else if (m_eGameMode == eMode_Bluetooth)
		Director::getInstance()->replaceScene(MainTitle::createScene());
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_BACKSPACE:
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
												  {
													  //Director::getInstance()->end();
													  showTipDialog("Exit", "Do you want to end such a wonderful game?",EXIT_DIALOG);
												  }
#endif
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
												   
												 MoveAndMergeCard(keycode);
	}
	default:
		break;
	}



}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	if (m_bMoving)
		return false;

	//记录一次触摸开始
	m_bMoving = true;

	m_iStartPt = touch->getLocation();
	//CCLOG("start x: %f y:%f", m_iStartPt.x, m_iStartPt.y);
	
	return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{

}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	Point endPt = touch->getLocation();

	//一次触摸结束
	m_bMoving = false;

	//CCLOG("end x: %f y:%f", endPt.x, endPt.y);

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

void HelloWorld::MoveAndMergeCard(EventKeyboard::KeyCode dir)
{
	if (m_pCardRegion)
		m_pCardRegion->MoveAndMergeCard(dir);
}

void HelloWorld::AddOtherPoint(int pt)
{
	auto label = dynamic_cast<LabelTTF*>(getChildByTag(eChild_OtherPoint));
	if (label)
	{
		char temp[20];
		sprintf(temp, "%d", pt);
		label->setString(temp);
	}
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

		if (m_eGameMode == eMode_Bluetooth)
			g_Transform.Send_Point(m_nPoint);
	}
}

