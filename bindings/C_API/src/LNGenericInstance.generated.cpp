#include "LNInternal.h"
#include "../include/LNBase.h"
#include "../include/LNGenericInstance.generated.h"

extern "C" {

LN_API void LNSceneNodeObjectList_SetBindingTypeInfo(void* data) { LNObjectList_SetBindingTypeInfo(data); }

LN_API LNResult LNSceneNodeObjectList_GetCount(LN_HANDLE(LNSceneNodeObjectList) listObject, int* outCount)
{
    return LNObjectList_GetCount(listObject, outCount);
}

LN_API LNResult LNSceneNodeObjectList_SetAt(LN_HANDLE(LNSceneNodeObjectList) listObject, int index, LN_HANDLE(LNSceneNode) itemPtr)
{
    return LNObjectList_SetAt(listObject, index, itemPtr);
}

LN_API LNResult LNSceneNodeObjectList_GetAt(LN_HANDLE(LNSceneNodeObjectList) listObject, int index, LN_HANDLE(LNSceneNode)* outItemPtr)
{
    return LNObjectList_GetAt(listObject, index, outItemPtr);
}

LN_API LNResult LNSceneNodeObjectList_Add(LN_HANDLE(LNSceneNodeObjectList) listObject, LN_HANDLE(LNSceneNode) itemPtr)
{
    return LNObjectList_Add(listObject, itemPtr);
}

LN_API LNResult LNSceneNodeObjectList_Clear(LN_HANDLE(LNSceneNodeObjectList) listObject)
{
    return LNObjectList_Clear(listObject);
}

LN_API LNResult LNSceneNodeObjectList_Insert(LN_HANDLE(LNSceneNodeObjectList) listObject, int index, LN_HANDLE(LNSceneNode) itemPtr)
{
    return LNObjectList_Insert(listObject, index, itemPtr);
}

LN_API LNResult LNSceneNodeObjectList_Remove(LN_HANDLE(LNSceneNodeObjectList) listObject, LN_HANDLE(LNSceneNode) itemPtr, LNBool* outRemoved)
{
    return LNObjectList_Remove(listObject, itemPtr, outRemoved);
}

LN_API LNResult LNSceneNodeObjectList_RemoveAt(LN_HANDLE(LNSceneNodeObjectList) listObject, int index)
{
    return LNObjectList_RemoveAt(listObject, index);
}

} // extern "C"
