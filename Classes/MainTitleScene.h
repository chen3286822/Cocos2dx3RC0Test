#ifndef __MAINTITLE_SCENE_H__
#define __MAINTITLE_SCENE_H__


#include "cocos2d.h"

class BgLayer : public cocos2d::Layer
{
public:
	enum eChild
	{
		eChild_Bg1,
		eChild_Bg2,
		eChild_Bg3,
		eChild_Bg4,
	};

	CREATE_FUNC(BgLayer);

	virtual bool init();
	virtual void update(float fDelta);
private:
	cocos2d::Sprite* m_pBg[4];
	int m_nPos[4];//标记4个背景图的位置, 0右上1左上2右下3左下
};

class MainTitle : public cocos2d::LayerColor
{
public:
	enum eChild
	{
		eChild_MainTitleLayer,
		eChild_Menu,
		eChild_Background,
	};

	static cocos2d::Scene* createScene();

	virtual bool init();

	void SingleMode(cocos2d::Ref* pSender);
	void BluetoothMode(cocos2d::Ref* pSender);
	void Rank(cocos2d::Ref* pSender);
	void Exit(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(MainTitle);

private:
	BgLayer* m_pBgLayer{ nullptr };
};

#endif