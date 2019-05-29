//------------------------------------------------------------------------------
//
// File Name:	MutableTilemap.cpp
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

#include "MutableTilemap.h"

// Systems
#include "Tilemap.h"

// Events
#include "Event.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new mutable tilemap behavior.
MutableTilemap::MutableTilemap() : Component("MutableTilemap"), map(nullptr)
{
}

// Clone the behavior, returning a dynamically allocated copy.
Component* MutableTilemap::Clone() const
{
	return new MutableTilemap(*this);
}

// Receive an event and handle it (if applicable).
// Params:
//   event = The event that has been received.
void MutableTilemap::HandleEvent(const Event& event)
{
	if (event.type == ET_MapChange)
	{
		const MapChangeEvent& mapChangeEvent = static_cast<const MapChangeEvent&>(event);

		if (map != nullptr)
			map->SetCellValue(mapChangeEvent.column, mapChangeEvent.row, mapChangeEvent.value);
	}
}

// Sets the tilemap data.
// Params:
//   map = A pointer to the tilemap resource.
void MutableTilemap::SetTilemap(Tilemap* map_)
{
	map = map_;
}

//------------------------------------------------------------------------------
