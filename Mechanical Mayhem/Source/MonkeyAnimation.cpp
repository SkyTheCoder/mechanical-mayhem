//------------------------------------------------------------------------------
//
// File Name:	MonkeyAnimation.cpp
// Author(s):	David Cohen (david.cohen)
//				Daniel Walther (daniel.walther)
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

#include "MonkeyAnimation.h"

// Systems
#include <GameObject.h>
#include <Space.h>
#include <Input.h>

// Components
#include <Animation.h>
#include <Physics.h>
#include <Transform.h>
#include <SpriteSource.h>
#include "PlayerMovement.h"
#include "AbilityHolder.h"

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
	//	 walkStart  = The starting frame for the walk animation.
	//   walkLength = The number of frames of the walk animation.
	//   jumpStart  = The starting frame for the jump animation.
	//   jumpLength = The number of frames of the jump animation.
	//   idleStart  = The starting frame for the idle animation.
	//   idleLength = The number of frames of the idle animation.
	MonkeyAnimation::MonkeyAnimation(unsigned walkStart, unsigned walkLength,
		unsigned jumpStart, unsigned jumpLength, unsigned idleStart, unsigned idleLength)
		: Component("MonkeyAnimation"),
		walkStart(walkStart), walkLength(walkLength), jumpStart(jumpStart), jumpLength(jumpLength),
		idleStart(idleStart), idleLength(idleLength), currentState(StateIdle), nextState(StateIdle),
		anims{ nullptr }, jetpackJump(nullptr), jetpackFall(nullptr),
		animation(nullptr), physics(nullptr), transform(nullptr), sprite(nullptr), monkeyMovement(nullptr), abilityHolder(nullptr),
		originalScale()
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* MonkeyAnimation::Clone() const
	{
		return new MonkeyAnimation(*this);
	}

	// Initialize this component (happens at object creation).
	void MonkeyAnimation::Initialize()
	{
		// Store the required components for ease of access.
		animation = GetOwner()->GetComponent<Animation>();
		physics = GetOwner()->GetComponent<Physics>();
		transform = GetOwner()->GetComponent<Transform>();
		sprite = GetOwner()->GetComponent<Sprite>();
		monkeyMovement = GetOwner()->GetComponent<PlayerMovement>();
		abilityHolder = GetOwner()->GetComponent<AbilityHolder>();

		sprite->SetSpriteSource(anims[abilityHolder->GetAbilityType()]);

		// Play the idle animation.
		animation->Play(idleStart, 1, 0.0f, true);

		// Backup the original scale.
		originalScale = transform->GetScale();
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void MonkeyAnimation::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Update the sprite source to use the current ability.
		sprite->SetSpriteSource(anims[abilityHolder->GetAbilityType()]);

		// Choose the next state.
		ChooseNextState();

		// Update the current state if necessary.
		ChangeCurrentState();

		// Flip the sprite if necessary.
		FlipSprite();

		// Let the movement behavior reset its animation variables.
		monkeyMovement->AnimFinished(dt);
	}

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
	void MonkeyAnimation::SetFrames(unsigned walkStart_, unsigned walkLength_,
		unsigned jumpStart_, unsigned jumpLength_, unsigned fallStart_, unsigned fallLength_,
		unsigned idleStart_, unsigned idleLength_, unsigned wallSlideStart_, unsigned wallSlideLength_)
	{
		walkStart = walkStart_;
		walkLength = walkLength_;
		jumpStart = jumpStart_;
		jumpLength = jumpLength_;
		fallStart = fallStart_;
		fallLength = fallLength_;
		idleStart = idleStart_;
		idleLength = idleLength_;
		wallSlideStart = wallSlideStart_;
		wallSlideLength = wallSlideLength_;
	}

	// Tells this animation component to use the sprite source with the specified variant.
	void MonkeyAnimation::GetSpriteSources(std::string variant)
	{
		ResourceManager& resourceManager = GetOwner()->GetSpace()->GetResourceManager();

		anims[Abilities::ABILITY_NONE] = resourceManager.GetSpriteSource("Ani" + variant + ".png");
		anims[Abilities::ABILITY_JETPACK] = resourceManager.GetSpriteSource("AniJetpack" + variant + ".png");
		anims[Abilities::ABILITY_FLAMETHROWER] = resourceManager.GetSpriteSource("AniFlame" + variant + ".png");
		anims[Abilities::ABILITY_PROXIMITYMINE] = resourceManager.GetSpriteSource("AniMine" + variant + ".png");
		jetpackJump = resourceManager.GetSpriteSource("AniJetpackJumpFire" + variant + ".png");
		jetpackFall = resourceManager.GetSpriteSource("AniJetpackFallFire" + variant + ".png");
	}

	// Gets the appropriate jetpack flame sprite based on the current state.
	SpriteSource* MonkeyAnimation::GetCurrentJetpackSprite() const
	{
		if (currentState == State::StateFall)
			return jetpackFall;

		return jetpackJump;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Choose the correct state based on velocity.
	void MonkeyAnimation::ChooseNextState()
	{
		Input& input = Input::GetInstance();

		Vector2D velocity = physics->GetVelocity();

		// Check for wall-sliding.
		bool wallSliding = monkeyMovement->animOnLeftWall > 0.0f && input.CheckHeld(monkeyMovement->GetLeftKeybind()) || monkeyMovement->animOnRightWall > 0.0f && input.CheckHeld(monkeyMovement->GetRightKeybind());
		
		// If we are jumping/falling, use the jumping/falling state accordingly.
		if (monkeyMovement->airTime > 0.1f)
		{
			if (velocity.y > 0.0f)
			{
				nextState = State::StateJump;
			}
			else
			{
				nextState = State::StateFall;
			}
		}
		// If we are moving to the side, use the walking state.
		else if (!wallSliding && abs(velocity.x) > 0.75f)
		{
			nextState = State::StateWalk;
		}
		else if (!wallSliding || monkeyMovement->animOnGround > 0.0f && wallSliding)
		{
			// If we are standing still, use the idle state.
			nextState = State::StateIdle;
		}
		else
		{
			nextState = State::StateWallSlide;
		}
	}

	// Change states and start the appropriate animation.
	void MonkeyAnimation::ChangeCurrentState()
	{
		// Check if the state should change.
		if (currentState != nextState)
		{
			currentState = nextState;
			switch (currentState)
			{
			// If the state is changed to the idle state, begin playing the idle animation.
			case State::StateIdle:
				animation->Play(idleStart, idleLength, 0.2f, true);
				break;
			// If the state is changed to the walking state, begin playing the walking animation.
			case State::StateWalk:
				animation->Play(walkStart, walkLength, 0.06f, true); // 0.08f
				break;
			// If the state is changed to the jumping state, begin playing the jumping animation.
			case State::StateJump:
				animation->Play(jumpStart, jumpLength, 0.2f, true);
				break;
			// If the state is changed to the jumping state, begin playing the jumping animation.
			case State::StateFall:
				animation->Play(fallStart, fallLength, 0.2f, true);
				break;
			// If the state is changed to the wall-slide state, begin playing the wall-sliding animation.
			case State::StateWallSlide:
				animation->Play(wallSlideStart, wallSlideLength, 0.2f, true);
				break;
			}
		}
	}

	// Flip the sprite based on velocity and current state.
	void MonkeyAnimation::FlipSprite() const
	{
		// Check if we are moving.
		if (currentState != State::StateIdle)
		{
			Vector2D velocity = physics->GetVelocity();

			if (currentState != State::StateWallSlide && abs(velocity.x) <= 0.1f)
				return;

			float xScalar = 1.0f;

			// If we are moving left, flip the sprite on the X axis.
			if (velocity.x < 0.0f)
				xScalar *= -1.0f;

			if (currentState == State::StateWallSlide)
				xScalar *= -1.0f;

			transform->SetScale(Vector2D(xScalar * originalScale.x, originalScale.y));
		}
	}
}

//------------------------------------------------------------------------------
