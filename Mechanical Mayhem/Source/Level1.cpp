//------------------------------------------------------------------------------
//
// File Name:	Level1.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Level1.h"

// Systems
#include <Engine.h>
#include "Space.h"
#include "MeshHelper.h"
#include <Mesh.h>
#include "SpriteSource.h"
#include <Texture.h>
#include <Input.h>
#include <ExtendedInput.h>
#include "Archetypes.h"
#include <glfw3.h>
#include "Tilemap.h"
#include <Graphics.h>
#include <Random.h>
#include <GameObjectFactory.h>
#include <SoundManager.h>
#include "InputSchemeManager.h"

// Components
#include "Sprite.h"
#include "Animation.h"
#include "Transform.h"
#include "Physics.h"
#include <SpriteTextMono.h>
#include "ShiftPickup.h"
#include "CameraFollow.h"
#include "PlayerMovement.h"
#include "DimensionController.h"
#include "MonkeyAnimation.h"
#include <SpriteTilemap.h>
#include <ColliderTilemap.h>
#include "AbilityHolder.h"

// Levels
#include "LevelSelect.h"
#include "HUDEmpty.h"
#include "HUDLevel.h"

// Effects
#include "ChromaticAberration.h"
#include "CameraShake.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Level 1.
	Level1::Level1(Map map_) : Level("Level1"),
		soundManager(nullptr), gearSound(nullptr),
		playWinSound(false),
		map(map_),
		backgroundImage(nullptr), shadow(nullptr),
		chromaticAberration(nullptr), cameraShake(nullptr),
		columnsMonkey(3), rowsMonkey(10),
		columnsCat(3), rowsCat(10),
		columnsSpikes(1), rowsSpikes(3),
		columnsExplosion(3), rowsExplosion(3),
		columnsFlame(2), rowsFlame(2), columnsJetpackFlame(2), rowsJetpackFlame(2),
		columnsRisingGears(1), rowsRisingGears(4),
		columnsPlayerIndicators(3), rowsPlayerIndicators(2), playerIndicators{ nullptr },
		columnsDeathAnimation(4), rowsDeathAnimation(4),
		columnsMine(2), rowsMine(2),
		columnsMinePickup(2), rowsMinePickup(2),
		columnsJetpackPickup(2), rowsJetpackPickup(2),
		columnsFlamethrowerPickup(2), rowsFlamethrowerPickup(2),
		dataStaticMap(nullptr), dataRedMap(nullptr), dataBlueMap(nullptr),
		columnsMap(2), rowsMap(2)
	{
	}

	// Load the resources associated with Level 1.
	void Level1::Load()
	{
		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		resourceManager.GetMesh("Quad", Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("Monkey", Vector2D(1.0f / columnsMonkey, 1.0f / rowsMonkey), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("Cat", Vector2D(1.0f / columnsCat, 1.0f / rowsCat), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("Spikes", Vector2D(1.0f / columnsSpikes, 1.0f / rowsSpikes), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("Map", Vector2D(1.0f / columnsMap, 1.0f / rowsMap), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("Explosion", Vector2D(1.0f / columnsExplosion, 1.0f / rowsExplosion), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("Flame", Vector2D(1.0f / columnsFlame, 1.0f / rowsFlame), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("Mine", Vector2D(1.0f / columnsMine, 1.0f / rowsMine), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("ProximityMinePickup", Vector2D(1.0f / columnsMinePickup, 1.0f / rowsMinePickup), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("JetpackPickup", Vector2D(1.0f / columnsJetpackPickup, 1.0f / rowsJetpackPickup), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("FlamethrowerPickup", Vector2D(1.0f / columnsFlamethrowerPickup, 1.0f / rowsFlamethrowerPickup), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("JetpackFlame", Vector2D(1.0f / columnsJetpackFlame, 1.0f / rowsJetpackFlame), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("RisingGears", Vector2D(1.0f / columnsRisingGears, 1.0f / rowsRisingGears), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("PlayerIndicators", Vector2D(1.0f / columnsPlayerIndicators, 1.0f / rowsPlayerIndicators), Vector2D(0.5f, 0.5f));
		resourceManager.GetMesh("DeathAnimation", Vector2D(1.0f / columnsDeathAnimation, 1.0f / rowsDeathAnimation), Vector2D(0.5f, 0.5f));

		resourceManager.GetSpriteSource("AniA.png", columnsMonkey, rowsMonkey);
		resourceManager.GetSpriteSource("AniJetpackA.png", columnsMonkey, rowsMonkey);
		resourceManager.GetSpriteSource("AniFlameA.png", columnsMonkey, rowsMonkey);
		resourceManager.GetSpriteSource("AniMineA.png", columnsMonkey, rowsMonkey);
		resourceManager.GetSpriteSource("AniB.png", columnsCat, rowsCat);
		resourceManager.GetSpriteSource("AniJetpackB.png", columnsCat, rowsCat);
		resourceManager.GetSpriteSource("AniFlameB.png", columnsCat, rowsCat);
		resourceManager.GetSpriteSource("AniMineB.png", columnsCat, rowsCat);
		resourceManager.GetSpriteSource("Spikes.png", columnsSpikes, rowsSpikes);
		resourceManager.GetSpriteSource("Spikes.png", columnsSpikes, rowsSpikes);
		resourceManager.GetSpriteSource("Spikes.png", columnsSpikes, rowsSpikes);
		resourceManager.GetSpriteSource("AniMineExplode.png", columnsExplosion, rowsExplosion);
		resourceManager.GetSpriteSource("AniMine.png", columnsMine, rowsMine);
		resourceManager.GetSpriteSource("AniFlame.png", columnsFlame, rowsFlame);
		resourceManager.GetSpriteSource("AniJetpackFallFireA.png", columnsJetpackFlame, rowsJetpackFlame);
		resourceManager.GetSpriteSource("AniJetpackFallFireB.png", columnsJetpackFlame, rowsJetpackFlame);
		resourceManager.GetSpriteSource("AniJetpackJumpFireA.png", columnsJetpackFlame, rowsJetpackFlame);
		resourceManager.GetSpriteSource("AniJetpackJumpFireB.png", columnsJetpackFlame, rowsJetpackFlame);
		resourceManager.GetSpriteSource("AniJetpackPickup.png", columnsJetpackPickup, rowsJetpackPickup);
		resourceManager.GetSpriteSource("AniFlamethrowerPickup.png", columnsFlamethrowerPickup, rowsFlamethrowerPickup);
		resourceManager.GetSpriteSource("AniMinePickup.png", columnsMinePickup, rowsMinePickup);
		resourceManager.GetSpriteSource("BackgroundImage.png");
		resourceManager.GetSpriteSource("Shadow.png");
		resourceManager.GetSpriteSource("Tilemap.png", columnsMap, rowsMap);
		resourceManager.GetSpriteSource("RisingGears.png", columnsRisingGears, rowsRisingGears);
		resourceManager.GetSpriteSource("PlayerIndicators.png", columnsPlayerIndicators, rowsPlayerIndicators);
		resourceManager.GetSpriteSource("AniDeathA.png", columnsDeathAnimation, rowsDeathAnimation);
		resourceManager.GetSpriteSource("AniDeathB.png", columnsDeathAnimation, rowsDeathAnimation);

		resourceManager.GetMesh("FontAtlas", 12, 8);
		resourceManager.GetSpriteSource("Code New Roman@2x.png", 12, 8);

		objectManager.AddArchetype(*objectFactory.CreateObject("Text", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman@2x.png")));

		// Load the tilemaps.
		std::string mapName;
		switch (map)
		{
		case Map::Tutorial:
			mapName = "Tutorial";
			break;
		case Map::Arena3:
			mapName = "Arena3";
			break;
		case Map::MediumBoy:
			mapName = "MediumBoy";
			break;
		case Map::Channels:
			mapName = "Channels";
			break;
		case Map::Separation:
			mapName = "Separation";
			break;
		case Map::Descent:
			mapName = "Descent";
			break;
		case Map::Clockwork:
			mapName = "Clockwork";
			break;
		case Map::Chase:
			mapName = "Chase";
			break;
		case Map::Merge:
			mapName = "Merge";
			break;
		case Map::Snake:
			mapName = "Snake";
			break;
		}
		
		dataStaticMap = Tilemap::CreateTilemapFromFile("Assets/Levels/" + mapName + "Static.txt");
		dataRedMap = Tilemap::CreateTilemapFromFile("Assets/Levels/" + mapName + "Red.txt");
		dataBlueMap = Tilemap::CreateTilemapFromFile("Assets/Levels/" + mapName + "Blue.txt");

		if (dataStaticMap == nullptr || dataRedMap == nullptr || dataBlueMap == nullptr)
		{
			std::cout << "Error loading map!" << std::endl;
		}

		objectManager.AddArchetype(*objectFactory.CreateObject("Player", resourceManager.GetMesh("Monkey"), resourceManager.GetSpriteSource("AniA.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("BackgroundImage", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("BackgroundImage.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Shadow", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Shadow.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("GameController"));
		objectManager.AddArchetype(*objectFactory.CreateObject("JetpackPickup", resourceManager.GetMesh("JetpackPickup"), resourceManager.GetSpriteSource("AniJetpackPickup.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("FlamethrowerPickup", resourceManager.GetMesh("FlamethrowerPickup"), resourceManager.GetSpriteSource("AniFlamethrowerPickup.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("ProximityMinePickup", resourceManager.GetMesh("ProximityMinePickup"), resourceManager.GetSpriteSource("AniMinePickup.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Flame"));
		objectManager.AddArchetype(*objectFactory.CreateObject("JetpackFlame"));
		objectManager.AddArchetype(*objectFactory.CreateObject("Mine", resourceManager.GetMesh("Mine"), resourceManager.GetSpriteSource("AniMine.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Explosion", resourceManager.GetMesh("Explosion"), resourceManager.GetSpriteSource("AniMineExplode.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("FlameEffect", resourceManager.GetMesh("Flame"), resourceManager.GetSpriteSource("AniFlame.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("JetpackFlameEffect", resourceManager.GetMesh("JetpackFlame"), resourceManager.GetSpriteSource("AniFlame.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("StaticSpike", resourceManager.GetMesh("Spikes"), resourceManager.GetSpriteSource("Spikes.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("RedSpike", resourceManager.GetMesh("Spikes"), resourceManager.GetSpriteSource("Spikes.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("BlueSpike", resourceManager.GetMesh("Spikes"), resourceManager.GetSpriteSource("Spikes.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Tilemap", resourceManager.GetMesh("Map"), resourceManager.GetSpriteSource("Tilemap.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("RisingGears", resourceManager.GetMesh("RisingGears"), resourceManager.GetSpriteSource("RisingGears.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("DeathAnimation", resourceManager.GetMesh("DeathAnimation"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("PlayerIndicators", resourceManager.GetMesh("PlayerIndicators"), resourceManager.GetSpriteSource("PlayerIndicators.png")));

		chromaticAberration = new Effects::ChromaticAberration();
		chromaticAberration->SetIntensity(2.0f);
		cameraShake = new Effects::CameraShake();

		// Set the background color.
		Graphics::GetInstance().SetBackgroundColor(Colors::Black);
	}

	// Initialize the memory associated with Level 1.
	void Level1::Initialize()
	{
		InputSchemeManager& inputSchemeManager = *Engine::GetInstance().GetModule<InputSchemeManager>();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Stop menu music and play in-game music
		soundManager = Engine::GetInstance().GetModule<SoundManager>();
		soundManager->GetMusicChannel()->stop();
		playWinSound = true;

		// Play background music
		soundManager->PlaySound("SoundPlay.wav");
		gearSound = soundManager->PlaySound("SoundGear.wav");

		// Load HUD Level
		Space* hudSpace = GetAltSpace();

		// Set initial level to the HUD level - HUDSpace.
		if (hudSpace != nullptr)
			hudSpace->SetLevel<HUDLevel>();

		// Create background sprite
		backgroundImage = new GameObject(*objectManager.GetArchetypeByName("BackgroundImage"));
		objectManager.AddObject(*backgroundImage);

		// Create the Game Controller, which handles the camera and dimensions.
		GameObject* gameController = new GameObject(*objectManager.GetArchetypeByName("GameController"));
		Behaviors::DimensionController& dimensionController = *static_cast<Behaviors::DimensionController*>(gameController->GetComponent("DimensionController"));
		objectManager.AddObject(*gameController);
		objectManager.DispatchEvent(new Event(ET_Generic, "SnapToTarget", 0.01f, GUID(), gameController->GetID()));
		Graphics::GetInstance().GetDefaultCamera().SetSize(0.0f);

		// Create the players and add them to the object manager.
		std::vector<GameObject*> players;
		for (int i = 0; i < static_cast<int>(inputSchemeManager.GetInputSchemes().size()); i++)
		{
			GameObject* player = new GameObject(*objectManager.GetArchetypeByName("Player"));
			player->GetComponent<Collider>()->SetGroup(i + 1);
			player->GetComponent<Collider>()->SetMask(CM_CREATE(i + 1));
			player->GetComponent<Behaviors::MonkeyAnimation>()->SetFrames(0, 8, 9, 4, 15, 4, 27, 2, 21, 4);
			Behaviors::PlayerMovement* playerMovement = static_cast<Behaviors::PlayerMovement*>(player->GetComponent("PlayerMovement"));
			playerMovement->SetPlayerID(i + 1);
			playerMovement->SetInputScheme(inputSchemeManager.GetPlayerScheme(i + 1));
			objectManager.AddObject(*player);
			player->GetComponent<Behaviors::MonkeyAnimation>()->GetSpriteSources(i % 2 == 0 ? "A" : "B");
			static_cast<Behaviors::CameraFollow*>(gameController->GetComponent("CameraFollow"))->AddPlayer(player);
			players.push_back(player);
		}

		if (dataStaticMap != nullptr && dataRedMap != nullptr && dataBlueMap != nullptr)
		{
			// Create the tilemap and add it to the object manager.
			GameObject* tilemapStatic = new GameObject(*objectManager.GetArchetypeByName("Tilemap"));//Archetypes::CreateTilemapObject(resourceManager.GetMesh("Tilemap"), resourceManager.GetSpriteSource("Tilemap.png"), dataStaticMap);
			tilemapStatic->GetComponent<SpriteTilemap>()->SetTilemap(dataStaticMap);
			tilemapStatic->GetComponent<ColliderTilemap>()->SetTilemap(dataStaticMap);
			objectManager.AddObject(*tilemapStatic);

			GameObject* tilemapRed = new GameObject(*objectManager.GetArchetypeByName("Tilemap"));//Archetypes::CreateTilemapObject(resourceManager.GetMesh("Tilemap"), resourceManager.GetSpriteSource("Tilemap.png"), dataRedMap);
			tilemapRed->GetComponent<SpriteTilemap>()->SetTilemap(dataRedMap);
			tilemapRed->GetComponent<ColliderTilemap>()->SetTilemap(dataRedMap);
			objectManager.AddObject(*tilemapRed);
			unsigned redDimension = dimensionController.AddDimension(tilemapRed);

			GameObject* tilemapBlue = new GameObject(*objectManager.GetArchetypeByName("Tilemap"));//Archetypes::CreateTilemapObject(resourceManager.GetMesh("Tilemap"), resourceManager.GetSpriteSource("Tilemap.png"), dataBlueMap);
			tilemapBlue->GetComponent<SpriteTilemap>()->SetTilemap(dataBlueMap);
			tilemapBlue->GetComponent<ColliderTilemap>()->SetTilemap(dataBlueMap);
			objectManager.AddObject(*tilemapBlue);
			unsigned blueDimension = dimensionController.AddDimension(tilemapBlue);

			float gearHeight = -100.0f;

			// Map data (spikes, abilities, etc.)
			// Map data automatically generated by Processing.
			// Each pair of floats in an array is a coordinate for a tile, in tilemap space.

			switch (map)
			{
			case Map::Tutorial:
			{
				float staticSpikes[84] = {
					1.0f, 12.0f, 1.0f, 13.0f, 1.0f, 14.0f, 1.0f, 15.0f, 1.0f, 16.0f, 1.0f, 17.0f, 20.0f, 17.0f, 1.0f, 18.0f, 20.0f, 18.0f, 1.0f, 19.0f, 20.0f, 19.0f, 1.0f, 20.0f, 1.0f, 21.0f, 1.0f, 22.0f, 1.0f, 23.0f, 1.0f, 24.0f,
					2.0f, 24.0f, 3.0f, 24.0f, 4.0f, 24.0f, 5.0f, 24.0f, 6.0f, 24.0f, 7.0f, 24.0f, 8.0f, 24.0f, 9.0f, 24.0f, 10.0f, 24.0f, 11.0f, 24.0f, 12.0f, 24.0f, 13.0f, 24.0f, 14.0f, 24.0f, 15.0f, 24.0f, 16.0f, 24.0f, 17.0f, 24.0f,
					18.0f, 24.0f, 28.0f, 30.0f, 29.0f, 30.0f, 30.0f, 30.0f, 34.0f, 30.0f, 35.0f, 30.0f, 36.0f, 30.0f, 40.0f, 30.0f, 41.0f, 30.0f, 42.0f, 30.0f
				};

				float redSpikes[8] = {
					23.0f, 20.0f, 33.0f, 20.0f, 23.0f, 21.0f, 33.0f, 21.0f
				};

				float blueSpikes[8] = {
					28.0f, 20.0f, 38.0f, 20.0f, 28.0f, 21.0f, 38.0f, 21.0f
				};

				float chipsSpawns[12] = {
					22.0f, 16.0f, 44.0f, 19.0f, 20.0f, 21.0f, 5.0f, 28.0f, 24.0f, 28.0f, 44.0f, 28.0f
				};

				AddStaticSpikes(staticSpikes, 42);
				AddRedSpikes(redSpikes, 4, redDimension);
				AddBlueSpikes(blueSpikes, 4, blueDimension);
				AddAbilities(chipsSpawns, 6);

				for (unsigned i = 0; i < players.size(); i++)
				{
					players[i]->GetComponent<Transform>()->SetTranslation(Vector2D(1.0f, -28.0f));
					players[i]->GetComponent<Sprite>()->SetAlpha(i == 0 ? 1.0f : 0.0f);
					players[i]->GetComponent<Behaviors::PlayerMovement>()->SetInputScheme(inputSchemeManager.GetPlayerScheme(1));
				}

				break;
			}
			case Map::Arena3:
			{
				float staticSpikes[168] = {
					13.0f, 0.0f, 14.0f, 0.0f, 15.0f, 0.0f, 29.0f, 0.0f, 30.0f, 0.0f, 31.0f, 0.0f, 33.0f, 0.0f, 34.0f, 0.0f, 35.0f, 0.0f, 48.0f, 1.0f, 48.0f, 2.0f, 48.0f, 3.0f, 4.0f, 4.0f, 5.0f, 4.0f, 6.0f, 4.0f, 7.0f, 4.0f,
					8.0f, 4.0f, 0.0f, 5.0f, 0.0f, 6.0f, 0.0f, 7.0f, 28.0f, 8.0f, 0.0f, 9.0f, 28.0f, 9.0f, 48.0f, 9.0f, 0.0f, 10.0f, 28.0f, 10.0f, 48.0f, 10.0f, 0.0f, 11.0f, 28.0f, 11.0f, 48.0f, 11.0f, 16.0f, 12.0f, 16.0f, 13.0f,
					16.0f, 14.0f, 16.0f, 15.0f, 37.0f, 16.0f, 38.0f, 16.0f, 39.0f, 16.0f, 1.0f, 24.0f, 2.0f, 24.0f, 3.0f, 24.0f, 4.0f, 24.0f, 5.0f, 24.0f, 6.0f, 24.0f, 7.0f, 24.0f, 8.0f, 24.0f, 9.0f, 24.0f, 10.0f, 24.0f, 11.0f, 24.0f,
					12.0f, 24.0f, 13.0f, 24.0f, 14.0f, 24.0f, 15.0f, 24.0f, 16.0f, 24.0f, 17.0f, 24.0f, 18.0f, 24.0f, 19.0f, 24.0f, 20.0f, 24.0f, 21.0f, 24.0f, 22.0f, 24.0f, 23.0f, 24.0f, 24.0f, 24.0f, 25.0f, 24.0f, 26.0f, 24.0f, 27.0f, 24.0f,
					28.0f, 24.0f, 29.0f, 24.0f, 30.0f, 24.0f, 31.0f, 24.0f, 32.0f, 24.0f, 33.0f, 24.0f, 34.0f, 24.0f, 35.0f, 24.0f, 36.0f, 24.0f, 37.0f, 24.0f, 38.0f, 24.0f, 39.0f, 24.0f, 40.0f, 24.0f, 41.0f, 24.0f, 42.0f, 24.0f, 43.0f, 24.0f,
					44.0f, 24.0f, 45.0f, 24.0f, 46.0f, 24.0f, 47.0f, 24.0f
				};

				float redSpikes[94] = {
					12.0f, 1.0f, 40.0f, 1.0f, 12.0f, 2.0f, 40.0f, 2.0f, 12.0f, 3.0f, 40.0f, 3.0f, 9.0f, 4.0f, 10.0f, 4.0f, 11.0f, 4.0f, 12.0f, 4.0f, 29.0f, 4.0f, 30.0f, 4.0f, 31.0f, 4.0f, 32.0f, 4.0f, 33.0f, 4.0f, 34.0f, 4.0f,
					35.0f, 4.0f, 1.0f, 8.0f, 2.0f, 8.0f, 3.0f, 8.0f, 8.0f, 8.0f, 45.0f, 8.0f, 46.0f, 8.0f, 47.0f, 8.0f, 8.0f, 9.0f, 8.0f, 10.0f, 8.0f, 11.0f, 17.0f, 12.0f, 18.0f, 12.0f, 19.0f, 12.0f, 4.0f, 13.0f, 40.0f, 13.0f,
					4.0f, 14.0f, 40.0f, 14.0f, 4.0f, 15.0f, 40.0f, 15.0f, 40.0f, 16.0f, 20.0f, 17.0f, 20.0f, 18.0f, 20.0f, 19.0f, 1.0f, 20.0f, 2.0f, 20.0f, 3.0f, 20.0f, 20.0f, 20.0f, 21.0f, 20.0f, 22.0f, 20.0f, 23.0f, 20.0f
				};

				float blueSpikes[72] = {
					24.0f, 1.0f, 24.0f, 2.0f, 24.0f, 3.0f, 24.0f, 4.0f, 20.0f, 5.0f, 40.0f, 5.0f, 20.0f, 6.0f, 40.0f, 6.0f, 20.0f, 7.0f, 40.0f, 7.0f, 4.0f, 8.0f, 20.0f, 8.0f, 4.0f, 9.0f, 4.0f, 10.0f, 4.0f, 11.0f, 4.0f, 12.0f,
					36.0f, 12.0f, 36.0f, 13.0f, 36.0f, 14.0f, 36.0f, 15.0f, 45.0f, 16.0f, 46.0f, 16.0f, 47.0f, 16.0f, 28.0f, 17.0f, 32.0f, 17.0f, 28.0f, 18.0f, 32.0f, 18.0f, 28.0f, 19.0f, 32.0f, 19.0f, 8.0f, 20.0f, 9.0f, 20.0f, 10.0f, 20.0f,
					11.0f, 20.0f, 12.0f, 20.0f, 28.0f, 20.0f, 32.0f, 20.0f
				};

				float chipsSpawns[16] = {
					10.0f, 2.0f, 30.0f, 2.0f, 2.0f, 6.0f, 42.0f, 6.0f, 18.0f, 10.0f, 26.0f, 10.0f, 6.0f, 18.0f, 38.0f, 18.0f
				};

				AddStaticSpikes(staticSpikes, 84);
				AddRedSpikes(redSpikes, 47, redDimension);
				AddBlueSpikes(blueSpikes, 36, blueDimension);
				AddAbilities(chipsSpawns, 8);

				for (unsigned i = 0; i < players.size(); i++)
				{
					players[i]->GetComponent<Transform>()->SetTranslation(i % 2 == 0 ? Vector2D(6.0f, -15.0f) : Vector2D(42.0f, -15.0f));
				}

				break;
			}
			case Map::MediumBoy:
			{
				float staticSpikes[60] = {
					1.0f, 31.0f, 2.0f, 31.0f, 3.0f, 31.0f, 4.0f, 31.0f, 5.0f, 31.0f, 6.0f, 31.0f, 7.0f, 31.0f, 8.0f, 31.0f, 9.0f, 31.0f, 10.0f, 31.0f, 11.0f, 31.0f, 12.0f, 31.0f, 13.0f, 31.0f, 14.0f, 31.0f, 15.0f, 31.0f, 16.0f, 31.0f,
					17.0f, 31.0f, 18.0f, 31.0f, 19.0f, 31.0f, 20.0f, 31.0f, 21.0f, 31.0f, 22.0f, 31.0f, 23.0f, 31.0f, 24.0f, 31.0f, 25.0f, 31.0f, 26.0f, 31.0f, 27.0f, 31.0f, 28.0f, 31.0f, 29.0f, 31.0f, 30.0f, 31.0f
				};

				float redSpikes[66] = {
					26.0f, 8.0f, 27.0f, 8.0f, 28.0f, 8.0f, 29.0f, 8.0f, 30.0f, 8.0f, 1.0f, 13.0f, 2.0f, 13.0f, 3.0f, 13.0f, 4.0f, 13.0f, 5.0f, 13.0f, 30.0f, 14.0f, 30.0f, 15.0f, 30.0f, 16.0f, 30.0f, 17.0f, 14.0f, 18.0f, 15.0f, 18.0f,
					30.0f, 18.0f, 30.0f, 19.0f, 30.0f, 20.0f, 28.0f, 21.0f, 29.0f, 21.0f, 30.0f, 21.0f, 30.0f, 22.0f, 16.0f, 23.0f, 17.0f, 23.0f, 30.0f, 23.0f, 30.0f, 24.0f, 30.0f, 25.0f, 30.0f, 26.0f, 30.0f, 27.0f, 30.0f, 28.0f, 30.0f, 29.0f,
					30.0f, 30.0f
				};

				float blueSpikes[66] = {
					1.0f, 8.0f, 2.0f, 8.0f, 3.0f, 8.0f, 4.0f, 8.0f, 5.0f, 8.0f, 26.0f, 13.0f, 27.0f, 13.0f, 28.0f, 13.0f, 29.0f, 13.0f, 30.0f, 13.0f, 1.0f, 14.0f, 1.0f, 15.0f, 1.0f, 16.0f, 1.0f, 17.0f, 1.0f, 18.0f, 16.0f, 18.0f,
					17.0f, 18.0f, 1.0f, 19.0f, 1.0f, 20.0f, 1.0f, 21.0f, 2.0f, 21.0f, 3.0f, 21.0f, 1.0f, 22.0f, 1.0f, 23.0f, 14.0f, 23.0f, 15.0f, 23.0f, 1.0f, 24.0f, 1.0f, 25.0f, 1.0f, 26.0f, 1.0f, 27.0f, 1.0f, 28.0f, 1.0f, 29.0f,
					1.0f, 30.0f
				};

				float chipsSpawns[16] = {
					4.0f, 6.0f, 27.0f, 6.0f, 2.0f, 16.0f, 29.0f, 16.0f, 10.0f, 21.0f, 21.0f, 21.0f, 2.0f, 28.0f, 29.0f, 28.0f
				};

				AddStaticSpikes(staticSpikes, 30);
				AddRedSpikes(redSpikes, 33, redDimension);
				AddBlueSpikes(blueSpikes, 33, blueDimension);
				AddAbilities(chipsSpawns, 8);

				for (unsigned i = 0; i < players.size(); i++)
				{
					players[i]->GetComponent<Transform>()->SetTranslation(i % 2 == 0 ? Vector2D(9.0f, -3.0f) : Vector2D(22.0f, -2.0f));
				}

				break;
			}
			case Map::Channels:
			{
				float staticSpikes[68] = {
					10.0f, 9.0f, 21.0f, 9.0f, 5.0f, 17.0f, 26.0f, 17.0f, 1.0f, 31.0f, 2.0f, 31.0f, 3.0f, 31.0f, 4.0f, 31.0f, 5.0f, 31.0f, 6.0f, 31.0f, 7.0f, 31.0f, 8.0f, 31.0f, 9.0f, 31.0f, 10.0f, 31.0f, 11.0f, 31.0f, 12.0f, 31.0f,
					13.0f, 31.0f, 14.0f, 31.0f, 15.0f, 31.0f, 16.0f, 31.0f, 17.0f, 31.0f, 18.0f, 31.0f, 19.0f, 31.0f, 20.0f, 31.0f, 21.0f, 31.0f, 22.0f, 31.0f, 23.0f, 31.0f, 24.0f, 31.0f, 25.0f, 31.0f, 26.0f, 31.0f, 27.0f, 31.0f, 28.0f, 31.0f,
					29.0f, 31.0f, 30.0f, 31.0f
				};

				float redSpikes[42] = {
					22.0f, 8.0f, 22.0f, 9.0f, 22.0f, 10.0f, 22.0f, 11.0f, 11.0f, 12.0f, 12.0f, 12.0f, 22.0f, 12.0f, 11.0f, 13.0f, 12.0f, 13.0f, 22.0f, 13.0f, 22.0f, 14.0f, 22.0f, 15.0f, 22.0f, 16.0f, 22.0f, 17.0f, 22.0f, 18.0f, 22.0f, 19.0f,
					22.0f, 20.0f, 22.0f, 21.0f, 22.0f, 22.0f, 22.0f, 23.0f, 22.0f, 24.0f
				};

				float blueSpikes[42] = {
					9.0f, 8.0f, 9.0f, 9.0f, 9.0f, 10.0f, 9.0f, 11.0f, 9.0f, 12.0f, 19.0f, 12.0f, 20.0f, 12.0f, 9.0f, 13.0f, 19.0f, 13.0f, 20.0f, 13.0f, 9.0f, 14.0f, 9.0f, 15.0f, 9.0f, 16.0f, 9.0f, 17.0f, 9.0f, 18.0f, 9.0f, 19.0f,
					9.0f, 20.0f, 9.0f, 21.0f, 9.0f, 22.0f, 9.0f, 23.0f, 9.0f, 24.0f
				};

				float chipsSpawns[8] = {
					6.0f, 6.0f, 25.0f, 6.0f, 4.0f, 21.0f, 27.0f, 21.0f
				};

				AddStaticSpikes(staticSpikes, 34);
				AddRedSpikes(redSpikes, 21, redDimension);
				AddBlueSpikes(blueSpikes, 21, blueDimension);
				AddAbilities(chipsSpawns, 4);

				for (unsigned i = 0; i < players.size(); i++)
				{
					players[i]->GetComponent<Transform>()->SetTranslation(i % 2 == 0 ? Vector2D(6.0f, -26.0f) : Vector2D(23.0f, -27.0f));
				}

				break;
			}
			case Map::Separation:
			{
				float staticSpikes[64] = {
					15.0f, 1.0f, 16.0f, 1.0f, 1.0f, 31.0f, 2.0f, 31.0f, 3.0f, 31.0f, 4.0f, 31.0f, 5.0f, 31.0f, 6.0f, 31.0f, 7.0f, 31.0f, 8.0f, 31.0f, 9.0f, 31.0f, 10.0f, 31.0f, 11.0f, 31.0f, 12.0f, 31.0f, 13.0f, 31.0f, 14.0f, 31.0f,
					15.0f, 31.0f, 16.0f, 31.0f, 17.0f, 31.0f, 18.0f, 31.0f, 19.0f, 31.0f, 20.0f, 31.0f, 21.0f, 31.0f, 22.0f, 31.0f, 23.0f, 31.0f, 24.0f, 31.0f, 25.0f, 31.0f, 26.0f, 31.0f, 27.0f, 31.0f, 28.0f, 31.0f, 29.0f, 31.0f, 30.0f, 31.0f
				};

				float redSpikes[16] = {
					10.0f, 15.0f, 11.0f, 15.0f, 20.0f, 15.0f, 21.0f, 15.0f, 10.0f, 16.0f, 11.0f, 16.0f, 20.0f, 16.0f, 21.0f, 16.0f
				};

				float blueSpikes[32] = {
					3.0f, 15.0f, 28.0f, 15.0f, 3.0f, 16.0f, 28.0f, 16.0f, 3.0f, 17.0f, 28.0f, 17.0f, 3.0f, 18.0f, 28.0f, 18.0f, 3.0f, 21.0f, 28.0f, 21.0f, 3.0f, 22.0f, 28.0f, 22.0f, 3.0f, 23.0f, 28.0f, 23.0f, 3.0f, 24.0f, 28.0f, 24.0f
				};

				float chipsSpawns[8] = {
					11.0f, 11.0f, 20.0f, 11.0f, 5.0f, 20.0f, 26.0f, 20.0f
				};

				AddStaticSpikes(staticSpikes, 32);
				AddRedSpikes(redSpikes, 8, redDimension);
				AddBlueSpikes(blueSpikes, 16, blueDimension);
				AddAbilities(chipsSpawns, 4);

				for (unsigned i = 0; i < players.size(); i++)
				{
					players[i]->GetComponent<Transform>()->SetTranslation(i % 2 == 0 ? Vector2D(3.0f, -2.0f) : Vector2D(28.0f, -2.0f));
				}

				break;
			}
			case Map::Descent:
			{
				float staticSpikes[144] = {
					3.0f, 10.0f, 40.0f, 10.0f, 2.0f, 11.0f, 41.0f, 11.0f, 1.0f, 12.0f, 42.0f, 12.0f, 0.0f, 16.0f, 1.0f, 16.0f, 2.0f, 16.0f, 3.0f, 16.0f, 4.0f, 16.0f, 39.0f, 16.0f, 40.0f, 16.0f, 41.0f, 16.0f, 42.0f, 16.0f, 43.0f, 16.0f,
					0.0f, 17.0f, 1.0f, 17.0f, 2.0f, 17.0f, 41.0f, 17.0f, 42.0f, 17.0f, 43.0f, 17.0f, 0.0f, 18.0f, 1.0f, 18.0f, 42.0f, 18.0f, 43.0f, 18.0f, 0.0f, 19.0f, 43.0f, 19.0f, 0.0f, 25.0f, 43.0f, 25.0f, 0.0f, 26.0f, 43.0f, 26.0f,
					0.0f, 27.0f, 43.0f, 27.0f, 0.0f, 28.0f, 43.0f, 28.0f, 0.0f, 29.0f, 43.0f, 29.0f, 0.0f, 30.0f, 43.0f, 30.0f, 0.0f, 31.0f, 43.0f, 31.0f, 0.0f, 32.0f, 43.0f, 32.0f, 0.0f, 33.0f, 43.0f, 33.0f, 0.0f, 34.0f, 43.0f, 34.0f,
					0.0f, 35.0f, 43.0f, 35.0f, 0.0f, 36.0f, 43.0f, 36.0f, 0.0f, 37.0f, 43.0f, 37.0f, 0.0f, 38.0f, 43.0f, 38.0f, 0.0f, 39.0f, 43.0f, 39.0f, 0.0f, 40.0f, 43.0f, 40.0f, 0.0f, 41.0f, 43.0f, 41.0f, 0.0f, 42.0f, 43.0f, 42.0f,
					0.0f, 43.0f, 43.0f, 43.0f, 0.0f, 44.0f, 43.0f, 44.0f, 0.0f, 45.0f, 43.0f, 45.0f, 0.0f, 46.0f, 43.0f, 46.0f
				};
				float redSpikes[150] = {
					23.0f, 12.0f, 24.0f, 12.0f, 25.0f, 12.0f, 26.0f, 12.0f, 27.0f, 12.0f, 28.0f, 12.0f, 29.0f, 12.0f, 30.0f, 12.0f, 31.0f, 12.0f, 32.0f, 12.0f, 33.0f, 12.0f, 34.0f, 12.0f, 35.0f, 12.0f, 36.0f, 12.0f, 37.0f, 12.0f, 38.0f, 12.0f,
					39.0f, 12.0f, 28.0f, 13.0f, 29.0f, 13.0f, 30.0f, 13.0f, 31.0f, 13.0f, 32.0f, 13.0f, 33.0f, 13.0f, 34.0f, 13.0f, 35.0f, 13.0f, 36.0f, 13.0f, 37.0f, 13.0f, 38.0f, 13.0f, 39.0f, 13.0f, 40.0f, 13.0f, 33.0f, 14.0f, 34.0f, 14.0f,
					35.0f, 14.0f, 36.0f, 14.0f, 37.0f, 14.0f, 38.0f, 14.0f, 39.0f, 14.0f, 40.0f, 14.0f, 41.0f, 14.0f, 38.0f, 15.0f, 39.0f, 15.0f, 40.0f, 15.0f, 41.0f, 15.0f, 42.0f, 15.0f, 42.0f, 20.0f, 41.0f, 21.0f, 41.0f, 22.0f, 41.0f, 23.0f,
					42.0f, 24.0f, 13.0f, 34.0f, 15.0f, 34.0f, 17.0f, 34.0f, 33.0f, 34.0f, 35.0f, 34.0f, 14.0f, 35.0f, 16.0f, 35.0f, 34.0f, 35.0f, 4.0f, 41.0f, 5.0f, 41.0f, 6.0f, 41.0f, 8.0f, 41.0f, 9.0f, 41.0f, 24.0f, 41.0f, 25.0f, 41.0f,
					26.0f, 41.0f, 28.0f, 41.0f, 29.0f, 41.0f, 3.0f, 42.0f, 7.0f, 42.0f, 23.0f, 42.0f, 27.0f, 42.0f, 3.0f, 43.0f, 7.0f, 43.0f, 23.0f, 43.0f, 27.0f, 43.0f
				};
				float blueSpikes[148] = {
					4.0f, 12.0f, 5.0f, 12.0f, 6.0f, 12.0f, 7.0f, 12.0f, 8.0f, 12.0f, 9.0f, 12.0f, 10.0f, 12.0f, 11.0f, 12.0f, 12.0f, 12.0f, 13.0f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f, 16.0f, 12.0f, 17.0f, 12.0f, 18.0f, 12.0f, 19.0f, 12.0f,
					20.0f, 12.0f, 3.0f, 13.0f, 4.0f, 13.0f, 5.0f, 13.0f, 6.0f, 13.0f, 7.0f, 13.0f, 8.0f, 13.0f, 9.0f, 13.0f, 10.0f, 13.0f, 11.0f, 13.0f, 12.0f, 13.0f, 13.0f, 13.0f, 14.0f, 13.0f, 15.0f, 13.0f, 2.0f, 14.0f, 3.0f, 14.0f,
					4.0f, 14.0f, 5.0f, 14.0f, 6.0f, 14.0f, 7.0f, 14.0f, 8.0f, 14.0f, 9.0f, 14.0f, 10.0f, 14.0f, 1.0f, 15.0f, 2.0f, 15.0f, 3.0f, 15.0f, 4.0f, 15.0f, 5.0f, 15.0f, 1.0f, 20.0f, 2.0f, 21.0f, 2.0f, 22.0f, 2.0f, 23.0f,
					1.0f, 24.0f, 8.0f, 34.0f, 10.0f, 34.0f, 26.0f, 34.0f, 28.0f, 34.0f, 30.0f, 34.0f, 9.0f, 35.0f, 27.0f, 35.0f, 29.0f, 35.0f, 14.0f, 41.0f, 15.0f, 41.0f, 18.0f, 41.0f, 19.0f, 41.0f, 34.0f, 41.0f, 35.0f, 41.0f, 37.0f, 41.0f,
					38.0f, 41.0f, 39.0f, 41.0f, 16.0f, 42.0f, 20.0f, 42.0f, 36.0f, 42.0f, 40.0f, 42.0f, 16.0f, 43.0f, 20.0f, 43.0f, 36.0f, 43.0f, 40.0f, 43.0f
				};
				float abilities[16] = {
					1.0f, 7.0f, 42.0f, 7.0f, 3.0f, 22.0f, 40.0f, 22.0f, 15.0f, 32.0f, 28.0f, 32.0f, 6.0f, 36.0f, 37.0f, 36.0f
				};

				AddStaticSpikes(staticSpikes, 72);
				AddRedSpikes(redSpikes, 75, redDimension);
				AddBlueSpikes(blueSpikes, 74, blueDimension);
				AddAbilities(abilities, 8);

				for (unsigned i = 0; i < players.size(); i++)
				{
					players[i]->GetComponent<Transform>()->SetTranslation(i % 2 == 0 ? Vector2D(16.0f, -38.0f) : Vector2D(27.0f, -38.0f));
				}

				gearHeight = -50.0f;

				break;
			}
			case Map::Clockwork:
			{
				float staticSpikes[260] = {
					0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, 3.0f, 0.0f, 4.0f, 0.0f, 5.0f, 0.0f, 6.0f, 0.0f, 7.0f, 0.0f, 8.0f, 0.0f, 9.0f, 0.0f, 10.0f, 0.0f, 11.0f, 0.0f, 12.0f, 0.0f, 13.0f, 0.0f, 14.0f, 0.0f, 15.0f, 0.0f,
					16.0f, 0.0f, 17.0f, 0.0f, 18.0f, 0.0f, 19.0f, 0.0f, 20.0f, 0.0f, 21.0f, 0.0f, 22.0f, 0.0f, 23.0f, 0.0f, 0.0f, 1.0f, 23.0f, 1.0f, 0.0f, 2.0f, 23.0f, 2.0f, 0.0f, 3.0f, 23.0f, 3.0f, 0.0f, 4.0f, 23.0f, 4.0f,
					0.0f, 5.0f, 23.0f, 5.0f, 0.0f, 9.0f, 23.0f, 9.0f, 0.0f, 10.0f, 3.0f, 10.0f, 4.0f, 10.0f, 19.0f, 10.0f, 20.0f, 10.0f, 23.0f, 10.0f, 0.0f, 11.0f, 2.0f, 11.0f, 3.0f, 11.0f, 4.0f, 11.0f, 19.0f, 11.0f, 20.0f, 11.0f,
					21.0f, 11.0f, 23.0f, 11.0f, 0.0f, 12.0f, 1.0f, 12.0f, 2.0f, 12.0f, 3.0f, 12.0f, 20.0f, 12.0f, 21.0f, 12.0f, 22.0f, 12.0f, 23.0f, 12.0f, 0.0f, 13.0f, 1.0f, 13.0f, 2.0f, 13.0f, 21.0f, 13.0f, 22.0f, 13.0f, 23.0f, 13.0f,
					0.0f, 14.0f, 1.0f, 14.0f, 22.0f, 14.0f, 23.0f, 14.0f, 0.0f, 15.0f, 3.0f, 15.0f, 20.0f, 15.0f, 23.0f, 15.0f, 0.0f, 30.0f, 23.0f, 30.0f, 0.0f, 31.0f, 23.0f, 31.0f, 0.0f, 32.0f, 23.0f, 32.0f, 0.0f, 33.0f, 23.0f, 33.0f,
					0.0f, 34.0f, 23.0f, 34.0f, 0.0f, 35.0f, 23.0f, 35.0f, 0.0f, 36.0f, 23.0f, 36.0f, 0.0f, 37.0f, 23.0f, 37.0f, 0.0f, 38.0f, 23.0f, 38.0f, 0.0f, 39.0f, 23.0f, 39.0f, 0.0f, 40.0f, 23.0f, 40.0f, 0.0f, 41.0f, 23.0f, 41.0f,
					0.0f, 42.0f, 23.0f, 42.0f, 0.0f, 43.0f, 23.0f, 43.0f, 0.0f, 44.0f, 23.0f, 44.0f, 0.0f, 45.0f, 23.0f, 45.0f, 0.0f, 46.0f, 23.0f, 46.0f, 0.0f, 47.0f, 1.0f, 47.0f, 2.0f, 47.0f, 3.0f, 47.0f, 4.0f, 47.0f, 5.0f, 47.0f,
					6.0f, 47.0f, 7.0f, 47.0f, 8.0f, 47.0f, 9.0f, 47.0f, 10.0f, 47.0f, 11.0f, 47.0f, 12.0f, 47.0f, 13.0f, 47.0f, 14.0f, 47.0f, 15.0f, 47.0f, 16.0f, 47.0f, 17.0f, 47.0f, 18.0f, 47.0f, 19.0f, 47.0f, 20.0f, 47.0f, 21.0f, 47.0f,
					22.0f, 47.0f, 23.0f, 47.0f
				};

				float redSpikes[44] = {
					16.0f, 14.0f, 15.0f, 15.0f, 17.0f, 15.0f, 16.0f, 16.0f, 21.0f, 20.0f, 22.0f, 20.0f, 21.0f, 21.0f, 21.0f, 22.0f, 21.0f, 23.0f, 21.0f, 24.0f, 22.0f, 24.0f, 14.0f, 33.0f, 16.0f, 33.0f, 13.0f, 34.0f, 15.0f, 34.0f, 17.0f, 34.0f,
					14.0f, 35.0f, 16.0f, 35.0f, 4.0f, 41.0f, 5.0f, 41.0f, 3.0f, 42.0f, 3.0f, 43.0f
				};

				float blueSpikes[44] = {
					7.0f, 14.0f, 6.0f, 15.0f, 8.0f, 15.0f, 7.0f, 16.0f, 1.0f, 20.0f, 2.0f, 20.0f, 2.0f, 21.0f, 2.0f, 22.0f, 2.0f, 23.0f, 1.0f, 24.0f, 2.0f, 24.0f, 7.0f, 33.0f, 9.0f, 33.0f, 6.0f, 34.0f, 8.0f, 34.0f, 10.0f, 34.0f,
					7.0f, 35.0f, 9.0f, 35.0f, 18.0f, 41.0f, 19.0f, 41.0f, 20.0f, 42.0f, 20.0f, 43.0f
				};

				float chipsSpawns[20] = {
					3.0f, 5.0f, 20.0f, 5.0f, 3.0f, 22.0f, 20.0f, 22.0f, 7.0f, 28.0f, 16.0f, 28.0f, 6.0f, 37.0f, 17.0f, 37.0f, 4.0f, 39.0f, 19.0f, 39.0f
				};

				AddStaticSpikes(staticSpikes, 130);
				AddRedSpikes(redSpikes, 22, redDimension);
				AddBlueSpikes(blueSpikes, 22, blueDimension);
				AddAbilities(chipsSpawns, 10);

				for (unsigned i = 0; i < players.size(); i++)
				{
					players[i]->GetComponent<Transform>()->SetTranslation(i % 2 == 0 ? Vector2D(7.0f, -38.0f) : Vector2D(16.0f, -38.0f));
				}

				gearHeight = -48.0f;

				break;
			}
			case Map::Chase:
			{
				float staticSpikes[78] = {
					1.0f, 6.0f, 2.0f, 6.0f, 3.0f, 6.0f, 1.0f, 7.0f, 2.0f, 7.0f, 1.0f, 8.0f, 0.0f, 15.0f, 1.0f, 15.0f, 2.0f, 15.0f, 3.0f, 15.0f, 0.0f, 16.0f, 1.0f, 16.0f, 2.0f, 16.0f, 14.0f, 16.0f, 15.0f, 16.0f, 16.0f, 16.0f,
					17.0f, 16.0f, 18.0f, 16.0f, 19.0f, 16.0f, 0.0f, 17.0f, 1.0f, 17.0f, 15.0f, 20.0f, 1.0f, 29.0f, 2.0f, 29.0f, 3.0f, 29.0f, 4.0f, 29.0f, 6.0f, 31.0f, 7.0f, 31.0f, 0.0f, 36.0f, 0.0f, 37.0f, 0.0f, 38.0f, 0.0f, 39.0f,
					0.0f, 40.0f, 12.0f, 42.0f, 0.0f, 43.0f, 13.0f, 43.0f, 0.0f, 44.0f, 0.0f, 45.0f, 0.0f, 46.0f
				};
				float redSpikes[22] = {
					19.0f, 2.0f, 20.0f, 3.0f, 13.0f, 10.0f, 14.0f, 11.0f, 6.0f, 21.0f, 7.0f, 21.0f, 8.0f, 21.0f, 18.0f, 38.0f, 19.0f, 38.0f, 20.0f, 38.0f, 21.0f, 38.0f
				};
				float blueSpikes[20] = {
					4.0f, 6.0f, 5.0f, 6.0f, 6.0f, 6.0f, 7.0f, 6.0f, 20.0f, 10.0f, 19.0f, 11.0f, 12.0f, 27.0f, 13.0f, 27.0f, 14.0f, 27.0f, 15.0f, 27.0f
				};
				float abilities[10] = {
					2.0f, 12.0f, 11.0f, 21.0f, 3.0f, 27.0f, 13.0f, 35.0f, 7.0f, 38.0f
				};

				AddStaticSpikes(staticSpikes, 39);
				AddRedSpikes(redSpikes, 11, redDimension);
				AddBlueSpikes(blueSpikes, 10, blueDimension);
				AddAbilities(abilities, 5);

				for (unsigned i = 0; i < players.size(); i++)
				{
					players[i]->GetComponent<Transform>()->SetTranslation(i % 2 == 0 ? Vector2D(9.0f, -46.0f) : Vector2D(9.0f, -46.0f));
				}

				gearHeight = -64.0f;

				break;
			}
			case Map::Merge:
			{
				float staticSpikes[60] = {
					1.0f, 8.0f, 2.0f, 8.0f, 21.0f, 8.0f, 22.0f, 8.0f, 11.0f, 12.0f, 12.0f, 12.0f, 10.0f, 29.0f, 11.0f, 29.0f, 12.0f, 29.0f, 13.0f, 29.0f, 10.0f, 39.0f, 13.0f, 39.0f, 1.0f, 46.0f, 2.0f, 46.0f, 3.0f, 46.0f, 4.0f, 46.0f,
					5.0f, 46.0f, 6.0f, 46.0f, 7.0f, 46.0f, 8.0f, 46.0f, 9.0f, 46.0f, 14.0f, 46.0f, 15.0f, 46.0f, 16.0f, 46.0f, 17.0f, 46.0f, 18.0f, 46.0f, 19.0f, 46.0f, 20.0f, 46.0f, 21.0f, 46.0f, 22.0f, 46.0f
				};
				float redSpikes[16] = {
					9.0f, 14.0f, 14.0f, 14.0f, 9.0f, 15.0f, 14.0f, 15.0f, 3.0f, 22.0f, 20.0f, 22.0f, 3.0f, 23.0f, 20.0f, 23.0f
				};
				float blueSpikes[28] = {
					6.0f, 3.0f, 17.0f, 3.0f, 10.0f, 5.0f, 13.0f, 5.0f, 8.0f, 23.0f, 9.0f, 23.0f, 10.0f, 23.0f, 13.0f, 23.0f, 14.0f, 23.0f, 15.0f, 23.0f, 1.0f, 26.0f, 2.0f, 26.0f, 21.0f, 26.0f, 22.0f, 26.0f
				};
				float abilities[12] = {
					5.0f, 6.0f, 18.0f, 6.0f, 2.0f, 27.0f, 21.0f, 27.0f, 2.0f, 39.0f, 21.0f, 39.0f
				};

				AddStaticSpikes(staticSpikes, 30);
				AddRedSpikes(redSpikes, 8, redDimension);
				AddBlueSpikes(blueSpikes, 14, blueDimension);
				AddAbilities(abilities, 6);

				for (unsigned i = 0; i < players.size(); i++)
				{
					players[i]->GetComponent<Transform>()->SetTranslation(i % 2 == 0 ? Vector2D(8.0f, -41.0f) : Vector2D(15.0f, -41.0f));
				}

				gearHeight = -50.0f;

				break;
			}
			case Map::Snake:
			{
				float staticSpikes[56] = {
					1.0f, 6.0f, 2.0f, 6.0f, 3.0f, 6.0f, 1.0f, 7.0f, 2.0f, 7.0f, 1.0f, 8.0f, 7.0f, 12.0f, 8.0f, 12.0f, 9.0f, 12.0f, 10.0f, 12.0f, 22.0f, 14.0f, 16.0f, 15.0f, 17.0f, 15.0f, 22.0f, 15.0f, 22.0f, 16.0f, 22.0f, 17.0f,
					15.0f, 21.0f, 16.0f, 21.0f, 17.0f, 21.0f, 18.0f, 21.0f, 19.0f, 21.0f, 20.0f, 21.0f, 22.0f, 37.0f, 22.0f, 38.0f, 22.0f, 39.0f, 22.0f, 40.0f, 22.0f, 41.0f, 22.0f, 42.0f
				};
				float redSpikes[40] = {
					21.0f, 7.0f, 22.0f, 7.0f, 11.0f, 8.0f, 11.0f, 9.0f, 7.0f, 17.0f, 8.0f, 17.0f, 9.0f, 17.0f, 10.0f, 17.0f, 18.0f, 18.0f, 18.0f, 19.0f, 6.0f, 26.0f, 6.0f, 27.0f, 4.0f, 38.0f, 5.0f, 38.0f, 4.0f, 39.0f, 5.0f, 39.0f,
					4.0f, 40.0f, 5.0f, 40.0f, 4.0f, 41.0f, 5.0f, 41.0f
				};
				float blueSpikes[20] = {
					13.0f, 6.0f, 14.0f, 6.0f, 13.0f, 7.0f, 14.0f, 7.0f, 15.0f, 18.0f, 15.0f, 19.0f, 2.0f, 28.0f, 3.0f, 28.0f, 14.0f, 30.0f, 14.0f, 31.0f
				};
				float abilities[16] = {
					2.0f, 12.0f, 1.0f, 16.0f, 10.0f, 16.0f, 22.0f, 19.0f, 1.0f, 27.0f, 19.0f, 31.0f, 4.0f, 36.0f, 18.0f, 36.0f
				};

				AddStaticSpikes(staticSpikes, 28);
				AddRedSpikes(redSpikes, 20, redDimension);
				AddBlueSpikes(blueSpikes, 10, blueDimension);
				AddAbilities(abilities, 8);

				for (unsigned i = 0; i < players.size(); i++)
				{
					players[i]->GetComponent<Transform>()->SetTranslation(i % 2 == 0 ? Vector2D(9.0f, -44.0f) : Vector2D(9.0f, -44.0f));
				}

				gearHeight = -60.0f;

				break;
			}
			}

			// Create shadow sprite
			shadow = new GameObject(*objectManager.GetArchetypeByName("Shadow"));
			objectManager.AddObject(*shadow);

			// Create rising gears and add to objectManager
			GameObject* risingGears = new GameObject(*objectManager.GetArchetypeByName("RisingGears"));
			risingGears->GetComponent<Transform>()->SetTranslation(Vector2D(11.5f, gearHeight));
			objectManager.AddObject(*risingGears);
			GameObject* risingGears2 = new GameObject(*objectManager.GetArchetypeByName("RisingGears"));
			risingGears2->GetComponent<Transform>()->SetTranslation(Vector2D(5.5f, gearHeight));
			objectManager.AddObject(*risingGears2);
			GameObject* risingGears3 = new GameObject(*objectManager.GetArchetypeByName("RisingGears"));
			risingGears3->GetComponent<Transform>()->SetTranslation(Vector2D(18.5f, gearHeight));
			objectManager.AddObject(*risingGears3);

			dimensionController.SetActiveDimension(redDimension);
		}

		for (int i = 0; i < 6; i++)
		{
			GameObject* playerIndicator = new GameObject(*objectManager.GetArchetypeByName("PlayerIndicators"));
			playerIndicator->GetComponent<Sprite>()->SetAlpha(0.0f);
			playerIndicator->GetComponent<Sprite>()->SetFrame(i);
			objectManager.AddObject(*playerIndicator);
			playerIndicators[i] = playerIndicator;
		}

		Graphics::GetInstance().PushEffect(*chromaticAberration);
		Graphics::GetInstance().PushEffect(*cameraShake);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level1::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		backgroundImage->GetComponent<Transform>()->SetTranslation(Graphics::GetInstance().GetDefaultCamera().GetTranslation());
		backgroundImage->GetComponent<Transform>()->SetScale(Graphics::GetInstance().GetDefaultCamera().GetSize() * 2.0f);

		Input& input = Input::GetInstance();

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		std::vector<GameObject*> players;
		players.reserve(2);
		objectManager.GetAllObjectsByName("Player", players);

		if (input.CheckTriggered('I'))
		{
			for (auto it = players.begin(); it != players.end(); ++it)
				(*it)->GetComponent<Behaviors::AbilityHolder>()->SetAbility(Abilities::ABILITY_JETPACK);
		}

		if (input.CheckTriggered('O'))
		{
			for (auto it = players.begin(); it != players.end(); ++it)
				(*it)->GetComponent<Behaviors::AbilityHolder>()->SetAbility(Abilities::ABILITY_FLAMETHROWER);
		}

		if (input.CheckTriggered('P'))
		{
			for (auto it = players.begin(); it != players.end(); ++it)
				(*it)->GetComponent<Behaviors::AbilityHolder>()->SetAbility(Abilities::ABILITY_PROXIMITYMINE);
		}

		for (int i = 0; i < 6; i++)
		{
			playerIndicators[i]->GetComponent<Sprite>()->SetAlpha(0.0f);
		}

		GameObject* risingGears = objectManager.GetObjectByName("RisingGears");

		ExtendedInput& extendedInput = ExtendedInput::GetInstance();

		float lowestGearsDistance = FLT_MAX;
		for (auto it = players.begin(); it != players.end(); ++it)
		{
			Behaviors::PlayerMovement* playerMovement = (*it)->GetComponent<Behaviors::PlayerMovement>();
			playerIndicators[playerMovement->GetPlayerID() - 1]->GetComponent<Transform>()->SetTranslation((*it)->GetComponent<Transform>()->GetTranslation() + Vector2D(0.0f, 2.0f));
			playerIndicators[playerMovement->GetPlayerID() - 1]->GetComponent<Sprite>()->SetAlpha(1.0f);
			Vector2D playerTranslation = (*it)->GetComponent<Transform>()->GetTranslation();
			Vector2D gearsTranslation = risingGears->GetComponent<Transform>()->GetTranslation();
			float gearsDistancee = abs(playerTranslation.y - gearsTranslation.y);

			float lowFreq = 0.0f;
			float highFreq = 0.0f;

			extendedInput.GetVibration(lowFreq, highFreq, playerMovement->GetControllerID());

			lowFreq = max(lowFreq, 10.0f / (max(1.0f, gearsDistancee * 0.5f) * 50.0f));
			highFreq = max(highFreq, 25.0f / (max(1.0f, gearsDistancee * 0.5f) * 50.0f));

			extendedInput.SetVibration(lowFreq, highFreq, playerMovement->GetControllerID());

			lowestGearsDistance = min(lowestGearsDistance, gearsDistancee);
		}

		shadow->GetComponent<Transform>()->SetTranslation(risingGears->GetComponent<Transform>()->GetTranslation() + Vector2D(0.0f, -26.0f));

		// Play gear sound louder as it gets closer
		gearSound->setVolume(130.0f / pow(max(1.0f, lowestGearsDistance + 1.0f), 2.3f));

		chromaticAberration->SetIntensity(50.0f / pow(max(1.0f, lowestGearsDistance - 0.5f), 1.5f));
		cameraShake->SetIntensity(pow(1.0f / (max(1.0f, lowestGearsDistance) * 50.0f), 1.2f));

		// End game if a player dies
		unsigned playerCount = objectManager.GetObjectCount("Player");
		if (playerCount == 1)
		{
			// Play win sound once
			if (playWinSound)
			{
				playWinSound = false;
				
				// Stop background music and play fanfare
				soundManager->GetMusicChannel()->stop();
				soundManager->PlaySound("SoundFanf.wav")->setVolume(5.0f);
			}

			// Restart on space, enter, or start.
			bool controllerPressing = false;
			for (int i = 0; i < 4; i++)
				controllerPressing = controllerPressing || extendedInput.CheckXBTriggered(XB_START, i);
			if (controllerPressing || input.CheckTriggered(' ') || input.CheckTriggered(VK_RETURN))
			{
				GetSpace()->SetLevel(new Levels::LevelSelect());
			}
		}
		else if (playerCount + objectManager.GetObjectCount("DeathAnimation") <= 0)
		{
			GetSpace()->SetLevel(new Levels::LevelSelect());
		}
		else if (map == Map::Tutorial)
		{
			// Restart on space, enter, or start.
			bool controllerPressing = false;
			for (int i = 0; i < 4; i++)
				controllerPressing = controllerPressing || extendedInput.CheckXBTriggered(XB_START, i);
			if (controllerPressing || input.CheckTriggered(' ') || input.CheckTriggered(VK_RETURN))
			{
				GetSpace()->RestartLevel();
			}
		}
	}

	// Shutdown function for Level 1
	void Level1::Shutdown()
	{
		Graphics::GetInstance().RemoveEffect(*chromaticAberration);
		Graphics::GetInstance().RemoveEffect(*cameraShake);

		Space* hudSpace = GetAltSpace();

		// Set initial level to the HUD level - HUDSpace.
		if (hudSpace != nullptr)
			hudSpace->SetLevel<HUDEmpty>();
	}

	// Unload the resources associated with Level 1.
	void Level1::Unload()
	{
		// Free all allocated memory.
		delete chromaticAberration;
		delete cameraShake;

		delete dataStaticMap;
		delete dataRedMap;
		delete dataBlueMap;

		// Stop current music and play main menu music
		soundManager->GetMusicChannel()->stop();
		soundManager->PlaySound("SoundMenuM.wav");
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Adds static spikes to the level.
	// Params:
	//   spikes = An array of floats, each pair being a coordinate.
	//   numSpikes = How many spikes are in the array.
	void Level1::AddStaticSpikes(const float* spikes, int numSpikes)
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		for (int i = 0; i < numSpikes * 2; i += 2)
		{
			GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("StaticSpike"));
			static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(spikes[i], -spikes[i + 1]));
			objectManager.AddObject(*spike);
		}
	}

	// Adds red spikes to the level.
	// Params:
	//   spikes = An array of floats, each pair being a coordinate.
	//   numSpikes = How many spikes are in the array.
	//   redDimension = The ID of the red dimension.
	void Level1::AddRedSpikes(const float* spikes, int numSpikes, unsigned redDimension)
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		GameObject* gameController = objectManager.GetObjectByName("GameController");
		Behaviors::DimensionController& dimensionController = *static_cast<Behaviors::DimensionController*>(gameController->GetComponent("DimensionController"));

		for (int i = 0; i < numSpikes * 2; i += 2)
		{
			GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("RedSpike"));
			static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(spikes[i], -spikes[i + 1]));
			dimensionController.AddSpikeToDimension(redDimension, spike);
			objectManager.AddObject(*spike);
		}
	}

	// Adds blue spikes to the level.
	// Params:
	//   spikes = An array of floats, each pair being a coordinate.
	//   numSpikes = How many spikes are in the array.
	//   blueDimension = The ID of the blue dimension.
	void Level1::AddBlueSpikes(const float* spikes, int numSpikes, unsigned blueDimension)
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		GameObject* gameController = objectManager.GetObjectByName("GameController");
		Behaviors::DimensionController& dimensionController = *static_cast<Behaviors::DimensionController*>(gameController->GetComponent("DimensionController"));

		for (int i = 0; i < numSpikes * 2; i += 2)
		{
			GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("BlueSpike"));
			static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(spikes[i], -spikes[i + 1]));
			dimensionController.AddSpikeToDimension(blueDimension, spike);
			objectManager.AddObject(*spike);
		}
	}

	// Adds chips to the level.
	// Params:
	//   chipsSpawns = An array of floats, each pair being a coordinate.
	//   numChis = How many chips are in the array.
	void Level1::AddAbilities(const float* chipsSpawns, int numChips)
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		for (int i = 0; i < numChips * 2; i += 2)
		{
			GameObject* ability = nullptr;

			// Random ability
			int abilityType = RandomRange(1, 3);
			switch (abilityType)
			{
			case 1:
				ability = new GameObject(*objectManager.GetArchetypeByName("JetpackPickup"));
				break;
			case 2:
				ability = new GameObject(*objectManager.GetArchetypeByName("FlamethrowerPickup"));
				break;
			case 3:
				ability = new GameObject(*objectManager.GetArchetypeByName("ProximityMinePickup"));
				break;
			default:
				return; // Won't even reach here, but without it Visual Studio complains about ability potentially being a nullptr.
			}

			static_cast<Transform*>(ability->GetComponent("Transform"))->SetTranslation(Vector2D(chipsSpawns[i], -chipsSpawns[i + 1]));
			objectManager.AddObject(*ability);
		}
	}
}

//----------------------------------------------------------------------------
