//------------------------------------------------------------------------------
//
// File Name:	ExtendedInput.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ExtendedInput.h"

// Systems
#include <Xinput.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const unsigned ExtendedInput::inputNum = 2;

// Returns a new dynamically allocated beta module to be added to the beta engine.
BetaObject* ExtendedInput::CreateModule()
{
	return new Module();
}

// Returns whether the specified controller was connected a number of frames before the current frame.
bool ExtendedInput::WasControllerConnected(int controller, unsigned framesBefore) const
{
	if (framesBefore >= inputBuffer.size() || controller < 0 || controller >= XUSER_MAX_COUNT)
		return false;

	return inputBuffer[framesBefore][controller].connected;
}

// Returns whether the specified controller became connected this frame.
bool ExtendedInput::IsControllerConnected(int controller) const
{
	return WasControllerConnected(controller, 0) && !WasControllerConnected(controller, 1);
}

// Test if extended button was down before and is still down
bool ExtendedInput::CheckXBHeld(ExtendedButton button, int controller) const
{
	return WasXBDown(button, controller, 0) && WasXBDown(button, controller, 1);
}

// Test if extended button was not down before but is down now
bool ExtendedInput::CheckXBTriggered(ExtendedButton button, int controller) const
{
	return WasXBDown(button, controller, 0) && !WasXBDown(button, controller, 1);
}

// Test if extended button was down before but is not down now
bool ExtendedInput::CheckXBReleased(ExtendedButton button, int controller) const
{
	return !WasXBDown(button, controller, 0) && WasXBDown(button, controller, 1);
}

// Test if extended button is currently down 
bool ExtendedInput::IsXBDown(ExtendedButton button, int controller) const
{
	return WasXBDown(button, controller, 0);
}

// Test if extended button was down a number of frames before the current frame
bool ExtendedInput::WasXBDown(ExtendedButton button, int controller, unsigned framesBefore) const
{
	if (framesBefore >= inputBuffer.size() || controller < 0 || controller >= XUSER_MAX_COUNT)
		return false;

	return inputBuffer[framesBefore][controller].GetButton(button);
}

// Gets the state of the triggers & thumbsticks.

float ExtendedInput::GetLTrigger(int controller, unsigned framesBefore) const
{
	if (framesBefore >= inputBuffer.size() || controller < 0 || controller >= XUSER_MAX_COUNT)
		return 0.0f;

	return inputBuffer[framesBefore][controller].lTrigger;
}

float ExtendedInput::GetRTrigger(int controller, unsigned framesBefore) const
{
	if (framesBefore >= inputBuffer.size() || controller < 0 || controller >= XUSER_MAX_COUNT)
		return 0.0f;

	return inputBuffer[framesBefore][controller].rTrigger;
}

Vector2D ExtendedInput::GetLThumb(int controller, unsigned framesBefore) const
{
	if (framesBefore >= inputBuffer.size() || controller < 0 || controller >= XUSER_MAX_COUNT)
		return Vector2D();

	return inputBuffer[framesBefore][controller].lThumb;
}

Vector2D ExtendedInput::GetRThumb(int controller, unsigned framesBefore) const
{
	if (framesBefore >= inputBuffer.size() || controller < 0 || controller >= XUSER_MAX_COUNT)
		return Vector2D();

	return inputBuffer[framesBefore][controller].rThumb;
}

// Sets the intensity of the rumble motors in the specified controller.
// Params:
//   lowFreq = The intensity of the low frequency (left) rumble motor.
//   highFreq = The intensity of the high frequency (right) rumble motor.
//   controller = The index of the controller.
void ExtendedInput::SetVibration(float lowFreq, float highFreq, int controller)
{
	if (controller < 0 || controller >= XUSER_MAX_COUNT)
		return;

	lowFreqVibrationQueue[controller] = lowFreq;
	highFreqVibrationQueue[controller] = highFreq;
}

