#pragma  once
#include "cocos2d.h"

USING_NS_CC;

//ÑªÌõÀà
class CProgress : public cocos2d::Sprite
{
public:
	CProgress();
	~CProgress();

	bool init(const char* background, const char* fillname);
	/*
	the inputs are SpriteFrame Names.
	they should be loaded into SpriteFrameCache before calling this.
	*/
	static CProgress* create(const char* background, const char* fill);

	void setFill(cocos2d::ProgressTimer* fill){ m_fill = fill; }

	void setProgress(float percetage){ m_fill->setPercentage(percetage); }
private:
	cocos2d::ProgressTimer* m_fill;
};
