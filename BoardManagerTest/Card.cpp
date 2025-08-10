#include "Card.h"

ActiveCard::ActiveCard(CardData* data, int slot, int side)
	:m_data(data), m_slot(slot), m_side(side), m_damageTaken(0)
{

}

CardData* ActiveCard::GetData()
{
	return m_data;
}

CreatureData* ActiveCard::GetCurrentFace()
{
	if (m_frontActive)
	{
		return m_data->frontCreature;
	}
	else
	{
		return m_data->backCreature;
	}
}

int ActiveCard::GetCost()
{
	return m_data->cost;
}

int ActiveCard::GetHP()
{
	return GetCurrentFace()->hp - m_damageTaken;
}

int ActiveCard::GetAtk()
{
	return GetCurrentFace()->atk;
}

int ActiveCard::GetFlipCost()
{
	return GetCurrentFace()->fCost;
}

std::vector<int> ActiveCard::GetTargets() // By default, target is opposite slot
{
	return std::vector<int>(1, m_slot);
}

void ActiveCard::TakeDamage(int damage)
{
	m_damageTaken += damage;
}
