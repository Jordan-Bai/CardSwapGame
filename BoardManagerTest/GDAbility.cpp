#include "GDAbility.h"

using namespace godot;

void GDAbility::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetAbilityTrigger"), &GDAbility::GetAbilityTrigger);
	ClassDB::bind_method(D_METHOD("SetAbilityTrigger", "abilityTrigger"), &GDAbility::SetAbilityTrigger);

	// Property hint
	ADD_PROPERTY(PropertyInfo(Variant::INT, "abilityTrigger", PROPERTY_HINT_ENUM, 
		"OnPlayed, OnDeath, OnAttack, OnAttacked, OnFlippedTo, OnActivate, OnTurnStarts, OnTurnEnds, OnCardDies, OnBoardUpdates"),
		"SetAbilityTrigger", "GetAbilityTrigger");
}

GDAbility::GDAbility()
{
}

GDAbility::~GDAbility()
{
}

int GDAbility::GetAbilityTrigger()
{
	return m_trigger;
}

void GDAbility::SetAbilityTrigger(int newTrigger)
{
	m_trigger = (AbilityTrigger)newTrigger;
}
