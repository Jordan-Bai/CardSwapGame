#include "Behaviour.h"

#include "Player.h"
#include "BoardManager.h"

int behaviours = 0;

void PlayCard::DoAction(Player* playerRef)
{
	playerRef->PlayCard(m_card, m_target);
}

void FlipCard::DoAction(Player* playerRef)
{
	playerRef->FlipCard(m_target);
}

void DestroyCard::DoAction(Player* playerRef)
{
	playerRef->m_boardRef->DestroyCard(m_target, playerRef->m_playerIndex);
}

void SwapSlots::DoAction(Player* playerRef)
{
	playerRef->SwapCards(m_target1, m_target2);
}

// FOR TESTING MEMORY LEAK
Behaviour::Behaviour()
{
	behaviours++;
}

Behaviour::~Behaviour()
{
	behaviours--;
}
