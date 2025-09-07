#include "Ability.h"

Ability::Ability(AbilityTrigger _trigger, std::function<void(ActiveCreature* owner)> _effect)
	: trigger(_trigger), effect(_effect)
{	
}

Ability::Ability(AbilityTrigger _trigger, std::function<void(ActiveCreature* owner, ActiveCard* other)> _attackEffect)
	: trigger(_trigger), attackEffect(_attackEffect)
{
}

void Ability::Init(ActiveCreature* owner)
{
	caller = [&owner, this]()
		{
			effect(owner);
		};

	attackCaller = [&owner, this](ActiveCard* other)
		{
			attackEffect(owner, other);
		};

	switch (trigger)
	{
	case AbilityTrigger::OnPlayed:
		owner->OnPlayed = caller;
		break;
	case AbilityTrigger::OnDeath:
		owner->OnDeath = caller;
		break;
	case AbilityTrigger::OnTurnStarts:
		owner->OnTurnStarts = caller;
		break;
	case AbilityTrigger::OnTurnEnds:
		owner->OnTurnEnds = caller;
		break;
	case AbilityTrigger::OnAttack:
		owner->OnAttack = attackCaller;
		break;
	case AbilityTrigger::OnAttacked:
		owner->OnAttacked = attackCaller;
		break;
	case AbilityTrigger::OnFlippedTo:
		owner->OnFlippedTo = caller;
		break;
	case AbilityTrigger::OnActivate:
		owner->OnActivate = caller;
		break;
	case AbilityTrigger::OnBoardUpdates:
		owner->OnBoardUpdates = caller;
		break;
	default:
		std::cout << "ERROR: Ability has no trigger (Ability::Init)";
	}
}
