#pragma once

#include "Card.h"

#include <vector>
#include <functional>

class ActiveCard;

class ActiveCreature
{
	CreatureData* m_data;
	ActiveCard* m_owner;

	bool m_overrideStats = false;
	int m_hpOverride;
	int m_atkOverride;
	int m_fCostOverride;
	int m_aCostOverride;

public:
	// Ability triggers:
	std::function<void(ActiveCreature* owner)> OnPlayed;
	std::function<void(ActiveCreature* owner)> OnDeath;
	std::function<void(ActiveCreature* owner, ActiveCard* target)> OnAttack;
	std::function<void(ActiveCreature* owner, ActiveCard* attacker)> OnAttacked;
	std::function<void(ActiveCreature* owner)> OnFlippedTo;
	std::function<void(ActiveCreature* owner)> OnActivate;

	std::function<void(ActiveCreature* owner)> OnTurnStarts;
	std::function<void(ActiveCreature* owner)> OnTurnEnds;
	std::function<void(ActiveCreature* owner)> OnCardDies;
	std::function<void(ActiveCreature* owner)> OnBoardUpdates;
	// Might want to add OnStack/ OnPickup

	ActiveCreature(CreatureData* data, ActiveCard* owner);
	ActiveCreature(const ActiveCreature& other, ActiveCard* owner);

	int GetHP();
	int GetAtk();
	int GetFlipCost();
	int GetAbilityCost();

	void SetHP(int hp);
	void SetAtk(int atk);
	void SetFlipCost(int fCost);
	void SetAbilityCost(int aCost);

	bool HasActivateAbility();

	ActiveCard* GetOwner();
	void SetOwner(ActiveCard* owner);
};

class BoardManager;

class ActiveCard
{
	CardData* m_data;
	ActiveCreature* m_frontFace;
	ActiveCreature* m_backFace;

	BoardManager* m_boardRef;

	int m_damageTaken;
	bool m_frontActive = true;
	bool m_flippedThisTurn = false;

public:
	int m_slot;
	int m_side;

	ActiveCard(CardData* data, int slot, int side, BoardManager* boardRef);
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
	int GetAbilityCost();

	BoardManager* GetBoard();
	void SetBoard(BoardManager* board);

	int GetDamageTaken();
	bool GetFrontActive();
	bool CanFlip();

	std::vector<int> GetTargets();

	void TakeDamage(int damage);
	void Heal(int healAmount);
	void Flip(); // Should this return a bool since Player.FlipCard() already uses CanFlip()?
	//bool ActivateEffect();

	//void OnStartTurn();

	// Triggers
	void OnPlayed();
	void OnDeath();
	void OnAttack(ActiveCard* target);
	void OnAttacked(ActiveCard* target);
	void OnFlip();
	void OnActivate();

	void OnTurnStarts();
	void OnTurnEnds();
	void OnCardDies();
	void OnBoardUpdates();
};