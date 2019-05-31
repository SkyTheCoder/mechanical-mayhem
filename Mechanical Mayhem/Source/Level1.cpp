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
#include "Archetypes.h"
#include <glfw3.h>
#include "Tilemap.h"
#include <Graphics.h>
#include <Random.h>
#include <GameObjectFactory.h>
#include <SoundManager.h>

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
		map(map_),
		backgroundImage(nullptr),
		chromaticAberration(nullptr),
		columnsMonkey(3), rowsMonkey(10),
		columnsCat(3), rowsCat(10),
		columnsSpikes(1), rowsSpikes(3),
		columnsExplosion(3), rowsExplosion(3),
		columnsFlame(2), rowsFlame(2), columnsJetpackFlame(2), rowsJetpackFlame(2),
		columnsRisingGears(1), rowsRisingGears(2),
		columnsMine(2), rowsMine(2),
		columnsMinePickup(2), rowsMinePickup(2), columnsJetpackPickup(2), rowsJetpackPickup(2), columnsFlamethrowerPickup(2), rowsFlamethrowerPickup(2),
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
		resourceManager.GetSpriteSource("Tilemap.png", columnsMap, rowsMap);
		resourceManager.GetSpriteSource("RisingGears.png", columnsRisingGears, rowsRisingGears);

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
		case Map::Blah:
			mapName = "Blah";
			break;
		case Map::Chase:
			mapName = "Chase";
			break;
		case Map::Merge:
			mapName = "Merge";
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
		objectManager.AddArchetype(*objectFactory.CreateObject("Text"));
		objectManager.AddArchetype(*objectFactory.CreateObject("GameController"));
		objectManager.AddArchetype(*objectFactory.CreateObject("Collectible", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Collectible.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("JetpackPickup", resourceManager.GetMesh("JetpackPickup"), resourceManager.GetSpriteSource("AniJetpackPickup.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("FlamethrowerPickup", resourceManager.GetMesh("FlamethrowerPickup"), resourceManager.GetSpriteSource("AniFlamethrowerPickup.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("ProximityMinePickup", resourceManager.GetMesh("ProximityMinePickup"), resourceManager.GetSpriteSource("AniMinePickup.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Flame", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Mine", resourceManager.GetMesh("Mine"), resourceManager.GetSpriteSource("AniMine.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Explosion", resourceManager.GetMesh("Explosion"), resourceManager.GetSpriteSource("AniMineExplode.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("FlameEffect", resourceManager.GetMesh("Flame"), resourceManager.GetSpriteSource("AniFlame.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("JetpackFlame", resourceManager.GetMesh("JetpackFlame"), resourceManager.GetSpriteSource("AniFlame.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("StaticSpike", resourceManager.GetMesh("Spikes"), resourceManager.GetSpriteSource("Spikes.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("RedSpike", resourceManager.GetMesh("Spikes"), resourceManager.GetSpriteSource("Spikes.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("BlueSpike", resourceManager.GetMesh("Spikes"), resourceManager.GetSpriteSource("Spikes.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Tilemap", resourceManager.GetMesh("Map"), resourceManager.GetSpriteSource("Tilemap.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("RisingGears", resourceManager.GetMesh("RisingGears"), resourceManager.GetSpriteSource("RisingGears.png")));

		chromaticAberration = new Effects::ChromaticAberration();
		chromaticAberration->SetIntensity(2.0f);

		// Set the background color.
		Graphics::GetInstance().SetBackgroundColor(Colors::Black);
	}

	// Initialize the memory associated with Level 1.
	void Level1::Initialize()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Stop menu music and play in-game music
		soundManager = Engine::GetInstance().GetModule<SoundManager>();
		soundManager->GetMusicChannel()->stop();
		playWinSound = true;

		// Play background music
		soundManager->PlaySound("SoundPlay.wav");

		// Load HUD Level
		Space* hudSpace = GetAltSpace();

		// Set initial level to the HUD level - HUDSpace.
		if (hudSpace != nullptr)
			hudSpace->SetLevel<HUDLevel>();

		// Create background sprite
		backgroundImage = new GameObject(*objectManager.GetArchetypeByName("BackgroundImage"));
		objectManager.AddObject(*backgroundImage);

		// Create the players and add them to the object manager.
		GameObject* player = new GameObject(*objectManager.GetArchetypeByName("Player"));
		player->GetComponent<Behaviors::MonkeyAnimation>()->SetFrames(0, 8, 9, 4, 15, 4, 27, 2, 21, 4);
		Behaviors::PlayerMovement* playerMovement = static_cast<Behaviors::PlayerMovement*>(player->GetComponent("PlayerMovement"));
		playerMovement->SetKeybinds(VK_UP, VK_LEFT, VK_RIGHT, VK_RCONTROL);
		playerMovement->SetPlayerID(1);
		objectManager.AddObject(*player);
		player->GetComponent<Behaviors::MonkeyAnimation>()->GetSpriteSources("A");

		GameObject* player2 = new GameObject(*objectManager.GetArchetypeByName("Player"));
		player2->GetComponent<Collider>()->SetGroup(2);
		player2->GetComponent<Collider>()->SetMask(1 << 2);
		player2->GetComponent<Behaviors::MonkeyAnimation>()->SetFrames(0, 8, 9, 4, 15, 4, 27, 2, 21, 4);
		Behaviors::PlayerMovement* player2Movement = static_cast<Behaviors::PlayerMovement*>(player2->GetComponent("PlayerMovement"));
		player2Movement->SetKeybinds('W', 'A', 'D', VK_LCONTROL);
		player2Movement->SetPlayerID(2);
		objectManager.AddObject(*player2);
		player2->GetComponent<Behaviors::MonkeyAnimation>()->GetSpriteSources("B");

		// Create the Game Controller, which handles the camera and dimensions.
		GameObject* gameController = new GameObject(*objectManager.GetArchetypeByName("GameController"));
		static_cast<Behaviors::CameraFollow*>(gameController->GetComponent("CameraFollow"))->AddPlayer(player);
		static_cast<Behaviors::CameraFollow*>(gameController->GetComponent("CameraFollow"))->AddPlayer(player2);
		Behaviors::DimensionController& dimensionController = *static_cast<Behaviors::DimensionController*>(gameController->GetComponent("DimensionController"));
		objectManager.AddObject(*gameController);
		objectManager.DispatchEvent(new Event(ET_Generic, "SnapToTarget", 0.01f, GUID(), gameController->GetID()));
		Graphics::GetInstance().GetDefaultCamera().SetSize(0.0f);

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

			// Map data (spikes, collectibles, etc.)
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

				static_cast<Sprite*>(player2->GetComponent("Sprite"))->SetAlpha(0.0f);
				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(1.0f, -28.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(1.0f, -28.0f));
				static_cast<Behaviors::PlayerMovement*>(player2->GetComponent("PlayerMovement"))->SetKeybinds(VK_UP, VK_LEFT, VK_RIGHT, VK_RCONTROL);

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

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(6.0f, -15.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(42.0f, -15.0f));

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

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(9.0f, -3.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(22.0f, -2.0f));

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

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(6.0f, -26.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(23.0f, -27.0f));

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

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(3.0f, -2.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(28.0f, -2.0f));

				break;
			}
			case Map::Descent:
			{
				float staticSpikes[44] = {
					1.0f, 47.0f, 2.0f, 47.0f, 3.0f, 47.0f, 4.0f, 47.0f, 5.0f, 47.0f, 6.0f, 47.0f, 7.0f, 47.0f, 8.0f, 47.0f, 9.0f, 47.0f, 10.0f, 47.0f, 11.0f, 47.0f, 12.0f, 47.0f, 13.0f, 47.0f, 14.0f, 47.0f, 15.0f, 47.0f, 16.0f, 47.0f,
					17.0f, 47.0f, 18.0f, 47.0f, 19.0f, 47.0f, 20.0f, 47.0f, 21.0f, 47.0f, 22.0f, 47.0f
				};

				float chipsSpawns[18] = {
					17.0f, 7.0f, 21.0f, 10.0f, 6.0f, 12.0f, 5.0f, 21.0f, 18.0f, 21.0f, 13.0f, 27.0f, 14.0f, 32.0f, 9.0f, 43.0f, 20.0f, 43.0f
				};

				AddStaticSpikes(staticSpikes, 22);
				AddAbilities(chipsSpawns, 8);

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(5.0f, -4.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(15.0f, -4.0f));

				break;
			}
			case Map::Blah:
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

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(7.0f, -38.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(16.0f, -38.0f));

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

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(9.0f, -45.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(9.0f, -45.0f));

				gearHeight = -62.0f;

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

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(14.0f, -41.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(9.0f, -41.0f));

				gearHeight = -50.0f;

				break;
			}
			}

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

		// Create winText and add to objectManager
		GameObject* winText = new GameObject(*objectManager.GetArchetypeByName("Text"));
		winText->GetComponent<SpriteTextMono>()->SetColor(Colors::White);
		objectManager.AddObject(*winText);

		Graphics::GetInstance().PushEffect(*chromaticAberration);
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

		GameObject* risingGears = objectManager.GetObjectByName("RisingGears");

		float lowestGearsDistance = FLT_MAX;
		for (auto it = players.begin(); it != players.end(); ++it)
		{
			Vector2D playerTranslation = (*it)->GetComponent<Transform>()->GetTranslation();
			Vector2D gearsTranslation = risingGears->GetComponent<Transform>()->GetTranslation();
			float gearsDistancee = abs(playerTranslation.y - gearsTranslation.y);
			lowestGearsDistance = min(lowestGearsDistance, gearsDistancee);
		}

		chromaticAberration->SetIntensity(50.0f / pow(max(1.0f, lowestGearsDistance - 3.0f), 1.5f));

		// End game if a player dies
		unsigned playerCount = objectManager.GetObjectCount("Player");
		if (playerCount == 1)
		{
			GameObject* winText = GetSpace()->GetObjectManager().GetObjectByName("Text");

			GameObject* lastPlayer = objectManager.GetObjectByName("Player");
			Behaviors::PlayerMovement* lastPlayerMovement = static_cast<Behaviors::PlayerMovement*>(lastPlayer->GetComponent("PlayerMovement"));

			// Play win sound once
			if (playWinSound)
			{
				playWinSound = false;
				
				// Stop background music and play fanfare
				soundManager->GetMusicChannel()->stop();
				soundManager->PlaySound("SoundFanf.wav")->setVolume(5.0f);
			}

			// Set text to winText
			SpriteTextMono* spriteText = winText->GetComponent<SpriteTextMono>();
			switch (lastPlayerMovement->GetPlayerID())
			{
			case 1:
				spriteText->SetText("Ninja Monkey won! Press <SPACE> to return to level select");
				break;
			case 2:
				spriteText->SetText("Cat Fighter won! Press <SPACE> to return to level select");
				break;
			}

			// Text follows camera
			static_cast<Transform*>(winText->GetComponent("Transform"))
				->SetTranslation(Graphics::GetInstance().GetDefaultCamera().GetTranslation());

			// Restart on <SPACE>
			if (input.CheckTriggered(' '))
			{
				GetSpace()->SetLevel(new Levels::LevelSelect());
			}
		}
		else if (playerCount <= 0)
		{
			GetSpace()->SetLevel(new Levels::LevelSelect());
		}
		else if (map == Map::Tutorial)
		{
			// Restart on <SPACE>
			if (input.CheckTriggered(' '))
			{
				GetSpace()->RestartLevel();
			}
		}
	}

	// Shutdown function for Level 1
	void Level1::Shutdown()
	{
		Graphics::GetInstance().RemoveEffect(*chromaticAberration);

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
