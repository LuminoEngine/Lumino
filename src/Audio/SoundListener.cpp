
#include "Internal.h"
#include <Lumino/Audio/SoundListener.h>
#include "AudioManager.h"
#include "AudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetPosition(const Vector3& position)
{
	Internal::AudioManager->GetAudioDevice()->getSoundListenerData()->Position = position;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3(x, y, z));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector3& SoundListener::GetPosition()
{
	return Internal::AudioManager->GetAudioDevice()->getSoundListenerData()->Position;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetDirection(const Vector3& direction)
{
	Internal::AudioManager->GetAudioDevice()->getSoundListenerData()->Direction = direction;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetDirection(float x, float y, float z)
{
	SetDirection(Vector3(x, y, z));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector3& SoundListener::GetDirection()
{
	return Internal::AudioManager->GetAudioDevice()->getSoundListenerData()->Direction;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetUpDirection(const Vector3& direction)
{
	Internal::AudioManager->GetAudioDevice()->getSoundListenerData()->UpDirection = direction;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetUpDirection(float x, float y, float z)
{
	SetUpDirection(Vector3(x, y, z));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector3& SoundListener::GetUpDirection(const Vector3& direction)
{
	return Internal::AudioManager->GetAudioDevice()->getSoundListenerData()->UpDirection;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetVelocity(const Vector3& velocity)
{
	Internal::AudioManager->GetAudioDevice()->getSoundListenerData()->Velocity = velocity;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetVelocity(float x, float y, float z)
{
	SetVelocity(Vector3(x, y, z));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector3& SoundListener::GetVelocity()
{
	return Internal::AudioManager->GetAudioDevice()->getSoundListenerData()->Velocity;
}

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
