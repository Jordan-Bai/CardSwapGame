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
	~DealerAI();

	void StartTurn();
	std::vector<Behaviour*> GetPossibleActions();
	std::pair<int, std::vector<Behaviour*>> GetBestBranch(std::vector<Behaviour*> parentSequence);
	// OPTIMIZATION
	//==============================================================
	std::vector<Behaviour*> GetDestroyActions();
	std::vector<Behaviour*> GetPlaceActions();
	std::vector<Behaviour*> GetFlipActions();
	std::vector<Behaviour*> GetSwapActions();

	std::pair<int, std::vector<Behaviour*>> CheckDestroyPhase(std::vector<Behaviour*> parentSequence);
	std::pair<int, std::vector<Behaviour*>> CheckPlacePhase(std::vector<Behaviour*> parentSequence);
	std::pair<int, std::vector<Behaviour*>> CheckFlipPhase(std::vector<Behaviour*> parentSequence);
	std::pair<int, std::vector<Behaviour*>> CheckSwapPhase(std::vector<Behaviour*> parentSequence);
	//==============================================================

	void CopyBoardData();
	void CopyPlayerData(Player* copyTarget, Player* copySource);
	float EvaluateBoard();
};