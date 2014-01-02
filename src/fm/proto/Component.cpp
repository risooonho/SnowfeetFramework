#include <iostream>
#include <cassert>
#include "../config.hpp"
#include "Component.hpp"
#include "Entity.hpp"

using namespace std;

namespace zn
{

AComponent::AComponent() :
	r_owner(nullptr),
	m_flags(0)
{
}

AComponent::~AComponent()
{
}

//------------------------------------------------------------------------------
void AComponent::onAdd(Entity * e)
{
	r_owner = e;
	m_flags |= CF_FIRST_UPDATE;
}

//------------------------------------------------------------------------------
void AComponent::init()
{
}

//------------------------------------------------------------------------------
void AComponent::onStart()
{
}

//------------------------------------------------------------------------------
void AComponent::update()
{
	if(m_flags && CF_FIRST_UPDATE)
	{
		onStart();
		onUpdate();
		m_flags &= ~CF_FIRST_UPDATE;
	}
	else
	{
		onUpdate();
	}
}

//------------------------------------------------------------------------------
void AComponent::onUpdate()
{
}

//------------------------------------------------------------------------------
void AComponent::onDestroy()
{
}

//------------------------------------------------------------------------------
Entity & AComponent::entity() const
{
#ifdef ZN_DEBUG
	if(r_owner == nullptr)
		cout << "E: AComponent::entity: _owner is null !" << endl;
#endif
	return *r_owner;
}

//------------------------------------------------------------------------------
// Serialization (member)

void AComponent::serializeData(JsonBox::Value & o)
{
	o["flags"] = m_flags;
}

void AComponent::unserializeData(JsonBox::Value & o)
{
	m_flags = o["flags"].getInt();
}

//------------------------------------------------------------------------------
// Serialization (static)

// TODO use a PropertyTree instead of directly JsonBox::Object?
// It would solve the problem about which serialization format to choose...

void AComponent::serialize(AComponent * component, JsonBox::Value & o)
{
	// Serialize type
	o["type"] = JsonBox::Value(component->componentType().name);

	// Serialize properties
	JsonBox::Value & properties = o["data"];
	component->serializeData(properties);
}

//------------------------------------------------------------------------------
AComponent * AComponent::unserialize(JsonBox::Value & o)
{
	// Get type of the component
	std::string type = o["type"].getString();

	// Instantiate it
	AComponent * component = ComponentType::instantiate(type);

	// Deserialize properties
	assert(component != nullptr);
	component->unserializeData(o["data"]);

	return component;
}

} // namespace zn


