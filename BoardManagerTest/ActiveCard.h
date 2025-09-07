#pragma once

#include "Card.h"

#include <vector>
#include <functional>

class ActiveCard;

class ActiveCreature
{
	CreatureData* m_data;

	bool m_overrideStats = false;
	int m_hpOverride;
	int m_atkOverride;
	int m_fCostOverride;

public:
	// Ability triggers:
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

	ActiveCreature(CreatureData* data);

	int GetHP();
	int GetAtk();
	int GetFlipCost();

	void SetHP(int hp);
	void SetAtk(int atk);
	void SetFlipCost(int fCost);
};

class ActiveCard
{
	CardData* m_data;
	ActiveCreature* m_frontFace;
	ActiveCreature* m_backFace;

	int m_damageTaken;
	bool m_frontActive = true;
	bool m_flippedThisTurn = false;

public:
	int m_slot;
	int m_side;

	ActiveCard(CardData* data, int slot, int side);
	ActiveCard(const ActiveCard& other); //copy constructor
	ActiveCard& operator=(const ActiveCard& ref) = delete; //assignment operator
	~ActiveCard();

	CardData* GetData();
	ActiveCreature* GetCurrentFace();
	ActiveCreature* GetOpositeFace();
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