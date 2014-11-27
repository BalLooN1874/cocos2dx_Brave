#pragma once
#include "cocos2d.h"
#include "FSM.h"
#include "Progress.h"


USING_NS_CC;

class CPlayer : public Sprite
{
public:
	CPlayer();
	~CPlayer();
public:
	enum PlayerType
	{
		PLAYER,
		ENEMY_1,
		ENEMY_2
	};

	enum ActionTag
	{
		WALKTO_TAG = 100
	};

	enum  AnimationType
	{
		WALKING = 0,
		ATTACKING,
		DEAD,
		BEINGHIT,
		SKILL
	};

public:
	bool iniWithPlayerType(PlayerType type);
	void addAnimation();
	void PlayAnimationForever(int index);
	void walkTo(cocos2d::Vec2& dest);
	void OnWalk(cocos2d::Vec2& dest);
	void initFSM();
	Animate* getAnimateByType(AnimationType type);
	void	setCanAttack(bool canAttack){ m_isCanAttack = canAttack; }
	static CPlayer* create(PlayerType type);
	static CPlayer* getInstance();
private:
	PlayerType							m_type;
	std::string								m_name;
	int										m_animationNum;
	std::vector<int>					m_anmationFrameNum;
	std::vector<std::string>			m_animationNames;
	cocos2d::Action*					m_seq;
	float										m_speed;
	CFSM*									m_fsm;
	bool										m_isCanAttack;
	CProgress*							m_progess;
	static CPlayer*						m_ptrInstance;
};

