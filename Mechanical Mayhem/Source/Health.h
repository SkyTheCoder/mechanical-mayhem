//------------------------------------------------------------------------------
//
// File Name:	Health.h
// Author(s):	A.J. Bussman
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class
#include <Color.h>

//------------------------------------------------------------------------------

class Sprite;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class Health : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//   maxHealth = The max health for the object.
		//   destroyOnDeath = hether the object should be destroyed when it dies.
		Health(int maxHealth = 100, bool destroyOnDeath = true);

		// Return a new copy of the component.
		Component* Clone() const;

		// Initialize data for this object.
		void Initialize();

		// Update function for this component.
		// Params:
		//   dt = The change in time since the last step.
		void Update(float dt) override;

		// Returns the current health
		int GetHealth();
		
		// Returns the current health
		int GetMaxHealth();

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;

		// Receive an event and handle it (if applicable).
		// Params:
		//   event = The event that has been received.
		void HandleEvent(const Event& event);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Health left
		int health;

		// Max health
		int maxHealth;

		// Whether the object should be destroyed when it dies
		bool destroyOnDeath;

		// Whether the object plays a death animation when it dies
		bool hasDeathAnimation;
		std::string deathAnimationName;
	};
}