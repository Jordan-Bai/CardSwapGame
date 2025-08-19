#include "Player.h"

#include "BoardManager.h"

//const int StartingEnergy = 3;
//const int MaxHandSize = 5;
//const int CostToSwap = 1;

void Player::StartMatch(std::vector<CardData*> deck)
{
	// Copy deck into draw pile & shuffle it: allows cards to be created/ destroyed during
	// match & deleted afterwards without messing with memory management
	
	//for (CardData* card : deck)
	//{
	//	CardData* copyCard = new CardData(card);
	//	m_drawPile.push_back(copyCard);
	//}

	std::vector<int> cardIndexs;

	for (int i = 0; i < deck.size(); i++)
	{
		cardIndexs.push_back(i);
	}

	for (int i = 0; i < deck.size(); i++)
	{
		int chosenIndex = rand() % cardIndexs.size();
		CardData* copyCard = new CardData(deck[cardIndexs[chosenIndex]]);
		m_drawPile.push_back(copyCard);
		cardIndexs.erase(cardIndexs.begin() + chosenIndex);
	}

	m_hp = m_maxHP;
}

void Player::EndMatch()
{
	// Destroy all cards
	for (CardData* card : m_drawPile)
	{
		delete card;
	}
	m_drawPile.clear();

	for (CardData* card : m_discardPile)
	{
		delete card;
	}
	m_discardPile.clear();

	for (CardData* card : m_hand)
	{
		delete card;
	}
	m_hand.clear();
}

void Player::StartTurn()
{
	m_energy = StartingEnergy;
	while (DrawCard()) // Draw cards till you can't anymore
	{

	};

	// Reset whether each card was flipped this turn
	for (int i = 0; i < m_boardRef->GetSlotCount(); i++)
	{
		ActiveCard* card = m_boardRef->GetSlot(i, m_playerIndex);
		if (card != nullptr)
		{
			card->OnStartTurn();
		}
	}
}

bool Player::DrawCard()
{
	// Draw a card
	if (m_hand.size() < MaxHandSize)
	{
		// If draw pile is empty, shuffle discard pile into draw pile
		if (m_drawPile.size() == 0)
		{
			while (m_discardPile.size() > 0)
			{
				int randomIndex = rand() % m_discardPile.size();
				m_drawPile.push_back(m_discardPile[randomIndex]);
				m_discardPile.erase(m_discardPile.begin() + randomIndex);
			}
		}

		if (m_drawPile.size() > 0)
		{
			CardData* drawnCard = m_drawPile[0];
			m_hand.push_back(drawnCard);
			m_drawPile.erase(m_drawPile.begin());
			return true;
		}
	}

	return false;
}

bool Player::PlayCard(int cardIndex, int targetSlot)
{
	if (m_hand.size() > cardIndex)
	{
		CardData* chosentCard = m_hand[cardIndex];
		if (chosentCard->cost > m_energy)
		{
			return false;
		}

		if (m_boardRef->PlayCard(m_hand[cardIndex], targetSlot, m_playerIndex))
		{
			m_energy -= chosentCard->cost;
			m_hand.erase(m_hand.begin() + cardIndex);

			return true;
		}
	}

	return false;
}

bool Player::FlipCard(int cardSlot)
{
	ActiveCard* targetSlot = m_boardRef->GetSlot(cardSlot, m_playerIndex);

	if (targetSlot != nullptr)
	{
		int cost = targetSlot->GetFlipCost();

		if (m_energy >= cost && m_boardRef->FlipCard(targetSlot))
		{
			m_energy -= cost;
			return true;
		}
	}
	
	return false;
}

bool Player::SwapCards(int slot1, int slot2)
{
	ActiveCard* target1 = m_boardRef->GetSlot(slot1, m_playerIndex);
	ActiveCard* target2 = m_boardRef->GetSlot(slot2, m_playerIndex);

	if ((target1 != nullptr || target2 != nullptr) && m_energy >= CostToSwap)
	{
		m_energy -= CostToSwap;
		m_boardRef->SetSlot(slot2, m_playerIndex, target1);
		m_boardRef->SetSlot(slot1, m_playerIndex, target2);
	}

	return false;
}
