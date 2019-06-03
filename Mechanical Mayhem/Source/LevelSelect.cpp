//------------------------------------------------------------------------------
//
// File Name:	LevelSelect.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Yesterday's Mayonnaise
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "LevelSelect.h"

// Systems
#include "Archetypes.h"
#include <Space.h>
#include <Engine.h>
#include <GameObjectFactory.h>
#include <Input.h>
#include <SoundManager.h>
#include <MeshHelper.h>
#include <Texture.h>
#include <SpriteSource.h>
#include <Mesh.h>

// Components
#include <SpriteTextMono.h>
#include <Transform.h>
#include "Button.h"

// Levels
#include "Level1.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Levels
{
	// Creates an instance of LevelSelect.
	LevelSelect::LevelSelect()
		: Level("LevelSelect")
	{
	}

	// Load the resources associated with LevelSelect.
	void LevelSelect::Load()
	{
		Menu::Load();

		std::cout << "LevelSelect::Load" << std::endl;

		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		resourceManager.GetMesh("Quad", Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("AniMainMenu", Vector2D(1.0f / 2, 1.0f / 2), Vector2D(0.5f, 0.5f));
		resourceManager.GetSpriteSource("Button.png");
		resourceManager.GetMesh("FontAtlas", 12, 8);
		resourceManager.GetSpriteSource("AniMainMenu.png", 2, 2);

		objectManager.AddArchetype(*objectFactory.CreateObject("FullScreenBackground", resourceManager.GetMesh("AniMainMenu"), resourceManager.GetSpriteSource("AniMainMenu.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Button", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Button.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Text", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman@2x.png")));
	}

	// Initialize the memory associated with LevelSelect.
	void LevelSelect::Initialize()
	{
		std::cout << "LevelSelect::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		objectManager.AddObject(*new GameObject(*objectManager.GetArchetypeByName("FullScreenBackground")));

		// Create and add descriptive text
		GameObject* text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		text->GetComponent<SpriteTextMono>()->SetText("Select Your Level");
		text->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, 2.5f));
		objectManager.AddObject(*text);

		/*AddMenuButton("Tutorial", Vector2D(-1.75f, 1.5f), Levels::Map::Tutorial);
		AddMenuButton("Arena 3", Vector2D(1.75f, 1.5f), Levels::Map::Arena3);
		AddMenuButton("MediumBoy", Vector2D(-1.75f, 0.5f), Levels::Map::MediumBoy);
		AddMenuButton("Channels", Vector2D(1.75, 0.5f), Levels::Map::Channels);
		AddMenuButton("Separation", Vector2D(-1.75f, -0.5f), Levels::Map::Separation);
		AddMenuButton("Descent", Vector2D(1.75f, -0.5f), Levels::Map::Descent);*/

		MenuButton* blah = AddMenuButton("Blah", Vector2D(-1.75f, 1.5f), Levels::Map::Blah);
		MenuButton* chase = AddMenuButton("Chase", Vector2D(1.75f, 1.5f), Levels::Map::Chase);
		MenuButton* merge = AddMenuButton("Merge", Vector2D(-1.75f, 0.5f), Levels::Map::Merge);
		MenuButton* descent = AddMenuButton("Descent", Vector2D(1.75f, 0.5f), Levels::Map::Descent);
		MenuButton* lobby = AddMenuButton("Lobby", Vector2D(0.0f, -2.5f), Levels::Map::Lobby);

		blah->north = lobby;
		blah->east = chase;
		blah->south = merge;
		blah->west = chase;

		chase->north = lobby;
		chase->east = blah;
		chase->south = descent;
		chase->west = blah;

		merge->north = blah;
		merge->east = descent;
		merge->south = lobby;
		merge->west = descent;

		descent->north = chase;
		descent->east = merge;
		descent->south = lobby;
		descent->west = merge;

		lobby->north = merge;
		lobby->south = blah;

		SetDefaultButton(blah);
	}

	// Update LevelSelect.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void LevelSelect::Update(float dt)
	{
		FixCamera();
		
		Menu::Update(dt);
	}

	// Unload the resources associated with LevelSelect.
	void LevelSelect::Unload()
	{
		Menu::Unload();

		std::cout << "LevelSelect::Unload" << std::endl;
	}
}
//----------------------------------------------------------------------------
