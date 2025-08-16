#include "DealerAI.h"

//#include "Behaviour.h"
#include <iostream>

const float HealthWeight = 2.0f;
const float CardWeight = 0.5f;
int branchCount = 0;

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

	branchCount = 0;

	//std::pair<int, std::vector<Behaviour*>> bestBranch = GetBestBranch(std::vector<Behaviour*>());
	std::pair<int, std::vector<Behaviour*>> bestBranch = CheckDestroyPhase(std::vector<Behaviour*>());

	std::cout << branchCount << '\n';

	for (Behaviour* action : bestBranch.second)
	{
		action->DoAction(m_data);
	}

	// Delete all actions
	for (Behaviour* action : bestBranch.second)
	{
		delete action;
	}
}

std::vector<Behaviour*> DealerAI::GetPossibleActions()
{
	int availableEnergy = m_copyDealer->m_energy;
	std::vector<Behaviour*> possibleActions;

	// Get which slots have cards in them
	//==============================================================
	std::vector<int> filledSlots;
	std::vector<int> emptySlots;
	for (int i = 0; i < m_copyBoard->GetSlotCount(); i++)
	{
		ActiveCard* slot = m_copyBoard->GetSlot(i, m_data->m_playerIndex);
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
			int cardCost = m_copyDealer->m_hand[i]->cost;
			if (availableEnergy >= cardCost)
			{
				Behaviour* playCardAction = new PlayCard(i, slot, cardCost);
				possibleActions.push_back(playCardAction);
			}
		}
	}

	// Create "Flip Card" & "Destroy Card" actions
	//==============================================================
	for (int slot : filledSlots)
	{
		ActiveCard* targetCard = m_copyBoard->GetSlot(slot, m_data->m_playerIndex);
		int flipCost = targetCard->GetFlipCost();
		if (targetCard->CanFlip() && availableEnergy >= flipCost)
		{
			Behaviour* flipCardAction = new FlipCard(slot, flipCost);
			possibleActions.push_back(flipCardAction);
		}


		// If the card was played this turn, probably shouldn't be destroyed this turn as well
		ActiveCard* realCard = m_boardRef->GetSlot(slot, m_data->m_playerIndex);
		if (realCard != nullptr && realCard->GetData() == targetCard->GetData())
		{
			Behaviour* destroyCardAction = new DestroyCard(slot);
			possibleActions.push_back(destroyCardAction);
		}
	}

	// Create "Swap Slots" actions
	//==============================================================
	if (availableEnergy >= CostToSwap)
	{
		for (int i = 0; i < filledSlots.size(); i++)
		{
			// Swaping this slot with an empty slot
			for (int slot : emptySlots)
			{
				Behaviour* swapSlotsAction = new SwapSlots(filledSlots[i], slot);
				possibleActions.push_back(swapSlotsAction);
			}
	
			// Swapping this slot with another filled slot (making sure not to make 2 behaviours for each of these)
			for (int j = i + 1; j < filledSlots.size(); j++)
			{
				Behaviour* swapSlotsAction = new SwapSlots(filledSlots[i], filledSlots[j]);
				possibleActions.push_back(swapSlotsAction);
			}
		}
	}

	return possibleActions;
}

std::pair<int, std::vector<Behaviour*>> DealerAI::GetBestBranch(std::vector<Behaviour*> parentSequence)
{
	//std::cout << "Branch depth: " << parentSequence.size() << '\n';

	// Set board state
	CopyBoardData();
	for (Behaviour* action : parentSequence)
	{
		action->DoAction(m_copyDealer);
	}

	std::vector<Behaviour*> possibleActions = GetPossibleActions();

	// First, evaluate the current action sequence
	m_copyBoard->DoAttackPhase();
	float bestScore = EvaluateBoard();

	std::vector<Behaviour*> childSequence;
	
	// Then, evaluate all child action sequences
	for (int i = 0; i < possibleActions.size(); i++)
	{
		std::vector<Behaviour*> totalSequence = parentSequence;
		totalSequence.push_back(possibleActions[i]);
		std::pair<int, std::vector<Behaviour*>> branchScore = GetBestBranch(totalSequence);

		if (branchScore.first > bestScore)
		{
			bestScore = branchScore.first;
			
			// Delete previous best branch
			for (Behaviour* action : childSequence)
			{
				delete action;
			}
			childSequence.clear();

			//childSequence = branchScore.second;
			childSequence.push_back(possibleActions[i]);
			for (Behaviour* action : branchScore.second)
			{
				childSequence.push_back(action);
			}
		}
		else
		{
			for (Behaviour* action : branchScore.second)
			{
				delete action;
			}
			branchScore.second.clear();
		}
	}

	branchCount++;
	return std::pair<int, std::vector<Behaviour*>>(bestScore, childSequence);
}

