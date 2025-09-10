#pragma once

#include "Card.h"
//#include <vector>

class BoardManager;

const int StartingEnergy = 3;
const int MaxHandSize = 5;
const int CostToSwap = 1;

class Player
{
	int m_maxHP = 20;
public:
	int m_hp = 20;
	int m_energy;
	int m_playerIndex;
	BoardManager* m_boardRef;

	std::vector<CardData*> m_drawPile;
	std::vector<CardData*> m_discardPile;
	std::vector<CardData*> m_hand;

	Player();
	Player(const Player& ref) = delete; //copy constructor
	Player& operator=(const Player& ref) = delete; //assignment operator
	~Player();

	void StartMatch(std::vector<CardData*> deck);
	void EndMatch();
	void StartTurn();
	bool DrawCard();
	void PickupCard(CardData* card);

	bool PlayCard(int cardIndex, int targetSlot);
	bool FlipCard(int cardSlot);
	bool SwapCards(int slot1, int slot2);
	bool ActivateCard(int cardSlot);
};