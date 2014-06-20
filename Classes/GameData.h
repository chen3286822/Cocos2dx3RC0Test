#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include "cocos2d.h"

class GameData
{
public:
	GameData();
	~GameData();

	void Clear();

	CC_SYNTHESIZE(bool, m_bSceneInit, SceneInit);
private:

};

extern GameData g_Gamedata;
#endif