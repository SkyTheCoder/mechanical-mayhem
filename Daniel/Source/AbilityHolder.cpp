//------------------------------------------------------------------------------
//
// File Name:	AbilityHolder.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AbilityHolder.h"

// Systems
#include <Input.h>
#include <GameObject.h>

// Components
#include "PlayerMovement.h"

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
	AbilityHolder::AbilityHolder() : Component("AbilityHolder"), playerMovement(nullptr), abilityType(Abilities::ABILITY_NONE), ability(nullptr), timer(0.0f), duration(30.0f)
	{
		
	}
	
	// Copy constructor
	AbilityHolder::AbilityHolder(const AbilityHolder& other) : Component("AbilityHolder"), playerMovement(nullptr), abilityType(other.abilityType), ability(nullptr), timer(0.0f), duration(other.duration)
	{
		SetAbility(abilityType);
		timer = other.timer;
	}

	// Destructor
	AbilityHolder::~AbilityHolder()
	{
		SetAbility(Abilities::ABILITY_NONE);
	}

	// Assignment operator
	AbilityHolder& AbilityHolder::operator=(const AbilityHolder& rhs)
	{
		if (&rhs == this)
			return *this;

		SetAbility(rhs.abilityType);
		timer = rhs.timer;
		duration = rhs.duration;

		return *this;
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* AbilityHolder::Clone() const
	{
		return new AbilityHolder(*this);
	}

	// Initialize this component (happens at object creation).
	void AbilityHolder::Initialize()
	{
		playerMovement = GetOwner()->GetComponent<Behaviors::PlayerMovement>();
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void AbilityHolder::Update(float dt)
	{
		if (ability != nullptr)
		{
			ability->Update(dt);

			if (Input::GetInstance().CheckTriggered(playerMovement->GetUseKeybind()))
			{
				ability->OnUse();
			}

			timer += dt;

			if (timer >= duration)
			{
				SetAbility(Abilities::ABILITY_NONE);
			}
		}
	}

	// Sets the current ability.
	// Params:
	//   abilityType = The current ability.
	void AbilityHolder::SetAbility(Abilities::Abilities abilityType_)
	{
		if (ability != nullptr)
		{
			ability->Shutdown();
			ability->Unload();
			delete ability;
			ability = nullptr;
		}

		abilityType = abilityType_;
		timer = 0.0f;

		if (abilityType_ != Abilities::ABILITY_NONE)
		{
			ability = Abilities::Ability::FromType(abilityType_);

			ability->SetOwner(GetOwner());
			ability->Load();
			ability->Initialize();
		}
	}
}

//------------------------------------------------------------------------------
