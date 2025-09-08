#include "BoardManager.h"

#include "Ability.h"

#include <iostream>
#include <string>

BoardManager::BoardManager(Player* dealer, Player* player, int slotsPerSide)
	:m_player1(dealer), m_player2(player), m_slots(slotsPerSide)
{
	m_player1->m_playerIndex = 1;
	m_player1->m_boardRef = this;
	m_player2->m_playerIndex = 2;
	m_player2->m_boardRef = this;

	m_side1 = new ActiveCard*[m_slots];
	m_side2 = new ActiveCard*[m_slots];

	for (int i = 0; i < m_slots; i++)
	{
		m_side1[i] = nullptr;
		m_side2[i] = nullptr;
	}
}

BoardManager::~BoardManager()
{
	for (int i = 0; i < m_slots; i++)
	{
		if (m_side1[i] != nullptr)
		{
			delete m_side1[i];
		}
		if (m_side2[i] != nullptr)
		{
			delete m_side2[i];
		}
	}

	delete[] m_side1;
	delete[] m_side2;
}


bool BoardManager::PlayCard(CardData* data, int slot, int playerIndex)
{
	ActiveCard* placedCard;

	if (playerIndex == 1)
	{
		if (m_side1[slot] != nullptr)
		{
			return false;
		}

		placedCard = new ActiveCard(data, slot, playerIndex, this);
		m_side1[slot] = placedCard;
	}
	else
	{
		if (m_side2[slot] != nullptr)
		{
			return false;
		}

		placedCard = new ActiveCard(data, slot, playerIndex, this);
		m_side2[slot] = placedCard;
	}

	//if (placedCard->GetCurrentFace()->OnPlayed != nullptr)
	//{
	//	placedCard->GetCurrentFace()->OnPlayed();
	//}
	placedCard->OnPlayed();

	CardPlayed(placedCard);

	return true;
}

bool BoardManager::FlipCard(int slot, int playerIndex)
{
	ActiveCard* targetSlot = GetSlot(slot, playerIndex);
	return FlipCard(targetSlot);
}

bool BoardManager::FlipCard(ActiveCard* card)
{
	if (card != nullptr)
	{
		if (card->CanFlip())
		{
			card->Flip();
			card->OnFlip();

			if (card->GetHP() <= 0)
			{
				DestroyCard(card);
			}

			// Send "OnBoardUpdates" signal to all cards
			BoardUpdates();

			return true;
		}
	}

	return false;
}

bool BoardManager::ActivateCard(int slot, int playerIndex)
{
	ActiveCard* targetSlot = GetSlot(slot, playerIndex);
	return ActivateCard(targetSlot);
}

bool BoardManager::ActivateCard(ActiveCard* card)
{
	if (card != nullptr)
	{
		if (card->GetCurrentFace()->HasActivateAbility())
		{
			card->OnActivate();
			// Send "OnBoardUpdates" signal to all cards
			BoardUpdates();

			return true;
		}
	}

	return false;
}

void BoardManager::DoAttackPhase()
{
	// Player's cards attack first
	for (int i = 0; i < m_slots; i++)
	{
		if (m_side2[i] != nullptr)
		{
			std::vector<int> cardTargets = m_side2[i]->GetTargets();
			for (int slot : cardTargets)
			{
				PerformAttack(m_side2[i], slot);
			}
		}
	}

	if (m_player1->m_hp <= 0) // If the dealer is dead
	{
		EndMatch();
		return;
	}

	// Dealer's cards attack second
	for (int i = 0; i < m_slots; i++)
	{
		if (m_side1[i] != nullptr)
		{
			std::vector<int> cardTargets = m_side1[i]->GetTargets();
			for (int slot : cardTargets)
			{
				PerformAttack(m_side1[i], slot);
			}
		}
	}

	if (m_player2->m_hp <= 0) // If the player is dead
	{
		EndMatch();
	}
}

