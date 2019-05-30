//------------------------------------------------------------------------------
//
// File Name:	SimpleAnimator.cpp
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
#include "SimpleAnimator.h"

// Components
#include <Animation.h>

// Systems
#include <Space.h>
#include <Parser.h>

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
	//	 animationStart  = The starting frame for the animation.
	//   animationLength = The number of frames of the animation.
	//   animationSpeed  = The speed at which the animation plays
	//	 looping		 = Whether the animation loops or not
	SimpleAnimator::SimpleAnimator(unsigned animationStart, unsigned animationLength, float animationSpeed, bool looping)
		: Component("SimpleAnimator"),
		animationStart(animationStart), animationLength(animationLength), animationSpeed(animationSpeed), looping(looping)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* SimpleAnimator::Clone() const
	{
		return new SimpleAnimator(*this);
	}

	// Initialize this component (happens at object creation).
	void SimpleAnimator::Initialize()
	{
		// Play the animation
		GetOwner()->GetComponent<Animation>()->Play(animationStart, animationLength, animationSpeed, looping);
	}

	// Update SimpleAnimator.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void SimpleAnimator::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void SimpleAnimator::Serialize(Parser& parser) const
	{
		parser.WriteVariable("animationStart", animationStart);
		parser.WriteVariable("animationLength", animationLength);
		parser.WriteVariable("animationSpeed", animationSpeed);
		parser.WriteVariable("looping", looping);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void SimpleAnimator::Deserialize(Parser& parser)
	{
		parser.ReadVariable("animationStart", animationStart);
		parser.ReadVariable("animationLength", animationLength);
		parser.ReadVariable("animationSpeed", animationSpeed);
		parser.ReadVariable("looping", looping);
	}
}

//------------------------------------------------------------------------------
