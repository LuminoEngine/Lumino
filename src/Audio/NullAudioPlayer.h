//==============================================================================
// NullAudioPlayer 
//------------------------------------------------------------------------------
///**
//  @file       NullAudioPlayer.h
//  @brief      NullAudioPlayer
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../AudioPlayerBase.h"

namespace LNote
{
namespace Core
{
namespace Audio
{
class AudioDeviceBase;

//==============================================================================
// ■ NullAudioPlayer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class NullAudioPlayer
    : public AudioPlayerBase
{
public:
	NullAudioPlayer( AudioDeviceBase* device )
		: AudioPlayerBase( device )
	{}
	virtual ~NullAudioPlayer(){}

public:
	virtual lnU64			getPlayedSamples() const { return 0; }
	virtual void			play() { }
	virtual void			stop() { }
	virtual void			pause( bool enable ) { }
	virtual bool			polling() { return false; }
    virtual bool            is3DSound() { return false; }
	virtual void			setPosition( const LVector3& pos ) { }
	virtual const LVector3& getPosition() { return LVector3::ZERO; }
	virtual void            setVelocity( const LVector3& v ) {  }
    virtual void            setEmitterDistance( lnFloat distance ) {  }
    virtual lnFloat         getEmitterDistance() const { return 0; }
};

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================