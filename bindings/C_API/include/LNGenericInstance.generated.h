LN_API LNResult LNSceneNodeObjectList_GetCount(LN_HANDLE(LNSceneNodeObjectList) listObject, int* outCount);

LN_API LNResult LNSceneNodeObjectList_SetAt(LN_HANDLE(LNSceneNodeObjectList) listObject, int index, LN_HANDLE(LNSceneNode) itemPtr);

LN_API LNResult LNSceneNodeObjectList_GetAt(LN_HANDLE(LNSceneNodeObjectList) listObject, int index, LN_HANDLE(LNSceneNode)* outItemPtr);

LN_API LNResult LNSceneNodeObjectList_Add(LN_HANDLE(LNSceneNodeObjectList) listObject, LN_HANDLE(LNSceneNode) itemPtr);

LN_API LNResult LNSceneNodeObjectList_Clear(LN_HANDLE(LNSceneNodeObjectList) listObject);

LN_API LNResult LNSceneNodeObjectList_Insert(LN_HANDLE(LNSceneNodeObjectList) listObject, int index, LN_HANDLE(LNSceneNode) itemPtr);

LN_API LNResult LNSceneNodeObjectList_Remove(LN_HANDLE(LNSceneNodeObjectList) listObject, LN_HANDLE(LNSceneNode) itemPtr, LNBool* outRemoved);

LN_API LNResult LNSceneNodeObjectList_RemoveAt(LN_HANDLE(LNSceneNodeObjectList) listObject, int index);

