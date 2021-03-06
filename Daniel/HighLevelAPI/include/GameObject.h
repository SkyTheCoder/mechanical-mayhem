//------------------------------------------------------------------------------
//
// File Name:	GameObject.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <BetaObject.h>
#include "Serializable.h"
#include "EventHandler.h"

// Components
#include "Transform.h"
#include "Sprite.h"
#include "Collider.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Component;
class Space;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class GameObject : public BetaObject, public Serializable, public EventHandler
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new game object.
	// Params:
	//	 name = The name of the game object being created.
	GameObject(const std::string& name);

	// Clone a game object from another game object.
	// Params:
	//	 other = A reference to the object being cloned.
	GameObject(const GameObject& other);

	// Free the memory associated with a game object.
	~GameObject();

	// Load this object's components.
	void Load() override;

	// Initialize this object's components and set it to active.
	void Initialize() override;

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	virtual void Deserialize(Parser& parser) override;

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	virtual void Serialize(Parser& parser) const override;

	// Update any components attached to the game object.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt) override;

	// Updates components using a fixed timestep (usually just physics)
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void FixedUpdate(float dt) override;

	// Draw any visible components attached to the game object.
	void Draw() override;

	// Forwards events to components.
	// Params:
	//   event = The event that has been received.
	void HandleEvent(const Event& event) override;

	// Adds a component to the object.
	void AddComponent(Component* component);

	// Retrieves the component with the given name if it exists.
	// Params:
	//   name = The name of the component to find.
	// Returns:
	//  A pointer to the component if it exists, nullptr otherwise.
	Component* GetComponent(const std::string& name);

	// Retrieves the component with the given type if it exists.
	// Template params:
	//  ComponentType = The type of component to retrieve.
	// Returns:
	//  A pointer to the component if it exists, nullptr otherwise.
	template<class ComponentType>
	ComponentType* GetComponent()
	{
		return GetComponentGeneric<ComponentType>();
	}

	// Retrieves the transform component if it exists.
	template<>
	Transform* GetComponent<Transform>()
	{
		if (_transform == nullptr)
			_transform = GetComponentGeneric<Transform>();

		return _transform;
	}

	// Retrieves the sprite component if it exists.
	template<>
	Sprite* GetComponent<Sprite>()
	{
		if (_sprite == nullptr)
			_sprite = GetComponentGeneric<Sprite>();

		return _sprite;
	}

	// Retrieves the collider component if it exists.
	template<>
	Collider* GetComponent<Collider>()
	{
		if (_collider == nullptr)
			_collider = GetComponentGeneric<Collider>();

		return _collider;
	}

	// Clears the list of cached components in case a component is removed.
	void ClearComponentCache();

	// Mark an object for destruction.
	void Destroy();

	// Shutdown this object's components.
	void Shutdown() override;

	// Unload this object's components.
	void Unload() override;

	// Whether the object has been marked for destruction.
	// Returns:
	//		True if the object will be destroyed, false otherwise.
	bool IsDestroyed() const;

	// Get the space that contains this object.
	Space* GetSpace() const;

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Retrieves the component with the given type if it exists.
	// Template params:
	//  ComponentType = The type of component to retrieve.
	// Returns:
	//  A pointer to the component if it exists, nullptr otherwise.
	template<class ComponentType>
	ComponentType* GetComponentGeneric()
	{
		// Loop through every component and check if it can be cast to the specified type.
		for (auto it = components.begin(); it != components.end(); it++)
		{
			ComponentType* component = dynamic_cast<ComponentType*>(*it);
			if (component != nullptr)
				return component;
		}

		return nullptr;
	}

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Components
	std::vector<Component*> components;

	Transform* _transform;
	Sprite* _sprite;
	Collider* _collider;

	// Whether the object has been marked for destruction.
	bool isDestroyed;
};

//------------------------------------------------------------------------------
