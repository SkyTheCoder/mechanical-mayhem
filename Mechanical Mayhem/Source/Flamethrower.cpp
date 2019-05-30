//------------------------------------------------------------------------------
//
// File Name:	Flamethrower.cpp
// Author(s):	David Cohen (david.cohen)
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

#include "Flamethrower.h"

// Systems
#include <GameObject.h>
#include <Space.h>
#include <GameObjectManager.h>
#include <Parser.h>
#include <Input.h>

// Components
#include <Transform.h>
#include <Physics.h>
#include <Collider.h>
#include "PlayerMovement.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Abilities
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	Flamethrower::Flamethrower() : Ability("Flamethrower", true),
		transform(nullptr), physics(nullptr), collider(nullptr), playerController(nullptr), flameArchetype(nullptr), flameEffect(nullptr),
		speed(0.0f), cooldown(0.0f), cooldownTimer(0.0f),
		currentFuel(1.0f), maxFuel(1.0f), fuelRefillRate(0.5f), fuelConsumptionRate(1.0f)
	{
	}

	// Initialize this ability.
	void Flamethrower::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();
		collider = GetOwner()->GetComponent<Collider>();
		playerController = GetOwner()->GetComponent<Behaviors::PlayerMovement>();
		flameArchetype = GetOwner()->GetSpace()->GetObjectManager().GetArchetypeByName(flameArchetypeName);
		flameEffect = new GameObject(*GetOwner()->GetSpace()->GetObjectManager().GetArchetypeByName(flameEffectName));
		GetOwner()->GetSpace()->GetObjectManager().AddObject(*flameEffect);
	}

	// Clone the current ability.
	Ability* Flamethrower::Clone() const
	{
		return new Flamethrower(*this);
	}

	// Update function for this ability.
	// Params:
	//   dt = The change in time since the last update.
	void Flamethrower::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (Input::GetInstance().CheckHeld(playerController->GetUseKeybind()))
		{
			currentFuel = std::clamp(currentFuel - fuelConsumptionRate * dt, 0.0f, maxFuel);
		}
		else
		{
			currentFuel = std::clamp(currentFuel + fuelRefillRate * dt, 0.0f, maxFuel);
		}

		cooldownTimer -= dt;
	}

	// Updates components using a fixed timestep (usually just for physics).
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void Flamethrower::FixedUpdate(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Position the flame in front of the flamethrower (close enough)
		Vector2D offset(0.92f, -0.03f);

		if (playerController->GetPlayerID() == 2)
			offset = Vector2D(0.94f, 0.04f);

		flameEffect->GetComponent<Transform>()->SetTranslation(transform->GetTranslation() + Vector2D(std::signbit(transform->GetScale().x) ? -offset.x : offset.x, offset.y));
		flameEffect->GetComponent<Transform>()->SetScale(Vector2D(std::signbit(transform->GetScale().x) ? -1.0f : 1.0f, 1.0f));

		if (cooldownTimer > 0.0f)
			flameEffect->GetComponent<Sprite>()->SetAlpha(1.0f);
		else
			flameEffect->GetComponent<Sprite>()->SetAlpha(0.0f);
	}

	// Removes any objects that will be recreated in Initialize.
	void Flamethrower::Shutdown()
	{
		flameEffect->Destroy();
	}

	// Callback for when the player attempts to use this ability.
	void Flamethrower::OnUse()
	{
		// If we are out of fuel, don't do anything.
		if (currentFuel <= 0.0f)
			return;

		// If the ability is on cooldown, don't do anything.
		if (cooldownTimer > 0.0f)
			return;

		cooldownTimer = cooldown;

		// Create and place the new mine.
		GameObject* fireball = new GameObject(*flameArchetype);
		fireball->GetComponent<Transform>()->SetTranslation(transform->GetTranslation());
		Vector2D direction = Vector2D(std::signbit(transform->GetScale().x) ? -1.0f : 1.0f, 0.0f);

		fireball->GetComponent<Physics>()->SetVelocity(direction * speed);

		Collider* fireballCollider = fireball->GetComponent<Collider>();
		fireballCollider->SetGroup(collider->GetGroup());
		fireballCollider->SetMask(CM_GENERIC | CM_CREATE(collider->GetGroup()) | CM_HAZARD);
		GetOwner()->GetSpace()->GetObjectManager().AddObject(*fireball);

		flameEffect->GetComponent<Sprite>()->SetAlpha(1.0f);
	}

	// Returns the % of mana/fuel/uses/whatever left on this ability.
	float Flamethrower::GetMana()
	{
		return std::clamp(currentFuel / maxFuel, 0.0f, 1.0f);
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void Flamethrower::Serialize(Parser& parser) const
	{
		parser.WriteVariable("flameArchetype", flameArchetypeName);
		parser.WriteVariable("flameEffect", flameEffectName);
		parser.WriteVariable("speed", speed);
		parser.WriteVariable("cooldown", cooldown);
		parser.WriteVariable("currentFuel", currentFuel);
		parser.WriteVariable("maxFuel", maxFuel);
		parser.WriteVariable("fuelRefillRate", fuelRefillRate);
		parser.WriteVariable("fuelConsumptionRate", fuelConsumptionRate);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void Flamethrower::Deserialize(Parser& parser)
	{
		parser.ReadVariable("flameArchetype", flameArchetypeName);
		parser.ReadVariable("flameEffect", flameEffectName);
		parser.ReadVariable("speed", speed);
		parser.ReadVariable("cooldown", cooldown);
		parser.ReadVariable("currentFuel", currentFuel);
		parser.ReadVariable("maxFuel", maxFuel);
		parser.ReadVariable("fuelRefillRate", fuelRefillRate);
		parser.ReadVariable("fuelConsumptionRate", fuelConsumptionRate);
	}
}

//------------------------------------------------------------------------------