void BoardManager::PerformAttack(ActiveCard* attacker, int targetSlot)
{
	if (targetSlot < 0 || targetSlot >= m_slots)
	{
		return;
	}

	int defendingSide = OppositeSide(attacker->m_side);
	ActiveCard* defendingCard = GetSlot(targetSlot, defendingSide);
	Player* defendingPlayer = GetPlayer(defendingSide);

	// Send "OnAttack" signal to attacker
	//if (attacker->GetCurrentFace()->OnAttack != nullptr)
	//{
	//	attacker->GetCurrentFace()->OnAttack(defendingCard);
	//}
	attacker->OnAttack(defendingCard);

	if (defendingCard == nullptr)
	{
		// Attack opponent directly
		defendingPlayer->m_hp -= attacker->GetAtk();
	}
	else
	{
		defendingCard->TakeDamage(attacker->GetAtk());
		// Send "OnAttacked" signal to defender
		//if (defendingCard->GetCurrentFace()->OnAttacked != nullptr)
		//{
		//	defendingCard->GetCurrentFace()->OnAttacked(attacker);
		//}

		// Check if card died
		if (defendingCard->GetHP() <= 0)
		{
			// Do overkill damage 
			defendingPlayer->m_hp += defendingCard->GetHP();

			DestroyCard(defendingCard);
		}
		else
		{
			// Send "OnAttacked" signal to defender
			defendingCard->OnAttacked(attacker);
		}

		// Send "OnBoardUpdates" signal to all cards
		BoardUpdates();
	}
}

void BoardManager::DestroyCard(ActiveCard* card)
{
	// Send "OnDeath" signal to card before it gets destroyed
	//if (card->GetCurrentFace()->OnDeath != nullptr)
	//{
	//	card->GetCurrentFace()->OnDeath();
	//}
	card->OnDeath();

	// Add card to corresponding discard pile
	GetPlayer(card->m_side)->m_discardPile.push_back(card->GetData());

	// Remove card from slot
	SetSlot(card->m_slot, card->m_side, nullptr);

	// Destroy card
	delete card;

	// Send "OnCardDies" signal to all other cards
	CardDies();
}

void BoardManager::DestroyCard(int slot, int side)
{
	ActiveCard* cardToDestroy = GetSlot(slot, side);
	
	if (cardToDestroy != nullptr)
	{
		DestroyCard(cardToDestroy);
	}
}

void BoardManager::EndMatch()
{
	m_player1->EndMatch();
	m_player2->EndMatch();
	m_gameShouldEnd = true;
}


void BoardManager::DisplayBoard()
{
	// Print dealer health
	//==============================================================
	std::cout << "\nHP: " << std::to_string(m_player1->m_hp) << '\n';

	// Print dealer hand
	//==============================================================
	//std::string dealerHand;
	//for (CardData* card : m_player1->m_hand)
	//{
	//	dealerHand += " <---> ";
	//}
	//std::cout << dealerHand << '\n';
	std::string dealerHand1;
	std::string dealerHand2;
	std::string dealerHand3;
	for (CardData* card : m_player1->m_hand)
	{
		//CreatureData* frontCreature = card->frontCreature;
		//dealerHand1 += " <" + std::to_string(card->cost) + "--> ";
		//dealerHand2 += " <" + std::to_string(frontCreature->atk) + "-" + std::to_string(frontCreature->hp) + "> ";
		std::vector<std::string> cardText = GetCardText(card);
		dealerHand1 += cardText[0];
		dealerHand2 += cardText[1];
		dealerHand3 += cardText[2];
	}
	std::cout << dealerHand1 << '\n' << dealerHand2 << '\n' << dealerHand3 << '\n';

	// Print dealer side
	//==============================================================
	std::cout << "================================\n";

	std::string dealerSide1;
	std::string dealerSide2;
	std::string dealerSide3;
	for (int i = 0; i < m_slots; i++)
	{
		//dealerSide1 += " |";
		//dealerSide2 += " |";
		//if (m_side1[i] == nullptr)
		//{
		//	dealerSide1 += "---";
		//	dealerSide2 += "---";
		//}
		//else
		//{
		//	dealerSide1 += "--" + std::to_string(m_side1[i]->GetFlipCost());
		//	dealerSide2 += std::to_string(m_side1[i]->GetAtk()) + "-" + std::to_string(m_side1[i]->GetHP());
		//}
		//dealerSide1 += "| ";
		//dealerSide2 += "| ";
		std::vector<std::string> cardText = GetCardText(m_side1[i]);
		dealerSide1 += cardText[0];
		dealerSide2 += cardText[1];
		dealerSide3 += cardText[2];
	}
	std::cout << dealerSide1 << '\n' << dealerSide2 << '\n' << dealerSide3 << "\n\n";


	// Print player side
	//==============================================================
	std::string playerSide1;
	std::string playerSide2;
	std::string playerSide3;
	for (int i = 0; i < m_slots; i++)
	{
		//playerSide1 += " |";
		//playerSide2 += " |";
		//if (m_side2[i] == nullptr)
		//{
		//	playerSide1 += "---";
		//	playerSide2 += "---";
		//}
		//else
		//{
		//	playerSide1 += "--" + std::to_string(m_side2[i]->GetFlipCost());
		//	playerSide2 += std::to_string(m_side2[i]->GetAtk()) + "-" + std::to_string(m_side2[i]->GetHP());
		//}
		//playerSide1 += "| ";
		//playerSide2 += "| ";
		std::vector<std::string> cardText = GetCardText(m_side2[i]);
		playerSide1 += cardText[0];
		playerSide2 += cardText[1];
		playerSide3 += cardText[2];
	}
	std::cout << playerSide1 << '\n' << playerSide2 << '\n' << playerSide3 << '\n';

	std::cout << "================================\n";


	// Print player hand
	//==============================================================
	std::string playerHand1;
	std::string playerHand2;
	std::string playerHand3;
	for (CardData* card : m_player2->m_hand)
	{
		//CreatureData* frontCreature = card->frontCreature;
		//playerHand1 += " <" + std::to_string(card->cost) + "--> ";
		//playerHand2 += " <" + std::to_string(frontCreature->atk) + "-" + std::to_string(frontCreature->hp) + "> ";
		std::vector<std::string> cardText = GetCardText(card);
		playerHand1 += cardText[0];
		playerHand2 += cardText[1];
		playerHand3 += cardText[2];
	}
	std::cout << playerHand1 << '\n' << playerHand2 << '\n' << playerHand3 << '\n';


	// Print player stats
	//==============================================================
	std::cout << "HP: " << std::to_string(m_player2->m_hp) << "   Energy: " << std::to_string(m_player2->m_energy) << "\n\n";
}

