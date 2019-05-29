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
#include "ColliderRectangle.h"
#include <GameObject.h>
#include <Intersection2D.h>
#include <SoundManager.h>
#include <Engine.h>
#include <Input.h>
#include <Vector2D.h>
#include <Graphics.h>
#include <Space.h>

// Components
#include "Sprite.h"
#include "Transform.h"

// Levels
#include "Level1.h"
#include "MainMenu.h"
#include "Controls.h"
#include "Credit.h"
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
	Button::Button() : Component("Button"), map(Levels::Map::Arena3), boundingRact(Vector2D(), Vector2D(50.0f, 25.0f))
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
		sprite = static_cast<Sprite*>(GetOwner()->GetComponent("Sprite"));
		boundingRact = BoundingRectangle(static_cast<Transform*>(GetOwner()->GetComponent("Transform"))->GetTranslation(), 
			static_cast<ColliderRectangle*>(GetOwner()->GetComponent("Collider"))->GetExtents());
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void Button::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();
		Vector2D mousePos =	Graphics::GetInstance().GetDefaultCamera().ScreenToWorldPosition(input.GetCursorPosition());

		if (PointRectangleIntersection(mousePos, boundingRact))
		{
			sprite->SetColor(Color(0.4f, 0.4f, 0.4f));
			if (input.CheckTriggered(VK_LBUTTON))
			{
				// Play sound
				Engine::GetInstance().GetModule<SoundManager>()->PlaySound("menu Apress.wav");

				switch (map)
				{
				case Levels::Map::MainMenu:
					GetOwner()->GetSpace()->SetLevel(new Levels::MainMenu());
					break;
				case Levels::Map::ControlScreen:
					GetOwner()->GetSpace()->SetLevel(new Levels::Controls());
					break;
				case Levels::Map::Credits:
					GetOwner()->GetSpace()->SetLevel(new Levels::Credit());
					break;
				case Levels::Map::LevelSelect:
					GetOwner()->GetSpace()->SetLevel(new Levels::LevelSelect());
					break;
				default:
					GetOwner()->GetSpace()->SetLevel(new Levels::Level1(map));
				}
			}
		}
		else
		{
			sprite->SetColor(Color(0.7f, 0.7f, 0.7f));
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
}

//------------------------------------------------------------------------------
