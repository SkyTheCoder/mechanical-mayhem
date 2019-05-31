//------------------------------------------------------------------------------
//
// File Name:	HUDLevel.cpp
// Author(s):	A.J. Bussman
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
#include "HUDLevel.h"

// Systems
#include "Archetypes.h"
#include "Space.h"
#include <Input.h>
#include <Graphics.h>
#include <Camera.h>
#include <Mesh.h>
#include <MeshHelper.h>
#include <Texture.h>
#include <SpriteSource.h>
#include "GameObject.h"
#include <GameObjectFactory.h>

// Components
#include "SpriteText.h"
#include <Transform.h>
#include "Sprite.h"
#include "PlayerMovement.h"
#include <SpriteTextMono.h>
#include "DimensionController.h"

#include "HUD.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Levels
{
	// Creates an instance of HUDLevel.
	HUDLevel::HUDLevel() : Level("HUDLevel"), player1(nullptr), player2(nullptr),
		dimensionSwapCountdown(nullptr), victoryText(nullptr),
		meshBackground(nullptr), textureBackground(nullptr), spriteSourceBackground(nullptr),
		HUD1(nullptr), HUD2(nullptr)
	{
		HUDCamera.SetTranslation(Vector2D());
		HUDCamera.SetSize(10.0f);
	}

	// Load the resources associated with MainMenu.
	void HUDLevel::Load()
	{
		std::cout << "HUDLevel::Load" << std::endl;

		// Test
		// Create the mesh and sprite source for the main menu.
		meshBackground = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));
		textureBackground = Texture::CreateTextureFromFile("Spring.png");
		spriteSourceBackground = new SpriteSource(1, 1, textureBackground);
	}

	// Initialize the memory associated with MainMenu.
	void HUDLevel::Initialize()
	{
		std::cout << "HUDLevel::Initialize" << std::endl;

		FindPlayers();

		// Create Player HUDs
		HUD1 = new Behaviors::HUD(player1, HUDCamera);
		HUD1->SetOwner(this);
		HUD1->Initialize();
		HUD2 = new Behaviors::HUD(player2, HUDCamera);
		HUD2->SetOwner(this);
		HUD2->Initialize();

		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		resourceManager.GetMesh("FontAtlas", 12, 8);
		resourceManager.GetSpriteSource("Code New Roman@2x.png", 12, 8);

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();

		dimensionSwapCountdown = objectFactory.CreateObject("Text", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman@2x.png"));
		dimensionSwapCountdown->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, 4.0f));
		objectManager.AddObject(*dimensionSwapCountdown);
		victoryText = objectFactory.CreateObject("Text", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman@2x.png"));
		objectManager.AddObject(*victoryText);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void HUDLevel::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		HUDCamera.Use();

		if (player1 == nullptr || player2 == nullptr)
		{
			FindPlayers();

			HUD1->SetPlayer(player1);
			HUD2->SetPlayer(player2);
		}

		// End game if a player dies
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		GameObjectManager& altObjectManager = GetAltSpace()->GetObjectManager();
		unsigned playerCount = altObjectManager.GetObjectCount("Player");
		if (playerCount == 1)
		{
			GameObject* lastPlayer = objectManager.GetObjectByName("Player");
			Behaviors::PlayerMovement* lastPlayerMovement = static_cast<Behaviors::PlayerMovement*>(lastPlayer->GetComponent("PlayerMovement"));

			// Set text to winText
			SpriteTextMono* spriteText = victoryText->GetComponent<SpriteTextMono>();
			switch (lastPlayerMovement->GetPlayerID())
			{
			case 1:
				spriteText->SetText("Jerry won! Press <SPACE> to return to level select");
				break;
			case 2:
				spriteText->SetText("Chad won! Press <SPACE> to return to level select");
				break;
			}
		}

		float switchCooldown = altObjectManager.GetObjectByName("GameController")->GetComponent<Behaviors::DimensionController>()->GetCooldown();

		dimensionSwapCountdown->GetComponent<SpriteTextMono>()->SetText(std::to_string(std::ceil(switchCooldown)));

		HUD1->Update(dt);
		HUD2->Update(dt);
	}

	// Removes any objects that will be recreated in Initialize.
	void HUDLevel::Shutdown()
	{
		HUD1->Shutdown();
		delete HUD1;
		HUD2->Shutdown();
		delete HUD2;
	}

	// Unload the resources associated with MainMenu.
	void HUDLevel::Unload()
	{
		std::cout << "HUDLevel::Unload" << std::endl;

		delete meshBackground;
		delete textureBackground;
		delete spriteSourceBackground;
	}

	// Finds the current player pointers.
	void HUDLevel::FindPlayers()
	{
		player1 = nullptr;
		player2 = nullptr;

		// Set Player pointers
		if (GetAltSpace() != nullptr)
		{
			std::vector<GameObject*> players;
			players.reserve(2);
			GetAltSpace()->GetObjectManager().GetAllObjectsByName("Player", players);

			for (auto it = players.begin(); it != players.end(); ++it)
			{
				Behaviors::PlayerMovement* playerMovement = (*it)->GetComponent<Behaviors::PlayerMovement>();
				if (playerMovement != nullptr)
				{
					switch (playerMovement->GetPlayerID())
					{
					case 1:
						player1 = *it;
						break;
					case 2:
						player2 = *it;
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
