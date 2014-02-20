#include <fm/proto/physics/Body.hpp>
#include <fm/proto/core/Scene.hpp>

namespace zn
{

void Body::onAdd(Entity * e)
{
	AComponent::onAdd(e);
	entity().scene().physics.registerBody(this);
}

void Body::onDestroy()
{
	entity().scene().physics.unregisterBody(this);
}

} // namespace zn

