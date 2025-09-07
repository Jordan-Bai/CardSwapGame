#pragma once

//#include "Ability.h"
#include <vector>
//#include <functional>
#include <string>

struct Ability;

struct CreatureData
{
	int hp;
	int atk;
	int fCost;
	std::string name;
	std::vector<Ability*> abilities;

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
