//------------------------------------------------------------------------------
//
// File Name:	ChromaticAberration.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		Beta Engine
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ChromaticAberration.h"

// Systems
#include <Graphics.h>
#include <ShaderProgram.h>

//------------------------------------------------------------------------------

namespace Effects
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	ChromaticAberration::ChromaticAberration() : PostEffect("chromaticAberration.frag"), intensity(0.0f)
	{
	}

	// Send data to OpenGL
	void ChromaticAberration::Draw()
	{
		Vector2D viewport = Graphics::GetInstance().GetViewport();
		GetProgram().SetUniform("invSize", Vector2D(1.0f / viewport.x, 1.0f / viewport.y));
		GetProgram().SetUniform("intensity", intensity);
	}

	// Sets the intensity of the chromatic aberration.
	void ChromaticAberration::SetIntensity(float intensity_)
	{
		intensity = intensity_;
	}

	// Returns the intensity of the chromatic aberration.
	float ChromaticAberration::GetIntensity() const
	{
		return intensity;
	}
}

//------------------------------------------------------------------------------
