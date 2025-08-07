#include "BoardManager.h"
#include <iostream>
#include <string>

int main()
{
	Player dealer;
	Player player;

	//CreatureData basicCreature(1, 1);
	//CreatureData basicCreature2(2, 2);
	//CardData basicCard(1, &basicCreature);
	//CardData basicCard2(2, &basicCreature);
	//for (int i = 0; i < 5; i++)
	//{
	//	dealer.m_drawPile.push_back(&basicCard);
	//	player.m_drawPile.push_back(&basicCard);
	//}

	//std::vector<CreatureData> creatures;
	//std::vector<CardData> cards;
	for (int i = 0; i < 5; i++)
	{
		CreatureData frontCreature(i, 1);
		//creatures.push_back(frontCreature);

		CardData newCard(1, &frontCreature);
		//cards.push_back(newCard);

		dealer.m_drawPile.push_back(&newCard);
		player.m_drawPile.push_back(&newCard);
	}

	BoardManager board(&dealer, &player, 4);

	std::string playerInput = " ";

	while (playerInput != "x") // X ends the game
	{
		dealer.StartTurn();
		player.StartTurn();
		board.DisplayBoard();

		playerInput = "";

		while (playerInput != "x" && playerInput != "e") // E ends the turn
		{
			std::cin >> playerInput;
			int num1Index = playerInput.find_first_of("0123456789");
			if (num1Index != std::string::npos)
			{
				int num2Index = playerInput.find_first_of("0123456789", num1Index + 1);
				if (num1Index != std::string::npos)
				{
					int num1 = std::stoi(playerInput.substr(num1Index, 1));
					int num2 = std::stoi(playerInput.substr(num2Index, 1));
					player.PlayCard(num1, num2);

					board.DisplayBoard();
				}
			}
		}

		board.DoAttackPhase();
	}
}