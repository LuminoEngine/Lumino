#include <lumino_internal.h>
#include <lumino_audio.h>
#include <LuminoEngine/Audio/Sound.hpp>
using namespace ln;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

LNResult LNSound_CreateFromData(const void* data_, int32_t length_, LNHandle* outSound_) {
    LN_FFI_TRY_BEGIN;

	//LNResult r = LN_OK;
	//Ref<Sound> sound = Sound::createFromData(
	//	Span<const uint8_t>((const uint8_t*)data, length), &r);
	//if (r != LN_OK) {
	//	return r;
	//}
	//*outSound = ObjectHelper::createHandleFromObject<Sound>(sound);
 //       return LN_OK;
	LN_FFI_TRY_END_RETURN;
}

/**
 */
extern LUMINO_API LNResult LNSound_Play(LNHandle sound_) {
	LN_FFI_TRY_BEGIN;
	//Sound* s = LN_HANDLE_TO_OBJECT(Sound, sound);
	//s->play();
	LN_FFI_TRY_END_RETURN;
}

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
