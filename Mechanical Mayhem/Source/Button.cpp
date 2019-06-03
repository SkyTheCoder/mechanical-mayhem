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

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Button.h"

// Systems
#include <ColliderRectangle.h>
#include <GameObject.h>
#include <Intersection2D.h>
#include <SoundManager.h>
#include <Engine.h>
#include <Input.h>
#include <Vector2D.h>
#include <Graphics.h>
#include <Space.h>
#include <Parser.h>

// Components
#include <Transform.h>
#include <Animation.h>

// Levels
#include "Level1.h"
#include "MainMenu.h"
#include "Controls.h"
#include "Credit.h"
#include "Lobby.h"
#include "LevelSelect.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Default constructor
	Button::Button() : Component("Button"), animation(nullptr), boundingRect(Vector2D(), Vector2D(50.0f, 25.0f)), map(Levels::Map::MAX_MAP), selected(false),
		unselectedIndex(0), unselectedLength(0), selectedIndex(0), selectedLength(0)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* Button::Clone() const
	{
		return new Button(*this);
	}

	// Initialize this component (happens at object creation).
	void Button::Initialize()
	{
		animation = GetOwner()->GetComponent<Animation>();
		boundingRect = BoundingRectangle(GetOwner()->GetComponent<Transform>()->GetTranslation(), 
			GetOwner()->GetComponent<ColliderRectangle>()->GetExtents());
	}

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file we want to read from.
	void Button::Deserialize(Parser& parser)
	{
		parser.ReadVariable("unselectedIndex", unselectedIndex);
		parser.ReadVariable("unselectedLength", unselectedLength);
		parser.ReadVariable("selectedIndex", selectedIndex);
		parser.ReadVariable("selectedLength", selectedLength);
	}

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file we want to write to.
	void Button::Serialize(Parser& parser) const
	{
		parser.WriteVariable("unselectedIndex", unselectedIndex);
		parser.WriteVariable("unselectedLength", unselectedLength);
		parser.WriteVariable("selectedIndex", selectedIndex);
		parser.WriteVariable("selectedLength", selectedLength);
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void Button::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();
		Vector2D mousePos =	Graphics::GetInstance().GetDefaultCamera().ScreenToWorldPosition(input.GetCursorPosition());

		if (selected)
		{
			if (input.CheckTriggered(VK_LBUTTON))
				OnPress();
		}
	}

	// Called when the button is pressed.
	void Button::OnPress()
	{
		// Play sound
		Engine::GetInstance().GetModule<SoundManager>()->PlaySound("menu Apress.wav");

		switch (map)
		{
		case Levels::Map::MainMenu:
			GetOwner()->GetSpace()->SetLevel(new Levels::MainMenu());
			break;
		case Levels::Map::Exit:
			Engine::GetInstance().Stop();
			break;
		case Levels::Map::ControlScreen:
			GetOwner()->GetSpace()->SetLevel(new Levels::Controls());
			break;
		case Levels::Map::Credits:
			GetOwner()->GetSpace()->SetLevel(new Levels::Credit());
			break;
		case Levels::Map::Lobby:
			GetOwner()->GetSpace()->SetLevel(new Levels::Lobby());
			break;
		case Levels::Map::LevelSelect:
			GetOwner()->GetSpace()->SetLevel(new Levels::LevelSelect());
			break;
		default:
			GetOwner()->GetSpace()->SetLevel(new Levels::Level1(map));
		}
	}

	// Returns the map the button loads
	Levels::Map Button::GetMap()
	{
		return map;
	}

	// Sets the map the button loads
	void Button::SetMap(Levels::Map map_)
	{
		map = map_;
	}

	// Sets whether the button is currently "selected," either by the mouse or a keyboard/controller.
	void Button::SetSelected(bool selected_)
	{
		selected = selected_;

		if (selected)
		{
			animation->Play(selectedIndex, selectedLength, 0.1f, true);
		}
		else
		{
			animation->Play(unselectedIndex, unselectedLength, 0.1f, true);
		}
	}

	// Returns whether the button is currently "selected," either by the mouse or a keyboard/controller.
	bool Button::IsSelected() const
	{
		return selected;
	}

	// Gets the bounding rectangle of this button.
	const BoundingRectangle& Button::GetBoundingRect() const
	{
		return boundingRect;
	}
}

//------------------------------------------------------------------------------
