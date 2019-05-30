//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GameObjectManager.h"

// Systems
#include <Graphics.h>
#include "Space.h"
#include <Vector2D.h>
#include "Event.h"
#include "Quadtree.h"

// Components
#include "Transform.h"
#include "Collider.h"
#include "ColliderTilemap.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor(s)
GameObjectManager::GameObjectManager(Space* space) : BetaObject("GameObjectmanager", space),
	gameObjectActiveList(), gameObjectArchetypes(), events(),
	fixedUpdateDt(1.0f / 200.0f), maxFixedUpdateTime(0.2f), timeAccumulator(0.0f),
	useQuadtree(true), quadtree(nullptr)
{
	// Reserve space for objects
	gameObjectActiveList.reserve(200);
	gameObjectArchetypes.reserve(10);
}

// Destructor
GameObjectManager::~GameObjectManager()
{
	Shutdown();
	Unload();

	delete quadtree;
}

// Update all objects in the active game objects list.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectManager::Update(float dt)
{
	Space* space = static_cast<Space*>(GetOwner());
	if (!space->IsPaused())
	{
		// General update.
		VariableUpdate(dt);

		// Dispatch any queued events that are ready.
		ProcessEvents(dt);

		// Update physics.
		FixedUpdate(dt);
	}

	DestroyObjects();

	Draw();
}

// Shutdown the game object manager, destroying all active objects.
void GameObjectManager::Shutdown(void)
{
	for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end(); it++)
	{
		delete *it;
	}

	gameObjectActiveList.clear();

	// Deallocate all unsent events.
	for (auto it = events.begin(); it != events.end(); ++it)
	{
		delete* it;
	}

	events.clear();
}

// Unload the game object manager, destroying all object archetypes.
void GameObjectManager::Unload(void)
{
	for (auto it = gameObjectArchetypes.begin(); it != gameObjectArchetypes.end(); it++)
	{
		delete *it;
	}

	gameObjectArchetypes.clear();
}

// Add a game object to the active game object list.
// Params:
//	 gameObject = Reference to the game object to be added to the list.
void GameObjectManager::AddObject(GameObject& gameObject)
{
	gameObjectActiveList.push_back(&gameObject);
	gameObject.SetOwner(GetOwner());
	gameObject.Load();
	gameObject.Initialize();
}

// Add a game object to the game object archetype list.
// Params:
//	 gameObject = Reference to the game object to be added to the list.
void GameObjectManager::AddArchetype(GameObject& gameObject)
{
	gameObjectArchetypes.push_back(&gameObject);
}

// Dispatch an event to one or more objects in this space.
// Params:
//   event = The event to be dispatched.
void GameObjectManager::DispatchEvent(Event* event)
{
	// If the event has no delay, dispatch it immediately.
	if (event->delay <= 0.0f)
	{
		// If the event is to be sent to a specific object, find that object and dispatch it.
		if (event->receiver != GUID())
		{
			BetaObject* receiver = BetaObject::GetObjectByID(event->receiver);
			if (receiver != nullptr)
				static_cast<GameObject*>(receiver)->HandleEvent(*event);
		}
		// Otherwise, dispatch the event to all active game objects.
		else
		{
			for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end(); ++it)
			{
				if ((*it)->IsDestroyed())
					continue;

				(*it)->HandleEvent(*event);
			}
		}

		delete event;
	}
	// Otherwise, add the event to the queue.
	else
	{
		events.push_back(event);
	}
}

// Returns a pointer to the first active game object matching the specified name.
// Params:
//	 objectName = The name of the object to be returned, if found.
// Returns:
//   If the named object is found,
//	   then return the pointer to the named game object,
//	   else return nullptr.
GameObject* GameObjectManager::GetObjectByName(const std::string& objectName) const
{
	// Loop through every game object and check if its name matches.
	for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end(); it++)
	{
		if ((*it)->GetName() == objectName)
			return *it;
	}

	return nullptr;
}

