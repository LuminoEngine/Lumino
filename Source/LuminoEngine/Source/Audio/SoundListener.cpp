
#include "Internal.h"
#include <Lumino/Audio/SoundListener.h>
#include "AudioManager.h"
#include "AudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

//------------------------------------------------------------------------------
void SoundListener::setPosition(const Vector3& position)
{
	detail::AudioManager::getInstance()->getAudioDevice()->getSoundListenerData()->position = position;
}

//------------------------------------------------------------------------------
void SoundListener::setPosition(float x, float y, float z)
{
	setPosition(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
const Vector3& SoundListener::getPosition()
{
	return detail::AudioManager::getInstance()->getAudioDevice()->getSoundListenerData()->position;
}

//------------------------------------------------------------------------------
void SoundListener::setDirection(const Vector3& direction)
{
	detail::AudioManager::getInstance()->getAudioDevice()->getSoundListenerData()->direction = direction;
}

//------------------------------------------------------------------------------
void SoundListener::setDirection(float x, float y, float z)
{
	setDirection(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
const Vector3& SoundListener::getDirection()
{
	return detail::AudioManager::getInstance()->getAudioDevice()->getSoundListenerData()->direction;
}

//------------------------------------------------------------------------------
void SoundListener::setUpDirection(const Vector3& direction)
{
	detail::AudioManager::getInstance()->getAudioDevice()->getSoundListenerData()->upDirection = direction;
}

//------------------------------------------------------------------------------
void SoundListener::setUpDirection(float x, float y, float z)
{
	setUpDirection(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
const Vector3& SoundListener::getUpDirection(const Vector3& direction)
{
	return detail::AudioManager::getInstance()->getAudioDevice()->getSoundListenerData()->upDirection;
}

//------------------------------------------------------------------------------
void SoundListener::setVelocity(const Vector3& velocity)
{
	detail::AudioManager::getInstance()->getAudioDevice()->getSoundListenerData()->velocity = velocity;
}

//------------------------------------------------------------------------------
void SoundListener::setVelocity(float x, float y, float z)
{
	setVelocity(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
const Vector3& SoundListener::getVelocity()
{
	return detail::AudioManager::getInstance()->getAudioDevice()->getSoundListenerData()->velocity;
}

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
