#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Card.h"

struct MoveCard
{
	Card* m_pCard;							
	cocos2d::Point m_iMovePos;		//将要移动到的位置
	bool m_bGoingMerge;				//是否会合并
	int m_nTag;									//标记是否为被合并卡片，0表示没有合并，1表示该卡片要被后面的合并，2表示该卡片将要去合并某卡片，用于标记删除被合并卡片，翻倍合并卡片
	bool m_bMoving;						//是否移动中
	MoveCard()
	{
		m_pCard = nullptr;
		m_bGoingMerge = false;
		m_nTag = 0;
		m_bMoving = false;
	}
};

class HelloWorld : public cocos2d::LayerColor
{
public:
	enum eChild
	{
		eChild_Point,
	};
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	//接受返回键
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	void AddNewCard();
	Card* FindCard(int x,int y);
	void Restart(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	void MoveAndMergeCard(cocos2d::EventKeyboard::KeyCode dir);
	void RemoveMergedCardAndDoubleNum(int x, int y, cocos2d::EventKeyboard::KeyCode dir);
	void MoveAction(int x, int y, cocos2d::EventKeyboard::KeyCode dir);
	void AddPoint(int pt);
	void CheckFailure();

	MoveCard m_iCardPark[4][4];
	int m_nPoint{ 0 };				//得分

	//一些卡片相关的变量
	int  m_nBorder{ 5 };		//卡片间距
	int  m_nShorter;			//屏幕较短的一边
	int  m_nCardLength;	//卡片边长
	int  m_nRectLength;	//卡片+间距长
	int  m_nOffsetX;			
	int  m_nOffsetY;			
};

#endif // __HELLOWORLD_SCENE_H__
