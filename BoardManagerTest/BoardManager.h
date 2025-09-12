#pragma once
#include "Player.h"
#include "ActiveCard.h"

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
	bool SwapCards(int slot1, int slot2, int playerIndex);
	bool ActivateCard(int slot, int playerIndex);
	bool ActivateCard(ActiveCard* card);

	void DoAttackPhase();
	void PerformAttack(ActiveCard* attacker, int targetSlot);
	void DestroyCard(ActiveCard* card);
	void DestroyCard(int slot, int side);
	void EndMatch();

	void DisplayBoard();
	std::vector<std::string> GetCardText(CardData* card);
	std::vector<std::string> GetCardText(ActiveCard* card);
	std::string AbilityToString(CreatureData* face, int abilityIndex);
	std::string FamilyToString(Family family);

	int OppositeSide(int side);
	Player* GetPlayer(int side);
	ActiveCard* GetSlot(int slot, int side);
	void SetSlot(int slot, int side, ActiveCard* newCard);

	int GetSlotCount();
	bool ShouldGameEnd();


	// Events
	void CardPlayed(ActiveCard* newCard);
	void CardDies();
	void TurnStarts(int playerIndex);
	void TurnEnds(int playerIndex);
	void BoardUpdates();

	std::function<void(ActiveCard* cardPlayed)> OnCardPlayed;
	std::function<void()> OnCardDies;
	std::function<void()> OnDealerTurnStarts;
	std::function<void()> OnDealerTurnEnds;
	std::function<void()> OnPlayerTurnStarts;
	std::function<void()> OnPlayerTurnEnds;
	std::function<void()> OnBoardUpdates;
};