// OPTIMIZATION V1
//==============================================================
std::vector<Behaviour*> DealerAI::GetDestroyActions()
{
	std::vector<Behaviour*> possibleActions;

	for (int i = 0; i < m_copyBoard->GetSlotCount(); i++)
	{
		ActiveCard* targetCard = m_copyBoard->GetSlot(i, m_data->m_playerIndex);
		if (targetCard != nullptr)
		{
			Behaviour* destroyCardAction = new DestroyCard(i);
			possibleActions.push_back(destroyCardAction);
		}
	}

	return possibleActions;
}

std::vector<Behaviour*> DealerAI::GetPlaceActions()
{
	int availableEnergy = m_copyDealer->m_energy;
	std::vector<Behaviour*> possibleActions;

	for (int i = 0; i < m_copyBoard->GetSlotCount(); i++)
	{
		ActiveCard* targetCard = m_copyBoard->GetSlot(i, m_data->m_playerIndex);
		if (targetCard == nullptr) // If slot is empty, cards can be placed here
		{
			for (int card = 0; card < m_copyDealer->m_hand.size(); card++)
			{
				int cardCost = m_copyDealer->m_hand[card]->cost;
				if (availableEnergy >= cardCost)
				{
					Behaviour* playCardAction = new PlayCard(card, i, cardCost);
					possibleActions.push_back(playCardAction);
				}
			}
		}
	}

	return possibleActions;
}

std::vector<Behaviour*> DealerAI::GetFlipActions()
{
	int availableEnergy = m_copyDealer->m_energy;
	std::vector<Behaviour*> possibleActions;

	for (int i = 0; i < m_copyBoard->GetSlotCount(); i++)
	{
		ActiveCard* targetCard = m_copyBoard->GetSlot(i, m_data->m_playerIndex);
		if (targetCard != nullptr)
		{
			int flipCost = targetCard->GetFlipCost();
			if (targetCard->CanFlip() && availableEnergy >= flipCost)
			{
				Behaviour* flipCardAction = new FlipCard(i, flipCost);
				possibleActions.push_back(flipCardAction);
			}
		}
	}

	return possibleActions;
}

std::vector<Behaviour*> DealerAI::GetSwapActions()
{
	int availableEnergy = m_copyDealer->m_energy;
	std::vector<Behaviour*> possibleActions;

	// Get which slots have cards in them
	//==============================================================
	std::vector<int> filledSlots;
	std::vector<int> emptySlots;
	for (int i = 0; i < m_copyBoard->GetSlotCount(); i++)
	{
		ActiveCard* slot = m_copyBoard->GetSlot(i, m_data->m_playerIndex);
		if (slot != nullptr)
		{
			filledSlots.push_back(i);
		}
		else
		{
			emptySlots.push_back(i);
		}
	}

	if (availableEnergy >= CostToSwap)
	{
		for (int i = 0; i < filledSlots.size(); i++)
		{
			// Swaping this slot with an empty slot
			for (int slot : emptySlots)
			{
				Behaviour* swapSlotsAction = new SwapSlots(filledSlots[i], slot);
				possibleActions.push_back(swapSlotsAction);
			}

			// Swapping this slot with another filled slot (making sure not to make 2 behaviours for each of these)
			for (int j = i + 1; j < filledSlots.size(); j++)
			{
				Behaviour* swapSlotsAction = new SwapSlots(filledSlots[i], filledSlots[j]);
				possibleActions.push_back(swapSlotsAction);
			}
		}
	}

	return possibleActions;
}


std::pair<int, std::vector<Behaviour*>> DealerAI::CheckDestroyPhase(std::vector<Behaviour*> parentSequence)
{
	CopyBoardData();
	for (Behaviour* action : parentSequence)
	{
		action->DoAction(m_copyDealer);
	}

	std::vector<Behaviour*> possibleActions = GetDestroyActions();

	// First, evaluate moving on to the next phase
	std::pair<int, std::vector<Behaviour*>> branchScore = CheckPlacePhase(parentSequence);

	float bestScore = branchScore.first;
	std::vector<Behaviour*> childSequence = branchScore.second;


	// Then, evaluate all child action sequences in the current phase
	for (int i = 0; i < possibleActions.size(); i++)
	{
		std::vector<Behaviour*> totalSequence = parentSequence;
		totalSequence.push_back(possibleActions[i]);
		std::pair<int, std::vector<Behaviour*>> branchScore = CheckDestroyPhase(totalSequence);

		if (branchScore.first > bestScore)
		{
			bestScore = branchScore.first;

			// Delete previous best branch
			for (Behaviour* action : childSequence)
			{
				delete action;
			}
			childSequence.clear();

			childSequence = branchScore.second;
			childSequence.insert(childSequence.begin(), possibleActions[i]);
		}
		else
		{
			for (Behaviour* action : branchScore.second)
			{
				delete action;
			}
			branchScore.second.clear();
		}
	}

	return std::pair<int, std::vector<Behaviour*>>(bestScore, childSequence);
}

