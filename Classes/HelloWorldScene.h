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

struct SimpleCard
{
	cocos2d::Point m_iPos;
	int m_nNum;
};

class CardRegion : public cocos2d::Node
{
public:
	static CardRegion* create(int cardLength,bool bOther);
	virtual bool init(int cardLength, bool bOther);
	virtual void update(float fDelta);

	void AddCard(bool bInit=false);
	void AddCard(int x,int y,int num);	//for other


	void MoveAndMergeCard(cocos2d::EventKeyboard::KeyCode dir);
	Card* FindCard(int x, int y);
	
	bool& GetOther(){ return m_bOther; }
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

	bool m_bOther{ false };		//是否代表对方的卡片区域
	std::vector<SimpleCard> m_vInitCards;
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
		eChild_OtherCardRegion,
		eChild_ShowLabel,
	};

	enum eMode
	{
		eMode_Single,
		eMode_Bluetooth,
	};
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(eMode mode);
	~HelloWorld();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init(eMode mode);
	virtual void update(float fDelta);
    
    // a selector callback
    //void menuCloseCallback(cocos2d::Ref* pSender);

	//接受返回键
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	//触摸控制
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	
	void Restart(cocos2d::Ref* pSender);
	void GoRank(cocos2d::Ref* pSender);
	void Back(cocos2d::Ref* pSender);

	void AddPoint(int pt);
	void AddOtherPoint(int pt);
	void SetGameMode(eMode mode){ m_eGameMode = mode; }
	eMode GetGameMode(){ return m_eGameMode; }
	bool& GetCardMoved(){ return m_bHasMoved; }
    
    // implement the "static create()" method manually
	static HelloWorld* create(eMode mode);

	void InformToStart(){ m_bGetStartInformed = true; }
private:
	cocos2d::EventListenerKeyboard* m_pKeyboardListener{ nullptr };
	cocos2d::EventListenerTouchOneByOne* m_pTouchListener{ nullptr };

	void MoveAndMergeCard(cocos2d::EventKeyboard::KeyCode dir);
	
	CardRegion* m_pCardRegion{ nullptr };
	CardRegion* m_pOtherCardRegion{ nullptr };

	//一些控制布局的变量
	int  m_nCardRegionOffset{ 2 };	//卡片区域到边界的偏移
	int  m_nBorder{ 10 };		//卡片间距
	int  m_nStatusHeight{ 70 };	//状态栏的高度
	int  m_nShorter;			//屏幕较短的一边
	int  m_nLonger;			//屏幕较长的一边
	int  m_nCardLength;	//卡片边长
	int  m_nRectLength;	//卡片区域边长
	int  m_nOffsetX;			
	int  m_nOffsetY;			
	float m_fScale;		//卡片区域的缩放

	cocos2d::Point m_iStartPt;			//记录每次开始触摸的开始点坐标，用于判断滑动方向
	bool m_bMoving{ false };		//判断卡片是否在移动中，用于决定是否接受触摸操作
	bool m_bHasMoved{ false };	//纪录玩家是否已经移动过卡片了，用于判断是否可以直接退出当前游戏

	eMode m_eGameMode{eMode_Single};			//游戏模式
	unsigned long m_dwStartTime{ 0 };
	int m_nStartSeconds{ 0 };
	bool m_bGetStartInformed{ false };
};

#endif // __HELLOWORLD_SCENE_H__
