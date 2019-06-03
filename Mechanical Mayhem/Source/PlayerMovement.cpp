//------------------------------------------------------------------------------
//
// File Name:	PlayerMovement.cpp
// Author(s):	David Cohen (david.cohen) and A.J. Bussman (anthony.bussman)
// Project:		Yesterday's Mayonnaise
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PlayerMovement.h"

// Systems
#include <Engine.h>
#include "GameObject.h"
#include <Input.h>
#include <SoundManager.h>
#include "Space.h"
#include <Interpolation.h>
#include <ExtendedInput.h>
#include <Random.h>

// Components
#include "Transform.h"
#include "Physics.h"
#include "Collider.h"
#include "DimensionController.h"
#include "ShiftPickup.h"
#include "AbilityHolder.h"
#include "AbilityPickup.h"

// Abilities
#include "Jetpack.h"

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
	PlayerMovement::PlayerMovement() : Component("PlayerMovement"),
		walkSpeed(5.0f), walkSpeedOld(walkSpeed), jumpSpeed(0.0f, 11.0f), jumpSpeedOld(jumpSpeed), slidingJumpSpeed(6.0f, 6.75f),
		gravity(0.0f, -16.0f), slidingGravity(0.0f, -9.0f), terminalVelocity(9.0f), slidingTerminalVelocity(1.5f), gracePeriod(0.15f),
		transform(nullptr), physics(nullptr), abilityHolder(nullptr),
		soundManager(nullptr), slideSound(nullptr),
		playerID(0), switchCharges(0),
		onGround(false), onLeftWall(false), onRightWall(false), animOnGround(0.0f), animOnLeftWall(0.0f), animOnRightWall(0.0f),
		hasJumped(false), airTime(0.0f), leftTime(0.0f), rightTime(0.0f), movementLerpGround(1.0f), movementLerpAir(0.98f), jumpCancelFactor(0.7f),
		stepTimer(0.0f)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerMovement::Clone() const
	{
		return new PlayerMovement(*this);
	}

	// Initialize this component (happens at object creation).
	void PlayerMovement::Initialize()
	{
		// Store the required components for ease of access.
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();
		abilityHolder = GetOwner()->GetComponent<AbilityHolder>();

		// Add sounds
		soundManager = Engine::GetInstance().GetModule<SoundManager>();
	}

	// Handles movement.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerMovement::Update(float dt)
	{
		// Handle horizontal movement.
		MoveHorizontal(dt);

		// Handle vertical movement.
		MoveVertical(dt);

		GameObject* gameController = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("GameController");
		DimensionController& dimensionController = *static_cast<DimensionController*>(gameController->GetComponent("DimensionController"));

		// Switch dimensions when pressing the key, there are unused charges, and dimension switching is not on cooldown.
		Input& input = Input::GetInstance();
		if (input.IsKeyDown(inputScheme.keyUse) && switchCharges > 0 && dimensionController.GetSwitchCooldown() <= 0.0f)
		{
			// Calculate next dimension ID
			unsigned newDimension = (dimensionController.GetActiveDimension() + 1) % dimensionController.GetDimensionCount();
			dimensionController.SetActiveDimension(newDimension);
			--switchCharges;
		}
	}

	// Receive an event and handle it (if applicable).
	// Params:
	//   event = The event that has been received.
	void PlayerMovement::HandleEvent(const Event& event)
	{
		GameObject& other = *static_cast<GameObject*>(event.GetSender());
		
		if (event.name == "CollisionStarted")
		{
			// Handle various pickups.

			// If the switch pickup is active, get more switcheroos.
			ShiftPickup* shiftPickup = other.GetComponent<ShiftPickup>();
			if (shiftPickup != nullptr && shiftPickup->IsActive())
			{
				switchCharges += shiftPickup->GetCharges();
			}

			// If the ability pickup is active, switch to that ability.
			AbilityPickup* abilityPickup = other.GetComponent<AbilityPickup>();
			if (abilityPickup != nullptr && abilityPickup->IsActive())
			{
				abilityHolder->SetAbility(abilityPickup->GetAbilityType());
			}
		}

		if (event.name == "MapCollisionStarted" || event.name == "MapCollisionPersisted")
		{
			const MapCollisionEvent& mapCollisionEvent = static_cast<const MapCollisionEvent&>(event);

			// If the monkey's collider is colliding on the bottom, mark the monkey as on ground.
			if (mapCollisionEvent.collision.bottom)
			{
				// Play landing sound
				if (airTime > 0.1f)
				{
					FMOD::Channel* landing = soundManager->PlaySound("Landing final.wav");
					landing->setVolume(10.0f);
					landing->setPitch(RandomRange(1.0f, 2.0f));

					// Delay step sound by a bit
					// stepTimer = -0.5f;
				}

				onGround = true;
				animOnGround = 3.5f / 60.0f;
			}

			// Save whether the monkey is touching a wall, used for wall jumping.

			if (mapCollisionEvent.collision.left)
			{
				onLeftWall = true;
				animOnLeftWall = 3.5f / 60.0f;
			}

			if (mapCollisionEvent.collision.right)
			{
				onRightWall = true;
				animOnRightWall = 3.5f / 60.0f;
			}
		}
	}

	// Sets the input scheme for the player.
	void PlayerMovement::SetInputScheme(const InputScheme& scheme)
	{
		inputScheme = scheme;
	}

	// Gets the keybind for jumping up.
	unsigned PlayerMovement::GetUpKeybind() const
	{
		return inputScheme.keyUp;
	}

	// Gets the keybind for left
	unsigned PlayerMovement::GetLeftKeybind() const
	{
		return inputScheme.keyLeft;
	}

	// Gets the keybind for right
	unsigned PlayerMovement::GetRightKeybind() const
	{
		return inputScheme.keyRight;
	}

	// Gets the keybind for using an ability.
	unsigned PlayerMovement::GetUseKeybind() const
	{
		return inputScheme.keyUse;
	}

	// Sets the player's ID.
	void PlayerMovement::SetPlayerID(int newID)
	{
		playerID = newID;
	}

	// Sets the player's ID.
	int PlayerMovement::GetPlayerID() const
	{
		return playerID;
	}

	// Sets the ID of the controller tied to this player.
	void PlayerMovement::SetControllerID(int newID)
	{
		inputScheme.controllerID = newID;
	}

	// Gets the ID of the controller tied to this player.
	int PlayerMovement::GetControllerID() const
	{
		return inputScheme.controllerID;
	}

	// Determines whether the player is grounded
	bool PlayerMovement::IsOnGround() const
	{
		return airTime <= 3.5f / 60.0f;//1e-6f;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Moves horizontally based on input
	void PlayerMovement::MoveHorizontal(float dt) const
	{
		Input& input = Input::GetInstance();

		Vector2D velocity = physics->GetVelocity();

		// Initialize target velocity to 0.
		float targetVelocityX = std::clamp(ExtendedInput::GetInstance().GetLThumb(inputScheme.controllerID).x * 2.0f, -1.0f, 1.0f);

		// If the right arrow key is pressed, move to the right.
		if (input.IsKeyDown(inputScheme.keyRight))
		{
			targetVelocityX += 1.0f;
		}

		// If the right arrow key is pressed, move to the left.
		if (input.IsKeyDown(inputScheme.keyLeft))
		{
			targetVelocityX -= 1.0f;
		}

		targetVelocityX = std::clamp(targetVelocityX, -1.0f, 1.0f);

		targetVelocityX *= walkSpeed;

		// Smoothly interpolate the X component of the player's velocity.
		float movementMix = 1.0f - pow(1.0f - (airTime < 1e-6f ? movementLerpGround : movementLerpAir), dt);
		velocity.x = Interpolate(velocity.x, targetVelocityX, movementMix);

		// Set the velocity.
		physics->SetVelocity(velocity);
	}

	// Moves vertically based on input
	void PlayerMovement::MoveVertical(float dt)
	{
		Input& input = Input::GetInstance();
		ExtendedInput& extendedInput = ExtendedInput::GetInstance();

		Vector2D velocity = physics->GetVelocity();

		// Reset time since touching walls.
		if (onLeftWall && !onRightWall)
			leftTime = 0.0f;
		else
			leftTime += dt;

		if (onRightWall && !onLeftWall)
			rightTime = 0.0f;
		else
			rightTime += dt;

		bool onlyLeftWall = leftTime <= gracePeriod;
		bool onlyRightWall = rightTime <= gracePeriod;
		bool isSliding = (onlyLeftWall || onlyRightWall) && !onGround;

		if (onGround || (onLeftWall && !onRightWall) || (onRightWall && !onLeftWall))
		{
			// Reset time spent in the air.
			airTime = 0.0f;

			// The player has not jumped since leaving the ground.
			hasJumped = false;
		}
		else
		{
			// Keep track of time spent in the air.
			airTime += dt;
		}

		bool canJump = airTime <= gracePeriod || onlyLeftWall || onlyRightWall;

		// If the monkey has not jumped since landing, was on the ground recently, and the up arrow key is pressed, jump.
		if (!hasJumped && canJump && (extendedInput.IsXBDown(XB_A, inputScheme.controllerID) || input.IsKeyDown(inputScheme.keyUp)))
		{
			if (isSliding)
			{
				// Increase Y velocity.
				velocity.y = slidingJumpSpeed.y;

				if (onlyLeftWall)
				{
					velocity.x = slidingJumpSpeed.x;
				}
				else if (onlyRightWall)
				{
					velocity.x = -slidingJumpSpeed.x;
				}

				// Hacky fix so ground movement lerp isn't used immediately after jumping off walls.
				airTime = 1e-6f;
			}
			else
			{
				// Increase Y velocity.
				velocity.y = jumpSpeed.y;
			}

			// Handle sliding sounds
			if (isSliding)
			{
				FMOD::Channel* walloff = soundManager->PlaySound("walloff.wav");
				walloff->setVolume(7.0f);
				walloff->setPitch(RandomRange(1.0f, 2.0f));
			}
			else
			{
				FMOD::Channel* jump = soundManager->PlaySound("jump.wav");
				jump->setVolume(7.0f);
				jump->setPitch(RandomRange(1.0f, 2.0f));
			}

			hasJumped = true;
		}
		
		// Apply gravity if in air.
		if (!onGround)
		{
			// Use different gravity when sliding
			if (isSliding)
			{
				if (velocity.y > 0.0f)
					physics->AddForce(gravity);
				else
					physics->AddForce(slidingGravity);
			}
			else
			{
				physics->AddForce(gravity);
			}

			if (velocity.y > 0.0f)
			{
				if (!(abilityHolder->GetAbilityType() == Abilities::ABILITY_JETPACK
					&& static_cast<Abilities::Jetpack*>(abilityHolder->GetAbility())->IsActive()))
				{
					if (input.CheckReleased(inputScheme.keyUp) || extendedInput.CheckXBReleased(XB_A, inputScheme.controllerID))
					{
						velocity.y *= jumpCancelFactor;
					}
				}
			}
		}

		// Clamp velocity.
		// Use different terminal velocity depending on whether the monkey is sliding.
		if (isSliding)
		{
			velocity.y = max(-slidingTerminalVelocity, velocity.y);
		}
		else
		{
			velocity.y = max(-terminalVelocity, velocity.y);
		}

		bool movingLeft = extendedInput.GetLThumb(inputScheme.controllerID).x < 0.0f || input.IsKeyDown(inputScheme.keyLeft);
		bool movingRight = extendedInput.GetLThumb(inputScheme.controllerID).x > 0.0f || input.IsKeyDown(inputScheme.keyRight);

		if (animOnGround <= 0.0f && (animOnLeftWall > 0.0f && movingLeft || animOnRightWall > 0.0f && movingRight))
		{
			// Sliding sound
			if (slideSound == nullptr)
			{
				slideSound = soundManager->PlaySound("wallslide.wav");
				slideSound->setVolume(10.0f);
			}
		}
		else if (slideSound != nullptr)
		{
			// Stop sliding sound
			slideSound->stop();
			slideSound = nullptr;
		}

		// Set the velocity.
		physics->SetVelocity(velocity);

		// Handle step audio
		if (airTime < 0.1f && (movingRight || movingLeft) && !isSliding)
		{
			stepTimer += dt;
			if (stepTimer > 0.25f)
			{
				stepTimer = 0.0f;
				FMOD::Channel* step = soundManager->PlaySound("step.wav");
				step->setVolume(25.0f);
				step->setPitch(RandomRange(1.0f, 2.0f));
			}
		}

		onGround = false;
		onLeftWall = false;
		onRightWall = false;
	}

	// Called when the animation component is finished accessing variables.
	void PlayerMovement::AnimFinished(float dt)
	{
		animOnGround -= dt;
		animOnLeftWall -= dt;
		animOnRightWall -= dt;
	}
}

//------------------------------------------------------------------------------
