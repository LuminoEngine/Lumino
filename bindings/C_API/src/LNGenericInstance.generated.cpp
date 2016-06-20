#include "LNInternal.h"
#include "../include/LNBase.h"
#include "../include/LNGenericInstance.generated.h"

extern "C" {

LN_API void LNSceneNodeList_SetBindingTypeInfo(void* data) { LNObjectList_SetBindingTypeInfo(data); }

LN_API LNResult LNSceneNodeList_GetCount(LN_HANDLE(LNSceneNodeList) listObject, int* outCount)
{
    return LNObjectList_GetCount(listObject, outCount);
}

LN_API LNResult LNSceneNodeList_SetAt(LN_HANDLE(LNSceneNodeList) listObject, int index, LN_HANDLE(LNSceneNode) itemPtr)
{
    return LNObjectList_SetAt(listObject, index, itemPtr);
}

LN_API LNResult LNSceneNodeList_GetAt(LN_HANDLE(LNSceneNodeList) listObject, int index, LN_HANDLE(LNSceneNode)* outItemPtr)
{
    return LNObjectList_GetAt(listObject, index, outItemPtr);
}

LN_API LNResult LNSceneNodeList_Add(LN_HANDLE(LNSceneNodeList) listObject, LN_HANDLE(LNSceneNode) itemPtr)
{
    return LNObjectList_Add(listObject, itemPtr);
}

LN_API LNResult LNSceneNodeList_Clear(LN_HANDLE(LNSceneNodeList) listObject)
{
    return LNObjectList_Clear(listObject);
}

LN_API LNResult LNSceneNodeList_Insert(LN_HANDLE(LNSceneNodeList) listObject, int index, LN_HANDLE(LNSceneNode) itemPtr)
{
    return LNObjectList_Insert(listObject, index, itemPtr);
}

LN_API LNResult LNSceneNodeList_Remove(LN_HANDLE(LNSceneNodeList) listObject, LN_HANDLE(LNSceneNode) itemPtr, LNBool* outRemoved)
{
    return LNObjectList_Remove(listObject, itemPtr, outRemoved);
}

LN_API LNResult LNSceneNodeList_RemoveAt(LN_HANDLE(LNSceneNodeList) listObject, int index)
{
    return LNObjectList_RemoveAt(listObject, index);
}

} // extern "C"
