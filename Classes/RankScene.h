#ifndef __RANK_SCENE_H__
#define __RANK_SCENE_H__


#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class RankLayer : public cocos2d::LayerColor, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
	enum eChild
	{
		eChild_RankLayer,
		eChild_TableView,
		eChild_Menu,
		eChild_CellLabel,
	};

	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);

	void BackToMainTitle(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(RankLayer);
};

#endif