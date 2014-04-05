/*
Scene.hpp
Copyright (C) 2013-2014 Marc GILLERON
This file is part of the Plane Framework project.
*/

#ifndef HEADER_ZN_SCENE_HPP_INCLUDED
#define HEADER_ZN_SCENE_HPP_INCLUDED

#include <list>
#include <unordered_set>
#include <SFML/Graphics.hpp>

#include <fm/proto/core/Entity.hpp>
#include <fm/proto/core/LayerMap.hpp>
#include <fm/proto/core/ComponentList.hpp>
#include <fm/proto/behaviour/Behaviour.hpp>
#include <fm/proto/audio/AudioSystem.hpp>
#include <fm/proto/graphics/RenderSystem.hpp>
#include <fm/proto/physics/PhysicsSystem.hpp>
#include <fm/proto/core/TagManager.hpp>

namespace zn
{

// A scene contains all stuff that the player can see and interact with.
// Only one scene should be active at a time.
// Disclaimer: despite being an sf::Drawable, the scene is not dedicated to graphics,
// it simply manages the entity list and specialized subsystems
// (sound, graphics, physics, behaviour...).
class ZN_API Scene : public sf::Drawable
{
public:

	Scene();
	~Scene();

	//----------------------------
	// Lifecycle
	//----------------------------

	// Updates the scene for a given time span
	void update(sf::Time deltaTime);

	// Erases everything in the scene.
	void clear();

	// Erases everything in the scene but entities having the EF_SCENE_CROSS flag set.
	void clearAllButCrossEntities();

	//----------------------------
	// State & entities
	//----------------------------

	// Creates an empty entity, adds it to the scene and returns it.
	// Note: by default, the update and draw order are defined by their creation order.
	Entity * createEntity(std::string name="", sf::Vector2f pos = sf::Vector2f());

	// Sets the given camera as the main camera
	inline void setMainCamera(Camera * cam) { r_mainCamera = cam; }

	// Gets the main camera (if defined).
	inline Camera * mainCamera() { return r_mainCamera; }

	// Finds the entity having the given ID.
	// Returns nullptr when not found.
	Entity * findEntityFromID(u32 id) const;

	// Finds the first encountered entity having the given name.
	// Returns nullptr when not found.
	Entity * findEntityFromName(const std::string & name);

	//----------------------------
	// Events
	//----------------------------

	// Called each time the screen resolution changes
	void onScreenResized(sf::Vector2u resolution);

	//----------------------------
	// Metadata
	//----------------------------

	// Convenient layer name associations
	LayerMap layers;

	TagManager tagManager;

	// Returns the amount of time elapsed since the scene was loaded
	inline sf::Time time() const { return m_time.getElapsedTime(); }

	// Returns the duration between two frames
	inline sf::Time deltaTime() const { return m_deltaTime; }

	//----------------------------
	// Serialization
	//----------------------------

	void serialize(JsonBox::Value & o);
	void unserialize(JsonBox::Value & o);

	bool saveToFile(const std::string & filePath);
	bool loadFromFile(const std::string & filePath);

	//----------------------------
	// Component systems
	//----------------------------

	// Called when a behaviour has been created
	void registerBehaviour(ABehaviour * behaviour);

	// Called just before a behaviour to be deleted
	void unregisterBehaviour(ABehaviour * behaviour);

	ComponentList<AAnimator>    animators;
	PhysicsSystem               physics;
	AudioSystem                 audioSystem;
	RenderSystem                renderSystem;

protected:

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

	u32                 m_nextID;

	// TODO add scene modes : EDIT / PLAY / RECORD

	std::list<Entity*>  m_entities; // Entities in play

	// References to behaviours [updateOrder][index]
	std::map< s32, ComponentList<ABehaviour> > m_behaviours;

	Camera *            r_mainCamera;
	sf::Clock           m_time;
	sf::Time            m_deltaTime;

};

} // namespace zn

#endif // HEADER_ZN_SCENE_HPP_INCLUDED



