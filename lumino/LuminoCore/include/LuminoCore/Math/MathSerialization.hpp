// Copyright (c) 2018+ lriki. Distributed under the MIT license...

#pragma once

namespace ln {

inline void serialize(Archive& ar, Vector2& value)
{
	int size = 0;
	ar.makeArrayTag(&size);
	if (ar.isLoading()) {
		assert(size == 2);	// TODO: error handling
	}
	ar.process(value.x);
	ar.process(value.y);
}

inline void serialize(Archive& ar, Vector3& value)
{
	int size = 0;
	ar.makeArrayTag(&size);
	if (ar.isLoading()) {
		assert(size == 3);	// TODO: error handling
	}
	ar.process(value.x);
	ar.process(value.y);
	ar.process(value.z);
}

inline void serialize(Archive& ar, Vector4& value)
{
	int size = 0;
	ar.makeArrayTag(&size);
	if (ar.isLoading()) {
		assert(size == 4);	// TODO: error handling
	}
	ar.process(value.x);
	ar.process(value.y);
	ar.process(value.z);
	ar.process(value.w);
}

} // namespace ln
