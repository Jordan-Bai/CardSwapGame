#include "GDPlayer.h"

#include "BoardManager.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // For printing stuff

using namespace godot;

void GDPlayer::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("StartTurn"), &GDPlayer::StartTurn);
	ClassDB::bind_method(D_METHOD("GetHealth"), &GDPlayer::GetHealth);
	ClassDB::bind_method(D_METHOD("GetEnergy"), &GDPlayer::GetEnergy);
	ClassDB::bind_method(D_METHOD("GetCard", "cardIndex"), &GDPlayer::GetCard);
	ClassDB::bind_method(D_METHOD("GetHandSize"), &GDPlayer::GetHandSize);

	ClassDB::bind_method(D_METHOD("PlayCard", "cardIndex", "targetSlot"), &GDPlayer::PlayCard);
	ClassDB::bind_method(D_METHOD("FlipCard", "targetSlot"), &GDPlayer::FlipCard);
	ClassDB::bind_method(D_METHOD("SwapCards", "targetSlot1", "targetSlot2"), &GDPlayer::SwapCards);
	ClassDB::bind_method(D_METHOD("DestroyCard", "targetSlot"), &GDPlayer::DestroyCard);
}

GDPlayer::GDPlayer()
	: m_data(nullptr)
{
}

GDPlayer::GDPlayer(Player* data)
	: m_data(data)
{
}

GDPlayer::~GDPlayer()
{
	for (GDCard* card : m_hand)
	{
		memdelete(card);
	}
	m_hand.clear();
}


//void GDPlayer::Init(GDBoard* board)
//{
//	m_data = 
//}

void GDPlayer::StartTurn()
{
	m_data->StartTurn();
	for (int i = m_hand.size(); i < m_data->m_hand.size(); i++) // For each card that was just picked up, add it to hand
	{
		GDCard* newCard = memnew(GDCard(m_data->m_hand[i]));
		m_hand.push_back(newCard);
	}
}


int GDPlayer::GetHealth()
{
	return m_data->m_hp;
}

int GDPlayer::GetEnergy()
{
	return m_data->m_energy;
}

GDCard* GDPlayer::GetCard(int cardIndex)
{
	if (cardIndex < 0 || cardIndex >= m_hand.size())
	{
		UtilityFunctions::print("ERROR: Card not in range (GDPlayer::GetCard)");
		return nullptr;
	}
	return m_hand[cardIndex];
}

int godot::GDPlayer::GetHandSize()
{
	return m_hand.size();
}


bool GDPlayer::PlayCard(int cardIndex, int targetSlot)
{
	if (m_data->PlayCard(cardIndex, targetSlot))
	{
		// If card was played, remove it from hand
		memdelete(m_hand[cardIndex]);
		m_hand.erase(m_hand.begin() + cardIndex);
	}
	return false;
}

bool GDPlayer::FlipCard(int targetSlot)
{
	return m_data->FlipCard(targetSlot);
}

bool GDPlayer::SwapCards(int targetSlot1, int targetSlot2)
{
	return m_data->SwapCards(targetSlot1, targetSlot2);
}

bool GDPlayer::DestroyCard(int targetSlot)
{
	bool slotOccupied = (m_data->m_boardRef->GetSlot(targetSlot, m_data->m_playerIndex) != nullptr);
	m_data->m_boardRef->DestroyCard(targetSlot, m_data->m_playerIndex);
	return slotOccupied;
}
