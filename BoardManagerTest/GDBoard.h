#pragma once

#include "BoardManager.h"
#include "DealerAI.h"

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
		std::vector<CreatureData*> creatures;
		std::vector<CardData*> cards;

		DealerAI* m_dealerAI;
		DealerAI* m_testPlayerAI; // FOR TESTING
		BoardManager* m_board; // The actual board itself
		
		Player* m_dealerData;
		Player* m_playerData;

		static void _bind_methods(); // Godot will call this to check what methods can be called from godot and "which properties it exposes"

	public:
		GDBoard();
		GDBoard(const GDBoard& ref) = delete; //copy constructor
		//GDBoard& operator=(const GDBoard& ref) = delete; //assignment operator
		~GDBoard();

		//int GetNumSlots();
		//void SetNumSlots(int num);

		void DoPlayerTurn();
		void DoDealerTurn();
		void DoAttackPhase();

		bool IsOccupied(int slot, int side);
	};
}
