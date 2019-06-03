//------------------------------------------------------------------------------
//
// File Name:	ExtendedInput.h
// Author(s):	David Cohen (david.cohen)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <BetaObject.h>

#include <Vector2D.h>
#include <deque>
#include <vector>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

struct _XINPUT_STATE;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

enum ExtendedButton
{
	XB_DPAD_UP = 0,
	XB_DPAD_DOWN,
	XB_DPAD_LEFT,
	XB_DPAD_RIGHT,
	XB_START,
	XB_BACK,
	XB_LTHUMB,
	XB_RTHUMB,
	XB_LSHOULDER,
	XB_RSHOULDER,
	XB_A,
	XB_B,
	XB_X,
	XB_Y,

	XB_MAX
};

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Extended input class - Handles controller input.
class ExtendedInput
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Returns a new dynamically allocated beta module to be added to the beta engine.
	BetaObject* CreateModule();

	// Returns whether the specified controller was connected a number of frames before the current frame.
	bool IsControllerConnected(int controller = 0, unsigned framesBefore = 0) const;

	// Returns whether the specified controller became connected this frame.
	bool ControllerConnectedThisFrame(int controller = 0) const;

	// Test if extended button was down before and is still down
	bool CheckXBHeld(ExtendedButton button, int controller = 0) const;
	// Test if extended button was not down before but is down now
	bool CheckXBTriggered(ExtendedButton button, int controller = 0) const;
	// Test if extended button was down before but is not down now
	bool CheckXBReleased(ExtendedButton button, int controller = 0) const;

	// Test if extended button is currently down 
	bool IsXBDown(ExtendedButton button, int controller = 0) const;
	// Test if extended button was down a number of frames before the current frame
	bool WasXBDown(ExtendedButton button, int controller = 0, unsigned framesBefore = 1) const;

	// Gets the state of the triggers & thumbsticks.

	float GetLTrigger(int controller = 0, unsigned framesBefore = 0) const;
	float GetRTrigger(int controller = 0, unsigned framesBefore = 0) const;
	Vector2D GetLThumb(int controller = 0, unsigned framesBefore = 0) const;
	Vector2D GetRThumb(int controller = 0, unsigned framesBefore = 0) const;

	// Sets the intensity of the rumble motors in the specified controller.
	// Params:
	//   lowFreq = The intensity of the low frequency (left) rumble motor.
	//   highFreq = The intensity of the high frequency (right) rumble motor.
	//   controller = The index of the controller.
	void SetVibration(float lowFreq, float highFreq, int controller = 0);

	// Gets the intensity of the rumble motors in the specified controller.
	// Params:
	//   lowFreq = The intensity of the low frequency (left) rumble motor.
	//   highFreq = The intensity of the high frequency (right) rumble motor.
	//   controller = The index of the controller.
	void GetVibration(float& lowFreq, float& highFreq, int controller = 0) const;

	// Sets the % of vibration intensity that should be left over the course of 1 second due to decay.
	void SetVibrationDecay(float vibrationDecay);

	// Gets the % of vibration intensity that should be left over the course of 1 second due to decay.
	float GetVibrationDecay() const;

	// Retrieve the instance of the ExtendedInput singleton.
	static ExtendedInput& GetInstance();

private:
	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	class Module : public BetaObject
	{
	public:
		// Constructor.
		Module();

		// Initialize controller data to clean state.
		void Initialize() override;

		// Updates all controller inputs.
		// Params:
		//   dt = The change in time between last frame and the current frame.
		void Update(float dt) override;

		// Disable controller vibrations.
		void Shutdown() override;
	};

	struct ControllerState
	{
		// Constructor.
		ControllerState();

		// Returns the value specified by the button.
		bool GetButton(ExtendedButton button) const;

		// Whether the controller is connected.
		bool connected;

		// Button states.
		bool dpadUp;
		bool dpadDown;
		bool dpadLeft;
		bool dpadRight;
		bool start;
		bool back;
		bool lThumbPressed;
		bool rThumbPressed;
		bool lShoulder;
		bool rShoulder;
		bool a;
		bool b;
		bool x;
		bool y;

		// Analog states.
		float lTrigger;
		float rTrigger;
		Vector2D lThumb;
		Vector2D rThumb;

		// Rumble motor intensities.
		float lowFreq;
		float highFreq;
	};

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Initialize controller data to clean state.
	void Initialize();

	// Updates all controller inputs.
	// Params:
	//   dt = The change in time between last frame and the current frame.
	void Update(float dt);

	// Disable controller vibrations.
	void Shutdown();

	// Sends a new vibration state to the specified controller.
	// Params:
	//   lowFreq = The intensity of the low frequency (left) rumble motor.
	//   highFreq = The intensity of the high frequency (right) rumble motor.
	//   controller = The index of the controller.
	void SendVibrationState(float lowFreq, float highFreq, int controller = 0);

	// Converts an XINPUT_STATE struct into a processed ControllerState struct.
	ControllerState ProcessXInputState(const _XINPUT_STATE& state) const;

	// Converts the values from an XInput thumbstick to a Vector2D within the range of -1 to +1.
	Vector2D ProcessThumbstick(short x, short y, short deadzone) const;

	// Constructor and destructor are private to prevent accidental instantiation/deletion.
	ExtendedInput();
	~ExtendedInput();

	// Copy constructor and assignment are disabled.
	ExtendedInput(const ExtendedInput& other) = delete;
	ExtendedInput& operator=(const ExtendedInput& other) = delete;
	
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// The % of vibration intensity that should be left over the course of 1 second due to decay.
	float vibrationDecay;

	// Intensity of rumble motors to be set during the next update.
	float lowFreqVibrationQueue[4];
	float highFreqVibrationQueue[4];

	// How many frames of input data to remember.
	static const unsigned inputNum;
	std::deque<std::vector<ControllerState>> inputBuffer;

	friend class Module;
};

//------------------------------------------------------------------------------
