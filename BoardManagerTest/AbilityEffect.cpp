#include "AbilityEffect.h"

#include "BoardManager.h"
#include <iostream>

void AbilityEffect::Init(ActiveCreature* owner)
{
	// Does nothing by default
}


ChangeStats::ChangeStats(int health, int attack, int flipCost)
	:hp(health), atk(attack), fCost(flipCost)
{
	effect = [this](ActiveCreature* owner)
		{
			owner->SetHP(owner->GetHP() + hp);
			owner->SetAtk(owner->GetAtk() + atk);
			owner->SetFlipCost(owner->GetFlipCost() + fCost);
		};
	attackEffect = [this](ActiveCreature* owner, ActiveCard* other)
		{
			effect(owner);
		};
	stackEffect = [this](ActiveCreature* owner, CardData* other)
		{
			effect(owner);
		};
}

Flip::Flip()
{
	effect = [this](ActiveCreature* owner)
		{
			owner->GetOwner()->Flip();
		};
	attackEffect = [this](ActiveCreature* owner, ActiveCard* other)
		{
			effect(owner);
		};
	stackEffect = [this](ActiveCreature* owner, CardData* other)
		{
			effect(owner);
		};
}

Heal::Heal(int healAmount)
	:hp(healAmount)
{
	effect = [this](ActiveCreature* owner)
		{
			owner->GetOwner()->Heal(hp);
		};
	attackEffect = [this](ActiveCreature* owner, ActiveCard* other)
		{
			effect(owner);
		};
	stackEffect = [this](ActiveCreature* owner, CardData* other)
		{
			effect(owner);
		};
}

GainEnergy::GainEnergy(int energyToGain)
	:energy(energyToGain)
{
	effect = [this](ActiveCreature* owner)
		{
			ActiveCard* parentCard = owner->GetOwner();
			parentCard->GetBoard()->GetPlayer(parentCard->m_side)->m_energy += energy;
		};
	attackEffect = [this](ActiveCreature* owner, ActiveCard* other)
		{
			effect(owner);
		};
	stackEffect = [this](ActiveCreature* owner, CardData* other)
		{
			effect(owner);
		};
}

PickupCard::PickupCard(CardData* cardToPickup)
	:card(cardToPickup)
{
	effect = [this](ActiveCreature* owner)
		{
			ActiveCard* parentCard = owner->GetOwner();
			parentCard->GetBoard()->GetPlayer(parentCard->m_side)->PickupCard(card);
		};
	attackEffect = [this](ActiveCreature* owner, ActiveCard* other)
		{
			effect(owner);
		};
	stackEffect = [this](ActiveCreature* owner, CardData* other)
		{
			effect(owner);
		};
}

RandomizeStats::RandomizeStats(int minInclusive, int maxInclusive)
	:min(minInclusive), max(maxInclusive)
{
	effect = [this](ActiveCreature* owner)
		{
			owner->SetHP((rand() % (max + 1 - min)) + min);
			owner->SetAtk((rand() % (max + 1 - min)) + min);
			owner->SetFlipCost((rand() % (max + 1 - min)) + min);
		};
	attackEffect = [this](ActiveCreature* owner, ActiveCard* other)
		{
			effect(owner);
		};
	stackEffect = [this](ActiveCreature* owner, CardData* other)
		{
			effect(owner);
		};
}

