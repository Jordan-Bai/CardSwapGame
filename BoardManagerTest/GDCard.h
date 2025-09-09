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
		//GDCreature(CreatureData* data);
		GDCreature(const GDCreature& ref) = delete; //copy constructor
		//GDCard& operator=(const GDCard& ref) = delete; //assignment operator
		~GDCreature();

		int GetHP();
		int GetAtk();
		int GetFlipCost();
		int GetAbilityCost();
		String GetName();

		void SetHP(int hp);
		void SetAtk(int atk);
		void SetFlipCost(int fCost);
		void SetAbilityCost(int aCost);
		void SetName(String name);

		CreatureData* GetData();
	};

	class GDCard : public Object
	{
		GDCLASS(GDCard, Object); // Macro that "sets up a few things for us"?
		// Arguments are the class, and the class it inherits from

	private:
		CardData* m_data;
		GDCreature* m_frontFace;
		GDCreature* m_backFace;

	protected:
		static void _bind_methods(); // Godot will call this to check what methods can be called from godot and "which properties it exposes"

	public:
		GDCard();
		//GDCard(CardData* data);
		GDCard(const GDCard& ref) = delete; //copy constructor
		//GDCard& operator=(const GDCard& ref) = delete; //assignment operator
		~GDCard();

		int GetCost();
		void SetCost(int cost);
		GDCreature* GetFrontFace();
		GDCreature* GetBackFace();
		void SetFrontFace(GDCreature* creature);
		void SetBackFace(GDCreature* creature);

		CardData* GetData();
	};

	class GDDisplayCard : public Node3D
	{
		GDCLASS(GDDisplayCard, Node3D); // Macro that "sets up a few things for us"?
		// Arguments are the class, and the class it inherits from

	protected:
		static void _bind_methods(); // Godot will call this to check what methods can be called from godot and "which properties it exposes"

	public:
		int m_cost;
		int m_flipCost;
		int m_abilityCost;
		int m_hp;
		int m_atk;
		String m_name;

		GDDisplayCard();
		GDDisplayCard(GDCard* card);
		GDDisplayCard(CardData* card);

		int GetHP();
		int GetAtk();
		int GetCost();
		int GetFlipCost();
		int GetAbilityCost();
		String GetName();

		void SetHP(int hp);
		void SetAtk(int atk);
		void SetCost(int cost);
		void SetFlipCost(int flipCost);
		void SetAbilityCost(int abilityCost);
		void SetName(String name);

		// Will set the hp,atk etc to the stats of this creature
		void SetStats(GDCreature* card);
	};
}