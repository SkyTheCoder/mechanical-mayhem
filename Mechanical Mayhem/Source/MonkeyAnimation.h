//------------------------------------------------------------------------------
//
// File Name:	MonkeyAnimation.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

typedef class Animation Animation;
typedef class Physics Physics;
typedef class Transform Transform;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	typedef class PlayerMovement PlayerMovement;

	class MonkeyAnimation : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//	 walkStart  = The starting frame for the walk animation.
		//   walkLength = The number of frames of the walk animation.
		//   jumpStart  = The starting frame for the jump animation.
		//   jumpLength = The number of frames of the jump animation.
		//   idleStart  = The starting frame for the idle animation.
		//   idleLength = The number of frames of the idle animation.
		MonkeyAnimation(unsigned walkStart = 0, unsigned walkLength = 0, 
			unsigned jumpStart = 0, unsigned jumpLength = 0, unsigned idleStart = 0, unsigned idleLength = 0);

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
		//	 walkStart  = The starting frame for the walk animation.
		//   walkLength = The number of frames of the walk animation.
		//   jumpStart  = The starting frame for the jump animation.
		//   jumpLength = The number of frames of the jump animation.
		//   idleStart  = The starting frame for the idle animation.
		//   idleLength = The number of frames of the idle animation.
		//   wallSlideStart = The number of frames of the wallSlide animation.
		//   wallSlideLength = The number of frames of the wallSlide animation.
		void SetFrames(unsigned walkStart, unsigned walkLength,
			unsigned jumpStart, unsigned jumpLength, unsigned fallStart, unsigned fallLength,
			unsigned idleStart, unsigned idleLength, unsigned wallSlideStart, unsigned WallSlideLength);

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Choose the correct state based on velocity.
		void ChooseNextState();

		// Change states and start the appropriate animation.
		void ChangeCurrentState();

		// Flip the sprite based on velocity and current state.
		void FlipSprite() const;

		//------------------------------------------------------------------------------
		// Private Structures:
		//------------------------------------------------------------------------------

		enum State
		{
			StateIdle,
			StateWalk,
			StateJump,
			StateFall,
			StateWallSlide,
		};

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Animation variables
		unsigned walkStart;
		unsigned walkLength;
		unsigned jumpStart;
		unsigned jumpLength;
		unsigned fallStart;
		unsigned fallLength;
		unsigned idleStart;
		unsigned idleLength;
		unsigned wallSlideStart;
		unsigned wallSlideLength;

		// Animation state
		State currentState;
		State nextState;

		// Components
		Animation* animation;
		Physics* physics;
		Transform* transform;
		PlayerMovement* monkeyMovement;

		// Flip
		Vector2D originalScale;
	};
}

//------------------------------------------------------------------------------