CopyCards::CopyCards(std::vector<int> slotsToCopy)
	:copyTargets(slotsToCopy)
{
	effect = [this](ActiveCreature* owner)
		{
			bool canCopy = false;
			int totalHP = 0;
			int totalAtk = 0;

			ActiveCard* parentCard = owner->GetOwner();
			for (int i : copyTargets)
			{
				ActiveCard* targetCard = parentCard->GetBoard()->GetSlot(parentCard->m_slot + i, parentCard->m_side);
				if (targetCard != nullptr && targetCard->GetCurrentFace()->m_canCopy)
				{
					canCopy = true;
					totalHP += targetCard->GetMaxHP();
					totalAtk += targetCard->GetAtk();
				}
			}

			if (canCopy)
			{
				owner->SetHP(totalHP);
				owner->SetAtk(totalAtk);
			}
			else
			{
				owner->SetStatsToDefault();
			}

			// If updating the card's max hp causes its current hp to go to 0 or below, make sure card dies
			if (parentCard->GetHP() <= 0)
			{
				parentCard->GetBoard()->DestroyCard(parentCard);
			}
		};
	attackEffect = [this](ActiveCreature* owner, ActiveCard* other)
		{
			effect(owner);
		};
	stackEffect = [this](ActiveCreature* owner, CardData* other)
		{
			effect(owner);
		};
}

void CopyCards::Init(ActiveCreature* owner)
{
	owner->m_canCopy = false;
}

BuffPerFamilyCard::BuffPerFamilyCard(Family familyType, int health, int attack, int flipCost)
	:targetFamily(familyType), hp(health), atk(attack), fCost(flipCost)
{
	effect = [this](ActiveCreature* owner)
		{
			int cardCount = 0;
			ActiveCard* ownerCardRef = owner->GetOwner();
			BoardManager* boardRef = ownerCardRef->GetBoard();
			for (int i = 0; i < boardRef->GetSlotCount(); i++)
			{
				if (i == ownerCardRef->m_slot)
				{
					continue;
				}

				ActiveCard* card = boardRef->GetSlot(i, ownerCardRef->m_side);
				if (card != nullptr)
				{
					if (card->GetCurrentFace()->GetFamily() == targetFamily)
					{
						cardCount++;
					}
				}
			}

			owner->AddHPBuff(hp * cardCount);
			owner->AddAtkBuff(atk * cardCount);
			owner->AddFlipCostBuff(fCost * cardCount);
		};
	attackEffect = [this](ActiveCreature* owner, ActiveCard* other)
		{
			effect(owner);
		};
	stackEffect = [this](ActiveCreature* owner, CardData* other)
		{
			effect(owner);
		};
}

BuffEachFamilyCard::BuffEachFamilyCard(Family familyType, int health, int attack, int flipCost)
	:targetFamily(familyType), hp(health), atk(attack), fCost(flipCost)
{
	effect = [this](ActiveCreature* owner)
		{
			ActiveCard* ownerCardRef = owner->GetOwner();
			BoardManager* boardRef = ownerCardRef->GetBoard();
			for (int i = 0; i < boardRef->GetSlotCount(); i++)
			{
				if (i == ownerCardRef->m_slot)
				{
					continue;
				}

				ActiveCard* card = boardRef->GetSlot(i, ownerCardRef->m_side);
				if (card != nullptr)
				{
					if (card->GetCurrentFace()->GetFamily() == targetFamily)
					{
						card->AddHPBuff(hp);
						card->AddAtkBuff(atk);
						card->AddFlipCostBuff(fCost);
					}
				}
			}
		};
	attackEffect = [this](ActiveCreature* owner, ActiveCard* other)
		{
			effect(owner);
		};
	stackEffect = [this](ActiveCreature* owner, CardData* other)
		{
			effect(owner);
		};
}


std::string ChangeStats::GetIcon()
{
	return "IS";
}

std::string Flip::GetIcon()
{
	return "Fl";
}

std::string Heal::GetIcon()
{
	return "He";
}

std::string GainEnergy::GetIcon()
{
	return "En";
}

std::string PickupCard::GetIcon()
{
	return "Pi";
}

std::string RandomizeStats::GetIcon()
{
	return "RS";
}

std::string CopyCards::GetIcon()
{
	return "CC";
}

std::string BuffPerFamilyCard::GetIcon()
{
	return "BP";
}

std::string BuffEachFamilyCard::GetIcon()
{
	return "BE";
}
