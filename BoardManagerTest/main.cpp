#include "BoardManager.h"
#include "DealerAI.h"
#include <iostream>
#include <string>

int main()
{
	srand(2);

	Player dealer;
	Player player;

	std::vector<CreatureData*> creatures;
	std::vector<CardData*> cards;
	for (int i = 0; i < 10; i++)
	{
		int stat1 = (rand() % 5) + 1;
		int stat2 = (rand() % 5) + 1;
		CreatureData* frontCreature = new CreatureData(stat1, stat2, 2);
		creatures.push_back(frontCreature);
		CreatureData* backCreature = new CreatureData(stat2 + 2, stat1 + 2, 1);
		creatures.push_back(backCreature);

		int cost = (stat1 + stat2) / 4;
		//if (cost == 0)
		//{
		//	cost = 1;
		//}

		CardData* newCard = new CardData(cost, frontCreature, backCreature);
		cards.push_back(newCard);

		dealer.m_drawPile.push_back(newCard);
		player.m_drawPile.push_back(newCard);
	}

	BoardManager board(&dealer, &player, 4);

	// MAKING CUSTOM BOARD STATE FOR TESTING THE DEALER
	//==============================================================
	if (false)
	{
		CreatureData* creature1 = new CreatureData(1, 0, 4);
		creatures.push_back(creature1);
		CardData* card1 = new CardData(1, creature1, creature1);
		cards.push_back(card1);

		CreatureData* creature2 = new CreatureData(2, 2, 4);
		creatures.push_back(creature2);
		CardData* card2 = new CardData(1, creature2, creature2);
		cards.push_back(card2);

		dealer.m_drawPile.clear();
		dealer.m_hand.push_back(card2);

		board.PlayCard(card1, 0, dealer.m_playerIndex);
		board.PlayCard(card1, 1, dealer.m_playerIndex);
		board.PlayCard(card1, 2, dealer.m_playerIndex);
		board.PlayCard(card1, 3, dealer.m_playerIndex);
	}
	//==============================================================

	DealerAI captain(&board, & dealer);

	std::string playerInput = " ";

	//board.DisplayBoard();

	while (playerInput != "x") // X ends the game
	{
		captain.StartTurn();
		player.StartTurn();
		board.DisplayBoard();

		playerInput = "";

		while (playerInput != "x" && playerInput != "e") // E ends the turn
		{
			std::cin >> playerInput;
			int fIndex = playerInput.find("f");
			int dIndex = playerInput.find("d");
			int num1Index = playerInput.find_first_of("0123456789");
			if (fIndex != std::string::npos)
			{
				if (num1Index != std::string::npos)
				{
					int num = std::stoi(playerInput.substr(num1Index, 1)) - 1;
					player.FlipCard(num);

					board.DisplayBoard();
				}
			}
			else if (dIndex != std::string::npos)
			{
				if (num1Index != std::string::npos)
				{
					int num = std::stoi(playerInput.substr(num1Index, 1)) - 1;
					board.DestroyCard(num, player.m_playerIndex);

					board.DisplayBoard();
				}
			}
			else if (num1Index != std::string::npos)
			{
				int num2Index = playerInput.find_first_of("0123456789", num1Index + 1);
				if (num1Index != std::string::npos)
				{
					int num1 = std::stoi(playerInput.substr(num1Index, 1)) - 1;
					int num2 = std::stoi(playerInput.substr(num2Index, 1)) - 1;
					player.PlayCard(num1, num2);

					board.DisplayBoard();
				}
			}
		}

		board.DoAttackPhase();
		board.DisplayBoard();

		while (playerInput != "x" && playerInput != "n") // N for next (advance turn)
		{
			std::cin >> playerInput;
		}
	}

	for (CreatureData* creature : creatures)
	{
		delete creature;
	}

	for (CardData* card : cards)
	{
		delete card;
	}
}