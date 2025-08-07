#include "Card.h"

ActiveCard::ActiveCard(CardData* data, int slot, int side)
	:m_data(data), m_slot(slot), m_side(side), m_damageTaken(0)
{

}

CardData* ActiveCard::GetData()
{
	return m_data;
}

int ActiveCard::GetCost()
{
	return m_data->cost;
}

int ActiveCard::GetHP()
{
	return m_data->frontCreature->hp - m_damageTaken;
}

int ActiveCard::GetAtk()
{
	return m_data->frontCreature->atk;
}

std::vector<int> ActiveCard::GetTargets() // By default, target is opposite slot
{
	return std::vector<int>(m_slot);
}

void ActiveCard::TakeDamage(int damage)
{
	m_damageTaken += damage;
}
