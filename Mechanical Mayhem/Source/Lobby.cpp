//------------------------------------------------------------------------------
//
// File Name:	Lobby.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Lobby.h"

// Systems
#include <Engine.h>
#include <GameObjectFactory.h>
#include <GameObjectManager.h>
#include <ResourceManager.h>
#include <Space.h>
#include <GameObject.h>
#include <Input.h>
#include <ExtendedInput.h>
#include "InputSchemeManager.h"

// Components
#include <Transform.h>
#include <Sprite.h>
#include <SpriteTextMono.h>
#include "Button.h"

// Levels
#include "Level1.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Lobby.
	Lobby::Lobby() : Level("Lobby"), joinHint(nullptr), joinHint2(nullptr), levelSelect(nullptr)
	{
	}

	// Load the resources associated with Lobby.
	void Lobby::Load()
	{
		Menu::Load();

		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		resourceManager.GetMesh("AniMainMenu", Vector2D(1.0f / 2, 1.0f / 2), Vector2D(0.5f, 0.5f));
		resourceManager.GetSpriteSource("AniMainMenu.png", 2, 2);

		objectManager.AddArchetype(*objectFactory.CreateObject("FullScreenBackground", resourceManager.GetMesh("AniMainMenu"), resourceManager.GetSpriteSource("AniMainMenu.png")));

		std::cout << "Lobby::Load" << std::endl;
	}

	// Initialize the memory associated with Lobby.
	void Lobby::Initialize()
	{
		std::cout << "Lobby::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		objectManager.AddObject(*new GameObject(*objectManager.GetArchetypeByName("FullScreenBackground")));

		// Create and add descriptive text
		GameObject* title = new GameObject(*objectManager.GetArchetypeByName("Text"));
		title->GetComponent<SpriteTextMono>()->SetText("Lobby");
		title->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, 2.5f));
		objectManager.AddObject(*title);

		MenuButton* mainMenu = AddMenuButton("Main Menu", Vector2D(-1.75f, -2.5f), Levels::Map::MainMenu);
		levelSelect = AddMenuButton("Level Select", Vector2D(1.75f, -2.5f), Levels::Map::LevelSelect);

		mainMenu->east = levelSelect;
		mainMenu->west = levelSelect;
		levelSelect->east = mainMenu;
		levelSelect->west = mainMenu;

		SetDefaultButton(levelSelect);

		joinHint = new GameObject(*objectManager.GetArchetypeByName("Text"));
		joinHint->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, -1.0f));
		joinHint->GetComponent<SpriteTextMono>()->SetText("The game requires at least 2 players to start!");
		objectManager.AddObject(*joinHint);

		joinHint2 = new GameObject(*objectManager.GetArchetypeByName("Text"));
		joinHint2->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, -1.5f));
		joinHint2->GetComponent<SpriteTextMono>()->SetText("Press CTRL or START to join the lobby");
		objectManager.AddObject(*joinHint2);

		for (int i = 0; i < 6; i++)
		{
			GameObject* text = new GameObject(*objectManager.GetArchetypeByName("Text"));
			text->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, 1.0f - i * 0.5f));
			objectManager.AddObject(*text);
			playerIcons.push_back(text);
		}
	}

	// Update Lobby.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Lobby::Update(float dt)
	{
		FixCamera();

		Menu::Update(dt);

		InputSchemeManager& inputSchemeManager = *Engine::GetInstance().GetModule<InputSchemeManager>();

		Input& input = Input::GetInstance();
		ExtendedInput& extendedInput = ExtendedInput::GetInstance();

		if (input.CheckTriggered(VK_LCONTROL))
		{
			InputScheme scheme('W', 'A', 'D', 'S', VK_LCONTROL);
			InputSource source = inputSchemeManager.FindScheme(scheme);
			if (source != IS_NONE)
			{
				inputSchemeManager.RemoveSource(source);
			}
			else
			{
				inputSchemeManager.AddScheme(scheme);
			}
		}
		
		if (input.CheckTriggered(VK_RCONTROL))
		{
			InputScheme scheme(VK_UP, VK_LEFT, VK_RIGHT, VK_DOWN, VK_RCONTROL);
			InputSource source = inputSchemeManager.FindScheme(scheme);
			if (source != IS_NONE)
			{
				inputSchemeManager.RemoveSource(source);
			}
			else
			{
				inputSchemeManager.AddScheme(scheme);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (extendedInput.CheckXBTriggered(XB_START, i))
			{
				InputScheme scheme(i);
				InputSource source = inputSchemeManager.FindScheme(scheme);
				if (source != IS_NONE)
				{
					inputSchemeManager.RemoveSource(source);
				}
				else
				{
					inputSchemeManager.AddScheme(scheme);
				}
			}
		}

		for (auto it = playerIcons.begin(); it != playerIcons.end(); ++it)
		{
			(*it)->GetComponent<Sprite>()->SetAlpha(0.0f);
		}

		const std::vector<InputScheme>& inputSchemes = inputSchemeManager.GetInputSchemes();
		for (auto it = inputSchemes.cbegin(); it != inputSchemes.cend(); ++it)
		{
			SpriteTextMono* spriteText = playerIcons[static_cast<size_t>(it->playerID) - 1]->GetComponent<SpriteTextMono>();
			spriteText->SetAlpha(1.0f);
			spriteText->SetText("Player " + std::to_string(it->playerID) + ": " + it->GetName());
		}
		if (inputSchemes.size() < 2)
		{
			joinHint->GetComponent<Sprite>()->SetAlpha(1.0f);
			joinHint2->GetComponent<Sprite>()->SetAlpha(1.0f);
			levelSelect->button->SetMap(Levels::Map::Lobby);
		}
		else
		{
			joinHint->GetComponent<Sprite>()->SetAlpha(0.0f);
			joinHint2->GetComponent<Sprite>()->SetAlpha(0.0f);
			levelSelect->button->SetMap(Levels::Map::LevelSelect);
		}
	}

	// Unload the resources associated with Lobby.
	void Lobby::Unload()
	{
		Menu::Unload();

		std::cout << "Lobby::Unload" << std::endl;
	}
}

//----------------------------------------------------------------------------
