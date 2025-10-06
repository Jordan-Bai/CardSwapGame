#pragma once

#include "BoardManager.h"
//class BoardManager;
#include "Behaviour.h"

class DealerAI
{
	BoardManager* m_boardRef;
	Player* m_data;

	BoardManager* m_copyBoard;
	Player* m_copyPlayer;
	Player* m_copyDealer;

public:
	DealerAI(BoardManager* board, Player* playerData);
	// Rule of 3
	DealerAI(const DealerAI& ref) = delete; //copy constructor
	DealerAI& operator=(const DealerAI& ref) = delete; //assignment operator
	~DealerAI();

	void DoActions();
	std::vector<Behaviour*> GetActions();

	// FOR TESTING: Shows process of simulating board
	void TestSimulation(std::vector<Behaviour*> actions);

	// OPTIMIZATION V2
	//==============================================================
	std::vector<Behaviour*> GetDestroyActions(int slot);
	// ^ Returning a vector to better match current framework but doesn't really need to (same w/ GetFlipActions)
	std::vector<Behaviour*> GetPlaceActions(int slot);
	std::vector<Behaviour*> GetFlipActions(int slot);
	std::vector<Behaviour*> GetSwapActions();

	std::pair<float, std::vector<Behaviour*>> CheckDestroyPhase(std::vector<Behaviour*> parentSequence, int slot);
	std::pair<float, std::vector<Behaviour*>> CheckPlacePhase(std::vector<Behaviour*> parentSequence, int slot);
	std::pair<float, std::vector<Behaviour*>> CheckFlipPhase(std::vector<Behaviour*> parentSequence, int slot);
	std::pair<float, std::vector<Behaviour*>> CheckSwapPhase(std::vector<Behaviour*> parentSequence);
	//==============================================================

	std::pair<float, std::vector<Behaviour*>> BestBranch(std::pair<float, std::vector<Behaviour*>> branch1, std::pair<float, std::vector<Behaviour*>> branch2);

	void CopyBoardData();
	void CopyPlayerData(Player* copyTarget, Player* copySource);
	void CopyCards(std::vector<CardData*>& copyTarget, std::vector<CardData*>& copySource);
	float EvaluateBoard();
};