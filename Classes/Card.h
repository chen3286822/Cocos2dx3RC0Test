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
	CC_PROPERTY(int, m_nNum, Num);//��¼�ÿ�Ƭ����
public:
	static Card* create(int num,int length);

	bool init(int length);
	cocos2d::Point& GetPos(){ return m_iPos; }
private:
	Card();
	~Card();

	cocos2d::Sprite* m_pCardTex;	//��Ƭ��ʾͼƬ
	cocos2d::Point m_iPos;
};

#endif