// Gets the intensity of the rumble motors in the specified controller.
// Params:
//   lowFreq = The intensity of the low frequency (left) rumble motor.
//   highFreq = The intensity of the high frequency (right) rumble motor.
//   controller = The index of the controller.
void ExtendedInput::GetVibration(float& lowFreq, float& highFreq, int controller) const
{
	if (controller < 0 || controller >= XUSER_MAX_COUNT)
		return;

	lowFreq = lowFreqVibrationQueue[controller];
	highFreq = highFreqVibrationQueue[controller];
}

// Sets the % of vibration intensity that should be left over the course of 1 second due to decay.
void ExtendedInput::SetVibrationDecay(float vibrationDecay_)
{
	vibrationDecay = vibrationDecay_;
}

// Gets the % of vibration intensity that should be left over the course of 1 second due to decay.
float ExtendedInput::GetVibrationDecay() const
{
	return vibrationDecay;
}

// Retrieve the instance of the ExtendedInput singleton.
ExtendedInput& ExtendedInput::GetInstance()
{
	static ExtendedInput instance;
	return instance;
}

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// Constructor.
ExtendedInput::Module::Module() : BetaObject("ExtendedInput")
{
}

// Initialize controller data to clean state.
void ExtendedInput::Module::Initialize()
{
	ExtendedInput::GetInstance().Initialize();
}

// Updates all controller inputs.
// Params:
//   dt = The change in time between last frame and the current frame.
void ExtendedInput::Module::Update(float dt)
{
	ExtendedInput::GetInstance().Update(dt);
}

// Disable controller vibrations.
void ExtendedInput::Module::Shutdown()
{
	ExtendedInput::GetInstance().Shutdown();
}

// Constructor.
ExtendedInput::ControllerState::ControllerState()
	: connected(false),
	dpadUp(false), dpadDown(false), dpadLeft(false), dpadRight(false),
	start(false), back(false),
	lThumbPressed(false), rThumbPressed(false), lShoulder(false), rShoulder(false),
	a(false), b(false), x(false), y(false),
	lTrigger(0.0f), rTrigger(0.0f),
	lowFreq(0.0f), highFreq(0.0f)
{
}

// Returns the value specified by the button.
bool ExtendedInput::ControllerState::GetButton(ExtendedButton button) const
{
	switch (button)
	{
	case XB_DPAD_UP:
		return dpadUp;
	case XB_DPAD_DOWN:
		return dpadDown;
	case XB_DPAD_LEFT:
		return dpadLeft;
	case XB_DPAD_RIGHT:
		return dpadRight;
	case XB_START:
		return start;
	case XB_BACK:
		return back;
	case XB_LTHUMB:
		return lThumbPressed;
	case XB_RTHUMB:
		return rThumbPressed;
	case XB_LSHOULDER:
		return lShoulder;
	case XB_RSHOULDER:
		return rShoulder;
	case XB_A:
		return a;
	case XB_B:
		return b;
	case XB_X:
		return x;
	case XB_Y:
		return y;
	}

	return false;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Initialize controller data to clean state.
void ExtendedInput::Initialize()
{
	inputBuffer.clear();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		lowFreqVibrationQueue[i] = 0.0f;
		highFreqVibrationQueue[i] = 0.0f;
	}
}

