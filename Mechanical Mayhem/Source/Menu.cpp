//------------------------------------------------------------------------------
//
// File Name:	Menu.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Menu.h"

// Systems
#include <Engine.h>
#include <GameObjectFactory.h>
#include <GameObjectManager.h>
#include <ResourceManager.h>
#include <Space.h>
#include <GameObject.h>
#include <Graphics.h>
#include <Camera.h>
#include <Input.h>
#include <ExtendedInput.h>
#include "InputSchemeManager.h"
#include <Intersection2D.h>

// Components
#include <Transform.h>
#include <SpriteTextMono.h>
#include "Button.h"

// Levels
#include "Level1.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Menu.
	Menu::Menu() : Level("Menu"), currentButton(nullptr)
	{
	}

	// Load the resources associated with Menu.
	void Menu::Load()
	{
		std::cout << "Menu::Load" << std::endl;

		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		resourceManager.GetMesh("Button", Vector2D(1.0f, 1.0f / 6.0f), Vector2D(0.5f, 0.5f));
		resourceManager.GetSpriteSource("AniButton.png", 1, 6);
		resourceManager.GetMesh("FontAtlas", 12, 8);
		resourceManager.GetSpriteSource("Code New Roman@2x.png", 12, 8);

		objectManager.AddArchetype(*objectFactory.CreateObject("Button", resourceManager.GetMesh("Button"), resourceManager.GetSpriteSource("AniButton.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Text", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman@2x.png")));
	}

	// Sets the camera position.
	void Menu::FixCamera()
	{
		Camera& camera = Graphics::GetInstance().GetDefaultCamera();
		camera.SetTranslation(Vector2D());
		camera.SetSize(9.0f);
	}

	// Update Menu.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Menu::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();
		ExtendedInput& extendedInput = ExtendedInput::GetInstance();

		Vector2D cursorPos = Graphics::GetInstance().GetDefaultCamera().ScreenToWorldPosition(input.GetCursorPosition());

		if (!AlmostEqual(cursorPos, lastCursor) && !AlmostEqual(lastCursor, Vector2D()))
		{
			for (auto it = menuButtons.begin(); it != menuButtons.end(); ++it)
			{
				if (PointRectangleIntersection(cursorPos, (*it)->button->GetBoundingRect()))
				{
					if (!(*it)->button->IsSelected())
						(*it)->SetSelected(true);
					currentButton = *it;
				}
			}

			for (auto it = menuButtons.begin(); it != menuButtons.end(); ++it)
			{
				if (*it != currentButton)
				{
					(*it)->SetSelected(false);
				}
			}
		}

		if (currentButton != nullptr)
		{
			std::vector<InputScheme> inputSchemes;
			inputSchemes.push_back(InputScheme('W', 'A', 'D', 'S', VK_LCONTROL));
			inputSchemes.push_back(InputScheme(VK_UP, VK_LEFT, VK_RIGHT, VK_DOWN, VK_RCONTROL));
			inputSchemes.push_back(InputScheme(0));

			for (auto it = inputSchemes.cbegin(); it != inputSchemes.cend(); ++it)
			{
				if (it->type == IT_KEYBOARD)
				{
					if (input.CheckTriggered(it->keyUp) && currentButton->north != nullptr)
					{
						currentButton->SetSelected(false);
						currentButton = currentButton->north;
						currentButton->SetSelected(true);
					}
					if (input.CheckTriggered(it->keyLeft) && currentButton->west != nullptr)
					{
						currentButton->SetSelected(false);
						currentButton = currentButton->west;
						currentButton->SetSelected(true);
					}
					if (input.CheckTriggered(it->keyRight) && currentButton->east != nullptr)
					{
						currentButton->SetSelected(false);
						currentButton = currentButton->east;
						currentButton->SetSelected(true);
					}
					if (input.CheckTriggered(it->keyDown) && currentButton->south != nullptr)
					{
						currentButton->SetSelected(false);
						currentButton = currentButton->south;
						currentButton->SetSelected(true);
					}
				}
				else if (it->type == IT_CONTROLLER)
				{
					Vector2D lThumb = extendedInput.GetLThumb(it->controllerID, 0);
					Vector2D prevLThumb = extendedInput.GetLThumb(it->controllerID, 1);
					std::cout << lThumb << " : " << prevLThumb << std::endl;
					if ((lThumb.y > 0.5f && abs(lThumb.x) <= 0.5f) && (prevLThumb.y <= 0.5f || abs(prevLThumb.x) > 0.5f) && currentButton->north != nullptr)
					{
						currentButton->SetSelected(false);
						currentButton = currentButton->north;
						currentButton->SetSelected(true);
					}
					if ((lThumb.x < -0.5f && abs(lThumb.y) <= 0.5f) && (prevLThumb.x >= -0.5f || abs(prevLThumb.y) > 0.5f) && currentButton->west != nullptr)
					{
						currentButton->SetSelected(false);
						currentButton = currentButton->west;
						currentButton->SetSelected(true);
					}
					if ((lThumb.x > 0.5f && abs(lThumb.y) <= 0.5f) && (prevLThumb.x <= 0.5f || abs(prevLThumb.y) > 0.5f) && currentButton->east != nullptr)
					{
						currentButton->SetSelected(false);
						currentButton = currentButton->east;
						currentButton->SetSelected(true);
					}
					if ((lThumb.y < -0.5f && abs(lThumb.x) <= 0.5f) && (prevLThumb.y >= -0.5f || abs(prevLThumb.x) > 0.5f) && currentButton->south != nullptr)
					{
						currentButton->SetSelected(false);
						currentButton = currentButton->south;
						currentButton->SetSelected(true);
					}
				}
			}

			if (extendedInput.CheckXBTriggered(XB_A, 0) || input.CheckTriggered(' ') || input.CheckTriggered(VK_RETURN))
			{
				currentButton->button->OnPress();
			}
		}

		if (!currentButton->button->IsSelected())
			currentButton->SetSelected(true);

		lastCursor = cursorPos;
	}

	// Unload the resources associated with Menu.
	void Menu::Unload()
	{
		std::cout << "Menu::Unload" << std::endl;

		for (auto it = menuButtons.begin(); it != menuButtons.end(); ++it)
			delete *it;
	}

	// Adds a new map button.
	// Params:
	//   name = The name of the level.
	//   position = The position of the button.
	//   map = The map the button should switch to.
	// Returns:
	//   A pointer to the MenuButton created.
	Menu::MenuButton* Menu::AddMenuButton(const char* name_, Vector2D position, Map map)
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		GameObject* levelButton = new GameObject(*objectManager.GetArchetypeByName("Button"));
		static_cast<Transform*>(levelButton->GetComponent("Transform"))->SetTranslation(position);
		static_cast<Behaviors::Button*>(levelButton->GetComponent("Button"))->SetMap(map);
		objectManager.AddObject(*levelButton);

		GameObject* text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		text->GetComponent<SpriteTextMono>()->SetText(name_);
		text->GetComponent<Transform>()->SetTranslation(position);
		objectManager.AddObject(*text);

		menuButtons.push_back(new MenuButton(levelButton));
		return menuButtons.back();
	}

	// Sets the current menu button.
	void Menu::SetDefaultButton(MenuButton* button)
	{
		currentButton = button;
		defaultButton = button;
	}

	//------------------------------------------------------------------------------
	// Protected Structures:
	//------------------------------------------------------------------------------

	// Constructor.
	Menu::MenuButton::MenuButton(GameObject* object)
		: object(object), button(nullptr), north(nullptr), east(nullptr), south(nullptr), west(nullptr)
	{
		if (object != nullptr)
			button = object->GetComponent<Behaviors::Button>();
		SetSelected(false);
	}

	// Sets whether the button is currently "selected," either by the mouse or a keyboard/controller.
	void Menu::MenuButton::SetSelected(bool selected)
	{
		if (object != nullptr)
		{
			button->SetSelected(selected);
		}
	}
}

//----------------------------------------------------------------------------
