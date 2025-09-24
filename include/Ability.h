#pragma once

#include "AbilityEffect.h"
//#include "ActiveCard.h"

#include <iostream>

enum AbilityTrigger
{
	OnPlayed, 
	OnStack,
	OnStackMaxed,
	OnDeath, 
	OnAttack, 
	OnAttacked, 
	OnFlippedTo, 
	OnActivate, 
	OnTurnStarts,
	OnTurnEnds,
	OnCardDies,
	OnBoardUpdates
};

struct Ability
{
	AbilityTrigger trigger;
	AbilityEffect* effect;
	std::string description;

	Ability(AbilityTrigger _trigger, AbilityEffect* effect);

	void Init(ActiveCreature* owner);
};
