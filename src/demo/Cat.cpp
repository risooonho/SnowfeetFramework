#include <fm/asset/AssetBank.hpp>
#include <fm/proto/core/Scene.hpp>
#include <fm/proto/physics/BoxCollider.hpp>
#include <fm/proto/animation/SpriteAnimator.hpp>
#include <fm/util/Direction.hpp>
#include <fm/util/math.hpp>

#include "Cat.hpp"

namespace demo
{

void Cat::init()
{
	Character::init();

	r_sprite->setAtlas(AssetBank::current()->atlases.get("cat"));
	r_sprite->setFrame("idle.down");

	r_mover->setMaxSpeed(1.0f);

	entity().addComponent<BoxCollider>()->setRect(sf::FloatRect(0, 0, 16, 16));

	entity().addComponent<SpriteAnimator>();

	m_timeBeforeAction = 5.f;
	m_motorDir = Direction::NONE;
}

void Cat::onUpdate()
{
	f32 dt = entity().scene().deltaTime().asSeconds();
	m_timeBeforeAction -= dt;

	if(m_timeBeforeAction < 0)
	{
		m_timeBeforeAction = math::randf(0.5f, 1.f);
		if(math::randf() < 0.5f)
		{
			m_motorDir = math::rand(0,3);
		}
		else
			m_motorDir = Direction::NONE;
	}

	if(m_motorDir != Direction::NONE)
	{
		const f32 acceleration = 0.5f;
		r_mover->accelerate(acceleration, Direction::toVector<s32>(m_motorDir));
		setSpriteDirection(m_motorDir);
	}

	Character::onUpdate();
}

} // namespace demo



