#include "Dialog.h"

USING_NS_CC;

bool Dialog::init()
{
	if (!LayerColor::initWithColor(Color4B(63, 63, 63, 155)))
	{
		return false;
	}


	m_vButtons.clear();
	return true;
}

void Dialog::onEnter()
{
	LayerColor::onEnter();

	m_pTouchListener = EventListenerTouchOneByOne::create();
	m_pTouchListener->setSwallowTouches(true);
	m_pTouchListener->onTouchBegan = CC_CALLBACK_2(Dialog::onTouchBegan, this);
	m_pTouchListener->onTouchMoved = CC_CALLBACK_2(Dialog::onTouchMoved, this);
	m_pTouchListener->onTouchEnded = CC_CALLBACK_2(Dialog::onTouchEnded, this);
	m_pTouchListener->onTouchCancelled = CC_CALLBACK_2(Dialog::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithFixedPriority(m_pTouchListener, -1);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	if (!m_strTitle.empty())
	{
		auto titleLabel = LabelTTF::create(m_strTitle, "Arial", 40);
		titleLabel->setColor(Color3B(30, 30, 30));
		titleLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 2 / 3 + origin.y));
		this->addChild(titleLabel);
	}

	if (!m_strContent.empty())
	{
		auto contentLabel = LabelTTF::create(m_strContent, "Arial", 25, cocos2d::Size(visibleSize.width * 4 / 5, visibleSize.height / 5));
		contentLabel->setAnchorPoint(Point(0.5, 1));
		contentLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 3 / 5 + origin.y));
		this->addChild(contentLabel);
	}

	if (!m_vButtons.empty())
	{
		Vector<MenuItem*> vItems;
		for (auto textButton : m_vButtons)
		{
			auto textLabel = LabelTTF::create(textButton.m_strText, "Arial", 20);
			auto textItem = MenuItemLabel::create(textLabel, textButton.m_iCallback);
			vItems.pushBack(textItem);
		}
		m_pMenu = Menu::createWithArray(vItems);
		m_pMenu->setAnchorPoint(Point(0.5, 1));
		m_pMenu->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 2 / 5 + origin.y));
		m_pMenu->alignItemsHorizontallyWithPadding(20);
		this->addChild(m_pMenu);
	}
}

void Dialog::onExit()
{
	m_pMenu = nullptr;
	m_bMenuTouched = false;
	_eventDispatcher->removeEventListener(m_pTouchListener);
	this->removeAllChildren();

	LayerColor::onExit();
}

//触摸控制
bool Dialog::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	//由于此layer触摸优先级是负数，所以会拦截所有控件的消息，这里我们需要手动
	//传递给那些menuitem 消息
	if (m_pMenu)
		m_bMenuTouched = m_pMenu->onTouchBegan(touch, event);
	return true;
}

void Dialog::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (m_bMenuTouched && m_pMenu)
		m_pMenu->onTouchMoved(touch, event);
}

void Dialog::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (m_bMenuTouched)
	{
		if (m_pMenu)
			m_pMenu->onTouchEnded(touch, event);
		m_bMenuTouched = false;
	}
}

void Dialog::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (m_bMenuTouched)
	{
		if (m_pMenu)
			m_pMenu->onTouchCancelled(touch, event);
		m_bMenuTouched = false;
	}
}

void Dialog::AddButton(const char* text, const cocos2d::ccMenuCallback& callback)
{
	TextButton textButton;
	textButton.m_strText = text;
	textButton.m_iCallback = callback;
	m_vButtons.push_back(textButton);
}

void Dialog::SetTitle(std::string title)
{
	m_strTitle = title;
}

void Dialog::SetContent(std::string content)
{
	m_strContent = content;
}
