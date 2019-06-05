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
#include <GameObject.h>
#include "Archetypes.h"
#include <Space.h>
#include <SoundManager.h>
#include <Input.h>
#include <Mesh.h>
#include <MeshHelper.h>
#include <Texture.h>
#include <SpriteSource.h>
#include <Interpolation.h>
#include <Random.h>

// Components
#include <Transform.h>
#include <Physics.h>
#include <SpriteTextMono.h>
#include "Button.h"

// Levels
#include "LevelSelect.h"
#include "Level1.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Levels
{
	// Creates an instance of MainMenu
	// Params:
	//   fastIntro = Whether to speed up the into (already played).
	MainMenu::MainMenu(bool fastIntro)
		: Level("MainMenu"), timer(0.0f), nextSpriteSpawn(5.0f), fastIntro(fastIntro), backgroundOverlay(nullptr), title(nullptr)
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
		resourceManager.GetMesh("AniMainMenu", Vector2D(1.0f / 2, 1.0f / 2), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("Player", Vector2D(1.0f / 3, 1.0f / 10), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("PlayerJetpack", Vector2D(1.0f / 2, 1.0f / 2), Vector2D(0.5f, 0.5f));
		resourceManager.GetSpriteSource("AniMainMenu.png", 2, 2);
		resourceManager.GetSpriteSource("Button.png");
		resourceManager.GetSpriteSource("ArtMechanicalMayhemTitleScreen.png");

		const char* spriteSuffixes[] = { "A", "B", "C", "D", "E", "F" };

		for (int i = 0; i < NUM_PLAYERS; i++)
		{
			std::string spriteSuffix = spriteSuffixes[i];
			resourceManager.GetSpriteSource("Ani" + spriteSuffix + ".png", 3, 10);
			resourceManager.GetSpriteSource("AniJetpack" + spriteSuffix + ".png", 3, 10);
			resourceManager.GetSpriteSource("AniFlame" + spriteSuffix + ".png", 3, 10);
			resourceManager.GetSpriteSource("AniMine" + spriteSuffix + ".png", 3, 10);
			resourceManager.GetSpriteSource("AniJetpackFallFireActive" + spriteSuffix + ".png", 2, 2);
		}

		resourceManager.GetMesh("FontAtlas", 12, 8);
		resourceManager.GetSpriteSource("Code New Roman@2x.png", 12, 8);

		objectManager.AddArchetype(*objectFactory.CreateObject("FullScreenBackground", resourceManager.GetMesh("AniMainMenu"), resourceManager.GetSpriteSource("AniMainMenu.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("FullScreenImage", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("ArtMechanicalMayhemTitleScreen.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Button", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Button.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Text", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman@2x.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("MainMenuWalkingSprite", resourceManager.GetMesh("Player"), resourceManager.GetSpriteSource("AniA.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("MainMenuJumpingSprite", resourceManager.GetMesh("Player"), resourceManager.GetSpriteSource("AniA.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("MainMenuFallingSprite", resourceManager.GetMesh("Player"), resourceManager.GetSpriteSource("AniA.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("MainMenuWallSlideSprite", resourceManager.GetMesh("Player"), resourceManager.GetSpriteSource("AniA.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("MainMenuJetpackSprite", resourceManager.GetMesh("PlayerJetpack"), resourceManager.GetSpriteSource("AniJetpackFallFireActiveA.png")));
	}

	// Initialize the memory associated with MainMenu.
	void MainMenu::Initialize()
	{
		std::cout << "MainMenu::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		objectManager.AddObject(*new GameObject(*objectManager.GetArchetypeByName("FullScreenBackground")));

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
		lobby->east = credits;
		lobby->south = controls;
		lobby->west = controls;
		exit->north = controls;
		exit->east = credits;
		exit->south = lobby;
		exit->west = controls;

		SetDefaultButton(lobby);

		// Main menu music
		SoundManager* soundManager = Engine::GetInstance().GetModule<SoundManager>();
		if (soundManager->GetMusicChannel() == nullptr)
			soundManager->PlaySound("SoundMenuM.wav");

		backgroundOverlay = new GameObject(*objectManager.GetArchetypeByName("FullScreenBackground"));
		objectManager.AddObject(*backgroundOverlay);
		title = new GameObject(*objectManager.GetArchetypeByName("FullScreenImage"));
		objectManager.AddObject(*title);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void MainMenu::Update(float dt)
	{
		FixCamera();

		Menu::Update(dt);

		timer += dt;

		float pauseTime = fastIntro ? 0.0f : 1.0f;
		float transitionTime = fastIntro ? 0.5f : 1.0f;

		// Handle the intro transition.
		if (timer < pauseTime)
		{
			backgroundOverlay->GetComponent<Sprite>()->SetAlpha(1.0f);
			title->GetComponent<Transform>()->SetTranslation(Vector2D());
			title->GetComponent<Transform>()->SetScale(Vector2D(16.0f, 9.0f));
		}
		else if (timer < pauseTime + transitionTime)
		{
			float lerp = pow((timer - pauseTime) / transitionTime, 2.0f);
			backgroundOverlay->GetComponent<Sprite>()->SetAlpha(1.0f - lerp);
			title->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, lerp * 0.5f));
			title->GetComponent<Transform>()->SetScale(Interpolate(Vector2D(16.0f, 9.0f), Vector2D(12.0f, 6.75f), lerp));
		}
		else
		{
			backgroundOverlay->GetComponent<Sprite>()->SetAlpha(0.0f);
			title->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, 0.5f));
			title->GetComponent<Transform>()->SetScale(Vector2D(12.0f, 6.75f));
		}

		// Periodically spawn sprites on the main menu.
		if (timer >= nextSpriteSpawn)
		{
			SpawnSprite();
			nextSpriteSpawn = timer + RandomRange(2.0f, 5.0f);
		}
	}

	// Unload the resources associated with MainMenu.
	void MainMenu::Unload()
	{
		Menu::Unload();

		std::cout << "MainMenu::Unload" << std::endl;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Spawns a new random sprite to run around the menu.
	void MainMenu::SpawnSprite()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		GameObject* sprite = nullptr;
		Vector2D position;

		int type = RandomRange(0, 6);
		bool spawnChoice = RandomRange(0, 1) == 1;
		float offset = RandomRange(0.0f, 2.0f);
		bool flipVelX = false;
		bool flipVelY = false;
		bool randomizeSprite = true;

		switch (type)
		{
		case 0:
		case 1:
		case 2:
			sprite = new GameObject(*objectManager.GetArchetypeByName("MainMenuWalkingSprite"));
			
			position = spawnChoice ? Vector2D(9.0f, -3.5f) : Vector2D(-9.0f, -3.5f);
			flipVelX = spawnChoice;

			break;
		case 3:
			sprite = new GameObject(*objectManager.GetArchetypeByName("MainMenuJumpingSprite"));

			position = spawnChoice ? Vector2D(7.0f - offset, -6.5f) : Vector2D(-7.0f + offset, -6.5f);
			flipVelX = spawnChoice;

			break;
		case 4:
			sprite = new GameObject(*objectManager.GetArchetypeByName("MainMenuFallingSprite"));

			position = spawnChoice ? Vector2D(7.0f - offset, 5.5f) : Vector2D(-7.0f + offset, 5.5f);
			flipVelX = spawnChoice;

			break;
		case 5:
			sprite = new GameObject(*objectManager.GetArchetypeByName("MainMenuWallSlideSprite"));

			position = spawnChoice ? Vector2D(7.5f, 5.5f) : Vector2D(-7.5f, 5.5f);
			flipVelX = spawnChoice;

			break;
		case 6:
			sprite = new GameObject(*objectManager.GetArchetypeByName("MainMenuJetpackSprite"));

			position = spawnChoice ? Vector2D(7.0f - offset, -5.5f) : Vector2D(-7.0f + offset, -5.5f);
			flipVelX = spawnChoice;

			// Randomly flip the vertical direction of the jetpack sprite.
			if (RandomRange(0, 1) == 1)
			{
				position.y = -position.y;
				flipVelY = true;
			}

			// Handle sprite randomization ourselves since the jetpack sprite isn't in the normal spritesheet.
			{
				const char* spriteNames[] = {
					"AniJetpackFallFireActiveA.png",
					"AniJetpackFallFireActiveB.png",
					"AniJetpackFallFireActiveC.png",
					"AniJetpackFallFireActiveD.png",
					"AniJetpackFallFireActiveE.png",
					"AniJetpackFallFireActiveF.png"
				};
				sprite->GetComponent<Sprite>()->SetSpriteSource(resourceManager.GetSpriteSource(spriteNames[RandomRange(0, 5)]));
			}

			randomizeSprite = false;

			break;
		}

		// Randomize the character in the sprite.
		if (randomizeSprite)
		{
			const char* spriteNames[] = {
				"AniA.png",
				"AniB.png",
				"AniC.png",
				"AniD.png",
				"AniE.png",
				"AniF.png",
				"AniJetpackA.png",
				"AniJetpackB.png",
				"AniJetpackC.png",
				"AniJetpackD.png",
				"AniJetpackE.png",
				"AniJetpackF.png",
				"AniFlameA.png",
				"AniFlameB.png",
				"AniFlameC.png",
				"AniFlameD.png",
				"AniFlameE.png",
				"AniFlameF.png",
				"AniMineA.png",
				"AniMineB.png",
				"AniMineC.png",
				"AniMineD.png",
				"AniMineE.png",
				"AniMineF.png"
			};

			sprite->GetComponent<Sprite>()->SetSpriteSource(resourceManager.GetSpriteSource(spriteNames[RandomRange(0, 23)]));
		}

		Vector2D velocity = sprite->GetComponent<Physics>()->GetVelocity();
		sprite->GetComponent<Physics>()->SetVelocity(Vector2D(flipVelX ? -velocity.x : velocity.x, flipVelY ? -velocity.y : velocity.y));

		sprite->GetComponent<Transform>()->SetTranslation(position);

		if (flipVelX)
		{
			Vector2D scale = sprite->GetComponent<Transform>()->GetScale();
			sprite->GetComponent<Transform>()->SetScale(Vector2D(-scale.x, scale.y));
		}

		objectManager.AddObject(*sprite);
	}
}
//----------------------------------------------------------------------------
