#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"

class Card : public cocos2d::Node
{
	CC_SYNTHESIZE(int,m_nNum,Num);//��¼�ÿ�Ƭ����
public:
	static Card* create(int num);

	virtual bool init();
private:
	Card();
	~Card();

	cocos2d::Sprite* m_pCardTex;	//��Ƭ��ʾͼƬ
};

#endif