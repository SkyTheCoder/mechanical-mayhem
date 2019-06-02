//------------------------------------------------------------------------------
//
// File Name:	RisingGears.cpp
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
#include "RisingGears.h"

// Components
#include <Physics.h>
#include <Transform.h>

// Systems
#include <Engine.h>
#include <Event.h>
#include <Parser.h>
#include <SoundManager.h>
#include <Space.h>

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Default Constructor
	RisingGears::RisingGears()
		: Component("RisingGears"),
		moveSpeed(0), startOffset(0), timer(0.0f),
		physics(nullptr), sprite(nullptr)
	{
	}

	// Return a new copy of the component.
	Component* RisingGears::Clone() const
	{
		return new RisingGears(*this);
	}

	// Initialize data for this object.
	void RisingGears::Initialize()
	{
		// Get components
		//physics = GetOwner()->GetComponent<Physics>();
		sprite = GetOwner()->GetComponent<Sprite>();
	}

	// Updates components using a fixed timestep (usually just for physics).
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void RisingGears::FixedUpdate(float dt)
	{
		static bool start = false;

		// Check if we should move
		if (timer >= startOffset)
		{
			// Only play once
			if (!start)
			{
				Engine::GetInstance().GetModule<SoundManager>()->PlaySound("SoundHorn.wav");
				start = true;
			}

			// std::cout << "RisingGears start" << std::endl;
			GetOwner()->GetComponent<Transform>()->SetTranslation(GetOwner()->GetComponent<Transform>()->GetTranslation() + Vector2D(0.0f, moveSpeed * dt));
		}
		else
		{
			// Increment timer
			timer += dt;
		}
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void RisingGears::Serialize(Parser& parser) const
	{
		parser.WriteVariable("moveSpeed", moveSpeed);
		parser.WriteVariable("startOffset", startOffset);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void RisingGears::Deserialize(Parser& parser)
	{
		parser.ReadVariable("moveSpeed", moveSpeed);
		parser.ReadVariable("startOffset", startOffset);
	}
}

//------------------------------------------------------------------------------
