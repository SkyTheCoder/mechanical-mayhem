//------------------------------------------------------------------------------
//
// File Name:	RisingGears.h
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h"

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Physics;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class RisingGears : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Default Constructor
		RisingGears();

		// Return a new copy of the component.
		Component* Clone() const;

		// Initialize data for this object.
		void Initialize();

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;

		// Receive an event and handle it (if applicable).
		// Params:
		//   event = The event that has been received.
		virtual void HandleEvent(const Event& event) override final;

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Speed of travel
		float moveSpeed;

		// Time before starting
		float startOffset;
		float timer;

		// Components
		Physics* physics;
	};
}

//------------------------------------------------------------------------------