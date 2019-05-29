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

#include "Vector2D.h"
#include "Level.h"

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Texture;
class Mesh;
class SpriteSource;
class SoundManager;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	enum class Map : int;

	class LevelSelect : public Level
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

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Adds a new map button.
		// name = The name of the level.
		// position = The position of the button.
		// map = The map the button should switch to.
		void AddMapButton(const char* name, Vector2D position, Levels::Map map);
	};
}

//----------------------------------------------------------------------------
