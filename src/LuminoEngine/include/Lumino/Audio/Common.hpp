#pragma once

namespace ln {

namespace detail {

struct AudioListenerParams
{
	Vector3 m_position;
	Vector3 m_forward;
	Vector3 m_up;
	Vector3 m_velocity;
	float m_dopplerFactor;
	float m_speedOfSound;

	AudioListenerParams();
};

struct AudioEmitterParams
{
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_direction;	// zero is legal value.

	AudioEmitterParams();
};

struct AudioConeParams
{
	float m_innerAngle;
	float m_outerAngle;
	float m_outerGain;

	AudioConeParams();
};

} // namespace detail
} // namespace ln

