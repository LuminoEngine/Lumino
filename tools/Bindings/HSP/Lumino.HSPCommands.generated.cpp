bool Commands_cmdfunc(int cmd, int* retVal)
{
    *retVal = RUNMODE_RUN;
    switch (cmd) {
        // LnPromiseFailureDelegate_Create
        case 0x64 : {
            // Fetch outPromiseFailureDelegate
            const PVal* pval_outPromiseFailureDelegate;
            const APTR aptr_outPromiseFailureDelegate = code_getva(&pval_outPromiseFailureDelegate);
            LnHandle local_outPromiseFailureDelegate;

            stat = LnPromiseFailureDelegate_Create(&local_outPromiseFailureDelegate);
            code_setva(pval_outPromiseFailureDelegate, aptr_outPromiseFailureDelegate, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outPromiseFailureDelegate));

            break;
        }
        // LnZVTestDelegate1_Create
        case 0x65 : {
            // Fetch outZVTestDelegate1
            const PVal* pval_outZVTestDelegate1;
            const APTR aptr_outZVTestDelegate1 = code_getva(&pval_outZVTestDelegate1);
            LnHandle local_outZVTestDelegate1;

            stat = LnZVTestDelegate1_Create(&local_outZVTestDelegate1);
            code_setva(pval_outZVTestDelegate1, aptr_outZVTestDelegate1, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outZVTestDelegate1));

            break;
        }
        // LnZVTestDelegate2_Create
        case 0x66 : {
            // Fetch outZVTestDelegate2
            const PVal* pval_outZVTestDelegate2;
            const APTR aptr_outZVTestDelegate2 = code_getva(&pval_outZVTestDelegate2);
            LnHandle local_outZVTestDelegate2;

            stat = LnZVTestDelegate2_Create(&local_outZVTestDelegate2);
            code_setva(pval_outZVTestDelegate2, aptr_outZVTestDelegate2, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outZVTestDelegate2));

            break;
        }
        // LnZVTestDelegate3_Create
        case 0x67 : {
            // Fetch outZVTestDelegate3
            const PVal* pval_outZVTestDelegate3;
            const APTR aptr_outZVTestDelegate3 = code_getva(&pval_outZVTestDelegate3);
            LnHandle local_outZVTestDelegate3;

            stat = LnZVTestDelegate3_Create(&local_outZVTestDelegate3);
            code_setva(pval_outZVTestDelegate3, aptr_outZVTestDelegate3, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outZVTestDelegate3));

            break;
        }
        // LnZVTestEventHandler1_Create
        case 0x68 : {
            // Fetch outZVTestEventHandler1
            const PVal* pval_outZVTestEventHandler1;
            const APTR aptr_outZVTestEventHandler1 = code_getva(&pval_outZVTestEventHandler1);
            LnHandle local_outZVTestEventHandler1;

            stat = LnZVTestEventHandler1_Create(&local_outZVTestEventHandler1);
            code_setva(pval_outZVTestEventHandler1, aptr_outZVTestEventHandler1, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outZVTestEventHandler1));

            break;
        }
        // LnZVTestEventHandler2_Create
        case 0x69 : {
            // Fetch outZVTestEventHandler2
            const PVal* pval_outZVTestEventHandler2;
            const APTR aptr_outZVTestEventHandler2 = code_getva(&pval_outZVTestEventHandler2);
            LnHandle local_outZVTestEventHandler2;

            stat = LnZVTestEventHandler2_Create(&local_outZVTestEventHandler2);
            code_setva(pval_outZVTestEventHandler2, aptr_outZVTestEventHandler2, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outZVTestEventHandler2));

            break;
        }
        // LnZVTestPromise1_ThenWith
        case 0x6A : {
            // Fetch zvtestpromise1
            const int local_zvtestpromise1 = fetchVAInt();
            // Fetch callback
            const int local_callback = fetchVAInt();

            stat = LnZVTestPromise1_ThenWith(local_zvtestpromise1, local_callback);


            break;
        }
        // LnZVTestPromise1_CatchWith
        case 0x6B : {
            // Fetch zvtestpromise1
            const int local_zvtestpromise1 = fetchVAInt();
            // Fetch callback
            const int local_callback = fetchVAInt();

            stat = LnZVTestPromise1_CatchWith(local_zvtestpromise1, local_callback);


            break;
        }
        // LnZVTestPromise2_ThenWith
        case 0x6C : {
            // Fetch zvtestpromise2
            const int local_zvtestpromise2 = fetchVAInt();
            // Fetch callback
            const int local_callback = fetchVAInt();

            stat = LnZVTestPromise2_ThenWith(local_zvtestpromise2, local_callback);


            break;
        }
        // LnZVTestPromise2_CatchWith
        case 0x6D : {
            // Fetch zvtestpromise2
            const int local_zvtestpromise2 = fetchVAInt();
            // Fetch callback
            const int local_callback = fetchVAInt();

            stat = LnZVTestPromise2_CatchWith(local_zvtestpromise2, local_callback);


            break;
        }
        // LnZVTestClass1_SetTestDelegate1
        case 0x6E : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const int local_value = fetchVAInt();

            stat = LnZVTestClass1_SetTestDelegate1(local_zvtestclass1, local_value);


            break;
        }
        // LnZVTestClass1_SetTestDelegate2
        case 0x6F : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const int local_value = fetchVAInt();

            stat = LnZVTestClass1_SetTestDelegate2(local_zvtestclass1, local_value);


            break;
        }
        // LnZVTestClass1_SetTestDelegate3
        case 0x70 : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const int local_value = fetchVAInt();

            stat = LnZVTestClass1_SetTestDelegate3(local_zvtestclass1, local_value);


            break;
        }
        // LnZVTestClass1_CallTestDelegate1
        case 0x71 : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();
            // Fetch a
            const int local_a = fetchVAInt();

            stat = LnZVTestClass1_CallTestDelegate1(local_zvtestclass1, local_a);


            break;
        }
        // LnZVTestClass1_CallTestDelegate2
        case 0x72 : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();
            // Fetch a
            const int local_a = fetchVAInt();
            // Fetch b
            const int local_b = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LnZVTestClass1_CallTestDelegate2(local_zvtestclass1, local_a, local_b, &local_outReturn);
            ??

            break;
        }
        // LnZVTestClass1_CallTestDelegate3
        case 0x73 : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();

            stat = LnZVTestClass1_CallTestDelegate3(local_zvtestclass1);


            break;
        }
        // LnZVTestClass1_LoadAsyncA
        case 0x74 : {
            // Fetch filePath
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnZVTestClass1_LoadAsyncA(local_filePath, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnZVTestClass1_ExecuteAsync
        case 0x75 : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnZVTestClass1_ExecuteAsync(local_zvtestclass1, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnZVTestClass1_GetFilePathA
        case 0x76 : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LnZVTestClass1_GetFilePathA(local_zvtestclass1, &local_outReturn);
            ??

            break;
        }
        // LnZVTestClass1_ConnectOnEvent1
        case 0x77 : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();
            // Fetch handler
            const int local_handler = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnZVTestClass1_ConnectOnEvent1(local_zvtestclass1, local_handler, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnZVTestClass1_RaiseEvent1
        case 0x78 : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();

            stat = LnZVTestClass1_RaiseEvent1(local_zvtestclass1);


            break;
        }
        // LnZVTestClass1_ConnectOnEvent2
        case 0x79 : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();
            // Fetch handler
            const int local_handler = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnZVTestClass1_ConnectOnEvent2(local_zvtestclass1, local_handler, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnZVTestClass1_RaiseEvent2
        case 0x7A : {
            // Fetch zvtestclass1
            const int local_zvtestclass1 = fetchVAInt();

            stat = LnZVTestClass1_RaiseEvent2(local_zvtestclass1);


            break;
        }
        // LnZVTestClass1_Create
        case 0x7B : {
            // Fetch outZVTestClass1
            const PVal* pval_outZVTestClass1;
            const APTR aptr_outZVTestClass1 = code_getva(&pval_outZVTestClass1);
            LnHandle local_outZVTestClass1;

            stat = LnZVTestClass1_Create(&local_outZVTestClass1);
            code_setva(pval_outZVTestClass1, aptr_outZVTestClass1, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outZVTestClass1));

            break;
        }
        // LnZVTestEventArgs1_GetValue
        case 0x7C : {
            // Fetch zvtesteventargs1
            const int local_zvtesteventargs1 = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LnZVTestEventArgs1_GetValue(local_zvtesteventargs1, &local_outReturn);
            ??

            break;
        }
        // LnZVTestEventArgs1_Create
        case 0x7D : {
            // Fetch outZVTestEventArgs1
            const PVal* pval_outZVTestEventArgs1;
            const APTR aptr_outZVTestEventArgs1 = code_getva(&pval_outZVTestEventArgs1);
            LnHandle local_outZVTestEventArgs1;

            stat = LnZVTestEventArgs1_Create(&local_outZVTestEventArgs1);
            code_setva(pval_outZVTestEventArgs1, aptr_outZVTestEventArgs1, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outZVTestEventArgs1));

            break;
        }
        // LnZVTestEventArgs1_CreateWithValue
        case 0x7E : {
            // Fetch v
            const int local_v = fetchVAInt();
            // Fetch outZVTestEventArgs1
            const PVal* pval_outZVTestEventArgs1;
            const APTR aptr_outZVTestEventArgs1 = code_getva(&pval_outZVTestEventArgs1);
            LnHandle local_outZVTestEventArgs1;

            stat = LnZVTestEventArgs1_CreateWithValue(local_v, &local_outZVTestEventArgs1);
            code_setva(pval_outZVTestEventArgs1, aptr_outZVTestEventArgs1, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outZVTestEventArgs1));

            break;
        }
        // LnSerializer_WriteBoolA
        case 0x7F : {
            // Fetch serializer
            const int local_serializer = fetchVAInt();
            // Fetch name
            ??
            // Fetch value
            const int local_value = fetchVAInt();

            stat = LnSerializer_WriteBoolA(local_serializer, local_name, local_value);


            break;
        }
        // LnSerializer_WriteIntA
        case 0x80 : {
            // Fetch serializer
            const int local_serializer = fetchVAInt();
            // Fetch name
            ??
            // Fetch value
            const int local_value = fetchVAInt();

            stat = LnSerializer_WriteIntA(local_serializer, local_name, local_value);


            break;
        }
        // LnSerializer_WriteFloatA
        case 0x81 : {
            // Fetch serializer
            const int local_serializer = fetchVAInt();
            // Fetch name
            ??
            // Fetch value
            ??

            stat = LnSerializer_WriteFloatA(local_serializer, local_name, local_value);


            break;
        }
        // LnSerializer_WriteStringA
        case 0x82 : {
            // Fetch serializer
            const int local_serializer = fetchVAInt();
            // Fetch name
            ??
            // Fetch value
            ??

            stat = LnSerializer_WriteStringA(local_serializer, local_name, local_value);


            break;
        }
        // LnSerializer_WriteObjectA
        case 0x83 : {
            // Fetch serializer
            const int local_serializer = fetchVAInt();
            // Fetch name
            ??
            // Fetch value
            const int local_value = fetchVAInt();

            stat = LnSerializer_WriteObjectA(local_serializer, local_name, local_value);


            break;
        }
        // LnSerializer_ReadBoolA
        case 0x84 : {
            // Fetch serializer
            const int local_serializer = fetchVAInt();
            // Fetch name
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnSerializer_ReadBoolA(local_serializer, local_name, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnSerializer_ReadIntA
        case 0x85 : {
            // Fetch serializer
            const int local_serializer = fetchVAInt();
            // Fetch name
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LnSerializer_ReadIntA(local_serializer, local_name, &local_outReturn);
            ??

            break;
        }
        // LnSerializer_ReadFloatA
        case 0x86 : {
            // Fetch serializer
            const int local_serializer = fetchVAInt();
            // Fetch name
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSerializer_ReadFloatA(local_serializer, local_name, &local_outReturn);
            ??

            break;
        }
        // LnSerializer_ReadStringA
        case 0x87 : {
            // Fetch serializer
            const int local_serializer = fetchVAInt();
            // Fetch name
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LnSerializer_ReadStringA(local_serializer, local_name, &local_outReturn);
            ??

            break;
        }
        // LnSerializer_ReadObjectA
        case 0x88 : {
            // Fetch serializer
            const int local_serializer = fetchVAInt();
            // Fetch name
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnSerializer_ReadObjectA(local_serializer, local_name, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnSerializer_SerializeA
        case 0x89 : {
            // Fetch value
            const int local_value = fetchVAInt();
            // Fetch basePath
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LnSerializer_SerializeA(local_value, local_basePath, &local_outReturn);
            ??

            break;
        }
        // LnSerializer_DeserializeA
        case 0x8A : {
            // Fetch str
            ??
            // Fetch basePath
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnSerializer_DeserializeA(local_str, local_basePath, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnAssetModel_Target
        case 0x8B : {
            // Fetch assetmodel
            const int local_assetmodel = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnAssetModel_Target(local_assetmodel, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnAssetModel_Create
        case 0x8C : {
            // Fetch target
            const int local_target = fetchVAInt();
            // Fetch outAssetModel
            const PVal* pval_outAssetModel;
            const APTR aptr_outAssetModel = code_getva(&pval_outAssetModel);
            LnHandle local_outAssetModel;

            stat = LnAssetModel_Create(local_target, &local_outAssetModel);
            code_setva(pval_outAssetModel, aptr_outAssetModel, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outAssetModel));

            break;
        }
        // LnAssets_SaveAssetToLocalFileA
        case 0x8D : {
            // Fetch asset
            const int local_asset = fetchVAInt();
            // Fetch filePath
            ??

            stat = LnAssets_SaveAssetToLocalFileA(local_asset, local_filePath);


            break;
        }
        // LnAssets_LoadAssetFromLocalFileA
        case 0x8E : {
            // Fetch filePath
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnAssets_LoadAssetFromLocalFileA(local_filePath, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnAssets_LoadAssetA
        case 0x8F : {
            // Fetch filePath
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnAssets_LoadAssetA(local_filePath, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnTexture2D_LoadA
        case 0x90 : {
            // Fetch filePath
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnTexture2D_LoadA(local_filePath, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnTexture2D_LoadEmojiA
        case 0x91 : {
            // Fetch code
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnTexture2D_LoadEmojiA(local_code, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnTexture2D_Create
        case 0x92 : {
            // Fetch width
            const int local_width = fetchVAInt();
            // Fetch height
            const int local_height = fetchVAInt();
            // Fetch outTexture2D
            const PVal* pval_outTexture2D;
            const APTR aptr_outTexture2D = code_getva(&pval_outTexture2D);
            LnHandle local_outTexture2D;

            stat = LnTexture2D_Create(local_width, local_height, &local_outTexture2D);
            code_setva(pval_outTexture2D, aptr_outTexture2D, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outTexture2D));

            break;
        }
        // LnTexture2D_CreateWithFormat
        case 0x93 : {
            // Fetch width
            const int local_width = fetchVAInt();
            // Fetch height
            const int local_height = fetchVAInt();
            // Fetch format
            ??
            // Fetch outTexture2D
            const PVal* pval_outTexture2D;
            const APTR aptr_outTexture2D = code_getva(&pval_outTexture2D);
            LnHandle local_outTexture2D;

            stat = LnTexture2D_CreateWithFormat(local_width, local_height, local_format, &local_outTexture2D);
            code_setva(pval_outTexture2D, aptr_outTexture2D, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outTexture2D));

            break;
        }
        // LnTexture2D_CreateFromFileA
        case 0x94 : {
            // Fetch filePath
            ??
            // Fetch format
            ??
            // Fetch outTexture2D
            const PVal* pval_outTexture2D;
            const APTR aptr_outTexture2D = code_getva(&pval_outTexture2D);
            LnHandle local_outTexture2D;

            stat = LnTexture2D_CreateFromFileA(local_filePath, local_format, &local_outTexture2D);
            code_setva(pval_outTexture2D, aptr_outTexture2D, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outTexture2D));

            break;
        }
        // LnVisualComponent_SetVisible
        case 0x95 : {
            // Fetch visualcomponent
            const int local_visualcomponent = fetchVAInt();
            // Fetch value
            const int local_value = fetchVAInt();

            stat = LnVisualComponent_SetVisible(local_visualcomponent, local_value);


            break;
        }
        // LnVisualComponent_IsVisible
        case 0x96 : {
            // Fetch visualcomponent
            const int local_visualcomponent = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnVisualComponent_IsVisible(local_visualcomponent, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnSpriteComponent_SetTexture
        case 0x97 : {
            // Fetch spritecomponent
            const int local_spritecomponent = fetchVAInt();
            // Fetch texture
            const int local_texture = fetchVAInt();

            stat = LnSpriteComponent_SetTexture(local_spritecomponent, local_texture);


            break;
        }
        // LnWorld_Add
        case 0x98 : {
            // Fetch world
            const int local_world = fetchVAInt();
            // Fetch obj
            const int local_obj = fetchVAInt();

            stat = LnWorld_Add(local_world, local_obj);


            break;
        }
        // LnComponentList_GetLength
        case 0x99 : {
            // Fetch componentlist
            const int local_componentlist = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LnComponentList_GetLength(local_componentlist, &local_outReturn);
            ??

            break;
        }
        // LnComponentList_GetItem
        case 0x9A : {
            // Fetch componentlist
            const int local_componentlist = fetchVAInt();
            // Fetch index
            const int local_index = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnComponentList_GetItem(local_componentlist, local_index, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnWorldObject_SetPosition
        case 0x9B : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch pos
            ??

            stat = LnWorldObject_SetPosition(local_worldobject, local_pos);


            break;
        }
        // LnWorldObject_SetPositionXYZ
        case 0x9C : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch x
            ??
            // Fetch y
            ??
            // Fetch z
            ??

            stat = LnWorldObject_SetPositionXYZ(local_worldobject, local_x, local_y, local_z);


            break;
        }
        // LnWorldObject_GetPosition
        case 0x9D : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnWorldObject_GetPosition(local_worldobject, &local_outReturn);
            ??

            break;
        }
        // LnWorldObject_SetRotationQuaternion
        case 0x9E : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch rot
            ??

            stat = LnWorldObject_SetRotationQuaternion(local_worldobject, local_rot);


            break;
        }
        // LnWorldObject_SetRotation
        case 0x9F : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch x
            ??
            // Fetch y
            ??
            // Fetch z
            ??

            stat = LnWorldObject_SetRotation(local_worldobject, local_x, local_y, local_z);


            break;
        }
        // LnWorldObject_GetRotation
        case 0xA0 : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnQuaternion local_outReturn;

            stat = LnWorldObject_GetRotation(local_worldobject, &local_outReturn);
            ??

            break;
        }
        // LnWorldObject_SetScale
        case 0xA1 : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch scale
            ??

            stat = LnWorldObject_SetScale(local_worldobject, local_scale);


            break;
        }
        // LnWorldObject_SetScaleS
        case 0xA2 : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch xyz
            ??

            stat = LnWorldObject_SetScaleS(local_worldobject, local_xyz);


            break;
        }
        // LnWorldObject_SetScaleXYZ
        case 0xA3 : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch x
            ??
            // Fetch y
            ??
            // Fetch z
            ??

            stat = LnWorldObject_SetScaleXYZ(local_worldobject, local_x, local_y, local_z);


            break;
        }
        // LnWorldObject_GetScale
        case 0xA4 : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnWorldObject_GetScale(local_worldobject, &local_outReturn);
            ??

            break;
        }
        // LnWorldObject_SetCenterPoint
        case 0xA5 : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch value
            ??

            stat = LnWorldObject_SetCenterPoint(local_worldobject, local_value);


            break;
        }
        // LnWorldObject_SetCenterPointXYZ
        case 0xA6 : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch x
            ??
            // Fetch y
            ??
            // Fetch z
            ??

            stat = LnWorldObject_SetCenterPointXYZ(local_worldobject, local_x, local_y, local_z);


            break;
        }
        // LnWorldObject_GetCenterPoint
        case 0xA7 : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnWorldObject_GetCenterPoint(local_worldobject, &local_outReturn);
            ??

            break;
        }
        // LnWorldObject_LookAt
        case 0xA8 : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch target
            ??

            stat = LnWorldObject_LookAt(local_worldobject, local_target);


            break;
        }
        // LnWorldObject_LookAtXYZ
        case 0xA9 : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch x
            ??
            // Fetch y
            ??
            // Fetch z
            ??

            stat = LnWorldObject_LookAtXYZ(local_worldobject, local_x, local_y, local_z);


            break;
        }
        // LnWorldObject_AddComponent
        case 0xAA : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch component
            const int local_component = fetchVAInt();

            stat = LnWorldObject_AddComponent(local_worldobject, local_component);


            break;
        }
        // LnWorldObject_GetComponents
        case 0xAB : {
            // Fetch worldobject
            const int local_worldobject = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnWorldObject_GetComponents(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnVisualObject_SetVisible
        case 0xAC : {
            // Fetch visualobject
            const int local_visualobject = fetchVAInt();
            // Fetch value
            const int local_value = fetchVAInt();

            stat = LnVisualObject_SetVisible(local_visualobject, local_value);


            break;
        }
        // LnVisualObject_IsVisible
        case 0xAD : {
            // Fetch visualobject
            const int local_visualobject = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnVisualObject_IsVisible(local_visualobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnDirectionalLight_SetEnabled
        case 0xAE : {
            // Fetch directionallight
            const int local_directionallight = fetchVAInt();
            // Fetch enabled
            const int local_enabled = fetchVAInt();

            stat = LnDirectionalLight_SetEnabled(local_directionallight, local_enabled);


            break;
        }
        // LnDirectionalLight_IsEnabled
        case 0xAF : {
            // Fetch directionallight
            const int local_directionallight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnDirectionalLight_IsEnabled(local_directionallight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnDirectionalLight_SetColor
        case 0xB0 : {
            // Fetch directionallight
            const int local_directionallight = fetchVAInt();
            // Fetch color
            ??

            stat = LnDirectionalLight_SetColor(local_directionallight, local_color);


            break;
        }
        // LnDirectionalLight_GetColor
        case 0xB1 : {
            // Fetch directionallight
            const int local_directionallight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnColor local_outReturn;

            stat = LnDirectionalLight_GetColor(local_directionallight, &local_outReturn);
            ??

            break;
        }
        // LnDirectionalLight_SetIntensity
        case 0xB2 : {
            // Fetch directionallight
            const int local_directionallight = fetchVAInt();
            // Fetch intensity
            ??

            stat = LnDirectionalLight_SetIntensity(local_directionallight, local_intensity);


            break;
        }
        // LnDirectionalLight_GetIntensity
        case 0xB3 : {
            // Fetch directionallight
            const int local_directionallight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnDirectionalLight_GetIntensity(local_directionallight, &local_outReturn);
            ??

            break;
        }
        // LnDirectionalLight_Create
        case 0xB4 : {
            // Fetch outDirectionalLight
            const PVal* pval_outDirectionalLight;
            const APTR aptr_outDirectionalLight = code_getva(&pval_outDirectionalLight);
            LnHandle local_outDirectionalLight;

            stat = LnDirectionalLight_Create(&local_outDirectionalLight);
            code_setva(pval_outDirectionalLight, aptr_outDirectionalLight, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outDirectionalLight));

            break;
        }
        // LnDirectionalLight_CreateWithColor
        case 0xB5 : {
            // Fetch color
            ??
            // Fetch outDirectionalLight
            const PVal* pval_outDirectionalLight;
            const APTR aptr_outDirectionalLight = code_getva(&pval_outDirectionalLight);
            LnHandle local_outDirectionalLight;

            stat = LnDirectionalLight_CreateWithColor(local_color, &local_outDirectionalLight);
            code_setva(pval_outDirectionalLight, aptr_outDirectionalLight, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outDirectionalLight));

            break;
        }
        // LnPointLight_SetEnabled
        case 0xB6 : {
            // Fetch pointlight
            const int local_pointlight = fetchVAInt();
            // Fetch enabled
            const int local_enabled = fetchVAInt();

            stat = LnPointLight_SetEnabled(local_pointlight, local_enabled);


            break;
        }
        // LnPointLight_IsEnabled
        case 0xB7 : {
            // Fetch pointlight
            const int local_pointlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnPointLight_IsEnabled(local_pointlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnPointLight_SetColor
        case 0xB8 : {
            // Fetch pointlight
            const int local_pointlight = fetchVAInt();
            // Fetch color
            ??

            stat = LnPointLight_SetColor(local_pointlight, local_color);


            break;
        }
        // LnPointLight_GetColor
        case 0xB9 : {
            // Fetch pointlight
            const int local_pointlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnColor local_outReturn;

            stat = LnPointLight_GetColor(local_pointlight, &local_outReturn);
            ??

            break;
        }
        // LnPointLight_SetIntensity
        case 0xBA : {
            // Fetch pointlight
            const int local_pointlight = fetchVAInt();
            // Fetch intensity
            ??

            stat = LnPointLight_SetIntensity(local_pointlight, local_intensity);


            break;
        }
        // LnPointLight_GetIntensity
        case 0xBB : {
            // Fetch pointlight
            const int local_pointlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnPointLight_GetIntensity(local_pointlight, &local_outReturn);
            ??

            break;
        }
        // LnPointLight_SetRange
        case 0xBC : {
            // Fetch pointlight
            const int local_pointlight = fetchVAInt();
            // Fetch range
            ??

            stat = LnPointLight_SetRange(local_pointlight, local_range);


            break;
        }
        // LnPointLight_GetRange
        case 0xBD : {
            // Fetch pointlight
            const int local_pointlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnPointLight_GetRange(local_pointlight, &local_outReturn);
            ??

            break;
        }
        // LnPointLight_SetAttenuation
        case 0xBE : {
            // Fetch pointlight
            const int local_pointlight = fetchVAInt();
            // Fetch attenuation
            ??

            stat = LnPointLight_SetAttenuation(local_pointlight, local_attenuation);


            break;
        }
        // LnPointLight_GetAttenuation
        case 0xBF : {
            // Fetch pointlight
            const int local_pointlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnPointLight_GetAttenuation(local_pointlight, &local_outReturn);
            ??

            break;
        }
        // LnPointLight_Create
        case 0xC0 : {
            // Fetch outPointLight
            const PVal* pval_outPointLight;
            const APTR aptr_outPointLight = code_getva(&pval_outPointLight);
            LnHandle local_outPointLight;

            stat = LnPointLight_Create(&local_outPointLight);
            code_setva(pval_outPointLight, aptr_outPointLight, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outPointLight));

            break;
        }
        // LnPointLight_CreateWithColorAndRange
        case 0xC1 : {
            // Fetch color
            ??
            // Fetch range
            ??
            // Fetch outPointLight
            const PVal* pval_outPointLight;
            const APTR aptr_outPointLight = code_getva(&pval_outPointLight);
            LnHandle local_outPointLight;

            stat = LnPointLight_CreateWithColorAndRange(local_color, local_range, &local_outPointLight);
            code_setva(pval_outPointLight, aptr_outPointLight, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outPointLight));

            break;
        }
        // LnSpotLight_SetEnabled
        case 0xC2 : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch enabled
            const int local_enabled = fetchVAInt();

            stat = LnSpotLight_SetEnabled(local_spotlight, local_enabled);


            break;
        }
        // LnSpotLight_IsEnabled
        case 0xC3 : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnSpotLight_IsEnabled(local_spotlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnSpotLight_SetColor
        case 0xC4 : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch color
            ??

            stat = LnSpotLight_SetColor(local_spotlight, local_color);


            break;
        }
        // LnSpotLight_GetColor
        case 0xC5 : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnColor local_outReturn;

            stat = LnSpotLight_GetColor(local_spotlight, &local_outReturn);
            ??

            break;
        }
        // LnSpotLight_SetIntensity
        case 0xC6 : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch intensity
            ??

            stat = LnSpotLight_SetIntensity(local_spotlight, local_intensity);


            break;
        }
        // LnSpotLight_GetIntensity
        case 0xC7 : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetIntensity(local_spotlight, &local_outReturn);
            ??

            break;
        }
        // LnSpotLight_SetRange
        case 0xC8 : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch range
            ??

            stat = LnSpotLight_SetRange(local_spotlight, local_range);


            break;
        }
        // LnSpotLight_GetRange
        case 0xC9 : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetRange(local_spotlight, &local_outReturn);
            ??

            break;
        }
        // LnSpotLight_SetAttenuation
        case 0xCA : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch attenuation
            ??

            stat = LnSpotLight_SetAttenuation(local_spotlight, local_attenuation);


            break;
        }
        // LnSpotLight_GetAttenuation
        case 0xCB : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetAttenuation(local_spotlight, &local_outReturn);
            ??

            break;
        }
        // LnSpotLight_SetAngle
        case 0xCC : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch angle
            ??

            stat = LnSpotLight_SetAngle(local_spotlight, local_angle);


            break;
        }
        // LnSpotLight_GetAngle
        case 0xCD : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetAngle(local_spotlight, &local_outReturn);
            ??

            break;
        }
        // LnSpotLight_SetPenumbra
        case 0xCE : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch penumbra
            ??

            stat = LnSpotLight_SetPenumbra(local_spotlight, local_penumbra);


            break;
        }
        // LnSpotLight_GetPenumbra
        case 0xCF : {
            // Fetch spotlight
            const int local_spotlight = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetPenumbra(local_spotlight, &local_outReturn);
            ??

            break;
        }
        // LnSpotLight_Create
        case 0xD0 : {
            // Fetch outSpotLight
            const PVal* pval_outSpotLight;
            const APTR aptr_outSpotLight = code_getva(&pval_outSpotLight);
            LnHandle local_outSpotLight;

            stat = LnSpotLight_Create(&local_outSpotLight);
            code_setva(pval_outSpotLight, aptr_outSpotLight, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outSpotLight));

            break;
        }
        // LnSpotLight_CreateWithColorAndRange
        case 0xD1 : {
            // Fetch color
            ??
            // Fetch range
            ??
            // Fetch angle
            ??
            // Fetch outSpotLight
            const PVal* pval_outSpotLight;
            const APTR aptr_outSpotLight = code_getva(&pval_outSpotLight);
            LnHandle local_outSpotLight;

            stat = LnSpotLight_CreateWithColorAndRange(local_color, local_range, local_angle, &local_outSpotLight);
            code_setva(pval_outSpotLight, aptr_outSpotLight, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outSpotLight));

            break;
        }
        // LnTestDelegate_Create
        case 0xD2 : {
            // Fetch outTestDelegate
            const PVal* pval_outTestDelegate;
            const APTR aptr_outTestDelegate = code_getva(&pval_outTestDelegate);
            LnHandle local_outTestDelegate;

            stat = LnTestDelegate_Create(&local_outTestDelegate);
            code_setva(pval_outTestDelegate, aptr_outTestDelegate, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outTestDelegate));

            break;
        }
        // LnSprite_SetTexture
        case 0xD3 : {
            // Fetch sprite
            const int local_sprite = fetchVAInt();
            // Fetch value
            const int local_value = fetchVAInt();

            stat = LnSprite_SetTexture(local_sprite, local_value);


            break;
        }
        // LnSprite_SetSize
        case 0xD4 : {
            // Fetch sprite
            const int local_sprite = fetchVAInt();
            // Fetch value
            ??

            stat = LnSprite_SetSize(local_sprite, local_value);


            break;
        }
        // LnSprite_SetSizeWH
        case 0xD5 : {
            // Fetch sprite
            const int local_sprite = fetchVAInt();
            // Fetch width
            ??
            // Fetch height
            ??

            stat = LnSprite_SetSizeWH(local_sprite, local_width, local_height);


            break;
        }
        // LnSprite_SetSourceRectXYWH
        case 0xD6 : {
            // Fetch sprite
            const int local_sprite = fetchVAInt();
            // Fetch x
            ??
            // Fetch y
            ??
            // Fetch width
            ??
            // Fetch height
            ??

            stat = LnSprite_SetSourceRectXYWH(local_sprite, local_x, local_y, local_width, local_height);


            break;
        }
        // LnSprite_SetCallerTest
        case 0xD7 : {
            // Fetch sprite
            const int local_sprite = fetchVAInt();
            // Fetch callback
            const int local_callback = fetchVAInt();

            stat = LnSprite_SetCallerTest(local_sprite, local_callback);


            break;
        }
        // LnSprite_Create
        case 0xD8 : {
            // Fetch outSprite
            const PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LnHandle local_outSprite;

            stat = LnSprite_Create(&local_outSprite);
            code_setva(pval_outSprite, aptr_outSprite, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outSprite));

            break;
        }
        // LnSprite_CreateWithTexture
        case 0xD9 : {
            // Fetch texture
            const int local_texture = fetchVAInt();
            // Fetch outSprite
            const PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LnHandle local_outSprite;

            stat = LnSprite_CreateWithTexture(local_texture, &local_outSprite);
            code_setva(pval_outSprite, aptr_outSprite, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outSprite));

            break;
        }
        // LnSprite_CreateWithTextureAndSize
        case 0xDA : {
            // Fetch texture
            const int local_texture = fetchVAInt();
            // Fetch width
            ??
            // Fetch height
            ??
            // Fetch outSprite
            const PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LnHandle local_outSprite;

            stat = LnSprite_CreateWithTextureAndSize(local_texture, local_width, local_height, &local_outSprite);
            code_setva(pval_outSprite, aptr_outSprite, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outSprite));

            break;
        }
        // LnCameraOrbitControlComponent_Create
        case 0xDB : {
            // Fetch outCameraOrbitControlComponent
            const PVal* pval_outCameraOrbitControlComponent;
            const APTR aptr_outCameraOrbitControlComponent = code_getva(&pval_outCameraOrbitControlComponent);
            LnHandle local_outCameraOrbitControlComponent;

            stat = LnCameraOrbitControlComponent_Create(&local_outCameraOrbitControlComponent);
            code_setva(pval_outCameraOrbitControlComponent, aptr_outCameraOrbitControlComponent, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outCameraOrbitControlComponent));

            break;
        }
        // LnRaycaster_FromScreen
        case 0xDC : {
            // Fetch point
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnRaycaster_FromScreen(local_point, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnRaycaster_IntersectPlane
        case 0xDD : {
            // Fetch raycaster
            const int local_raycaster = fetchVAInt();
            // Fetch normalX
            ??
            // Fetch normalY
            ??
            // Fetch normalZ
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnRaycaster_IntersectPlane(local_raycaster, local_normalX, local_normalY, local_normalZ, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnRaycastResult_GetPoint
        case 0xDE : {
            // Fetch raycastresult
            const int local_raycastresult = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnRaycastResult_GetPoint(local_raycastresult, &local_outReturn);
            ??

            break;
        }
        // LnWorldRenderView_SetGuideGridEnabled
        case 0xDF : {
            // Fetch worldrenderview
            const int local_worldrenderview = fetchVAInt();
            // Fetch value
            const int local_value = fetchVAInt();

            stat = LnWorldRenderView_SetGuideGridEnabled(local_worldrenderview, local_value);


            break;
        }
        // LnWorldRenderView_GetGuideGridEnabled
        case 0xE0 : {
            // Fetch worldrenderview
            const int local_worldrenderview = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnWorldRenderView_GetGuideGridEnabled(local_worldrenderview, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnBoxMesh_Create
        case 0xE1 : {
            // Fetch outBoxMesh
            const PVal* pval_outBoxMesh;
            const APTR aptr_outBoxMesh = code_getva(&pval_outBoxMesh);
            LnHandle local_outBoxMesh;

            stat = LnBoxMesh_Create(&local_outBoxMesh);
            code_setva(pval_outBoxMesh, aptr_outBoxMesh, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outBoxMesh));

            break;
        }
        // LnPlaneMesh_Create
        case 0xE2 : {
            // Fetch outPlaneMesh
            const PVal* pval_outPlaneMesh;
            const APTR aptr_outPlaneMesh = code_getva(&pval_outPlaneMesh);
            LnHandle local_outPlaneMesh;

            stat = LnPlaneMesh_Create(&local_outPlaneMesh);
            code_setva(pval_outPlaneMesh, aptr_outPlaneMesh, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outPlaneMesh));

            break;
        }
        // LnUIEventArgs_Sender
        case 0xE3 : {
            // Fetch uieventargs
            const int local_uieventargs = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnUIEventArgs_Sender(local_uieventargs, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnUIGeneralEventHandler_Create
        case 0xE4 : {
            // Fetch outUIGeneralEventHandler
            const PVal* pval_outUIGeneralEventHandler;
            const APTR aptr_outUIGeneralEventHandler = code_getva(&pval_outUIGeneralEventHandler);
            LnHandle local_outUIGeneralEventHandler;

            stat = LnUIGeneralEventHandler_Create(&local_outUIGeneralEventHandler);
            code_setva(pval_outUIGeneralEventHandler, aptr_outUIGeneralEventHandler, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outUIGeneralEventHandler));

            break;
        }
        // LnUIEventHandler_Create
        case 0xE5 : {
            // Fetch outUIEventHandler
            const PVal* pval_outUIEventHandler;
            const APTR aptr_outUIEventHandler = code_getva(&pval_outUIEventHandler);
            LnHandle local_outUIEventHandler;

            stat = LnUIEventHandler_Create(&local_outUIEventHandler);
            code_setva(pval_outUIEventHandler, aptr_outUIEventHandler, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outUIEventHandler));

            break;
        }
        // LnUIElement_SetMargin
        case 0xE6 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch margin
            ??

            stat = LnUIElement_SetMargin(local_uielement, local_margin);


            break;
        }
        // LnUIElement_GetMargin
        case 0xE7 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnThickness local_outReturn;

            stat = LnUIElement_GetMargin(local_uielement, &local_outReturn);
            ??

            break;
        }
        // LnUIElement_SetPadding
        case 0xE8 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch padding
            ??

            stat = LnUIElement_SetPadding(local_uielement, local_padding);


            break;
        }
        // LnUIElement_GetPadding
        case 0xE9 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnThickness local_outReturn;

            stat = LnUIElement_GetPadding(local_uielement, &local_outReturn);
            ??

            break;
        }
        // LnUIElement_SetHAlignment
        case 0xEA : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch value
            ??

            stat = LnUIElement_SetHAlignment(local_uielement, local_value);


            break;
        }
        // LnUIElement_GetHAlignment
        case 0xEB : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHAlignment local_outReturn;

            stat = LnUIElement_GetHAlignment(local_uielement, &local_outReturn);
            ??

            break;
        }
        // LnUIElement_SetVAlignment
        case 0xEC : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch value
            ??

            stat = LnUIElement_SetVAlignment(local_uielement, local_value);


            break;
        }
        // LnUIElement_GetVAlignment
        case 0xED : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVAlignment local_outReturn;

            stat = LnUIElement_GetVAlignment(local_uielement, &local_outReturn);
            ??

            break;
        }
        // LnUIElement_SetAlignments
        case 0xEE : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch halign
            ??
            // Fetch valign
            ??

            stat = LnUIElement_SetAlignments(local_uielement, local_halign, local_valign);


            break;
        }
        // LnUIElement_SetPosition
        case 0xEF : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch pos
            ??

            stat = LnUIElement_SetPosition(local_uielement, local_pos);


            break;
        }
        // LnUIElement_SetPositionXYZ
        case 0xF0 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch x
            ??
            // Fetch y
            ??
            // Fetch z
            ??

            stat = LnUIElement_SetPositionXYZ(local_uielement, local_x, local_y, local_z);


            break;
        }
        // LnUIElement_GetPosition
        case 0xF1 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnUIElement_GetPosition(local_uielement, &local_outReturn);
            ??

            break;
        }
        // LnUIElement_SetRotation
        case 0xF2 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch rot
            ??

            stat = LnUIElement_SetRotation(local_uielement, local_rot);


            break;
        }
        // LnUIElement_SetEulerAngles
        case 0xF3 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch x
            ??
            // Fetch y
            ??
            // Fetch z
            ??

            stat = LnUIElement_SetEulerAngles(local_uielement, local_x, local_y, local_z);


            break;
        }
        // LnUIElement_GetRotation
        case 0xF4 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnQuaternion local_outReturn;

            stat = LnUIElement_GetRotation(local_uielement, &local_outReturn);
            ??

            break;
        }
        // LnUIElement_SetScale
        case 0xF5 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch scale
            ??

            stat = LnUIElement_SetScale(local_uielement, local_scale);


            break;
        }
        // LnUIElement_SetScaleS
        case 0xF6 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch xyz
            ??

            stat = LnUIElement_SetScaleS(local_uielement, local_xyz);


            break;
        }
        // LnUIElement_SetScaleXY
        case 0xF7 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch x
            ??
            // Fetch y
            ??

            stat = LnUIElement_SetScaleXY(local_uielement, local_x, local_y);


            break;
        }
        // LnUIElement_GetScale
        case 0xF8 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnUIElement_GetScale(local_uielement, &local_outReturn);
            ??

            break;
        }
        // LnUIElement_SetCenterPoint
        case 0xF9 : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch value
            ??

            stat = LnUIElement_SetCenterPoint(local_uielement, local_value);


            break;
        }
        // LnUIElement_SetCenterPointXYZ
        case 0xFA : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch x
            ??
            // Fetch y
            ??
            // Fetch z
            ??

            stat = LnUIElement_SetCenterPointXYZ(local_uielement, local_x, local_y, local_z);


            break;
        }
        // LnUIElement_GetCenterPoint
        case 0xFB : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnUIElement_GetCenterPoint(local_uielement, &local_outReturn);
            ??

            break;
        }
        // LnUIElement_AddChild
        case 0xFC : {
            // Fetch uielement
            const int local_uielement = fetchVAInt();
            // Fetch child
            const int local_child = fetchVAInt();

            stat = LnUIElement_AddChild(local_uielement, local_child);


            break;
        }
        // LnUIButtonBase_SetTextA
        case 0xFD : {
            // Fetch uibuttonbase
            const int local_uibuttonbase = fetchVAInt();
            // Fetch text
            ??

            stat = LnUIButtonBase_SetTextA(local_uibuttonbase, local_text);


            break;
        }
        // LnUIButton_Create
        case 0xFE : {
            // Fetch outUIButton
            const PVal* pval_outUIButton;
            const APTR aptr_outUIButton = code_getva(&pval_outUIButton);
            LnHandle local_outUIButton;

            stat = LnUIButton_Create(&local_outUIButton);
            code_setva(pval_outUIButton, aptr_outUIButton, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outUIButton));

            break;
        }
        // LnUIButton_CreateWithTextA
        case 0xFF : {
            // Fetch text
            ??
            // Fetch outUIButton
            const PVal* pval_outUIButton;
            const APTR aptr_outUIButton = code_getva(&pval_outUIButton);
            LnHandle local_outUIButton;

            stat = LnUIButton_CreateWithTextA(local_text, &local_outUIButton);
            code_setva(pval_outUIButton, aptr_outUIButton, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outUIButton));

            break;
        }
        // LnUIButton_ConnectOnClicked
        case 0x100 : {
            // Fetch uibutton
            const int local_uibutton = fetchVAInt();
            // Fetch handler
            const int local_handler = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnUIButton_ConnectOnClicked(local_uibutton, local_handler, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnUITextBlock_Create
        case 0x101 : {
            // Fetch outUITextBlock
            const PVal* pval_outUITextBlock;
            const APTR aptr_outUITextBlock = code_getva(&pval_outUITextBlock);
            LnHandle local_outUITextBlock;

            stat = LnUITextBlock_Create(&local_outUITextBlock);
            code_setva(pval_outUITextBlock, aptr_outUITextBlock, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outUITextBlock));

            break;
        }
        // LnUITextBlock_CreateWithTextA
        case 0x102 : {
            // Fetch text
            ??
            // Fetch outUITextBlock
            const PVal* pval_outUITextBlock;
            const APTR aptr_outUITextBlock = code_getva(&pval_outUITextBlock);
            LnHandle local_outUITextBlock;

            stat = LnUITextBlock_CreateWithTextA(local_text, &local_outUITextBlock);
            code_setva(pval_outUITextBlock, aptr_outUITextBlock, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outUITextBlock));

            break;
        }
        // LnUISprite_SetTexture
        case 0x103 : {
            // Fetch uisprite
            const int local_uisprite = fetchVAInt();
            // Fetch texture
            const int local_texture = fetchVAInt();

            stat = LnUISprite_SetTexture(local_uisprite, local_texture);


            break;
        }
        // LnUISprite_SetSourceRect
        case 0x104 : {
            // Fetch uisprite
            const int local_uisprite = fetchVAInt();
            // Fetch rect
            ??

            stat = LnUISprite_SetSourceRect(local_uisprite, local_rect);


            break;
        }
        // LnUISprite_SetSourceRectXYWH
        case 0x105 : {
            // Fetch uisprite
            const int local_uisprite = fetchVAInt();
            // Fetch x
            ??
            // Fetch y
            ??
            // Fetch width
            ??
            // Fetch height
            ??

            stat = LnUISprite_SetSourceRectXYWH(local_uisprite, local_x, local_y, local_width, local_height);


            break;
        }
        // LnUISprite_GetSourceRect
        case 0x106 : {
            // Fetch uisprite
            const int local_uisprite = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnRect local_outReturn;

            stat = LnUISprite_GetSourceRect(local_uisprite, &local_outReturn);
            ??

            break;
        }
        // LnUISprite_Create
        case 0x107 : {
            // Fetch outUISprite
            const PVal* pval_outUISprite;
            const APTR aptr_outUISprite = code_getva(&pval_outUISprite);
            LnHandle local_outUISprite;

            stat = LnUISprite_Create(&local_outUISprite);
            code_setva(pval_outUISprite, aptr_outUISprite, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outUISprite));

            break;
        }
        // LnUISprite_CreateWithTexture
        case 0x108 : {
            // Fetch texture
            const int local_texture = fetchVAInt();
            // Fetch outUISprite
            const PVal* pval_outUISprite;
            const APTR aptr_outUISprite = code_getva(&pval_outUISprite);
            LnHandle local_outUISprite;

            stat = LnUISprite_CreateWithTexture(local_texture, &local_outUISprite);
            code_setva(pval_outUISprite, aptr_outUISprite, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outUISprite));

            break;
        }
        // LnInput_PressedA
        case 0x109 : {
            // Fetch buttonName
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_PressedA(local_buttonName, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnInput_TriggeredA
        case 0x10A : {
            // Fetch buttonName
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_TriggeredA(local_buttonName, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnInput_TriggeredOffA
        case 0x10B : {
            // Fetch buttonName
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_TriggeredOffA(local_buttonName, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnInput_RepeatedA
        case 0x10C : {
            // Fetch buttonName
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_RepeatedA(local_buttonName, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnInput_GetAxisValueA
        case 0x10D : {
            // Fetch buttonName
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnInput_GetAxisValueA(local_buttonName, &local_outReturn);
            ??

            break;
        }
        // LnInput_ClearAllBindings
        case 0x10E : {

            stat = LnInput_ClearAllBindings();


            break;
        }
        // LnMouse_Pressed
        case 0x10F : {
            // Fetch button
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_Pressed(local_button, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnMouse_Triggered
        case 0x110 : {
            // Fetch button
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_Triggered(local_button, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnMouse_TriggeredOff
        case 0x111 : {
            // Fetch button
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_TriggeredOff(local_button, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnMouse_Repeated
        case 0x112 : {
            // Fetch button
            ??
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_Repeated(local_button, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnMouse_Position
        case 0x113 : {
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnPoint local_outReturn;

            stat = LnMouse_Position(&local_outReturn);
            ??

            break;
        }
        // LnEngineSettings_SetMainWindowSize
        case 0x114 : {
            // Fetch width
            const int local_width = fetchVAInt();
            // Fetch height
            const int local_height = fetchVAInt();

            stat = LnEngineSettings_SetMainWindowSize(local_width, local_height);


            break;
        }
        // LnEngineSettings_SetMainWorldViewSize
        case 0x115 : {
            // Fetch width
            const int local_width = fetchVAInt();
            // Fetch height
            const int local_height = fetchVAInt();

            stat = LnEngineSettings_SetMainWorldViewSize(local_width, local_height);


            break;
        }
        // LnEngineSettings_SetMainWindowTitleA
        case 0x116 : {
            // Fetch title
            ??

            stat = LnEngineSettings_SetMainWindowTitleA(local_title);


            break;
        }
        // LnEngineSettings_AddAssetDirectoryA
        case 0x117 : {
            // Fetch path
            ??

            stat = LnEngineSettings_AddAssetDirectoryA(local_path);


            break;
        }
        // LnEngineSettings_AddAssetArchiveA
        case 0x118 : {
            // Fetch fileFullPath
            ??
            // Fetch password
            ??

            stat = LnEngineSettings_AddAssetArchiveA(local_fileFullPath, local_password);


            break;
        }
        // LnEngineSettings_SetFrameRate
        case 0x119 : {
            // Fetch value
            const int local_value = fetchVAInt();

            stat = LnEngineSettings_SetFrameRate(local_value);


            break;
        }
        // LnEngineSettings_SetDebugToolEnabled
        case 0x11A : {
            // Fetch enabled
            const int local_enabled = fetchVAInt();

            stat = LnEngineSettings_SetDebugToolEnabled(local_enabled);


            break;
        }
        // LnEngineSettings_SetEngineLogEnabled
        case 0x11B : {
            // Fetch enabled
            const int local_enabled = fetchVAInt();

            stat = LnEngineSettings_SetEngineLogEnabled(local_enabled);


            break;
        }
        // LnEngineSettings_SetEngineLogFilePathA
        case 0x11C : {
            // Fetch filePath
            ??

            stat = LnEngineSettings_SetEngineLogFilePathA(local_filePath);


            break;
        }
        // LnEngine_Initialize
        case 0x11D : {

            stat = LnEngine_Initialize();


            break;
        }
        // LnEngine_Finalize
        case 0x11E : {

            stat = LnEngine_Finalize();


            break;
        }
        // LnEngine_Update
        case 0x11F : {
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnEngine_Update(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnEngine_Time
        case 0x120 : {
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            double local_outReturn;

            stat = LnEngine_Time(&local_outReturn);
            ??

            break;
        }
        // LnEngine_GetCamera
        case 0x121 : {
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnEngine_GetCamera(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnEngine_GetLight
        case 0x122 : {
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnEngine_GetLight(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnEngine_GetRenderView
        case 0x123 : {
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnEngine_GetRenderView(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnApplication_OnInit
        case 0x124 : {
            // Fetch application
            const int local_application = fetchVAInt();

            stat = LnApplication_OnInit(local_application);


            break;
        }
        // LnApplication_OnUpdate
        case 0x125 : {
            // Fetch application
            const int local_application = fetchVAInt();

            stat = LnApplication_OnUpdate(local_application);


            break;
        }
        // LnApplication_World
        case 0x126 : {
            // Fetch application
            const int local_application = fetchVAInt();
            // Fetch outReturn
            const PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnApplication_World(local_application, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outReturn));

            break;
        }
        // LnApplication_Create
        case 0x127 : {
            // Fetch outApplication
            const PVal* pval_outApplication;
            const APTR aptr_outApplication = code_getva(&pval_outApplication);
            LnHandle local_outApplication;

            stat = LnApplication_Create(&local_outApplication);
            code_setva(pval_outApplication, aptr_outApplication, HSPVAR_FLAG_INT, static_cast<int32_t*>(&local_outApplication));

            break;
        }
        // LnDebug_PrintA
        case 0x128 : {
            // Fetch str
            ??

            stat = LnDebug_PrintA(local_str);


            break;
        }
        // LnDebug_PrintWithTimeA
        case 0x129 : {
            // Fetch time
            ??
            // Fetch str
            ??

            stat = LnDebug_PrintWithTimeA(local_time, local_str);


            break;
        }
        // LnDebug_PrintWithTimeAndColorA
        case 0x12A : {
            // Fetch time
            ??
            // Fetch color
            ??
            // Fetch str
            ??

            stat = LnDebug_PrintWithTimeAndColorA(local_time, local_color, local_str);


            break;
        }
    }
}
