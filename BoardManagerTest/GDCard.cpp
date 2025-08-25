#include "GDCard.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDCreature::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetHP"), &GDCreature::GetHP);
	ClassDB::bind_method(D_METHOD("GetAtk"), &GDCreature::GetAtk);
	ClassDB::bind_method(D_METHOD("GetFlipCost"), &GDCreature::GetFlipCost);
}

GDCreature::GDCreature()
	: m_data(nullptr)
{
}

GDCreature::GDCreature(CreatureData* data)
	: m_data(data)
{
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



void GDCard::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetCost"), &GDCard::GetCost);
	ClassDB::bind_method(D_METHOD("GetFrontFace"), &GDCard::GetFrontFace);
	ClassDB::bind_method(D_METHOD("GetBackFace"), &GDCard::GetBackFace);
}

GDCard::GDCard()
	: m_data(nullptr), m_frontFace(nullptr), m_backFace(nullptr)
{
	//CreatureData* front = new CreatureData(1, 1, 1);
	//CreatureData* back = new CreatureData(2, 2, 2);
	//
	//m_data = new CardData(1, front, back);
}

GDCard::GDCard(CardData* data)
	: m_data(data)
{
	m_frontFace = memnew(GDCreature(m_data->frontCreature));
	m_backFace = memnew(GDCreature(m_data->backCreature));
}

GDCard::~GDCard()
{
	if (m_frontFace != nullptr)
	{
		memdelete(m_frontFace);
	}
	if (m_backFace != nullptr)
	{
		memdelete(m_backFace);
	}
}

int GDCard::GetCost()
{
	return m_data->cost;
}

GDCreature* GDCard::GetFrontFace()
{
	//Ref<godot::GDCreature> frontFace;
	//return GDCreature(m_data->frontCreature);
	return m_frontFace;
}

GDCreature* GDCard::GetBackFace()
{
	//return GDCreature(m_data->backCreature);
	return m_backFace;
}
