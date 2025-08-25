#pragma once

#include "Card.h"

#include <godot_cpp/classes/object.hpp>

// Namespace that everything in GDExtension is defined in
namespace godot
{
	class GDCard : public Object
	{
		GDCLASS(GDCard, Object); // Macro that "sets up a few things for us"?
		// Arguments are the class, and the class it inherits from

	private:
		CardData* m_data;

	protected:
		static void _bind_methods(); // Godot will call this to check what methods can be called from godot and "which properties it exposes"

	public:
		GDCard();
		GDCard(const GDCard& ref) = delete; //copy constructor
		//GDCard& operator=(const GDCard& ref) = delete; //assignment operator
		~GDCard();
	};
}