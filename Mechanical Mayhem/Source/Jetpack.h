//------------------------------------------------------------------------------
//
// File Name:	Jetpack.h
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Ability.h"
#include "PlayerMovement.h"

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;

namespace Behaviors
{
	class PlayerMovement;
	class MonkeyAnimation;
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
	class Jetpack : public Ability
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Default Constructor
		Jetpack();

		// Initialize data for this object.
		void Initialize();

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

		// Callback for when the player attempts to use this ability.
		void OnUse() override;

		// Returns the % of mana/fuel/uses/whatever left on this ability.
		float GetMana() const override;

		// Sets whether the jetpack is active
		void SetActive(bool active_);

		// Gets whether the jetpack is active
		bool IsActive() const;

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;

		// Shutdown function for jetpack
		void Shutdown() override;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Handles fuel consumption/refilling
		void FuelManagement(float dt);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Other components
		Transform* transform;
		Physics* physics;
		Behaviors::PlayerMovement* playerMovement;
		Behaviors::MonkeyAnimation* monkeyAnimation;

		std::string flameEffectName;
		GameObject* flameEffect;

		// Sounds
		SoundManager* soundManager;
		FMOD::Channel* jetpackSound;

		// Jetpack is being used
		bool active;

		// Speed
		float jetpackForce;			// Force jetpack exerts when active
		
		// Fuel
		float currentFuel;			// Current amount of fuel (f)
		float maxFuel;				// Maximum fuel capacity (f)
		float fuelRefillRate;		// Rate at which the fuel refills on the ground (f/s)
		float fuelConsumptionRate;	// Rate at which the fuel is consumed (f/s)
	};
}

//------------------------------------------------------------------------------