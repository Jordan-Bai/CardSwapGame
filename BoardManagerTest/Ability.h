#pragma once

#include "AbilityEffect.h"
//#include "ActiveCard.h"

#include <iostream>

enum AbilityTrigger
{
	OnPlayed, 
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
	//std::function<void(ActiveCreature* owner)> effect;
	//std::function<void(ActiveCreature* owner, ActiveCard* other)> attackEffect;
	//std::function<void()> caller;
	//std::function<void(ActiveCard* other)> attackCaller;

	Ability(AbilityTrigger _trigger, AbilityEffect* effect);
	//Ability(AbilityTrigger _trigger, std::function<void(ActiveCreature* owner)> _effect);
	//Ability(AbilityTrigger _trigger, std::function<void(ActiveCreature* owner, ActiveCard* other)> _attackEffect);

	void Init(ActiveCreature* owner);
};
