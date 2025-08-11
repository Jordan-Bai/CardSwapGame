#pragma once

#include <vector>

struct CreatureData
{
	int hp;
	int atk;
	int fCost;

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
};

class ActiveCard
{
	CardData* m_data;

	int m_damageTaken;

public:
	int m_slot;
	int m_side;
	bool m_frontActive = true;

	ActiveCard(CardData* data, int slot, int side);

	CardData* GetData();
	CreatureData* GetCurrentFace();
	int GetCost();
	int GetHP();
	int GetAtk();
	int GetFlipCost();
	int GetDamageTaken();


	std::vector<int> GetTargets();

	void TakeDamage(int damage);
};