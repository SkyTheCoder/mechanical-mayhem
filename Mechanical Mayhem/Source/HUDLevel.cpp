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

// Components
#include "SpriteText.h"
#include <Transform.h>
#include "Sprite.h"
#include "PlayerMovement.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

HUD::~HUD()
{
	delete PlayerIcon;
	delete HealthBar;
	delete HealthText;
	delete AbilityBar;
	delete AbilityIcon;
}

namespace Levels
{
	// Creates an instance of HUDLevel.
	HUDLevel::HUDLevel() : Level("HUDLevel"), player1(nullptr), player2(nullptr),
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

		// Set Player pointers
		if (GetAltSpace() != nullptr)
		{
			std::vector<GameObject*> players;
			players.reserve(2);
			GetAltSpace()->GetObjectManager().GetAllObjectsByName("player", players);

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

		// Create Player HUDs
		HUD1 = AddPlayerHUD("Player1", Vector2D());
		HUD2 = AddPlayerHUD("Player2", Vector2D());
	}

	// Initialize the memory associated with MainMenu.
	void HUDLevel::Initialize()
	{
		std::cout << "HUDLevel::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Test

		GameObject* test = new GameObject("Test");
		// Create a new transform.
		test->AddComponent(new Transform(Vector2D(), Vector2D(1.0f, 1.0f)));

		// Create a new sprite.
		Sprite* sprite = new Sprite();
		sprite->SetMesh(meshBackground);
		sprite->SetSpriteSource(spriteSourceBackground);
		test->AddComponent(sprite);

		objectManager.AddObject(*test);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void HUDLevel::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		HUDCamera.Use();
	}

	// Unload the resources associated with MainMenu.
	void HUDLevel::Unload()
	{
		std::cout << "HUDLevel::Unload" << std::endl;

		delete HUD1;
		delete HUD2;

		delete meshBackground;
		delete textureBackground;
		delete spriteSourceBackground;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Creates a Player's HUD
	// name = Which Player's HUD we're making.
	// position = The position of the HUD.
	HUD* HUDLevel::AddPlayerHUD(const char* name_, Vector2D position)
	{
		UNREFERENCED_PARAMETER(name_);
		UNREFERENCED_PARAMETER(position);

		// PlayerIcon
		GameObject* PlayerIcon = new GameObject("PlayerIcon");

		// HealthBar
		GameObject* HealthBar = new GameObject("HealthBar");

		// HealthText
		GameObject* HealthText = new GameObject("HealthText");

		// AbilityBar
		GameObject* AbilityBar = new GameObject("AbilityBar");

		// AbilityIcon
		GameObject* AbilityIcon = new GameObject("AbilityIcon");

		return new HUD(PlayerIcon, HealthBar, HealthText, AbilityBar, AbilityIcon);
	}
}
//----------------------------------------------------------------------------
