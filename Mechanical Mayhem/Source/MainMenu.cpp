//------------------------------------------------------------------------------
//
// File Name:	MainMenu.cpp
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
#include "MainMenu.h"

// Systems
#include <Engine.h>
#include <FullscreenManager.h>
#include <GameObjectManager.h>
#include <GameObjectFactory.h>
#include <ResourceManager.h>
#include "Archetypes.h"
#include <Space.h>
#include <SoundManager.h>
#include <Input.h>
#include <Graphics.h>
#include <Camera.h>
#include <Mesh.h>
#include <MeshHelper.h>
#include <Texture.h>
#include <SpriteSource.h>

// Components
#include <SpriteTextMono.h>
#include <Transform.h>
#include "Button.h"

// Levels
#include "LevelSelect.h"
#include "Level1.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Levels
{
	// Creates an instance of MainMenu.
	MainMenu::MainMenu()
		: Level("MainMenu")
	{
	}

	// Load the resources associated with MainMenu.
	void MainMenu::Load()
	{
		Menu::Load();

		std::cout << "MainMenu::Load" << std::endl;

		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		resourceManager.GetMesh("Quad", Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));
		resourceManager.GetSpriteSource("MainMenu.png");
		resourceManager.GetSpriteSource("Button.png");
		resourceManager.GetMesh("FontAtlas", 12, 8);
		resourceManager.GetSpriteSource("Code New Roman@2x.png", 12, 8);

		objectManager.AddArchetype(*objectFactory.CreateObject("FullScreenImage", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("MainMenu.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Button", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Button.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Text", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman@2x.png")));
	}

	// Initialize the memory associated with MainMenu.
	void MainMenu::Initialize()
	{
		std::cout << "MainMenu::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		//objectManager.AddArchetype(*Archetypes::CreateFullScreenImageArchetype(meshBackground, spriteSourceBackground));
		objectManager.AddObject(*new GameObject(*objectManager.GetArchetypeByName("FullScreenImage")));

		//objectManager.AddArchetype(*Archetypes::CreateButtonArchetype(meshButton, spriteSourceButton));

		// Create and add descriptive text
		//objectManager.AddArchetype(*Archetypes::CreateText());

		/*GameObject* text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("Press space to go level select");
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(Vector2D(0.0f, -240.0f));
		//static_cast<Transform*>(text->GetComponent("Transform"))->SetScale(Vector2D(32.0f, 32.0f));
		objectManager.AddObject(*text);*/

		MenuButton* controls = AddMenuButton("Controls", Vector2D(-1.75f, -2.5f), Levels::Map::ControlScreen);
		MenuButton* credits = AddMenuButton("Credits", Vector2D(1.75f, -2.5f), Levels::Map::Credits);
		MenuButton* lobby = AddMenuButton("Lobby", Vector2D(0.0f, -1.5f), Levels::Map::Lobby);
		MenuButton* exit = AddMenuButton("Exit", Vector2D(0.0f, -3.5f), Levels::Map::Exit);

		controls->north = lobby;
		controls->east = credits;
		controls->south = exit;
		controls->west = credits;
		credits->north = lobby;
		credits->east = controls;
		credits->south = exit;
		credits->west = controls;
		lobby->north = exit;
		lobby->south = controls;
		exit->north = controls;
		exit->south = lobby;

		SetDefaultButton(lobby);

		Camera& camera = Graphics::GetInstance().GetDefaultCamera();
		camera.SetTranslation(Vector2D());
		camera.SetSize(10.0f);

		// Main menu music
		SoundManager* soundManager = Engine::GetInstance().GetModule<SoundManager>();
		if (soundManager->GetMusicChannel() == nullptr)
			soundManager->PlaySound("SoundMenuM.wav");
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void MainMenu::Update(float dt)
	{
		Menu::Update(dt);
	}

	// Unload the resources associated with MainMenu.
	void MainMenu::Unload()
	{
		Menu::Unload();

		std::cout << "MainMenu::Unload" << std::endl;
	}
}
//----------------------------------------------------------------------------
