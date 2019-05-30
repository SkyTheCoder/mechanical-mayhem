//------------------------------------------------------------------------------
//
// File Name:	ChromaticAberration.h
// Author(s):	David Cohen (david.cohen)
// Project:		Beta Engine
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <PostEffect.h>

//------------------------------------------------------------------------------

namespace Effects
{
	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	class ChromaticAberration : public PostEffect
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		ChromaticAberration();

		// Send data to OpenGL
		void Draw() override;

		// Sets the intensity of the chromatic aberration.
		void SetIntensity(float intensity);

		// Returns the intensity of the chromatic aberration.
		float GetIntensity() const;

	private:
		//------------------------------------------------------------------------------
		// Private Data:
		//------------------------------------------------------------------------------

		// The intensity of the chromatic aberration.
		float intensity;
	};
}

//------------------------------------------------------------------------------
