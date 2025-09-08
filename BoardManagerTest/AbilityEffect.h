#pragma once

#include "ActiveCard.h"

struct AbilityEffect
{
	std::function<void(ActiveCreature* owner)> effect;
	std::function<void(ActiveCreature* owner, ActiveCard* other)> attackEffect;
};

struct ChangeStats : public AbilityEffect
{
	int hp;
	int atk;
	int fCost;

	ChangeStats(int health, int attack, int flipCost);
};

struct Flip : public AbilityEffect
{
	Flip();
};

struct Heal : public AbilityEffect
{
	int hp;

	Heal(int healAmount);
};

struct GainEnergy : public AbilityEffect
{
	int energy;

	GainEnergy(int energyToGain);
};

struct PickupCard : public AbilityEffect
{
	CardData* card;

	PickupCard(CardData* cardToPickup);
};

struct A : public AbilityEffect
{

};

//struct CopyCards : public AbilityEffect
//{
//	std::vector<int> copyTargets;
//
//	CopyCards(std::vector<int> slotsToCopy);
//};
