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

// Components
#include "SpriteText.h"
#include <Transform.h>
#include "Sprite.h"
#include "PlayerMovement.h"

#include "HUD.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

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
		HUD1 = new Behaviors::HUD(player1, Vector2D());
		HUD2 = new Behaviors::HUD(player2, Vector2D());
	}

	// Initialize the memory associated with MainMenu.
	void HUDLevel::Initialize()
	{
		std::cout << "HUDLevel::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		UNREFERENCED_PARAMETER(objectManager);
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

		delete meshBackground;
		delete textureBackground;
		delete spriteSourceBackground;
	}
}
//----------------------------------------------------------------------------
