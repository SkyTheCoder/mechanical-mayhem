//------------------------------------------------------------------------------
//
// File Name:	RisingGears.h
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
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
class Sprite;

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

		// Updates components using a fixed timestep (usually just for physics).
		// Params:
		//	 dt = A fixed change in time, usually 1/60th of a second.
		void FixedUpdate(float dt) override;

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;

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
		Sprite* sprite;
	};
}

//------------------------------------------------------------------------------