std::vector<std::string> BoardManager::GetCardText(CardData* card)
{
	std::string cardLine1 = " |";
	std::string cardLine2 = " |";
	std::string cardLine3 = " |";

	CreatureData* frontCreature = card->frontCreature;
	cardLine1 += std::to_string(card->cost) + "---";
	// Add text for first ability
	cardLine2 += AbilityToString(frontCreature, 0);
	cardLine3 += std::to_string(frontCreature->atk) + "--" + std::to_string(frontCreature->hp);

	cardLine1 += "| ";
	cardLine2 += "| ";
	cardLine3 += "| ";

	return std::vector<std::string>{ cardLine1, cardLine2, cardLine3 };
}

std::vector<std::string> BoardManager::GetCardText(ActiveCard* card)
{
	std::string cardLine1 = " |";
	std::string cardLine2 = " |";
	std::string cardLine3 = " |";

	if (card == nullptr)
	{
		cardLine1 += "----";
		cardLine2 += "----";
		cardLine3 += "----";
	}
	else
	{
		CreatureData* faceData = card->GetCurrentFace()->GetData();
		cardLine1 += "---" + std::to_string(card->GetFlipCost());
		// Add text for first ability
		cardLine2 += AbilityToString(faceData, 0) + "";
		cardLine3 += std::to_string(card->GetAtk()) + "--" + std::to_string(card->GetHP());
	}

	cardLine1 += "| ";
	cardLine2 += "| ";
	cardLine3 += "| ";

	return std::vector<std::string>{ cardLine1, cardLine2, cardLine3 };
}

std::string BoardManager::AbilityToString(CreatureData* face, int abilityIndex)
{
	if (face->abilities.size() <= abilityIndex)
	{
		return "----";
	}

	Ability* ability = face->abilities[abilityIndex];
	if (ability == nullptr)
	{
		return "----";
	}

	std::string abilityStr;

	// Add character for trigger
	switch (ability->trigger)
	{
	case AbilityTrigger::OnPlayed:
		abilityStr += "Pl";
		break;
	case AbilityTrigger::OnDeath:
		abilityStr += "De";
		break;
	case AbilityTrigger::OnAttack:
		abilityStr += "At";
		break;
	case AbilityTrigger::OnAttacked:
		abilityStr += "Da";
		break;
	case AbilityTrigger::OnFlippedTo:
		abilityStr += "FT";
		break;
	case AbilityTrigger::OnActivate:
		abilityStr += "-" + std::to_string(face->aCost);
		break;
	case AbilityTrigger::OnTurnStarts:
		abilityStr += "TS";
		break;
	case AbilityTrigger::OnTurnEnds:
		abilityStr += "TE";
		break;
	case AbilityTrigger::OnCardDies:
		abilityStr += "CD";
		break;
	case AbilityTrigger::OnBoardUpdates:
		abilityStr += "BU";
		break;
	default:
		std::cout << "ERROR: Ability has no trigger (Ability::Init)\n";
	}

	abilityStr += ">" + ability->effect->GetIcon();

	return abilityStr;
}


int BoardManager::OppositeSide(int side)
{
	if (side == 1)
	{
		return 2;
	}
	else
	{
		return 1;
	}
}

