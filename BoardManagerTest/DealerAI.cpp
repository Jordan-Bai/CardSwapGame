#include "DealerAI.h"

//#include "Behaviour.h"
#include <iostream>
#include <string>

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
	//std::pair<float, std::vector<Behaviour*>> bestBranch = GetBestBranch(std::vector<Behaviour*>());
	std::pair<float, std::vector<Behaviour*>> bestBranch = CheckDestroyPhase(std::vector<Behaviour*>(), 0);

	for (Behaviour* action : bestBranch.second)
	{
		action->DoAction(m_data);
	}

	// Delete all actions
	for (Behaviour* action : bestBranch.second)
	{
		delete action;
	}
	bestBranch.second.clear();

	std::cout << branchCount << '\n';
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


		// If the card was played this turn, proobably shouldn't be destroyed this turn as well
		ActiveCard* realCard = m_boardRef->GetSlot(slot, m_data->m_playerIndex);
		if (realCard != nullptr && realCard->GetData() == targetCard->GetData())
		{
			Behaviour* destroyCardAction = new DestroyCard(slot);
			possibleActions.push_back(destroyCardAction);
		}
	}

	// Create "Swap Slots" actions - CURRENTLY DISABLED FOR PERFORMANCE
	//==============================================================
	//if (availableEnergy >= CostToSwap)
	//{
	//	for (int i = 0; i < filledSlots.size(); i++)
	//	{
	//		// Swaping this slot with an empty slot
	//		for (int slot : emptySlots)
	//		{
	//			Behaviour* swapSlotsAction = new SwapSlots(filledSlots[i], slot);
	//			possibleActions.push_back(swapSlotsAction);
	//		}
	//
	//		// Swapping this slot with another filled slot (making sure not to make 2 behaviours for each of these)
	//		for (int j = i + 1; j < filledSlots.size(); j++)
	//		{
	//			Behaviour* swapSlotsAction = new SwapSlots(filledSlots[i], filledSlots[j]);
	//			possibleActions.push_back(swapSlotsAction);
	//		}
	//	}
	//}

	return possibleActions;
}

std::pair<float, std::vector<Behaviour*>> DealerAI::GetBestBranch(std::vector<Behaviour*> parentSequence)
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

	//if (possibleActions.size() == 0)
	//{
	//	if (bestScore > 30)
	//	{
	//		std::string debug;
	//		for (Behaviour* action : parentSequence)
	//		{
	//			PlayCard* cardAction = dynamic_cast<PlayCard*>(action);
	//			if (cardAction != nullptr)
	//			{
	//				debug += "(" + std::to_string(cardAction->m_card) + ", " + std::to_string(cardAction->m_target) + ") ";
	//			}
	//			else
	//			{
	//				FlipCard* flipAction = dynamic_cast<FlipCard*>(action);
	//				if (flipAction != nullptr)
	//				{
	//					debug += "(" + std::to_string(flipAction->m_target) + ") ";
	//				}
	//			}
	//		}
	//		std::cout << debug << ": " << std::to_string(bestScore) << '\n';
	//	}
	//}

	return std::pair<int, std::vector<Behaviour*>>(bestScore, childSequence);
}


// OPTIMIZATION
//==============================================================
std::vector<Behaviour*> DealerAI::GetDestroyActions(int slot)
{
	std::vector<Behaviour*> possibleActions;

	ActiveCard* targetCard = m_copyBoard->GetSlot(slot, m_data->m_playerIndex);
	if (targetCard != nullptr)
	{
		Behaviour* destroyCardAction = new DestroyCard(slot);
		possibleActions.push_back(destroyCardAction);
	}

	if (possibleActions.size() > 0)
	{
		std::cout << "Creating " << possibleActions.size() << " destroy behaviours.\n";
	}

	return possibleActions;
}

