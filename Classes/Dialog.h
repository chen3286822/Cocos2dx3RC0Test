#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

struct TextButton
{
	std::string m_strText;
	cocos2d::ccMenuCallback m_iCallback;
};
typedef std::vector<TextButton> VTextButton;

class Dialog : public cocos2d::LayerColor, public cocos2d::extension::EditBoxDelegate
{
	CC_SYNTHESIZE(bool, m_bNewRecord, NewRecord);
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

	void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::TouchEventType type);

	virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::extension::EditBox* editBox);

	void AddButton(const char* text, const cocos2d::ccMenuCallback& callback);
	void SetTitle(std::string title);
	void SetContent(std::string content);
private:
	std::string m_strTitle;
	std::string m_strContent;
	VTextButton m_vButtons;
	cocos2d::ui::Button* m_pButton{ nullptr };
	//ui::TextField has some bugs, use TextFieldTTF instead
	//cocos2d::ui::TextField* m_pTextField{ nullptr };
	cocos2d::extension::EditBox* m_pEditBox{ nullptr };
	cocos2d::Point m_iEditBoxPos;
	std::string m_strName;
	bool m_bButtonTouched{ false };
	bool m_bTextFieldTouched{ false };

	cocos2d::EventListenerTouchOneByOne* m_pTouchListener{ nullptr };

	cocos2d::ui::Layout* m_pLayout{ nullptr };
};

#endif