//------------------------------------------------------------------------------
//
// File Name:	Health.cpp
// Author(s):	A.J. Bussman
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
#include "Health.h"

// Systems
#include <Space.h>
#include <Parser.h> // Read/WriteVariable
//#include <Event.h>
#include <Graphics.h>

// Components
#include <GameObject.h>
#include <Sprite.h>
#include <Collider.h>
#include "MonkeyAnimation.h"
#include "CameraFollow.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   maxHealth = The max health for the object.
	//   destroyOnDeath = hether the object should be destroyed when it dies.
	Health::Health(int maxHealth, bool destroyOnDeath) : Component("Health"),
		health(maxHealth), maxHealth(maxHealth), destroyOnDeath(destroyOnDeath), hasDeathAnimation(false)
	{
	}

	// Return a new copy of the component.
	Component* Health::Clone() const
	{
		return new Health(*this);
	}

	// Initialize data for this object.
	void Health::Initialize()
	{
	}

	// Update function for this component.
	// Params:
	//   dt = The change in time since the last step.
	void Health::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (health <= 0)
			GetOwner()->GetSpace()->GetObjectManager().DispatchEvent(new Event(ET_Death, "Death", 0.0f, GetOwner()->GetID()));
	}

	// Returns the current health
	int Health::GetHealth()
	{
		return health;
	}

	// Returns the current health
	int Health::GetMaxHealth()
	{
		return maxHealth;
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void Health::Serialize(Parser& parser) const
	{
		parser.WriteVariable("maxHealth", maxHealth);
		parser.WriteVariable("destroyOnDeath", destroyOnDeath);
		parser.WriteVariable("hasDeathAnimation", hasDeathAnimation);
		parser.WriteVariable("deathAnimationName", deathAnimationName);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void Health::Deserialize(Parser& parser)
	{
		parser.ReadVariable("maxHealth", maxHealth);
		parser.ReadVariable("destroyOnDeath", destroyOnDeath);
		parser.ReadVariable("hasDeathAnimation", hasDeathAnimation);
		parser.ReadVariable("deathAnimationName", deathAnimationName);
	}

	// Receive an event and handle it (if applicable).
	// Params:
	//   event = The event that has been received.
	void Health::HandleEvent(const Event& event)
	{
		// Handle Damage event
		if (event.type == ET_Damage)
		{
			const DamageEvent& damageEvent = static_cast<const DamageEvent&>(event);

			// Decrease health by the damage dealt
			health -= damageEvent.damage;
		}

		// Handle Heal event
		if (event.type == ET_Heal)
		{
			const HealEvent& healEvent = static_cast<const HealEvent&>(event);

			// Increase health by the health healed
			health += healEvent.health;
		}

		if (event.type == ET_Death && event.sender == GetOwner()->GetID() && destroyOnDeath)
		{
			GetOwner()->Destroy();

			if (hasDeathAnimation)
			{
				GameObjectManager& GOM = GetOwner()->GetSpace()->GetObjectManager();
				GameObject* deathAnimation = new GameObject(*GOM.GetArchetypeByName(deathAnimationName));

				// Get death animation
				deathAnimation->GetComponent<Sprite>()->SetSpriteSource(GetOwner()->GetComponent<Behaviors::MonkeyAnimation>()->GetDeathAnimation());

				// Set transform
				deathAnimation->GetComponent<Transform>()->SetTranslation(GetOwner()->GetComponent<Transform>()->GetTranslation());

				// Add to camera follow
				GOM.GetObjectByName("GameController")->GetComponent<CameraFollow>()->AddPlayer(deathAnimation);

				// Add to object manager
				GOM.AddObject(*deathAnimation);
			}
		}
	}
}
