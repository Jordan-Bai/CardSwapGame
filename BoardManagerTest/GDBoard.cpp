#include "GDBoard.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDBoard::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("StartMatch"), &GDBoard::StartMatch);

	ClassDB::bind_method(D_METHOD("DoDealerTurn"), &GDBoard::DoDealerTurn);
	ClassDB::bind_method(D_METHOD("DoAttackPhase"), &GDBoard::DoAttackPhase);
	ClassDB::bind_method(D_METHOD("UpdateBoardState"), &GDBoard::UpdateBoardState);

	//ClassDB::bind_method(D_METHOD("PlayCard", "cardIndex", "targetSlot"), &GDBoard::PlayCard);
	//ClassDB::bind_method(D_METHOD("FlipCard", "targetSlot"), &GDBoard::FlipCard);
	//ClassDB::bind_method(D_METHOD("SwapCards", "targetSlot1", "targetSlot2"), &GDBoard::SwapCards);
	//ClassDB::bind_method(D_METHOD("DestroyCard", "targetSlot"), &GDBoard::DestroyCard);

	ClassDB::bind_method(D_METHOD("IsOccupied", "slot", "side"), &GDBoard::IsOccupied);
	ClassDB::bind_method(D_METHOD("GetCard", "slot", "side"), &GDBoard::GetCard);

	ClassDB::bind_method(D_METHOD("GetDealer"), &GDBoard::GetDealer);
	ClassDB::bind_method(D_METHOD("GetPlayer"), &GDBoard::GetPlayer);
}

GDBoard::GDBoard()
{
	m_dealerData = new Player();
	m_playerData = new Player();

	m_dealer = memnew(GDPlayer(m_dealerData));
	m_player = memnew(GDPlayer(m_playerData));

	m_board = new BoardManager(m_dealerData, m_playerData, m_slots);

	m_dealerAI = new DealerAI(m_board, m_dealerData);
	//m_testPlayerAI = new DealerAI(m_board, m_playerData);

	// Setup each side of the board
	for (int i = 0; i < m_board->GetSlotCount(); i++)
	{
		m_side1.push_back(nullptr);
		m_side2.push_back(nullptr);
	}
}

GDBoard::~GDBoard()
{
	delete m_dealerData;
	delete m_playerData;
	delete m_board;
	delete m_dealerAI;
	//delete m_testPlayerAI;

	memdelete(m_dealer);
	memdelete(m_player);
}

void GDBoard::StartMatch()
{
	m_dealer->StartMatch();
	m_player->StartMatch();
}


void GDBoard::DoDealerTurn()
{
	m_dealer->StartTurn();
	m_dealerAI->DoActions();
}

void GDBoard::DoAttackPhase()
{
	m_board->DoAttackPhase();
}

void GDBoard::UpdateBoardState()
{
	for (int i = 0; i < m_board->GetSlotCount(); i++)
	{
		GDDisplayCard* displayCard = m_side1[i];
		ActiveCard* realCard = m_board->GetSlot(i, 1);
		// If there's a display card at this slot
		if (displayCard != nullptr)
		{
			// If there's a display card but not a real card at this slot, destroy the display card
			if (realCard == nullptr)
			{
				memdelete(m_side1[i]);
				m_side1[i] = nullptr;
			}
			else // Otherwise, make sure the display card matches the real card's stats
			{
				UpdateCardStats(displayCard, realCard);
			}
		}
		else if (realCard != nullptr)
		{
			// If there's a real card but not a display card at this slot, create a display card
			GDDisplayCard* newCard = memnew(GDDisplayCard());
			m_side1[i] = newCard;
			UpdateCardStats(newCard, realCard);
		}

		// Same thing for side 2
		displayCard = m_side2[i];
		realCard = m_board->GetSlot(i, 2);
		// If there's a display card at this slot
		if (displayCard != nullptr)
		{
			// If there's a display card but not a real card at this slot, destroy the display card
			if (realCard == nullptr)
			{
				memdelete(m_side2[i]);
				m_side2[i] = nullptr;
			}
			else // Otherwise, make sure the display card matches the real card's stats
			{
				UpdateCardStats(displayCard, realCard);
			}
		}
		else if (realCard != nullptr)
		{
			// If there's a real card but not a display card at this slot, create a display card
			GDDisplayCard* newCard = memnew(GDDisplayCard());
			m_side2[i] = newCard;
			UpdateCardStats(newCard, realCard);
		}
	}
}

void GDBoard::UpdateCardStats(GDDisplayCard* displayCard, ActiveCard* realCard)
{
	displayCard->m_hp = realCard->GetHP();
	displayCard->m_atk = realCard->GetAtk();
	displayCard->m_cost = realCard->GetCost();
	displayCard->m_flipCost = realCard->GetFlipCost();
	displayCard->m_abilityCost = realCard->GetAbilityCost();
	displayCard->m_name = String(realCard->GetCurrentFace()->GetName().c_str());
}

bool GDBoard::IsOccupied(int slot, int side)
{
	return m_board->GetSlot(slot, side) != nullptr;
}

GDDisplayCard* GDBoard::GetCard(int slot, int side)
{
	if (slot < 0 || slot >= m_board->GetSlotCount())
	{
		UtilityFunctions::print("ERROR: Slot not in range (GDBoard::GetCard)");
		return nullptr;
	}

	switch (side)
	{
	case 1:
		return m_side1[slot];
	case 2:
		return m_side2[slot];
	default:
		UtilityFunctions::print("ERROR: Invalid side (GDBoard::GetCard)");
		return nullptr;
	}
}

GDPlayer* GDBoard::GetDealer()
{
	return m_dealer;
}

GDPlayer* GDBoard::GetPlayer()
{
	return m_player;
}


//bool GDBoard::PlayCard(int cardIndex, int targetSlot)
//{
//	return m_playerData->PlayCard(cardIndex, targetSlot);
//}
//
//bool GDBoard::FlipCard(int targetSlot)
//{
//	return m_playerData->FlipCard(targetSlot);
//}
//
//bool GDBoard::SwapCards(int targetSlot1, int targetSlot2)
//{
//	return m_playerData->SwapCards(targetSlot1, targetSlot2);
//}
//
//bool GDBoard::DestroyCard(int targetSlot)
//{
//	bool slotOccupied = (m_board->GetSlot(targetSlot, m_playerData->m_playerIndex) != nullptr);
//	m_board->DestroyCard(targetSlot, m_playerData->m_playerIndex);
//	return slotOccupied;
//}
