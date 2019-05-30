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

class Physics;

namespace Behaviors
{
	class PlayerMovement;
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

		// Callback for when the player attempts to use this ability.
		void OnUse() override;

		// Returns the % of mana/fuel/uses/whatever left on this ability.
		float GetMana() override;

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
		Behaviors::PlayerMovement* playerMovement;
		Physics* physics;

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