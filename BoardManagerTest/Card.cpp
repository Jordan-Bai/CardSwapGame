#include "Card.h"

ActiveCard::ActiveCard(CardData* data, int slot, int side)
	:m_data(data), m_slot(slot), m_side(side), m_damageTaken(0)
{

}

ActiveCard::ActiveCard(ActiveCard* other)
	:m_data(other->m_data), m_slot(other->m_slot), m_side(other->m_side), m_damageTaken(other->m_damageTaken), 
	m_frontActive(other->m_frontActive), m_flippedThisTurn(other->m_flippedThisTurn)
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


int ActiveCard::GetDamageTaken()
{
	return m_damageTaken;
}

bool ActiveCard::GetFrontActive()
{
	return m_frontActive;
}

bool ActiveCard::CanFlip()
{
	return !m_flippedThisTurn;
}

std::vector<int> ActiveCard::GetTargets() // By default, target is opposite slot
{
	return std::vector<int>(1, m_slot);
}

void ActiveCard::TakeDamage(int damage)
{
	m_damageTaken += damage;
}

bool ActiveCard::Flip()
{
	if (CanFlip())
	{
		m_frontActive = !m_frontActive;
		m_flippedThisTurn = true;
		return true;
	}

	return false;
}

void ActiveCard::OnStartTurn()
{
	m_flippedThisTurn = false;
}
