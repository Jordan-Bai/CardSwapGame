#pragma once

#include "Player.h"
#include "GDCard.h"

//#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/object.hpp>

// Namespace that everything in GDExtension is defined in
namespace godot
{
	class GDBoard;

	class GDPlayer : public Object
	{
		GDCLASS(GDPlayer, Object); // Macro that "sets up a few things for us"?
		// Arguments are the class, and the class it inherits from

	private:

	protected:
		Player* m_data;
		std::vector<GDDisplayCard*> m_hand;
		std::vector<CardData*> m_deck;

		static void _bind_methods(); // Godot will call this to check what methods can be called from godot and "which properties it exposes"

	public:
		GDPlayer();
		GDPlayer(Player* data);
		GDPlayer(const GDPlayer& ref) = delete; //copy constructor
		//GDPlayer& operator=(const GDPlayer& ref) = delete; //assignment operator
		~GDPlayer();

		void AddCardToDeckGD(GDCard* card);
		void AddCardToDeck(CardData* card);

		//void Init(GDBoard* board);
		void StartMatch();
		void StartTurn();

		int GetHealth();
		int GetEnergy();
		GDDisplayCard* GetDisplayCard(int cardIndex);
		int GetHandSize();

		bool PlayCard(int cardIndex, int targetSlot);
		bool FlipCard(int targetSlot);
		bool SwapCards(int targetSlot1, int targetSlot2);
		bool DestroyCard(int targetSlot);
	};
}