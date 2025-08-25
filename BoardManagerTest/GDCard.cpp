#include "GDCard.h"

void godot::GDCard::_bind_methods()
{
}

godot::GDCard::GDCard()
{
	CreatureData* front = new CreatureData(1, 1, 1);
	CreatureData* back = new CreatureData(2, 2, 2);

	m_data = new CardData(1, front, back);
}

godot::GDCard::~GDCard()
{
	if (m_data->frontCreature != nullptr)
	{
		delete m_data->frontCreature;
	}
	if (m_data->backCreature != nullptr)
	{
		delete m_data->backCreature;
	}

	delete m_data;
}
