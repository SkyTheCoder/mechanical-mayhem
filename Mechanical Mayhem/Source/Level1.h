//------------------------------------------------------------------------------
//
// File Name:	Level1.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
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

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Texture;
class Mesh;
class SpriteSource;
class GameObject;
class Tilemap;
class SoundManager;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	enum class Map : int
	{
		MainMenu = 0,
		ControlScreen,
		Credits,
		LevelSelect,
		Tutorial,
		Arena3,
		MediumBoy,
		Channels,
		Separation,
		Descent,
		Blah,
		Chase,
		Merge,
		MAX_MAP
	};

	class Level1 : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Structures:
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Level 1.
		Level1(Map map);

		// Load the resources associated with Level 1.
		void Load() override;

		// Initialize the memory associated with Level 1.
		void Initialize() override;

		// Update Level 2.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Shutdown function for Level 1
		void Shutdown() override;

		// Unload the resources associated with Level 1.
		void Unload() override;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Adds static spikes to the level.
		// Params:
		//   spikes = An array of floats, each pair being a coordinate.
		//   numSpikes = How many spikes are in the array.
		void AddStaticSpikes(const float* spikes, int numSpikes);

		// Adds red spikes to the level.
		// Params:
		//   spikes = An array of floats, each pair being a coordinate.
		//   numSpikes = How many spikes are in the array.
		//   redDimension = The ID of the red dimension.
		void AddRedSpikes(const float* spikes, int numSpikes, unsigned redDimension);

		// Adds blue spikes to the level.
		// Params:
		//   spikes = An array of floats, each pair being a coordinate.
		//   numSpikes = How many spikes are in the array.
		//   blueDimension = The ID of the blue dimension.
		void AddBlueSpikes(const float* spikes, int numSpikes, unsigned blueDimension);

		// Adds chips to the level.
		// Params:
		//   chipsSpawns = An array of floats, each pair being a coordinate.
		//   numChis = How many chips are in the array.
		void AddAbilities(const float* chipsSpawns, int numChips);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Components
		SoundManager* soundManager;

		Map map;

		GameObject* backgroundImage;

		// Monkey
		unsigned columnsMonkey;
		unsigned rowsMonkey;

		// Cat
		unsigned columnsCat;
		unsigned rowsCat;

		// Spikes
		unsigned columnsSpikes;
		unsigned rowsSpikes;

		// Explosions
		unsigned columnsExplosion;
		unsigned rowsExplosion;
		
		// Flames
		unsigned columnsFlame;
		unsigned rowsFlame;

		// Rising Gears
		unsigned columnsRisingGears;
		unsigned rowsRisingGears;

		// Tilemap
		Tilemap* dataStaticMap;
		Tilemap* dataRedMap;
		Tilemap* dataBlueMap;
		unsigned columnsMap;
		unsigned rowsMap;
	};
}

//----------------------------------------------------------------------------
