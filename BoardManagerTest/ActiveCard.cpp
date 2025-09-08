#include "ActiveCard.h"

#include "Ability.h"

// Active creature
ActiveCreature::ActiveCreature(CreatureData* data, ActiveCard* owner)
	: m_data(data), m_owner(owner), m_atkOverride(data->atk), m_hpOverride(data->hp), 
	m_fCostOverride(data->fCost), m_aCostOverride(data->aCost)
{
	for (Ability* a : m_data->abilities)
	{
		a->Init(this);
	}
}

ActiveCreature::ActiveCreature(const ActiveCreature& other, ActiveCard* owner)
	:m_data(other.m_data), m_owner(owner), m_overrideStats(other.m_overrideStats),
	m_atkOverride(other.m_atkOverride), m_hpOverride(other.m_hpOverride),
	m_fCostOverride(other.m_fCostOverride), m_aCostOverride(other.m_aCostOverride)
{
	for (Ability* a : m_data->abilities)
	{
		a->Init(this);
	}
}

CreatureData* ActiveCreature::GetData()
{
	return m_data;
}

ActiveCard* ActiveCreature::GetOwner()
{
	return m_owner;
}

void ActiveCreature::SetOwner(ActiveCard* owner)
{
	m_owner = owner;
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

int ActiveCreature::GetAbilityCost()
{
	if (m_overrideStats)
	{
		return m_aCostOverride;
	}
	return m_data->aCost;
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

void ActiveCreature::SetAbilityCost(int aCost)
{
	m_overrideStats = true;
	m_fCostOverride = aCost;
}

bool ActiveCreature::HasActivateAbility()
{
	return OnActivate != nullptr;
}


// Active Card
ActiveCard::ActiveCard(CardData* data, int slot, int side, BoardManager* boardRef)
	:m_data(data), m_slot(slot), m_side(side), m_damageTaken(0), m_boardRef(boardRef)
{
	if (m_data->frontCreature != nullptr)
	{
		m_frontFace = new ActiveCreature(m_data->frontCreature, this);
	}
	else
	{
		m_frontFace = nullptr;
	}

	if (m_data->backCreature != nullptr)
	{
		m_backFace = new ActiveCreature(m_data->backCreature, this);
	}
	else
	{
		m_backFace = nullptr;
	}
	// Call "OnPlayed"
}

ActiveCard::ActiveCard(const ActiveCard& other)
	:m_data(other.m_data), m_slot(other.m_slot), m_side(other.m_side), m_damageTaken(other.m_damageTaken),
	m_frontActive(other.m_frontActive), m_flippedThisTurn(other.m_flippedThisTurn), m_boardRef(nullptr)
{
	if (m_data->frontCreature != nullptr)
	{
		//m_frontFace = new ActiveCreature(m_data->frontCreature, this);
		m_frontFace = new ActiveCreature(*other.m_frontFace, this);
	}
	else
	{
		m_frontFace = nullptr;
	}

	if (m_data->backCreature != nullptr)
	{
		//m_backFace = new ActiveCreature(m_data->backCreature, this);
		m_backFace = new ActiveCreature(*other.m_backFace, this);
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

int ActiveCard::GetAbilityCost()
{
	return GetCurrentFace()->GetAbilityCost();
}

BoardManager* ActiveCard::GetBoard()
{
	return m_boardRef;
}

void ActiveCard::SetBoard(BoardManager* board)
{
	m_boardRef = board;
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

void ActiveCard::Heal(int healAmount)
{
	m_damageTaken -= healAmount;
	if (m_damageTaken < 0)
	{
		m_damageTaken = 0;
	}
}

void ActiveCard::Flip()
{
	//if (CanFlip())
	//{
	//	m_frontActive = !m_frontActive;
	//	m_flippedThisTurn = true;
	//	return true;
	//}
	//
	//return false;
	if (GetOpositeFace() != nullptr)
	{
		m_frontActive = !m_frontActive;
		m_flippedThisTurn = true;
	}
	else
	{
		std::cout << "ERROR: Card has no backface (ActiveCard::Flip)\n";
	}
}

//bool ActiveCard::ActivateEffect()
//{
//	if (GetCurrentFace()->HasActivateAbility())
//	{
//		GetCurrentFace()->OnActivate();
//		return true;
//	}
//	return false;
//}

//void ActiveCard::OnStartTurn()
//{
//	m_flippedThisTurn = false;
//}

// Ability Triggers
void ActiveCard::OnPlayed()
{
	ActiveCreature* currentFace = GetCurrentFace();
	if (currentFace->OnPlayed != nullptr)
	{
		currentFace->OnPlayed(currentFace);
	}
}

void ActiveCard::OnDeath()
{
	ActiveCreature* currentFace = GetCurrentFace();
	if (currentFace->OnDeath != nullptr)
	{
		currentFace->OnDeath(currentFace);
	}
}

void ActiveCard::OnAttack(ActiveCard* target)
{
	ActiveCreature* currentFace = GetCurrentFace();
	if (currentFace->OnAttack != nullptr)
	{
		currentFace->OnAttack(currentFace, target);
	}
}

void ActiveCard::OnAttacked(ActiveCard* target)
{
	ActiveCreature* currentFace = GetCurrentFace();
	if (currentFace->OnAttacked != nullptr)
	{
		currentFace->OnAttacked(currentFace, target);
	}
}

void ActiveCard::OnFlip()
{
	ActiveCreature* currentFace = GetCurrentFace();
	if (currentFace->OnFlippedTo != nullptr)
	{
		currentFace->OnFlippedTo(currentFace);
	}
}

void ActiveCard::OnActivate()
{
	ActiveCreature* currentFace = GetCurrentFace();
	if (currentFace->HasActivateAbility())
	{
		currentFace->OnActivate(currentFace);
	}
}

void ActiveCard::OnTurnStarts()
{
	m_flippedThisTurn = false;

	ActiveCreature* currentFace = GetCurrentFace();
	if (currentFace->OnTurnStarts != nullptr)
	{
		currentFace->OnTurnStarts(currentFace);
	}
}

void ActiveCard::OnTurnEnds()
{
	ActiveCreature* currentFace = GetCurrentFace();
	if (currentFace->OnTurnEnds != nullptr)
	{
		currentFace->OnTurnEnds(currentFace);
	}
}

void ActiveCard::OnCardDies()
{
	ActiveCreature* currentFace = GetCurrentFace();
	if (currentFace->OnCardDies != nullptr)
	{
		currentFace->OnCardDies(currentFace);
	}
}

void ActiveCard::OnBoardUpdates()
{
	ActiveCreature* currentFace = GetCurrentFace();
	if (currentFace->OnBoardUpdates != nullptr)
	{
		currentFace->OnBoardUpdates(currentFace);
	}
}
