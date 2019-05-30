//------------------------------------------------------------------------------
//
// File Name:	ProximityMine.h
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

#include <deque>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Collider;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Abilities
{
	class ProximityMine : public Ability
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		ProximityMine();

		// Initialize this ability.
		void Initialize() override;

		// Clone the current ability.
		Ability* Clone() const override;

		// Update function for this ability.
		// Params:
		//   dt = The change in time since the last update.
		void Update(float dt) override;

		// Shutdown this ability.
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
		// Protected Variables:
		//------------------------------------------------------------------------------

		// Components
		Transform* transform;
		Collider* collider;

		std::string mineArchetypeName;
		GameObject* mineArchetype;
		
		// Cooldown between placing mines.
		float cooldown;
		float cooldownTimer;

		// The maximum amount of mines that can be placed at once.
		int maxProximityMines;

		// The list of mines in the world.
		std::deque<GUID> proximityMines;
	};
}

//------------------------------------------------------------------------------
