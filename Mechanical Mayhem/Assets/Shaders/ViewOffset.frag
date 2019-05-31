//------------------------------------------------------------------------------
//
// File Name:	ViewOffset.frag
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Mechanical Mayhem
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#version 330 core

// Offsets
uniform float offsetX;
uniform float offsetY;

in vec2 textureCoordinate;
out vec4 fragColor;
uniform sampler2D sourceTexture;

void main()
{
	fragColor = texture2D(sourceTexture, vec2(textureCoordinate.x + offsetX, textureCoordinate.y + offsetY));
}
