#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include "cocos2d.h"

class GameData
{
public:
	GameData();
	~GameData();

	void Clear();

	//��Ϸ�����Ƿ��Ѿ���ʼ��
	CC_SYNTHESIZE(bool, m_bSceneInit, SceneInit);
	//��ǰ��õķ���
	CC_SYNTHESIZE(int, m_nPoint, Point);
	//��ǰĿ�����
	CC_SYNTHESIZE(int, m_nCurrentTargetPoint, CurrentTargetPoint);
private:

};

extern GameData g_Gamedata;
#endif