Player* BoardManager::GetPlayer(int side)
{
	if (side == 1)
	{
		return m_player1;
	}
	else
	{
		return m_player2;
	}
}

ActiveCard* BoardManager::GetSlot(int slot, int side)
{
	if (slot < 0 || slot >= m_slots)
	{
		//std::cout << "ERROR: Accessing slot out of range (BoardManager::GetSlot)\n";
		return nullptr;
	}

	if (side == 1)
	{
		return m_side1[slot];
	}
	else
	{
		return m_side2[slot];
	}
}

void BoardManager::SetSlot(int slot, int side, ActiveCard* newCard)
{
	if (slot < 0 || slot >= m_slots)
	{
		std::cout << "ERROR: Accessing slot out of range (BoardManager::SetSlot)\n";
		return;
	}

	if (side == 1)
	{
		m_side1[slot] = newCard;
	}
	else
	{
		m_side2[slot] = newCard;
	}

	if (newCard != nullptr)
	{
		newCard->m_slot = slot;
		newCard->m_side = side;
	}

	BoardUpdates();
}


int BoardManager::GetSlotCount()
{
	return m_slots;
}

bool BoardManager::ShouldGameEnd()
{
	return m_gameShouldEnd;
}


void BoardManager::CardPlayed(ActiveCard* newCard)
{
	if (OnCardPlayed != nullptr)
	{
		OnCardPlayed(newCard);
	}

	// Send "OnBoardUpdates" signal to all cards
	BoardUpdates();
}

// Events
void BoardManager::CardDies()
{
	// Send signal to all cards
	for (int side = 1; side <= 2; side++)
	{
		for (int i = 0; i < m_slots; i++)
		{
			ActiveCard* targetCard = GetSlot(i, side);
			//if (targetCard != nullptr && targetCard->GetCurrentFace()->OnCardDies != nullptr)
			//{
			//	targetCard->GetCurrentFace()->OnCardDies();
			//}
			if (targetCard != nullptr)
			{
				targetCard->OnCardDies();
			}
		}
	}
	
	// Send signal to any external system
	if (OnCardDies != nullptr)
	{
		OnCardDies();
	}

	// Send BoardUpdate signal
	BoardUpdates();
}

void BoardManager::TurnStarts(int playerIndex)
{
	// Send signal to all cards on that side
	for (int i = 0; i < m_slots; i++)
	{
		ActiveCard* targetCard = GetSlot(i, playerIndex);
		//if (targetCard != nullptr && targetCard->GetCurrentFace()->OnTurnStarts != nullptr)
		//{
		//	targetCard->GetCurrentFace()->OnTurnStarts();
		//}
		if (targetCard != nullptr)
		{
			targetCard->OnTurnStarts();
		}
	}

	// Send signal to any external system
	if (playerIndex == 1)
	{
		if (OnDealerTurnStarts != nullptr)
		{
			OnDealerTurnStarts();
		}
	}
	else
	{
		if (OnPlayerTurnStarts != nullptr)
		{
			OnPlayerTurnStarts();
		}
	}
}

void BoardManager::TurnEnds(int playerIndex)
{
	// Send signal to all cards on that side
	for (int i = 0; i < m_slots; i++)
	{
		ActiveCard* targetCard = GetSlot(i, playerIndex);
		//if (targetCard != nullptr && targetCard->GetCurrentFace()->OnTurnEnds != nullptr)
		//{
		//	targetCard->GetCurrentFace()->OnTurnEnds();
		//}
		if (targetCard != nullptr)
		{
			targetCard->OnTurnEnds();
		}
	}

	// Send signal to any external system
	if (playerIndex == 1)
	{
		if (OnDealerTurnEnds != nullptr)
		{
			OnDealerTurnEnds();
		}
	}
	else
	{
		if (OnPlayerTurnEnds != nullptr)
		{
			OnPlayerTurnEnds();
		}
	}
}

void BoardManager::BoardUpdates()
{
	// Send signal to all cards
	for (int side = 1; side <= 2; side++)
	{
		for (int i = 0; i < m_slots; i++)
		{
			ActiveCard* targetCard = GetSlot(i, side);
			//if (targetCard != nullptr && targetCard->GetCurrentFace()->OnBoardUpdates != nullptr)
			//{
			//	targetCard->GetCurrentFace()->OnBoardUpdates();
			//}
			if (targetCard != nullptr)
			{
				targetCard->OnBoardUpdates();
			}
		}
	}

	// Send signal to any external system
	if (OnBoardUpdates != nullptr)
	{
		OnBoardUpdates();
	}
}
