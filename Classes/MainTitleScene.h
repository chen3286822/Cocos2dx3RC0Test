#ifndef __MAINTITLE_SCENE_H__
#define __MAINTITLE_SCENE_H__


#include "cocos2d.h"

class MainTitle : public cocos2d::LayerColor
{
public:
	enum eChild
	{
		eChild_MainTitleLayer,
		eChild_Menu,
	};

	static cocos2d::Scene* createScene();

	virtual bool init();

	void SingleMode(cocos2d::Ref* pSender);
	void BluetoothMode(cocos2d::Ref* pSender);
	void Exit(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(MainTitle);
};

#endif