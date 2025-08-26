#pragma once

#include "Card.h"

//#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/node3d.hpp>

// Namespace that everything in GDExtension is defined in
namespace godot
{
	class GDCreature : public Object
	{
		GDCLASS(GDCreature, Object); // Macro that "sets up a few things for us"?
		// Arguments are the class, and the class it inherits from

	private:
		CreatureData* m_data;

	protected:
		static void _bind_methods(); // Godot will call this to check what methods can be called from godot and "which properties it exposes"

	public:
		GDCreature();
		GDCreature(CreatureData* data);

		int GetHP();
		int GetAtk();
		int GetFlipCost();
	};

	class GDCard : public Node3D
	{
		GDCLASS(GDCard, Node3D); // Macro that "sets up a few things for us"?
		// Arguments are the class, and the class it inherits from

	private:
		CardData* m_data;
		GDCreature* m_frontFace;
		GDCreature* m_backFace;

	protected:
		static void _bind_methods(); // Godot will call this to check what methods can be called from godot and "which properties it exposes"

	public:
		GDCard();
		GDCard(CardData* data);
		GDCard(const GDCard& ref) = delete; //copy constructor
		//GDCard& operator=(const GDCard& ref) = delete; //assignment operator
		~GDCard();

		int GetCost();
		GDCreature* GetFrontFace();
		GDCreature* GetBackFace();
	};
}