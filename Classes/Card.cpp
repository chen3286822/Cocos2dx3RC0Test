#include "Card.h"

USING_NS_CC;

Card* Card::create(int num)
{
	Card* card = new Card();
	if(card)
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
	CC_SAFE_RELEASE(m_pCardTex);
}

bool Card::init()
{
	m_pCardTex = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("blank.png"));
	m_pCardTex->setTextureRect(CCRectMake(0, 0, 50, 50));
	return true;
}