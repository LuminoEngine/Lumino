extern "C" {

// LNViewportLayerList

LN_CLASS(LNViewportLayerList, LNObject);

LN_API LNResult LNViewportLayerList_GetCount(LN_HANDLE(LNViewportLayerList) listObject, int* outCount);

LN_API LNResult LNViewportLayerList_SetAt(LN_HANDLE(LNViewportLayerList) listObject, int index, LN_HANDLE(LNViewportLayer) itemPtr);

LN_API LNResult LNViewportLayerList_GetAt(LN_HANDLE(LNViewportLayerList) listObject, int index, LN_HANDLE(LNViewportLayer)* outItemPtr);

LN_API LNResult LNViewportLayerList_Add(LN_HANDLE(LNViewportLayerList) listObject, LN_HANDLE(LNViewportLayer) itemPtr);

LN_API LNResult LNViewportLayerList_Clear(LN_HANDLE(LNViewportLayerList) listObject);

LN_API LNResult LNViewportLayerList_Insert(LN_HANDLE(LNViewportLayerList) listObject, int index, LN_HANDLE(LNViewportLayer) itemPtr);

LN_API LNResult LNViewportLayerList_Remove(LN_HANDLE(LNViewportLayerList) listObject, LN_HANDLE(LNViewportLayer) itemPtr, LNBool* outRemoved);

LN_API LNResult LNViewportLayerList_RemoveAt(LN_HANDLE(LNViewportLayerList) listObject, int index);

// LNSceneNodeList

LN_CLASS(LNSceneNodeList, LNObject);

LN_API LNResult LNSceneNodeList_GetCount(LN_HANDLE(LNSceneNodeList) listObject, int* outCount);

LN_API LNResult LNSceneNodeList_SetAt(LN_HANDLE(LNSceneNodeList) listObject, int index, LN_HANDLE(LNSceneNode) itemPtr);

LN_API LNResult LNSceneNodeList_GetAt(LN_HANDLE(LNSceneNodeList) listObject, int index, LN_HANDLE(LNSceneNode)* outItemPtr);

LN_API LNResult LNSceneNodeList_Add(LN_HANDLE(LNSceneNodeList) listObject, LN_HANDLE(LNSceneNode) itemPtr);

LN_API LNResult LNSceneNodeList_Clear(LN_HANDLE(LNSceneNodeList) listObject);

LN_API LNResult LNSceneNodeList_Insert(LN_HANDLE(LNSceneNodeList) listObject, int index, LN_HANDLE(LNSceneNode) itemPtr);

LN_API LNResult LNSceneNodeList_Remove(LN_HANDLE(LNSceneNodeList) listObject, LN_HANDLE(LNSceneNode) itemPtr, LNBool* outRemoved);

LN_API LNResult LNSceneNodeList_RemoveAt(LN_HANDLE(LNSceneNodeList) listObject, int index);

} // extern "C"
