//------------------------------------------------------------------------------
//
// File Name:	DimensionController.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		Yesterday's Mayonnaise
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "DimensionController.h"

// Systems
#include <Engine.h>
#include <GameObject.h>
#include <Input.h>
#include <Space.h>
#include <SoundManager.h>
#include <Interpolation.h>

// Components
#include <Transform.h>
#include <Physics.h>
#include <Sprite.h>
#include <ColliderTilemap.h>
#include "Hazard.h"

// Misc.
#include <Random.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	DimensionController::DimensionController() : Component("DimensionController"),
		dimensions(std::vector<Dimension>()), cooldown(0.5f), currentCooldown(0.0f)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* DimensionController::Clone() const
	{
		return new DimensionController(*this);
	}

	// Initialize this component (happens at object creation).
	void DimensionController::Initialize()
	{
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void DimensionController::Update(float dt)
	{
		currentCooldown = max(0.0f, currentCooldown - dt);
	}

	// Calculates how long until the dimension can be switched again.
	// Returns:
	//   How much longer until the dimension can be switched.
	float DimensionController::GetSwitchCooldown() const
	{
		return currentCooldown;
	}

	// Sets the active dimension and deactivates all others.
	// Params:
	//   dimension = The dimension to make active.
	void DimensionController::SetActiveDimension(unsigned dimension)
	{
		if (dimension >= dimensions.size())
			return;

		activeDimension = dimension;

		// Make all non-active dimensions transparent and non-collidable.
		for (unsigned i = 0; i < dimensions.size(); i++)
		{
			if (i == activeDimension)
				continue;

			static_cast<ColliderTilemap*>(dimensions[i].tilemap->GetComponent("Collider"))->SetActive(false);
			static_cast<Sprite*>(dimensions[i].tilemap->GetComponent("Sprite"))->SetAlpha(0.3f);
			for (GameObject* spike : dimensions[i].spikes)
			{
				UNREFERENCED_PARAMETER(spike);
				static_cast<Hazard*>(spike->GetComponent("Hazard"))->SetCollidable(false);
				static_cast<Sprite*>(spike->GetComponent("Sprite"))->SetAlpha(0.3f);
			}
		}

		// Make the active dimension opaque and collidable.
		static_cast<ColliderTilemap*>(dimensions[activeDimension].tilemap->GetComponent("Collider"))->SetActive(true);
		static_cast<Sprite*>(dimensions[activeDimension].tilemap->GetComponent("Sprite"))->SetAlpha(1.0f);
		for (GameObject* spike : dimensions[activeDimension].spikes)
		{
			UNREFERENCED_PARAMETER(spike);
			static_cast<Hazard*>(spike->GetComponent("Hazard"))->SetCollidable(true);
			static_cast<Sprite*>(spike->GetComponent("Sprite"))->SetAlpha(1.0f);
		}

		currentCooldown = cooldown;

		// Play sound
		if (RandomRange(0, 1))
		{
			Engine::GetInstance().GetModule<SoundManager>()->PlaySound("SoundDimensionShiftA.wav");
		}
		else
		{
			Engine::GetInstance().GetModule<SoundManager>()->PlaySound("SoundDimensionShiftB.wav");
		}
	}

	// Returns the active dimension.
	unsigned DimensionController::GetActiveDimension() const
	{
		return activeDimension;
	}

	// Returns the number of dimensions.
	unsigned DimensionController::GetDimensionCount() const
	{
		return static_cast<unsigned>(dimensions.size());
	}

	// Adds a new dimension.
	// Params:
	//   tilemap = The game object with the tilemap for the dimension.
	// Returns:
	//   The ID of the new dimension.
	unsigned DimensionController::AddDimension(GameObject* tilemap)
	{
		Dimension dimension(tilemap);
		dimensions.push_back(tilemap);
		return static_cast<unsigned>(dimensions.size() - 1);
	}

	// Adds a spike to an existing dimension.
	// Params:
	//   dimension = The ID of the dimension to add the spike to.
	//   spike = The game object for the spike.
	void DimensionController::AddSpikeToDimension(unsigned dimension, GameObject* spike)
	{
		if (dimension >= dimensions.size())
			return;

		dimensions[dimension].spikes.push_back(spike);
	}

	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   tilemap = The game oject with the tilemap for the dimension.
	DimensionController::Dimension::Dimension(GameObject* tilemap) : tilemap(tilemap), spikes(std::vector<GameObject*>())
	{
	}
}

//------------------------------------------------------------------------------
