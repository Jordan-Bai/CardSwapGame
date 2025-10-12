#include "BoardManager.h"
#include "DealerAI.h"
#include "Ability.h"
#include <iostream>
#include <string>
#include <fstream>

//int cardDataInstances = 0;

int main()
{
	int seed = time(0);
	//seed = 1759710567;
	srand(seed);

	bool doOutput = true;
	bool doAppend = false;

	std::fstream testOutput;
	if (doOutput)
	{
		// Read what's already in the file
		testOutput.open("testOutput.txt", std::fstream::in); // Open in input mode
		std::string txt;
		std::getline(testOutput, txt);
		testOutput.close();
		// Start outputing stuff to file
		std::ios_base::openmode fileMode = std::ofstream::out;
		if (doAppend)
		{
			fileMode = fileMode | std::ofstream::app;
		}
		testOutput.open("testOutput.txt", fileMode); // Open in output/ append mode
		if (txt.length() > 1)
		{
			testOutput << "\n";
		}
		testOutput << "Seed: " << seed << '\n';
		testOutput.close();
	}


	Player dealer;
	Player player;

	// Abilities
	ChangeStats testEffect(8, 0, 0);
	Ability testAbility(AbilityTrigger::OnPlayed, &testEffect);

	Flip flipEffect;
	Ability flipAbility(AbilityTrigger::OnActivate, &flipEffect);

	Heal healEffect(2);
	Ability healAbility(AbilityTrigger::OnCardDies, &healEffect);

	GainEnergy energyEffect(2);
	Ability energyAbility(AbilityTrigger::OnFlippedTo, &energyEffect);

	RandomizeStats randomizeEffect(1, 3);
	Ability randomizeAbility(AbilityTrigger::OnPlayed, &randomizeEffect);

	CopyCards copyEffect({-1, 1});
	Ability copyAbility(AbilityTrigger::OnBoardUpdates, &copyEffect);

	Ability stackMaxedAbility(AbilityTrigger::OnStackMaxed, &flipEffect);

	BuffPerFamilyCard buffPerEffect(Family::Bird, 1, 1, 0);
	Ability buffBirdAbility(AbilityTrigger::OnBoardUpdates, &buffPerEffect);

	BuffEachFamilyCard buffEachEffect(Family::Bird, 1, 1, 0);
	Ability buffOtherBirdAbility(AbilityTrigger::OnBoardUpdates, &buffEachEffect);

	// DECK CREATION
	std::vector<CreatureData*> creatures;
	std::vector<CardData*> cards;
	for (int i = 0; i < 4; i++)
	{
		int stat1 = (rand() % 5) + 1;
		int stat2 = (rand() % 5) + 1;
		CreatureData* frontCreature = new CreatureData(stat1, stat2, 2);
		creatures.push_back(frontCreature);
		CreatureData* backCreature = new CreatureData(stat2 + 2, stat1 + 2, 1);
		creatures.push_back(backCreature);

		Family family1 = (Family)(rand() % Family::ERROR);
		Family family2 = (Family)(rand() % Family::ERROR);
		frontCreature->family = family1;
		backCreature->family = family2;

		int cost = (stat1 + stat2) / 4;
		//if (cost == 0)
		//{
		//	cost = 1;
		//}

		CardData* newCard = new CardData(cost, frontCreature, backCreature);
		cards.push_back(newCard);
	}

	CreatureData* duck = new CreatureData(2, 1, 2, 1);
	duck->abilities.push_back(&stackMaxedAbility);
	duck->stackOptions.canStack = true;
	duck->stackOptions.stackLimit = 3;
	duck->family = Family::Bird;
	creatures.push_back(duck);
	CreatureData* duckTower = new CreatureData(9, 5, 0, 1);
	duckTower->family = Family::Bird;
	creatures.push_back(duckTower);
	CardData* duckCard = new CardData(1, duck, duckTower);
	cards.push_back(duckCard);

	CardData* copyDuck1 = new CardData(duckCard);
	cards.push_back(copyDuck1);
	CardData* copyDuck2 = new CardData(duckCard);
	cards.push_back(copyDuck2);

	CreatureData* frontCreature = new CreatureData(1, 1, 2, 1);
	frontCreature->abilities.push_back(&buffBirdAbility);
	frontCreature->family = Family::Bird;
	creatures.push_back(frontCreature);
	CardData* newCard = new CardData(1, frontCreature, nullptr);
	cards.push_back(newCard);

	CreatureData* frontCreature2 = new CreatureData(2, 2, 2, 1);
	frontCreature2->abilities.push_back(&buffOtherBirdAbility);
	frontCreature2->family = Family::Bird;
	creatures.push_back(frontCreature2);
	CardData* newCard2 = new CardData(1, frontCreature2, nullptr);
	cards.push_back(newCard2);

	//CreatureData* frontCreature3 = new CreatureData(7, 7, 1, 1);
	//CreatureData* backCreature = new CreatureData(0, 2, 2, 1);
	//creatures.push_back(frontCreature3);
	//creatures.push_back(backCreature);
	//CardData* newCard3 = new CardData(1, frontCreature3, backCreature);
	//cards.push_back(newCard3);

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
		for (int i = 0; i < 10; i++)
		{
			dealer.StartTurn();
			captain.DoActions();
			//captain.CopyBoardData();

			//board.PlayCard(cards[0], 1, 1);
			//board.DestroyCard(1, 1);
		}
		//board.DisplayBoard();

		std::cout << "B: " << behaviours << '\n';
		std::cout << "A: " << activeCards << '\n';
	}
	//==============================================================

	std::string playerInput = " ";
	int turn = 0;

	board.ClearBoard();
	board.EndMatch();
	dealer.StartMatch(cards);
	player.StartMatch(cards);
	board.StartMatch();

	// TEST SEQUENCE OF ACTIONS (for checking dealer simulations)
	if (false)
	{
		std::vector<std::vector<Behaviour*>> dealerActions;
		std::vector<std::vector<Behaviour*>> playerActions;

		// TURN 1
		dealerActions.push_back(std::vector<Behaviour*>({ new PlayCard(2, 2), new PlayCard(3, 3) }));
		playerActions.push_back(std::vector<Behaviour*>({ new PlayCard(0, 2), new PlayCard(1, 3) }));

		// TURN 2
		//dealerActions.push_back(std::vector<Behaviour*>({ new PlayCard(0, 0), new PlayCard(0, 1) }));
		//playerActions.push_back(std::vector<Behaviour*>({ new PlayCard(1, 0), new PlayCard(1, 1), new PlayCard(1, 2) }));

		// TURN 3
		//dealerActions.push_back(std::vector<Behaviour*>({ new PlayCard(0, 0), new PlayCard(3, 1), new PlayCard(1, 2) }));
		//playerActions.push_back(std::vector<Behaviour*>({ new PlayCard(2, 3) }));

		for (int i = 0; i < dealerActions.size(); i++)
		{
			dealer.StartTurn();
			//board.DisplayBoard();
			for (Behaviour* action : dealerActions[i])
			{
				action->DoAction(&dealer);
			}
			dealer.EndTurn();

			player.StartTurn();
			for (Behaviour* action : playerActions[i])
			{
				action->DoAction(&player);
			}
			player.EndTurn();
			board.DisplayBoard();

			board.DoAttackPhase();
		}

		// Do turn we want to test
		std::vector<Behaviour*> testActions = { new PlayCard(0, 0), new PlayCard(0, 1) };
		dealer.StartTurn();
		board.DisplayBoard();
		std::cout << "SIMULATION:\n//==============================================================\n";
		captain.TestSimulation(testActions);
		std::cout << "//==============================================================\n";

		for (int i = 0; i < dealerActions.size(); i++)
		{
			for (Behaviour* action : dealerActions[i])
			{
				delete action;
			}
			for (Behaviour* action : playerActions[i])
			{
				delete action;
			}
		}

		for (Behaviour* action : testActions)
		{
			delete action;
		}
	}

	while (playerInput != "x" && !board.ShouldGameEnd()) // X ends the game
	{
		turn++;
		if (doOutput)
		{
			testOutput.open("testOutput.txt", std::ofstream::out | std::ofstream::app); // Open in output/ append mode
			if (turn != 1)
			{
				testOutput << ", ";
			}
			testOutput << turn;
			testOutput.close();
		}

		dealer.StartTurn();
		// MANUAL DEALER ACTIONS FOR TESTING
		//==============================================================
		if (false)
		{
			std::vector<Behaviour*> dealerActions = captain.GetActions();

			for (int i = 0; i < dealerActions.size(); i++)
			{
				if (i != 0)
				{
					std::cin >> playerInput;
				}

				dealerActions[i]->DoAction(&dealer);
				board.DisplayBoard();
			}

			for (Behaviour* action : dealerActions)
			{
				delete action;
			}

			dealer.EndTurn();
		}
		else
		{
			captain.DoActions();
		}
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

			// Debug commands
			int commandIndex = playerInput.find("/");
			if (commandIndex != std::string::npos && num1Index != std::string::npos)
			{
				int numEnd = playerInput.find_first_not_of("0123456789", num1Index);
				if (numEnd == std::string::npos)
				{
					numEnd = playerInput.length();
				}
				std::string numStr = playerInput.substr(num1Index, numEnd - num1Index);
				int num = std::stoi(numStr);

				// Gain energy
				int eIndex = playerInput.find("e");
				if (eIndex != std::string::npos)
				{
					if (num1Index != std::string::npos)
					{
						// Get energy to add
						player.m_energy += num;
					}
				}

				// Gain health
				int hIndex = playerInput.find("h");
				if (hIndex != std::string::npos)
				{
					if (num1Index != std::string::npos)
					{
						// Get health to add
						player.m_hp += num;
					}
				}

				// Draw cards
				int dIndex = playerInput.find("d");
				if (dIndex != std::string::npos)
				{
					if (num1Index != std::string::npos)
					{
						// Get cards to draw
						for (int i = 0; i < num; i++)
						{
							player.DrawCard();
						}
					}
				}

				board.DisplayBoard();
				continue;
			}

			// Inspect card
			int iIndex = playerInput.find("i");
			if (iIndex != std::string::npos)
			{
				if (num1Index != std::string::npos)
				{
					// Get slot to inspect
					int num1 = std::stoi(playerInput.substr(num1Index, 1)) - 1;

					std::vector<std::string> cardText;
					CreatureData* creature;

					// if a is also typed, inspect an active card
					int aIndex = playerInput.find("a");
					if (aIndex != std::string::npos)
					{
						if (num2Index == std::string::npos)
						{
							std::cout << "No side specified: Side 1 is the dealer, side 2 is the player\n";
							continue;
						}
						int num2 = std::stoi(playerInput.substr(num2Index, 1));
						ActiveCard* activeCard = board.GetSlot(num1, num2);
						if (activeCard == nullptr)
						{
							std::cout << "No card at that slot\n";
							continue;
						}

						cardText = board.GetCardText(activeCard);
						creature = activeCard->GetCurrentFace()->GetData();
					}
					else
					{
						if (num1 < player.m_hand.size())
						{
							CardData* data = player.m_hand[num1];
							cardText = board.GetCardText(data);
							creature = data->frontCreature;
						}
						else
						{
							std::cout << "No card at that index\n";
							continue;
						}
					}

					for (std::string cardLine : cardText)
					{
						std::cout << cardLine << '\n';
					}
					for (int i = 0; i < creature->abilities.size(); i++)
					{
						std::cout << "[Ability " << i + 1 << "] " << creature->abilities[i]->description << '\n';
					}

					board.DisplayBoard();
				}
				continue;
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

			// Activate card ability
			int aIndex = playerInput.find("a");
			if (aIndex != std::string::npos)
			{
				if (num1Index != std::string::npos)
				{
					// Get first slot to swap
					int num1 = std::stoi(playerInput.substr(num1Index, 1)) - 1;
					player.ActivateCard(num1);

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

		//board.TurnEnds(player.m_playerIndex);
		player.EndTurn();

		// MANUAL ATTACKS FOR TESTING
		//==============================================================
		if (false)
		{
			// Do player attacks
			for (int i = 0; i < 4; i++)
			{
				if (board.GetSlot(i, 2) == nullptr)
				{
					continue;
				}

				if (i != 0)
				{
					std::cin >> playerInput;
				}

				board.ManualAttack(i, 2);
				board.DisplayBoard();
			}
			// Do delear attacks
			for (int i = 0; i < 4; i++)
			{
				if (board.GetSlot(i, 1) == nullptr)
				{
					continue;
				}

				std::cin >> playerInput;

				board.ManualAttack(i, 1);
				board.DisplayBoard();
			}
		}
		//==============================================================
		else
		{
			board.DoAttackPhase();
			board.DisplayBoard();
		}
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

	if (doOutput)
	{
		testOutput.open("testOutput.txt", std::ofstream::out | std::ofstream::app); // Open in output/ append mode
		testOutput << " - Exited Normally\n";
		testOutput.close();
	}
}