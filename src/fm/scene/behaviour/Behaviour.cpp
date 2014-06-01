#include <fm/scene/behaviour/Behaviour.hpp>
#include <fm/scene/core/Scene.hpp>

namespace zn
{

void Behaviour::onAdd(Entity * e)
{
	Component::onAdd(e);
	entity().scene().registerBehaviour(this);
}

void Behaviour::onDestroy()
{
	entity().scene().unregisterBehaviour(this);
}

} // namespace zn

