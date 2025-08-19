#include "BoardManager.h"
#include "DealerAI.h"
#include <iostream>
#include <string>

int main()
{
	//srand(2);
	srand(time(0));

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

		//dealer.m_drawPile.push_back(newCard);
		//player.m_drawPile.push_back(newCard);
	}

	dealer.StartMatch(cards);
	player.StartMatch(cards);

	BoardManager board(&dealer, &player, 4);
	DealerAI captain(&board, &dealer);

	// MAKING CUSTOM BOARD STATE FOR TESTING THE DEALER
	//==============================================================
	if (false)
	{
		CreatureData* creature1 = new CreatureData(1, 5, 4);
		creatures.push_back(creature1);
		CardData* card1 = new CardData(1, creature1, creature1);
		cards.push_back(card1);

		CreatureData* creature2 = new CreatureData(5, 1, 4);
		creatures.push_back(creature2);
		CardData* card2 = new CardData(1, creature2, creature2);
		cards.push_back(card2);

		CreatureData* creature3 = new CreatureData(1, 1, 4);
		creatures.push_back(creature3);
		CardData* card3 = new CardData(1, creature3, creature3);
		cards.push_back(card3);

		dealer.m_drawPile.clear();
		dealer.m_hand.push_back(card2);

		//board.PlayCard(card1, 0, dealer.m_playerIndex);
		board.PlayCard(card1, 1, dealer.m_playerIndex);
		board.PlayCard(card2, 2, dealer.m_playerIndex);
		//board.PlayCard(card1, 3, dealer.m_playerIndex);
		board.PlayCard(card3, 1, player.m_playerIndex);
	}
	//==============================================================

	// FOR TESTING MEMORY LEAK
	//==============================================================
	if (false)
	{
		dealer.StartTurn();
		for (int i = 0; i < 50; i++)
		{
			//captain.CheckPlacePhase(std::vector<Behaviour*>(), 3);
			//captain.CheckFlipPhase(std::vector<Behaviour*>(), 0);
			captain.StartTurn();
			//captain.CopyBoardData();

			//board.PlayCard(cards[0], 1, 1);
			//board.DestroyCard(1, 1);
		}
		//board.DisplayBoard();

		std::cout << "B: " << behaviours << '\n';
	}
	//==============================================================

	std::string playerInput = " ";

	while (playerInput != "x" && !board.ShouldGameEnd()) // X ends the game
	{
		captain.StartTurn();
		player.StartTurn();
		board.DisplayBoard();

		playerInput = "";

		// PLAYER CONTROLS
		while (playerInput != "x" && playerInput != "e") // E ends the turn
		{
			std::cin >> playerInput;

			int num1Index = playerInput.find_first_of("0123456789");
			int num2Index = std::string::npos;
			if (num1Index != std::string::npos)
			{
				num2Index = playerInput.find_first_of("0123456789", num1Index + 1);
			}


			// Flip card
			int fIndex = playerInput.find("f");
			if (fIndex != std::string::npos)
			{
				if (num1Index != std::string::npos)
				{
					// Get slot to flip
					int num = std::stoi(playerInput.substr(num1Index, 1)) - 1;
					player.FlipCard(num);

					board.DisplayBoard();
				}
				continue;
			}

			// Destroy card
			int dIndex = playerInput.find("d");
			if (dIndex != std::string::npos)
			{
				if (num1Index != std::string::npos)
				{
					// Get slot to destroy
					int num = std::stoi(playerInput.substr(num1Index, 1)) - 1;
					board.DestroyCard(num, player.m_playerIndex);

					board.DisplayBoard();
				}
				continue;
			}

			// Swap cards
			int sIndex = playerInput.find("s");
			if (sIndex != std::string::npos)
			{
				if (num1Index != std::string::npos && num2Index != std::string::npos)
				{
					// Get first slot to swap
					int num1 = std::stoi(playerInput.substr(num1Index, 1)) - 1;
					int num2 = std::stoi(playerInput.substr(num2Index, 1)) - 1;
					player.SwapCards(num1, num2);

					board.DisplayBoard();
				}
				continue;
			}

			// Play card
			if (num1Index != std::string::npos && num2Index != std::string::npos)
			{
				// Get slot to destroy
				int num1 = std::stoi(playerInput.substr(num1Index, 1)) - 1;
				int num2 = std::stoi(playerInput.substr(num2Index, 1)) - 1;
				player.PlayCard(num1, num2);

				board.DisplayBoard();
			}
		}

		board.DoAttackPhase();
		board.DisplayBoard();

		//while (playerInput != "x" && playerInput != "n") // N for next (advance turn)
		//{
		//	std::cin >> playerInput;
		//}
	}

	if (player.m_hp <= 0)
	{
		std::cout << "GAME LOST";
	}
	else if (dealer.m_hp <= 0)
	{
		std::cout << "GAME WON";
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