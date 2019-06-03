//------------------------------------------------------------------------------
//
// File Name:	DimensionShiftGearAnimation.h
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class

#include <Vector2D.h> // Vector2D

#include "Ability.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Animation;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class DimensionController;

	class DimensionShiftGearAnimation : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//	 redToBlueStart = The starting frame for fading from red to blue
		//	 blueToRedStart = The starting frame for fading from blue to red
		//	 fadeLength		= How many frames fading takes
		DimensionShiftGearAnimation(unsigned redToBlueStart = 0, unsigned blueToRedStart = 0, unsigned fadeLength = 0);

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Fixed update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Sets the frames for the animation.
		// Params:
		//	 redToBlueStart = The starting frame for fading from red to blue
		//	 blueToRedStart = The starting frame for fading from blue to red
			//	 fadeLength		= How many frames fading takes
		void SetFrames(unsigned redToBlueStart_, unsigned blueToRedStart_, unsigned fadeLength_);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Components
		Animation* animation;
		DimensionController* dimensionController;

		// Animation variables
		unsigned redToBlueStart;
		unsigned blueToRedStart;
		unsigned fadeLength;
		float animationTimer;

		// Misc
		bool isCurrentlyRed;
	};
}

//------------------------------------------------------------------------------
