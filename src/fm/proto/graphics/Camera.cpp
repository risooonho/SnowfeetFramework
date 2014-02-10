#include <fm/proto/Entity.hpp>
#include <fm/proto/Scene.hpp>
#include <fm/proto/graphics/Camera.hpp>
#include <fm/proto/Application.hpp>

#include "fm/json/json_utils.hpp"

namespace zn
{

//------------------------------------------------------------------------------
Camera::Camera() : AComponent(),
	depth(0),
	layerMask(1), // See first layer by default
	m_scaleMode(FIXED)
{
	// Note: Application should never be null here, because it's the first object
	// to be created when the engine runs
	sf::Vector2i screenSize = Application::instance()->screenSize();
	onScreenResized(sf::Vector2u(screenSize.x, screenSize.y));
}

//------------------------------------------------------------------------------
Camera::~Camera()
{
}

//------------------------------------------------------------------------------
void Camera::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().renderSystem.registerCamera(this);
}

//------------------------------------------------------------------------------
void Camera::onDestroy()
{
	entity().scene().renderSystem.unregisterCamera(this);
}

//------------------------------------------------------------------------------
void Camera::init()
{
}

//------------------------------------------------------------------------------
void Camera::setSize(const sf::Vector2f & s)
{
	m_view.setSize(s);
}

//------------------------------------------------------------------------------
void Camera::setViewport(const sf::FloatRect & r)
{
	m_view.setViewport(r);
}

//------------------------------------------------------------------------------
void Camera::onUpdate()
{
	m_view.setCenter(entity().transform.position());
	m_view.setRotation(entity().transform.rotation());
}

//------------------------------------------------------------------------------
void Camera::setScaleMode(u8 mode)
{
	m_scaleMode = mode;
}

//------------------------------------------------------------------------------
void Camera::onScreenResized(sf::Vector2u resolution)
{
	if(m_scaleMode == FIXED)
	{
		m_view.setSize(resolution.x, resolution.y);
	}
	else if(m_scaleMode == ADAPTED)
	{
		sf::Vector2f s = m_view.getSize();
		f32 ratio = static_cast<f32>(resolution.y) / static_cast<f32>(resolution.x);
		m_view.setSize(s.x, ratio*s.x);
	}
}

//------------------------------------------------------------------------------
void Camera::serializeData(JsonBox::Value & o)
{
	AComponent::serializeData(o);

	o["depth"] = depth;

	zn::serialize(o["size"], m_view.getSize());
	zn::serialize(o["viewport"], m_view.getViewport());

	std::string scaleModeStr;
	switch(m_scaleMode)
	{
	case FIXED: scaleModeStr = "fixed"; break;
	case ADAPTED: scaleModeStr = "adapted"; break;
	case STRETCHED: scaleModeStr = "stretched"; break;
	default: break;
	}
	o["scaleMode"] = scaleModeStr;
}

//------------------------------------------------------------------------------
void Camera::unserializeData(JsonBox::Value & o)
{
	AComponent::unserializeData(o);

	depth = o["depth"].getInt();

	sf::Vector2f size;
	zn::unserialize(o["size"], size);
	m_view.setSize(size);

	sf::FloatRect viewport;
	zn::unserialize(o["viewport"], viewport);
	m_view.setViewport(viewport);

	std::string scaleModeStr = o["scaleMode"].getString();
	if(scaleModeStr == "fixed")
	{
		m_scaleMode = FIXED;
	}
	else if(scaleModeStr == "adapted")
	{
		m_scaleMode = ADAPTED;
	}
	else if(scaleModeStr == "stretched")
	{
		m_scaleMode = STRETCHED;
	}
}

//------------------------------------------------------------------------------
void Camera::postUnserialize()
{

}

} // namespace zn




