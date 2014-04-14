#include "Card.h"

USING_NS_CC;

Card* Card::create(int num)
{
	Card* card = new Card();
	if(card && card->init())
	{
		card->autorelease();
		card->setNum(num);
		return card;
	}

	CC_SAFE_DELETE(card);
	return nullptr;
}

Card::Card():
	m_nNum(0),
	m_pCardTex(nullptr)
{

}

Card::~Card()
{
	//CC_SAFE_RELEASE(m_pCardTex);
}

void Card::setNum(int var)
{
	m_nNum = var;
	auto label = dynamic_cast<LabelAtlas*>(getChildByTag(eChild_Label));
	if (label)
	{
		char temp[50];
		sprintf(temp, "%d", m_nNum);
		label->setString(temp);
	}
}

int Card::getNum()
{
	return m_nNum;
}

bool Card::init()
{
	m_pCardTex = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("blank.png"));
	m_pCardTex->setTextureRect(cocos2d::Rect(0, 0, 50, 50));
	m_pCardTex->setColor(Color3B::ORANGE);
	m_pCardTex->setAnchorPoint(cocos2d::Point(0, 0));
	addChild(m_pCardTex, 1, eChild_CardTex);

	auto label = LabelAtlas::create("", "fonts/tuffy_bold_italic-charmap.plist");
	//label->setAnchorPoint(cocos2d::Point(0.5, 0.5));
	label->setPosition(Point(50/2,50/2));
	addChild(label, 1, eChild_Label);
	return true;
}