// Updates all controller inputs.
// Params:
//   dt = The change in time between last frame and the current frame.
void ExtendedInput::Update(float dt)
{
	std::vector<ControllerState> controllerStates;
	controllerStates.reserve(XUSER_MAX_COUNT);

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ControllerState state;

		// Read and process the current controller state from XInput.
		XINPUT_STATE xState;
		ZeroMemory(&xState, sizeof(XINPUT_STATE));
		if (XInputGetState(i, &xState) == ERROR_SUCCESS)
			state = ProcessXInputState(xState);

		// Don't transmit any minor rumbles.

		if (lowFreqVibrationQueue[i] <= 0.05f)
			lowFreqVibrationQueue[i] = 0.0f;

		if (highFreqVibrationQueue[i] <= 0.05f)
			highFreqVibrationQueue[i] = 0.0f;

		state.lowFreq = lowFreqVibrationQueue[i];
		state.highFreq = highFreqVibrationQueue[i];

		// Dispatch the latest vibration update.
		SendVibrationState(state.lowFreq, state.highFreq, i);

		// Apply vibration decay.
		lowFreqVibrationQueue[i] *= pow(vibrationDecay, dt);
		highFreqVibrationQueue[i] *= pow(vibrationDecay, dt);

		controllerStates.push_back(state);
	}

	if (inputBuffer.size() >= inputNum)
		inputBuffer.pop_back();

	inputBuffer.push_front(controllerStates);
}

// Disable controller vibrations.
void ExtendedInput::Shutdown()
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		SendVibrationState(0.0f, 0.0f, i);
	}
}

// Sends a new vibration state to the specified controller.
// Params:
//   lowFreq = The intensity of the low frequency (left) rumble motor.
//   highFreq = The intensity of the high frequency (right) rumble motor.
//   controller = The index of the controller.
void ExtendedInput::SendVibrationState(float lowFreq, float highFreq, int controller)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = static_cast<unsigned short>(std::clamp(lowFreq, 0.0f, 1.0f) * 65536.0f);
	vibration.wRightMotorSpeed = static_cast<unsigned short>(std::clamp(highFreq, 0.0f, 1.0f) * 65536.0f);
	XInputSetState(controller, &vibration);
}

// Converts an XINPUT_STATE struct into a processed ControllerState struct.
ExtendedInput::ControllerState ExtendedInput::ProcessXInputState(const _XINPUT_STATE& state) const
{
	ControllerState controllerState;

	controllerState.connected = true;

	// Read values from the bitmask.
	controllerState.dpadUp = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
	controllerState.dpadDown = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
	controllerState.dpadLeft = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
	controllerState.dpadRight = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
	controllerState.start = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
	controllerState.back = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
	controllerState.lThumbPressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
	controllerState.rThumbPressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
	controllerState.lShoulder = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
	controllerState.rShoulder = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
	controllerState.a = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
	controllerState.b = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
	controllerState.x = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
	controllerState.y = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;

	// Handle deadzones on triggers.
	if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		controllerState.lTrigger = state.Gamepad.bLeftTrigger / 255.0f;
	if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		controllerState.rTrigger = state.Gamepad.bRightTrigger / 255.0f;

	// Process thumbsticks.
	controllerState.lThumb = ProcessThumbstick(state.Gamepad.sThumbLX, state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	controllerState.rThumb = ProcessThumbstick(state.Gamepad.sThumbRX, state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

	return controllerState;
}

// Converts the values from an XInput thumbstick to a Vector2D within the range of -1 to +1.
Vector2D ExtendedInput::ProcessThumbstick(short x, short y, short deadzone) const
{
	Vector2D thumb = Vector2D(x, y);
	float magnitude = thumb.Magnitude();

	// Check if input is above deadzone.
	if (magnitude > deadzone)
	{
		// Clamp magnitude to the expected range.
		if (magnitude > 32767.0f)
			magnitude = 32767.0f;

		// Map the magnitude from (deadzone - 32767) to (0.0 - 1.0)
		magnitude = (magnitude - deadzone) / (32767.0f - deadzone);

		return thumb.Normalized() * magnitude;
	}

	// If the input was in the deadzone, return a zero vector.
	return Vector2D();
}

// Constructor and destructor are private to prevent accidental instantiation/deletion.
ExtendedInput::ExtendedInput() : vibrationDecay(0.0f), lowFreqVibrationQueue{ 0 }, highFreqVibrationQueue{ 0 }
{
}

ExtendedInput::~ExtendedInput()
{
}

//------------------------------------------------------------------------------
