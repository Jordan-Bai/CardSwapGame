#include "Behaviour.h"

#include "Player.h"
#include "BoardManager.h"

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
