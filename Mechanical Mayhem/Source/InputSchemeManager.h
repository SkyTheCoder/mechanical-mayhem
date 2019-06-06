//------------------------------------------------------------------------------
//
// File Name:	InputSchemeManager.h
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

#include <BetaObject.h>

#include <vector>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

enum InputType
{
	IT_KEYBOARD = 0,
	IT_CONTROLLER,

	IT_MAX
};

enum InputSource
{
	IS_NONE = 0,

	IS_KEYBOARD1,
	IS_KEYBOARD2,
	IS_CONTROLLER1,
	IS_CONTROLLER2,
	IS_CONTROLLER3,
	IS_CONTROLLER4,

	IS_MAX
};

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

struct InputScheme
{
public:
	// Constructor.
	InputScheme();

	// Constructor for keyboard inputs.
	// Params:
	//   keyUp = The up keybind.
	//   keyLeft = The left keybind.
	//   keyRight = The right keybind.
	//   keyDown = The down keybind.
	//   keyUse = The use keybind.
	InputScheme(unsigned keyUp, unsigned keyLeft, unsigned keyRight, unsigned keyDown, unsigned keyUse);

	// Constructor for controller inputs.
	// Params:
	//   controllerID = The ID of the controller.
	InputScheme(int controllerID);

	// Equality operator compares keybinds.
	bool operator==(const InputScheme& rhs) const;

	// Returns a printable version of the input source automatically assigned by the input scheme manager.
	// In this function, keyboard and controller names are just whichever joined first.
	std::string GetInputSourceName() const;

	// Returns the name of the input scheme that players will understand.
	// In this function, keyboard 1 = WASD, keyboard 2 = arrow keys,
	// and controller X = whichever controller displays it is player X
	// (automatically assigned by windows, usually lowest to highest based on which was plugged in first)
	std::string GetName() const;

	// The type of input used for this input scheme.
	InputType type;

	// The source of this input scheme.
	InputSource source;

	// The ID of the player this input scheme is bound to.
	int playerID;

	// Keyboard keybinds.
	unsigned keyUp;
	unsigned keyLeft;
	unsigned keyRight;
	unsigned keyDown;
	unsigned keyUse;

	// Controller ID.
	int controllerID;
};

// Manages binding keyboard & controller inputs to ingame players.
class InputSchemeManager : public BetaObject
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	InputSchemeManager();

	// Initialize input schemes to clear data.
	void Initialize();

	// Checks whether an input scheme with the specified input source has already been added.
	bool DoesSourceExist(InputSource source) const;

	// Checks whether an input scheme with the specified keybinds has already been added, and returns its source if found.
	InputSource FindScheme(const InputScheme& scheme) const;

	// Returns the input scheme bound to the specified player ID.
	InputScheme GetPlayerScheme(int playerID) const;

	// Checks whether an input source is currently connected.
	bool IsSourceConnected(InputSource source) const;

	// Removes the input scheme with the specified source.
	void RemoveSource(InputSource source);

	// Adds a new input scheme.
	// Params:
	//   scheme = The input scheme.
	// Returns:
	//   The input source assigned to the scheme.
	InputSource AddScheme(InputScheme& scheme);

	// Returns the internal list of input schemes.
	const std::vector<InputScheme>& GetInputSchemes() const;

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	std::vector<InputScheme> inputSchemes;
};

//------------------------------------------------------------------------------