std::pair<int, std::vector<Behaviour*>> DealerAI::CheckPlacePhase(std::vector<Behaviour*> parentSequence)
{
	CopyBoardData();
	for (Behaviour* action : parentSequence)
	{
		action->DoAction(m_copyDealer);
	}

	std::vector<Behaviour*> possibleActions = GetPlaceActions();

	// First, evaluate moving on to the next phase
	std::pair<int, std::vector<Behaviour*>> branchScore = CheckFlipPhase(parentSequence);

	float bestScore = branchScore.first;
	std::vector<Behaviour*> childSequence = branchScore.second;


	// Then, evaluate all child action sequences in the current phase
	for (int i = 0; i < possibleActions.size(); i++)
	{
		std::vector<Behaviour*> totalSequence = parentSequence;
		totalSequence.push_back(possibleActions[i]);
		std::pair<int, std::vector<Behaviour*>> branchScore = CheckPlacePhase(totalSequence);

		if (branchScore.first > bestScore)
		{
			bestScore = branchScore.first;

			// Delete previous best branch
			for (Behaviour* action : childSequence)
			{
				delete action;
			}
			childSequence.clear();

			childSequence = branchScore.second;
			childSequence.insert(childSequence.begin(), possibleActions[i]);
		}
		else
		{
			for (Behaviour* action : branchScore.second)
			{
				delete action;
			}
			branchScore.second.clear();
		}
	}

	return std::pair<int, std::vector<Behaviour*>>(bestScore, childSequence);
}

std::pair<int, std::vector<Behaviour*>> DealerAI::CheckFlipPhase(std::vector<Behaviour*> parentSequence)
{
	CopyBoardData();
	for (Behaviour* action : parentSequence)
	{
		action->DoAction(m_copyDealer);
	}

	std::vector<Behaviour*> possibleActions = GetFlipActions();

	// First, evaluate moving on to the next phase
	std::pair<int, std::vector<Behaviour*>> branchScore = CheckSwapPhase(parentSequence);
	
	float bestScore = branchScore.first;
	std::vector<Behaviour*> childSequence = branchScore.second;

	//// First, evaluate the current action sequence
	//m_copyBoard->DoAttackPhase();
	//float bestScore = EvaluateBoard();
	//std::vector<Behaviour*> childSequence;


	// Then, evaluate all child action sequences in the current phase
	for (int i = 0; i < possibleActions.size(); i++)
	{
		std::vector<Behaviour*> totalSequence = parentSequence;
		totalSequence.push_back(possibleActions[i]);
		std::pair<int, std::vector<Behaviour*>> branchScore = CheckFlipPhase(totalSequence);

		if (branchScore.first > bestScore)
		{
			bestScore = branchScore.first;

			// Delete previous best branch
			for (Behaviour* action : childSequence)
			{
				delete action;
			}
			childSequence.clear();

			childSequence = branchScore.second;
			childSequence.insert(childSequence.begin(), possibleActions[i]);
		}
		else
		{
			for (Behaviour* action : branchScore.second)
			{
				delete action;
			}
			branchScore.second.clear();
		}
	}

	//branchCount++;
	return std::pair<int, std::vector<Behaviour*>>(bestScore, childSequence);
}

std::pair<int, std::vector<Behaviour*>> DealerAI::CheckSwapPhase(std::vector<Behaviour*> parentSequence)
{
	CopyBoardData();
	for (Behaviour* action : parentSequence)
	{
		action->DoAction(m_copyDealer);
	}

	std::vector<Behaviour*> possibleActions = GetSwapActions();

	// First, evaluate the current action sequence
	m_copyBoard->DoAttackPhase();
	float bestScore = EvaluateBoard();
	std::vector<Behaviour*> childSequence;


	// Then, evaluate all child action sequences in the current phase
	for (int i = 0; i < possibleActions.size(); i++)
	{
		std::vector<Behaviour*> totalSequence = parentSequence;
		totalSequence.push_back(possibleActions[i]);
		std::pair<int, std::vector<Behaviour*>> branchScore = CheckSwapPhase(totalSequence);

		if (branchScore.first > bestScore)
		{
			bestScore = branchScore.first;

			// Delete previous best branch
			for (Behaviour* action : childSequence)
			{
				delete action;
			}
			childSequence.clear();

			childSequence = branchScore.second;
			childSequence.insert(childSequence.begin(), possibleActions[i]);
		}
		else
		{
			for (Behaviour* action : branchScore.second)
			{
				delete action;
			}
			branchScore.second.clear();
		}
	}

	branchCount++;
	return std::pair<int, std::vector<Behaviour*>>(bestScore, childSequence);
}
//==============================================================


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
			ActiveCard* copyCard = new ActiveCard(cardSide1);

			m_copyBoard->SetSlot(i, 1, copyCard);
		}
		ActiveCard* cardSide2 = m_boardRef->GetSlot(i, 2);
		if (cardSide2 != nullptr)
		{
			ActiveCard* copyCard = new ActiveCard(cardSide2);

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
