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
#include <Graphics.h>
#include <Camera.h>

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

		FixCamera();
		BoundingRectangle screenDimensions = Graphics::GetInstance().GetDefaultCamera().GetScreenWorldDimensions();
		float aspectRatio = screenDimensions.extents.x / screenDimensions.extents.y;
		GameObject* fullScreenBackground = new GameObject(*objectManager.GetArchetypeByName("FullScreenBackground"));
		if (aspectRatio < 16.0f / 9.0f)
			fullScreenBackground->GetComponent<Transform>()->SetScale(Vector2D(16.0f / 9.0f * screenDimensions.extents.y * 2.0f, screenDimensions.extents.y * 2.0f));
		else
			fullScreenBackground->GetComponent<Transform>()->SetScale(Vector2D(screenDimensions.extents.x * 2.0f, screenDimensions.extents.x * 2.0f / (16.0f / 9.0f)));
		objectManager.AddObject(*fullScreenBackground);

		// Create and add descriptive text
		GameObject* title = new GameObject(*objectManager.GetArchetypeByName("Text"));
		title->GetComponent<SpriteTextMono>()->SetText("Lobby");
		title->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, 2.5f));
		objectManager.AddObject(*title);

		MenuButton* mainMenu = AddMenuButton("Main Menu", Vector2D(-1.75f, -2.5f), Map::MainMenu);
		levelSelect = AddMenuButton("Level Select", Vector2D(1.75f, -2.5f), Map::LevelSelect);

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

		// Create and color the sprite text used to display players in the lobby.
		Color colors[] = { HexColorRGB(0xF05555), HexColorRGB(0x5755F0), HexColorRGB(0x17AB28), HexColorRGB(0xDDB61F), HexColorRGB(0xE83FE4), HexColorRGB(0xC01818) };

		for (int i = 0; i < NUM_PLAYERS; i++)
		{
			GameObject* text = new GameObject(*objectManager.GetArchetypeByName("Text"));
			text->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, 1.5f - i * 0.5f));
			text->GetComponent<Sprite>()->SetColor(colors[i]);
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

		// Handle players joining/leaving via CTRL & START.

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

		// Display players in the lobby.

		for (auto it = playerIcons.begin(); it != playerIcons.end(); ++it)
		{
			(*it)->GetComponent<Sprite>()->SetAlpha(0.0f);
		}

		const char* playerNames[] = {
			"Jerry",
			"Chad",
			"Jared",
			"Charles",
			"Jimbo",
			"Vlad"
		};

		const std::vector<InputScheme>& inputSchemes = inputSchemeManager.GetInputSchemes();
		for (auto it = inputSchemes.cbegin(); it != inputSchemes.cend(); ++it)
		{
			SpriteTextMono* spriteText = playerIcons[static_cast<size_t>(it->playerID) - 1]->GetComponent<SpriteTextMono>();
			spriteText->SetAlpha(1.0f);
			spriteText->SetText(std::string(playerNames[it->playerID - 1]) + ": " + it->GetName());
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