// Fills out a vector of game object pointers with all game objects matching the specified name. Does NOT clear the vector beforehand.
// Params:
//	 objectName = The name of the object to be returned, if found.
//   objectList = A reference to a vector of game object pointers which will be filled with the found game objects.
// Returns:
//   The number of game objects found.
size_t GameObjectManager::GetAllObjectsByName(const std::string & objectName, std::vector<GameObject*> & objectList) const
{
	size_t found = 0;

	// Loop through every game object and check if its name matches.
	for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end(); it++)
	{
		if ((*it)->GetName() == objectName)
		{
			objectList.push_back(*it);
			++found;
		}
	}

	return found;
}

// Retrieve all objects from the tree that could collide with a given object.
// Params:
//   object  = The object for which we want to retrieve nearby objects.
//   results = The list that stores the nearby objects.
void GameObjectManager::RetrieveNearbyObjects(GameObject* object, std::vector<GameObject*>& results)
{
	if (useQuadtree)
		quadtree->RetrieveNearbyObjects(object, results);
}

// Retrieve all objects from the tree that could collide with a given bounding rectangle.
// Params:
//   object  = The bounding rectangle for which we want to retrieve nearby objects.
//   results = The list that stores the nearby objects.
void GameObjectManager::RetrieveNearbyObjects(const BoundingRectangle& object, std::vector<GameObject*>& results)
{
	if (useQuadtree)
		quadtree->RetrieveNearbyObjects(object, results);
}

// Returns a pointer to the first game object archetype matching the specified name.
// Params:
//	 objectName = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return nullptr.
GameObject* GameObjectManager::GetArchetypeByName(const std::string & objectName) const
{
	// Loop through every archetype and check if its name matches.
	for (auto it = gameObjectArchetypes.begin(); it != gameObjectArchetypes.end(); it++)
	{
		if ((*it)->GetName() == objectName)
			return *it;
	}

	return nullptr;
}

// Returns the number of active objects with the given name.
// Params:
//   objectName = The name of the objects that should be counted.
unsigned GameObjectManager::GetObjectCount(const std::string & objectName) const
{
	unsigned count = 0;

	// Count objects with specific name.
	if (objectName != "")
	{
		for (auto it = gameObjectActiveList.cbegin(); it != gameObjectActiveList.cend(); it++)
		{
			// If the object is not destroyed and its name matches the search, increment count.
			if (!(*it)->IsDestroyed() && (*it)->GetName() == objectName)
				++count;
		}
	}
	// If no name was specified, count all objects.
	else
	{
		for (auto it = gameObjectActiveList.cbegin(); it != gameObjectActiveList.cend(); it++)
		{
			// If the object is not destroyed, increment count.
			if (!(*it)->IsDestroyed())
				++count;
		}
	}

	return count;
}

// Update object logic using variable timestep.
void GameObjectManager::VariableUpdate(float dt)
{
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		gameObjectActiveList[i]->Update(dt);
	}
}

// Update object physics using fixed timestep.
void GameObjectManager::FixedUpdate(float dt)
{
	timeAccumulator += dt;

	// Clamp the time accumulator so the engine never freezes due to too many collisions.
	if (timeAccumulator > maxFixedUpdateTime)
		timeAccumulator = maxFixedUpdateTime;

	// Call the FixedUpdate functions the appropriate amount of times.
	while (timeAccumulator >= fixedUpdateDt)
	{
		for (size_t i = 0; i < gameObjectActiveList.size(); i++)
		{
			gameObjectActiveList[i]->FixedUpdate(fixedUpdateDt);
		}

		// Check for collisions between pairs of objects
		if (useQuadtree) {
			PopulateQuadtree();
			CheckCollisionsQuadtree();
		}
		else
		{
			CheckCollisions();
		}

		timeAccumulator -= fixedUpdateDt;
	}
}

// Update event timers, dispatch events that are ready.
// Params:
//   dt = The change in time since the previous update.
void GameObjectManager::ProcessEvents(float dt)
{
	for (auto it = events.begin(); it != events.end();)
	{
		(*it)->delay -= dt;
		if ((*it)->delay <= 0.0f)
		{
			DispatchEvent(*it);
			it = events.erase(it);
		}
		else
		{
			++it;
		}
	}
}

