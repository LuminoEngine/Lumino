#pragma once

#define LNI_REFERENCE_RETAINED (1)
#define LNI_REFERENCE_RELEASED (2)
#define LN_NULL_HANDLE 0
typedef uint32_t LNHandle;
typedef void (*LNRuntimeFinalizedCallback)();
typedef void (*LNReferenceCountTrackerCallback)(LNHandle handle, int method, int count);
typedef void (*LNRuntimeGetTypeInfoIdCallback)(LNHandle handle, int* outTypeInfoId);

