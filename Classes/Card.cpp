#include "Card.h"
#include "Unity.h"

USING_NS_CC;

Card* Card::create(int num,int length)
{
	Card* card = new Card();
	if(card && card->init(length))
	{
		card->autorelease();
		card->setNum(num);
		return card;
	}

	CC_SAFE_DELETE(card);
	return nullptr;
}

Card::Card():
	m_nNum(0),
	m_pCardTex(nullptr)
{

}

Card::~Card()
{
	//CC_SAFE_RELEASE(m_pCardTex);
}

void Card::setNum(int var, bool initNum)
{
	m_nNum = var;
	auto label = dynamic_cast<LabelTTF*>(getChildByTag(eChild_Label));
	if (label && m_pCardTex)
	{
		char temp[50];
		sprintf(temp, "%d", m_nNum);
		label->setString(temp);
		switch (m_nNum)
		{
		case 2:
		{
				  m_pCardTex->setColor(Color3B(238, 228, 218));
				  label->setColor(Color3B(119,110,101));
		}
			break;
		case 4:
		{
				  m_pCardTex->setColor(Color3B(236, 224, 200));
				  label->setColor(Color3B(119, 110, 101));
		}
			break;
		case 8:
		{
				  m_pCardTex->setColor(Color3B(242, 177, 121));
				  label->setColor(Color3B(255, 247, 235));
		}
			break;
		case 16:
		{
				  m_pCardTex->setColor(Color3B(245, 149, 99));
				  label->setColor(Color3B(255, 247, 235));
		}
			break;
		case 32:
		{
				   m_pCardTex->setColor(Color3B(245, 124, 95));
				   label->setColor(Color3B(255, 247, 235));
		}
			break;
		case 64:
		{
				   m_pCardTex->setColor(Color3B(246, 93, 59));
				   label->setColor(Color3B(255, 247, 235));
		}
			break;
		case 128:
		{
				   m_pCardTex->setColor(Color3B(237, 206, 113));
				   label->setColor(Color3B(255, 247, 235));
		}
			break;
		case 256:
		{
					m_pCardTex->setColor(Color3B(236, 200, 80));
					label->setColor(Color3B(255, 247, 235));
		}
			break;
		case 512:
		{
					m_pCardTex->setColor(Color3B(237, 200, 80));
					label->setColor(Color3B(249, 246, 242));
		}
			break;
		case 1024:
		{
					m_pCardTex->setColor(Color3B(0, 122, 204));
					label->setColor(Color3B(249, 246, 242));
		}
			break;
		case 2048:
		{
					 m_pCardTex->setColor(Color3B(185, 117, 181));
					 label->setColor(Color3B(255, 216, 0));
		}
			break;
		default:
		{
				   m_pCardTex->setColor(Color3B(236, 200, 80));
				   label->setColor(Color3B(255, 247, 235));
		}
			break;
		}
		if (!initNum)
		{
			float _originalScale = label->getScale();
			auto zoomAction1 = ScaleTo::create(0.1f, _originalScale * 1.2f);
			auto zoomAction2 = ScaleTo::create(0.1f, _originalScale);
			auto sequenceAction = Sequence::create(zoomAction1, zoomAction2, NULL);
			label->runAction(sequenceAction);
		}
	}
}

int Card::getNum()
{
	return m_nNum;
}

bool Card::init(int length)
{
	//其实Node::init一直都返回true
	if (!Node::init())
	{
		return false;
	}


	m_pCardTex = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("roundedrectangle.png"));
	m_pCardTex->setScale(length / m_pCardTex->getTextureRect().size.width, length / m_pCardTex->getTextureRect().size.height);
	m_pCardTex->setColor(Color3B::ORANGE);
	m_pCardTex->setAnchorPoint(cocos2d::Point(0, 0));
	addChild(m_pCardTex, 1, eChild_CardTex);

	auto label = LabelTTF::create("", "fonts/arial.ttf", length / 2);
	//label->setAnchorPoint(cocos2d::Point(0.5, 0.5));
	label->setPosition(Point(length / 2, length/2));
	addChild(label, 1, eChild_Label);
	return true;
}