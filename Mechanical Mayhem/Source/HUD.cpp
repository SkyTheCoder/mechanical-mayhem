//------------------------------------------------------------------------------
//
// File Name:	HUD.cpp
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
#include "HUD.h"

// System
#include "GameObject.h"
#include <GameObjectFactory.h>
#include <GameObjectManager.h>
#include <ResourceManager.h>
#include "Vector2D.h"
#include <Level.h>
#include <Space.h>
#include <Graphics.h>
#include <Camera.h>
#include <Interpolation.h>

// Component
#include "Health.h"
#include "Ability.h"
#include "AbilityHolder.h"
#include <Sprite.h>
#include <SpriteTextMono.h>
#include "PlayerMovement.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	// Constructor
	HUD::HUD(GameObject* player, Camera& camera)
		: BetaObject("HUD"),
		playerID(0), health(nullptr), abilityHolder(nullptr), hudBackground(nullptr),
		camera(camera),
		healthBarCenter(nullptr), healthBarEnd(nullptr), healthBarBackgroundCenter(nullptr), healthBarBackgroundEnd(nullptr), animHealth(0.0f),
		abilityBarCenter(nullptr), abilityBarEnd(nullptr), abilityBarBackgroundCenter(nullptr), abilityBarBackgroundEnd(nullptr), animAbility(0.0f)
	{
		SetPlayer(player);
	}

	// Return a new copy of the component.
	HUD* HUD::Clone() const
	{
		return new HUD(*this);
	}

	// Initialize data for this object.
	void HUD::Initialize()
	{
		GameObjectFactory& ojectFactory = GameObjectFactory::GetInstance();
		ResourceManager& resourceManager = static_cast<Level*>(GetOwner())->GetSpace()->GetResourceManager();

		// HUD background
		hudBackground = ojectFactory.CreateObject("HUDBackground", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Icon" + std::string(playerID == 1 ? "A" : "B") + "_01.png"));

		// Health bar
		healthBarCenter = ojectFactory.CreateObject("HealthBar", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("HealthBar.png"));
		healthBarEnd = ojectFactory.CreateObject("HealthBarEnd", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("HealthBarEnd.png"));

		// Ability bar
		abilityBarCenter = ojectFactory.CreateObject("AbilityBar", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("HealthBar.png"));
		abilityBarEnd = ojectFactory.CreateObject("AbilityBarEnd", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("HealthBarEnd.png"));

		// Add HUD Objects
		GameObjectManager& objectManager = static_cast<Level*>(GetOwner())->GetSpace()->GetObjectManager();

		healthBarBackgroundCenter = new GameObject(*healthBarCenter);
		healthBarBackgroundEnd = new GameObject(*healthBarEnd);

		abilityBarBackgroundCenter = new GameObject(*abilityBarCenter);
		abilityBarBackgroundEnd = new GameObject(*abilityBarEnd);

		objectManager.AddObject(*healthBarBackgroundCenter);
		objectManager.AddObject(*healthBarBackgroundEnd);
		objectManager.AddObject(*abilityBarBackgroundCenter);
		objectManager.AddObject(*abilityBarBackgroundEnd);
		objectManager.AddObject(*healthBarCenter);
		objectManager.AddObject(*healthBarEnd);
		objectManager.AddObject(*abilityBarCenter);
		objectManager.AddObject(*abilityBarEnd);
		objectManager.AddObject(*hudBackground);

		healthBarBackgroundCenter->GetComponent<Sprite>()->SetColor(Color(0.5f, 0.5f, 0.5f));
		healthBarBackgroundEnd->GetComponent<Sprite>()->SetColor(Color(0.5f, 0.5f, 0.5f));
		abilityBarBackgroundCenter->GetComponent<Sprite>()->SetColor(Color(0.5f, 0.5f, 0.5f));
		abilityBarBackgroundEnd->GetComponent<Sprite>()->SetColor(Color(0.5f, 0.5f, 0.5f));

		healthBarCenter->GetComponent<Sprite>()->SetColor(Color(255.0f / 255.0f, 0.0f / 255.0f, 14.0f / 255.0f));
		healthBarEnd->GetComponent<Sprite>()->SetColor(Color(255.0f / 255.0f, 0.0f / 255.0f, 14.0f / 255.0f));

		// Set the Locations of the objects
		SetHUDObjectLocations();
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void HUD::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		float targetHealth = -0.2f;
		float targetAbility = -0.2f;

		if (GetPlayer() != nullptr)
		{
			targetHealth = static_cast<float>(health->GetHealth()) / static_cast<float>(health->GetMaxHealth());
			if (targetHealth <= 1e-6f)
				targetHealth = -0.2f;

			Abilities::Ability* ability = abilityHolder->GetAbility();
			if (ability != nullptr)
				targetAbility = ability->GetMana();
			if (targetAbility <= 1e-6f)
				targetAbility = -0.2f;

			if (ability != nullptr)
			{
				switch (abilityHolder->GetAbilityType())
				{
				case Abilities::ABILITY_JETPACK:
					abilityBarCenter->GetComponent<Sprite>()->SetColor(Color(255.0f / 255.0f, 240.0f / 255.0f, 87.0f / 255.0f));
					abilityBarEnd->GetComponent<Sprite>()->SetColor(Color(255.0f / 255.0f, 240.0f / 255.0f, 87.0f / 255.0f));
					//abilityBarCenter->GetComponent<Sprite>()->SetColor(Color(0.2f, 0.6f, 1.0f));
					//abilityBarEnd->GetComponent<Sprite>()->SetColor(Color(0.2f, 0.6f, 1.0f));
					break;
				case Abilities::ABILITY_FLAMETHROWER:
					abilityBarCenter->GetComponent<Sprite>()->SetColor(Color(1.0f, 0.5f, 0.1f));
					abilityBarEnd->GetComponent<Sprite>()->SetColor(Color(1.0f, 0.5f, 0.1f));
					break;
				case Abilities::ABILITY_PROXIMITYMINE:
					//abilityBarCenter->GetComponent<Sprite>()->SetColor(Color(83.0f / 255.0f, 0.0f / 255.0f, 203.0f / 255.0f));
					//abilityBarEnd->GetComponent<Sprite>()->SetColor(Color(83.0f / 255.0f, 0.0f / 255.0f, 203.0f / 255.0f));
					abilityBarCenter->GetComponent<Sprite>()->SetColor(Color(1.0f, 1.0f, 0.2f));
					abilityBarEnd->GetComponent<Sprite>()->SetColor(Color(1.0f, 1.0f, 0.2f));
					break;
				}
			}
		}

		animHealth = Interpolate(animHealth, targetHealth, pow(0.35f, dt * 60.0f));
		animAbility = Interpolate(animAbility, targetAbility, pow(0.35f, dt * 60.0f));

		SetHUDObjectLocations();
	}

	// Removes any objects that will be recreated in Initialize.
	void HUD::Shutdown()
	{
	}

	// Sets the current player pointer.
	void HUD::SetPlayer(GameObject* player_)
	{
		player = player_->GetID();
		playerID = player_->GetComponent<PlayerMovement>()->GetPlayerID();
		health = player_->GetComponent<Health>();
		abilityHolder = player_->GetComponent<AbilityHolder>();
	}

	HUD::~HUD()
	{
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Sets the location for each HUD object
	void HUD::SetHUDObjectLocations()
	{
		BoundingRectangle screenDimensions = camera.GetScreenWorldDimensions();

		// Center for HUDOutline
		float x = screenDimensions.left + 3.5f;
		float y = screenDimensions.top - 1.5f;
		
		float flipx = 1.0f;
		if (playerID == 2)
			flipx = -1.0f;

		// HUD Outline
		hudBackground->GetComponent<Transform>()->SetScale(Vector2D(flipx * 6.0f, 2.0f));
		hudBackground->GetComponent<Transform>()->SetTranslation(Vector2D(flipx * x, y));

		float healthBarLeft = flipx * x + flipx * 0.465f - flipx * 2.125f;
		float healthBarWidth = flipx * 4.25f;

		healthBarBackgroundCenter->GetComponent<Transform>()->SetScale(Vector2D(healthBarWidth, 0.32f));
		healthBarBackgroundCenter->GetComponent<Transform>()->SetTranslation(Vector2D(healthBarLeft + healthBarWidth / 2.0f, y + 0.12f));
		healthBarBackgroundEnd->GetComponent<Transform>()->SetScale(Vector2D(flipx * 0.31f, 0.32f));
		healthBarBackgroundEnd->GetComponent<Transform>()->SetTranslation(Vector2D(healthBarLeft + healthBarWidth + flipx * 0.155f, y + 0.12f));

		healthBarWidth *= animHealth;

		healthBarCenter->GetComponent<Transform>()->SetScale(Vector2D(healthBarWidth, 0.32f));
		healthBarCenter->GetComponent<Transform>()->SetTranslation(Vector2D(healthBarLeft + healthBarWidth / 2.0f, y + 0.12f));
		healthBarEnd->GetComponent<Transform>()->SetScale(Vector2D(flipx * 0.31f, 0.32f));
		healthBarEnd->GetComponent<Transform>()->SetTranslation(Vector2D(healthBarLeft + healthBarWidth + flipx * 0.155f, y + 0.12f));

		float abilityBarLeft = flipx * x + flipx * 0.465f - flipx * 2.125f;
		float abilityBarWidth = flipx * 3.75f;

		abilityBarBackgroundCenter->GetComponent<Transform>()->SetScale(Vector2D(abilityBarWidth, 0.27f));
		abilityBarBackgroundCenter->GetComponent<Transform>()->SetTranslation(Vector2D(abilityBarLeft + abilityBarWidth / 2.0f, y - 0.37f));
		abilityBarBackgroundEnd->GetComponent<Transform>()->SetScale(Vector2D(flipx * 0.27f, 0.27f));
		abilityBarBackgroundEnd->GetComponent<Transform>()->SetTranslation(Vector2D(abilityBarLeft + abilityBarWidth + flipx * 0.135f, y - 0.37f));

		abilityBarWidth *= animAbility;

		abilityBarCenter->GetComponent<Transform>()->SetScale(Vector2D(abilityBarWidth, 0.27f));
		abilityBarCenter->GetComponent<Transform>()->SetTranslation(Vector2D(abilityBarLeft + abilityBarWidth / 2.0f, y - 0.37f));
		abilityBarEnd->GetComponent<Transform>()->SetScale(Vector2D(flipx * 0.27f, 0.27f));
		abilityBarEnd->GetComponent<Transform>()->SetTranslation(Vector2D(abilityBarLeft + abilityBarWidth + flipx * 0.135f, y - 0.37f));
	}

	// Gets the player pointer.
	GameObject* HUD::GetPlayer()
	{
		return static_cast<GameObject*>(BetaObject::GetObjectByID(player));
	}
}