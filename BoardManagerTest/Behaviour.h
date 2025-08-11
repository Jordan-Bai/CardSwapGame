#pragma once

//#include "BoardManager.h"
//#include "Player.h"
class Player;

class Behaviour
{
public:
	//Behaviour();

	virtual void DoAction(Player* playerRef) = 0;
};

class PlayCard : public Behaviour
{
	int m_card;
	int m_target;
public:
	PlayCard(int cardToPlay, int targetSlot)
		: m_card(cardToPlay), m_target(targetSlot)
	{
	}

	void DoAction(Player* playerRef) override;
};

class FlipCard : public Behaviour
{
	int m_target;
public:
	FlipCard(int targetSlot)
		: m_target(targetSlot)
	{
	}

	void DoAction(Player* playerRef) override;
};

class DestroyCard : public Behaviour
{
	int m_target;
public:
	DestroyCard(int targetSlot)
		: m_target(targetSlot)
	{
	}

	void DoAction(Player* playerRef) override;
};