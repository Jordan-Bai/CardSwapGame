#include "GDCard.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// CREATURE DATA
void GDCreature::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetHP"), &GDCreature::GetHP);
	ClassDB::bind_method(D_METHOD("GetAtk"), &GDCreature::GetAtk);
	ClassDB::bind_method(D_METHOD("GetFlipCost"), &GDCreature::GetFlipCost);
	ClassDB::bind_method(D_METHOD("GetAbilityCost"), &GDCreature::GetAbilityCost);
	ClassDB::bind_method(D_METHOD("GetName"), &GDCreature::GetName);

	ClassDB::bind_method(D_METHOD("SetHP", "hp"), &GDCreature::SetHP);
	ClassDB::bind_method(D_METHOD("SetAtk", "atk"), &GDCreature::SetAtk);
	ClassDB::bind_method(D_METHOD("SetFlipCost", "flipCost"), &GDCreature::SetFlipCost);
	ClassDB::bind_method(D_METHOD("SetAbilityCost", "abilityCost"), &GDCreature::SetAbilityCost);
	ClassDB::bind_method(D_METHOD("SetName", "name"), &GDCreature::SetName);
	
	ADD_PROPERTY(PropertyInfo(Variant::INT, "hp"), "SetHP", "GetHP");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atk"), "SetAtk", "GetAtk");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "flipCost"), "SetFlipCost", "GetFlipCost");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "abilityCost"), "SetAbilityCost", "GetAbilityCost");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "SetName", "GetName");
}

GDCreature::GDCreature()
{
	m_data = new CreatureData(-1, -1, -1);
}

//GDCreature::GDCreature(CreatureData* data)
//	: m_data(data), m_hp(data->hp), m_atk(data->atk), m_flipCost(data->fCost)
//{
//}

GDCreature::~GDCreature()
{
	delete m_data;
}

int GDCreature::GetHP()
{
	return m_data->hp;
}

int GDCreature::GetAtk()
{
	return m_data->atk;
}

int GDCreature::GetFlipCost()
{
	return m_data->fCost;
}

int GDCreature::GetAbilityCost()
{
	return m_data->aCost;
}

String GDCreature::GetName()
{
	return String(m_data->name.c_str());
}

void GDCreature::SetHP(int hp)
{
	m_data->hp = hp;
}

void GDCreature::SetAtk(int atk)
{
	m_data->atk = atk;
}

void GDCreature::SetFlipCost(int fCost)
{
	m_data->fCost = fCost;
}

void GDCreature::SetAbilityCost(int aCost)
{
	m_data->aCost = aCost;
}

void GDCreature::SetName(String name)
{
	m_data->name = name.utf8();
}

CreatureData* GDCreature::GetData()
{
	return m_data;
}


// CARD DATA
void GDCard::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetCost"), &GDCard::GetCost);
	ClassDB::bind_method(D_METHOD("GetFrontFace"), &GDCard::GetFrontFace);
	ClassDB::bind_method(D_METHOD("GetBackFace"), &GDCard::GetBackFace);

	ClassDB::bind_method(D_METHOD("SetCost", "cost"), &GDCard::SetCost);
	ClassDB::bind_method(D_METHOD("SetFrontFace", "creature"), &GDCard::SetFrontFace);
	ClassDB::bind_method(D_METHOD("SetBackFace", "creature"), &GDCard::SetBackFace);
	
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cost"), "SetCost", "GetCost");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "frontCreature"), "SetFrontFace", "GetFrontFace");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "backCreature"), "SetBackFace", "GetBackFace");
}

GDCard::GDCard()
	: m_frontFace(nullptr), m_backFace(nullptr)
{
	//CreatureData* front = new CreatureData(1, 1, 1);
	//CreatureData* back = new CreatureData(2, 2, 2);

	//m_frontFace = memnew(GDCreature());
	//m_backFace = memnew(GDCreature());
	
	m_data = new CardData(0, nullptr, nullptr);
}

//GDCard::GDCard(CardData* data)
//	: m_data(data), m_frontFace(nullptr), m_backFace(nullptr), m_cost(0)
//{
//	//m_frontFace = memnew(GDCreature(m_data->frontCreature));
//	//m_backFace = memnew(GDCreature(m_data->backCreature));
//}

GDCard::~GDCard()
{
	//if (m_frontFace != nullptr)
	//{
	//	memdelete(m_frontFace);
	//}
	//if (m_backFace != nullptr)
	//{
	//	memdelete(m_backFace);
	//}
	delete m_data;
}

int GDCard::GetCost()
{
	return m_data->cost;
}

