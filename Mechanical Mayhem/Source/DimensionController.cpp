//------------------------------------------------------------------------------
//
// File Name:	DimensionController.cpp
// Author(s):	David Cohen (david.cohen), A.J. Bussman
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
#include <SoundManager.h>
#include <Space.h>
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
	DimensionController::DimensionController() : Component("DimensionController"), dimensions(std::vector<Dimension>()),
		cooldown(5.0f), currentCooldown(5.0f), oldCooldown(currentCooldown), cdIndex(9), cdCount(-1), cdCounts{ 0 }, gameTimer(0.0), activeDimension(0)
	{
		// Set the different cooldown times
		cdCounts[0]  = 0.5f;
		cdCounts[1]  = 0.75f;
		cdCounts[2]  = 1.0f;
		cdCounts[3]  = 1.5f;
		cdCounts[4]  = 2.0f;
		cdCounts[5]  = 2.5f;
		cdCounts[6]  = 3.0f;
		cdCounts[7]  = 3.5f;
		cdCounts[8]  = 4.0f;
		cdCounts[9]  = 4.25f;
		//cdCounts[10] = 5.0f;
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
		currentCooldown = 5.0f;
		soundManager = Engine::GetInstance().GetModule<SoundManager>();;
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void DimensionController::FixedUpdate(float dt)
	{
		gameTimer += dt;

		oldCooldown = currentCooldown;
		currentCooldown -= dt;

		if (currentCooldown <= 0.0f)
		{
			// Increment cdCounts
			++cdCount;
			// Check if cdCounts >= 4 and cdIndex isn't 0
			//if (++cdCount >= 4 && cdIndex != 0)
				// Get the new cooldown time
			SetCoolDownTime();

			currentCooldown = cooldown;

			SetActiveDimension((activeDimension + 1) % GetDimensionCount());
		}

		// Play timer tick effect
		if (std::trunc(currentCooldown) != std::trunc(oldCooldown) && oldCooldown >= 1.0f)
		{
			 soundManager->PlaySound("SoundTick.wav");
		}
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
			static_cast<Hazard*>(spike->GetComponent("Hazard"))->SetCollidable(true);
			static_cast<Sprite*>(spike->GetComponent("Sprite"))->SetAlpha(1.0f);
		}

		// Play either dimension-shift sound
		if (RandomRange(0, 1))
		{
			soundManager->PlaySound("SoundDimensionShiftA.wav");
		}
		else
		{
			soundManager->PlaySound("SoundDimensionShiftB.wav");
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

	// Return time left for dimension switch
	float DimensionController::GetCoolDown() const
	{
		return currentCooldown;
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

	// Sets the cooldown time
	void DimensionController::SetCoolDownTime()
	{
		// Check for new Cooldown time
		if (NeedsNewCooldown())
		{
			// Decrement cdindex
			--cdIndex;
			// Reset cdCount to 0
			cdCount = 0;
		}
		// Otherwise use index 0
		else if (gameTimer >= 146.25f)
			cdIndex = 0;

		cooldown = cdCounts[cdIndex];
	}

	// Returns if a NeedsNewCooldown is needed
	bool DimensionController::NeedsNewCooldown()
	{
		// Return true if Cooldowm is greater than or equal to X and has iterated X times
		if (cdIndex >= 7 && cdCount >= 5)
			return true;
		else if (cdIndex >= 3 && cdCount >= 4)
			return true;
		else if (cdIndex >= 2 && cdCount >= 10)
			return true;
		else if (cdIndex >= 1 && cdCount >= 7)
			return true;
		// Otherwise return false
		return false;
	}

	// Constructor
	// Params:
	//   tilemap = The game oject with the tilemap for the dimension.
	DimensionController::Dimension::Dimension(GameObject* tilemap) : tilemap(tilemap), spikes(std::vector<GameObject*>())
	{
	}
}

//------------------------------------------------------------------------------
