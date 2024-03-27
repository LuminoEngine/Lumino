
#include "Internal.hpp"
#include <LuminoFramework/Audio/AudioListener.hpp>

namespace ln {

namespace detail {

AudioListenerParams::AudioListenerParams()
	: m_position(0.f, 0.f, 0.f)
	, m_forward(0.f, 0.f, -1.f)
	, m_up(0.f, 1.f, 0.f)
	, m_velocity(0.f, 0.f, 0.f)
	, m_dopplerFactor(1.f)	// 0.01f ~ 100.f
	, m_speedOfSound(343.f)	// 1.f ~ 10000.f
{
}

AudioEmitterParams::AudioEmitterParams()
	: m_position(0.f, 0.f, 0.f)
	, m_velocity(0.f, 0.f, 0.f)
	, m_direction(0.f, 0.f, 0.f)
{
}

AudioConeParams::AudioConeParams()
	: m_innerAngle(Math::PI2)
	, m_outerAngle(Math::PI2)
	, m_outerGain(0.f)
{
}

} // namespace detail

//==============================================================================
// AudioListener

AudioListener::AudioListener()
{

}

void AudioListener::init()
{
	Object::init();
}

} // namespace ln

