#pragma once

//#include "BoardManager.h"
//#include "Player.h"
class Player;

class Behaviour
{
protected:
	//int m_cost;
public:
	virtual void DoAction(Player* playerRef) = 0;
};

class PlayCard : public Behaviour
{
	int m_card;
	int m_target;
public:
	PlayCard(int cardToPlay, int targetSlot, int cardCost)
		: m_card(cardToPlay), m_target(targetSlot)
	{
		//m_cost = cardCost;
	}

	void DoAction(Player* playerRef) override;
};

class FlipCard : public Behaviour
{
	int m_target;
public:
	FlipCard(int targetSlot, int flipCost)
		: m_target(targetSlot)
	{
		//m_cost = flipCost;
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
		//m_cost = 0;
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
		//m_cost = 0;
	}

	void DoAction(Player* playerRef) override;
};