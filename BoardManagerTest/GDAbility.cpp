#include "GDAbility.h"

using namespace godot;

void GDAbility::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetTrigger"), &GDAbility::GetTrigger);
	ClassDB::bind_method(D_METHOD("GetEffect"), &GDAbility::GetEffect);

	ClassDB::bind_method(D_METHOD("SetTrigger", "abilityTrigger"), &GDAbility::SetTrigger);
	ClassDB::bind_method(D_METHOD("SetEffect", "abilityTrigger"), &GDAbility::SetEffect);

	// Property hint tells godot that this is an enum and what the name of each value is
	ADD_PROPERTY(PropertyInfo(Variant::INT, "abilityTrigger", PROPERTY_HINT_ENUM, 
		"OnPlayed, OnDeath, OnAttack, OnAttacked, OnFlippedTo, OnActivate, OnTurnStarts, OnTurnEnds, OnCardDies, OnBoardUpdates"),
		"SetTrigger", "GetTrigger");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect"), "SetEffect", "GetEffect");
}

GDAbility::GDAbility()
	:m_effect(nullptr), m_data(nullptr)
{
	//if (m_effectRef.ptr() != nullptr)
	//{
	//	m_effect = m_effectRef.ptr()->CreateEffect();
	//	m_data = new Ability(m_trigger, m_effect);
	//}
	//else
	//{
	//	UtilityFunctions::print("ABILITY NOT INITIALIZED");
	//}
}

GDAbility::~GDAbility()
{
	if (m_effect != nullptr)
	{
		delete m_effect;
	}
	if (m_data != nullptr)
	{
		delete m_data;
	}
}

int GDAbility::GetTrigger()
{
	return m_trigger;
}

Ref<GDAbilityEffect> GDAbility::GetEffect()
{
	return m_effectRef;
}

void GDAbility::SetTrigger(int newTrigger)
{
	m_trigger = (AbilityTrigger)newTrigger;
}

void GDAbility::SetEffect(Ref<GDAbilityEffect> effect)
{
	m_effectRef = effect;
}

Ability* GDAbility::GetData()
{
	if (m_data == nullptr)
	{
		if (m_effectRef.ptr() != nullptr)
		{
			m_effect = m_effectRef.ptr()->CreateEffect();
			m_data = new Ability(m_trigger, m_effect);
		}
		else
		{
			UtilityFunctions::print("ABILITY NOT INITIALIZED");
		}
	}
	return m_data;
}
