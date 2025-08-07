#pragma once

#include <vector>

struct CreatureData
{
	int hp;
	int atk;

	CreatureData(int health, int attack)
		:hp(health), atk(attack)
	{
	}
};

struct CardData
{
	int cost;
	CreatureData* frontCreature;

	CardData(int _cost, CreatureData* _frontCreature)
		:cost(_cost), frontCreature(_frontCreature)
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

	ActiveCard(CardData* data, int slot, int side);

	CardData* GetData();
	int GetCost();
	int GetHP();
	int GetAtk();
	std::vector<int> GetTargets();

	void TakeDamage(int damage);
};