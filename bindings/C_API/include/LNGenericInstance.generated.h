extern "C" {

LN_API LNResult LNSceneNodeList_GetCount(LN_HANDLE(LNSceneNodeList) listObject, int* outCount);

LN_API LNResult LNSceneNodeList_SetAt(LN_HANDLE(LNSceneNodeList) listObject, int index, LN_HANDLE(LNSceneNode) itemPtr);

LN_API LNResult LNSceneNodeList_GetAt(LN_HANDLE(LNSceneNodeList) listObject, int index, LN_HANDLE(LNSceneNode)* outItemPtr);

LN_API LNResult LNSceneNodeList_Add(LN_HANDLE(LNSceneNodeList) listObject, LN_HANDLE(LNSceneNode) itemPtr);

LN_API LNResult LNSceneNodeList_Clear(LN_HANDLE(LNSceneNodeList) listObject);

LN_API LNResult LNSceneNodeList_Insert(LN_HANDLE(LNSceneNodeList) listObject, int index, LN_HANDLE(LNSceneNode) itemPtr);

LN_API LNResult LNSceneNodeList_Remove(LN_HANDLE(LNSceneNodeList) listObject, LN_HANDLE(LNSceneNode) itemPtr, LNBool* outRemoved);

LN_API LNResult LNSceneNodeList_RemoveAt(LN_HANDLE(LNSceneNodeList) listObject, int index);

} // extern "C"
