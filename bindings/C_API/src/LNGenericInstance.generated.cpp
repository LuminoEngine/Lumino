#include "LNInternal.h"
#include <LuminoEngine.h>
#include "../include/LNBase.h"
#include "../include/LNGenericInstance.generated.h"

extern "C" {

// LNViewportLayerList

LN_TYPE_INFO_IMPL(ViewportLayerList, LNViewportLayerList);

LN_API LNResult LNViewportLayerList_SetAt(LN_HANDLE(LNViewportLayerList) listObject, int index, LN_HANDLE(LNViewportLayer) itemPtr)
{
    return LNObjectList_SetAt(listObject, index, itemPtr);
}

LN_API LNResult LNViewportLayerList_GetAt(LN_HANDLE(LNViewportLayerList) listObject, int index, LN_HANDLE(LNViewportLayer)* outItemPtr)
{
    return LNObjectList_GetAt(listObject, index, outItemPtr);
}

LN_API LNResult LNViewportLayerList_Add(LN_HANDLE(LNViewportLayerList) listObject, LN_HANDLE(LNViewportLayer) itemPtr)
{
    return LNObjectList_Add(listObject, itemPtr);
}

LN_API LNResult LNViewportLayerList_Clear(LN_HANDLE(LNViewportLayerList) listObject)
{
    return LNObjectList_Clear(listObject);
}

LN_API LNResult LNViewportLayerList_Insert(LN_HANDLE(LNViewportLayerList) listObject, int index, LN_HANDLE(LNViewportLayer) itemPtr)
{
    return LNObjectList_Insert(listObject, index, itemPtr);
}

LN_API LNResult LNViewportLayerList_Remove(LN_HANDLE(LNViewportLayerList) listObject, LN_HANDLE(LNViewportLayer) itemPtr, LNBool* outRemoved)
{
    return LNObjectList_Remove(listObject, itemPtr, outRemoved);
}

LN_API LNResult LNViewportLayerList_RemoveAt(LN_HANDLE(LNViewportLayerList) listObject, int index)
{
    return LNObjectList_RemoveAt(listObject, index);
}

// LNSceneNodeList

LN_TYPE_INFO_IMPL(SceneNodeList, LNSceneNodeList);

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
