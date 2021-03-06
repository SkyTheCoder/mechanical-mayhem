//------------------------------------------------------------------------------
//
// File Name:	Flamethrower.h
// Author(s):	David Cohen (david.cohen)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Ability.h" // Base class

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
class Collider;
class SoundManager;

namespace Behaviors
{
	class PlayerMovement;
}

namespace FMOD
{
	class Channel;
}

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Abilities
{
	class Flamethrower : public Ability
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		Flamethrower();

		// Initialize this ability.
		void Initialize() override;

		// Clone the current ability.
		Ability* Clone() const override;

		// Update function for this ability.
		// Params:
		//   dt = The change in time since the last update.
		void Update(float dt) override;

		// Updates components using a fixed timestep (usually just for physics).
		// Params:
		//	 dt = A fixed change in time, usually 1/60th of a second.
		void FixedUpdate(float dt) override;

		// Removes any objects that will be recreated in Initialize.
		void Shutdown() override;

		// Callback for when the player attempts to use this ability.
		void OnUse() override;

		// Returns the % of mana/fuel/uses/whatever left on this ability.
		float GetMana() const override;

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;

	protected:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Deals with audio for flamethrower
		void HandleAudio();

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Components
		Transform* transform;
		Physics* physics;
		Collider* collider;
		Behaviors::PlayerMovement* playerController;

		std::string flameArchetypeName;
		GameObject* flameArchetype;

		std::string flameEffectName;
		GameObject* flameEffect;

		// Sounds
		SoundManager* soundManager;
		FMOD::Channel* flamethrowerSound;

		// Cooldown between shooting flames.
		float speed;
		float cooldown;
		float cooldownTimer;

		// Fuel
		float currentFuel;			// Current amount of fuel (f)
		float maxFuel;				// Maximum fuel capacity (f)
		float fuelRefillRate;		// Rate at which the fuel refills on the ground (f/s)
		float fuelConsumptionRate;	// Rate at which the fuel is consumed (f/s)
	};
}

//------------------------------------------------------------------------------
