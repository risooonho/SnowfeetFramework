#ifndef HEADET_DEMO_ROTATE_HPP_INCLUDED
#define HEADET_DEMO_ROTATE_HPP_INCLUDED

#include "fm/proto/behaviour/Behaviour.hpp"

namespace demo
{

class Rotate : public zn::ABehaviour
{
public:

	ZN_BEHAVIOUR(demo::Rotate, 10)

	void onUpdate() override
	{
		f32 delta = entity().scene().deltaTime().asSeconds();
		entity().transform.rotate(90.f * delta);
	}

};

} // namespace demo

#endif // HEADER_DEMO_ROTATE_HPP_INCLUDED
