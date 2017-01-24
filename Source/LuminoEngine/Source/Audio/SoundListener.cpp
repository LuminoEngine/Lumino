
#include "Internal.h"
#include <Lumino/Audio/SoundListener.h>
#include "AudioManager.h"
#include "AudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

//------------------------------------------------------------------------------
void SoundListener::SetPosition(const Vector3& position)
{
	detail::AudioManager::GetInstance()->GetAudioDevice()->GetSoundListenerData()->position = position;
}

//------------------------------------------------------------------------------
void SoundListener::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
const Vector3& SoundListener::GetPosition()
{
	return detail::AudioManager::GetInstance()->GetAudioDevice()->GetSoundListenerData()->position;
}

//------------------------------------------------------------------------------
void SoundListener::SetDirection(const Vector3& direction)
{
	detail::AudioManager::GetInstance()->GetAudioDevice()->GetSoundListenerData()->direction = direction;
}

//------------------------------------------------------------------------------
void SoundListener::SetDirection(float x, float y, float z)
{
	SetDirection(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
const Vector3& SoundListener::GetDirection()
{
	return detail::AudioManager::GetInstance()->GetAudioDevice()->GetSoundListenerData()->direction;
}

//------------------------------------------------------------------------------
void SoundListener::SetUpDirection(const Vector3& direction)
{
	detail::AudioManager::GetInstance()->GetAudioDevice()->GetSoundListenerData()->upDirection = direction;
}

//------------------------------------------------------------------------------
void SoundListener::SetUpDirection(float x, float y, float z)
{
	SetUpDirection(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
const Vector3& SoundListener::GetUpDirection(const Vector3& direction)
{
	return detail::AudioManager::GetInstance()->GetAudioDevice()->GetSoundListenerData()->upDirection;
}

//------------------------------------------------------------------------------
void SoundListener::SetVelocity(const Vector3& velocity)
{
	detail::AudioManager::GetInstance()->GetAudioDevice()->GetSoundListenerData()->velocity = velocity;
}

//------------------------------------------------------------------------------
void SoundListener::SetVelocity(float x, float y, float z)
{
	SetVelocity(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
const Vector3& SoundListener::GetVelocity()
{
	return detail::AudioManager::GetInstance()->GetAudioDevice()->GetSoundListenerData()->velocity;
}

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
