#include "Player.h"


CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
}


bool CPlayer::iniWithPlayerTyer(PlayerType type)
{
	std::string spName = "";
	switch (type)
	{
	case CPlayer::PLAYER:
		spName = "player1-1-1.png";
		break;
	case CPlayer::ENEMY_1:
		spName = "enemy1-1-1.png";
		break;
	case CPlayer::ENEMY_2:
		spName = "enemy2-1-1.png";
		break;
	default:
		break;
	}
	this->initWithSpriteFrameName(spName);
	return true;
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