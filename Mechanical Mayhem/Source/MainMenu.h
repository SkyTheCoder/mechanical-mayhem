//------------------------------------------------------------------------------
//
// File Name:	MainMenu.h
// Author(s):	A.J. Bussman
// Project:		Yesterday's Mayonnaise
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Menu.h"
#include <Vector2D.h>

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class MainMenu : public Menu
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of MainMenu.
		// Params:
		//   fastIntro = Whether to speed up the into (already played).
		MainMenu(bool fastIntro = false);

		// Load the resources associated with MainMenu.
		void Load() override;

		// Initialize the memory associated with MainMenu.
		void Initialize() override;

		// Update MainMenu.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Unload the resources associated with MainMenu.
		void Unload() override;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Spawns a new random sprite to run around the menu.
		void SpawnSprite();

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Used for animations.
		float timer;

		float nextSpriteSpawn;

		// Whether to speed up the into (already played).
		bool fastIntro;

		// Animated elements.
		GameObject* backgroundOverlay;
		GameObject* title;
	};
}

//----------------------------------------------------------------------------
