#include "ActiveCard.h"

#include "Ability.h"

int activeCards = 0;
int cardDataInstances = 0;

// Active creature
ActiveCreature::ActiveCreature(CreatureData* data, ActiveCard* owner)
	: m_data(data), m_owner(owner), m_hpOverride(data->hp), m_atkOverride(data->atk),
	m_fCostOverride(data->fCost), m_aCostOverride(data->aCost)
{
	for (Ability* a : m_data->abilities)
	{
		a->Init(this);
	}
}

ActiveCreature::ActiveCreature(const ActiveCreature& other, ActiveCard* owner)
	:m_data(other.m_data), m_owner(owner), m_overrideStats(other.m_overrideStats),
	m_hpOverride(other.m_hpOverride), m_atkOverride(other.m_atkOverride),
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
		return m_hpOverride + m_hpBuff;
	}
	return m_data->hp + m_hpBuff;
}

int ActiveCreature::GetAtk()
{
	if (m_overrideStats)
	{
		return m_atkOverride + m_atkBuff;
	}
	return m_data->atk + m_atkBuff;
}

int ActiveCreature::GetFlipCost()
{
	if (m_overrideStats)
	{
		return m_fCostOverride + m_fCostBuff;
	}
	return m_data->fCost + m_fCostBuff;
}

int ActiveCreature::GetAbilityCost()
{
	if (m_overrideStats)
	{
		return m_aCostOverride;
	}
	return m_data->aCost;
}

std::string ActiveCreature::GetName()
{
	return m_data->name;
}

Family ActiveCreature::GetFamily()
{
	return m_data->family;
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
	m_aCostOverride = aCost;
}

void ActiveCreature::SetStatsToDefault()
{
	m_hpOverride = m_data->hp;
	m_atkOverride = m_data->atk;
	m_fCostOverride = m_data->fCost;
	m_aCostOverride = m_data->aCost;
	m_overrideStats = false;
}


void ActiveCreature::ResetBuffs()
{
	m_hpBuff = 0;
	m_atkBuff = 0;
	m_fCostBuff = 0;
}

void ActiveCreature::AddHPBuff(int hp)
{
	m_hpBuff += hp;
}

void ActiveCreature::AddAtkBuff(int atk)
{
	m_atkBuff += atk;
}

void ActiveCreature::AddFlipCostBuff(int fCost)
{
	m_fCostBuff += fCost;
}


bool ActiveCreature::HasActivateAbility()
{
	return OnActivate != nullptr;
}

bool ActiveCreature::FlipsAllowed()
{
	return m_data->allowFlips;
}

bool ActiveCreature::CanStack(CardData* card)
{
	StackOptions myStackOptions = m_data->stackOptions;
	if (myStackOptions.canStack && m_stackedCards.size() + 1 < myStackOptions.stackLimit) // +1 since the original card is included in the stack count
	{
		if (m_data->owner->cardID == card->cardID)
		{
			return true;
		}
	}
	return false;
}

bool ActiveCreature::Stack(CardData* card)
{
	if (CanStack(card))
	{
		m_stackedCards.push_back(card);
		if (OnStackMaxed != nullptr && m_stackedCards.size() + 1 == m_data->stackOptions.stackLimit)
		{
			OnStackMaxed(this);
		}

		if (OnStacked != nullptr)
		{
			OnStacked(this, card);
		}
		return true;
	}

	return false;
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

	id = activeCards;
	activeCards++;
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

	id = activeCards;
	activeCards++;
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

int ActiveCard::GetMaxHP()
{
	return GetCurrentFace()->GetHP();
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

std::string ActiveCard::GetName()
{
	return GetCurrentFace()->GetName();
}

Family ActiveCard::GetFamily()
{
	return GetCurrentFace()->GetFamily();
}

void ActiveCard::ResetBuffs()
{
	GetCurrentFace()->ResetBuffs();
}

void ActiveCard::AddHPBuff(int hp)
{
	GetCurrentFace()->AddHPBuff(hp);
}

void ActiveCard::AddAtkBuff(int atk)
{
	GetCurrentFace()->AddAtkBuff(atk);
}

void ActiveCard::AddFlipCostBuff(int fCost)
{
	GetCurrentFace()->AddFlipCostBuff(fCost);
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

bool ActiveCard::FlipsAllowed()
{
	return GetOpositeFace() != nullptr && GetCurrentFace()->FlipsAllowed();
}

bool ActiveCard::CanFlip()
{
	return !m_flippedThisTurn && FlipsAllowed();
}

bool ActiveCard::CanStack(CardData* card)
{
	return GetCurrentFace()->CanStack(card);
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

bool ActiveCard::Stack(CardData* card)
{
	return GetCurrentFace()->Stack(card);
}

void ActiveCard::Discard(Player* m_owner)
{
	m_owner->m_discardPile.push_back(m_data);
	// Discard all cards that had been stacked on this one
	if (m_frontFace != nullptr)
	{
		for (CardData* card : m_frontFace->m_stackedCards)
		{
			m_owner->m_discardPile.push_back(card);
		}
	}
	if (m_backFace != nullptr)
	{
		for (CardData* card : m_backFace->m_stackedCards)
		{
			m_owner->m_discardPile.push_back(card);
		}
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
