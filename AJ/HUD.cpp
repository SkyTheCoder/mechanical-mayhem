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

// Component
#include "Health.h"
#include "Ability.h"
#include "AbilityHolder.h"
#include <Sprite.h>
#include <SpriteTextMono.h>

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	// Constructor
	HUD::HUD(GameObject* player, Vector2D translation) : BetaObject("HUD"), player(player), translation(translation), healthText(nullptr)
	{
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
		ResourceManager& resourceManager = static_cast<Space*>(GetOwner())->GetResourceManager();

		// HUD outline
		hud = ojectFactory.CreateObject("HUDOutline", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("HUDOutline.png"));
		// Player Icon
		playerIcon = ojectFactory.CreateObject("PlayerIcon", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("PlayerIcon.png"));

		// HealthText
		healthText = ojectFactory.CreateObject("HealthText", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman@2x.png"));

		// HealthBar
		healthBar.push_back(ojectFactory.CreateObject("HealthBarStart", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("HealthStart.png")));
		healthBar.push_back(ojectFactory.CreateObject("HealthBar",      resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("HealthBar.png")  ));
		healthBar.push_back(ojectFactory.CreateObject("HealthBarEnd",   resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("HealthEnd.png")  ));

		// AbilityBar
		abilityBar.push_back(ojectFactory.CreateObject("AbilityBarStart", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("AbilityStart.png")));
		abilityBar.push_back(ojectFactory.CreateObject("AbilityBar",      resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("AbilityBar.png")  )); 
		abilityBar.push_back(ojectFactory.CreateObject("AbilityBarEnd",   resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("AbilityEnd.png")  )); 

		// Set the Locations of the objects
		SetHUDObjectLocations();

		// Add HUD Objects
		GameObjectManager& objectManager = static_cast<Level*>(GetOwner())->GetSpace()->GetObjectManager();

		objectManager.AddObject(*hud);
		objectManager.AddObject(*playerIcon);
		objectManager.AddObject(*healthText);

		for (auto it = healthBar.begin(); it != healthBar.end(); ++it)
		{
			objectManager.AddObject(**it);
		}

		for (auto it = abilityBar.begin(); it != abilityBar.end(); ++it)
		{
			objectManager.AddObject(**it);
		}
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void HUD::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (player == nullptr)
			return;

		// Get player's health component
		Health* healthC = player->GetComponent<Health>();

		// Check if their health has changed
		if (prevHealth != healthC->GetHealth())
		{
			// Update health text
			healthText->GetComponent<SpriteTextMono>()->SetText(std::to_string(healthC->GetHealth() * 100.0f / healthC->GetMaxHealth()));

			// Displaying ends of healthBar
			if (healthC->GetHealth() < 1.0f)
				healthBar[2]->GetComponent<Sprite>()->SetAlpha(0.0f);
			else
				healthBar[2]->GetComponent<Sprite>()->SetAlpha(1.0f);
			
			if (AlmostEqual(static_cast<float>(healthC->GetHealth()), 0.0f))
				healthBar[0]->GetComponent<Sprite>()->SetAlpha(0.0f);
			else
				healthBar[0]->GetComponent<Sprite>()->SetAlpha(1.0f);


			// Get healthBar's transform
			Transform* health = healthBar[1]->GetComponent<Transform>();

			// Get and set new scale
			float normScale = health->GetScale().x / prevHealth;
			float newScale = normScale * healthC->GetHealth();
			health->SetScale(Vector2D(newScale, health->GetScale().y));


			// Create center offset for player 2
			Vector2D offset = Vector2D((healthC->GetHealth() - prevHealth) * normScale, 0.0f);
			// if player1 negate offset
			if (player->GetName() == "Player1")
				offset = Vector2D(-offset.x, 0.0f);
			// Set new healthBars translation
			health->SetTranslation(health->GetTranslation() + offset);

			// Set prevHealth to the current health
			prevHealth = static_cast<float>(healthC->GetHealth());
		}

		// Get players's ability component
		Abilities::Ability* ability = player->GetComponent<Behaviors::AbilityHolder>()->GetAbility();

		// Check if their mana has changed
		if (prevMana != ability->GetMana())
		{
			// Displaying ends of abilityBar
			if (ability->GetMana() < 1.0f)
				abilityBar[2]->GetComponent<Sprite>()->SetAlpha(0.0f);
			else
				abilityBar[2]->GetComponent<Sprite>()->SetAlpha(1.0f);

			if (AlmostEqual(ability->GetMana(), 0.0f))
				abilityBar[0]->GetComponent<Sprite>()->SetAlpha(0.0f);
			else
				abilityBar[0]->GetComponent<Sprite>()->SetAlpha(1.0f);

			// Get ability's transform
			Transform* mana = abilityBar[1]->GetComponent<Transform>();

			// Get and set new scale
			float normScale = mana->GetScale().x / prevMana;
			float newScale = normScale * ability->GetMana();
			mana->SetScale(Vector2D(newScale, mana->GetScale().y));

			// Create center offset for player 2
			Vector2D offset = Vector2D((ability->GetMana() - prevMana) * normScale, 0.0f);
			// if player1 negate offset
			if (player->GetName() == "Player1")
				offset = Vector2D(-offset.x, 0.0f);
			// Set new healthBars translation
			mana->SetTranslation(mana->GetTranslation() + offset);

			// Set prevMana to the current mana
			prevMana = ability->GetMana();
		}
	}

	// Removes any objects that will be recreated in Initialize.
	void HUD::Shutdown()
	{
		for (auto it = healthBar.begin(); it != healthBar.end(); ++it)
		{
			delete* it;
		}

		delete healthText;

		for (auto it = abilityBar.begin(); it != abilityBar.end(); ++it)
		{
			delete* it;
		}
	}

	// Sets the current player pointer.
	void HUD::SetPlayer(GameObject* player_)
	{
		player = player_;
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
		BoundingRectangle screenDimensions = Graphics::GetInstance().GetDefaultCamera().GetScreenWorldDimensions();
		// Center for HUDOutline
		float x = screenDimensions.left;
		float y = screenDimensions.top;
		
		float flipx = 1.0f;
		if (player->GetName() == "player2")
			flipx = -1.0f;

		// HUD Outline
		hud->GetComponent<Transform>()->SetTranslation(Vector2D(flipx * x, y));

		// Player 
		playerIcon->GetComponent<Transform>()->SetTranslation(Vector2D(flipx * x, y));
		
		// HealthText
		healthText->GetComponent<Transform>()->SetTranslation(Vector2D(flipx * x, y));

		// HealthBar
		healthBar[0]->GetComponent<Transform>()->SetTranslation(Vector2D(flipx * x, y));
		healthBar[1]->GetComponent<Transform>()->SetTranslation(Vector2D(flipx * x, y));
		healthBar[2]->GetComponent<Transform>()->SetTranslation(Vector2D(flipx * x, y));

		// AbilityBar
		abilityBar[0]->GetComponent<Transform>()->SetTranslation(Vector2D(flipx * x, y));
		abilityBar[1]->GetComponent<Transform>()->SetTranslation(Vector2D(flipx * x, y));
		abilityBar[2]->GetComponent<Transform>()->SetTranslation(Vector2D(flipx * x, y));
	}
}