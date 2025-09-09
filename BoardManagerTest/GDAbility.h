#pragma once

#include "Ability.h"

#include <godot_cpp/classes/resource.hpp>

namespace godot
{
	class GDAbility : public Resource
	{
		GDCLASS(GDAbility, Resource); // Macro that "sets up a few things for us"?
		// Arguments are the class, and the class it inherits from

	protected:
		AbilityTrigger m_trigger;

		static void _bind_methods(); // Godot will call this to check what methods can be called from godot and "which properties it exposes"
	public:
		GDAbility();
		GDAbility(const GDAbility& ref) = delete; //copy constructor
		//GDAbility& operator=(const GDAbility& ref) = delete; //assignment operator
		~GDAbility();

		int GetAbilityTrigger();
		void SetAbilityTrigger(int newTrigger);
	};
}