void GDCard::SetCost(int cost)
{
	m_data->cost = cost;
}

GDCreature* GDCard::GetFrontFace()
{
	return m_frontFace;
}

GDCreature* GDCard::GetBackFace()
{
	return m_backFace;
}

void GDCard::SetFrontFace(GDCreature* creature)
{
	m_frontFace = creature;
	m_data->frontCreature = creature->GetData();
}

void GDCard::SetBackFace(GDCreature* creature)
{
	m_backFace = creature;
	m_data->backCreature = creature->GetData();
}

CardData* GDCard::GetData()
{
	return m_data;
}


// DISPLAY CARD
void GDDisplayCard::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetHP"), &GDDisplayCard::GetHP);
	ClassDB::bind_method(D_METHOD("GetAtk"), &GDDisplayCard::GetAtk);
	ClassDB::bind_method(D_METHOD("GetCost"), &GDDisplayCard::GetCost);
	ClassDB::bind_method(D_METHOD("GetFlipCost"), &GDDisplayCard::GetFlipCost);
	ClassDB::bind_method(D_METHOD("GetAbilityCost"), &GDDisplayCard::GetAbilityCost);
	ClassDB::bind_method(D_METHOD("GetName"), &GDDisplayCard::GetName);

	ClassDB::bind_method(D_METHOD("SetHP", "hp"), &GDDisplayCard::SetHP);
	ClassDB::bind_method(D_METHOD("SetAtk", "atk"), &GDDisplayCard::SetAtk);
	ClassDB::bind_method(D_METHOD("SetCost", "cost"), &GDDisplayCard::SetCost);
	ClassDB::bind_method(D_METHOD("SetFlipCost", "flipCost"), &GDDisplayCard::SetFlipCost);
	ClassDB::bind_method(D_METHOD("SetAbilityCost", "abilityCost"), &GDDisplayCard::SetAbilityCost);
	ClassDB::bind_method(D_METHOD("SetName", "name"), &GDDisplayCard::SetName);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "hp"), "SetHP", "GetHP");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atk"), "SetAtk", "GetAtk");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cost"), "SetCost", "GetCost");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "flipCost"), "SetFlipCost", "GetFlipCost");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "abilityCost"), "SetAbilityCost", "GetAbilityCost");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "SetName", "GetName");
}

GDDisplayCard::GDDisplayCard()
	:m_hp(-1), m_atk(-1),  m_cost(-1), m_flipCost(-1), m_abilityCost(-1)
{
}

GDDisplayCard::GDDisplayCard(GDCard* card)
{
	m_hp = card->GetFrontFace()->GetHP();
	m_atk = card->GetFrontFace()->GetAtk();
	m_cost = card->GetCost();
	m_flipCost = card->GetFrontFace()->GetFlipCost();
	m_abilityCost = card->GetFrontFace()->GetAbilityCost();
	m_name = card->GetFrontFace()->GetName();
}

GDDisplayCard::GDDisplayCard(CardData* card)
{
	m_hp = card->frontCreature->hp;
	m_atk = card->frontCreature->atk;
	m_cost = card->cost;
	m_flipCost = card->frontCreature->fCost;
	m_abilityCost = card->frontCreature->aCost;
	m_name = String(card->frontCreature->name.c_str());
}

int GDDisplayCard::GetHP()
{
	return m_hp;
}

int GDDisplayCard::GetAtk()
{
	return m_atk;
}

int GDDisplayCard::GetCost()
{
	return m_cost;
}

int GDDisplayCard::GetFlipCost()
{
	return m_flipCost;
}

int godot::GDDisplayCard::GetAbilityCost()
{
	return m_abilityCost;
}

String GDDisplayCard::GetName()
{
	return m_name;
}


void GDDisplayCard::SetHP(int hp)
{
	m_hp = hp;
}

void GDDisplayCard::SetAtk(int atk)
{
	m_atk = atk;
}

void GDDisplayCard::SetCost(int cost)
{
	m_cost = cost;
}

void GDDisplayCard::SetFlipCost(int flipCost)
{
	m_flipCost = flipCost;
}

void godot::GDDisplayCard::SetAbilityCost(int abilityCost)
{
	m_abilityCost = abilityCost;
}

void GDDisplayCard::SetName(String name)
{
	m_name = name;
}


void GDDisplayCard::SetStats(GDCreature* creature)
{
	// Can't set cost cuz that's tied to the card itself: might want to change that at some point
	m_flipCost = creature->GetFlipCost();
	m_hp = creature->GetHP();
	m_atk = creature->GetAtk();
	m_name = creature->GetName();
}
