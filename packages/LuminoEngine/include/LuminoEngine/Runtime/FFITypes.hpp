#pragma once

#define LNI_REFERENCE_RETAINED (1)
#define LNI_REFERENCE_RELEASED (2)
#define LN_NULL_HANDLE 0
typedef int32_t LNHandle;
typedef void (*LNInstanceFinalizedCallback)();
typedef void (*LNReferenceCountTrackerCallback)(LNHandle handle, int method, int count);
typedef void (*LNInstanceGetTypeInfoIdCallback)(LNHandle handle, int* outTypeInfoId);

