#pragma once

//#include "Ability.h"
#include <vector>
//#include <functional>
#include <string>

extern int cardDataInstances;

struct Ability;

struct CardData;

struct StackOptions
{
	bool canStack = false;
	int stackLimit = 1000;
};

struct CreatureData
{
	int hp;
	int atk;
	int fCost;
	int aCost;
	std::string name;
	std::vector<Ability*> abilities;
	StackOptions stackOptions;

	CardData* owner;

	CreatureData(int health, int attack, int flipCost, int abilityCost = 0)
		:hp(health), atk(attack), fCost(flipCost), aCost(abilityCost), name("[EMPTY]"), owner(nullptr)
	{
	}
};

struct CardData
{
	int cost;
	CreatureData* frontCreature;
	CreatureData* backCreature;
	bool isTemp = false;

	int cardID;

	CardData(int _cost, CreatureData* _frontCreature, CreatureData* _backCreature)
		:cost(_cost), frontCreature(_frontCreature), backCreature(_backCreature), cardID(cardDataInstances)
	{
		if (frontCreature != nullptr)
		{
			frontCreature->owner = this;
		}
		if (backCreature != nullptr)
		{
			backCreature->owner = this;
		}

		cardDataInstances++;
	}
	CardData(CardData* other)
		:cost(other->cost), frontCreature(other->frontCreature), backCreature(other->backCreature), cardID(other->cardID)
	{
		if (frontCreature != nullptr)
		{
			frontCreature->owner = this;
		}
		if (backCreature != nullptr)
		{
			backCreature->owner = this;
		}
	}
};
