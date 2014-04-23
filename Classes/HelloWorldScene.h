#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Card.h"

struct MoveCard
{
	Card* m_pCard;							
	cocos2d::Point m_iMovePos;		//��Ҫ�ƶ�����λ��
	bool m_bGoingMerge;				//�Ƿ��ϲ�
	MoveCard()
	{
		m_pCard = nullptr;
		m_bGoingMerge = false;
	}
};

class HelloWorld : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	//���ܷ��ؼ�
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	void AddNewCard();
	Card* FindCard(int x,int y);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	void MoveAndMergeCard(cocos2d::EventKeyboard::KeyCode dir);

	MoveCard m_iCardPark[4][4];
	//std::list<Card*> m_lCards;

	//һЩ��Ƭ��صı���
	int  m_nBorder{ 5 };		//��Ƭ���
	int  m_nShorter;			//��Ļ�϶̵�һ��
	int  m_nCardLength;	//��Ƭ�߳�
	int  m_nOffsetX;			
	int  m_nOffsetY;			
};

#endif // __HELLOWORLD_SCENE_H__
