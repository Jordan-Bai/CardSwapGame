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
	switch (trigger)
	{
	case AbilityTrigger::OnPlayed:
		description = "On Played";
		break;
	case AbilityTrigger::OnStack:
		description = "On Stack";
		break;
	case AbilityTrigger::OnStackMaxed:
		description = "On Stack Maxed";
		break;
	case AbilityTrigger::OnDeath:
		description = "On Death";
		break;
	case AbilityTrigger::OnAttack:
		description = "On Attack";
		break;
	case AbilityTrigger::OnAttacked:
		description = "On Attacked";
		break;
	case AbilityTrigger::OnFlippedTo:
		description = "On Flipped To";
		break;
	case AbilityTrigger::OnActivate:
		description = "On Activate";
		break;
	case AbilityTrigger::OnTurnStarts:
		description = "On Turn Starts";
		break;
	case AbilityTrigger::OnTurnEnds:
		description = "On Turn Ends";
		break;
	case AbilityTrigger::OnCardDies:
		description = "On Card Dies";
		break;
	case AbilityTrigger::OnBoardUpdates:
		description = "On Board Updates";
		break;
	default:
		std::cout << "ERROR: Ability has no trigger (Ability::Init)\n";
	}

	description += ": " + effect->GetDescription();
}

void Ability::Init(ActiveCreature* owner)
{
	effect->Init(owner);

	switch (trigger)
	{
	case AbilityTrigger::OnPlayed:
		owner->OnPlayed = effect->effect;
		break;
	case AbilityTrigger::OnStack:
		owner->OnStacked = effect->stackEffect;
		break;
	case AbilityTrigger::OnStackMaxed:
		owner->OnStackMaxed = effect->effect;
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
