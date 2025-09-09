#include "GDAbilityEffect.h"

using namespace godot;

void GDAbilityEffect::_bind_methods()
{
}

GDAbilityEffect::GDAbilityEffect()
{
}

AbilityEffect* GDAbilityEffect::CreateEffect()
{
	return nullptr;
}


void GDChangeStats::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetHP"), &GDChangeStats::GetHP);
	ClassDB::bind_method(D_METHOD("GetAtk"), &GDChangeStats::GetAtk);
	ClassDB::bind_method(D_METHOD("GetFlipCost"), &GDChangeStats::GetFlipCost);

	ClassDB::bind_method(D_METHOD("SetHP", "hp"), &GDChangeStats::SetHP);
	ClassDB::bind_method(D_METHOD("SetAtk", "atk"), &GDChangeStats::SetAtk);
	ClassDB::bind_method(D_METHOD("SetFlipCost", "cost"), &GDChangeStats::SetFlipCost);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "hp"), "SetHP", "GetHP");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atk"), "SetAtk", "GetAtk");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "flipCost"), "SetFlipCost", "GetFlipCost");
}

AbilityEffect* GDChangeStats::CreateEffect()
{
	return new ChangeStats(m_hp, m_atk, m_fCost);
}

int GDChangeStats::GetHP()
{
	return m_hp;
}

int GDChangeStats::GetAtk()
{
	return m_atk;
}

int GDChangeStats::GetFlipCost()
{
	return m_fCost;
}

void GDChangeStats::SetHP(int hp)
{
	m_hp = hp;
}

void GDChangeStats::SetAtk(int atk)
{
	m_atk = atk;
}

void GDChangeStats::SetFlipCost(int fCost)
{
	m_fCost = fCost;
}


void GDFlip::_bind_methods()
{
}

AbilityEffect* GDFlip::CreateEffect()
{
	return new Flip();
}


void GDHeal::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetHP"), &GDHeal::GetHP);
	ClassDB::bind_method(D_METHOD("SetHP", "hp"), &GDHeal::SetHP);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "hp"), "SetHP", "GetHP");
}

AbilityEffect* GDHeal::CreateEffect()
{
	return new Heal(m_hp);
}

int GDHeal::GetHP()
{
	return m_hp;
}

void GDHeal::SetHP(int hp)
{
	m_hp = hp;
}


void GDGainEnergy::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetEnergy"), &GDGainEnergy::GetEnergy);
	ClassDB::bind_method(D_METHOD("SetEnergy", "energy"), &GDGainEnergy::SetEnergy);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "energy"), "SetEnergy", "GetEnergy");
}

AbilityEffect* GDGainEnergy::CreateEffect()
{
	return new GainEnergy(m_energy);
}

int GDGainEnergy::GetEnergy()
{
	return m_energy;
}

void GDGainEnergy::SetEnergy(int energy)
{
	m_energy = energy;
}


void GDRandomizeStats::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetMin"), &GDRandomizeStats::GetMin);
	ClassDB::bind_method(D_METHOD("GetMax"), &GDRandomizeStats::GetMax);

	ClassDB::bind_method(D_METHOD("SetMin", "min"), &GDRandomizeStats::SetMin);
	ClassDB::bind_method(D_METHOD("SetMax", "max"), &GDRandomizeStats::SetMax);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "min"), "SetMin", "GetMin");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max"), "SetMax", "GetMax");
}

AbilityEffect* GDRandomizeStats::CreateEffect()
{
	return new RandomizeStats(m_min, m_max);
}

int GDRandomizeStats::GetMin()
{
	return m_min;
}

int GDRandomizeStats::GetMax()
{
	return m_max;
}

void GDRandomizeStats::SetMin(int min)
{
	m_min = min;
}

void GDRandomizeStats::SetMax(int max)
{
	m_max = max;
}
