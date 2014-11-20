#include "Player.h"


CPlayer::CPlayer() : m_seq(nullptr), m_speed(100)
{
}


CPlayer::~CPlayer()
{
	if (m_ptrInstance)
	{
		delete m_ptrInstance;
		m_ptrInstance = nullptr;
	}
}


bool CPlayer::iniWithPlayerTyer(PlayerType type)
{
	std::string spName = "";
	m_type = type;
	int animationFrameNum[5] = { 4, 4, 4, 2, 4 };
	int animationFrameNum2[5] = { 3, 3, 3, 2, 0 };

	//setup according to PlayerType
	switch (type)
	{
	case CPlayer::PLAYER:
		spName = "player1-1-1.png";
		m_name = "player1";
		m_animationNum = 5;
		m_anmationFrameNum.assign(animationFrameNum, animationFrameNum + 5);
		break;
	case CPlayer::ENEMY_1:
		spName = "enemy1-1-1.png";
		m_name = "enemy1";
		m_animationNum = 4;
		m_anmationFrameNum.assign(animationFrameNum2, animationFrameNum2 + 5);
		break;
	case CPlayer::ENEMY_2:
		spName = "enemy2-1-1.png";
		m_name = "enemy2";
		m_animationNum = 4;
		m_anmationFrameNum.assign(animationFrameNum2, animationFrameNum2 + 5);
		break;
	default:
		break;
	}
	this->initWithSpriteFrameName(spName);
	std::string animationNames[] = { "walk", "attack", "dead", "hit", "skill" };
	m_animationNames.assign(animationNames, animationNames + 5);
	this->addAnimation();
	return true;
} 
void CPlayer::addAnimation()
{
	auto animation  = AnimationCache::getInstance()->getAnimation(String::createWithFormat("%s-%s", m_name.c_str(), m_animationNames[0])->getCString());
		
	if (animation)
	{
		return;
	}
	for (int i = 0; i < m_animationNum; ++i)
	{
		auto animation = Animation::create();
		animation->setDelayPerUnit(0.2f);
		//put frames into animation
		for (int j = 0; j < m_anmationFrameNum[i]; ++j)
		{
			auto sfName = String::createWithFormat("%s-%d-%d.png", m_name.c_str(), i + 1, j + 1)->getCString();
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(sfName));
		}
		//put the animation int cache
		AnimationCache::getInstance()->addAnimation(animation, String::createWithFormat("%s-%s", m_name.c_str(),m_animationNames[i].c_str())->getCString());
	}
}

void CPlayer::PlayAnimationForever(int index)
{
	if (index < 0 || index >= m_animationNum)
	{
		log("illegal animation index!");
		return;
	}
	auto str = String::createWithFormat("%s-%s", m_name.c_str(), m_animationNames[index].c_str())->getCString();
	auto animation = AnimationCache::getInstance()->getAnimation(str);
	auto animate = Animate::create(animation);
	this->runAction(RepeatForever::create(animate));
}
CPlayer* CPlayer::create(PlayerType type)
{
	CPlayer* ptrPlayer = new CPlayer();
	if (ptrPlayer && ptrPlayer->iniWithPlayerTyer(type))
	{
		ptrPlayer->autorelease();
		return ptrPlayer; 
	}
	else
	{
		delete ptrPlayer;
		ptrPlayer = NULL;
		return NULL;
	}
}
void CPlayer::walkTo(cocos2d::Vec2& dest)
{
	//stop current moving action, if any
	if (m_seq)
	{
		this->stopAction(m_seq);
	}
	auto& curPos = this->getPosition();
	//flip when moving backward
	if (curPos.x > dest.x)
		this->setFlippedX(true);
	else
		this->setFlippedX(false);
	
	//calculate the time needed to move
	auto& diff = dest - curPos;
	auto time = diff.getLength() / m_speed;
	auto move = MoveTo::create(time, dest);
	//lambda function
	auto func = [&]()
	{
		this->stopAllActions();
		m_seq = nullptr;
	};
	auto callback = CallFunc::create(func);
	m_seq = Sequence::create(move, callback, nullptr);
	this->runAction(m_seq);
	this->PlayAnimationForever(0);
}

CPlayer* CPlayer::m_ptrInstance = NULL;
CPlayer* CPlayer::getInstance()
{
	if (NULL == m_ptrInstance)
	{
		m_ptrInstance = new CPlayer();
	}
	return m_ptrInstance;
}