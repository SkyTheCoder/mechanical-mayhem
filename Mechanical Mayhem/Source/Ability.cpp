//------------------------------------------------------------------------------
//
// File Name:	Ability.cpp
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

#include "Ability.h"

// Systems
#include <Engine.h>
#include <Parser.h>

// Abilities
#include "Jetpack.h"
#include "Flamethrower.h"
#include "ProximityMine.h"

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
	// Params:
	//   name = The name of this ability.
	//   holdAbility = Whether the player can hold down the use key to use the ability.
	Ability::Ability(const std::string& name, bool holdAbility) : BetaObject(name), holdAbility(holdAbility), owner(nullptr)
	{
	}

	// Returns whether the player can hold down the use key to use the ability.
	bool Ability::IsHoldAbility() const
	{
		return holdAbility;
	}

	// Sets the owner of this ability.
	void Ability::SetOwner(GameObject* owner_)
	{
		owner = owner_;
	}

	// Gets the owner of this ability.
	GameObject* Ability::GetOwner() const
	{
		return owner;
	}

	// Creates a new Ability based on the type specified.
	// Params:
	//   abilityType = Which ability to create.
	Ability* Ability::FromType(Abilities abilityType)
	{
		Ability* ability = nullptr;

		switch (abilityType)
		{
		case ABILITY_JETPACK:
			ability = new Jetpack();
			break;
		case ABILITY_FLAMETHROWER:
			ability = new Flamethrower();
			break;
		case ABILITY_PROXIMITYMINE:
			ability = new ProximityMine();
			break;
		}

		if (ability != nullptr)
		{
			Parser parser(Engine::GetInstance().GetFilePath() + "Abilities/" + ability->GetName() + ".txt", std::fstream::in);
			parser.ReadSkip(ability->GetName());
			parser.ReadSkip('{');
			ability->Deserialize(parser);
			parser.ReadSkip('}');
		}

		return ability;
	}
}

//------------------------------------------------------------------------------
