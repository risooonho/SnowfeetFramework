/*
range.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetFramework project.
*/

#ifndef HEADER_ZN_RANGE_HPP_INCLUDED
#define HEADER_ZN_RANGE_HPP_INCLUDED

#include <iostream>
#include <fm/types.hpp>

namespace zn
{

template <typename T>
class Range
{
public:

	Range() : _min(0), _max(0) {}
	Range(T min, T max) { set(min, max); }

	void set(T min, T max)
	{
		if(min > max)
		{
			_min = max;
			_max = min;
		}
		else
		{
			_min = min;
			_max = max;
		}
	}

	inline T width() const { return _max - _min; }

	inline f32 inverseLerp(T v)
	{
		return static_cast<f32>(v - _min) / static_cast<f32>(width());
	}

	inline f32 lerp(f32 t)
	{
		return _min + t * width();
	}

	inline T min() const { return _min; }
	inline T max() const { return _max; }

	inline bool contains(T x) const { return x >= _min && x <= _max; }

	inline void print(std::ostream & os) const
	{
		os << "[" << _min << ", " << _max << "]";
	}

private:

	T _min;
	T _max;

};

} // namespace zn

#endif // HEADER_ZN_RANGE_HPP_INCLUDED



