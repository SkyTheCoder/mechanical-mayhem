//------------------------------------------------------------------------------
//
// File Name:	Hazard.h
// Author(s):	A.J. Bussman
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
#include <Color.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class Hazard : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		Hazard();

		// Return a new copy of the component.
		Component* Clone() const;

		// Initializes the component
		void Initialize() override;

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
		void HandleEvent(const Event& event) override;

		// Returns if the hazard is collidable.
		bool IsCollidable();

		// Changes whether the hazard is collidable or not.
		// Params:
		//   collidable = Whether the hazard is collidable.
		void SetCollidable(bool collidable);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Properties
		bool alwaysCollidable;
		bool collidable;
		int damage;
		bool destroyOnCollide;
		float destroyOnCollideDelay;
		std::string destroyedArchetypeName;
		GameObject* destroyedArchetype;
	};
}