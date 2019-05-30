//------------------------------------------------------------------------------
//
// File Name:	ProximityMine.cpp
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

#include "ProximityMine.h"

// Systems
#include <GameObject.h>
#include <Space.h>
#include <GameObjectManager.h>
#include <Parser.h>

// Components
#include <Transform.h>
#include <Collider.h>

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
	ProximityMine::ProximityMine() : Ability("ProximityMine", false), transform(nullptr), collider(nullptr), mineArchetype(nullptr),
		cooldown(0.0f), cooldownTimer(0.0f), maxProximityMines(4)
	{
	}

	// Initialize this ability.
	void ProximityMine::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		collider = GetOwner()->GetComponent<Collider>();
		mineArchetype = GetOwner()->GetSpace()->GetObjectManager().GetArchetypeByName(mineArchetypeName);
	}

	// Clone the current ability.
	Ability* ProximityMine::Clone() const
	{
		return new ProximityMine(*this);
	}

	// Update function for this ability.
	// Params:
	//   dt = The change in time since the last update.
	void ProximityMine::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		cooldownTimer -= dt;

		// Remove destroyed mines from the list.
		for (auto it = proximityMines.begin(); it != proximityMines.end();)
		{
			GameObject* mine = static_cast<GameObject*>(BetaObject::GetObjectByID(proximityMines.front()));
			if (mine == nullptr || mine->IsDestroyed())
				it = proximityMines.erase(it);
			else
				++it;
		}
	}

	// Shutdown this ability.
	void ProximityMine::Shutdown()
	{
		// Destroy all mines.
		while (proximityMines.size() > 0)
		{
			GameObject* mine = static_cast<GameObject*>(BetaObject::GetObjectByID(proximityMines.front()));
			if (mine != nullptr && !mine->IsDestroyed())
				mine->Destroy();
			proximityMines.pop_front();
		}
	}

	// Callback for when the player attempts to use this ability.
	void ProximityMine::OnUse()
	{
		// If the ability is on cooldown, don't do anything.
		if (cooldownTimer > 0.0f)
			return;

		cooldownTimer = cooldown;

		// If there are too many mines, remove the oldest one.
		if (proximityMines.size() >= maxProximityMines)
		{
			static_cast<GameObject*>(BetaObject::GetObjectByID(proximityMines.front()))->Destroy();
			proximityMines.pop_front();
		}

		GameObjectManager& objectManager = GetOwner()->GetSpace()->GetObjectManager();

		// Create and place the new mine.
		GameObject* mine = new GameObject(*mineArchetype);
		mine->GetComponent<Transform>()->SetTranslation(transform->GetTranslation());
		Collider* mineCollider = mine->GetComponent<Collider>();
		mineCollider->SetGroup(collider->GetGroup());
		mineCollider->SetMask(CM_GENERIC | CM_CREATE(collider->GetGroup()) | CM_HAZARD);
		objectManager.AddObject(*mine);
		proximityMines.push_back(mine->GetID());
	}

	// Returns the % of mana/fuel/uses/whatever left on this ability.
	float ProximityMine::GetMana()
	{
		return std::clamp(static_cast<float>(maxProximityMines - proximityMines.size()) / maxProximityMines, 0.0f, 1.0f);
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void ProximityMine::Serialize(Parser& parser) const
	{
		parser.WriteVariable("mineArchetype", mineArchetypeName);
		parser.WriteVariable("cooldown", cooldown);
		parser.WriteVariable("maxProximityMines", maxProximityMines);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void ProximityMine::Deserialize(Parser& parser)
	{
		parser.ReadVariable("mineArchetype", mineArchetypeName);
		parser.ReadVariable("cooldown", cooldown);
		parser.ReadVariable("maxProximityMines", maxProximityMines);
	}
}

//------------------------------------------------------------------------------