std::vector<Behaviour*> DealerAI::GetPlaceActions(int slot)
{
	int availableEnergy = m_copyDealer->m_energy;
	std::vector<Behaviour*> possibleActions;

	ActiveCard* targetCard = m_copyBoard->GetSlot(slot, m_data->m_playerIndex);
	if (targetCard == nullptr)
	{
		for (int i = 0; i < m_copyDealer->m_hand.size(); i++)
		{
			int cardCost = m_copyDealer->m_hand[i]->cost;
			if (availableEnergy >= cardCost)
			{
				Behaviour* playCardAction = new PlayCard(i, slot, cardCost);
				possibleActions.push_back(playCardAction);
			}
		}

		if (possibleActions.size() > 0)
		{
			std::cout << "Creating " << possibleActions.size() << " place behaviours.\n";
		}
	}

	return possibleActions;
}

std::vector<Behaviour*> DealerAI::GetFlipActions(int slot)
{
	int availableEnergy = m_copyDealer->m_energy;
	std::vector<Behaviour*> possibleActions;

	ActiveCard* targetCard = m_copyBoard->GetSlot(slot, m_data->m_playerIndex);
	if (targetCard != nullptr)
	{
		int flipCost = targetCard->GetFlipCost();
		if (targetCard->CanFlip() && availableEnergy >= flipCost)
		{
			Behaviour* flipCardAction = new FlipCard(slot, flipCost);
			possibleActions.push_back(flipCardAction);
		}
	}

	if (possibleActions.size() > 0)
	{
		std::cout << "Creating " << possibleActions.size() << " flip behaviours.\n";
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

	if (possibleActions.size() > 0)
	{
		std::cout << "Creating " << possibleActions.size() << " swap behaviours.\n";
	}

	return possibleActions;
}

std::pair<float, std::vector<Behaviour*>> DealerAI::CheckDestroyPhase(std::vector<Behaviour*> parentSequence, int slot)
{
	CopyBoardData();
	for (Behaviour* action : parentSequence)
	{
		action->DoAction(m_copyDealer);
	}
	std::vector<Behaviour*> possibleActions = GetDestroyActions(slot);

	float bestScore = 0;
	std::vector<Behaviour*> childSequence;

	// Check if this should move on to the next phase
	if (slot >= m_copyBoard->GetSlotCount())
	{
		std::pair<float, std::vector<Behaviour*>> branchScore = CheckPlacePhase(parentSequence, 0);
		bestScore = branchScore.first;
		childSequence = branchScore.second;
	}
	else
	{
		// First, evaluate skipping this slot
		std::pair<float, std::vector<Behaviour*>> branchScore = CheckDestroyPhase(parentSequence, slot + 1);
		bestScore = branchScore.first;
		childSequence = branchScore.second;

		// Then, evaluate all actions for this slot
		//std::vector<Behaviour*> possibleActions = GetDestroyActions(slot);
		for (int i = 0; i < possibleActions.size(); i++)
		{
			std::vector<Behaviour*> totalSequence = parentSequence;
			totalSequence.push_back(possibleActions[i]);
			branchScore = CheckDestroyPhase(totalSequence, slot + 1);

			if (branchScore.first > bestScore)
			{
				bestScore = branchScore.first;

				// Delete previous best branch
				for (Behaviour* action : childSequence)
				{
					delete action;
				}
				std::cout << "Deleting " << childSequence.size() << " old destroy behaviours.\n";
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
				std::cout << "Deleting " << branchScore.second.size() << " new destroy behaviours.\n";
				branchScore.second.clear();

				delete possibleActions[i];
				std::cout << "Deleting 1 destroy behaviour.\n";
			}
		}
	}

	return std::pair<float, std::vector<Behaviour*>>(bestScore, childSequence);
}

std::pair<float, std::vector<Behaviour*>> DealerAI::CheckPlacePhase(std::vector<Behaviour*> parentSequence, int slot)
{
	CopyBoardData();
	for (Behaviour* action : parentSequence)
	{
		action->DoAction(m_copyDealer);
	}
	std::vector<Behaviour*> possibleActions = GetPlaceActions(slot);

	float bestScore = -FLT_MAX;
	std::vector<Behaviour*> childSequence;

	// Check if this should move on to the next phase
	if (slot >= m_copyBoard->GetSlotCount())
	{
		//std::pair<float, std::vector<Behaviour*>> branchScore = CheckSwapPhase(parentSequence);
		std::pair<float, std::vector<Behaviour*>> branchScore = CheckFlipPhase(parentSequence, 0);
		bestScore = branchScore.first;
		childSequence = branchScore.second;
	}
	else
	{
		// First, evaluate skipping this slot
		std::pair<float, std::vector<Behaviour*>> branchScore = CheckPlacePhase(parentSequence, slot + 1);
		bestScore = branchScore.first;
		childSequence = branchScore.second;
		
		// Then, evaluate all actions for this slot
		//std::vector<Behaviour*> possibleActions = GetPlaceActions(slot);
		for (int i = 0; i < possibleActions.size(); i++)
		{
			std::vector<Behaviour*> totalSequence = parentSequence;
			totalSequence.push_back(possibleActions[i]); // POSSIBLE ACTIONS NOT BEING DELETED PROPERLY
			branchScore = CheckPlacePhase(totalSequence, slot + 1);
		
			if (branchScore.first > bestScore)
			{
				bestScore = branchScore.first;
		
				// Delete previous best branch
				for (Behaviour* action : childSequence)
				{
					delete action;
				}
				std::cout << "Deleting " << childSequence.size() << " old place behaviours.\n";
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
				std::cout << "Deleting " << branchScore.second.size() << " new place behaviours.\n";
				branchScore.second.clear();

				delete possibleActions[i];
				std::cout << "Deleting 1 place behaviour.\n";
			}
		}
	}

	return std::pair<float, std::vector<Behaviour*>>(bestScore, childSequence);
}

std::pair<float, std::vector<Behaviour*>> DealerAI::CheckFlipPhase(std::vector<Behaviour*> parentSequence, int slot)
{
	//return CheckSwapPhase(parentSequence);

	CopyBoardData();
	for (Behaviour* action : parentSequence)
	{
		action->DoAction(m_copyDealer);
	}
	//std::vector<Behaviour*> possibleActions = GetFlipActions(slot);


	// V1
	//==============================================================
	if (true)
	{
		float bestScore;
		std::vector<Behaviour*> childSequence;

		// Check if this should move on to the next phase
		if (slot >= m_copyBoard->GetSlotCount())
		{
			std::pair<float, std::vector<Behaviour*>> branchScore = CheckSwapPhase(parentSequence);
			bestScore = branchScore.first;
			childSequence = branchScore.second;
		}
		else
		{
			// First, evaluate skipping this slot
			std::pair<float, std::vector<Behaviour*>> branchScore = CheckFlipPhase(parentSequence, slot + 1);
			bestScore = branchScore.first;
			childSequence = branchScore.second;

			// Then, evaluate all actions for this slot
			std::vector<Behaviour*> possibleActions = GetFlipActions(slot);
			for (float i = 0; i < possibleActions.size(); i++)
			{
				std::vector<Behaviour*> totalSequence = parentSequence;
				totalSequence.push_back(possibleActions[i]);
				branchScore = CheckFlipPhase(totalSequence, slot + 1);

				if (branchScore.first > bestScore)
				{
					bestScore = branchScore.first;

					// Delete previous best branch
					for (Behaviour* action : childSequence)
					{
						delete action;
					}
					std::cout << "Deleting " << childSequence.size() << " old flip behaviours.\n";
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
					std::cout << "Deleting " << branchScore.second.size() << " new flip behaviours.\n";
					branchScore.second.clear();

					delete possibleActions[i];
					std::cout << "Deleting 1 flip behaviour.\n";
				}
			}
		}

		return std::pair<float, std::vector<Behaviour*>>(bestScore, childSequence);
	}
	//==============================================================

	// V2 -- NOT WORKING
	//==============================================================
	// Not working because possibleAction[i] isn't being added to the sequence, I think getting it to be added correctly would probably
	// undo the point of this version which was to make the function smaller/ more readable
	{
		std::pair<float, std::vector<Behaviour*>> bestBranch;

		// Check if this should move on to the next phase
		if (slot >= m_copyBoard->GetSlotCount())
		{
			bestBranch = CheckSwapPhase(parentSequence);
		}
		else
		{
			// First, evaluate skipping this slot
			bestBranch = CheckFlipPhase(parentSequence, slot + 1);

			// Then, evaluate all actions for this slot
			std::vector<Behaviour*> possibleActions = GetFlipActions(slot);
			for (float i = 0; i < possibleActions.size(); i++)
			{
				std::vector<Behaviour*> totalSequence = parentSequence;
				totalSequence.push_back(possibleActions[i]); 
				//bestBranch = BestBranch(CheckFlipPhase(totalSequence, slot + 1), bestBranch); // Doesn't work for some reason??

				std::pair<float, std::vector<Behaviour*>> newBestBranch = BestBranch(bestBranch, CheckFlipPhase(totalSequence, slot + 1));
				bestBranch = newBestBranch;
			}
		}

		return bestBranch;
	}
	//==============================================================
}

std::pair<float, std::vector<Behaviour*>> DealerAI::CheckSwapPhase(std::vector<Behaviour*> parentSequence)
{
	CopyBoardData();
	for (Behaviour* action : parentSequence)
	{
		action->DoAction(m_copyDealer);
	}

	// First, evaluate the current action sequence
	m_copyBoard->DoAttackPhase();
	float bestScore = EvaluateBoard();

	// PRINTING STUFF FOR DEBUGGING
	//if (bestScore > 20)
	//{
	//	std::string debug;
	//	for (Behaviour* action : parentSequence)
	//	{
	//		PlayCard* cardAction = dynamic_cast<PlayCard*>(action);
	//		if (cardAction != nullptr)
	//		{
	//			debug += "(" + std::to_string(cardAction->m_card) + ", " + std::to_string(cardAction->m_target) + ") ";
	//		}
	//		else
	//		{
	//			FlipCard* flipAction = dynamic_cast<FlipCard*>(action);
	//			if (flipAction != nullptr)
	//			{
	//				debug += "(" + std::to_string(flipAction->m_target) + ") ";
	//			}
	//		}
	//	}
	//	std::cout << debug << ": " << std::to_string(bestScore) << '\n';
	//}

	std::vector<Behaviour*> childSequence;

	//branchCount++;
	return std::pair<float, std::vector<Behaviour*>>(bestScore, childSequence);
}
//==============================================================


std::pair<float, std::vector<Behaviour*>> DealerAI::BestBranch(std::pair<float, std::vector<Behaviour*>> branch1, std::pair<float, std::vector<Behaviour*>> branch2)
{
	//std::pair<float, std::vector<Behaviour*>> betterBranch = branch1;
	//std::vector<Behaviour*> branchToDelete = branch2.second;
	//if (branch2.first > branch1.first)
	//{
	//	betterBranch = branch2;
	//	branchToDelete = branch1.second;
	//}
	//
	//for (Behaviour* action : branchToDelete)
	//{
	//	delete action;
	//}
	//branchToDelete.clear();
	//
	//return betterBranch;

	if (branch1.first > branch2.first)
	{
		for (Behaviour* action : branch2.second)
		{
			delete action;
		}
		branch2.second.clear();
		return branch1;
	}
	else
	{
		for (Behaviour* action : branch1.second)
		{
			delete action;
		}
		branch1.second.clear();
		return branch2;
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
	branchCount++;

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
