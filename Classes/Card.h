#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"

enum eChild
{
	eChild_CardTex,
	eChild_Label,
};

class Card : public cocos2d::Node
{
	CC_PROPERTY(int, m_nNum, Num);//¼ÇÂ¼¸Ã¿¨Æ¬Êý×Ö
public:
	static Card* create(int num,int length);

	bool init(int length);
private:
	Card();
	~Card();

	cocos2d::Sprite* m_pCardTex;	//¿¨Æ¬ÏÔÊ¾Í¼Æ¬
};

#endif