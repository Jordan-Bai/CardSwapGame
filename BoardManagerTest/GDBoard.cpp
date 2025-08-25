#include "GDBoard.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDBoard::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("DoPlayerTurn"), &GDBoard::DoPlayerTurn);
	ClassDB::bind_method(D_METHOD("DoDealerTurn"), &GDBoard::DoDealerTurn);
	ClassDB::bind_method(D_METHOD("DoAttackPhase"), &GDBoard::DoAttackPhase);
	ClassDB::bind_method(D_METHOD("IsOccupied", "slot", "side"), &GDBoard::IsOccupied);
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
	m_testPlayerAI->StartTurn();
}

void GDBoard::DoDealerTurn()
{
	m_dealerAI->StartTurn();
}

void GDBoard::DoAttackPhase()
{
	m_board->DoAttackPhase();
}

bool godot::GDBoard::IsOccupied(int slot, int side)
{
	return m_board->GetSlot(slot, side) != nullptr;
}
