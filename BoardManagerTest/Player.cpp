#include "Player.h"

#include "BoardManager.h"

const int StartingEnergy = 3;
const int MaxHandSize = 5;

void Player::StartTurn()
{
	m_energy = StartingEnergy;
	while (DrawCard()) // Draw cards till you can't anymore
	{

	};
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
	//ActiveCard* targetSlot = m_boardRef->GetSlot(cardSlot, m_playerIndex);
	//if (targetSlot != nullptr)
	//{
	//	if (m_energy >= targetSlot->GetFlipCost())
	//	{
	//		targetSlot->m_frontActive = !targetSlot->m_frontActive;
	//		m_energy -= targetSlot->GetFlipCost();
	//		return true;
	//	}
	//}

	//if (m_boardRef->FlipCard(cardSlot, m_playerIndex))
	//{
	//	ActiveCard* targetSlot = m_boardRef->GetSlot(cardSlot, m_playerIndex);
	//	m_energy -= targetSlot->GetFlipCost();
	//}
	ActiveCard* targetSlot = m_boardRef->GetSlot(cardSlot, m_playerIndex);
	if (targetSlot != nullptr && targetSlot->CanFlip())
	{
		m_energy -= targetSlot->GetFlipCost();
		targetSlot->Flip();

		return true;
	}
	
	return false;
}
