#include "BoardManager.h"

#include <iostream>
#include <string>

BoardManager::BoardManager(Player* dealer, Player* player, int slotsPerSide)
	:m_player1(dealer), m_player2(player), m_slots(slotsPerSide)
{
	m_player1->m_playerIndex = 1;
	m_player1->m_boardRef = this;
	m_player2->m_playerIndex = 2;
	m_player2->m_boardRef = this;

	m_side1 = new ActiveCard*[m_slots];
	m_side2 = new ActiveCard*[m_slots];

	for (int i = 0; i < m_slots; i++)
	{
		m_side1[i] = nullptr;
		m_side2[i] = nullptr;
	}
}

BoardManager::~BoardManager()
{
	for (int i = 0; i < m_slots; i++)
	{
		if (m_side1[i] != nullptr)
		{
			delete m_side1[i];
		}
		if (m_side2[i] != nullptr)
		{
			delete m_side2[i];
		}
	}

	delete[] m_side1;
	delete[] m_side2;
}


bool BoardManager::PlayCard(CardData* data, int slot, int playerIndex)
{
	if (playerIndex == 1)
	{
		if (m_side1[slot] != nullptr)
		{
			return false;
		}

		ActiveCard* placedCard = new ActiveCard(data, slot, playerIndex);
		m_side1[slot] = placedCard;
	}
	else
	{
		if (m_side2[slot] != nullptr)
		{
			return false;
		}

		ActiveCard* placedCard = new ActiveCard(data, slot, playerIndex);
		m_side2[slot] = placedCard;
	}

	return true;
}

void BoardManager::DoAttackPhase()
{
	// Player's cards attack first
	for (int i = 0; i < m_slots; i++)
	{
		if (m_side2[i] != nullptr)
		{
			std::vector<int> cardTargets = m_side2[i]->GetTargets();
			for (int slot : cardTargets)
			{
				PerformAttack(m_side2[i], slot);
			}
		}
	}

	if (m_player1->m_hp <= 0) // If the dealer is dead
	{
		// TODO: End game when participant dies
	}

	// Dealer's cards attack second
	for (int i = 0; i < m_slots; i++)
	{
		if (m_side1[i] != nullptr)
		{
			std::vector<int> cardTargets = m_side1[i]->GetTargets();
			for (int slot : cardTargets)
			{
				PerformAttack(m_side1[i], slot);
			}
		}
	}

	if (m_player2->m_hp <= 0) // If the player is dead
	{
		// TODO: End game when participant dies
	}
}

void BoardManager::PerformAttack(ActiveCard* attacker, int targetSlot)
{
	if (targetSlot < 0 || targetSlot >= m_slots)
	{
		return;
	}

	int defendingSide = OppositeSide(attacker->m_side);
	ActiveCard* defendingCard = GetSlot(targetSlot, defendingSide);
	Player* defendingPlayer = GetPlayer(defendingSide);


	if (defendingCard == nullptr)
	{
		// Attack opponent directly
		defendingPlayer->m_hp -= attacker->GetAtk();
	}
	else
	{
		defendingCard->TakeDamage(attacker->GetAtk());
		if (defendingCard->GetHP() <= 0)
		{
			DestroyCard(defendingCard);
		}
	}
}

void BoardManager::DestroyCard(ActiveCard* card)
{
	// Add card to corresponding discard pile
	GetPlayer(card->m_side)->m_discardPile.push_back(card->GetData());

	// Remove card from slot
	SetSlot(card->m_slot, card->m_side, nullptr);

	// Destroy card
	delete card;
}

void BoardManager::DisplayBoard()
{
	// Print dealer health
	//==============================================================
	std::cout << "\nHP: " << std::to_string(m_player1->m_hp) << '\n';

	// Print dealer hand
	//==============================================================
	std::string dealerHand;
	for (CardData* card : m_player1->m_hand)
	{
		dealerHand += " <---> ";
	}
	std::cout << dealerHand << '\n';

	// Print dealer side
	//==============================================================
	std::cout << "============================\n";

	std::string dealerSide1;
	std::string dealerSide2;
	for (int i = 0; i < m_slots; i++)
	{
		dealerSide1 += " |";
		dealerSide2 += " |";
		if (m_side1[i] == nullptr)
		{
			dealerSide1 += "---";
			dealerSide2 += "---";
		}
		else
		{
			dealerSide1 += std::to_string(m_side1[i]->GetCost()) + "--";
			dealerSide2 += std::to_string(m_side1[i]->GetAtk()) + "-" + std::to_string(m_side1[i]->GetHP());
		}
		dealerSide1 += "| ";
		dealerSide2 += "| ";
	}
	std::cout << dealerSide1 << '\n' << dealerSide2 << "\n\n";


	// Print player side
	//==============================================================
	std::string playerSide1;
	std::string playerSide2;
	for (int i = 0; i < m_slots; i++)
	{
		playerSide1 += " |";
		playerSide2 += " |";
		if (m_side2[i] == nullptr)
		{
			playerSide1 += "---";
			playerSide2 += "---";
		}
		else
		{
			playerSide1 += std::to_string(m_side2[i]->GetCost()) + "--";
			playerSide2 += std::to_string(m_side2[i]->GetAtk()) + "-" + std::to_string(m_side2[i]->GetHP());
		}
		playerSide1 += "| ";
		playerSide2 += "| ";
	}
	std::cout << playerSide1 << '\n' << playerSide2 << '\n';

	std::cout << "============================\n";


	// Print player hand
	//==============================================================
	std::string playerHand1;
	std::string playerHand2;
	for (CardData* card : m_player2->m_hand)
	{
		CreatureData* frontCreature = card->frontCreature;
		playerHand1 += " <" + std::to_string(card->cost) + "--> ";
		playerHand2 += " <" + std::to_string(frontCreature->atk) + "-" + std::to_string(frontCreature->hp) + "> ";
	}
	std::cout << playerHand1 << '\n' << playerHand2 << '\n';


	// Print player stats
	//==============================================================
	std::cout << "HP: " << std::to_string(m_player2->m_hp) << "   Energy: " << std::to_string(m_player2->m_energy) << "\n\n";
}


int BoardManager::OppositeSide(int side)
{
	if (side == 1)
	{
		return 2;
	}
	else
	{
		return 1;
	}
}

Player* BoardManager::GetPlayer(int side)
{
	if (side == 1)
	{
		return m_player1;
	}
	else
	{
		return m_player2;
	}
}

ActiveCard* BoardManager::GetSlot(int slot, int side)
{
	if (side == 1)
	{
		return m_side1[slot];
	}
	else
	{
		return m_side2[slot];
	}
}

void BoardManager::SetSlot(int slot, int side, ActiveCard* newCard)
{
	if (side == 1)
	{
		m_side1[slot] = newCard;
	}
	else
	{
		m_side2[slot] = newCard;
	}
}
