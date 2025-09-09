#pragma once

#include "AbilityEffect.h"

#include <godot_cpp/classes/resource.hpp>

namespace godot
{
	class GDAbilityEffect : public godot::Resource
	{
		GDCLASS(GDAbilityEffect, Resource); // Macro that "sets up a few things for us"?
		// Arguments are the class, and the class it inherits from

	protected:
		static void _bind_methods(); // Godot will call this to check what methods can be called from godot and "which properties it exposes"

	public:
		GDAbilityEffect();

		virtual AbilityEffect* CreateEffect();
	};

	// ChangeStats
	class GDChangeStats : public GDAbilityEffect
	{
		GDCLASS(GDChangeStats, GDAbilityEffect);

	protected:
		int m_hp = 0;
		int m_atk = 0;
		int m_fCost = 0;

		static void _bind_methods();

	public:
		GDChangeStats() = default;

		virtual AbilityEffect* CreateEffect() override;

		int GetHP();
		int GetAtk();
		int GetFlipCost();

		void SetHP(int hp);
		void SetAtk(int atk);
		void SetFlipCost(int fCost);
	};

	// Flip
	class GDFlip : public GDAbilityEffect
	{
		GDCLASS(GDFlip, GDAbilityEffect);

	protected:
		static void _bind_methods();

	public:
		GDFlip() = default;

		virtual AbilityEffect* CreateEffect() override;
	};

	// Heal
	class GDHeal : public GDAbilityEffect
	{
		GDCLASS(GDHeal, GDAbilityEffect);

	protected:
		int m_hp = 0;

		static void _bind_methods();

	public:
		GDHeal() = default;

		virtual AbilityEffect* CreateEffect() override;

		int GetHP();
		void SetHP(int hp);
	};

	// GainEnergy
	class GDGainEnergy : public GDAbilityEffect
	{
		GDCLASS(GDGainEnergy, GDAbilityEffect);

	protected:
		int m_energy = 0;

		static void _bind_methods();

	public:
		GDGainEnergy() = default;

		virtual AbilityEffect* CreateEffect() override;

		int GetEnergy();
		void SetEnergy(int energy);
	};

	// PickupCard
	class GDPickupCard : public GDAbilityEffect
	{
		GDCLASS(GDPickupCard, GDAbilityEffect);

	protected:
		static void _bind_methods();

	public:
		GDPickupCard();

		virtual AbilityEffect* CreateEffect() override;
	};

	// RandomizeStats
	class GDRandomizeStats : public GDAbilityEffect
	{
		GDCLASS(GDRandomizeStats, GDAbilityEffect);

	protected:
		int m_min = 0;
		int m_max = 0;

		static void _bind_methods();

	public:
		GDRandomizeStats() = default;

		virtual AbilityEffect* CreateEffect() override;

		int GetMin();
		int GetMax();

		void SetMin(int min);
		void SetMax(int max);
	};

	// CopyCards
	class GDCopyCards : public GDAbilityEffect
	{
		GDCLASS(GDCopyCards, GDAbilityEffect);

	protected:
		static void _bind_methods();

	public:
		GDCopyCards();

		virtual AbilityEffect* CreateEffect() override;
	};

	class A : public GDAbilityEffect
	{
		GDCLASS(A, GDAbilityEffect);

	protected:
		static void _bind_methods();

	public:
		

		virtual AbilityEffect* CreateEffect() override;
	};
}