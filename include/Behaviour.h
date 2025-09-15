#pragma once

//#include "BoardManager.h"
//#include "Player.h"

extern int behaviours;

class Player;

class Behaviour
{
public:
	Behaviour();
	virtual ~Behaviour();
	virtual void DoAction(Player* playerRef) = 0;
};

class PlayCard : public Behaviour
{
public:
	int m_card;
	int m_target;
public:
	PlayCard(int cardToPlay, int targetSlot, int cardCost)
		: m_card(cardToPlay), m_target(targetSlot)
	{
	}

	void DoAction(Player* playerRef) override;
};

class FlipCard : public Behaviour
{
public:
	int m_target;
public:
	FlipCard(int targetSlot, int flipCost)
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

class SwapSlots : public Behaviour
{
	int m_target1;
	int m_target2;
public:
	SwapSlots(int slot1, int slot2)
		: m_target1(slot1), m_target2(slot2)
	{
	}

	void DoAction(Player* playerRef) override;
};