#pragma once
#include "cocos2d.h"
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
public:
	bool iniWithPlayerTyer(PlayerType type);

	static CPlayer* create(PlayerType type);
};

