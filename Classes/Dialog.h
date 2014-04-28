#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "cocos2d.h"

struct TextButton
{
	std::string m_strText;
	cocos2d::ccMenuCallback m_iCallback;
};
typedef std::vector<TextButton> VTextButton;

class Dialog : public cocos2d::LayerColor
{
public:
	CREATE_FUNC(Dialog);

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	//´¥Ãþ¿ØÖÆ
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

	void AddButton(const char* text, const cocos2d::ccMenuCallback& callback);
	void SetTitle(std::string title);
	void SetContent(std::string content);
private:
	std::string m_strTitle;
	std::string m_strContent;
	VTextButton m_vButtons;
	cocos2d::Menu* m_pMenu;
	bool m_bMenuTouched{ false };

	cocos2d::EventListenerTouchOneByOne* m_pTouchListener{nullptr};
};

#endif