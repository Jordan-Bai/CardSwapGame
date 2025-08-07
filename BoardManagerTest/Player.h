#pragma once

#include "Card.h"
#include <vector>

class BoardManager;

class Player
{
public:
	int m_hp = 10;
	int m_energy;
	int m_playerIndex;
	BoardManager* m_boardRef;

	std::vector<CardData*> m_drawPile;
	std::vector<CardData*> m_discardPile;
	std::vector<CardData*> m_hand;

	void StartTurn();

	bool PlayCard(int cardIndex, int targetSlot);
};