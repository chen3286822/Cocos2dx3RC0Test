#include "Dialog.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

bool Dialog::init()
{
	if (!LayerColor::initWithColor(Color4B(63, 63, 63, 155)))
	{
		return false;
	}

	m_bNewRecord = false;
	m_vButtons.clear();
	return true;
}

void Dialog::onEnter()
{
	LayerColor::onEnter();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	if (m_bNewRecord)
		m_pLayout = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("2048UI_Dialog/2048UI_Dialog1.ExportJson"));
	else
		m_pLayout = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("2048UI_Dialog/2048UI_Dialog2.ExportJson"));

	this->addChild(m_pLayout);
	Size rootSize = m_pLayout->getSize();
	m_pLayout->setPosition(Point((visibleSize.width-rootSize.width)/2,(visibleSize.height-rootSize.height)/2));

	m_pButton = static_cast<Button*>(Helper::seekWidgetByName(m_pLayout, "OKButton"));
	m_pButton->addTouchEventListener(this, toucheventselector(Dialog::touchEvent));


	m_pTouchListener = EventListenerTouchOneByOne::create();
	m_pTouchListener->setSwallowTouches(true);
	m_pTouchListener->onTouchBegan = CC_CALLBACK_2(Dialog::onTouchBegan, this);
	m_pTouchListener->onTouchMoved = CC_CALLBACK_2(Dialog::onTouchMoved, this);
	m_pTouchListener->onTouchEnded = CC_CALLBACK_2(Dialog::onTouchEnded, this);
	m_pTouchListener->onTouchCancelled = CC_CALLBACK_2(Dialog::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithFixedPriority(m_pTouchListener, -1);



	if (!m_strTitle.empty())
	{
		auto titleLabel = static_cast<Text*>(Helper::seekWidgetByName(m_pLayout, "TitleLabel"));
		//这里需要重新设置下字体，因为中文字体名称在读取json文件时变成乱码
		titleLabel->setFontName("Arial");
		titleLabel->setText(m_strTitle);
// 		auto titleLabel = LabelTTF::create(m_strTitle, "Arial", 40);
// 		titleLabel->setColor(Color3B(30, 30, 30));
// 		titleLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 2 / 3 + origin.y));
// 		this->addChild(titleLabel);
	}

	if (!m_strContent.empty())
	{
		auto contentLabel = static_cast<Text*>(Helper::seekWidgetByName(m_pLayout, "ContentLabel"));
		contentLabel->setFontName("Arial");
		contentLabel->setText(m_strContent);
		contentLabel->setFontSize(18);
// 		auto contentLabel = LabelTTF::create(m_strContent, "Arial", 25, cocos2d::Size(visibleSize.width * 4 / 5, visibleSize.height / 5));
// 		contentLabel->setAnchorPoint(Point(0.5, 1));
// 		contentLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 3 / 5 + origin.y));
// 		this->addChild(contentLabel);
	}

	if (!m_vButtons.empty())
	{
//		Vector<MenuItem*> vItems;
		for (auto textButton : m_vButtons)
		{
// 			auto textLabel = LabelTTF::create(textButton.m_strText, "Arial", 20);
//			auto textItem = MenuItemLabel::create(textLabel, textButton.m_iCallback);
//			vItems.pushBack(textItem);

			//暂时只容纳一个按钮及其回调函数
			m_pButton->setTitleText(textButton.m_strText);
			m_pButton->setTitleFontName("Arial");
			m_pButton->setTitleFontSize(20);
			m_pButton->setTitleColor(Color3B::BLACK);
			break;
		}
// 		m_pMenu = Menu::createWithArray(vItems);
// 		m_pMenu->setAnchorPoint(Point(0.5, 1));
// 		m_pMenu->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 2 / 5 + origin.y));
// 		m_pMenu->alignItemsHorizontallyWithPadding(20);
// 		this->addChild(m_pMenu);
	}

	this->stopAllActions();
	this->setScale(0.3f);
	auto scaleAction = ScaleTo::create(0.3f, 1.0f);
	this->runAction(scaleAction);
}

void Dialog::touchEvent(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		break;

	case TOUCH_EVENT_MOVED:
		break;

	case TOUCH_EVENT_ENDED:
	{
							  m_vButtons[0].m_iCallback(pSender);
	}
		break;

	case TOUCH_EVENT_CANCELED:
		break;

	default:
		break;
	}
}

void Dialog::onExit()
{
	m_pButton = nullptr;
	m_bButtonTouched = false;
	_eventDispatcher->removeEventListener(m_pTouchListener);
	this->removeAllChildren();

	LayerColor::onExit();
}

//触摸控制
bool Dialog::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	//由于此layer触摸优先级是负数，所以会拦截所有控件的消息，这里我们需要手动
	//传递给那些menuitem 消息
	if (m_pButton)
		m_bButtonTouched = m_pButton->onTouchBegan(touch, event);
	return true;
}

void Dialog::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (m_bButtonTouched && m_pButton)
		m_pButton->onTouchMoved(touch, event);
}

void Dialog::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (m_bButtonTouched)
	{
		if (m_pButton)
			m_pButton->onTouchEnded(touch, event);
		m_bButtonTouched = false;
	}
}

void Dialog::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (m_bButtonTouched)
	{
		if (m_pButton)
			m_pButton->onTouchCancelled(touch, event);
		m_bButtonTouched = false;
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
