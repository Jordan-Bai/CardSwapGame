#include "GDBoard.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDBoard::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("DoPlayerTurn"), &GDBoard::DoPlayerTurn);
	ClassDB::bind_method(D_METHOD("DoDealerTurn"), &GDBoard::DoDealerTurn);
	ClassDB::bind_method(D_METHOD("DoAttackPhase"), &GDBoard::DoAttackPhase);

	ClassDB::bind_method(D_METHOD("PlayCard", "cardIndex", "targetSlot"), &GDBoard::PlayCard);
	ClassDB::bind_method(D_METHOD("FlipCard", "targetSlot"), &GDBoard::FlipCard);
	ClassDB::bind_method(D_METHOD("SwapCards", "targetSlot1", "targetSlot2"), &GDBoard::SwapCards);
	ClassDB::bind_method(D_METHOD("DestroyCard", "targetSlot"), &GDBoard::DestroyCard);

	ClassDB::bind_method(D_METHOD("IsOccupied", "slot", "side"), &GDBoard::IsOccupied);
	//ClassDB::bind_method(D_METHOD("GetCard"), &GDBoard::GetCard);
}

GDBoard::GDBoard()
//	: m_dealerData(new Player()), m_playerData(new Player())
//	m_board(new BoardManager(m_dealerData, m_playerData, m_slots))
//	m_board(m_dealerData, m_playerData, m_slots), 
//	m_dealerAI(&m_board, m_dealerData), m_testPlayerAI(&m_board, m_playerData)
{
	m_dealerData = new Player();
	m_playerData = new Player();

	m_board = new BoardManager(m_dealerData, m_playerData, m_slots);

	m_dealerAI = new DealerAI(m_board, m_dealerData);
	m_testPlayerAI = new DealerAI(m_board, m_playerData);


	// DECK CREATION FOR TESTING
	srand(time(0));

	for (int i = 0; i < 10; i++)
	{
		int stat1 = (rand() % 5) + 1;
		int stat2 = (rand() % 5) + 1;
		CreatureData* frontCreature = new CreatureData(stat1, stat2, 2);
		creatures.push_back(frontCreature);
		CreatureData* backCreature = new CreatureData(stat2 + 2, stat1 + 2, 1);
		creatures.push_back(backCreature);

		int cost = (stat1 + stat2) / 4;

		CardData* newCard = new CardData(cost, frontCreature, backCreature);
		cards.push_back(newCard);
	}

	m_dealerData->StartMatch(cards);
	m_playerData->StartMatch(cards);
}

GDBoard::~GDBoard()
{
	delete m_dealerData;
	delete m_playerData;
	delete m_board;
	delete m_dealerAI;
	delete m_testPlayerAI;
}

//int godot::GDBoard::GetNumSlots()
//{
//	return 0;
//}
//
//void godot::GDBoard::SetNumSlots(int num)
//{
//}

void GDBoard::DoPlayerTurn()
{
	//m_testPlayerAI->StartTurn();
	m_playerData->StartTurn();
}

void GDBoard::DoDealerTurn()
{
	m_dealerAI->StartTurn();
}

void GDBoard::DoAttackPhase()
{
	m_board->DoAttackPhase();
}

bool GDBoard::IsOccupied(int slot, int side)
{
	return m_board->GetSlot(slot, side) != nullptr;
}

//GDCard GDBoard::GetCard(int slot, int side)
//{
//	ActiveCard* targetCard = m_board->GetSlot(slot, side);
//	CardData* data = nullptr;
//	if (targetCard != nullptr)
//	{
//		data = targetCard->GetData();
//	}
//
//	//Ref<GDCard> cardRef;
//	//cardRef.instantiate();
//	//cardRef()
//
//	return GDCard(data);
//}


bool GDBoard::PlayCard(int cardIndex, int targetSlot)
{
	return m_playerData->PlayCard(cardIndex, targetSlot);
}

bool GDBoard::FlipCard(int targetSlot)
{
	return m_playerData->FlipCard(targetSlot);
}

bool GDBoard::SwapCards(int targetSlot1, int targetSlot2)
{
	return m_playerData->SwapCards(targetSlot1, targetSlot2);
}

bool GDBoard::DestroyCard(int targetSlot)
{
	bool slotOccupied = (m_board->GetSlot(targetSlot, m_playerData->m_playerIndex) != nullptr);
	m_board->DestroyCard(targetSlot, m_playerData->m_playerIndex);
	return slotOccupied;
}
