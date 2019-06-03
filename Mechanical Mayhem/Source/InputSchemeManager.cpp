//------------------------------------------------------------------------------
//
// File Name:	InputSchemeManager.cpp
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

#include "InputSchemeManager.h"

// Systems
#include <ExtendedInput.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Constructor.
InputScheme::InputScheme() : type(IT_MAX), source(IS_NONE), playerID(0), keyUp(0), keyLeft(0), keyRight(0), keyUse(0), controllerID(-1)
{
}

// Constructor for keyboard inputs.
// Params:
//   keyUp = The up keybind.
//   keyLeft = The left keybind.
//   keyRight = The right keybind.
//   keyDown = The down keybind.
//   keyUse = The use keybind.
InputScheme::InputScheme(unsigned keyUp, unsigned keyLeft, unsigned keyRight, unsigned keyDown, unsigned keyUse)
	: type(IT_KEYBOARD), source(IS_NONE), playerID(0), keyUp(keyUp), keyLeft(keyLeft), keyRight(keyRight), keyDown(keyDown), keyUse(keyUse), controllerID(-1)
{
}

// Constructor for controller inputs.
// Params:
//   controllerID = The ID of the controller.
InputScheme::InputScheme(int controllerID)
	: type(IT_CONTROLLER), source(IS_NONE), playerID(0), keyUp(0), keyLeft(0), keyRight(0), keyUse(0), controllerID(controllerID)
{
}

// Equality operator compares keybinds.
bool InputScheme::operator==(const InputScheme& rhs) const
{
	if (type != rhs.type)
		return false;

	switch (type)
	{
	case IT_KEYBOARD:
		return keyUp == rhs.keyUp && keyLeft == rhs.keyLeft && keyRight == rhs.keyRight && keyDown == rhs.keyDown && keyUse == rhs.keyUse;
	case IT_CONTROLLER:
		return controllerID == rhs.controllerID;
	}

	return false;
}

// Returns a printable version of the input source automatically assigned by the input scheme manager.
std::string InputScheme::GetInputSourceName() const
{
	switch (source)
	{
	case IS_KEYBOARD1:
		return "Keyboard 1";
	case IS_KEYBOARD2:
		return "Keyboard 2";
	case IS_CONTROLLER1:
		return "Controller 1";
	case IS_CONTROLLER2:
		return "Controller 2";
	case IS_CONTROLLER3:
		return "Controller 3";
	case IS_CONTROLLER4:
		return "Controller 4";
	}

	return "";
}

// Returns the name of the input scheme that players will understand.
std::string InputScheme::GetName() const
{
	if (type == IT_KEYBOARD)
	{
		if (keyUp == 'W')
			return "Keyboard 1";
		else if (keyUp == VK_UP)
			return "Keyboard 2";
	}
	else if (type == IT_CONTROLLER)
	{
		return "Controller " + std::to_string(controllerID + 1);
	}

	return "";
}

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor
InputSchemeManager::InputSchemeManager() : BetaObject("InputSchemeManager")
{

}

// Initialize input schemes to clear data.
void InputSchemeManager::Initialize()
{
	inputSchemes.clear();
}

// Checks whether an input scheme with the specified input source has already been added.
bool InputSchemeManager::DoesSourceExist(InputSource source) const
{
	for (auto it = inputSchemes.begin(); it != inputSchemes.end(); ++it)
	{
		if (it->source == source)
			return true;
	}

	return false;
}

// Checks whether an input scheme with the specified keybinds has already been added.
InputSource InputSchemeManager::FindScheme(const InputScheme& scheme) const
{
	for (auto it = inputSchemes.begin(); it != inputSchemes.end(); ++it)
	{
		if (*it == scheme)
			return it->source;
	}

	return IS_NONE;
}

// Returns the input scheme bound to the specified player ID.
InputScheme InputSchemeManager::GetPlayerScheme(int playerID) const
{
	for (auto it = inputSchemes.begin(); it != inputSchemes.end(); ++it)
	{
		if (it->playerID == playerID)
			return *it;
	}

	return InputScheme();
}

// Checks whether an input source is currently connected.
bool InputSchemeManager::IsSourceConnected(InputSource source) const
{
	switch (source)
	{
	case IS_KEYBOARD1:
	case IS_KEYBOARD2:
		return true;
	case IS_CONTROLLER1:
		return ExtendedInput::GetInstance().IsControllerConnected(0);
	case IS_CONTROLLER2:
		return ExtendedInput::GetInstance().IsControllerConnected(1);
	case IS_CONTROLLER3:
		return ExtendedInput::GetInstance().IsControllerConnected(2);
	case IS_CONTROLLER4:
		return ExtendedInput::GetInstance().IsControllerConnected(3);
	}

	return false;
}

// Removes the input scheme with the specified source.
void InputSchemeManager::RemoveSource(InputSource source)
{
	for (auto it = inputSchemes.begin(); it != inputSchemes.end(); ++it)
	{
		if (it->source == source)
		{
			inputSchemes.erase(it);
			return;
		}
	}
}

// Adds a new input scheme.
// Params:
//   scheme = The input scheme.
// Returns:
//   The input source assigned to the scheme.
InputSource InputSchemeManager::AddScheme(InputScheme& scheme)
{
	int count = 0;
	for (auto it = inputSchemes.begin(); it != inputSchemes.end(); ++it)
	{
		if (it->type == scheme.type)
			++count;
	}
	
	switch (scheme.type)
	{
	case IT_KEYBOARD:
		if (count >= 2)
			throw std::exception("Too many keyboard inputs!");

		scheme.source = static_cast<InputSource>(IS_KEYBOARD1 + count);

		break;
	case IT_CONTROLLER:
		if (count >= 4)
			throw std::exception("Too many controller inputs!");

		scheme.source = static_cast<InputSource>(IS_CONTROLLER1 + count);

		break;
	}

	// Find the lowest unused player ID.
	for (int i = 1; i <= 6; i++)
	{
		bool found = false;
		for (auto it = inputSchemes.cbegin(); it != inputSchemes.cend(); ++it)
		{
			if (it->playerID == i)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			scheme.playerID = i;
			break;
		}
	}

	// Place the scheme into the vector, sorted by player ID.
	for (auto it = inputSchemes.begin(); it != inputSchemes.end(); ++it)
	{
		if (it->playerID > scheme.playerID)
		{
			inputSchemes.insert(it, scheme);
			return scheme.source;
		}
	}

	inputSchemes.push_back(scheme);
	return scheme.source;
}

// Returns the internal list of input schemes.
const std::vector<InputScheme>& InputSchemeManager::GetInputSchemes() const
{
	return inputSchemes;
}

//------------------------------------------------------------------------------
