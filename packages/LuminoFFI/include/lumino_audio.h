#ifndef LUMINO_AUDIO_H_
#define LUMINO_AUDIO_H_
#include "lumino_types.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//==============================================================================
// LNSound
//==============================================================================

/**
 */
extern LUMINO_API LNResult LNSound_CreateFromData(const void* data, int32_t length, LNHandle* outSound);

/**
 */
extern LUMINO_API LNResult LNSound_Play(LNHandle sound);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // LUMINO_AUDIO_H_
