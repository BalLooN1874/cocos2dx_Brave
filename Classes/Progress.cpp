#include "Progress.h"


CProgress::CProgress()
{

}
CProgress::~CProgress()
{

}

bool CProgress::init(const char* background, const char* fillname)
{
	this->initWithSpriteFrameName(background);
	auto fill = cocos2d::ProgressTimer::create(Sprite::createWithSpriteFrameName(fillname));
	this->setFill(fill);
	this->addChild(fill);

	fill->setType(cocos2d::ProgressTimer::Type::BAR);
	fill->setMidpoint(Point(0, 0.5));
	fill->setBarChangeRate(Point(1.0, 0)); 
	fill->setPosition(this->getContentSize() / 2); 
	fill->setPercentage(100);
	return true;
}
/*
the inputs are SpriteFrame Names.
they should be loaded into SpriteFrameCache before calling this.
*/
CProgress* CProgress::create(const char* background, const char* fill)
{
	CProgress* progress = new CProgress();
	if (progress && progress->init(background, fill))
	{
		progress->autorelease();
		return progress;
	}
	else
	{
		delete progress;
		progress = nullptr;
		return nullptr;
	}
}