//------------------------------------------------------------------------------
//
// File Name:	HUD.h
// Author(s):	A.J. Bussman
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


//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class GameObject;
class Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class HUD
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		HUD(GameObject* player, Vector2D translation);

		// Return a new copy of the component.
		HUD* Clone() const;

		// Initialize data for this object.
		void Initialize();

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt);

		~HUD();

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		GameObject* player;

		Vector2D translation;

		// Health variables
		std::vector<GameObject*> healthBar;
		GameObject* healthText;
		float prevHealth;

		// Ability variables
		std::vector<GameObject*> abilityBar;
		float prevMana;
	};
}