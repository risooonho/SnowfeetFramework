#include <fm/util/math.hpp>
#include <fm/scene/core/Transform.hpp>
#include <fm/scene/core/Entity.hpp>
#include <fm/scene/core/Scene.hpp>

namespace zn
{

ZN_OBJECT_IMPL(zn::Transform)

//------------------------------------------------------------------------------
Transform::Transform() :
//	r_parent(nullptr),
	m_position(0,0),
	m_scale(1,1),
	m_rotation(0),
	m_localMatrixNeedUpdate(true),
	m_globalMatrixNeedUpdate(true)
{
}

//------------------------------------------------------------------------------
void Transform::onParentChanged()
{
	m_globalMatrixNeedUpdate = true;
	notifyChildrenForParentChange();
}

//------------------------------------------------------------------------------
void Transform::notifyChildrenForParentChange()
{
	for(u32 i = 0; i < entity().childCount(); ++i)
	{
		AbstractTransform * t = entity().child(i).transform();
		if(t)
		{
			t->onParentChanged();
		}
	}
}

//------------------------------------------------------------------------------
sf::Vector2f Transform::localPosition() const
{
	return m_position;
}

//------------------------------------------------------------------------------
sf::Vector2f Transform::position() const
{
	if(entity().parent())
	{
		AbstractTransform * t = entity().parent()->transform();
		if(t)
		{
			if(t->matrix())
			{
				return t->matrix()->transformPoint(m_position);
			}
			else
			{
				return t->position() + m_position;
			}
		}
	}
	return m_position;
}

//------------------------------------------------------------------------------
sf::Vector2f Transform::localScale() const
{
	return m_scale;
}

//------------------------------------------------------------------------------
sf::Vector2f Transform::scale() const
{
	if(entity().parent())
	{
		AbstractTransform * t = entity().parent()->transform();
		if(t)
		{
			sf::Vector2f parentScale = t->scale();
			return sf::Vector2f(
				parentScale.x * m_scale.x,
				parentScale.y * m_scale.y
			);
		}
	}
	return m_scale;
}

//------------------------------------------------------------------------------
f32 Transform::localRotation() const
{
	return m_rotation;
}

//------------------------------------------------------------------------------
f32 Transform::rotation() const
{
	if(entity().parent())
	{
		AbstractTransform * t = entity().parent()->transform();
		if(t)
		{
			return m_rotation + t->rotation();
		}
	}
	return m_rotation;
}

//------------------------------------------------------------------------------
void Transform::setLocalPosition(const sf::Vector2f & newPosition)
{
	m_position = newPosition;

	m_localMatrixNeedUpdate = true;
	m_globalMatrixNeedUpdate = true;
	notifyChildrenForParentChange();
}

//------------------------------------------------------------------------------
void Transform::setPosition(sf::Vector2f newPosition)
{
	// TODO cache inverted matrices?
	AbstractTransform * t = parent();

	if(t)
	{
		if(t->matrix())
		{
			newPosition = t->matrix()->getInverse().transformPoint(newPosition);
		}
		else
		{
			newPosition -= t->position();
		}
	}

	setLocalPosition(newPosition);
}

//------------------------------------------------------------------------------
void Transform::setLocalScale(const sf::Vector2f& newScale)
{
	m_scale = newScale;

	m_localMatrixNeedUpdate = true;
	m_globalMatrixNeedUpdate = true;
	notifyChildrenForParentChange();
}

//------------------------------------------------------------------------------
void Transform::setScale(sf::Vector2f newScale)
{
	AbstractTransform * t = parent();

	if(t)
	{
		sf::Vector2f parentScale = t->scale();
		newScale.x *= parentScale.x;
		newScale.y *= parentScale.y;
	}

	setLocalScale(newScale);
}

//------------------------------------------------------------------------------
void Transform::setLocalRotation(f32 newRotation)
{
	m_rotation = newRotation;

	m_localMatrixNeedUpdate = true;
	m_globalMatrixNeedUpdate = true;
	notifyChildrenForParentChange();
}

//------------------------------------------------------------------------------
void Transform::setRotation(f32 newRotation)
{
	AbstractTransform * t = parent();

	if(t)
	{
		f32 parentRotation = t->rotation();
		newRotation -= parentRotation;
	}

	setLocalRotation(newRotation);
}

//------------------------------------------------------------------------------
void Transform::translate(sf::Vector2f motion)
{
	setLocalPosition(m_position + motion);
}

//------------------------------------------------------------------------------
void Transform::rotate(f32 degrees)
{
	setLocalRotation(m_rotation + degrees);
}

//------------------------------------------------------------------------------
void Transform::scale(sf::Vector2f scaleFactor)
{
	setLocalScale(sf::Vector2f(
		m_scale.x * scaleFactor.x,
		m_scale.y * scaleFactor.y
	));
}

//------------------------------------------------------------------------------
const sf::Transform & Transform::localMatrix() const
{
	if(m_localMatrixNeedUpdate)
	{
		// Update local matrix
		f32 angle  = -m_rotation * math::DEG2RAD;
		f32 cosine = static_cast<f32>(cos(angle));
		f32 sine   = static_cast<f32>(sin(angle));
		f32 sxc    = m_scale.x * cosine;
		f32 syc    = m_scale.y * cosine;
		f32 sxs    = m_scale.x * sine;
		f32 sys    = m_scale.y * sine;
		f32 tx     = m_position.x;
		f32 ty     = m_position.y;

		m_localMatrix = sf::Transform(
			sxc, sys, tx,
			-sxs, syc, ty,
			0.f, 0.f, 1.f
		);

		m_localMatrixNeedUpdate = false;
	}

	return m_localMatrix;
}

//------------------------------------------------------------------------------
const sf::Transform * Transform::matrix() const
{
	if(m_globalMatrixNeedUpdate)
	{
		// Update global matrix

		AbstractTransform * t = parent();

		if(t && t->matrix())
		{
			m_globalMatrix = *(t->matrix());
			m_globalMatrix.combine(localMatrix());
		}
		else
		{
			m_globalMatrix = localMatrix();
		}

		m_globalMatrixNeedUpdate = false;
	}

	return &m_globalMatrix;
}

//------------------------------------------------------------------------------
AbstractTransform * Transform::parent() const
{
	if(entity().parent())
	{
		return entity().parent()->transform();
	}
	return nullptr;
}

//------------------------------------------------------------------------------
/*
void Transform::setParent(Transform * parent)
{
	// Don't do anything if the parent is the same
	if(parent == r_parent)
		return;

	// If the transform has a parent now
	if(r_parent != nullptr)
	{
#ifdef ZN_DEBUG
		std::cout << "D: Unparenting \"" << entity().name()
			<< "\" from \"" << r_parent->entity().name() << '"' << std::endl;
#endif
		// Notify the parent that its child moved
		r_parent->onRemoveChild(this);
	}

	// Set new parent

	r_parent = parent;

	// If the new parent is not null
	if(r_parent != nullptr)
	{
#ifdef ZN_DEBUG
		std::cout << "D: Parenting \"" << entity().name()
			<< "\" to \"" << r_parent->entity().name() << '"' << std::endl;
#endif
		// Notify the parent that it has a new child
		r_parent->onAddChild(this);
	}

	m_globalMatrixNeedUpdate = true;
	notifyChildrenForParentChange();
}
*/

//------------------------------------------------------------------------------
/*
void Transform::unparentChildren()
{
	// Make a copy of the child references to avoid concurrent modification
	// involved by setParent(). After this, m_children should be empty.
	auto childrenCopy = m_children;
	for(auto it = childrenCopy.begin(); it != childrenCopy.end(); ++it)
	{
		(*it)->setParent(nullptr);
	}
}
*/

//------------------------------------------------------------------------------
/*
Transform * Transform::child(u32 index) const
{
#ifdef ZN_DEBUG
	if(index >= m_children.size())
	{
		std::cout << "E: Transform::child: child index is out of bounds "
			"(index=" << index << ", "
			"size=" << m_children.size() << ", "
			"entityName=\"" << entity().name() << "\""
			")" << std::endl;
		return nullptr;
	}
#endif
	return m_children[index];
}
*/

//------------------------------------------------------------------------------
void Transform::serializeData(JsonBox::Value& o)
{
	zn::serialize(o["position"], m_position);
	zn::serialize(o["scale"], m_scale);
	o["rotation"] = m_rotation;

//	JsonBox::Value & parentData = o["parent"];
//	if(r_parent != nullptr)
//	{
//		parentData = (s32)(r_parent->entity().id());
//	}
//	else
//	{
//		parentData.setNull();
//	}
}

//------------------------------------------------------------------------------
void Transform::unserializeData(JsonBox::Value& o)
{
	zn::unserialize(o["position"], m_position);
	zn::unserialize(o["scale"], m_scale);
	m_rotation = o["rotation"].getDouble();

	m_localMatrixNeedUpdate = true;
	m_globalMatrixNeedUpdate = true;
}

//------------------------------------------------------------------------------
//void Transform::postUnserialize(JsonBox::Value & o)
//{
//	JsonBox::Value & parentData = o["parent"];
//	if(!parentData.isNull())
//	{
//		u32 parentID = parentData.getInt();
//		Entity * parent = entity().scene().findEntityFromID(parentID);
//
//		if(parent == nullptr)
//		{
//			std::cout << "E: Transform::postUnserialize: "
//				"parentID [" << parentID << "] not found in scene "
//				"(entity: \"" << entity().name() << "\")" << std::endl;
//		}
//		else
//		{
//			r_parent = parent->transform();
//			r_parent->onAddChild(this);
//		}
//	}
//}

//------------------------------------------------------------------------------
//void Transform::onAddChild(Transform* child)
//{
//	m_children.push_back(child);
//}

//------------------------------------------------------------------------------
//void Transform::onRemoveChild(Transform * child)
//{
//	for(auto it = m_children.begin(); it != m_children.end(); ++it)
//	{
//		if(*it == child)
//		{
//			m_children.erase(it);
//			return;
//		}
//	}
//}

} // namespace zn


