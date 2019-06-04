//------------------------------------------------------------------------------
//
// File Name:	Jetpack.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Jetpack.h"

// Systems
#include <Engine.h>
#include <Input.h>
#include <ExtendedInput.h>
#include <Parser.h>
#include <SoundManager.h>
#include <Space.h>

// Components
#include <Transform.h>
#include <Physics.h>
#include <Collider.h>
#include "PlayerMovement.h"
#include "MonkeyAnimation.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Abilities
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Default Constructor
	Jetpack::Jetpack()
		: Ability("Jetpack", true),
		transform(nullptr), physics(nullptr), collider(nullptr), playerMovement(nullptr), monkeyAnimation(nullptr),
		flameArchetype(nullptr), flameEffect(nullptr),
		soundManager(nullptr), jetpackSound(nullptr),
		active(false),
		jetpackForce(1), flameSpeed(0.0f), flameCooldown(0.0f), flameCooldownTimer(0.0f),
		currentFuel(0), maxFuel(100),
		fuelRefillRate(0.5f), fuelConsumptionRate(1.0f)
	{
	}

	// Initialize data for this object.
	void Jetpack::Initialize()
	{
		// Get components
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();
		collider = GetOwner()->GetComponent<Collider>();
		playerMovement = GetOwner()->GetComponent<Behaviors::PlayerMovement>();
		monkeyAnimation = GetOwner()->GetComponent<Behaviors::MonkeyAnimation>();
		soundManager = Engine::GetInstance().GetModule<SoundManager>();

		// Fill fuel tank
		currentFuel = maxFuel;

		flameArchetype = new GameObject(*GetOwner()->GetSpace()->GetObjectManager().GetArchetypeByName(flameArchetypeName));
		flameEffect = new GameObject(*GetOwner()->GetSpace()->GetObjectManager().GetArchetypeByName(flameEffectName));
		GetOwner()->GetSpace()->GetObjectManager().AddObject(*flameEffect);
	}

	// Clone the current ability.
	Ability* Jetpack::Clone() const
	{
		return new Jetpack(*this);
	}

	// Update function for this ability.
	// Params:
	//   dt = The change in time since the last update.
	void Jetpack::Update(float dt)
	{
		// Check if user is even using the jetpack
		active = !playerMovement->IsOnGround() && (Input::GetInstance().CheckHeld(playerMovement->GetUseKeybind()) || ExtendedInput::GetInstance().GetRTrigger(playerMovement->GetControllerID()) > 0.0f);

		// Manage fuel amounts
		FuelManagement(dt);

		// Can't use jetpack if out of fuel
		if (currentFuel <= 0.0f)
		{
			active = false;
		}

		if (active)
		{
			// Add force if active
			physics->AddForce(Vector2D(0.0f, jetpackForce));

			if (jetpackSound == nullptr)
			{
				jetpackSound = soundManager->PlaySound("SoundJetp.wav");
				jetpackSound->setVolume(15.0f);
			}
		}
		else if (jetpackSound != nullptr)
		{
			// Fade volume down then stop
			//float volume;
			//jetpackSound->getVolume(&volume);
			//jetpackSound->setVolume(volume / 2.0f);
			//
			//if (volume <= 0.125f)
			{
				jetpackSound->stop();
			}

			jetpackSound = nullptr;
		}
	}

	// Updates components using a fixed timestep (usually just for physics).
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void Jetpack::FixedUpdate(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Position the flame under the jetpack (close enough)
		Vector2D offset(-0.0625f, -0.5f);

		if ((playerMovement->GetPlayerID() - 1) % 2 == 1)
			offset.y = -0.4f;

		// Thanks for scaling up everything, Kat.
		//offset *= 1.25f;

		flameEffect->GetComponent<Transform>()->SetTranslation(transform->GetTranslation() + Vector2D(std::signbit(transform->GetScale().x) ? -offset.x : offset.x, offset.y));
		flameEffect->GetComponent<Transform>()->SetScale(Vector2D(std::signbit(transform->GetScale().x) ? -1.25f : 1.25f, 1.25f));

		flameEffect->GetComponent<Sprite>()->SetSpriteSource(monkeyAnimation->GetCurrentJetpackSprite());

		if (IsActive())
			flameEffect->GetComponent<Sprite>()->SetAlpha(1.0f);
		else
			flameEffect->GetComponent<Sprite>()->SetAlpha(0.0f);

		flameCooldownTimer -= dt;

		if (IsActive())
		{
			if (flameCooldownTimer <= 0.0f)
			{
				// Create and place the new flame.
				GameObject* flame = new GameObject(*flameArchetype);
				flame->GetComponent<Transform>()->SetTranslation(transform->GetTranslation());
				flame->GetComponent<Physics>()->SetVelocity(Vector2D(0.0f, -flameSpeed));

				Collider* flameCollider = flame->GetComponent<Collider>();
				flameCollider->SetGroup(collider->GetGroup());
				flameCollider->SetMask(CM_GENERIC | CM_CREATE(collider->GetGroup()) | CM_HAZARD);
				GetOwner()->GetSpace()->GetObjectManager().AddObject(*flame);

				flameCooldownTimer = flameCooldown;
			}
		}
	}

	// Callback for when the player attempts to use this ability.
	void Jetpack::OnUse()
	{
	}

	// Returns the % of mana/fuel/uses/whatever left on this ability.
	float Jetpack::GetMana() const
	{
		return std::clamp(currentFuel / maxFuel, 0.0f, 1.0f);
	}

	// Sets whether the jetpack is active
	void Jetpack::SetActive(bool active_)
	{
		active = active_;
	}

	// Gets whether the jetpack is active
	bool Jetpack::IsActive() const
	{
		return active;
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void Jetpack::Serialize(Parser& parser) const
	{
		parser.WriteVariable("flameArchetype", flameArchetypeName);
		parser.WriteVariable("flameEffect", flameEffectName);
		parser.WriteVariable("jetpackForce", jetpackForce);
		parser.WriteVariable("flameSpeed", flameSpeed);
		parser.WriteVariable("flameCooldown", flameCooldown);
		parser.WriteVariable("currentFuel", currentFuel);
		parser.WriteVariable("maxFuel", maxFuel);
		parser.WriteVariable("fuelRefillRate", fuelRefillRate);
		parser.WriteVariable("fuelConsumptionRate", fuelConsumptionRate);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void Jetpack::Deserialize(Parser& parser)
	{
		parser.ReadVariable("flameArchetype", flameArchetypeName);
		parser.ReadVariable("flameEffect", flameEffectName);
		parser.ReadVariable("jetpackForce", jetpackForce);
		parser.ReadVariable("flameSpeed", flameSpeed);
		parser.ReadVariable("flameCooldown", flameCooldown);
		parser.ReadVariable("currentFuel", currentFuel);
		parser.ReadVariable("maxFuel", maxFuel);
		parser.ReadVariable("fuelRefillRate", fuelRefillRate);
		parser.ReadVariable("fuelConsumptionRate", fuelConsumptionRate);
	}

	// Shutdown function for jetpack
	void Jetpack::Shutdown()
	{
		flameEffect->Destroy();

		// Stop jetpack sound
		jetpackSound->stop();
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Handles fuel consumption/refilling
	void Jetpack::FuelManagement(float dt)
	{
		// Check if player is grounded
		if (playerMovement->IsOnGround())
		{
			// Check if fuel tank is not full
			if (currentFuel < maxFuel)
			{
				// Fill fuel tank at refill rate
				currentFuel += dt * fuelRefillRate;

				// Cap fuel amount at max
				if (currentFuel > maxFuel)
				{
					currentFuel = maxFuel;
				}
			}
		}
		// Not on ground and jetpack is active
		else if (active)
		{
			// Check if fuel tank is not empty
			if (currentFuel > 0.0f)
			{
				// Empty fuel at consumption rate
				currentFuel -= dt * fuelConsumptionRate;

				// Don't go below zero fuel
				if (currentFuel < 0.0f)
				{
					currentFuel = 0.0f;
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
