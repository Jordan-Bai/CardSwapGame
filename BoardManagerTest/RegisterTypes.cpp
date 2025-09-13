#include "RegisterTypes.h"

#include "GDBoard.h"
#include "GDCard.h"
#include "GDPlayer.h"
#include "GDAbility.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

// Called when godot loads this "plugin" - does this count as a plugin?
void initialize_example_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}

	// Do this for each class we need to register
	GDREGISTER_RUNTIME_CLASS(GDBoard);
	GDREGISTER_RUNTIME_CLASS(GDPlayer);
	GDREGISTER_RUNTIME_CLASS(GDCreature);
	GDREGISTER_RUNTIME_CLASS(GDCard);
	GDREGISTER_RUNTIME_CLASS(GDDisplayCard);
	GDREGISTER_RUNTIME_CLASS(GDAbility);
	//GDREGISTER_ABSTRACT_CLASS(GDAbilityEffect);
	GDREGISTER_RUNTIME_CLASS(GDAbilityEffect);
	GDREGISTER_RUNTIME_CLASS(GDChangeStats);
	GDREGISTER_RUNTIME_CLASS(GDFlip);
	GDREGISTER_RUNTIME_CLASS(GDHeal);
	GDREGISTER_RUNTIME_CLASS(GDGainEnergy);
	GDREGISTER_RUNTIME_CLASS(GDRandomizeStats);
	GDREGISTER_RUNTIME_CLASS(GDBuffPerFamilyCard);
	GDREGISTER_RUNTIME_CLASS(GDBuffEachFamilyCard);
}

// Called when godot unloads this
void uninitialize_example_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
}

extern "C"
{
	// Initializes the library itself?
	GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
	{
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_example_module);
		init_obj.register_terminator(uninitialize_example_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}