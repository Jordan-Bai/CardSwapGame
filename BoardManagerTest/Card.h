#pragma once

#include <vector>
#include <functional>

//class ActiveCard;

struct CreatureData
{
	int hp;
	int atk;
	int fCost;

	// Ability triggers:
	//std::function<void(ActiveCard* owningCard)> OnPlayed;
	std::function<void(int slot, int side)> OnPlayed;
	std::function<void(int slot, int side)> OnDeath;
	std::function<void(int slot, int side)> OnTurnStarts;
	std::function<void(int slot, int side)> OnTurnEnds;

	std::function<void(int slot, int side)> OnAttack;
	std::function<void(int slot, int side)> OnAttacked;
	std::function<void(int slot, int side)> OnFlippedTo;
	std::function<void(int slot, int side)> OnActivate;
	std::function<void(int slot, int side)> OnBoardUpdates;
	// Might want to add OnStack/ OnPickup

	CreatureData(int health, int attack, int flipCost)
		:hp(health), atk(attack), fCost(flipCost)
	{
	}
};

struct CardData
{
	int cost;
	CreatureData* frontCreature;
	CreatureData* backCreature;

	CardData(int _cost, CreatureData* _frontCreature, CreatureData* _backCreature)
		:cost(_cost), frontCreature(_frontCreature), backCreature(_backCreature)
	{
	}
	CardData(CardData* other)
		:cost(other->cost), frontCreature(other->frontCreature), backCreature(other->backCreature)
	{
	}
};

class ActiveCard
{
	CardData* m_data;

	int m_damageTaken;
	bool m_frontActive = true;
	bool m_flippedThisTurn = false;

public:
	int m_slot;
	int m_side;
	//bool m_frontActive = true;
	//bool m_flippedThisTurn = false;

	ActiveCard(CardData* data, int slot, int side);
	ActiveCard(ActiveCard* other);

	CardData* GetData();
	CreatureData* GetCurrentFace();
	int GetCost();
	int GetHP();
	int GetAtk();
	int GetFlipCost();

	int GetDamageTaken();
	bool GetFrontActive();
	bool CanFlip();

	std::vector<int> GetTargets();

	void TakeDamage(int damage);
	bool Flip(); // Should this return a bool since Player.FlipCard() already uses CanFlip()?

	void OnStartTurn();
};