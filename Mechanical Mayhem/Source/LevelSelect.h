//------------------------------------------------------------------------------
//
// File Name:	LevelSelect.h
// Author(s):	Daniel Walther (daniel.walther)
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

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class LevelSelect : public Menu
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of LevelSelect.
		LevelSelect();

		// Load the resources associated with LevelSelect.
		void Load() override;

		// Initialize the memory associated with LevelSelect.
		void Initialize() override;

		// Update LevelSelect.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Unload the resources associated with LevelSelect.
		void Unload() override;
	};
}

//----------------------------------------------------------------------------
