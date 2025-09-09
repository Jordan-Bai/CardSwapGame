#pragma once

#include "BoardManager.h"
#include "DealerAI.h"
#include "GDCard.h"
#include "GDPlayer.h"

#include <godot_cpp/classes/node3d.hpp>

// Namespace that everything in GDExtension is defined in
namespace godot
{
	class GDBoard : public Node3D
	{
		GDCLASS(GDBoard, Node3D); // Macro that "sets up a few things for us"?
		// Arguments are the class, and the class it inherits from

	private:
		int m_slots = 4;

	protected:
		DealerAI* m_dealerAI;
		BoardManager* m_board; // The actual board itself
		
		Player* m_dealerData;
		Player* m_playerData;
		GDPlayer* m_dealer;
		GDPlayer* m_player;

		std::vector<GDDisplayCard*> m_side1;
		std::vector<GDDisplayCard*> m_side2;

		static void _bind_methods(); // Godot will call this to check what methods can be called from godot and "which properties it exposes"

	public:
		GDBoard();
		GDBoard(const GDBoard& ref) = delete; //copy constructor
		//GDBoard& operator=(const GDBoard& ref) = delete; //assignment operator
		~GDBoard();

		void StartMatch();

		void DoDealerTurn();
		void DoAttackPhase();

		void UpdateBoardState();
		void UpdateCardStats(GDDisplayCard* displayCard, ActiveCard* realCard);

		bool IsOccupied(int slot, int side);
		GDDisplayCard* GetCard(int slot, int side);

		GDPlayer* GetDealer();
		GDPlayer* GetPlayer();

		//bool PlayCard(int cardIndex, int targetSlot);
		//bool FlipCard(int targetSlot);
		//bool SwapCards(int targetSlot1, int targetSlot2);
		//bool DestroyCard(int targetSlot);
	};
}
