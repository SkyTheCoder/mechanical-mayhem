//------------------------------------------------------------------------------
//
// File Name:	HUDLevel.h
// Author(s):	A.J. Bussman, David Cohen (david.cohen), Daniel Walther (daniel.walther)
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

#include "Vector2D.h"
#include "Level.h"
#include "Camera.h"

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class GameObject;

namespace Behaviors
{
	class HUD;
}

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	enum class Map : int;

	class HUDLevel : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of MainMenu.
		HUDLevel();

		// Load the resources associated with MainMenu.
		void Load() override;

		// Initialize the memory associated with MainMenu.
		void Initialize() override;

		// Update MainMenu.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Removes any objects that will be recreated in Initialize.
		void Shutdown() override;

		// Unload the resources associated with MainMenu.
		void Unload() override;

		// Finds the current player pointers.
		void FindPlayers();

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// HUDs & Players
		Behaviors::HUD* HUDS[4];
		GameObject* players[4];

		GameObject* dimensionSwapCountdown;
		GameObject* victoryText;
		GameObject* dimensionShiftGear;

		Camera HUDCamera;
	};
}

//----------------------------------------------------------------------------
