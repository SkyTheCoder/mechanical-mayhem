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

		MenuButton* clockwork = AddMenuButton("Clockwork", Vector2D(-1.75f, 1.5f), Levels::Map::Clockwork);
		MenuButton* chase = AddMenuButton("Chase", Vector2D(1.75f, 1.5f), Levels::Map::Chase);
		MenuButton* merge = AddMenuButton("Merge", Vector2D(-1.75f, 0.5f), Levels::Map::Merge);
		MenuButton* descent = AddMenuButton("Descent", Vector2D(1.75f, 0.5f), Levels::Map::Descent);
		MenuButton* snake = AddMenuButton("Snake", Vector2D(-1.75f, -0.5f), Levels::Map::Snake);
		MenuButton* cavern = AddMenuButton("Cavern", Vector2D(1.75f, -0.5f), Levels::Map::Cavern);
		MenuButton* lobby = AddMenuButton("Lobby", Vector2D(0.0f, -2.5f), Levels::Map::Lobby);

		clockwork->north = lobby;
		clockwork->east = chase;
		clockwork->south = merge;
		clockwork->west = chase;

		chase->north = lobby;
		chase->east = clockwork;
		chase->south = descent;
		chase->west = clockwork;

		merge->north = clockwork;
		merge->east = descent;
		merge->south = snake;
		merge->west = descent;

		descent->north = chase;
		descent->east = merge;
		descent->south = cavern;
		descent->west = merge;

		snake->north = merge;
		snake->east = cavern;
		snake->south = lobby;
		snake->west = cavern;

		cavern->north = descent;
		cavern->east = snake;
		cavern->south = lobby;
		cavern->west = snake;

		lobby->north = snake;
		lobby->east = cavern;
		lobby->south = clockwork;
		lobby->west = snake;

		SetDefaultButton(clockwork);
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
