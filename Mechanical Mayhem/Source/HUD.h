//------------------------------------------------------------------------------
//
// File Name:	HUD.h
// Author(s):	A.J. Bussman, David Cohen (david.cohen)
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
class Camera;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class Health;
	class AbilityHolder;

	class HUD : public BetaObject
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		HUD(GameObject* player, Camera& camera);

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
		// Private Functions:
		//------------------------------------------------------------------------------

		// Sets the location for each HUD object
		void SetHUDObjectLocations();
		
		// Gets the player pointer.
		GameObject* GetPlayer();

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		GUID player;
		int playerID;
		Health* health;
		AbilityHolder* abilityHolder;
		GameObject* hudBackground;

		Camera& camera;

		// Health variables
		GameObject* healthBarCenter;
		GameObject* healthBarEnd;
		GameObject* healthBarBackgroundCenter;
		GameObject* healthBarBackgroundEnd;
		float animHealth;

		// Ability variables
		GameObject* abilityBarCenter;
		GameObject* abilityBarEnd;
		GameObject* abilityBarBackgroundCenter;
		GameObject* abilityBarBackgroundEnd;
		float animAbility;
	};
}