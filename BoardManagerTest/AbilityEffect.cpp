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


std::string ChangeStats::GetDescription()
{
	std::vector<int> stats;
	std::vector<std::string> statNames;
	if (hp != 0)
	{
		stats.push_back(hp);
		statNames.push_back("hp");
	}
	if (atk != 0)
	{
		stats.push_back(atk);
		statNames.push_back("attack");
	}
	if (fCost != 0)
	{
		stats.push_back(fCost);
		statNames.push_back("flip cost");
	}

	std::string description = "";
	for (int i = 0; i < stats.size(); i++)
	{
		if (stats[i] > 0)
		{
			description += "increase ";
		}
		if (stats[i] < 0)
		{
			description += "decrease ";
		}

		description += statNames[i] + " by " + std::to_string(stats[i]);

		// If this stat is the 2nd last, add an &
		if (i == stats.size() - 2)
		{
			description += " & ";
		}
		// If this stat has 2 or more stats after it, add a comma (have to convert size to an int so it can be negative)
		else if (i < (int)stats.size() - 2)
		{
			description += ", ";
		}
	}

	// Make sure 1st letter is capitalized
	description[0] = toupper(description[0]);

	return description;
}

std::string ChangeStats::GetIcon()
{
	return "IS";
}


std::string Flip::GetDescription()
{
	return "Flip";
}

std::string Flip::GetIcon()
{
	return "Fl";
}


std::string Heal::GetDescription()
{
	return "Heal " + std::to_string(hp) + " hp";
}

std::string Heal::GetIcon()
{
	return "He";
}


std::string GainEnergy::GetDescription()
{
	return "Gain " + std::to_string(energy) + " energy";
}

std::string GainEnergy::GetIcon()
{
	return "En";
}


std::string PickupCard::GetDescription()
{
	return "Pickup " + card->frontCreature->name;
}

std::string PickupCard::GetIcon()
{
	return "Pi";
}


std::string RandomizeStats::GetDescription()
{
	return "Randomize stats from " + std::to_string(min) + " to " + std::to_string(max);
}

std::string RandomizeStats::GetIcon()
{
	return "RS";
}


std::string CopyCards::GetDescription()
{
	std::string description = "Copy ";
	if (copyTargets.size() > 1)
	{
		description += "& combine stats of the cards ";
	}
	else
	{
		description += "stats of the card ";
	}

	for (int i = 0; i < copyTargets.size(); i++)
	{
		if (abs(copyTargets[i]) > 1)
		{
			description += std::to_string(abs(copyTargets[i])) + " slots ";
		}

		if (copyTargets[i] > 0)
		{
			description += "to the right";
		}
		else if (copyTargets[i] < 0)
		{
			description += "to the left";
		}
		else
		{
			description += "[ERROR: Can't copy self]";
		}

		// If this target is the 2nd last, add an &
		if (i == copyTargets.size() - 2)
		{
			description += " & ";
		}
		// If this target has 2 or more targets after it, add a comma (have to convert size to an int so it can be negative)
		else if (i < (int)copyTargets.size() - 2)
		{
			description += ", ";
		}
	}

	description += " of this card";

	return description;
}

std::string CopyCards::GetIcon()
{
	return "CC";
}


std::string BuffPerFamilyCard::GetDescription()
{
	std::string description = "For each ";
	switch (targetFamily)
	{
	case Cnidaria:
		description += "Cnidaria";
		break;
	case Bird:
		description += "Bird";
		break;
	case Fish:
		description += "Fish";
		break;
	case Shark:
		description += "Shark";
		break;
	case Pinnipeds:
		description += "Pinnipeds";
		break;
	case Mollusk:
		description += "Mollusk";
		break;
	case Crustaceans:
		description += "Crustaceans";
		break;
	case Bug:
		description += "Bug";
		break;
	case Mammal:
		description += "Mammal";
		break;
	case Reptile:
		description += "Reptile";
		break;
	case ERROR:
		description += "[ERROR TYPE]";
		break;
	}
	description += " card, buff this card's ";

	std::vector<int> stats;
	std::vector<std::string> statNames;
	if (hp != 0)
	{
		stats.push_back(hp);
		statNames.push_back("hp");
	}
	if (atk != 0)
	{
		stats.push_back(atk);
		statNames.push_back("attack");
	}
	if (fCost != 0)
	{
		stats.push_back(fCost);
		statNames.push_back("flip cost");
	}

	for (int i = 0; i < stats.size(); i++)
	{
		description += statNames[i] + " by " + std::to_string(stats[i]);

		// If this stat is the 2nd last, add an &
		if (i == stats.size() - 2)
		{
			description += " & ";
		}
		// If this stat has 2 or more stats after it, add a comma (have to convert size to an int so it can be negative)
		else if (i < (int)stats.size() - 2)
		{
			description += ", ";
		}
	}

	return description;
}

std::string BuffPerFamilyCard::GetIcon()
{
	return "BP";
}


std::string BuffEachFamilyCard::GetDescription()
{
	std::string description = "Buff each ";
	switch (targetFamily)
	{
	case Cnidaria:
		description += "Cnidaria";
		break;
	case Bird:
		description += "Bird";
		break;
	case Fish:
		description += "Fish";
		break;
	case Shark:
		description += "Shark";
		break;
	case Pinnipeds:
		description += "Pinnipeds";
		break;
	case Mollusk:
		description += "Mollusk";
		break;
	case Crustaceans:
		description += "Crustaceans";
		break;
	case Bug:
		description += "Bug";
		break;
	case Mammal:
		description += "Mammal";
		break;
	case Reptile:
		description += "Reptile";
		break;
	case ERROR:
		description += "[ERROR TYPE]";
		break;
	}
	description += " card's ";

	std::vector<int> stats;
	std::vector<std::string> statNames;
	if (hp != 0)
	{
		stats.push_back(hp);
		statNames.push_back("hp");
	}
	if (atk != 0)
	{
		stats.push_back(atk);
		statNames.push_back("attack");
	}
	if (fCost != 0)
	{
		stats.push_back(fCost);
		statNames.push_back("flip cost");
	}

	for (int i = 0; i < stats.size(); i++)
	{
		description += statNames[i] + " by " + std::to_string(stats[i]);

		// If this stat is the 2nd last, add an &
		if (i == stats.size() - 2)
		{
			description += " & ";
		}
		// If this stat has 2 or more stats after it, add a comma (have to convert size to an int so it can be negative)
		else if (i < (int)stats.size() - 2)
		{
			description += ", ";
		}
	}

	return description;
}

std::string BuffEachFamilyCard::GetIcon()
{
	return "BE";
}
