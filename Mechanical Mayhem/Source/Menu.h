//------------------------------------------------------------------------------
//
// File Name:	Menu.h
// Author(s):	David Cohen (david.cohen)
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

#include <Vector2D.h>
#include <Level.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declaration:
//------------------------------------------------------------------------------

class GameObject;

namespace Behaviors
{
	class Button;
}

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	enum class Map : int;

	class Menu : virtual public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Menu.
		Menu();

		// Load the resources associated with Menu.
		void Load() override;

		// Sets the camera position.
		void FixCamera();

		// Update Menu.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Unload the resources associated with Menu.
		void Unload() override;

	protected:
		//------------------------------------------------------------------------------
		// Protected Structures:
		//------------------------------------------------------------------------------

		struct MenuButton
		{
			// Constructor.
			MenuButton(GameObject* object = nullptr);

			// Sets whether the button is currently "selected," either by the mouse or a keyboard/controller.
			void SetSelected(bool selected);

			GameObject* object;
			Behaviors::Button* button;

			MenuButton* north;
			MenuButton* east;
			MenuButton* south;
			MenuButton* west;
		};

		//------------------------------------------------------------------------------
		// Protected Functions:
		//------------------------------------------------------------------------------

		// Adds a new map button.
		// Params:
		//   name = The name of the level.
		//   position = The position of the button.
		//   map = The map the button should switch to.
		// Returns:
		//   A pointer to the MenuButton created.
		MenuButton* AddMenuButton(const char* name, Vector2D position, Map map);

		// Sets the current menu button.
		void SetDefaultButton(MenuButton* button);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		MenuButton* currentButton;
		MenuButton* defaultButton;

		Vector2D lastCursor;

		std::vector<MenuButton*> menuButtons;
	};
}

//----------------------------------------------------------------------------
