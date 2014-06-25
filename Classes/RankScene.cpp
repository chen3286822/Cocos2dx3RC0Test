#include "RankScene.h"
#include "MainTitleScene.h"
#include "Unity.h"

USING_NS_CC;
USING_NS_CC_EXT;

Scene* RankLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = RankLayer::create();

	// add layer as a child to scene
	scene->addChild(layer, 0, eChild_RankLayer);

	// return the scene
	return scene;
}

bool RankLayer::init()
{
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	//List
	auto tableView = TableView::create(this, Size(visibleSize.width - 20, visibleSize.height - 50));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(Point(10 + origin.x, 50 + origin.y));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tableView);
	tableView->reloadData();


	auto labelBack = LabelTTF::create("Back", unity::GetDefaultFontType(), 25);
	auto itemBack = MenuItemLabel::create(labelBack, CC_CALLBACK_1(RankLayer::BackToMainTitle, this));
	itemBack->setAnchorPoint(Point(1,0));
	itemBack->setPosition(Point(visibleSize.width + origin.x - 20, origin.y + 10));
	auto menu = Menu::create(itemBack, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1, eChild_Menu);
	return true;
}

void RankLayer::BackToMainTitle(cocos2d::Ref* pSender)
{
	Director::getInstance()->replaceScene(MainTitle::createScene());
}

void RankLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size RankLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	return Size(visibleSize.width - 20, 80);
}

TableViewCell* RankLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto record = unity::UserData::getInstance()->GetRecord(idx+1);
	auto string = String::createWithFormat("%-15s : %d", record->m_strName.c_str(),record->m_nScore);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	
	TableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = new TableViewCell();
		cell->autorelease();

		auto label = LabelTTF::create(string->getCString(), "fonts/Marker Felt.ttf", 24.0);
		label->setPosition(Point(visibleSize.width/2, 40));
		label->setAnchorPoint(Point(0.5,0.5));
		label->setTag(eChild_CellLabel);
		cell->addChild(label);
	}
	else
	{
		auto label = (LabelTTF*)cell->getChildByTag(eChild_CellLabel);
		label->setString(string->getCString());
	}


	return cell;
}

ssize_t RankLayer::numberOfCellsInTableView(TableView *table)
{
	return 10;
}