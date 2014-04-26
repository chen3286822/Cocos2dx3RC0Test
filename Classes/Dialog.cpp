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
		auto menu = Menu::createWithArray(vItems);
		menu->setAnchorPoint(Point(0.5, 1));
		menu->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 2 / 5 + origin.y));
		menu->alignItemsHorizontallyWithPadding(20);
		this->addChild(menu);
	}
}

void Dialog::onExit()
{
	this->removeAllChildren();

	LayerColor::onExit();
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
