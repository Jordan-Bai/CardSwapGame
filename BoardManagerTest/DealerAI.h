#pragma once

#include "BoardManager.h"
//class BoardManager;

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

	void CopyBoardData();
	void CopyPlayerData(Player* copyTarget, Player* copySource);
	float EvaluateBoard();
};