// Destroy any objects marked for destruction.
void GameObjectManager::DestroyObjects()
{
	for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end();)
	{
		// If the object is destroyed, delete it and erase it from the vector.
		if ((*it)->IsDestroyed())
		{
			(*it)->Shutdown();
			(*it)->Unload();
			delete* it;
			it = gameObjectActiveList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

// Draw all game objects in the active game object list.
void GameObjectManager::Draw(void)
{
	if (useQuadtree && quadtree != nullptr)
	{
		// Draw the quadtree (debug)
		quadtree->Draw();
	}

	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		gameObjectActiveList[i]->Draw();
	}
}

// Check for collisions between each pair of objects.
void GameObjectManager::CheckCollisions()
{
	// Iterate over every object.
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		// If the current object is marked for destruction, skip it.
		if (gameObjectActiveList[i]->IsDestroyed())
			continue;

		// Get the current object's collider.
		Collider* collider = gameObjectActiveList[i]->GetComponent<Collider>();

		// If the current object does not have a collider, skip it.
		if (collider == nullptr)
			continue;

		// Iterate over all remaining objects.
		for (size_t j = i + 1; j < gameObjectActiveList.size(); j++)
		{
			// If the current object is marked for destruction, skip it.
			if (gameObjectActiveList[j]->IsDestroyed())
				continue;

			// Get the current object's collider.
			Collider* collider2 = gameObjectActiveList[j]->GetComponent<Collider>();

			// If the current object does not have a collider, skip it.
			if (collider2 == nullptr)
				continue;

			// Check for collisions between the two objects.
			collider->CheckCollision(*collider2);
		}
	}
}

// Recalculates all objects' positions in the quadtree.
void GameObjectManager::PopulateQuadtree()
{
	delete quadtree;

	// Create the new quadtree.
	BoundingRectangle worldScreenDimensions = Graphics::GetInstance().GetDefaultCamera().GetScreenWorldDimensions();
	BoundingRectangle quadtreeBase = BoundingRectangle(worldScreenDimensions.center, worldScreenDimensions.extents * 2.0f);
	quadtree = new Quadtree(quadtreeBase, 4, 4);

	// Add all active objects to the quadtree.
	for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end(); ++it)
	{
		if ((*it)->IsDestroyed())
			continue;

		Transform* transform = (*it)->GetComponent<Transform>();

		if (transform == nullptr)
			continue;

		quadtree->AddObject(*it, transform);
	}
}

// Check for collisions nearby all objects using a quadtree.
void GameObjectManager::CheckCollisionsQuadtree()
{
	std::vector<GameObject*> nearbyObjects;
	std::vector<GameObject*> tilemaps;
	GetAllObjectsByName("Tilemap", tilemaps);

	for (auto it1 = gameObjectActiveList.begin(); it1 != gameObjectActiveList.end(); ++it1)
	{
		if ((*it1)->IsDestroyed())
			continue;

		Collider* collider1 = (*it1)->GetComponent<Collider>();

		if (collider1 == nullptr)
			continue;

		// Get all nearby objects to perform collision checks on.
		nearbyObjects.clear();
		nearbyObjects.insert(nearbyObjects.end(), tilemaps.begin(), tilemaps.end());
		quadtree->RetrieveNearbyObjects(*it1, nearbyObjects);

		for (auto it2 = nearbyObjects.begin(); it2 != nearbyObjects.end(); ++it2)
		{
			if ((*it2)->IsDestroyed())
				continue;

			Collider* collider2 = (*it2)->GetComponent<Collider>();

			if (collider2 == nullptr)
				continue;

			if (collider2->WasProcessed())
				continue;

			// Perform collision checks
			collider1->CheckCollision(*collider2);
		}

		collider1->SetProcessed();
	}
}

//------------------------------------------------------------------------------
