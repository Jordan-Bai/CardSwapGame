#include "Player.h"

#include "BoardManager.h"

//const int StartingEnergy = 3;
//const int MaxHandSize = 5;
//const int CostToSwap = 1;

Player::Player()
	: m_boardRef(nullptr), m_energy(0), m_playerIndex(-1)
{
}

Player::~Player()
{
	EndMatch(); // Destroy all cards in draw pile/ discard pile/ hand
}

void Player::StartMatch(std::vector<CardData*> deck)
{
	// Copy deck into draw pile & shuffle it
	std::vector<int> cardIndexs;

	for (int i = 0; i < deck.size(); i++)
	{
		cardIndexs.push_back(i);
	}

	for (int i = 0; i < deck.size(); i++)
	{
		int chosenIndex = rand() % cardIndexs.size();
		//CardData* copyCard = new CardData(deck[cardIndexs[chosenIndex]]);
		//m_drawPile.push_back(copyCard);
		m_drawPile.push_back(deck[cardIndexs[chosenIndex]]);
		cardIndexs.erase(cardIndexs.begin() + chosenIndex);
		//copyCard->REAL = true;
	}

	m_hp = m_maxHP;
}

void Player::EndMatch()
{
	// Destroy any temp cards
	for (CardData* card : m_drawPile)
	{
		if (card->isTemp)
		{
			delete card;
		}
	}
	m_drawPile.clear();

	for (CardData* card : m_discardPile)
	{
		if (card->isTemp)
		{
			delete card;
		}
	}
	m_discardPile.clear();

	for (CardData* card : m_hand)
	{
		if (card->isTemp)
		{
			delete card;
		}
	}
	m_hand.clear();
}

void Player::StartTurn()
{
	m_energy = StartingEnergy;
	while (DrawCard()) // Draw cards till you can't anymore
	{

	};

	m_boardRef->TurnStarts(m_playerIndex);
	//// Reset whether each card was flipped this turn
	//for (int i = 0; i < m_boardRef->GetSlotCount(); i++)
	//{
	//	ActiveCard* card = m_boardRef->GetSlot(i, m_playerIndex);
	//	if (card != nullptr)
	//	{
	//		card->OnStartTurn();
	//	}
	//}
}

void Player::EndTurn()
{
	m_boardRef->TurnEnds(m_playerIndex);
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

void Player::PickupCard(CardData* card)
{
	m_hand.push_back(card);
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
	if (m_energy >= CostToSwap)
	{
		if (m_boardRef->SwapCards(slot1, slot2, m_playerIndex))
		{
			m_energy -= CostToSwap;
			return true;
		}
	}

	return false;
}

bool Player::ActivateCard(int cardSlot)
{
	ActiveCard* targetSlot = m_boardRef->GetSlot(cardSlot, m_playerIndex);

	if (targetSlot != nullptr)
	{
		int cost = targetSlot->GetAbilityCost();

		if (m_energy >= cost && m_boardRef->ActivateCard(targetSlot))
		{
			m_energy -= cost;
			return true;
		}
	}

	return false;
}
