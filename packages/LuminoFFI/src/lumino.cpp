#include <stdio.h>
#include <lumino.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

LUMINO_API LNResult LNRuntime_Initialize() {
	printf("LNRuntime_Initialize\n");
	return LN_OK;
}

LUMINO_API void LNRuntime_Terminate() {
	printf("LNRuntime_Terminate\n");
}

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
