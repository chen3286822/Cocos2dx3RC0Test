#include "Dialog.h"
#include "Unity.h"
#include "HelloWorldScene.h"
#include "GameData.h"
#include "JNIFunc.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace extension;

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
	m_pLayout->setPosition(Point((visibleSize.width - rootSize.width) / 2 + origin.x, (visibleSize.height - rootSize.height) / 2 + origin.y));

	m_pButton = static_cast<Button*>(Helper::seekWidgetByName(m_pLayout, "OKButton"));
	m_pButton->addTouchEventListener(this, toucheventselector(Dialog::touchEvent));

// 	m_pTextField = static_cast<TextField*>(Helper::seekWidgetByName(m_pLayout, "NameEdit"));
// 	m_pTextField->addEventListenerTextField(this, textfieldeventselector(Dialog::textFieldEvent));

	if (m_bNewRecord)
	{
		auto editBg = Scale9Sprite::create("greenbuttonup.png");
		m_pEditBox = EditBox::create(Size(171, 33), editBg);
		m_pEditBox->setPosition(Point(m_pLayout->getPosition().x + 180, m_pLayout->getPosition().y + 87));
		m_pEditBox->setPlaceHolder("Input Your Name");
		m_pEditBox->setDelegate(this);
		m_pEditBox->setInputMode(EditBox::InputMode::ANY);
		m_pEditBox->setReturnType(EditBox::KeyboardReturnType::DONE);
		m_pEditBox->setMaxLength(13);
		m_pEditBox->setFontColor(Color3B::BLUE);
		m_pEditBox->setFont(unity::GetDefaultFontType(), 18);
		m_pEditBox->setPlaceholderFont(unity::GetDefaultFontType(), 18);
		m_pEditBox->setPlaceholderFontColor(Color3B::BLUE);
		m_iEditBoxPos = m_pEditBox->getPosition();
		this->addChild(m_pEditBox, 1);
	}


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
		titleLabel->setFontName(unity::GetDefaultFontType());
		titleLabel->setText(m_strTitle);
	}

	if (!m_strContent.empty())
	{
		auto contentLabel = static_cast<Text*>(Helper::seekWidgetByName(m_pLayout, "ContentLabel"));
		contentLabel->setFontName(unity::GetDefaultFontType());
		contentLabel->setText(m_strContent);
		contentLabel->setFontSize(19);
	}

	if (!m_vButtons.empty())
	{
		for (auto textButton : m_vButtons)
		{
			//暂时只容纳一个按钮及其回调函数
			m_pButton->setTitleText(textButton.m_strText);
			m_pButton->setTitleFontName(unity::GetDefaultFontType());
			m_pButton->setTitleFontSize(22);
			m_pButton->setTitleColor(Color3B::BLACK);
			break;
		}
	}

	this->stopAllActions();
	this->setScale(0.3f);
	auto scaleAction = ScaleTo::create(0.3f, 1.0f);
	this->runAction(scaleAction);
}

void Dialog::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox)
{
}

void Dialog::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox)
{
}

void Dialog::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text)
{

}

void Dialog::editBoxReturn(cocos2d::extension::EditBox* editBox)
{

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
		if (m_bNewRecord && m_pEditBox)
		{
			std::string Name = m_pEditBox->getText();
			if (!Name.empty())
			{
				//插入新纪录
				auto scene = dynamic_cast<HelloWorld*>(getParent());
				if (scene)
				{
					unity::UserData::getInstance()->InsertRecord(Name.c_str(), g_Gamedata.getPoint());
				}
			}
			else
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				toastMsg("You should input a name");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				cocos2d::MessageBox("You should input a name","Error");
#endif		
				return;
			}

		}
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
	m_pEditBox = nullptr;
	m_bButtonTouched = false;
	m_bTextFieldTouched = false;
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

	if (m_pEditBox)
		m_bTextFieldTouched = m_pEditBox->onTouchBegan(touch, event);
	return true;
}

void Dialog::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (m_bButtonTouched && m_pButton)
		m_pButton->onTouchMoved(touch, event);
	if (m_bTextFieldTouched && m_pEditBox)
		m_pEditBox->onTouchMoved(touch, event);
}

void Dialog::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (m_bButtonTouched)
	{
		if (m_pButton)
			m_pButton->onTouchEnded(touch, event);
		m_bButtonTouched = false;
	}
	if (m_bTextFieldTouched)
	{
		if (m_pEditBox)
			m_pEditBox->onTouchEnded(touch, event);
		m_bTextFieldTouched = false;
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
	if (m_bTextFieldTouched)
	{
		if (m_pEditBox)
			m_pEditBox->onTouchCancelled(touch, event);
		m_bTextFieldTouched = false;
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
