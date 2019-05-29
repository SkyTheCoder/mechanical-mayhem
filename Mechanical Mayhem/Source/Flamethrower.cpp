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
		transform(nullptr), physics(nullptr), collider(nullptr), playerController(nullptr), fireballArchetype(nullptr),
		speed(0.0f), cooldown(0.0f), cooldownTimer(0.0f)
	{
	}

	// Initialize this ability.
	void Flamethrower::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();
		collider = GetOwner()->GetComponent<Collider>();
		playerController = GetOwner()->GetComponent<Behaviors::PlayerMovement>();
		fireballArchetype = GetOwner()->GetSpace()->GetObjectManager().GetArchetypeByName(fireballArchetypeName);
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

		cooldownTimer -= dt;
	}

	// Callback for when the player attempts to use this ability.
	void Flamethrower::OnUse()
	{
		// If the ability is on cooldown, don't do anything.
		if (cooldownTimer > 0.0f)
			return;

		cooldownTimer = cooldown;

		GameObjectManager& objectManager = GetOwner()->GetSpace()->GetObjectManager();

		std::vector<GameObject*> players;
		players.reserve(2);
		objectManager.GetAllObjectsByName("Player", players);

		GameObject* otherPlayer = nullptr;

		for (auto it = players.begin(); it != players.end(); ++it)
		{
			if ((*it)->GetComponent<Behaviors::PlayerMovement>()->GetID() != playerController->GetID())
			{
				otherPlayer = *it;
				break;
			}
		}

		// Create and place the new mine.
		GameObject* fireball = new GameObject(*fireballArchetype);
		fireball->GetComponent<Transform>()->SetTranslation(transform->GetTranslation());
		Vector2D direction;
		if (otherPlayer != nullptr)
		{
			direction = (otherPlayer->GetComponent<Transform>()->GetTranslation() - transform->GetTranslation()).Normalized();
		}
		else
		{
			direction = physics->GetVelocity().Normalized();
		}

		fireball->GetComponent<Physics>()->SetVelocity(direction * speed);

		Collider* fireballCollider = fireball->GetComponent<Collider>();
		fireballCollider->SetGroup(collider->GetGroup());
		fireballCollider->SetMask(CM_GENERIC | CM_CREATE(collider->GetGroup()) | CM_HAZARD);
		objectManager.AddObject(*fireball);
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void Flamethrower::Serialize(Parser& parser) const
	{
		parser.WriteVariable("fireballArchetype", fireballArchetypeName);
		parser.WriteVariable("speed", speed);
		parser.WriteVariable("cooldown", cooldown);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void Flamethrower::Deserialize(Parser& parser)
	{
		parser.ReadVariable("fireballArchetype", fireballArchetypeName);
		parser.ReadVariable("speed", speed);
		parser.ReadVariable("cooldown", cooldown);
	}
}

//------------------------------------------------------------------------------
