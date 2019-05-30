//------------------------------------------------------------------------------
//
// File Name:	chromaticAberration.frag
// Author(s):	David Cohen (david.cohen)
// Project:		Beta Engine
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#version 330 core

in vec2 textureCoordinate;

out vec4 fragColor;

uniform sampler2D sourceTexture;

// The inverse size of the viewport.
uniform vec2 invSize;

// The intensity of the chromatic aberration.
uniform float intensity;

// The number of steps to sample for the radial blur.
const int steps = 10;

void main()
{
	// The delta from the center of the screen.
	vec2 delta = (vec2(0.5) - textureCoordinate) * 2.0;
	float deltaMag = sqrt(delta.x * delta.x + delta.y * delta.y);

	vec4 color = vec4(vec3(0.0), 1.0);

	// Calculate the direction for the radial blur.
	vec2 offset = delta * deltaMag * deltaMag * invSize;

	// Sample the radial blur with different intensities for different RGB channels.
	for (int i = 0; i < steps; i++)
	{
		float scale = float(i) / float(steps) * intensity;
		color.b += texture2D(sourceTexture, textureCoordinate + offset * scale * 0.33).b;
		color.g += texture2D(sourceTexture, textureCoordinate + offset * scale * 0.66).g;
		color.r += texture2D(sourceTexture, textureCoordinate + offset * scale).r;
	}
	
	// Average the samples.
	fragColor = vec4(color.rgb / float(steps), color.a);
}
