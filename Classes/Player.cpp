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

//初始化
bool CPlayer::iniWithPlayerType(PlayerType type)
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

	initFSM();

	m_progess = CProgress::create("small-enemy-progress-bg.png", "small-enemy-progress-fill.png");
	m_progess->setPosition(200, 200);
	this->addChild(m_progess);
	return true;
} 


//添加动画
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

//播放特效
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
	if (ptrPlayer && ptrPlayer->iniWithPlayerType(type))
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

//初始化状态机
void CPlayer::initFSM()
{
	std::string strtmp("idle");
	m_fsm = CFSM::create(strtmp);
	m_fsm->retain();
	auto onIdle = [&]()
	{
		cocos2d::log("onIdle: Enter idle");
		this->stopActionByTag(WALKING);
		auto sfName = String::createWithFormat("%s-1-1.png", m_name.c_str());
		auto spriteName = SpriteFrameCache::getInstance()->getSpriteFrameByName(sfName->getCString());
		this->setSpriteFrame(spriteName);
	};
	m_fsm->setOnEnter("idle", onIdle);

	auto onAttacking = [&]()
	{
		cocos2d::log("onAttacking: Enter Attacking");
		auto animate = this->getAnimateByType(ATTACKING); 
		auto func = [&]()
		{
			this->m_fsm->doEvent("stop");
		};
		auto callback = CallFunc::create(func);
		auto seq = Sequence::create(animate, callback, nullptr);
		this->runAction(seq);
	};
	m_fsm->setOnEnter("attacking", onAttacking);

	auto onBeingHit = [&]()
	{
		cocos2d::log("onBeingHit: OnEnter");
		auto animate = this->getAnimateByType(BEINGHIT);
		auto func = [&]()
		{
			this->m_fsm->doEvent("stop");
		};
		auto wait = DelayTime::create(0.6f);
		auto callback = CallFunc::create(func);
		auto seq = Sequence::create(wait, animate, callback, nullptr);
		this->runAction(seq);
	};
	m_fsm->setOnEnter("beinghit", onBeingHit);

	auto onDead = [&]()
	{
		this->setCanAttack(false);
		cocos2d::log("onDead: Enter Dead");
		auto animate = getAnimateByType(DEAD);
		auto func = [&]()
		{
			cocos2d::log("a charactor died");
			cocos2d::NotificationCenter::getInstance()->postNotification("enemyDead", this);
			this->removeFromParentAndCleanup(true); 
		};
		auto blink = cocos2d::Blink::create(3, 5);
		auto callback = CallFunc::create(func);
		auto seq = Sequence::create(animate, blink, callback, nullptr);
		this->stopAllActions();
		this->runAction(seq);
		m_progess->setVisible(false);
	};
	m_fsm->setOnEnter("dead", onDead);
}

void CPlayer::walkTo(cocos2d::Vec2& dest)
{
	std::function<void()> onWalk = CC_CALLBACK_0(CPlayer::OnWalk, this, dest);
	m_fsm->setOnEnter("walking", onWalk);
	m_fsm->doEvent("walk");
}

void CPlayer::OnWalk(cocos2d::Vec2& dest)
{
	cocos2d::log("onIdle: Enter walk");
	this->stopActionByTag(WALKTO_TAG);
	auto& curPos = this->getPosition();

	if (curPos.x  > dest.x)
	{
		this->setFlippedX(true);
	}
	else
	{
		this->setFlippedX(false);
	}
	
	auto& diff = dest - curPos;
	auto time = diff.getLength() / m_speed;
	auto move = MoveTo::create(time, dest);
	auto func = [&]()
	{
		m_fsm->doEvent("stop");
	};
	auto callback = CallFunc::create(func);
	auto seq = Sequence::create(move, callback, nullptr);
	seq->setTag(WALKTO_TAG);
	this->runAction(seq);
	this->PlayAnimationForever(0);
}

Animate* CPlayer::getAnimateByType(CPlayer::AnimationType type)
{
	if (type < 0 || type >= m_animationNum)
	{
		cocos2d::log("illegal animation index!");
		return nullptr;
	}

	auto str = String::createWithFormat("%s-%s", m_name.c_str(), m_animationNames[type].c_str())->getCString();
	auto animation = AnimationCache::getInstance()->getAnimation(str);
	auto animate = Animate::create(animation);
	animate->setTag(type);
	return animate;
}

//单例
CPlayer* CPlayer::m_ptrInstance = NULL;
CPlayer* CPlayer::getInstance()
{
	if (NULL == m_ptrInstance)
	{
		m_ptrInstance = new CPlayer();
	}
	return m_ptrInstance;
}