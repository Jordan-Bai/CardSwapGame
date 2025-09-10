#pragma once

#include "ActiveCard.h"

struct AbilityEffect
{
	std::function<void(ActiveCreature* owner)> effect;
	std::function<void(ActiveCreature* owner, ActiveCard* other)> attackEffect;
	std::function<void(ActiveCreature* owner, CardData* card)> stackEffect;

	virtual void Init(ActiveCreature* owner);
	virtual std::string GetIcon() = 0;
};

struct ChangeStats : public AbilityEffect
{
	int hp;
	int atk;
	int fCost;

	ChangeStats(int health, int attack, int flipCost);

	virtual std::string GetIcon() override;
};

struct Flip : public AbilityEffect
{
	Flip();

	virtual std::string GetIcon() override;
};

struct Heal : public AbilityEffect
{
	int hp;

	Heal(int healAmount);

	virtual std::string GetIcon() override;
};

struct GainEnergy : public AbilityEffect
{
	int energy;

	GainEnergy(int energyToGain);

	virtual std::string GetIcon() override;
};

struct PickupCard : public AbilityEffect
{
	CardData* card;

	PickupCard(CardData* cardToPickup);

	virtual std::string GetIcon() override;
};

struct RandomizeStats : public AbilityEffect
{
	int min;
	int max;

	RandomizeStats(int minInclusive, int maxInclusive);

	virtual std::string GetIcon() override;
};

struct CopyCards : public AbilityEffect
{
	std::vector<int> copyTargets;

	CopyCards(std::vector<int> slotsToCopy);

	virtual void Init(ActiveCreature* owner) override;
	virtual std::string GetIcon() override;
};

struct A : public AbilityEffect
{

	virtual std::string GetIcon() override;
};
