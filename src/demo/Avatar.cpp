#include <iostream>

#include <fm/sfml/sfml2_utils.hpp>
#include <fm/util/Direction.hpp>
#include <fm/asset/AssetBank.hpp>
#include <fm/scene/animation.hpp>
#include <fm/scene/physics.hpp>
#include "Avatar.hpp"
#include "Cat.hpp"

using namespace zn;

namespace demo
{

ZN_OBJECT_IMPL(demo::Avatar)

void Avatar::onCreate()
{
	Character::onCreate();

	Entity & e = entity();

	r_sprite->setTextureByName("avatar");
	r_sprite->setAtlasByName("avatar");
	r_sprite->setFrame("idle.down");
	//r_sprite->setScale(sf::Vector2f(100.f, 100.f));

	r_mover->setMaxSpeed(4.5f);

	auto collider = e.addComponent<BoxCollider>();
	collider->setRect(sf::FloatRect(0.125, 0.21875, 0.75, 0.75));
	//collider->enable(false);

	e.addComponent<SpriteAnimator>();
	e.addComponent<AudioEmitter>();

	m_pressedLastFrame = false;
}

void Avatar::onUpdate()
{
	// Poll input

	sf::Vector2i v;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		v.x = -1;
		setSpriteDirection(Direction::LEFT);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		v.x = 1;
		setSpriteDirection(Direction::RIGHT);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		v.y = -1;
		setSpriteDirection(Direction::UP);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		v.y = 1;
		setSpriteDirection(Direction::DOWN);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		if(!m_pressedLastFrame)
		{
			m_pressedLastFrame = true;
			entity().audio()->playBuffer("cat_meow1");
//			entity().audio()->playStream("theme");

			// TEST
//			MusicSystem & music = entity().scene().audioSystem.music;
//			if(music.playing())
//			{
//				music.stop();
//			}
//			else
//			{
//				music.play("theme");
//			}
		}
	}
	else
	{
		m_pressedLastFrame = false;
	}

	// Move

	const f32 acceleration = 0.5f;
	r_mover->accelerate(acceleration, v);

	// Make the camera follow

	Entity & cam = entity().scene().mainCamera()->entity();
	cam.transform()->setPosition(entity().transform()->position() + sf::Vector2f(0.5f,0.5f));
	cam.transform()->setRotation(sin(0.5f*entity().scene().time().asSeconds()));

	// Character-specific stuff (sprite orientation...)

	Character::onUpdate();
}

void Avatar::onCollisionEnter(const CollisionInfo & info)
{

}

} // namespace demo


