//------------------------------------------------------------------------------
//
// File Name:	DimensionController.h
// Author(s):	David Cohen (david.cohen), A.J. Bussman
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
#include <vector> // std::vector

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class DimensionController : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		DimensionController();

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void FixedUpdate(float dt) override;

		// Calculates how long until the dimension can be switched again.
		// Returns:
		//   How much longer until the dimension can be switched.
		float GetSwitchCooldown() const;

		// Sets the active dimension and deactivates all others.
		// Params:
		//   dimension = The dimension to make active.
		void SetActiveDimension(unsigned dimension);

		// Returns the active dimension.
		unsigned GetActiveDimension() const;

		// Returns the number of dimensions.
		unsigned GetDimensionCount() const;

		// Adds a new dimension.
		// Params:
		//   tilemap = The game object with the tilemap for the dimension.
		// Returns:
		//   The ID of the new dimension.
		unsigned AddDimension(GameObject* tilemap);

		// Adds a spike to an existing dimension.
		// Params:
		//   dimension = The ID of the dimension to add the spike to.
		//   spike = The game object for the spike.
		void AddSpikeToDimension(unsigned dimension, GameObject* spike);

	private:
		//------------------------------------------------------------------------------
		// Private Structures:
		//------------------------------------------------------------------------------

		struct Dimension
		{
			// Constructor
			// Params:
			//   tilemap = The game oject with the tilemap for the dimension.
			Dimension(GameObject* tilemap);

			GameObject* tilemap;
			std::vector<GameObject*> spikes;
		};

		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Sets the cooldown time
		void SetCoolDownTime();

		// Returns if a NeedsNewCooldown is needed
		bool NeedsNewCooldown();

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Misc
		float cooldown;
		float currentCooldown;
		int cdIndex;
		int cdCount;
		float cdCounts[11];
		double gameTimer;
		unsigned activeDimension;
		std::vector<Dimension> dimensions;
	};
}

//------------------------------------------------------------------------------
