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

class CardRegion : public cocos2d::Node
{
public:
	static CardRegion* create(int cardLength);
	virtual bool init(int cardLength);

	void AddCard();
	void MoveAndMergeCard(cocos2d::EventKeyboard::KeyCode dir);
	Card* FindCard(int x, int y);
private:
	void CheckFailure();
	void MoveAction(int x, int y, cocos2d::EventKeyboard::KeyCode dir);
	void RemoveMergedCardAndDoubleNum(int x, int y, cocos2d::EventKeyboard::KeyCode dir);


	MoveCard m_iCardPark[4][4];

	int  m_nBorder{ 10 };		//卡片间距
	int  m_nShorter;			//屏幕较短的一边
	int  m_nCardLength;	//卡片边长
	int  m_nRectLength;	//卡片+间距长
	int  m_nOffsetX;
	int  m_nOffsetY;
};

class HelloWorld : public cocos2d::LayerColor
{
public:
	enum eChild
	{
		eChild_HelloWorldLayer,
		eChild_Point,
		eChild_HighPoint,
		eChild_OtherPoint,
		eChild_CardRegion,
	};

	enum eMode
	{
		eMode_Single,
		eMode_Bluetooth,
	};
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(eMode mode);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    //void menuCloseCallback(cocos2d::Ref* pSender);

	//接受返回键
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	//触摸控制
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	
	void Restart(cocos2d::Ref* pSender);

	void AddPoint(int pt);
	void AddOtherPoint(int pt);
	void SetGameMode(eMode mode){ m_eGameMode = mode; }
	eMode GetGameMode(){ return m_eGameMode; }
	void SetPoint(int pt){ m_nPoint = pt; }
	int GetPoint(){ return m_nPoint; }
	void SetHighPoint(int pt){ m_nHighScore = pt; }
	int GetHighPoint(){ return m_nHighScore; }
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	void MoveAndMergeCard(cocos2d::EventKeyboard::KeyCode dir);
	
	CardRegion* m_pCardRegion{ nullptr };
	int m_nHighScore;				//历史最高得分
	int m_nPoint{ 0 };				//得分

	//一些卡片相关的变量
	int  m_nBorder{ 10 };		//卡片间距
	int  m_nShorter;			//屏幕较短的一边
	int  m_nCardLength;	//卡片边长
	int  m_nRectLength;	//卡片+间距长
	int  m_nOffsetX;			
	int  m_nOffsetY;			

	cocos2d::Point m_iStartPt;			//记录每次开始触摸的开始点坐标，用于判断滑动方向
	bool m_bMoving{ false };		//判断卡片是否在移动中，用于决定是否接受触摸操作

	eMode m_eGameMode{eMode_Single};			//游戏模式
};

#endif // __HELLOWORLD_SCENE_H__
