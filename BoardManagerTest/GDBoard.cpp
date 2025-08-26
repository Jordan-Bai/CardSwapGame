#include "GDBoard.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDBoard::_bind_methods()
{
	//ClassDB::bind_method(D_METHOD("StartPlayerTurn"), &GDBoard::StartPlayerTurn);
	//ClassDB::bind_method(D_METHOD("StartDealerTurn"), &GDBoard::StartDealerTurn);
	ClassDB::bind_method(D_METHOD("DoDealerTurn"), &GDBoard::DoDealerTurn);
	ClassDB::bind_method(D_METHOD("DoAttackPhase"), &GDBoard::DoAttackPhase);

	//ClassDB::bind_method(D_METHOD("PlayCard", "cardIndex", "targetSlot"), &GDBoard::PlayCard);
	//ClassDB::bind_method(D_METHOD("FlipCard", "targetSlot"), &GDBoard::FlipCard);
	//ClassDB::bind_method(D_METHOD("SwapCards", "targetSlot1", "targetSlot2"), &GDBoard::SwapCards);
	//ClassDB::bind_method(D_METHOD("DestroyCard", "targetSlot"), &GDBoard::DestroyCard);

	ClassDB::bind_method(D_METHOD("IsOccupied", "slot", "side"), &GDBoard::IsOccupied);
	//ClassDB::bind_method(D_METHOD("GetCard"), &GDBoard::GetCard);

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

	memdelete(m_dealer);
	memdelete(m_player);
}

//int godot::GDBoard::GetNumSlots()
//{
//	return 0;
//}
//
//void godot::GDBoard::SetNumSlots(int num)
//{
//}

//void GDBoard::StartPlayerTurn()
//{
//	m_playerData->StartTurn(); // TODO: REWORK HOW TURN STARTING WORKS
//	m_player->StartTurn();
//}
//
//void GDBoard::StartDealerTurn()
//{
//	m_dealerAI->StartTurn();
//	m_dealer->StartTurn();
//}


void GDBoard::DoDealerTurn()
{
	m_dealer->StartTurn();
	m_dealerAI->StartTurn(); 
	// TODO: Make sure dealer AI doesn't run player.TurnStart() since that's already done by GDPlayer
}

void GDBoard::DoAttackPhase()
{
	m_board->DoAttackPhase();
}

bool GDBoard::IsOccupied(int slot, int side)
{
	return m_board->GetSlot(slot, side) != nullptr;
}

GDPlayer* GDBoard::GetDealer()
{
	return m_dealer;
}

GDPlayer* GDBoard::GetPlayer()
{
	return m_player;
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
