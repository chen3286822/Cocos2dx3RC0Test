#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"


class Card : public cocos2d::Node
{
public:
	enum eChild
	{
		eChild_CardTex,
		eChild_Label,
	};

	static Card* create(int num,int length);

	bool init(int length);
	cocos2d::Point& GetPos(){ return m_iPos; }
	int getNum();
	void setNum(int var,bool initNum=true);
private:
	Card();
	~Card();

	cocos2d::Sprite* m_pCardTex;	//ø®∆¨œ‘ æÕº∆¨
	cocos2d::Point m_iPos;
	int m_nNum;
};

#endif