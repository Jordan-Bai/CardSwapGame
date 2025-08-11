#include "DealerAI.h"

#include "Behaviour.h"
#include <iostream>

const float HealthWeight = 2.0f;
const float CardWeight = 0.5f;

DealerAI::DealerAI(BoardManager* board, Player* playerData)
	:m_boardRef(board), m_data(playerData)
{
	m_copyDealer = new Player();
	m_copyPlayer = new Player();
	m_copyBoard = new BoardManager(m_copyDealer, m_copyPlayer, m_boardRef->GetSlotCount());
}

DealerAI::~DealerAI()
{
	delete m_copyDealer;
	delete m_copyPlayer;
	delete m_copyBoard;
}


void DealerAI::StartTurn()
{
	m_data->StartTurn();

	CopyBoardData();

	std::vector<Behaviour*> possibleActions;

	// Get which slots have cards in them
	//==============================================================
	std::vector<int> filledSlots;
	std::vector<int> emptySlots;
	for (int i = 0; i < m_boardRef->GetSlotCount(); i++)
	{
		ActiveCard* slot = m_boardRef->GetSlot(i, m_data->m_playerIndex);
		if (slot != nullptr)
		{
			filledSlots.push_back(i);
		}
		else
		{
			emptySlots.push_back(i);
		}
	}

	// Create "Play Card" actions
	//==============================================================
	for (int i = 0; i < m_copyDealer->m_hand.size(); i++)
	{
		for (int slot : emptySlots)
		{
			Behaviour* playCardAction = new PlayCard(i, slot);
			possibleActions.push_back(playCardAction);
		}
	}

	// Create "Flip Card" & "Destroy Card" actions
	//==============================================================
	for (int slot : filledSlots)
	{
		Behaviour* flipCardAction = new FlipCard(slot);
		possibleActions.push_back(flipCardAction);
		Behaviour* destroyCardAction = new DestroyCard(slot);
		possibleActions.push_back(destroyCardAction);
	}


	// Evaluate each action 
	//==============================================================
	float bestScore = -FLT_MAX;
	Behaviour* bestAction = nullptr;

	for (Behaviour* action : possibleActions)
	{
		CopyBoardData();
		action->DoAction(m_copyDealer);

		m_copyBoard->DoAttackPhase();
		float actionScore = EvaluateBoard();
		
		if (actionScore > bestScore)
		{
			bestScore = actionScore;
			bestAction = action;
		}
	}

	// After evaluating each action, actually do the best one
	if (bestAction == nullptr)
	{
		std::cout << "ERROR: Dealer can't do any actions\n";
	}
	else
	{
		bestAction->DoAction(m_data);
	}

	// Delete all actions
	for (Behaviour* action : possibleActions)
	{
		delete action;
	}
}


void DealerAI::CopyBoardData()
{
	for (int i = 0; i < m_copyBoard->GetSlotCount(); i++)
	{
		// Clear current cards
		m_copyBoard->DestroyCard(i, 1);
		m_copyBoard->DestroyCard(i, 2);

		// Copy cards from real board
		ActiveCard* cardSide1 = m_boardRef->GetSlot(i, 1);
		if (cardSide1 != nullptr)
		{
			ActiveCard* copyCard = new ActiveCard(cardSide1->GetData(), i, 1);
			copyCard->m_frontActive = cardSide1->m_frontActive;
			copyCard->TakeDamage(cardSide1->GetDamageTaken());
			m_copyBoard->SetSlot(i, 1, copyCard);
		}
		ActiveCard* cardSide2 = m_boardRef->GetSlot(i, 2);
		if (cardSide2 != nullptr)
		{
			ActiveCard* copyCard = new ActiveCard(cardSide2->GetData(), i, 2);
			copyCard->m_frontActive = cardSide2->m_frontActive;
			copyCard->TakeDamage(cardSide2->GetDamageTaken());
			m_copyBoard->SetSlot(i, 2, copyCard);
		}

		// Copy player data
		CopyPlayerData(m_copyDealer, m_data);
		CopyPlayerData(m_copyPlayer, m_boardRef->GetPlayer(m_boardRef->OppositeSide(m_data->m_playerIndex)));
	}
}

void DealerAI::CopyPlayerData(Player* copyTarget, Player* copySource)
{
	copyTarget->m_hp = copySource->m_hp;
	copyTarget->m_energy = copySource->m_energy;
	copyTarget->m_drawPile.clear();
	copyTarget->m_drawPile = copySource->m_drawPile;
	copyTarget->m_discardPile.clear();
	copyTarget->m_discardPile = copySource->m_discardPile;
	copyTarget->m_hand.clear();
	copyTarget->m_hand = copySource->m_hand;
}

float DealerAI::EvaluateBoard()
{
	float finalScore = 0;

	if (m_copyDealer->m_hp <= 0)
	{
		// The dealer dying has more weight than the player dying, since if the dealer dies the game will end before the player dies
		finalScore -= 100;
	}
	if (m_copyPlayer->m_hp <= 0)
	{
		finalScore += 50;
	}

	finalScore += m_copyDealer->m_hp * HealthWeight;
	finalScore -= m_copyPlayer->m_hp * HealthWeight;

	float cardScore = 0;

	// probably also want to see how many cards the opponent has/ how many cards the dealer has
	for (int i = 0; i < m_copyBoard->GetSlotCount(); i++)
	{
		ActiveCard* dealerCard = m_copyBoard->GetSlot(i, m_data->m_playerIndex);
		if (dealerCard != nullptr)
		{
			cardScore += dealerCard->GetAtk() + dealerCard->GetHP();
		}

		ActiveCard* playerCard = m_copyBoard->GetSlot(i, m_copyBoard->OppositeSide(m_data->m_playerIndex));
		if (playerCard != nullptr)
		{
			cardScore -= playerCard->GetAtk() + playerCard->GetHP();
		}
	}

	finalScore += cardScore * CardWeight;

	return finalScore;
}
