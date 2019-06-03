//------------------------------------------------------------------------------
//
// File Name:	Archetypes.cpp
// Author(s):	David Cohen (david.cohen), A.J. Bussman (anthony.bussman), Daniel Walther (daniel.walther)
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

#include "Archetypes.h"

// Systems
#include "GameObject.h"
#include <GameObjectFactory.h>
#include "Space.h"
#include "SpriteSource.h"
#include <Mesh.h>

// Components
#include "Transform.h"
#include "Sprite.h"
#include "Physics.h"
#include "PlayerShip.h"
#include "TimedDeath.h"
#include "PlayerMovement.h"
#include "Animation.h"
#include "ColliderPoint.h"
#include "ColliderCircle.h"
#include "ColliderRectangle.h"
#include "ColorChange.h"
#include "ScreenWrap.h"
#include "SpriteTilemap.h"
#include "ColliderTilemap.h"
#include "MonkeyAnimation.h"
#include <SpriteTextMono.h>
#include "CameraFollow.h"
#include "Hazard.h"
#include "DimensionController.h"
#include "ShiftPickup.h"
#include "Button.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Archetypes
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create the player game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	//   spriteSource = The sprite source to use for the object.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreatePlayer(Mesh* mesh, SpriteSource* spriteSource)
	{
		// Create a new game object.
		GameObject* player = new GameObject("Player");

		// Create a new transform.
		player->AddComponent(new Transform(Vector2D(), Vector2D(100.0f, 100.0f)));

		// Create a new sprite.
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetSpriteSource(spriteSource);
		player->AddComponent(sprite);

		// Create a new physics.
		player->AddComponent(new Physics());

		// Create a new rectangle collider.
		player->AddComponent(new ColliderRectangle(Vector2D(49.99f, 49.99f)));

		// Create a new animation.
		player->AddComponent(new Animation());

		// Create a new player movement.
		player->AddComponent(new Behaviors::PlayerMovement());

		// Create a new MonkeyAnimation.
		player->AddComponent(new Behaviors::MonkeyAnimation(0, 8, 9, 1, 15, 1));

		GameObjectFactory::GetInstance().SaveObjectToFile(player);

		return player;
	}

	// Create a tilemap object
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	//   map = The map to use for tilemap related components.
	// Returns:
	//   A pointer to the newly constructed game object.
	GameObject* CreateTilemapObject(Mesh* mesh, SpriteSource* spriteSource, Tilemap* map)
	{
		// Create a new game object.
		GameObject* tilemap = new GameObject("Tilemap");

		// Create a new transform.
		Transform* transform = new Transform(Vector2D(), Vector2D(100.0f, 100.0f));// Vector2D(-350.0f, 250.0f), Vector2D(100.0f, 100.0f));
		tilemap->AddComponent(transform);

		// Create a new sprite tilemap.
		SpriteTilemap* spriteTilemap = new SpriteTilemap();
		spriteTilemap->SetMesh(mesh);
		spriteTilemap->SetSpriteSource(spriteSource);
		spriteTilemap->SetTilemap(map);
		tilemap->AddComponent(spriteTilemap);
		 
		// Create a new collider tilemap.
		ColliderTilemap* colliderTilemap = new ColliderTilemap();
		colliderTilemap->SetTilemap(map);
		tilemap->AddComponent(colliderTilemap);

		GameObjectFactory::GetInstance().SaveObjectToFile(tilemap);

		return tilemap;
	}

	// Create a text game object.
	// Returns:
	//	 A pointer to the newly constructed game object.
	GameObject* CreateText()
	{
		// Create a new game object.
		GameObject* text = new GameObject("Text");

		// Create a new transform.
		Transform* transform = new Transform(0.0f, 0.0f);
		transform->SetScale(Vector2D(64.0f, 64.0f));
		text->AddComponent(transform);

		// Create a new sprite text.
		SpriteTextMono* spriteText = new SpriteTextMono();
		spriteText->SetHorizontalAlignment(SpriteTextMono::Alignment::CENTER);
		spriteText->SetVerticalAlignment(SpriteTextMono::Alignment::CENTER);
		text->AddComponent(spriteText);

		GameObjectFactory::GetInstance().SaveObjectToFile(text);

		return text;
	}

	// Create a collectible archetype
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateCollectibleArchetype(Mesh* mesh, SpriteSource* spriteSource)
	{
		GameObject* Collectible = new GameObject("Collectible");

		// Create transform
		Transform* transform = new Transform();
		transform->SetScale(Vector2D(75.0f, 75.0f));

		// Create sprite
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetSpriteSource(spriteSource);

		// Create collider
		ColliderRectangle* colliderrect = new ColliderRectangle(transform->GetScale() / 2.0f);

		// Create chip collectible
		Behaviors::ShiftPickup* chipcollectible = new Behaviors::ShiftPickup();

		// Add components
		Collectible->AddComponent(chipcollectible);
		Collectible->AddComponent(transform);
		Collectible->AddComponent(sprite);
		Collectible->AddComponent(colliderrect);

		GameObjectFactory::GetInstance().SaveObjectToFile(Collectible);

		return Collectible;
	}

	// Create a game controller object.
	// Returns;
	//	 A pointer to the newly constructed game object.
	GameObject* CreateGameController()
	{
		// Create a new game object.
		GameObject* gameController = new GameObject("GameController");

		// Create a new CameraFollow.
		gameController->AddComponent(new Behaviors::CameraFollow(Vector2D(100.0f, 200.0f), 0.98f));

		// Create a new DimensionController.
		gameController->AddComponent(new Behaviors::DimensionController());

		GameObjectFactory::GetInstance().SaveObjectToFile(gameController);

		return gameController;
	}

	// Create a hazard archetype
	// Params:
	//	 name = name of hazard
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	//	 frame = frame to start on
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateHazardArchetype(std::string name, Mesh* mesh, SpriteSource* spriteSource, unsigned frame)
	{
		GameObject* Hazard = new GameObject(name);

		// Create transform
		Transform* transform = new Transform();
		transform->SetScale(Vector2D(100.0f, 100.0f));
		transform->SetRotation(M_PI_F / 4.0f);

		// Create sprite
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetSpriteSource(spriteSource);
		sprite->SetFrame(frame);

		// Create collider
		ColliderRectangle* collider = new ColliderRectangle(transform->GetScale() / 2.0f - Vector2D(0.5f, 0.5f));
		collider->SetGroup(1);
		collider->SetMask(1 << 1);

		// Create hazard component
		Behaviors::Hazard* hazard = new Behaviors::Hazard();

		// Add components
		Hazard->AddComponent(transform);
		Hazard->AddComponent(sprite);
		Hazard->AddComponent(collider);
		Hazard->AddComponent(hazard);

		GameObjectFactory::GetInstance().SaveObjectToFile(Hazard);

		return Hazard;
	}

	// Create a jumpBoost archetype
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateJumpBoostArchetype(Mesh* mesh, SpriteSource* spriteSource)
	{
		GameObject* JumpBoost = new GameObject("JumpBoost");

		// Create transform
		Transform* transform = new Transform();
		transform->SetScale(Vector2D(100.0f, 100.0f));

		// Create sprite
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetSpriteSource(spriteSource);

		// Create collider
		ColliderCircle* collider = new ColliderCircle(transform->GetScale().x / 2.0f - 0.5f);

		// Add components
		JumpBoost->AddComponent(transform);
		JumpBoost->AddComponent(sprite);
		JumpBoost->AddComponent(collider);

		GameObjectFactory::GetInstance().SaveObjectToFile(JumpBoost);

		return JumpBoost;
	}

	// Create a speedBoost archetype
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateSpeedBoostArchetype(Mesh* mesh, SpriteSource* spriteSource)
	{
		GameObject* SpeedBoost = new GameObject("SpeedBoost");

		// Create transform
		Transform* transform = new Transform();
		transform->SetScale(Vector2D(100.0f, 100.0f));

		// Create sprite
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetSpriteSource(spriteSource);

		// Create collider
		ColliderCircle* collider = new ColliderCircle(transform->GetScale().x / 2.0f - 0.5f);

		// Add components
		SpeedBoost->AddComponent(transform);
		SpeedBoost->AddComponent(sprite);
		SpeedBoost->AddComponent(collider);

		GameObjectFactory::GetInstance().SaveObjectToFile(SpeedBoost);

		return SpeedBoost;
	}

	// Create a button archetype
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateButtonArchetype(Mesh* mesh, SpriteSource* spriteSource)
	{
		GameObject* Button = new GameObject("Button");

		// Create transform
		Transform* transform = new Transform();
		transform->SetScale(Vector2D(300.0f, 75.0f));

		// Create sprite
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetSpriteSource(spriteSource);

		// Create collider
		ColliderRectangle* collider = new ColliderRectangle(transform->GetScale() / 2.0f);

		// Create button
		Behaviors::Button* button = new Behaviors::Button();

		// Add components
		Button->AddComponent(transform);
		Button->AddComponent(sprite);
		Button->AddComponent(collider);
		Button->AddComponent(button);

		GameObjectFactory::GetInstance().SaveObjectToFile(Button);

		return Button;
	}

	// Create a fullscreen image archetype
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateFullScreenImageArchetype(Mesh* mesh, SpriteSource* spriteSource)
	{
		GameObject* gameObject = new GameObject("FullScreenImage");

		// Create a new transform.
		gameObject->AddComponent(new Transform(Vector2D(), Vector2D(800.0f, 600.0f)));

		// Create a new sprite.
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetSpriteSource(spriteSource);
		gameObject->AddComponent(sprite);

		GameObjectFactory::GetInstance().SaveObjectToFile(gameObject);

		return gameObject;
	}
}

//----------------------------------------------------------------------------
