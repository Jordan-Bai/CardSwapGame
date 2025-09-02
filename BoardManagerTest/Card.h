#pragma once

#include <vector>
#include <functional>
#include <string>

class ActiveCard;

struct CreatureData
{
	int hp;
	int atk;
	int fCost;
	std::string name;

	// Ability triggers:
	//std::function<void(ActiveCard* owningCard)> OnPlayed;
	std::function<void()> OnPlayed;
	std::function<void()> OnDeath;
	std::function<void(ActiveCard* target)> OnAttack;
	std::function<void(ActiveCard* attacker)> OnAttacked;
	std::function<void()> OnFlippedTo;
	std::function<void()> OnActivate;

	std::function<void()> OnTurnStarts;
	std::function<void()> OnTurnEnds;
	std::function<void()> OnCardDies;
	std::function<void()> OnBoardUpdates;
	// Might want to add OnStack/ OnPickup

	CreatureData(int health, int attack, int flipCost)
		:hp(health), atk(attack), fCost(flipCost), name("[EMPTY]")
	{
	}
};

struct CardData
{
	int cost;
	CreatureData* frontCreature;
	CreatureData* backCreature;
	bool isTemp = false;

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