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
    if ( !Layer::init() )
    {
        return false;
    }

	srand((unsigned int)time(NULL));
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
   /* auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);*/
    
	Director::getInstance()->getTextureCache()->addImage("blank.png");
	for (int i = 0; i < 16;i++)
	{
		int x = i / 4;
		int y = i % 4;
		m_Cards[x][y] = Card::create(i);
		addChild(m_Cards[x][y]);
		m_Cards[x][y]->setPosition(cocos2d::Point(y*60,x*60));

	}
	//AddNewNum();
    return true;
}

void HelloWorld::AddNewNum()
{
	//计算剩余空卡
	int num = 0;
	std::vector<int> emptyCard;
	emptyCard.reserve(16);
	for (int i = 0; i < 4; i ++)
	{
		int x = i / 4;
		int y = i % 4;
		if (m_Cards[x][y]->getNum() == 0)
		{
			num++;
			emptyCard.push_back(i);
		}
	}
	if (num < 2)
	{
		//没有空间，失败
	}
	//随机选2个放置数字
	auto first = rand() % num;
	auto second = rand() % (num - 1);
	
	auto is2 = (rand() % 4) >= 1;
	int x = emptyCard[first] / 4;
	int y = emptyCard[first] % 4;
	m_Cards[x][y]->setNum(is2 ? 2 : 4);

	is2 = (rand() % 4) >= 1;
	if (second < first)
	{
		x = emptyCard[second] / 4;
		y = emptyCard[second] % 4;
	}
	else
	{
		x = emptyCard[second + 1] / 4;
		y = emptyCard[second + 1] % 4;
	}
	m_Cards[x][y]->setNum(is2 ? 2 : 4);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
