//------------------------------------------------------------------------------
//
// File Name:	PlayerMovement.h
// Author(s):	David Cohen (david.cohen) and A.J. Bussman (anthony.bussman)
// Project:		Yesterday's Mayonnaise
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

#include "Vector2D.h" // Vector2D

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
struct MapCollision;
class SoundManager;

namespace FMOD
{
	class Channel;
}

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class MonkeyAnimation;

	class PlayerMovement : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		PlayerMovement(unsigned keyUp = 0, unsigned keyLeft = 0, unsigned keyRight = 0, unsigned keyUse = 0);

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Handles movement.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Receive an event and handle it (if applicable).
		// Params:
		//   event = The event that has been received.
		void HandleEvent(const Event& event) override;

		// Sets the keybinds for the monkey.
		// Params:
		//   keyUp = The up keybind.
		//   keyLeft = The left keybind.
		//   keyRight = The right keybind.
		//	 keyUse = The use keybind.
		void SetKeybinds(unsigned keyUp, unsigned keyLeft, unsigned keyRight, unsigned keyUse);

		// Gets the keybind for jumping up.
		unsigned GetUpKeybind() const;

		// Gets the keybind for left
		unsigned GetLeftKeybind() const;

		// Gets the keybind for right
		unsigned GetRightKeybind() const;

		// Gets the keybind for using an ability.
		unsigned GetUseKeybind() const;

		// Sets the player's ID.
		// Params:
		//   newID = The ID to set to.
		void SetPlayerID(int newID);

		// Sets the player's ID.
		// Returns:
		//   The player's ID.
		int GetPlayerID() const;

		// Starts PowerUp Timer
		void StartPUTimer();

		// Determines whether the player is grounded
		bool IsOnGround() const;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Moves horizontally based on input
		void MoveHorizontal(float dt) const;

		// Moves vertically based on input
		void MoveVertical(float dt);

		// Called when the animation component is finished accessing variables.
		void AnimFinished(float dt);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Keybinds
		unsigned keyUp;
		unsigned keyLeft;
		unsigned keyRight;
		unsigned keyUse;

		// Movement properties
		float walkSpeed;
		float walkSpeedOld;
		Vector2D jumpSpeed;
		Vector2D jumpSpeedOld;
		Vector2D slidingJumpSpeed;
		Vector2D gravity;
		Vector2D slidingGravity;
		float terminalVelocity;
		float slidingTerminalVelocity;
		float gracePeriod;

		// Components
		Transform* transform;
		Physics* physics;

		// Sound manager
		SoundManager* soundManager;
		FMOD::Channel* slideSound;

		// Misc
		int playerID;
		int switchCharges;

		bool onGround;
		bool onLeftWall;
		bool onRightWall;
		float animOnGround;
		float animOnLeftWall;
		float animOnRightWall;
		bool hasJumped;
		float airTime;
		float leftTime;
		float rightTime;
		float movementLerpGround;
		float movementLerpAir;

		float stepTimer;

		friend class MonkeyAnimation;
	};
}

//------------------------------------------------------------------------------
