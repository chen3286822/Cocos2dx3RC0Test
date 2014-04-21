#include "Card.h"

USING_NS_CC;

Card* Card::create(int num,int length)
{
	Card* card = new Card();
	if(card && card->init(length))
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
	auto label = dynamic_cast<LabelTTF*>(getChildByTag(eChild_Label));
	if (label && m_pCardTex)
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

bool Card::init(int length)
{
	//其实Node::init一直都返回true
	if (!Node::init())
	{
		return false;
	}


	m_pCardTex = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("blank.png"));
	m_pCardTex->setTextureRect(cocos2d::Rect(0, 0, length, length));
	m_pCardTex->setColor(Color3B::ORANGE);
	m_pCardTex->setAnchorPoint(cocos2d::Point(0, 0));
	addChild(m_pCardTex, 1, eChild_CardTex);

	auto label = LabelTTF::create("", "Arial", length/2);
	//label->setAnchorPoint(cocos2d::Point(0.5, 0.5));
	label->setPosition(Point(length / 2, length/2));
	addChild(label, 1, eChild_Label);
	return true;
}