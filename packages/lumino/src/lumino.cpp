#include <lumino.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

LUMINO_API LNResult LNRuntime_Initialize() {
	return LN_OK;
}

LUMINO_API void LNRuntime_Terminate() {
}

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
