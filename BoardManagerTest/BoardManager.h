#pragma once
#include "Player.h"

class BoardManager
{
	int m_slots;
	ActiveCard** m_side1;
	ActiveCard** m_side2;

	Player* m_player1;
	Player* m_player2;

	bool m_gameShouldEnd = false;

public:
	BoardManager(Player* dealer, Player* player, int slotsPerSide);
	// Rule of 3
	BoardManager(const BoardManager& ref) = delete; //copy constructor
	BoardManager& operator=(const BoardManager& ref) = delete; //assignment operator
	~BoardManager();

	bool PlayCard(CardData* data, int slot, int playerIndex);
	bool FlipCard(int slot, int playerIndex);
	bool FlipCard(ActiveCard* card);

	void DoAttackPhase();
	void PerformAttack(ActiveCard* attacker, int targetSlot);
	void DestroyCard(ActiveCard* card);
	void DestroyCard(int slot, int side);
	void EndMatch();

	void DisplayBoard();

	int OppositeSide(int side);
	Player* GetPlayer(int side);
	ActiveCard* GetSlot(int slot, int side);
	void SetSlot(int slot, int side, ActiveCard* newCard);

	int GetSlotCount();
	bool ShouldGameEnd();
};