//------------------------------------------------------------------------------
//
// File Name:	Button.h
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
#include <Shapes2D.h> // BoundingRectangle

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Animation;
class Parser;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	enum class Map : int;
}

namespace Behaviors
{
	class Button : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Default constructor
		Button();

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Loads object data from a file.
		// Params:
		//   parser = The parser for the file we want to read from.
		void Deserialize(Parser& parser) override;

		// Saves object data to a file.
		// Params:
		//   parser = The parser for the file we want to write to.
		void Serialize(Parser& parser) const override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Called when the button is pressed.
		void OnPress();

		// Returns the map the button loads
		Levels::Map GetMap();

		// Sets the map the button loads
		void SetMap(Levels::Map map);

		// Sets whether the button is currently "selected," either by the mouse or a keyboard/controller.
		void SetSelected(bool selected);

		// Returns whether the button is currently "selected," either by the mouse or a keyboard/controller.
		bool IsSelected() const;

		// Gets the bounding rectangle of this button.
		const BoundingRectangle& GetBoundingRect() const;

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Components
		Animation* animation;
		BoundingRectangle boundingRect;
		Levels::Map map;

		// Whether the button is currently "selected," either by the mouse or a keyboard/controller.
		bool selected;

		// Animation frame indexes.
		int unselectedIndex;
		int unselectedLength;
		int selectedIndex;
		int selectedLength;
	};
}

//------------------------------------------------------------------------------
