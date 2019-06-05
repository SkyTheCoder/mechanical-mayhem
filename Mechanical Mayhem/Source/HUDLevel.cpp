//------------------------------------------------------------------------------
//
// File Name:	HUDLevel.cpp
// Author(s):	A.J. Bussman, David Cohen (david.cohen), Daniel Walther (daniel.walther)
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
#include <sstream>
#include <iomanip>
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
#include "DimensionShiftGearAnimation.h"

#include "HUD.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Levels
{
	// Creates an instance of HUDLevel.
	HUDLevel::HUDLevel() : Level("HUDLevel"), HUDS{ nullptr }, players{ nullptr },
		dimensionSwapCountdown(nullptr), victoryText(nullptr)
	{
		HUDCamera.SetTranslation(Vector2D());
		HUDCamera.SetSize(13.0f);
	}

	// Load the resources associated with MainMenu.
	void HUDLevel::Load()
	{
		std::cout << "HUDLevel::Load" << std::endl;
	}

	// Initialize the memory associated with MainMenu.
	void HUDLevel::Initialize()
	{
		std::cout << "HUDLevel::Initialize" << std::endl;

		FindPlayers();

		// Create Player HUDs
		for (int i = 0; i < NUM_PLAYERS; i++)
		{
			if (players[i] != nullptr)
			{
				HUDS[i] = new Behaviors::HUD(players[i], HUDCamera);
				HUDS[i]->SetOwner(this);
				HUDS[i]->Initialize();
			}
		}

		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		resourceManager.GetMesh("FontAtlas", 12, 8);
		resourceManager.GetSpriteSource("Code New Roman@2x.png", 12, 8);
		resourceManager.GetSpriteSource("AniDimensionShiftGear.png", 3, 6);
		resourceManager.GetMesh("DimensionShiftGear", Vector2D(1.0f / 3, 1.0f / 6), Vector2D(0.5f, 0.5f));

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();

		// DimensionShiftGear Indicator
		BoundingRectangle screenDimensions = HUDCamera.GetScreenWorldDimensions();

		dimensionShiftGear = objectFactory.CreateObject("DimensionShiftGear", resourceManager.GetMesh("DimensionShiftGear"), resourceManager.GetSpriteSource("AniDimensionShiftGear.png"));
		dimensionShiftGear->GetComponent<Transform>()->SetTranslation(Vector2D(0.07f, screenDimensions.top - 1.48f));
		dimensionShiftGear->GetComponent<Behaviors::DimensionShiftGearAnimation>()->SetFrames(1, 9, 8);
		objectManager.AddObject(*dimensionShiftGear);

		dimensionSwapCountdown = objectFactory.CreateObject("Text", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman@2x.png"));
		dimensionSwapCountdown->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, screenDimensions.top - 1.5f));
		dimensionSwapCountdown->GetComponent<Transform>()->SetScale(Vector2D(0.75f, 0.75f));
		objectManager.AddObject(*dimensionSwapCountdown);

		victoryText = objectFactory.CreateObject("Text", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman@2x.png"));
		victoryText->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, -1.5f));
		victoryText->GetComponent<Transform>()->SetScale(Vector2D(1.0f, 1.0f));
		objectManager.AddObject(*victoryText);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void HUDLevel::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		HUDCamera.Use();

		// End game if a player dies
		GameObjectManager& altObjectManager = GetAltSpace()->GetObjectManager();
		unsigned playerCount = altObjectManager.GetObjectCount("Player");
		if (playerCount == 1)
		{
			GameObject* lastPlayer = altObjectManager.GetObjectByName("Player");
			Behaviors::PlayerMovement* lastPlayerMovement = static_cast<Behaviors::PlayerMovement*>(lastPlayer->GetComponent("PlayerMovement"));

			// Set text to winText
			SpriteTextMono* spriteText = victoryText->GetComponent<SpriteTextMono>();
			switch (lastPlayerMovement->GetPlayerID())
			{
			case 1:
				spriteText->SetText("Jerry won! Press <SPACE> or start to return to level select");
				break;
			case 2:
				spriteText->SetText("Chad won! Press <SPACE> or start to return to level select");
				break;
			case 3:
				spriteText->SetText("Jared won! Press <SPACE> or start to return to level select");
				break;
			case 4:
				spriteText->SetText("Charles won! Press <SPACE> or start to return to level select");
				break;
			case 5:
				spriteText->SetText("Jimbo won! Press <SPACE> or start to return to level select");
				break;
			case 6:
				spriteText->SetText("Vlad won! Press <SPACE> or start to return to level select");
				break;
			}

			Color colors[] = { HexColorRGB(0xF05555), HexColorRGB(0x5755F0), HexColorRGB(0x17AB28), HexColorRGB(0xDDB61F), HexColorRGB(0xE83FE4), HexColorRGB(0xE57207) };
			spriteText->SetColor(colors[lastPlayerMovement->GetPlayerID() - 1]);
		}

		float switchCooldown = altObjectManager.GetObjectByName("GameController")->GetComponent<Behaviors::DimensionController>()->GetCoolDown();

		std::stringstream cooldownText;
		cooldownText << std::fixed << std::setprecision(1) << ceil(switchCooldown * 10.0f) / 10.0f;
		dimensionSwapCountdown->GetComponent<SpriteTextMono>()->SetText(cooldownText.str());

		for (int i = 0; i < NUM_PLAYERS; i++)
		{
			if (HUDS[i] != nullptr)
				HUDS[i]->Update(dt);
		}
	}

	// Removes any objects that will be recreated in Initialize.
	void HUDLevel::Shutdown()
	{
		for (int i = 0; i < NUM_PLAYERS; i++)
		{
			if (HUDS[i] != nullptr)
				HUDS[i]->Shutdown();
			delete HUDS[i];
		}
	}

	// Unload the resources associated with MainMenu.
	void HUDLevel::Unload()
	{
		std::cout << "HUDLevel::Unload" << std::endl;
	}

	// Finds the current player pointers.
	void HUDLevel::FindPlayers()
	{
		for (int i = 0; i < NUM_PLAYERS; i++)
			players[i] = nullptr;

		// Set Player pointers
		if (GetAltSpace() != nullptr)
		{
			std::vector<GameObject*> playerObjects;
			playerObjects.reserve(2);
			GetAltSpace()->GetObjectManager().GetAllObjectsByName("Player", playerObjects);

			for (auto it = playerObjects.begin(); it != playerObjects.end(); ++it)
			{
				Behaviors::PlayerMovement* playerMovement = (*it)->GetComponent<Behaviors::PlayerMovement>();
				if (playerMovement != nullptr)
				{
					players[playerMovement->GetPlayerID() - 1] = *it;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
