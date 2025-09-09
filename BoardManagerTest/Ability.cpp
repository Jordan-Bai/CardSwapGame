#include "Ability.h"

//Ability::Ability(AbilityTrigger _trigger, std::function<void(ActiveCreature* owner)> _effect)
//	: trigger(_trigger), effect(_effect)
//{	
//}

//Ability::Ability(AbilityTrigger _trigger, std::function<void(ActiveCreature* owner, ActiveCard* other)> _attackEffect)
//	: trigger(_trigger), attackEffect(_attackEffect)
//{
//}

Ability::Ability(AbilityTrigger _trigger, AbilityEffect* _effect)
	: trigger(_trigger), effect(_effect)
{
}

void Ability::Init(ActiveCreature* owner)
{
	//std::function<void()> caller = [&owner, this]()
	//	{
	//		//effect(owner);
	//		effect->effect(owner);
	//	};
	//
	//std::function<void(ActiveCard* other)> attackCaller = [&owner, this](ActiveCard* other)
	//	{
	//		//attackEffect(owner, other);
	//		effect->attackEffect(owner, other);
	//	};

	switch (trigger)
	{
	case AbilityTrigger::OnPlayed:
		owner->OnPlayed = effect->effect;
		break;
	case AbilityTrigger::OnDeath:
		owner->OnDeath = effect->effect;
		break;
	case AbilityTrigger::OnAttack:
		owner->OnAttack = effect->attackEffect;
		break;
	case AbilityTrigger::OnAttacked:
		owner->OnAttacked = effect->attackEffect;
		break;
	case AbilityTrigger::OnFlippedTo:
		owner->OnFlippedTo = effect->effect;
		break;
	case AbilityTrigger::OnActivate:
		owner->OnActivate = effect->effect;
		break;
	case AbilityTrigger::OnTurnStarts:
		owner->OnTurnStarts = effect->effect;
		break;
	case AbilityTrigger::OnTurnEnds:
		owner->OnTurnEnds = effect->effect;
		break;
	case AbilityTrigger::OnCardDies:
		owner->OnCardDies = effect->effect;
		break;
	case AbilityTrigger::OnBoardUpdates:
		owner->OnBoardUpdates = effect->effect;
		break;
	default:
		std::cout << "ERROR: Ability has no trigger (Ability::Init)\n";
	}
}
