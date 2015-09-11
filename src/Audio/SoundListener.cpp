
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
	Audio::Internal::Manager->GetAudioDevice()->getSoundListener()->Position = position;
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
	return Audio::Internal::Manager->GetAudioDevice()->getSoundListener()->Position;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetDirection(const Vector3& direction)
{
	Audio::Internal::Manager->GetAudioDevice()->getSoundListener()->Direction = direction;
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
	return Audio::Internal::Manager->GetAudioDevice()->getSoundListener()->Direction;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetUpDirection(const Vector3& direction)
{
	Audio::Internal::Manager->GetAudioDevice()->getSoundListener()->UpDirection = direction;
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
	return Audio::Internal::Manager->GetAudioDevice()->getSoundListener()->UpDirection;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SoundListener::SetVelocity(const Vector3& velocity)
{
	Audio::Internal::Manager->GetAudioDevice()->getSoundListener()->Velocity = velocity;
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
	return Audio::Internal::Manager->GetAudioDevice()->getSoundListener()->Velocity;
}

LN_NAMESPACE_AUDIO_END
} // namespace Lumino
