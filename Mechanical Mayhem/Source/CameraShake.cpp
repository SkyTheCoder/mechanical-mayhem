//------------------------------------------------------------------------------
//
// File Name:	CameraShake.cpp
// Author(s):	Daniel Walther (daniel.walther)
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
#include "CameraShake.h"

// Systems
#include <ShaderProgram.h>

// Misc.
#include <Random.h>

//------------------------------------------------------------------------------

namespace Effects
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	CameraShake::CameraShake()
		: PostEffect("ViewOffset.frag"),
		intensity(0.0f)
	{
	}

	// Send data to OpenGL
	void CameraShake::Draw()
	{
		if (intensity != 0.0f)
		{
			GetProgram().SetUniform("offsetX", RandomRange(-intensity, intensity));
			GetProgram().SetUniform("offsetY", RandomRange(-intensity, intensity));
		}
	}

	// Sets the intensity of the camera shake.
	void CameraShake::SetIntensity(float intensity_)
	{
		intensity = intensity_;
	}
}

//------------------------------------------------------------------------------
