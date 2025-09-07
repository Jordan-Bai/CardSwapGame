#pragma once

//#include "Card.h"
#include "ActiveCard.h"

#include <iostream>

enum AbilityTrigger
{
	OnPlayed, 
	OnDeath, 
	OnTurnStarts, 
	OnTurnEnds, 
	OnAttack, 
	OnAttacked, 
	OnFlippedTo, 
	OnActivate, 
	OnBoardUpdates
};

struct Ability
{
	AbilityTrigger trigger;
	std::function<void(ActiveCreature* owner)> effect;
	std::function<void(ActiveCreature* owner, ActiveCard* other)> attackEffect;
	std::function<void()> caller;
	std::function<void(ActiveCard* other)> attackCaller;

	Ability(AbilityTrigger _trigger, std::function<void(ActiveCreature* owner)> _effect);
	Ability(AbilityTrigger _trigger, std::function<void(ActiveCreature* owner, ActiveCard* other)> _attackEffect);

	void Init(ActiveCreature* owner);
};

//struct AbilityEffect
//{
//	void OnTriggered(ActiveCard* owningCard);
//	void OnTriggered(ActiveCard* owningCard, ActiveCard* targetCard);
//};

