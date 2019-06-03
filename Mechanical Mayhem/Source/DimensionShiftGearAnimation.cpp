//------------------------------------------------------------------------------
//
// File Name:	DimensionShiftGearAnimation.cpp
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

#include "DimensionShiftGearAnimation.h"

// Systems
#include <GameObjectManager.h>
#include <GameObject.h>
#include <Space.h>

// Components
#include <Animation.h>
#include "DimensionController.h"

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
	//	 redToBlueStart = The starting frame for fading from red to blue
	//	 blueToRedStart = The starting frame for fading from blue to red
	//	 fadeLength		= How many frames fading takes
	DimensionShiftGearAnimation::DimensionShiftGearAnimation(unsigned redToBlueStart, unsigned blueToRedStart, unsigned fadeLength)
		: Component("DimensionShiftGearAnimation"), redToBlueStart(redToBlueStart), blueToRedStart(blueToRedStart), fadeLength(fadeLength), isCurrentlyRed(true), animationTimer(0.25f)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* DimensionShiftGearAnimation::Clone() const
	{
		return new DimensionShiftGearAnimation(*this);
	}

	// Initialize this component (happens at object creation).
	void DimensionShiftGearAnimation::Initialize()
	{
		// Store the required components for ease of access.
		animation = GetOwner()->GetComponent<Animation>();
		dimensionController = GetOwner()->GetSpace()->GetLevel()->GetAltSpace()->GetObjectManager().GetObjectByName("GameController")->GetComponent<DimensionController>();
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void DimensionShiftGearAnimation::Update(float dt)
	{
		if (dimensionController->GetCoolDown() <= 0.25f)
		{
			// Determine next switch
			if (animationTimer > 0.25f)
			{
				// Play proper animation
				if (isCurrentlyRed)
				{
					isCurrentlyRed = false;
					animation->Play(redToBlueStart, fadeLength, 0.05f, false);
					animationTimer = 0.0f;
				}
				else
				{
					isCurrentlyRed = true;
					animation->Play(blueToRedStart, fadeLength, 0.05f, false);
					animationTimer = 0.0f;
				}
			}

			animationTimer += dt;
		}
	}

	// Sets the frames for the animation.
	// Params:
	//	 redToBlueStart = The starting frame for fading from red to blue
	//	 blueToRedStart = The starting frame for fading from blue to red
	//	 fadeLength		= How many frames fading takes
	void DimensionShiftGearAnimation::SetFrames(unsigned redToBlueStart_, unsigned blueToRedStart_, unsigned fadeLength_)
	{
		redToBlueStart = redToBlueStart_;
		blueToRedStart = blueToRedStart_;
		fadeLength = fadeLength_;
	}
}

//------------------------------------------------------------------------------
