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

// Components
#include "PlayerMovement.h"
#include <Physics.h>

// Systems
#include <Engine.h>
#include <Input.h>
#include <Parser.h>
#include <SoundManager.h>
#include <Space.h>

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
		playerMovement(nullptr), physics(nullptr),
		flameEffect(nullptr),
		active(false), jetpackForce(1), currentFuel(0), maxFuel(100),
		fuelRefillRate(0.5f), fuelConsumptionRate(1.0f)
	{
	}

	// Initialize data for this object.
	void Jetpack::Initialize()
	{
		// Get components
		playerMovement = GetOwner()->GetComponent<Behaviors::PlayerMovement>();
		physics = GetOwner()->GetComponent<Physics>();
		soundManager = Engine::GetInstance().GetModule<SoundManager>();

		// Fill fuel tank
		currentFuel = maxFuel;

		//flameEffect = new GameObject(*GetOwner()->GetSpace()->GetObjectManager().GetArchetypeByName(flameEffectName));
		//GetOwner()->GetSpace()->GetObjectManager().AddObject(*flameEffect);
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
		active = Input::GetInstance().CheckHeld(playerMovement->GetUpKeybind());

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
		/*Vector2D offset(0.92f, -0.03f);

		if (playerController->GetPlayerID() == 2)
			offset = Vector2D(0.94f, 0.04f);

		flameEffect->GetComponent<Transform>()->SetTranslation(transform->GetTranslation() + Vector2D(std::signbit(transform->GetScale().x) ? -offset.x : offset.x, offset.y));
		flameEffect->GetComponent<Transform>()->SetScale(Vector2D(std::signbit(transform->GetScale().x) ? -1.0f : 1.0f, 1.0f));

		if (cooldownTimer > 0.0f)
			flameEffect->GetComponent<Sprite>()->SetAlpha(1.0f);
		else
			flameEffect->GetComponent<Sprite>()->SetAlpha(0.0f);*/
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
		parser.WriteVariable("jetpackForce", jetpackForce);
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
		parser.ReadVariable("jetpackForce", jetpackForce);
		parser.ReadVariable("currentFuel", currentFuel);
		parser.ReadVariable("maxFuel", maxFuel);
		parser.ReadVariable("fuelRefillRate", fuelRefillRate);
		parser.ReadVariable("fuelConsumptionRate", fuelConsumptionRate);
	}

	// Shutdown function for jetpack
	void Jetpack::Shutdown()
	{
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
