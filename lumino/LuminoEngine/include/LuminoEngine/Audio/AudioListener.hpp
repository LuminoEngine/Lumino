#pragma once

namespace ln {
class AudioContext;

class AudioListener
	: public Object
{
public:
	void setPosition(const Vector3& value) { m_params.m_position = value; }
	const Vector3& position() const { return m_params.m_position; }

	void setForward(const Vector3& value) { m_params.m_forward = value; }
	const Vector3& forward() const { return m_params.m_forward; }

	void setUp(const Vector3& value) { m_params.m_up = value; }
	const Vector3& up() const { return m_params.m_up; }

	void setVelocity(const Vector3& value) { m_params.m_velocity = value; }
	const Vector3& velocity() const { return m_params.m_velocity; }

	void setDopplerFactor(float value) { m_params.m_dopplerFactor = value; }
	float dopplerFactor() const { return m_params.m_dopplerFactor; }

	void setSpeedOfSound(float value) { m_params.m_speedOfSound = value; }
	float speedOfSound() const { return m_params.m_speedOfSound; }

private:
	AudioListener();
	virtual ~AudioListener() = default;
	void init();

	detail::AudioListenerParams m_params;

	friend class AudioContext;
};

} // namespace ln

