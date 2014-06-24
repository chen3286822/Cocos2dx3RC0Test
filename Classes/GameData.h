#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include "cocos2d.h"

class GameData
{
public:
	GameData();
	~GameData();

	void Clear();

	//游戏场景是否已经初始化
	CC_SYNTHESIZE(bool, m_bSceneInit, SceneInit);
	//当前获得的分数
	CC_SYNTHESIZE(int, m_nPoint, Point);
	//当前目标分数
	CC_SYNTHESIZE(int, m_nCurrentTargetPoint, CurrentTargetPoint);
private:

};

extern GameData g_Gamedata;
#endif