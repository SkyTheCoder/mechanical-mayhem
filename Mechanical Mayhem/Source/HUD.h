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

#include <BetaObject.h>

#include <Vector2D.h>

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class GameObject;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class HUD : public BetaObject
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
		void Initialize() override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Removes any objects that will be recreated in Initialize.
		void Shutdown() override;

		// Sets the current player pointer.
		void SetPlayer(GameObject* player);

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