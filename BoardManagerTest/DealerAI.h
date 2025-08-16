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
		//<<<<<<< HEAD
		//std::pair<int, std::vector<Behaviour*>> GetBestBranch(std::vector<Behaviour*> parentSequence);
	// OPTIMIZATION V1
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
		//=======
	std::pair<float, std::vector<Behaviour*>> GetBestBranch(std::vector<Behaviour*> parentSequence);
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
	std::pair<float, std::vector<Behaviour*>> CheckSwapPhaseV2(std::vector<Behaviour*> parentSequence);
	//==============================================================

	std::pair<float, std::vector<Behaviour*>> BestBranch(std::pair<float, std::vector<Behaviour*>> branch1, std::pair<float, std::vector<Behaviour*>> branch2);
		//>>>>>>> origin/Optimization

	void CopyBoardData();
	void CopyPlayerData(Player* copyTarget, Player* copySource);
	float EvaluateBoard();
};