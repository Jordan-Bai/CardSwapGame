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
