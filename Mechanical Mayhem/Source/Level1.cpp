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

// Levels
#include "LevelSelect.h"
#include "HUDEmpty.h"
#include "HUDLevel.h"

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
		columnsMonkey(3), rowsMonkey(10),
		columnsCat(3), rowsCat(10),
		columnsSpikes(1), rowsSpikes(3),
		columnsRisingGears(1), rowsRisingGears(2),
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
		resourceManager.GetMesh("RisingGears", Vector2D(1.0f / columnsRisingGears, 1.0f / rowsRisingGears), Vector2D(0.5f, 0.5f));

		resourceManager.GetSpriteSource("AniA.png", columnsMonkey, rowsMonkey);
		resourceManager.GetSpriteSource("AniB.png", columnsCat, rowsCat);
		resourceManager.GetSpriteSource("Spikes.png", columnsSpikes, rowsSpikes);
		resourceManager.GetSpriteSource("jetpackCollectible.png");
		resourceManager.GetSpriteSource("flamethrowerCollectible.png");
		resourceManager.GetSpriteSource("proximityMineCollectible.png");
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
		objectManager.AddArchetype(*objectFactory.CreateObject("JetpackPickup", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("jetpackCollectible.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("FlamethrowerPickup", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("flamethrowerCollectible.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("ProximityMinePickup", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("proximityMineCollectible.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("JetpackPickup", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Collectible.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Fireball", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Mine", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("StaticSpike", resourceManager.GetMesh("Spikes"), resourceManager.GetSpriteSource("Spikes.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("RedSpike", resourceManager.GetMesh("Spikes"), resourceManager.GetSpriteSource("Spikes.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("BlueSpike", resourceManager.GetMesh("Spikes"), resourceManager.GetSpriteSource("Spikes.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Tilemap", resourceManager.GetMesh("Map"), resourceManager.GetSpriteSource("Tilemap.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("RisingGears", resourceManager.GetMesh("RisingGears"), resourceManager.GetSpriteSource("RisingGears.png")));

		// Set the background color.
		Graphics::GetInstance().SetBackgroundColor(Colors::Black);
	}

	// Initialize the memory associated with Level 1.
	void Level1::Initialize()
	{
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Stop menu music and play in-game music
		SoundManager* soundManager = Engine::GetInstance().GetModule<SoundManager>();
		soundManager->GetMusicChannel()->stop();
		// soundManager->PlaySound("");

		// Play startup sounds
		//soundManager->PlaySound("gamestart.wav");
		soundManager->PlaySound("SoundFanf.wav");

		// Load HUD Level
		Space* hudSpace = GetAltSpace();

		// Set initial level to the HUD level - HUDSpace.
		if (hudSpace != nullptr)
			hudSpace->SetLevel<HUDLevel>();

		// Create background sprite
		GameObject* backgroundImage = new GameObject(*objectManager.GetArchetypeByName("BackgroundImage"));
		objectManager.AddObject(*backgroundImage);

		// Create the players and add them to the object manager.
		GameObject* player = new GameObject(*objectManager.GetArchetypeByName("Player"));
		//*reinterpret_cast<std::string*>(reinterpret_cast<uintptr_t>(player) + sizeof(void**)) = "Player1";
		player->GetComponent< Behaviors::MonkeyAnimation>()->SetFrames(0, 8, 9, 4, 15, 4, 27, 2, 21, 4);
		Behaviors::PlayerMovement* playerMovement = static_cast<Behaviors::PlayerMovement*>(player->GetComponent("PlayerMovement"));
		playerMovement->SetKeybinds(VK_UP, VK_LEFT, VK_RIGHT, VK_RCONTROL);
		playerMovement->SetPlayerID(1);
		objectManager.AddObject(*player);

		GameObject* player2 = new GameObject(*objectManager.GetArchetypeByName("Player"));
		//*reinterpret_cast<std::string*>(reinterpret_cast<uintptr_t>(player2) + sizeof(void**)) = "Player2";
		player2->GetComponent<Collider>()->SetGroup(2);
		player2->GetComponent<Collider>()->SetMask(1 << 2);
		Sprite* player2Sprite = static_cast<Sprite*>(player2->GetComponent("Sprite"));
		player2Sprite->SetMesh(resourceManager.GetMesh("Cat"));
		player2Sprite->SetSpriteSource(resourceManager.GetSpriteSource("AniB.png"));
		player2->GetComponent< Behaviors::MonkeyAnimation>()->SetFrames(0, 8, 9, 4, 15, 4, 27, 2, 21, 4);
		Behaviors::PlayerMovement* player2Movement = static_cast<Behaviors::PlayerMovement*>(player2->GetComponent("PlayerMovement"));
		player2Movement->SetKeybinds('W', 'A', 'D', VK_LCONTROL);
		player2Movement->SetPlayerID(2);
		objectManager.AddObject(*player2);

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
				AddChips(chipsSpawns, 6);

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
				AddChips(chipsSpawns, 8);

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
				AddChips(chipsSpawns, 8);

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
				AddChips(chipsSpawns, 4);

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
				AddChips(chipsSpawns, 4);

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
				AddChips(chipsSpawns, 8);

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
				AddChips(chipsSpawns, 10);

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(7.0f, -38.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(16.0f, -38.0f));

				// Create rising gears and add to objectManager
				GameObject* risingGears = new GameObject(*objectManager.GetArchetypeByName("RisingGears"));
				risingGears->GetComponent<Transform>()->SetTranslation(Vector2D(11.5f, -48.0f));
				objectManager.AddObject(*risingGears);
				GameObject* risingGears2 = new GameObject(*objectManager.GetArchetypeByName("RisingGears"));
				risingGears2->GetComponent<Transform>()->SetTranslation(Vector2D(5.5f, -48.0f));
				objectManager.AddObject(*risingGears2);
				GameObject* risingGears3 = new GameObject(*objectManager.GetArchetypeByName("RisingGears"));
				risingGears3->GetComponent<Transform>()->SetTranslation(Vector2D(18.5f, -48.0f));
				objectManager.AddObject(*risingGears3);

				break;
			}
			case Map::Chase:
			{
				float staticSpikes[62] = {
					0.0f, 15.0f, 1.0f, 15.0f, 2.0f, 15.0f, 3.0f, 15.0f, 0.0f, 16.0f, 1.0f, 16.0f, 2.0f, 16.0f, 14.0f, 16.0f, 15.0f, 16.0f, 16.0f, 16.0f, 17.0f, 16.0f, 18.0f, 16.0f, 19.0f, 16.0f, 0.0f, 17.0f, 1.0f, 17.0f, 0.0f, 18.0f,
					0.0f, 19.0f, 0.0f, 20.0f, 1.0f, 28.0f, 2.0f, 28.0f, 3.0f, 28.0f, 4.0f, 28.0f, 0.0f, 36.0f, 0.0f, 37.0f, 0.0f, 38.0f, 0.0f, 39.0f, 0.0f, 40.0f, 0.0f, 43.0f, 0.0f, 44.0f, 0.0f, 45.0f, 0.0f, 46.0f
				};

				float redSpikes[14] = {
					18.0f, 2.0f, 19.0f, 3.0f, 13.0f, 10.0f, 14.0f, 11.0f, 6.0f, 21.0f, 7.0f, 21.0f, 8.0f, 21.0f
				};

				float blueSpikes[12] = {
					20.0f, 10.0f, 19.0f, 11.0f, 12.0f, 27.0f, 13.0f, 27.0f, 14.0f, 27.0f, 15.0f, 27.0f
				};

				float chipsSpawns[10] = {
					2.0f, 12.0f, 11.0f, 21.0f, 3.0f, 27.0f, 13.0f, 35.0f, 7.0f, 38.0f
				};

				AddStaticSpikes(staticSpikes, 31);
				AddRedSpikes(redSpikes, 7, redDimension);
				AddBlueSpikes(blueSpikes, 6, blueDimension);
				AddChips(chipsSpawns, 5);

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(9.0f, -45.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(9.0f, -45.0f));

				// Create rising gears and add to objectManager
				GameObject* risingGears = new GameObject(*objectManager.GetArchetypeByName("RisingGears"));
				risingGears->GetComponent<Transform>()->SetTranslation(Vector2D(11.5f, -60.0f));
				objectManager.AddObject(*risingGears);
				GameObject* risingGears2 = new GameObject(*objectManager.GetArchetypeByName("RisingGears"));
				risingGears2->GetComponent<Transform>()->SetTranslation(Vector2D(5.5f, -60.0f));
				objectManager.AddObject(*risingGears2);
				GameObject* risingGears3 = new GameObject(*objectManager.GetArchetypeByName("RisingGears"));
				risingGears3->GetComponent<Transform>()->SetTranslation(Vector2D(18.5f, -60.0f));
				objectManager.AddObject(*risingGears3);

				break;
			}
			}

			dimensionController.SetActiveDimension(redDimension);
		}

		// Create winText and add to objectManager
		GameObject* winText = new GameObject(*objectManager.GetArchetypeByName("Text"));
		winText->GetComponent<SpriteTextMono>()->SetColor(Colors::White);
		objectManager.AddObject(*winText);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level1::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// End game if a player dies
		unsigned playerCount = objectManager.GetObjectCount("Player");
		if (playerCount == 1)
		{
			GameObject* winText = GetSpace()->GetObjectManager().GetObjectByName("Text");

			GameObject* lastPlayer = objectManager.GetObjectByName("Player");
			Behaviors::PlayerMovement* lastPlayerMovement = static_cast<Behaviors::PlayerMovement*>(lastPlayer->GetComponent("PlayerMovement"));

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
		Space* hudSpace = GetAltSpace();

		// Set initial level to the HUD level - HUDSpace.
		if (hudSpace != nullptr)
			hudSpace->SetLevel<HUDEmpty>();
	}

	// Unload the resources associated with Level 1.
	void Level1::Unload()
	{
		// Free all allocated memory.
		delete dataStaticMap;
		delete dataRedMap;
		delete dataBlueMap;

		// Play main menu music
		Engine::GetInstance().GetModule<SoundManager>()->PlaySound("SoundMenuM.wav");
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
	void Level1::AddChips(const float* chipsSpawns, int numChips)
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
			}

			static_cast<Transform*>(ability->GetComponent("Transform"))->SetTranslation(Vector2D(chipsSpawns[i], -chipsSpawns[i + 1]));
			objectManager.AddObject(*ability);
		}
	}
}

//----------------------------------------------------------------------------
