//------------------------------------------------------------------------------
//
// File Name:	Controls.h
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

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class Controls : public Menu
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Controls.
		Controls();

		// Load the resources associated with Controls.
		void Load() override;

		// Initialize the memory associated with Controls.
		void Initialize() override;

		// Update Controls.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Unload the resources associated with Controls.
		void Unload() override;
	};
}

//----------------------------------------------------------------------------