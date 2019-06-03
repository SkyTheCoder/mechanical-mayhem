//------------------------------------------------------------------------------
//
// File Name:	Controls.cpp
// Author(s):	A.J. Bussman
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
#include "Controls.h"

// Systems
#include "Archetypes.h"
#include "Space.h"
#include <Input.h>
#include "MeshHelper.h"
#include <Texture.h>
#include <SpriteSource.h>
#include <Mesh.h>
#include <GameObjectFactory.h>

// Components
#include <SpriteTextMono.h>
#include <Transform.h>
#include "Button.h"

// Levels
#include "MainMenu.h"
#include "Level1.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Levels
{
	// Creates an instance of Controls.
	Controls::Controls() : Level("Controls")
	{
	}

	// Load the resources associated with Controls.
	void Controls::Load()
	{
		Menu::Load();

		std::cout << "Controls::Load" << std::endl;

		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		resourceManager.GetMesh("Quad", Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("AniMainMenu", Vector2D(1.0f / 2, 1.0f / 2), Vector2D(0.5f, 0.5f));
		resourceManager.GetSpriteSource("Controls.png");
		resourceManager.GetSpriteSource("Button.png");
		resourceManager.GetMesh("FontAtlas", 12, 8);
		resourceManager.GetSpriteSource("Code New Roman@2x.png", 12, 8);
		resourceManager.GetSpriteSource("AniMainMenu.png", 2, 2);

		objectManager.AddArchetype(*objectFactory.CreateObject("FullScreenBackground", resourceManager.GetMesh("AniMainMenu"), resourceManager.GetSpriteSource("AniMainMenu.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("FullScreenImage", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Controls.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Button", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Button.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Text", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman@2x.png")));
	}

	// Initialize the memory associated with Controls.
	void Controls::Initialize()
	{
		std::cout << "Controls::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		objectManager.AddObject(*new GameObject(*objectManager.GetArchetypeByName("FullScreenBackground")));
		objectManager.AddObject(*new GameObject(*objectManager.GetArchetypeByName("FullScreenImage")));

		MenuButton* mainMenu = AddMenuButton("Main Menu", Vector2D(0.0f, -2.5f), Levels::Map::MainMenu);

		SetDefaultButton(mainMenu);
	}

	// Update Controls.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Controls::Update(float dt)
	{
		FixCamera();

		Menu::Update(dt);
	}

	// Unload the resources associated with Controls.
	void Controls::Unload()
	{
		Menu::Unload();

		std::cout << "Controls::Unload" << std::endl;
	}
}
//----------------------------------------------------------------------------