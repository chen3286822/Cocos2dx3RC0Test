#include "GameData.h"

GameData g_Gamedata;

GameData::GameData()
{
	Clear();
}

GameData::~GameData()
{

}

void GameData::Clear()
{
	m_bSceneInit = false;
}