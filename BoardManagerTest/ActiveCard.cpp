#include "ActiveCard.h"

#include "Ability.h"

// Active creature
ActiveCreature::ActiveCreature(CreatureData* data)
	: m_data(data), m_atkOverride(data->atk), m_hpOverride(data->hp), m_fCostOverride(data->fCost)
{
	for (Ability* a : m_data->abilities)
	{
		a->Init(this);
	}
}

int ActiveCreature::GetHP()
{
	if (m_overrideStats)
	{
		return m_hpOverride;
	}
	return m_data->hp;
}

int ActiveCreature::GetAtk()
{
	if (m_overrideStats)
	{
		return m_atkOverride;
	}
	return m_data->atk;
}

int ActiveCreature::GetFlipCost()
{
	if (m_overrideStats)
	{
		return m_fCostOverride;
	}
	return m_data->fCost;
}

void ActiveCreature::SetHP(int hp)
{
	m_overrideStats = true;
	m_hpOverride = hp;
}

void ActiveCreature::SetAtk(int atk)
{
	m_overrideStats = true;
	m_atkOverride = atk;
}

void ActiveCreature::SetFlipCost(int fCost)
{
	m_overrideStats = true;
	m_fCostOverride = fCost;
}


// Active Card
ActiveCard::ActiveCard(CardData* data, int slot, int side)
	:m_data(data), m_slot(slot), m_side(side), m_damageTaken(0)
{
	if (m_data->frontCreature != nullptr)
	{
		m_frontFace = new ActiveCreature(m_data->frontCreature);
	}
	else
	{
		m_frontFace = nullptr;
	}

	if (m_data->backCreature != nullptr)
	{
		m_backFace = new ActiveCreature(m_data->backCreature);
	}
	else
	{
		m_backFace = nullptr;
	}
	// Call "OnPlayed"
}

ActiveCard::ActiveCard(const ActiveCard& other)
	:m_data(other.m_data), m_slot(other.m_slot), m_side(other.m_side), m_damageTaken(other.m_damageTaken),
	m_frontActive(other.m_frontActive), m_flippedThisTurn(other.m_flippedThisTurn)
{
	if (m_data->frontCreature != nullptr)
	{
		m_frontFace = new ActiveCreature(m_data->frontCreature);
	}
	else
	{
		m_frontFace = nullptr;
	}

	if (m_data->backCreature != nullptr)
	{
		m_backFace = new ActiveCreature(m_data->backCreature);
	}
	else
	{
		m_backFace = nullptr;
	}
}

ActiveCard::~ActiveCard()
{
	if (m_frontFace != nullptr)
	{
		delete m_frontFace;
	}
	if (m_backFace != nullptr)
	{
		delete m_backFace;
	}
}


CardData* ActiveCard::GetData()
{
	return m_data;
}

ActiveCreature* ActiveCard::GetCurrentFace()
{
	if (m_frontActive)
	{
		return m_frontFace;
	}
	else
	{
		return m_backFace;
	}
}

ActiveCreature* ActiveCard::GetOpositeFace()
{
	if (m_frontActive)
	{
		return m_backFace;
	}
	else
	{
		return m_frontFace;
	}
}

int ActiveCard::GetCost()
{
	return m_data->cost;
}

int ActiveCard::GetHP()
{
	return GetCurrentFace()->GetHP() - m_damageTaken;
}

int ActiveCard::GetAtk()
{
	return GetCurrentFace()->GetAtk();
}

int ActiveCard::GetFlipCost()
{
	return GetCurrentFace()->GetFlipCost();
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
	return !m_flippedThisTurn && GetOpositeFace() != nullptr;
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
