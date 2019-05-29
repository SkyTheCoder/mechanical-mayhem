//------------------------------------------------------------------------------
//
// File Name:	Flamethrower.h
// Author(s):	David Cohen (david.cohen)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
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

namespace Behaviors
{
	class PlayerMovement;
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

		// Callback for when the player attempts to use this ability.
		void OnUse() override;

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
		// Protected Variables:
		//------------------------------------------------------------------------------

		// Components
		Transform* transform;
		Physics* physics;
		Collider* collider;
		Behaviors::PlayerMovement* playerController;

		std::string fireballArchetypeName;
		GameObject* fireballArchetype;

		// Cooldown between shooting flames.
		float speed;
		float cooldown;
		float cooldownTimer;
	};
}

//------------------------------------------------------------------------------
