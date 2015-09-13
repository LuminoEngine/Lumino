
#include "Internal.h"
#include <Lumino/Audio/SoundListener.h>
#include <Lumino/Audio/AudioManager.h>
#include "AudioDevice.h"

namespace Lumino
{
LN_NAMESPACE_AUDIO_BEGIN

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetPosition(const Vector3& position)
{
	Internal::Manager->GetAudioDevice()->getSoundListenerData()->Position = position;
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
	return Internal::Manager->GetAudioDevice()->getSoundListenerData()->Position;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetDirection(const Vector3& direction)
{
	Internal::Manager->GetAudioDevice()->getSoundListenerData()->Direction = direction;
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
	return Internal::Manager->GetAudioDevice()->getSoundListenerData()->Direction;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetUpDirection(const Vector3& direction)
{
	Internal::Manager->GetAudioDevice()->getSoundListenerData()->UpDirection = direction;
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
	return Internal::Manager->GetAudioDevice()->getSoundListenerData()->UpDirection;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetVelocity(const Vector3& velocity)
{
	Internal::Manager->GetAudioDevice()->getSoundListenerData()->Velocity = velocity;
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
	return Internal::Manager->GetAudioDevice()->getSoundListenerData()->Velocity;
}

LN_NAMESPACE_AUDIO_END
} // namespace Lumino
