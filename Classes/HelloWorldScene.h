#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Card.h"

struct MoveCard
{
	Card* m_pCard;							
	cocos2d::Point m_iMovePos;		//��Ҫ�ƶ�����λ��
	bool m_bGoingMerge;				//�Ƿ��ϲ�
	int m_nTag;									//����Ƿ�Ϊ���ϲ���Ƭ��0��ʾû�кϲ���1��ʾ�ÿ�ƬҪ������ĺϲ���2��ʾ�ÿ�Ƭ��Ҫȥ�ϲ�ĳ��Ƭ�����ڱ��ɾ�����ϲ���Ƭ�������ϲ���Ƭ
	bool m_bMoving;						//�Ƿ��ƶ���
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

	int  m_nBorder{ 10 };		//��Ƭ���
	int  m_nShorter;			//��Ļ�϶̵�һ��
	int  m_nCardLength;	//��Ƭ�߳�
	int  m_nRectLength;	//��Ƭ+��೤
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

	//���ܷ��ؼ�
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	//��������
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
	int m_nHighScore;				//��ʷ��ߵ÷�
	int m_nPoint{ 0 };				//�÷�

	//һЩ��Ƭ��صı���
	int  m_nBorder{ 10 };		//��Ƭ���
	int  m_nShorter;			//��Ļ�϶̵�һ��
	int  m_nCardLength;	//��Ƭ�߳�
	int  m_nRectLength;	//��Ƭ+��೤
	int  m_nOffsetX;			
	int  m_nOffsetY;			

	cocos2d::Point m_iStartPt;			//��¼ÿ�ο�ʼ�����Ŀ�ʼ�����꣬�����жϻ�������
	bool m_bMoving{ false };		//�жϿ�Ƭ�Ƿ����ƶ��У����ھ����Ƿ���ܴ�������

	eMode m_eGameMode{eMode_Single};			//��Ϸģʽ
};

#endif // __HELLOWORLD_SCENE_H__
