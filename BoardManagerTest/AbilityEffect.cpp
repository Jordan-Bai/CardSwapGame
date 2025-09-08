#include "AbilityEffect.h"

#include "BoardManager.h"

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
			owner->SetHP(owner->GetHP() + hp);
			owner->SetAtk(owner->GetAtk() + atk);
			owner->SetFlipCost(owner->GetFlipCost() + fCost);
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
			owner->GetOwner()->Flip();
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
			owner->GetOwner()->Heal(hp);
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
			ActiveCard* parentCard = owner->GetOwner();
			parentCard->GetBoard()->GetPlayer(parentCard->m_side)->m_energy += energy;
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
			ActiveCard* parentCard = owner->GetOwner();
			parentCard->GetBoard()->GetPlayer(parentCard->m_side)->PickupCard(card);
		};
}

RandomizeStats::RandomizeStats(int statMin, int statMax)
	:min(statMin), max(statMax)
{
	effect = [this](ActiveCreature* owner)
		{
			owner->SetHP((rand() % (max - min)) + min);
			owner->SetAtk((rand() % (max - min)) + min);
			owner->SetFlipCost((rand() % (max - min)) + min);
		};
	attackEffect = [this](ActiveCreature* owner, ActiveCard* other)
		{
			owner->SetHP((rand() % (max - min)) + min);
			owner->SetAtk((rand() % (max - min)) + min);
			owner->SetFlipCost((rand() % (max - min)) + min);
		};
}

CopyCards::CopyCards(std::vector<int> slotsToCopy)
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
				if (targetCard != nullptr)
				{
					canCopy = true;
					totalHP += targetCard->GetCurrentFace()->GetHP();
					totalAtk += targetCard->GetCurrentFace()->GetAtk();
				}
			}

			if (canCopy)
			{
				owner->SetHP(totalHP);
				owner->SetAtk(totalAtk);
			}
		};
	attackEffect = [this](ActiveCreature* owner, ActiveCard* other)
		{
			bool canCopy = false;
			int totalHP = 0;
			int totalAtk = 0;

			ActiveCard* parentCard = owner->GetOwner();
			for (int i : copyTargets)
			{
				ActiveCard* targetCard = parentCard->GetBoard()->GetSlot(parentCard->m_slot + i, parentCard->m_side);
				if (targetCard != nullptr)
				{
					canCopy = true;
					totalHP += targetCard->GetCurrentFace()->GetHP();
					totalAtk += targetCard->GetCurrentFace()->GetAtk();
				}
			}

			if (canCopy)
			{
				owner->SetHP(totalHP);
				owner->SetAtk(totalAtk);
			}
		};
}

std::string ChangeStats::GetIcon()
{
	return "S";
}

std::string Flip::GetIcon()
{
	return "F";
}

std::string Heal::GetIcon()
{
	return "H";
}

std::string GainEnergy::GetIcon()
{
	return "E";
}

std::string PickupCard::GetIcon()
{
	return "P";
}

std::string RandomizeStats::GetIcon()
{
	return "R";
}

std::string CopyCards::GetIcon()
{
	return "C";
}
