
#pragma once
#include "FlatCommon.h"

extern "C"
{
/**
    @brief 3次元のベクトルを定義します。
*/
struct LNVector3
{
    float x;
    float y;
    float z;
};

/**
    @brief 4次元のベクトルを定義します。
*/
struct LNVector4
{
    float x;
    float y;
    float z;
    float w;
};

/**
    @brief クォータニオンを定義します。
    @details このクォータニオンクラスの乗算の順番は左から右です。つまり、q1 * q2 は、先に q1、次に q2 の回転を適用する事と同じです。
    この乗算順序は数学的な四元数の定義からは逆行している点に注意してください。
*/
struct LNQuaternion
{
    float x;
    float y;
    float z;
    float w;
};

/**
    @brief 4x4 の行列を定義します。
    @details 行列は数学的な定義とメモリレイアウト共に行優先 (column-major) です。
    このクラスは似た機能の static 関数とインスタンス関数があります。例えば makeRotationX(static 関数) と rotateX(インスタンス関数) です。前者は新しい行列を作成して返すのに対し、後者は現在の行列を変更します。例えば、以下の m1 と m2 は同じ結果になります。
    ~~~Matrix m1 = Matrix::makeRotationX(0.1) * Matrix::makeRotationY(0.2) * Matrix::makeRotationZ(0.3);
    Matrix m2;m2.rotateX(0.1);m2.rotateY(0.2);m2.rotateZ(0.3);~~~
    なお、後者は行列の生成と乗算をまとめて行うように最適化されており、高速に動作します。
*/
struct LNMatrix
{
    LNVector4 row0;
    LNVector4 row1;
    LNVector4 row2;
    LNVector4 row3;
};

/**
    @brief 各要素を 0.0～1.0 の範囲で表す RGBA カラーを定義します。
*/
struct LNColor
{
    float r;
    float g;
    float b;
    float a;
};

/**
    @brief 2次元上の点を表します。
*/
struct LNPoint
{
    float x;
    float y;
};

/**
    @brief 2次元上のオブジェクトサイズを表します。
*/
struct LNSize
{
    float width;
    float height;
};

/**
    @brief 2次元の矩形を表すクラスです。
*/
struct LNRect
{
    float x;
    float y;
    float width;
    float height;
};

/**
    @brief 四角形の枠の太さを表すクラスです。
*/
struct LNThickness
{
    float left;
    float top;
    float right;
    float bottom;
};



/**
    @brief マウスボタン
*/
typedef enum tagLNMouseButtons
{
    /**
        @brief 無効または押されていないことを示す
    */
    LN_MOUSE_BUTTONS_NONE = 0,

    /**
        @brief 左ボタン
    */
    LN_MOUSE_BUTTONS_LEFT = 1,

    /**
        @brief 右ボタン
    */
    LN_MOUSE_BUTTONS_RIGHT = 2,

    /**
        @brief 中央ボタン
    */
    LN_MOUSE_BUTTONS_MIDDLE = 3,

    /**
        @brief 拡張ボタン1
    */
    LN_MOUSE_BUTTONS_X1 = 4,

    /**
        @brief 拡張ボタン2
    */
    LN_MOUSE_BUTTONS_X2 = 5,

} LNMouseButtons;

/**
    @brief ピクセルフォーマット
*/
typedef enum tagLNPixelFormat
{
    /**
        @brief Unknown
    */
    LN_PIXEL_FORMAT_UNKNOWN = 0,

    /**
        @brief 8bit アルファ値のみのフォーマット
    */
    LN_PIXEL_FORMAT_A8 = 1,

    /**
        @brief RGBA オーダーの各要素 8bit フォーマット
    */
    LN_PIXEL_FORMAT_RGBA8 = 2,

    /**
        @brief RGB オーダーの各要素 8bit フォーマット
    */
    LN_PIXEL_FORMAT_RGB8 = 3,

    /**
        @brief RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    */
    LN_PIXEL_FORMAT_RGBA32F = 4,

    /**
        @brief RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    */
    LN_PIXEL_FORMAT_R32S = 5,

} LNPixelFormat;

/**
    @brief テクスチャのピクセルフォーマット
*/
typedef enum tagLNTextureFormat
{
    /**
        @brief Unknown
    */
    LN_TEXTURE_FORMAT_UNKNOWN = 0,

    /**
        @brief RGBA オーダーの各要素 8bit フォーマット
    */
    LN_TEXTURE_FORMAT_RGBA8 = 1,

    /**
        @brief RGB オーダーの各要素 8bit フォーマット
    */
    LN_TEXTURE_FORMAT_RGB8 = 2,

    /**
        @brief RGBA オーダーの各要素 16bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_RGBA16F = 3,

    /**
        @brief RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_RGBA32F = 4,

    /**
        @brief 16bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_R16F = 5,

    /**
        @brief 32bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_R32F = 6,

    /**
        @brief 32bit の符号あり整数フォーマット
    */
    LN_TEXTURE_FORMAT_R32S = 7,

} LNTextureFormat;

/**
    @brief 深度バッファのフォーマット
*/
typedef enum tagLNDepthBufferFormat
{
    /**
        @brief S8 32 ビット
    */
    LN_DEPTH_BUFFER_FORMAT_D24S8 = 0,

} LNDepthBufferFormat;

/**
    @brief 縦方向の表示位置を示します。
*/
typedef enum tagLNVAlignment
{
    /**
        @brief 子要素を、親のレイアウト スロットの上端に揃えて配置します。
    */
    LN_VALIGNMENT_TOP = 0,

    /**
        @brief 子要素を、親のレイアウト スロットの中央に揃えて配置します。
    */
    LN_VALIGNMENT_CENTER = 1,

    /**
        @brief 子要素を、親のレイアウト スロットの下端に揃えて配置します。
    */
    LN_VALIGNMENT_BOTTOM = 2,

    /**
        @brief 子要素を、親のレイアウト スロット全体に引き伸ばします。
    */
    LN_VALIGNMENT_STRETCH = 3,

} LNVAlignment;

/**
    @brief 横方向の表示位置を示します。
*/
typedef enum tagLNHAlignment
{
    /**
        @brief 子要素を、親のレイアウト スロットの左側に揃えて配置します。
    */
    LN_HALIGNMENT_LEFT = 0,

    /**
        @brief 子要素を、親のレイアウト スロットの中央に揃えて配置します。
    */
    LN_HALIGNMENT_CENTER = 1,

    /**
        @brief 子要素を、親のレイアウト スロットの右側に揃えて配置します。
    */
    LN_HALIGNMENT_RIGHT = 2,

    /**
        @brief 子要素を、親のレイアウト スロット全体に引き伸ばします。
    */
    LN_HALIGNMENT_STRETCH = 3,

} LNHAlignment;

typedef LNResult(*LNPromiseFailureDelegateCallback)(LNHandle promisefailuredelegate);
typedef LNResult(*LNZVTestDelegate1Callback)(LNHandle zvtestdelegate1, int p1);
typedef LNResult(*LNZVTestDelegate2Callback)(LNHandle zvtestdelegate2, int p1, int p2, int* outReturn);
typedef LNResult(*LNZVTestDelegate3Callback)(LNHandle zvtestdelegate3, LNHandle p1);
typedef LNResult(*LNZVTestEventHandler1Callback)(LNHandle zvtesteventhandler1);
typedef LNResult(*LNZVTestEventHandler2Callback)(LNHandle zvtesteventhandler2, LNHandle p1);
typedef LNResult(*LNTexture2DDelegateCallback)(LNHandle texture2ddelegate, LNHandle p1);
typedef LNResult(*LNTestDelegateCallback)(LNHandle testdelegate, int p1, int* outReturn);
typedef LNResult(*LNUIGeneralEventHandlerCallback)(LNHandle uigeneraleventhandler, LNHandle p1);
typedef LNResult(*LNUIEventHandlerCallback)(LNHandle uieventhandler);
typedef LNResult(*LNInterpreterCommandDelegateCallback)(LNHandle interpretercommanddelegate, LNHandle p1, LNBool* outReturn);
typedef LNResult(*LNObjectSerializeHandlerCallback)(LNHandle objectserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNEventConnectionSerializeHandlerCallback)(LNHandle eventconnectionserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNZVTestClass1SerializeHandlerCallback)(LNHandle zvtestclass1serializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNZVTestEventArgs1SerializeHandlerCallback)(LNHandle zvtesteventargs1serializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNSerializer2SerializeHandlerCallback)(LNHandle serializer2serializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNAssetModelSerializeHandlerCallback)(LNHandle assetmodelserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNTextureSerializeHandlerCallback)(LNHandle textureserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNTexture2DSerializeHandlerCallback)(LNHandle texture2dserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNRenderViewSerializeHandlerCallback)(LNHandle renderviewserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNComponentSerializeHandlerCallback)(LNHandle componentserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNVisualComponentSerializeHandlerCallback)(LNHandle visualcomponentserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNSpriteComponentSerializeHandlerCallback)(LNHandle spritecomponentserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNWorldSerializeHandlerCallback)(LNHandle worldserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNComponentListSerializeHandlerCallback)(LNHandle componentlistserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNWorldObjectSerializeHandlerCallback)(LNHandle worldobjectserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNWorldObjectUpdateHandlerCallback)(LNHandle worldobjectupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNVisualObjectSerializeHandlerCallback)(LNHandle visualobjectserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNVisualObjectUpdateHandlerCallback)(LNHandle visualobjectupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNCameraSerializeHandlerCallback)(LNHandle cameraserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNCameraUpdateHandlerCallback)(LNHandle cameraupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNEnvironmentLightSerializeHandlerCallback)(LNHandle environmentlightserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNEnvironmentLightUpdateHandlerCallback)(LNHandle environmentlightupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNDirectionalLightSerializeHandlerCallback)(LNHandle directionallightserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNDirectionalLightUpdateHandlerCallback)(LNHandle directionallightupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNPointLightSerializeHandlerCallback)(LNHandle pointlightserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNPointLightUpdateHandlerCallback)(LNHandle pointlightupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNSpotLightSerializeHandlerCallback)(LNHandle spotlightserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNSpotLightUpdateHandlerCallback)(LNHandle spotlightupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNSpriteSerializeHandlerCallback)(LNHandle spriteserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNSpriteUpdateHandlerCallback)(LNHandle spriteupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNCameraOrbitControlComponentSerializeHandlerCallback)(LNHandle cameraorbitcontrolcomponentserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNRaycasterSerializeHandlerCallback)(LNHandle raycasterserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNRaycastResultSerializeHandlerCallback)(LNHandle raycastresultserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNWorldRenderViewSerializeHandlerCallback)(LNHandle worldrenderviewserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNBoxMeshSerializeHandlerCallback)(LNHandle boxmeshserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNBoxMeshUpdateHandlerCallback)(LNHandle boxmeshupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNPlaneMeshSerializeHandlerCallback)(LNHandle planemeshserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNPlaneMeshUpdateHandlerCallback)(LNHandle planemeshupdatehandler, LNHandle self, float elapsedSeconds);
typedef LNResult(*LNLevelSerializeHandlerCallback)(LNHandle levelserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNLevelStartHandlerCallback)(LNHandle levelstarthandler, LNHandle self);
typedef LNResult(*LNLevelStopHandlerCallback)(LNHandle levelstophandler, LNHandle self);
typedef LNResult(*LNLevelPauseHandlerCallback)(LNHandle levelpausehandler, LNHandle self);
typedef LNResult(*LNLevelResumeHandlerCallback)(LNHandle levelresumehandler, LNHandle self);
typedef LNResult(*LNLevelUpdateHandlerCallback)(LNHandle levelupdatehandler, LNHandle self);
typedef LNResult(*LNUIEventArgsSerializeHandlerCallback)(LNHandle uieventargsserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUILayoutElementSerializeHandlerCallback)(LNHandle uilayoutelementserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUIElementSerializeHandlerCallback)(LNHandle uielementserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUITextBlockSerializeHandlerCallback)(LNHandle uitextblockserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNUISpriteSerializeHandlerCallback)(LNHandle uispriteserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNInterpreterCommandSerializeHandlerCallback)(LNHandle interpretercommandserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNInterpreterCommandListSerializeHandlerCallback)(LNHandle interpretercommandlistserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNInterpreterSerializeHandlerCallback)(LNHandle interpreterserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNInterpreterUpdateWaitHandlerCallback)(LNHandle interpreterupdatewaithandler, LNHandle self, const LNChar* waitMode, LNBool* outReturn);
typedef LNResult(*LNApplicationSerializeHandlerCallback)(LNHandle applicationserializehandler, LNHandle self, LNHandle ar);
typedef LNResult(*LNApplicationInitHandlerCallback)(LNHandle applicationinithandler, LNHandle self);
typedef LNResult(*LNApplicationUpdateHandlerCallback)(LNHandle applicationupdatehandler, LNHandle self);


//==============================================================================
// ln::Vector3

/**
    @brief すべての要素を 0.0 に設定してインスタンスを初期化します。
    @param[in] vector3 : instance
*/
LN_FLAT_API LNResult LNVector3_SetZeros(LNVector3* vector3);


/**
    @brief 指定した値を使用してインスタンスを初期化します。
    @param[in] vector3 : instance
*/
LN_FLAT_API LNResult LNVector3_Set(LNVector3* vector3, float x, float y, float z);


/**
    @brief ベクトルの長さを返します。
    @param[in] vector3 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNVector3_Length(const LNVector3* vector3, float* outReturn);


/**
    @brief ベクトルの長さの 2 乗を返します。
    @param[in] vector3 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNVector3_LengthSquared(const LNVector3* vector3, float* outReturn);


/**
    @brief このベクトルを正規化します。
    @param[in] vector3 : instance
    @details ベクトルの長さが 0 の場合は正規化を行いません。
*/
LN_FLAT_API LNResult LNVector3_MutatingNormalize(LNVector3* vector3);


/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] x : 処理の基になるベクトルの X 要素
    @param[in] y : 処理の基になるベクトルの Y 要素
    @param[in] z : 処理の基になるベクトルの Z 要素
    @param[out] outReturn : instance.
    @return 正規化されたベクトル
*/
LN_FLAT_API LNResult LNVector3_NormalizeXYZ(float x, float y, float z, LNVector3* outReturn);


/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] vec : 処理の基になるベクトル
    @param[out] outReturn : instance.
    @return 正規化されたベクトル
*/
LN_FLAT_API LNResult LNVector3_Normalize(const LNVector3* vec, LNVector3* outReturn);


//==============================================================================
// ln::Vector4

/**
    @brief すべての要素を 0.0 に設定してインスタンスを初期化します。
    @param[in] vector4 : instance
*/
LN_FLAT_API LNResult LNVector4_SetZeros(LNVector4* vector4);


/**
    @brief 指定した値を使用してインスタンスを初期化します。
    @param[in] vector4 : instance
*/
LN_FLAT_API LNResult LNVector4_Set(LNVector4* vector4, float x, float y, float z, float w);


//==============================================================================
// ln::Quaternion

/**
    @brief 単位クォータニオンを設定してインスタンスを初期化します。
    @param[in] quaternion : instance
*/
LN_FLAT_API LNResult LNQuaternion_SetZeros(LNQuaternion* quaternion);


/**
    @brief 指定した値を使用してインスタンスを初期化します。
    @param[in] quaternion : instance
*/
LN_FLAT_API LNResult LNQuaternion_Set(LNQuaternion* quaternion, float x, float y, float z, float w);


/**
    @brief 回転軸と回転角度を指定してインスタンスを初期化します。
    @param[in] quaternion : instance
    @param[in] axis : 回転軸
    @param[in] r : 回転角度 (ラジアン単位)
    @details axis が単位ベクトルでなければ正規化してから計算を行います。
*/
LN_FLAT_API LNResult LNQuaternion_SetFromAxis(LNQuaternion* quaternion, const LNVector3* axis, float r);


//==============================================================================
// ln::Matrix

/**
    @brief 単位行列を設定してインスタンスを初期化します。
    @param[in] matrix : instance
*/
LN_FLAT_API LNResult LNMatrix_SetZeros(LNMatrix* matrix);


/**
    @brief 各要素を指定してインスタンスを初期化します。
    @param[in] matrix : instance
*/
LN_FLAT_API LNResult LNMatrix_Set(LNMatrix* matrix, float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);


//==============================================================================
// ln::Color

/**
    @brief すべての要素を 0.0 で初期化します。
    @param[in] color : instance
*/
LN_FLAT_API LNResult LNColor_SetZeros(LNColor* color);


/**
    @brief 各要素を指定して初期化します。
    @param[in] color : instance
*/
LN_FLAT_API LNResult LNColor_Set(LNColor* color, float r_, float g_, float b_, float a_);


//==============================================================================
// ln::Point

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] point : instance
*/
LN_FLAT_API LNResult LNPoint_SetZeros(LNPoint* point);


/**
    @brief 位置を指定して初期化します。
    @param[in] point : instance
*/
LN_FLAT_API LNResult LNPoint_Set(LNPoint* point, float x_, float y_);


//==============================================================================
// ln::Size

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] size : instance
*/
LN_FLAT_API LNResult LNSize_SetZeros(LNSize* size);


/**
    @brief 幅と高さを指定して初期化します。
    @param[in] size : instance
*/
LN_FLAT_API LNResult LNSize_Set(LNSize* size, float w, float h);


//==============================================================================
// ln::Rect

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] rect : instance
*/
LN_FLAT_API LNResult LNRect_SetZeros(LNRect* rect);


/**
    @brief 位置とサイズを指定して初期化します。
    @param[in] rect : instance
*/
LN_FLAT_API LNResult LNRect_Set(LNRect* rect, float x, float y, float width, float height);


/**
    @brief 左辺の x 軸の値を取得します。
    @param[in] rect : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNRect_GetLeft(const LNRect* rect, float* outReturn);


/**
    @brief 幅と高さを設定します。
    @param[in] rect : instance
*/
LN_FLAT_API LNResult LNRect_SetSize(LNRect* rect, const LNSize* size);


/**
    @brief 幅と高さを取得します。
    @param[in] rect : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNRect_GetSize(const LNRect* rect, LNSize* outReturn);


//==============================================================================
// ln::Thickness

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] thickness : instance
*/
LN_FLAT_API LNResult LNThickness_SetZeros(LNThickness* thickness);


/**
    @brief 各辺の幅を指定して初期化します。
    @param[in] thickness : instance
*/
LN_FLAT_API LNResult LNThickness_Set(LNThickness* thickness, float left_, float top_, float right_, float bottom_);




//==============================================================================
// ln::Object

/**
    @brief オブジェクトの参照を開放します。
    @param[in] object : instance
*/
LN_FLAT_API LNResult LNObject_Release(LNHandle object);

/**
    @brief オブジェクトの参照を取得します。
    @param[in] object : instance
*/
LN_FLAT_API LNResult LNObject_Retain(LNHandle object);

/**
    @brief オブジェクトの参照カウントを取得します。これは内部的に使用される関数です。
    @param[in] object : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNObject_ReferenceCount(LNHandle object, int* outReturn);

typedef LNResult(*LNObject_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNObject_OnSerialize_SetOverrideCallback(LNObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] object : instance
*/
LN_FLAT_API LNResult LNObject_SetPrototype_OnSerialize(LNHandle object, LNHandle callback);

extern LN_FLAT_API int LNObject_GetTypeInfoId();
LN_FLAT_API void LNObject_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNObject_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNObject_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNObject_SubclassRegistrationInfo;

extern LN_FLAT_API void LNObject_RegisterSubclassTypeInfo(const LNObject_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNObject_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::EventConnection

typedef LNResult(*LNEventConnection_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNEventConnection_OnSerialize_SetOverrideCallback(LNEventConnection_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNEventConnection_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] eventconnection : instance
*/
LN_FLAT_API LNResult LNEventConnection_SetPrototype_OnSerialize(LNHandle eventconnection, LNHandle callback);

extern LN_FLAT_API int LNEventConnection_GetTypeInfoId();
LN_FLAT_API void LNEventConnection_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEventConnection_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNEventConnection_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNEventConnection_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEventConnection_RegisterSubclassTypeInfo(const LNEventConnection_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEventConnection_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::PromiseFailureDelegate

LN_FLAT_API LNResult LNPromiseFailureDelegate_Create(LNPromiseFailureDelegateCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPromiseFailureDelegate_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPromiseFailureDelegate_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPromiseFailureDelegate_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPromiseFailureDelegate_RegisterSubclassTypeInfo(const LNPromiseFailureDelegate_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPromiseFailureDelegate_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestDelegate1

LN_FLAT_API LNResult LNZVTestDelegate1_Create(LNZVTestDelegate1Callback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestDelegate1_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestDelegate1_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestDelegate1_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestDelegate1_RegisterSubclassTypeInfo(const LNZVTestDelegate1_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestDelegate1_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestDelegate2

LN_FLAT_API LNResult LNZVTestDelegate2_Create(LNZVTestDelegate2Callback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestDelegate2_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestDelegate2_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestDelegate2_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestDelegate2_RegisterSubclassTypeInfo(const LNZVTestDelegate2_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestDelegate2_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestDelegate3

LN_FLAT_API LNResult LNZVTestDelegate3_Create(LNZVTestDelegate3Callback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestDelegate3_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestDelegate3_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestDelegate3_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestDelegate3_RegisterSubclassTypeInfo(const LNZVTestDelegate3_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestDelegate3_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestEventHandler1

LN_FLAT_API LNResult LNZVTestEventHandler1_Create(LNZVTestEventHandler1Callback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestEventHandler1_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestEventHandler1_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestEventHandler1_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestEventHandler1_RegisterSubclassTypeInfo(const LNZVTestEventHandler1_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestEventHandler1_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestEventHandler2

LN_FLAT_API LNResult LNZVTestEventHandler2_Create(LNZVTestEventHandler2Callback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestEventHandler2_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestEventHandler2_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestEventHandler2_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestEventHandler2_RegisterSubclassTypeInfo(const LNZVTestEventHandler2_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestEventHandler2_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestPromise1

/**
    @brief 
    @param[in] zvtestpromise1 : instance
*/
LN_FLAT_API LNResult LNZVTestPromise1_ThenWith(LNHandle zvtestpromise1, LNHandle callback);

/**
    @brief 
    @param[in] zvtestpromise1 : instance
*/
LN_FLAT_API LNResult LNZVTestPromise1_CatchWith(LNHandle zvtestpromise1, LNHandle callback);

extern LN_FLAT_API int LNZVTestPromise1_GetTypeInfoId();
LN_FLAT_API void LNZVTestPromise1_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestPromise1_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestPromise1_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestPromise1_RegisterSubclassTypeInfo(const LNZVTestPromise1_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestPromise1_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestPromise2

/**
    @brief 
    @param[in] zvtestpromise2 : instance
*/
LN_FLAT_API LNResult LNZVTestPromise2_ThenWith(LNHandle zvtestpromise2, LNHandle callback);

/**
    @brief 
    @param[in] zvtestpromise2 : instance
*/
LN_FLAT_API LNResult LNZVTestPromise2_CatchWith(LNHandle zvtestpromise2, LNHandle callback);

extern LN_FLAT_API int LNZVTestPromise2_GetTypeInfoId();
LN_FLAT_API void LNZVTestPromise2_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestPromise2_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestPromise2_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestPromise2_RegisterSubclassTypeInfo(const LNZVTestPromise2_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestPromise2_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestClass1

/**
    @brief init method.
    @param[out] outZVTestClass1 : instance.
*/
LN_FLAT_API LNResult LNZVTestClass1_Create(LNHandle* outZVTestClass1);

/**
    @brief setTestDelegate1 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LNResult LNZVTestClass1_SetTestDelegate1(LNHandle zvtestclass1, LNHandle value);

/**
    @brief setTestDelegate2 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LNResult LNZVTestClass1_SetTestDelegate2(LNHandle zvtestclass1, LNHandle value);

/**
    @brief setTestDelegate3 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LNResult LNZVTestClass1_SetTestDelegate3(LNHandle zvtestclass1, LNHandle value);

/**
    @brief callTestDelegate1 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LNResult LNZVTestClass1_CallTestDelegate1(LNHandle zvtestclass1, int a);

/**
    @brief callTestDelegate2 method.
    @param[in] zvtestclass1 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNZVTestClass1_CallTestDelegate2(LNHandle zvtestclass1, int a, int b, int* outReturn);

/**
    @brief callTestDelegate3 method. (create object in internal)
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LNResult LNZVTestClass1_CallTestDelegate3(LNHandle zvtestclass1);

/**
    @brief Promise test method. (static)
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNZVTestClass1_LoadAsync(const LNChar* filePath, LNHandle* outReturn);
LN_FLAT_API LNResult LNZVTestClass1_LoadAsyncA(const char* filePath, LNHandle* outReturn);

/**
    @brief Promise test method. (instance)
    @param[in] zvtestclass1 : instance
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNZVTestClass1_ExecuteAsync(LNHandle zvtestclass1, LNHandle* outReturn);

/**
    @brief Promise test method.
    @param[in] zvtestclass1 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNZVTestClass1_GetFilePath(LNHandle zvtestclass1, const LNChar** outReturn);
LN_FLAT_API LNResult LNZVTestClass1_GetFilePathA(LNHandle zvtestclass1, const char** outReturn);

/**
    @brief connectOnEvent1 method.
    @param[in] zvtestclass1 : instance
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNZVTestClass1_ConnectOnEvent1(LNHandle zvtestclass1, LNHandle handler, LNHandle* outReturn);

/**
    @brief raiseEvent1 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LNResult LNZVTestClass1_RaiseEvent1(LNHandle zvtestclass1);

/**
    @brief connectOnEvent2 method.
    @param[in] zvtestclass1 : instance
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNZVTestClass1_ConnectOnEvent2(LNHandle zvtestclass1, LNHandle handler, LNHandle* outReturn);

/**
    @brief raiseEvent2 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LNResult LNZVTestClass1_RaiseEvent2(LNHandle zvtestclass1);

typedef LNResult(*LNZVTestClass1_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNZVTestClass1_OnSerialize_SetOverrideCallback(LNZVTestClass1_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNZVTestClass1_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LNResult LNZVTestClass1_SetPrototype_OnSerialize(LNHandle zvtestclass1, LNHandle callback);

extern LN_FLAT_API int LNZVTestClass1_GetTypeInfoId();
LN_FLAT_API void LNZVTestClass1_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestClass1_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNZVTestClass1_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNZVTestClass1_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestClass1_RegisterSubclassTypeInfo(const LNZVTestClass1_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestClass1_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ZVTestEventArgs1

/**
    @brief init method.
    @param[out] outZVTestEventArgs1 : instance.
*/
LN_FLAT_API LNResult LNZVTestEventArgs1_Create(LNHandle* outZVTestEventArgs1);

/**
    @brief init method.
    @param[out] outZVTestEventArgs1 : instance.
*/
LN_FLAT_API LNResult LNZVTestEventArgs1_CreateWithValue(int v, LNHandle* outZVTestEventArgs1);

/**
    @brief value method.
    @param[in] zvtesteventargs1 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNZVTestEventArgs1_GetValue(LNHandle zvtesteventargs1, int* outReturn);

typedef LNResult(*LNZVTestEventArgs1_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNZVTestEventArgs1_OnSerialize_SetOverrideCallback(LNZVTestEventArgs1_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNZVTestEventArgs1_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] zvtesteventargs1 : instance
*/
LN_FLAT_API LNResult LNZVTestEventArgs1_SetPrototype_OnSerialize(LNHandle zvtesteventargs1, LNHandle callback);

extern LN_FLAT_API int LNZVTestEventArgs1_GetTypeInfoId();
LN_FLAT_API void LNZVTestEventArgs1_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestEventArgs1_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNZVTestEventArgs1_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNZVTestEventArgs1_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestEventArgs1_RegisterSubclassTypeInfo(const LNZVTestEventArgs1_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestEventArgs1_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Serializer2

typedef LNResult(*LNSerializer2_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNSerializer2_OnSerialize_SetOverrideCallback(LNSerializer2_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNSerializer2_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] serializer2 : instance
*/
LN_FLAT_API LNResult LNSerializer2_SetPrototype_OnSerialize(LNHandle serializer2, LNHandle callback);

extern LN_FLAT_API int LNSerializer2_GetTypeInfoId();
LN_FLAT_API void LNSerializer2_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSerializer2_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNSerializer2_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNSerializer2_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSerializer2_RegisterSubclassTypeInfo(const LNSerializer2_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSerializer2_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::AssetModel

/**
    @brief init
    @param[out] outAssetModel : instance.
*/
LN_FLAT_API LNResult LNAssetModel_Create(LNHandle target, LNHandle* outAssetModel);

/**
    @brief target
    @param[in] assetmodel : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNAssetModel_Target(LNHandle assetmodel, LNHandle* outReturn);

typedef LNResult(*LNAssetModel_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNAssetModel_OnSerialize_SetOverrideCallback(LNAssetModel_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNAssetModel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] assetmodel : instance
*/
LN_FLAT_API LNResult LNAssetModel_SetPrototype_OnSerialize(LNHandle assetmodel, LNHandle callback);

extern LN_FLAT_API int LNAssetModel_GetTypeInfoId();
LN_FLAT_API void LNAssetModel_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAssetModel_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNAssetModel_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNAssetModel_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAssetModel_RegisterSubclassTypeInfo(const LNAssetModel_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAssetModel_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Assets

/**
    @brief Internal
*/
LN_FLAT_API LNResult LNAssets_SaveAssetToLocalFile(LNHandle asset, const LNChar* filePath);
LN_FLAT_API LNResult LNAssets_SaveAssetToLocalFileA(LNHandle asset, const char* filePath);

/**
    @brief Internal
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNAssets_LoadAssetFromLocalFile(const LNChar* filePath, LNHandle* outReturn);
LN_FLAT_API LNResult LNAssets_LoadAssetFromLocalFileA(const char* filePath, LNHandle* outReturn);

/**
    @brief 指定したアセットファイルを読み込み、オブジェクト生成します。
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
    @details ファイルの拡張子は .lnasset です。ただし、filePath に指定する値は拡張子を省略可能です。
*/
LN_FLAT_API LNResult LNAssets_LoadAsset(const LNChar* filePath, LNHandle* outReturn);
LN_FLAT_API LNResult LNAssets_LoadAssetA(const char* filePath, LNHandle* outReturn);

/**
    @brief 指定したアセットファイルを読み込み、作成済みのオブジェクトへ適用します。
    @details このメソッドは Lumino の型システムを使用しないオブジェクトの読み込みに使用します。
*/
LN_FLAT_API LNResult LNAssets_ReloadAsset(const LNChar* filePath, LNHandle obj);
LN_FLAT_API LNResult LNAssets_ReloadAssetA(const char* filePath, LNHandle obj);


//==============================================================================
// ln::Texture2DDelegate

LN_FLAT_API LNResult LNTexture2DDelegate_Create(LNTexture2DDelegateCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNTexture2DDelegate_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTexture2DDelegate_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNTexture2DDelegate_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTexture2DDelegate_RegisterSubclassTypeInfo(const LNTexture2DDelegate_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTexture2DDelegate_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Texture2DPromise

/**
    @brief 
    @param[in] texture2dpromise : instance
*/
LN_FLAT_API LNResult LNTexture2DPromise_ThenWith(LNHandle texture2dpromise, LNHandle callback);

/**
    @brief 
    @param[in] texture2dpromise : instance
*/
LN_FLAT_API LNResult LNTexture2DPromise_CatchWith(LNHandle texture2dpromise, LNHandle callback);

extern LN_FLAT_API int LNTexture2DPromise_GetTypeInfoId();
LN_FLAT_API void LNTexture2DPromise_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTexture2DPromise_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNTexture2DPromise_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTexture2DPromise_RegisterSubclassTypeInfo(const LNTexture2DPromise_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTexture2DPromise_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Texture

typedef LNResult(*LNTexture_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNTexture_OnSerialize_SetOverrideCallback(LNTexture_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNTexture_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] texture : instance
*/
LN_FLAT_API LNResult LNTexture_SetPrototype_OnSerialize(LNHandle texture, LNHandle callback);

extern LN_FLAT_API int LNTexture_GetTypeInfoId();
LN_FLAT_API void LNTexture_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTexture_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNTexture_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNTexture_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTexture_RegisterSubclassTypeInfo(const LNTexture_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTexture_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Texture2D

/**
    @brief テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    @param[in] width : 幅 (px 単位)
    @param[in] height : 高さ (px 単位)
    @param[out] outTexture2D : instance.
    @return 作成されたテクスチャ
*/
LN_FLAT_API LNResult LNTexture2D_Create(int width, int height, LNHandle* outTexture2D);

/**
    @brief テクスチャを作成します。
    @param[in] width : 幅 (px 単位)
    @param[in] height : 高さ (px 単位)
    @param[in] format : ピクセルフォーマット
    @param[out] outTexture2D : instance.
*/
LN_FLAT_API LNResult LNTexture2D_CreateWithFormat(int width, int height, LNTextureFormat format, LNHandle* outTexture2D);

/**
    @brief アセットからテクスチャを読み込みます。
    @param[in] filePath : 読み込むファイルのパス
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
    @details サポートしているフォーマットは次の通りです。PNG(.png), JPG(.jpg), TGA(.tga), BMP(.bmp), GIF(.gif)
*/
LN_FLAT_API LNResult LNTexture2D_Load(const LNChar* filePath, LNHandle* outReturn);
LN_FLAT_API LNResult LNTexture2D_LoadA(const char* filePath, LNHandle* outReturn);

/**
    @brief loadEmoji
    @param[in] code : xxxx
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LNResult LNTexture2D_LoadEmoji(const LNChar* code, LNHandle* outReturn);
LN_FLAT_API LNResult LNTexture2D_LoadEmojiA(const char* code, LNHandle* outReturn);

typedef LNResult(*LNTexture2D_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNTexture2D_OnSerialize_SetOverrideCallback(LNTexture2D_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNTexture2D_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] texture2d : instance
*/
LN_FLAT_API LNResult LNTexture2D_SetPrototype_OnSerialize(LNHandle texture2d, LNHandle callback);

extern LN_FLAT_API int LNTexture2D_GetTypeInfoId();
LN_FLAT_API void LNTexture2D_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTexture2D_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNTexture2D_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNTexture2D_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTexture2D_RegisterSubclassTypeInfo(const LNTexture2D_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTexture2D_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::RenderView

typedef LNResult(*LNRenderView_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNRenderView_OnSerialize_SetOverrideCallback(LNRenderView_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNRenderView_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] renderview : instance
*/
LN_FLAT_API LNResult LNRenderView_SetPrototype_OnSerialize(LNHandle renderview, LNHandle callback);

extern LN_FLAT_API int LNRenderView_GetTypeInfoId();
LN_FLAT_API void LNRenderView_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRenderView_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNRenderView_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNRenderView_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRenderView_RegisterSubclassTypeInfo(const LNRenderView_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRenderView_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Component

typedef LNResult(*LNComponent_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNComponent_OnSerialize_SetOverrideCallback(LNComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] component : instance
*/
LN_FLAT_API LNResult LNComponent_SetPrototype_OnSerialize(LNHandle component, LNHandle callback);

extern LN_FLAT_API int LNComponent_GetTypeInfoId();
LN_FLAT_API void LNComponent_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNComponent_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNComponent_SubclassRegistrationInfo;

extern LN_FLAT_API void LNComponent_RegisterSubclassTypeInfo(const LNComponent_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNComponent_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::VisualComponent

/**
    @brief 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    @param[in] visualcomponent : instance
*/
LN_FLAT_API LNResult LNVisualComponent_SetVisible(LNHandle visualcomponent, LNBool value);

/**
    @brief 可視状態を取得します。
    @param[in] visualcomponent : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNVisualComponent_IsVisible(LNHandle visualcomponent, LNBool* outReturn);

typedef LNResult(*LNVisualComponent_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNVisualComponent_OnSerialize_SetOverrideCallback(LNVisualComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNVisualComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] visualcomponent : instance
*/
LN_FLAT_API LNResult LNVisualComponent_SetPrototype_OnSerialize(LNHandle visualcomponent, LNHandle callback);

extern LN_FLAT_API int LNVisualComponent_GetTypeInfoId();
LN_FLAT_API void LNVisualComponent_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVisualComponent_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNVisualComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNVisualComponent_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVisualComponent_RegisterSubclassTypeInfo(const LNVisualComponent_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVisualComponent_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::SpriteComponent

/**
    @brief スプライトが表示するテクスチャを設定します。
    @param[in] spritecomponent : instance
*/
LN_FLAT_API LNResult LNSpriteComponent_SetTexture(LNHandle spritecomponent, LNHandle texture);

typedef LNResult(*LNSpriteComponent_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNSpriteComponent_OnSerialize_SetOverrideCallback(LNSpriteComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNSpriteComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] spritecomponent : instance
*/
LN_FLAT_API LNResult LNSpriteComponent_SetPrototype_OnSerialize(LNHandle spritecomponent, LNHandle callback);

extern LN_FLAT_API int LNSpriteComponent_GetTypeInfoId();
LN_FLAT_API void LNSpriteComponent_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpriteComponent_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNSpriteComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNSpriteComponent_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpriteComponent_RegisterSubclassTypeInfo(const LNSpriteComponent_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpriteComponent_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::World

/**
    @brief オブジェクトを World に追加します。
    @param[in] world : instance
*/
LN_FLAT_API LNResult LNWorld_Add(LNHandle world, LNHandle obj);

typedef LNResult(*LNWorld_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNWorld_OnSerialize_SetOverrideCallback(LNWorld_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNWorld_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] world : instance
*/
LN_FLAT_API LNResult LNWorld_SetPrototype_OnSerialize(LNHandle world, LNHandle callback);

extern LN_FLAT_API int LNWorld_GetTypeInfoId();
LN_FLAT_API void LNWorld_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorld_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNWorld_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNWorld_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorld_RegisterSubclassTypeInfo(const LNWorld_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorld_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::ComponentList

/**
    @brief 
    @param[in] componentlist : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNComponentList_GetLength(LNHandle componentlist, int* outReturn);

/**
    @brief 
    @param[in] componentlist : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNComponentList_GetItem(LNHandle componentlist, int index, LNHandle* outReturn);

typedef LNResult(*LNComponentList_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNComponentList_OnSerialize_SetOverrideCallback(LNComponentList_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNComponentList_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] componentlist : instance
*/
LN_FLAT_API LNResult LNComponentList_SetPrototype_OnSerialize(LNHandle componentlist, LNHandle callback);

extern LN_FLAT_API int LNComponentList_GetTypeInfoId();
LN_FLAT_API void LNComponentList_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNComponentList_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNComponentList_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNComponentList_SubclassRegistrationInfo;

extern LN_FLAT_API void LNComponentList_RegisterSubclassTypeInfo(const LNComponentList_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNComponentList_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::WorldObject

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_SetPosition(LNHandle worldobject, const LNVector3* pos);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_SetPositionXYZ(LNHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの位置を位置を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNWorldObject_GetPosition(LNHandle worldobject, LNVector3* outReturn);

/**
    @brief このオブジェクトの回転を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_SetRotationQuaternion(LNHandle worldobject, const LNQuaternion* rot);

/**
    @brief このオブジェクトの回転をオイラー角から設定します(radian) 。回転順序は Z(Roll) > X(Pich) > Y(Yaw) です。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_SetRotation(LNHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの回転を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNWorldObject_GetRotation(LNHandle worldobject, LNQuaternion* outReturn);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_SetScale(LNHandle worldobject, const LNVector3* scale);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_SetScaleS(LNHandle worldobject, float xyz);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_SetScaleXYZ(LNHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの拡大率を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNWorldObject_GetScale(LNHandle worldobject, LNVector3* outReturn);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_SetCenterPoint(LNHandle worldobject, const LNVector3* value);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_SetCenterPointXYZ(LNHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトのローカルの中心位置を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNWorldObject_GetCenterPoint(LNHandle worldobject, LNVector3* outReturn);

/**
    @brief 指定した座標を向くように、オブジェクトを回転させます。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_LookAt(LNHandle worldobject, const LNVector3* target);

/**
    @brief 指定した座標を向くように、オブジェクトを回転させます。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_LookAtXYZ(LNHandle worldobject, float x, float y, float z);

/**
    @brief Component を追加します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_AddComponent(LNHandle worldobject, LNHandle component);

/**
    @brief Component を除外します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_RemoveComponent(LNHandle worldobject, LNHandle component);

/**
    @brief 
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNWorldObject_GetComponents(LNHandle worldobject, LNHandle* outReturn);

typedef LNResult(*LNWorldObject_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNWorldObject_OnSerialize_SetOverrideCallback(LNWorldObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNWorldObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNWorldObject_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNWorldObject_OnUpdate_SetOverrideCallback(LNWorldObject_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNWorldObject_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_SetPrototype_OnSerialize(LNHandle worldobject, LNHandle callback);

/**
    @brief 
    @param[in] worldobject : instance
*/
LN_FLAT_API LNResult LNWorldObject_SetPrototype_OnUpdate(LNHandle worldobject, LNHandle callback);

extern LN_FLAT_API int LNWorldObject_GetTypeInfoId();
LN_FLAT_API void LNWorldObject_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldObject_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNWorldObject_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNWorldObject_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNWorldObject_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldObject_RegisterSubclassTypeInfo(const LNWorldObject_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldObject_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::VisualObject

/**
    @brief 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    @param[in] visualobject : instance
*/
LN_FLAT_API LNResult LNVisualObject_SetVisible(LNHandle visualobject, LNBool value);

/**
    @brief 可視状態を取得します。
    @param[in] visualobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNVisualObject_IsVisible(LNHandle visualobject, LNBool* outReturn);

typedef LNResult(*LNVisualObject_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNVisualObject_OnSerialize_SetOverrideCallback(LNVisualObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNVisualObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNVisualObject_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNVisualObject_OnUpdate_SetOverrideCallback(LNVisualObject_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNVisualObject_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] visualobject : instance
*/
LN_FLAT_API LNResult LNVisualObject_SetPrototype_OnSerialize(LNHandle visualobject, LNHandle callback);

/**
    @brief 
    @param[in] visualobject : instance
*/
LN_FLAT_API LNResult LNVisualObject_SetPrototype_OnUpdate(LNHandle visualobject, LNHandle callback);

extern LN_FLAT_API int LNVisualObject_GetTypeInfoId();
LN_FLAT_API void LNVisualObject_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVisualObject_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNVisualObject_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNVisualObject_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNVisualObject_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVisualObject_RegisterSubclassTypeInfo(const LNVisualObject_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVisualObject_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Camera

typedef LNResult(*LNCamera_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNCamera_OnSerialize_SetOverrideCallback(LNCamera_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNCamera_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNCamera_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNCamera_OnUpdate_SetOverrideCallback(LNCamera_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNCamera_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] camera : instance
*/
LN_FLAT_API LNResult LNCamera_SetPrototype_OnSerialize(LNHandle camera, LNHandle callback);

/**
    @brief 
    @param[in] camera : instance
*/
LN_FLAT_API LNResult LNCamera_SetPrototype_OnUpdate(LNHandle camera, LNHandle callback);

extern LN_FLAT_API int LNCamera_GetTypeInfoId();
LN_FLAT_API void LNCamera_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCamera_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNCamera_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNCamera_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNCamera_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCamera_RegisterSubclassTypeInfo(const LNCamera_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCamera_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::EnvironmentLight

/**
    @brief ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    @param[in] environmentlight : instance
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetEnabled(LNHandle environmentlight, LNBool enabled);

/**
    @brief ライトの有効状態を取得します。
    @param[in] environmentlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEnvironmentLight_IsEnabled(LNHandle environmentlight, LNBool* outReturn);

/**
    @brief ディレクショナルライトの光源色を設定します。(default: White)
    @param[in] environmentlight : instance
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetColor(LNHandle environmentlight, const LNColor* value);

/**
    @brief ディレクショナルライトの光源色を取得します。
    @param[in] environmentlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetColor(LNHandle environmentlight, LNColor* outReturn);

/**
    @brief シーン全体の環境光の色を設定します。(default: White)
    @param[in] environmentlight : instance
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetAmbientColor(LNHandle environmentlight, const LNColor* value);

/**
    @brief シーン全体の環境光の色を取得します。
    @param[in] environmentlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetAmbientColor(LNHandle environmentlight, LNColor* outReturn);

/**
    @brief 空の環境光の色を取得します。
    @param[in] environmentlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetSkyColor(LNHandle environmentlight, LNColor* outReturn);

/**
    @brief 空の環境光の色を設定します。(default: Black)
    @param[in] environmentlight : instance
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetSkyColor(LNHandle environmentlight, const LNColor* value);

/**
    @brief 地面の環境光の色を取得します。
    @param[in] environmentlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetGroundColor(LNHandle environmentlight, LNColor* outReturn);

/**
    @brief 地面の環境光の色を設定します。(default: Black)
    @param[in] environmentlight : instance
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetGroundColor(LNHandle environmentlight, const LNColor* value);

/**
    @brief ライトの明るさを設定します。(default: 0.5)
    @param[in] environmentlight : instance
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetIntensity(LNHandle environmentlight, float intensity);

/**
    @brief ライトの明るさを取得します。
    @param[in] environmentlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetIntensity(LNHandle environmentlight, float* outReturn);

/**
    @brief 視点からの、影を生成できる距離を指定します。 (default: 0.0f)
    @param[in] environmentlight : instance
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetShadowEffectiveDistance(LNHandle environmentlight, float value);

/**
    @brief 視点からの、影を生成できる距離を取得します。
    @param[in] environmentlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetShadowEffectiveDistance(LNHandle environmentlight, float* outReturn);

/**
    @brief 光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。
    @param[in] environmentlight : instance
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetShadowEffectiveDepth(LNHandle environmentlight, float value);

/**
    @brief 光源方向からの、影を生成できる距離を指定します。
    @param[in] environmentlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEnvironmentLight_GetShadowEffectiveDepth(LNHandle environmentlight, float* outReturn);

typedef LNResult(*LNEnvironmentLight_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNEnvironmentLight_OnSerialize_SetOverrideCallback(LNEnvironmentLight_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNEnvironmentLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNEnvironmentLight_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNEnvironmentLight_OnUpdate_SetOverrideCallback(LNEnvironmentLight_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNEnvironmentLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] environmentlight : instance
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetPrototype_OnSerialize(LNHandle environmentlight, LNHandle callback);

/**
    @brief 
    @param[in] environmentlight : instance
*/
LN_FLAT_API LNResult LNEnvironmentLight_SetPrototype_OnUpdate(LNHandle environmentlight, LNHandle callback);

extern LN_FLAT_API int LNEnvironmentLight_GetTypeInfoId();
LN_FLAT_API void LNEnvironmentLight_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEnvironmentLight_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNEnvironmentLight_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNEnvironmentLight_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNEnvironmentLight_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEnvironmentLight_RegisterSubclassTypeInfo(const LNEnvironmentLight_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEnvironmentLight_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::DirectionalLight

/**
    @brief 既定の設定でディレクショナルライトを作成します。
    @param[out] outDirectionalLight : instance.
*/
LN_FLAT_API LNResult LNDirectionalLight_Create(LNHandle* outDirectionalLight);

/**
    @brief 色を指定してディレクショナルライトを作成します。
    @param[out] outDirectionalLight : instance.
*/
LN_FLAT_API LNResult LNDirectionalLight_CreateWithColor(const LNColor* color, LNHandle* outDirectionalLight);

/**
    @brief ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    @param[in] directionallight : instance
*/
LN_FLAT_API LNResult LNDirectionalLight_SetEnabled(LNHandle directionallight, LNBool enabled);

/**
    @brief ライトの有効状態を取得します。
    @param[in] directionallight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNDirectionalLight_IsEnabled(LNHandle directionallight, LNBool* outReturn);

/**
    @brief ライトカラーを設定します。(default: White)
    @param[in] directionallight : instance
*/
LN_FLAT_API LNResult LNDirectionalLight_SetColor(LNHandle directionallight, const LNColor* color);

/**
    @brief ライトカラーを取得します。
    @param[in] directionallight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNDirectionalLight_GetColor(LNHandle directionallight, LNColor* outReturn);

/**
    @brief ライトの明るさを設定します。(default: 0.5)
    @param[in] directionallight : instance
*/
LN_FLAT_API LNResult LNDirectionalLight_SetIntensity(LNHandle directionallight, float intensity);

/**
    @brief ライトの明るさを取得します。
    @param[in] directionallight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNDirectionalLight_GetIntensity(LNHandle directionallight, float* outReturn);

/**
    @brief 視点からの、影を生成できる距離を指定します。 (default: 0.0f)
    @param[in] directionallight : instance
*/
LN_FLAT_API LNResult LNDirectionalLight_SetShadowEffectiveDistance(LNHandle directionallight, float value);

/**
    @brief 視点からの、影を生成できる距離を取得します。
    @param[in] directionallight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNDirectionalLight_GetShadowEffectiveDistance(LNHandle directionallight, float* outReturn);

/**
    @brief 光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。
    @param[in] directionallight : instance
*/
LN_FLAT_API LNResult LNDirectionalLight_SetShadowEffectiveDepth(LNHandle directionallight, float value);

/**
    @brief 光源方向からの、影を生成できる距離を指定します。
    @param[in] directionallight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNDirectionalLight_GetShadowEffectiveDepth(LNHandle directionallight, float* outReturn);

typedef LNResult(*LNDirectionalLight_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNDirectionalLight_OnSerialize_SetOverrideCallback(LNDirectionalLight_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNDirectionalLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNDirectionalLight_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNDirectionalLight_OnUpdate_SetOverrideCallback(LNDirectionalLight_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNDirectionalLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] directionallight : instance
*/
LN_FLAT_API LNResult LNDirectionalLight_SetPrototype_OnSerialize(LNHandle directionallight, LNHandle callback);

/**
    @brief 
    @param[in] directionallight : instance
*/
LN_FLAT_API LNResult LNDirectionalLight_SetPrototype_OnUpdate(LNHandle directionallight, LNHandle callback);

extern LN_FLAT_API int LNDirectionalLight_GetTypeInfoId();
LN_FLAT_API void LNDirectionalLight_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNDirectionalLight_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNDirectionalLight_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNDirectionalLight_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNDirectionalLight_SubclassRegistrationInfo;

extern LN_FLAT_API void LNDirectionalLight_RegisterSubclassTypeInfo(const LNDirectionalLight_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNDirectionalLight_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::PointLight

/**
    @brief 既定の設定でポイントライトを作成します。
    @param[out] outPointLight : instance.
*/
LN_FLAT_API LNResult LNPointLight_Create(LNHandle* outPointLight);

/**
    @brief 色と範囲を指定してポイントライトを作成します。
    @param[out] outPointLight : instance.
*/
LN_FLAT_API LNResult LNPointLight_CreateWithColorAndRange(const LNColor* color, float range, LNHandle* outPointLight);

/**
    @brief ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    @param[in] pointlight : instance
*/
LN_FLAT_API LNResult LNPointLight_SetEnabled(LNHandle pointlight, LNBool enabled);

/**
    @brief ライトの有効状態を取得します。
    @param[in] pointlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNPointLight_IsEnabled(LNHandle pointlight, LNBool* outReturn);

/**
    @brief ライトカラーを設定します。(default: White)
    @param[in] pointlight : instance
*/
LN_FLAT_API LNResult LNPointLight_SetColor(LNHandle pointlight, const LNColor* color);

/**
    @brief ライトカラーを取得します。
    @param[in] pointlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNPointLight_GetColor(LNHandle pointlight, LNColor* outReturn);

/**
    @brief ライトの明るさを設定します。(default: 1.0)
    @param[in] pointlight : instance
*/
LN_FLAT_API LNResult LNPointLight_SetIntensity(LNHandle pointlight, float intensity);

/**
    @brief ライトの明るさを取得します。
    @param[in] pointlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNPointLight_GetIntensity(LNHandle pointlight, float* outReturn);

/**
    @brief ライトの影響範囲を設定します。(default: 10.0)
    @param[in] pointlight : instance
*/
LN_FLAT_API LNResult LNPointLight_SetRange(LNHandle pointlight, float range);

/**
    @brief ライトの影響範囲を取得します。
    @param[in] pointlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNPointLight_GetRange(LNHandle pointlight, float* outReturn);

/**
    @brief ライトの減衰を設定します。(default: 1.0)
    @param[in] pointlight : instance
*/
LN_FLAT_API LNResult LNPointLight_SetAttenuation(LNHandle pointlight, float attenuation);

/**
    @brief ライトの減衰を取得します。
    @param[in] pointlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNPointLight_GetAttenuation(LNHandle pointlight, float* outReturn);

typedef LNResult(*LNPointLight_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNPointLight_OnSerialize_SetOverrideCallback(LNPointLight_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNPointLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNPointLight_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNPointLight_OnUpdate_SetOverrideCallback(LNPointLight_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNPointLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] pointlight : instance
*/
LN_FLAT_API LNResult LNPointLight_SetPrototype_OnSerialize(LNHandle pointlight, LNHandle callback);

/**
    @brief 
    @param[in] pointlight : instance
*/
LN_FLAT_API LNResult LNPointLight_SetPrototype_OnUpdate(LNHandle pointlight, LNHandle callback);

extern LN_FLAT_API int LNPointLight_GetTypeInfoId();
LN_FLAT_API void LNPointLight_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPointLight_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNPointLight_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNPointLight_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNPointLight_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPointLight_RegisterSubclassTypeInfo(const LNPointLight_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPointLight_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::SpotLight

/**
    @brief 既定の設定でスポットライトを作成します。
    @param[out] outSpotLight : instance.
*/
LN_FLAT_API LNResult LNSpotLight_Create(LNHandle* outSpotLight);

/**
    @brief 色と範囲を指定してスポットライトを作成します。
    @param[out] outSpotLight : instance.
*/
LN_FLAT_API LNResult LNSpotLight_CreateWithColorAndRange(const LNColor* color, float range, float angle, LNHandle* outSpotLight);

/**
    @brief ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    @param[in] spotlight : instance
*/
LN_FLAT_API LNResult LNSpotLight_SetEnabled(LNHandle spotlight, LNBool enabled);

/**
    @brief ライトの有効状態を取得します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNSpotLight_IsEnabled(LNHandle spotlight, LNBool* outReturn);

/**
    @brief ライトカラーを設定します。(default: White)
    @param[in] spotlight : instance
*/
LN_FLAT_API LNResult LNSpotLight_SetColor(LNHandle spotlight, const LNColor* color);

/**
    @brief ライトカラーを取得します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNSpotLight_GetColor(LNHandle spotlight, LNColor* outReturn);

/**
    @brief ライトの明るさを設定します。(default: 1.0)
    @param[in] spotlight : instance
*/
LN_FLAT_API LNResult LNSpotLight_SetIntensity(LNHandle spotlight, float intensity);

/**
    @brief ライトの明るさを取得します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNSpotLight_GetIntensity(LNHandle spotlight, float* outReturn);

/**
    @brief ライトの影響範囲を設定します。(default: 10.0)
    @param[in] spotlight : instance
*/
LN_FLAT_API LNResult LNSpotLight_SetRange(LNHandle spotlight, float range);

/**
    @brief ライトの影響範囲を取得します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNSpotLight_GetRange(LNHandle spotlight, float* outReturn);

/**
    @brief ライトの減衰を設定します。(default: 1.0)
    @param[in] spotlight : instance
*/
LN_FLAT_API LNResult LNSpotLight_SetAttenuation(LNHandle spotlight, float attenuation);

/**
    @brief ライトの減衰を取得します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNSpotLight_GetAttenuation(LNHandle spotlight, float* outReturn);

/**
    @brief スポットライトのコーン角度を設定します。(ラジアン単位、default: PI / 3)
    @param[in] spotlight : instance
*/
LN_FLAT_API LNResult LNSpotLight_SetAngle(LNHandle spotlight, float angle);

/**
    @brief スポットライトのコーン角度を取得します。(ラジアン単位)
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNSpotLight_GetAngle(LNHandle spotlight, float* outReturn);

/**
    @brief スポットライトのコーン角度に対する減衰率を設定します。(0..1, default: 0)
    @param[in] spotlight : instance
*/
LN_FLAT_API LNResult LNSpotLight_SetPenumbra(LNHandle spotlight, float penumbra);

/**
    @brief スポットライトのコーン角度に対する減衰率を設定します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNSpotLight_GetPenumbra(LNHandle spotlight, float* outReturn);

typedef LNResult(*LNSpotLight_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNSpotLight_OnSerialize_SetOverrideCallback(LNSpotLight_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNSpotLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNSpotLight_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNSpotLight_OnUpdate_SetOverrideCallback(LNSpotLight_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNSpotLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] spotlight : instance
*/
LN_FLAT_API LNResult LNSpotLight_SetPrototype_OnSerialize(LNHandle spotlight, LNHandle callback);

/**
    @brief 
    @param[in] spotlight : instance
*/
LN_FLAT_API LNResult LNSpotLight_SetPrototype_OnUpdate(LNHandle spotlight, LNHandle callback);

extern LN_FLAT_API int LNSpotLight_GetTypeInfoId();
LN_FLAT_API void LNSpotLight_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpotLight_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNSpotLight_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNSpotLight_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNSpotLight_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpotLight_RegisterSubclassTypeInfo(const LNSpotLight_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpotLight_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::TestDelegate

LN_FLAT_API LNResult LNTestDelegate_Create(LNTestDelegateCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNTestDelegate_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTestDelegate_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNTestDelegate_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTestDelegate_RegisterSubclassTypeInfo(const LNTestDelegate_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTestDelegate_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Sprite

/**
    @brief init
    @param[out] outSprite : instance.
*/
LN_FLAT_API LNResult LNSprite_Create(LNHandle* outSprite);

/**
    @brief init
    @param[out] outSprite : instance.
*/
LN_FLAT_API LNResult LNSprite_CreateWithTexture(LNHandle texture, LNHandle* outSprite);

/**
    @brief init
    @param[out] outSprite : instance.
*/
LN_FLAT_API LNResult LNSprite_CreateWithTextureAndSize(LNHandle texture, float width, float height, LNHandle* outSprite);

/**
    @brief スプライトが表示するテクスチャを設定します。
    @param[in] sprite : instance
*/
LN_FLAT_API LNResult LNSprite_SetTexture(LNHandle sprite, LNHandle value);

/**
    @brief スプライトの大きさを設定します。
    @param[in] sprite : instance
*/
LN_FLAT_API LNResult LNSprite_SetSize(LNHandle sprite, const LNSize* value);

/**
    @brief スプライトの大きさを設定します。
    @param[in] sprite : instance
*/
LN_FLAT_API LNResult LNSprite_SetSizeWH(LNHandle sprite, float width, float height);

/**
    @brief 
    @param[in] sprite : instance
*/
LN_FLAT_API LNResult LNSprite_SetSourceRectXYWH(LNHandle sprite, float x, float y, float width, float height);

/**
    @brief test
    @param[in] sprite : instance
*/
LN_FLAT_API LNResult LNSprite_SetCallerTest(LNHandle sprite, LNHandle callback);

typedef LNResult(*LNSprite_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNSprite_OnSerialize_SetOverrideCallback(LNSprite_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNSprite_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNSprite_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNSprite_OnUpdate_SetOverrideCallback(LNSprite_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNSprite_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] sprite : instance
*/
LN_FLAT_API LNResult LNSprite_SetPrototype_OnSerialize(LNHandle sprite, LNHandle callback);

/**
    @brief 
    @param[in] sprite : instance
*/
LN_FLAT_API LNResult LNSprite_SetPrototype_OnUpdate(LNHandle sprite, LNHandle callback);

extern LN_FLAT_API int LNSprite_GetTypeInfoId();
LN_FLAT_API void LNSprite_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSprite_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNSprite_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNSprite_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNSprite_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSprite_RegisterSubclassTypeInfo(const LNSprite_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSprite_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::CameraOrbitControlComponent

/**
    @brief CameraOrbitControlComponent を作成します。
    @param[out] outCameraOrbitControlComponent : instance.
*/
LN_FLAT_API LNResult LNCameraOrbitControlComponent_Create(LNHandle* outCameraOrbitControlComponent);

typedef LNResult(*LNCameraOrbitControlComponent_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNCameraOrbitControlComponent_OnSerialize_SetOverrideCallback(LNCameraOrbitControlComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNCameraOrbitControlComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] cameraorbitcontrolcomponent : instance
*/
LN_FLAT_API LNResult LNCameraOrbitControlComponent_SetPrototype_OnSerialize(LNHandle cameraorbitcontrolcomponent, LNHandle callback);

extern LN_FLAT_API int LNCameraOrbitControlComponent_GetTypeInfoId();
LN_FLAT_API void LNCameraOrbitControlComponent_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCameraOrbitControlComponent_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNCameraOrbitControlComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNCameraOrbitControlComponent_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCameraOrbitControlComponent_RegisterSubclassTypeInfo(const LNCameraOrbitControlComponent_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCameraOrbitControlComponent_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Raycaster

/**
    @brief メインのカメラを使用して、指定したスクリーン座標から正面に向かうレイを定義した Raycaster を取得します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNRaycaster_FromScreen(const LNPoint* point, LNHandle* outReturn);

/**
    @brief 指定した向きの平面との交差判定を行います。
    @param[in] raycaster : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNRaycaster_IntersectPlane(LNHandle raycaster, float normalX, float normalY, float normalZ, LNHandle* outReturn);

typedef LNResult(*LNRaycaster_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNRaycaster_OnSerialize_SetOverrideCallback(LNRaycaster_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNRaycaster_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] raycaster : instance
*/
LN_FLAT_API LNResult LNRaycaster_SetPrototype_OnSerialize(LNHandle raycaster, LNHandle callback);

extern LN_FLAT_API int LNRaycaster_GetTypeInfoId();
LN_FLAT_API void LNRaycaster_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRaycaster_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNRaycaster_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNRaycaster_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRaycaster_RegisterSubclassTypeInfo(const LNRaycaster_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRaycaster_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::RaycastResult

/**
    @brief ワールド座標上の交差点
    @param[in] raycastresult : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNRaycastResult_GetPoint(LNHandle raycastresult, LNVector3* outReturn);

typedef LNResult(*LNRaycastResult_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNRaycastResult_OnSerialize_SetOverrideCallback(LNRaycastResult_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNRaycastResult_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] raycastresult : instance
*/
LN_FLAT_API LNResult LNRaycastResult_SetPrototype_OnSerialize(LNHandle raycastresult, LNHandle callback);

extern LN_FLAT_API int LNRaycastResult_GetTypeInfoId();
LN_FLAT_API void LNRaycastResult_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRaycastResult_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNRaycastResult_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNRaycastResult_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRaycastResult_RegisterSubclassTypeInfo(const LNRaycastResult_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRaycastResult_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::WorldRenderView

/**
    @brief この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを設定します。
    @param[in] worldrenderview : instance
*/
LN_FLAT_API LNResult LNWorldRenderView_SetGuideGridEnabled(LNHandle worldrenderview, LNBool value);

/**
    @brief この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを取得します。
    @param[in] worldrenderview : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNWorldRenderView_GetGuideGridEnabled(LNHandle worldrenderview, LNBool* outReturn);

typedef LNResult(*LNWorldRenderView_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNWorldRenderView_OnSerialize_SetOverrideCallback(LNWorldRenderView_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNWorldRenderView_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] worldrenderview : instance
*/
LN_FLAT_API LNResult LNWorldRenderView_SetPrototype_OnSerialize(LNHandle worldrenderview, LNHandle callback);

extern LN_FLAT_API int LNWorldRenderView_GetTypeInfoId();
LN_FLAT_API void LNWorldRenderView_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldRenderView_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNWorldRenderView_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNWorldRenderView_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldRenderView_RegisterSubclassTypeInfo(const LNWorldRenderView_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldRenderView_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::BoxMesh

/**
    @brief 各軸のサイズが 1 である BoxMesh を作成します。
    @param[out] outBoxMesh : instance.
*/
LN_FLAT_API LNResult LNBoxMesh_Create(LNHandle* outBoxMesh);

typedef LNResult(*LNBoxMesh_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNBoxMesh_OnSerialize_SetOverrideCallback(LNBoxMesh_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNBoxMesh_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNBoxMesh_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNBoxMesh_OnUpdate_SetOverrideCallback(LNBoxMesh_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNBoxMesh_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] boxmesh : instance
*/
LN_FLAT_API LNResult LNBoxMesh_SetPrototype_OnSerialize(LNHandle boxmesh, LNHandle callback);

/**
    @brief 
    @param[in] boxmesh : instance
*/
LN_FLAT_API LNResult LNBoxMesh_SetPrototype_OnUpdate(LNHandle boxmesh, LNHandle callback);

extern LN_FLAT_API int LNBoxMesh_GetTypeInfoId();
LN_FLAT_API void LNBoxMesh_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNBoxMesh_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNBoxMesh_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNBoxMesh_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNBoxMesh_SubclassRegistrationInfo;

extern LN_FLAT_API void LNBoxMesh_RegisterSubclassTypeInfo(const LNBoxMesh_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNBoxMesh_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::PlaneMesh

/**
    @brief 
    @param[out] outPlaneMesh : instance.
*/
LN_FLAT_API LNResult LNPlaneMesh_Create(LNHandle* outPlaneMesh);

typedef LNResult(*LNPlaneMesh_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNPlaneMesh_OnSerialize_SetOverrideCallback(LNPlaneMesh_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNPlaneMesh_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNPlaneMesh_OnUpdate_OverrideCallback)(LNHandle worldobject, float elapsedSeconds);
LN_FLAT_API LNResult LNPlaneMesh_OnUpdate_SetOverrideCallback(LNPlaneMesh_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNPlaneMesh_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds);

/**
    @brief 
    @param[in] planemesh : instance
*/
LN_FLAT_API LNResult LNPlaneMesh_SetPrototype_OnSerialize(LNHandle planemesh, LNHandle callback);

/**
    @brief 
    @param[in] planemesh : instance
*/
LN_FLAT_API LNResult LNPlaneMesh_SetPrototype_OnUpdate(LNHandle planemesh, LNHandle callback);

extern LN_FLAT_API int LNPlaneMesh_GetTypeInfoId();
LN_FLAT_API void LNPlaneMesh_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPlaneMesh_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNPlaneMesh_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNPlaneMesh_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNPlaneMesh_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPlaneMesh_RegisterSubclassTypeInfo(const LNPlaneMesh_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPlaneMesh_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Level

/**
    @brief Initialize
    @param[out] outLevel : instance.
*/
LN_FLAT_API LNResult LNLevel_Create(LNHandle* outLevel);

typedef LNResult(*LNLevel_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNLevel_OnSerialize_SetOverrideCallback(LNLevel_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNLevel_OnStart_OverrideCallback)(LNHandle level);
LN_FLAT_API LNResult LNLevel_OnStart_SetOverrideCallback(LNLevel_OnStart_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnStart_CallOverrideBase(LNHandle level);
typedef LNResult(*LNLevel_OnStop_OverrideCallback)(LNHandle level);
LN_FLAT_API LNResult LNLevel_OnStop_SetOverrideCallback(LNLevel_OnStop_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnStop_CallOverrideBase(LNHandle level);
typedef LNResult(*LNLevel_OnPause_OverrideCallback)(LNHandle level);
LN_FLAT_API LNResult LNLevel_OnPause_SetOverrideCallback(LNLevel_OnPause_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnPause_CallOverrideBase(LNHandle level);
typedef LNResult(*LNLevel_OnResume_OverrideCallback)(LNHandle level);
LN_FLAT_API LNResult LNLevel_OnResume_SetOverrideCallback(LNLevel_OnResume_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnResume_CallOverrideBase(LNHandle level);
typedef LNResult(*LNLevel_OnUpdate_OverrideCallback)(LNHandle level);
LN_FLAT_API LNResult LNLevel_OnUpdate_SetOverrideCallback(LNLevel_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNLevel_OnUpdate_CallOverrideBase(LNHandle level);

/**
    @brief 
    @param[in] level : instance
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnSerialize(LNHandle level, LNHandle callback);

/**
    @brief 
    @param[in] level : instance
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnStart(LNHandle level, LNHandle callback);

/**
    @brief 
    @param[in] level : instance
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnStop(LNHandle level, LNHandle callback);

/**
    @brief 
    @param[in] level : instance
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnPause(LNHandle level, LNHandle callback);

/**
    @brief 
    @param[in] level : instance
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnResume(LNHandle level, LNHandle callback);

/**
    @brief 
    @param[in] level : instance
*/
LN_FLAT_API LNResult LNLevel_SetPrototype_OnUpdate(LNHandle level, LNHandle callback);

extern LN_FLAT_API int LNLevel_GetTypeInfoId();
LN_FLAT_API void LNLevel_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevel_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNLevel_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNLevel_OnStart_OverrideCallback OnStart_OverrideFunc;
    LNLevel_OnStop_OverrideCallback OnStop_OverrideFunc;
    LNLevel_OnPause_OverrideCallback OnPause_OverrideFunc;
    LNLevel_OnResume_OverrideCallback OnResume_OverrideFunc;
    LNLevel_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNLevel_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevel_RegisterSubclassTypeInfo(const LNLevel_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevel_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIEventArgs

/**
    @brief イベントの発生元となった要素を取得します。
    @param[in] uieventargs : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNUIEventArgs_Sender(LNHandle uieventargs, LNHandle* outReturn);

typedef LNResult(*LNUIEventArgs_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIEventArgs_OnSerialize_SetOverrideCallback(LNUIEventArgs_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIEventArgs_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uieventargs : instance
*/
LN_FLAT_API LNResult LNUIEventArgs_SetPrototype_OnSerialize(LNHandle uieventargs, LNHandle callback);

extern LN_FLAT_API int LNUIEventArgs_GetTypeInfoId();
LN_FLAT_API void LNUIEventArgs_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIEventArgs_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIEventArgs_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIEventArgs_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIEventArgs_RegisterSubclassTypeInfo(const LNUIEventArgs_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIEventArgs_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIGeneralEventHandler

LN_FLAT_API LNResult LNUIGeneralEventHandler_Create(LNUIGeneralEventHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIGeneralEventHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIGeneralEventHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIGeneralEventHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIGeneralEventHandler_RegisterSubclassTypeInfo(const LNUIGeneralEventHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIGeneralEventHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIEventHandler

LN_FLAT_API LNResult LNUIEventHandler_Create(LNUIEventHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIEventHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIEventHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIEventHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIEventHandler_RegisterSubclassTypeInfo(const LNUIEventHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIEventHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UILayoutElement

typedef LNResult(*LNUILayoutElement_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUILayoutElement_OnSerialize_SetOverrideCallback(LNUILayoutElement_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUILayoutElement_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uilayoutelement : instance
*/
LN_FLAT_API LNResult LNUILayoutElement_SetPrototype_OnSerialize(LNHandle uilayoutelement, LNHandle callback);

extern LN_FLAT_API int LNUILayoutElement_GetTypeInfoId();
LN_FLAT_API void LNUILayoutElement_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUILayoutElement_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUILayoutElement_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUILayoutElement_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUILayoutElement_RegisterSubclassTypeInfo(const LNUILayoutElement_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUILayoutElement_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UIElement

/**
    @brief 要素の margin 値 (外側の余白) を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetMargin(LNHandle uielement, const LNThickness* margin);

/**
    @brief 要素の margin 値 (外側の余白) を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNUIElement_GetMargin(LNHandle uielement, LNThickness* outReturn);

/**
    @brief 要素の padding 値 (内側の余白) を設定します。この余白は論理ツリーの子要素のレイアウトに影響します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetPadding(LNHandle uielement, const LNThickness* padding);

/**
    @brief 要素の padding 値 (内側の余白) を取得します。この余白は論理ツリーの子要素のレイアウトに影響します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNUIElement_GetPadding(LNHandle uielement, LNThickness* outReturn);

/**
    @brief 要素の横方向の配置方法を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetHAlignment(LNHandle uielement, LNHAlignment value);

/**
    @brief 要素の横方向の配置方法を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNUIElement_GetHAlignment(LNHandle uielement, LNHAlignment* outReturn);

/**
    @brief 要素の縦方向の配置方法を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetVAlignment(LNHandle uielement, LNVAlignment value);

/**
    @brief 要素の縦方向の配置方法を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNUIElement_GetVAlignment(LNHandle uielement, LNVAlignment* outReturn);

/**
    @brief 要素の配置方法を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetAlignments(LNHandle uielement, LNHAlignment halign, LNVAlignment valign);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetPosition(LNHandle uielement, const LNVector3* pos);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetPositionXYZ(LNHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトの位置を位置を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNUIElement_GetPosition(LNHandle uielement, LNVector3* outReturn);

/**
    @brief このオブジェクトの回転を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetRotation(LNHandle uielement, const LNQuaternion* rot);

/**
    @brief このオブジェクトの回転をオイラー角から設定します。(radian)
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetEulerAngles(LNHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトの回転を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNUIElement_GetRotation(LNHandle uielement, LNQuaternion* outReturn);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetScale(LNHandle uielement, const LNVector3* scale);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetScaleS(LNHandle uielement, float xyz);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetScaleXY(LNHandle uielement, float x, float y);

/**
    @brief このオブジェクトの拡大率を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNUIElement_GetScale(LNHandle uielement, LNVector3* outReturn);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetCenterPoint(LNHandle uielement, const LNVector3* value);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetCenterPointXYZ(LNHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトのローカルの中心位置を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNUIElement_GetCenterPoint(LNHandle uielement, LNVector3* outReturn);

/**
    @brief Add element to container. 論理的な子要素として追加する。
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_AddChild(LNHandle uielement, LNHandle child);

typedef LNResult(*LNUIElement_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUIElement_OnSerialize_SetOverrideCallback(LNUIElement_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUIElement_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uielement : instance
*/
LN_FLAT_API LNResult LNUIElement_SetPrototype_OnSerialize(LNHandle uielement, LNHandle callback);

extern LN_FLAT_API int LNUIElement_GetTypeInfoId();
LN_FLAT_API void LNUIElement_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIElement_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUIElement_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUIElement_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIElement_RegisterSubclassTypeInfo(const LNUIElement_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIElement_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UITextBlock

/**
    @brief UITextBlock を作成します。
    @param[out] outUITextBlock : instance.
*/
LN_FLAT_API LNResult LNUITextBlock_Create(LNHandle* outUITextBlock);

/**
    @brief 表示文字列を指定して、UITextBlock を作成します。
    @param[in] text : 表示文字列
    @param[out] outUITextBlock : instance.
*/
LN_FLAT_API LNResult LNUITextBlock_CreateWithText(const LNChar* text, LNHandle* outUITextBlock);
LN_FLAT_API LNResult LNUITextBlock_CreateWithTextA(const char* text, LNHandle* outUITextBlock);

typedef LNResult(*LNUITextBlock_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUITextBlock_OnSerialize_SetOverrideCallback(LNUITextBlock_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUITextBlock_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uitextblock : instance
*/
LN_FLAT_API LNResult LNUITextBlock_SetPrototype_OnSerialize(LNHandle uitextblock, LNHandle callback);

extern LN_FLAT_API int LNUITextBlock_GetTypeInfoId();
LN_FLAT_API void LNUITextBlock_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUITextBlock_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUITextBlock_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUITextBlock_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUITextBlock_RegisterSubclassTypeInfo(const LNUITextBlock_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUITextBlock_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::UISprite

/**
    @brief UISprite を作成します。
    @param[out] outUISprite : instance.
*/
LN_FLAT_API LNResult LNUISprite_Create(LNHandle* outUISprite);

/**
    @brief テクスチャを指定して UISprite を作成します。
    @param[out] outUISprite : instance.
*/
LN_FLAT_API LNResult LNUISprite_CreateWithTexture(LNHandle texture, LNHandle* outUISprite);

/**
    @brief スプライトが表示するテクスチャを設定します。
    @param[in] uisprite : instance
*/
LN_FLAT_API LNResult LNUISprite_SetTexture(LNHandle uisprite, LNHandle texture);

/**
    @brief テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) デフォルトは Rect::Empty で、テクスチャ全体を転送することを示します。
    @param[in] uisprite : instance
*/
LN_FLAT_API LNResult LNUISprite_SetSourceRect(LNHandle uisprite, const LNRect* rect);

/**
    @brief 
    @param[in] uisprite : instance
*/
LN_FLAT_API LNResult LNUISprite_SetSourceRectXYWH(LNHandle uisprite, float x, float y, float width, float height);

/**
    @brief テクスチャのどの部分を表示するかを示す転送矩形を取得します。(ピクセル単位)
    @param[in] uisprite : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNUISprite_GetSourceRect(LNHandle uisprite, LNRect* outReturn);

typedef LNResult(*LNUISprite_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNUISprite_OnSerialize_SetOverrideCallback(LNUISprite_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNUISprite_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] uisprite : instance
*/
LN_FLAT_API LNResult LNUISprite_SetPrototype_OnSerialize(LNHandle uisprite, LNHandle callback);

extern LN_FLAT_API int LNUISprite_GetTypeInfoId();
LN_FLAT_API void LNUISprite_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUISprite_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNUISprite_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNUISprite_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUISprite_RegisterSubclassTypeInfo(const LNUISprite_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUISprite_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Input

/**
    @brief ボタンが現在押されているかを判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNInput_Pressed(const LNChar* buttonName, LNBool* outReturn);
LN_FLAT_API LNResult LNInput_PressedA(const char* buttonName, LNBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNInput_Triggered(const LNChar* buttonName, LNBool* outReturn);
LN_FLAT_API LNResult LNInput_TriggeredA(const char* buttonName, LNBool* outReturn);

/**
    @brief ボタンが離された瞬間を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNInput_TriggeredOff(const LNChar* buttonName, LNBool* outReturn);
LN_FLAT_API LNResult LNInput_TriggeredOffA(const char* buttonName, LNBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間とリピート状態を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNInput_Repeated(const LNChar* buttonName, LNBool* outReturn);
LN_FLAT_API LNResult LNInput_RepeatedA(const char* buttonName, LNBool* outReturn);

/**
    @brief 指定した軸のアナログ値を取得します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNInput_GetAxisValue(const LNChar* buttonName, float* outReturn);
LN_FLAT_API LNResult LNInput_GetAxisValueA(const char* buttonName, float* outReturn);

/**
    @brief 全てのアクションマッピングを削除します。
*/
LN_FLAT_API LNResult LNInput_ClearAllBindings();


//==============================================================================
// ln::Mouse

/**
    @brief ボタンが現在押されているかを判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNMouse_Pressed(LNMouseButtons button, LNBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNMouse_Triggered(LNMouseButtons button, LNBool* outReturn);

/**
    @brief ボタンが離された瞬間を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNMouse_TriggeredOff(LNMouseButtons button, LNBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間とリピート状態を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNMouse_Repeated(LNMouseButtons button, LNBool* outReturn);

/**
    @brief マウスポインタの位置を取得します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNMouse_Position(LNPoint* outReturn);


//==============================================================================
// ln::InterpreterCommand

/**
    @brief コマンドの実行コードを取得します。
    @param[in] interpretercommand : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNInterpreterCommand_Code(LNHandle interpretercommand, const LNChar** outReturn);
LN_FLAT_API LNResult LNInterpreterCommand_CodeA(LNHandle interpretercommand, const char** outReturn);

/**
    @brief コマンドのパラメータ数を取得します。
    @param[in] interpretercommand : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNInterpreterCommand_ParamsCount(LNHandle interpretercommand, int* outReturn);

/**
    @brief コマンドのパラメータを取得します。
    @param[in] interpretercommand : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNInterpreterCommand_Param(LNHandle interpretercommand, int index, const LNChar** outReturn);
LN_FLAT_API LNResult LNInterpreterCommand_ParamA(LNHandle interpretercommand, int index, const char** outReturn);

typedef LNResult(*LNInterpreterCommand_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNInterpreterCommand_OnSerialize_SetOverrideCallback(LNInterpreterCommand_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNInterpreterCommand_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] interpretercommand : instance
*/
LN_FLAT_API LNResult LNInterpreterCommand_SetPrototype_OnSerialize(LNHandle interpretercommand, LNHandle callback);

extern LN_FLAT_API int LNInterpreterCommand_GetTypeInfoId();
LN_FLAT_API void LNInterpreterCommand_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterCommand_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNInterpreterCommand_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNInterpreterCommand_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterCommand_RegisterSubclassTypeInfo(const LNInterpreterCommand_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterCommand_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::InterpreterCommandList

/**
    @brief InterpreterCommandList を作成します。
    @param[out] outInterpreterCommandList : instance.
*/
LN_FLAT_API LNResult LNInterpreterCommandList_Create(LNHandle* outInterpreterCommandList);

/**
    @brief コマンドを追加します。
    @param[in] interpretercommandlist : instance
*/
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand(LNHandle interpretercommandlist, const LNChar* code);
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommandA(LNHandle interpretercommandlist, const char* code);

/**
    @brief コマンドと 1 つのパラメータを追加します。
    @param[in] interpretercommandlist : instance
*/
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand1(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0);
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand1A(LNHandle interpretercommandlist, const char* code, const char* param0);

/**
    @brief コマンドと 2 つのパラメータを追加します。
    @param[in] interpretercommandlist : instance
*/
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand2(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0, const LNChar* param1);
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand2A(LNHandle interpretercommandlist, const char* code, const char* param0, const char* param1);

/**
    @brief コマンドと 3 つのパラメータを追加します。
    @param[in] interpretercommandlist : instance
*/
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand3(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0, const LNChar* param1, const LNChar* param2);
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand3A(LNHandle interpretercommandlist, const char* code, const char* param0, const char* param1, const char* param2);

/**
    @brief コマンドと 4 つのパラメータを追加します。
    @param[in] interpretercommandlist : instance
*/
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand4(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0, const LNChar* param1, const LNChar* param2, const LNChar* param3);
LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand4A(LNHandle interpretercommandlist, const char* code, const char* param0, const char* param1, const char* param2, const char* param3);

typedef LNResult(*LNInterpreterCommandList_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNInterpreterCommandList_OnSerialize_SetOverrideCallback(LNInterpreterCommandList_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNInterpreterCommandList_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);

/**
    @brief 
    @param[in] interpretercommandlist : instance
*/
LN_FLAT_API LNResult LNInterpreterCommandList_SetPrototype_OnSerialize(LNHandle interpretercommandlist, LNHandle callback);

extern LN_FLAT_API int LNInterpreterCommandList_GetTypeInfoId();
LN_FLAT_API void LNInterpreterCommandList_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterCommandList_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNInterpreterCommandList_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;

} LNInterpreterCommandList_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterCommandList_RegisterSubclassTypeInfo(const LNInterpreterCommandList_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterCommandList_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::InterpreterCommandDelegate

LN_FLAT_API LNResult LNInterpreterCommandDelegate_Create(LNInterpreterCommandDelegateCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNInterpreterCommandDelegate_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterCommandDelegate_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNInterpreterCommandDelegate_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterCommandDelegate_RegisterSubclassTypeInfo(const LNInterpreterCommandDelegate_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterCommandDelegate_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Interpreter

/**
    @brief Interpreter を作成します。
    @param[out] outInterpreter : instance.
*/
LN_FLAT_API LNResult LNInterpreter_Create(LNHandle* outInterpreter);

/**
    @brief コマンドリストをクリアします。実行中のコマンドリストは強制的に破棄されます。
    @param[in] interpreter : instance
*/
LN_FLAT_API LNResult LNInterpreter_Clear(LNHandle interpreter);

/**
    @brief コマンドリストを設定し、実行を開始します。
    @param[in] interpreter : instance
*/
LN_FLAT_API LNResult LNInterpreter_Run(LNHandle interpreter, LNHandle commandList);

/**
    @brief コマンドリストの実行中であるかを確認します。
    @param[in] interpreter : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNInterpreter_IsRunning(LNHandle interpreter, LNBool* outReturn);

/**
    @brief 
    @param[in] interpreter : instance
*/
LN_FLAT_API LNResult LNInterpreter_Update(LNHandle interpreter);

/**
    @brief 
    @param[in] interpreter : instance
*/
LN_FLAT_API LNResult LNInterpreter_Terminate(LNHandle interpreter);

/**
    @brief 
    @param[in] interpreter : instance
*/
LN_FLAT_API LNResult LNInterpreter_RegisterCommandHandler(LNHandle interpreter, const LNChar* name, LNHandle handler);
LN_FLAT_API LNResult LNInterpreter_RegisterCommandHandlerA(LNHandle interpreter, const char* name, LNHandle handler);

typedef LNResult(*LNInterpreter_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNInterpreter_OnSerialize_SetOverrideCallback(LNInterpreter_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNInterpreter_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNInterpreter_OnUpdateWait_OverrideCallback)(LNHandle interpreter, const LNChar* waitMode, LNBool* outReturn);
LN_FLAT_API LNResult LNInterpreter_OnUpdateWait_SetOverrideCallback(LNInterpreter_OnUpdateWait_OverrideCallback callback);
LN_FLAT_API LNResult LNInterpreter_OnUpdateWait_CallOverrideBase(LNHandle interpreter, const LNChar* waitMode, LNBool* outReturn);

/**
    @brief 
    @param[in] interpreter : instance
*/
LN_FLAT_API LNResult LNInterpreter_SetPrototype_OnSerialize(LNHandle interpreter, LNHandle callback);

/**
    @brief 
    @param[in] interpreter : instance
*/
LN_FLAT_API LNResult LNInterpreter_SetPrototype_OnUpdateWait(LNHandle interpreter, LNHandle callback);

extern LN_FLAT_API int LNInterpreter_GetTypeInfoId();
LN_FLAT_API void LNInterpreter_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreter_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNInterpreter_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNInterpreter_OnUpdateWait_OverrideCallback OnUpdateWait_OverrideFunc;

} LNInterpreter_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreter_RegisterSubclassTypeInfo(const LNInterpreter_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreter_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::EngineSettings

/**
    @brief メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480)
*/
LN_FLAT_API LNResult LNEngineSettings_SetMainWindowSize(int width, int height);

/**
    @brief メインウィンドウに対して作成される WorldView のサイズを設定します。(default: クライアント領域のサイズと同等)
*/
LN_FLAT_API LNResult LNEngineSettings_SetMainWorldViewSize(int width, int height);

/**
    @brief メインウィンドウのタイトル文字列を設定します。
*/
LN_FLAT_API LNResult LNEngineSettings_SetMainWindowTitle(const LNChar* title);
LN_FLAT_API LNResult LNEngineSettings_SetMainWindowTitleA(const char* title);

/**
    @brief アセットが保存されているディレクトリを登録します。
*/
LN_FLAT_API LNResult LNEngineSettings_AddAssetDirectory(const LNChar* path);
LN_FLAT_API LNResult LNEngineSettings_AddAssetDirectoryA(const char* path);

/**
    @brief アセットファイルを登録します。
*/
LN_FLAT_API LNResult LNEngineSettings_AddAssetArchive(const LNChar* fileFullPath, const LNChar* password);
LN_FLAT_API LNResult LNEngineSettings_AddAssetArchiveA(const char* fileFullPath, const char* password);

/**
    @brief フレームレートを設定します。(default: 60)
*/
LN_FLAT_API LNResult LNEngineSettings_SetFrameRate(int value);

/**
    @brief (default: Debug ビルドの場合true、それ以外は false)
*/
LN_FLAT_API LNResult LNEngineSettings_SetDebugToolEnabled(LNBool enabled);

/**
    @brief デバッグ用のログファイルの出力有無を設定します。(default: Debug ビルドの場合true、それ以外は false)
*/
LN_FLAT_API LNResult LNEngineSettings_SetEngineLogEnabled(LNBool enabled);

/**
    @brief デバッグ用のログファイルの出力先ファイルパスを設定します。(default: Empty(実行ファイルのディレクトリへ出力))
*/
LN_FLAT_API LNResult LNEngineSettings_SetEngineLogFilePath(const LNChar* filePath);
LN_FLAT_API LNResult LNEngineSettings_SetEngineLogFilePathA(const char* filePath);


//==============================================================================
// ln::Engine

/**
    @brief エンジンの初期化処理を行います。
*/
LN_FLAT_API LNResult LNEngine_Initialize();

/**
    @brief エンジンの終了処理を行います。
*/
LN_FLAT_API LNResult LNEngine_Finalize();

/**
    @brief 
    @param[out] outReturn : instance.
    @return アプリケーションの終了が要求されている場合は false を返します。
*/
LN_FLAT_API LNResult LNEngine_Update(LNBool* outReturn);

/**
    @brief 指定した Application の実行を開始します。
    @details この機能を呼び出した場合、Engine::initialize(), Engine::finalize(), Engine::update() を呼び出すことはできなくなります。代わりに Application::onInit(), Application::onUpdate() などを使用してください。
*/
LN_FLAT_API LNResult LNEngine_Run(LNHandle app);

/**
    @brief アプリケーション開始からの経過時間を取得します。この値はタイムスケールの影響を受けます。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEngine_Time(double* outReturn);

/**
    @brief デフォルトで作成されるメインの Camera です。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEngine_GetCamera(LNHandle* outReturn);

/**
    @brief デフォルトで作成されるメインの Light です。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEngine_GetMainLight(LNHandle* outReturn);

/**
    @brief デフォルトで作成されるメインの RenderView です。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNEngine_GetRenderView(LNHandle* outReturn);


//==============================================================================
// ln::Application

/**
    @brief 
    @param[out] outApplication : instance.
*/
LN_FLAT_API LNResult LNApplication_Create(LNHandle* outApplication);

/**
    @brief エンジンの初期化処理が完了した後に呼び出されます。
    @param[in] application : instance
*/
LN_FLAT_API LNResult LNApplication_OnInit(LNHandle application);

/**
    @brief 毎フレーム呼び出されます。
    @param[in] application : instance
*/
LN_FLAT_API LNResult LNApplication_OnUpdate(LNHandle application);

/**
    @brief デフォルトで作成されるメインの World を取得します。
    @param[in] application : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LNResult LNApplication_World(LNHandle application, LNHandle* outReturn);

typedef LNResult(*LNApplication_OnSerialize_OverrideCallback)(LNHandle object, LNHandle ar);
LN_FLAT_API LNResult LNApplication_OnSerialize_SetOverrideCallback(LNApplication_OnSerialize_OverrideCallback callback);
LN_FLAT_API LNResult LNApplication_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar);
typedef LNResult(*LNApplication_OnInit_OverrideCallback)(LNHandle application);
LN_FLAT_API LNResult LNApplication_OnInit_SetOverrideCallback(LNApplication_OnInit_OverrideCallback callback);
LN_FLAT_API LNResult LNApplication_OnInit_CallOverrideBase(LNHandle application);
typedef LNResult(*LNApplication_OnUpdate_OverrideCallback)(LNHandle application);
LN_FLAT_API LNResult LNApplication_OnUpdate_SetOverrideCallback(LNApplication_OnUpdate_OverrideCallback callback);
LN_FLAT_API LNResult LNApplication_OnUpdate_CallOverrideBase(LNHandle application);

/**
    @brief 
    @param[in] application : instance
*/
LN_FLAT_API LNResult LNApplication_SetPrototype_OnSerialize(LNHandle application, LNHandle callback);

/**
    @brief 
    @param[in] application : instance
*/
LN_FLAT_API LNResult LNApplication_SetPrototype_OnInit(LNHandle application, LNHandle callback);

/**
    @brief 
    @param[in] application : instance
*/
LN_FLAT_API LNResult LNApplication_SetPrototype_OnUpdate(LNHandle application, LNHandle callback);

extern LN_FLAT_API int LNApplication_GetTypeInfoId();
LN_FLAT_API void LNApplication_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNApplication_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;
    LNApplication_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
    LNApplication_OnInit_OverrideCallback OnInit_OverrideFunc;
    LNApplication_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

} LNApplication_SubclassRegistrationInfo;

extern LN_FLAT_API void LNApplication_RegisterSubclassTypeInfo(const LNApplication_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNApplication_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ln::Debug

/**
    @brief ウィンドウ上にデバッグ文字列を表示します。
    @param[in] str : 表示文字列
*/
LN_FLAT_API LNResult LNDebug_Print(const LNChar* str);
LN_FLAT_API LNResult LNDebug_PrintA(const char* str);

/**
    @brief 表示時間を指定して、ウィンドウ上にデバッグ文字列を表示します。
    @param[in] time : 表示時間 (s)
    @param[in] str : 表示文字列
*/
LN_FLAT_API LNResult LNDebug_PrintWithTime(float time, const LNChar* str);
LN_FLAT_API LNResult LNDebug_PrintWithTimeA(float time, const char* str);

/**
    @brief 表示時間と文字色を指定して、ウィンドウ上にデバッグ文字列を表示します。
    @param[in] time : 表示時間 (s)
    @param[in] color : 文字色
    @param[in] str : 表示文字列
*/
LN_FLAT_API LNResult LNDebug_PrintWithTimeAndColor(float time, const LNColor* color, const LNChar* str);
LN_FLAT_API LNResult LNDebug_PrintWithTimeAndColorA(float time, const LNColor* color, const char* str);


//==============================================================================
// ObjectSerializeHandler

LN_FLAT_API LNResult LNObjectSerializeHandler_Create(LNObjectSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNObjectSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNObjectSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNObjectSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNObjectSerializeHandler_RegisterSubclassTypeInfo(const LNObjectSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNObjectSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// EventConnectionSerializeHandler

LN_FLAT_API LNResult LNEventConnectionSerializeHandler_Create(LNEventConnectionSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNEventConnectionSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEventConnectionSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNEventConnectionSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEventConnectionSerializeHandler_RegisterSubclassTypeInfo(const LNEventConnectionSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEventConnectionSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ZVTestClass1SerializeHandler

LN_FLAT_API LNResult LNZVTestClass1SerializeHandler_Create(LNZVTestClass1SerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestClass1SerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestClass1SerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestClass1SerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestClass1SerializeHandler_RegisterSubclassTypeInfo(const LNZVTestClass1SerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestClass1SerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ZVTestEventArgs1SerializeHandler

LN_FLAT_API LNResult LNZVTestEventArgs1SerializeHandler_Create(LNZVTestEventArgs1SerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNZVTestEventArgs1SerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNZVTestEventArgs1SerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNZVTestEventArgs1SerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNZVTestEventArgs1SerializeHandler_RegisterSubclassTypeInfo(const LNZVTestEventArgs1SerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNZVTestEventArgs1SerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// Serializer2SerializeHandler

LN_FLAT_API LNResult LNSerializer2SerializeHandler_Create(LNSerializer2SerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSerializer2SerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSerializer2SerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSerializer2SerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSerializer2SerializeHandler_RegisterSubclassTypeInfo(const LNSerializer2SerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSerializer2SerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// AssetModelSerializeHandler

LN_FLAT_API LNResult LNAssetModelSerializeHandler_Create(LNAssetModelSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNAssetModelSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNAssetModelSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNAssetModelSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNAssetModelSerializeHandler_RegisterSubclassTypeInfo(const LNAssetModelSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNAssetModelSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// TextureSerializeHandler

LN_FLAT_API LNResult LNTextureSerializeHandler_Create(LNTextureSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNTextureSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTextureSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNTextureSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTextureSerializeHandler_RegisterSubclassTypeInfo(const LNTextureSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTextureSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// Texture2DSerializeHandler

LN_FLAT_API LNResult LNTexture2DSerializeHandler_Create(LNTexture2DSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNTexture2DSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNTexture2DSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNTexture2DSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNTexture2DSerializeHandler_RegisterSubclassTypeInfo(const LNTexture2DSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNTexture2DSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// RenderViewSerializeHandler

LN_FLAT_API LNResult LNRenderViewSerializeHandler_Create(LNRenderViewSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNRenderViewSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRenderViewSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNRenderViewSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRenderViewSerializeHandler_RegisterSubclassTypeInfo(const LNRenderViewSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRenderViewSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ComponentSerializeHandler

LN_FLAT_API LNResult LNComponentSerializeHandler_Create(LNComponentSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNComponentSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNComponentSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNComponentSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNComponentSerializeHandler_RegisterSubclassTypeInfo(const LNComponentSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNComponentSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// VisualComponentSerializeHandler

LN_FLAT_API LNResult LNVisualComponentSerializeHandler_Create(LNVisualComponentSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNVisualComponentSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVisualComponentSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNVisualComponentSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVisualComponentSerializeHandler_RegisterSubclassTypeInfo(const LNVisualComponentSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVisualComponentSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpriteComponentSerializeHandler

LN_FLAT_API LNResult LNSpriteComponentSerializeHandler_Create(LNSpriteComponentSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpriteComponentSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpriteComponentSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpriteComponentSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpriteComponentSerializeHandler_RegisterSubclassTypeInfo(const LNSpriteComponentSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpriteComponentSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// WorldSerializeHandler

LN_FLAT_API LNResult LNWorldSerializeHandler_Create(LNWorldSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNWorldSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNWorldSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldSerializeHandler_RegisterSubclassTypeInfo(const LNWorldSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ComponentListSerializeHandler

LN_FLAT_API LNResult LNComponentListSerializeHandler_Create(LNComponentListSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNComponentListSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNComponentListSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNComponentListSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNComponentListSerializeHandler_RegisterSubclassTypeInfo(const LNComponentListSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNComponentListSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// WorldObjectSerializeHandler

LN_FLAT_API LNResult LNWorldObjectSerializeHandler_Create(LNWorldObjectSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNWorldObjectSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldObjectSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNWorldObjectSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldObjectSerializeHandler_RegisterSubclassTypeInfo(const LNWorldObjectSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldObjectSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// WorldObjectUpdateHandler

LN_FLAT_API LNResult LNWorldObjectUpdateHandler_Create(LNWorldObjectUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNWorldObjectUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldObjectUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNWorldObjectUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldObjectUpdateHandler_RegisterSubclassTypeInfo(const LNWorldObjectUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldObjectUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// VisualObjectSerializeHandler

LN_FLAT_API LNResult LNVisualObjectSerializeHandler_Create(LNVisualObjectSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNVisualObjectSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVisualObjectSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNVisualObjectSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVisualObjectSerializeHandler_RegisterSubclassTypeInfo(const LNVisualObjectSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVisualObjectSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// VisualObjectUpdateHandler

LN_FLAT_API LNResult LNVisualObjectUpdateHandler_Create(LNVisualObjectUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNVisualObjectUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNVisualObjectUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNVisualObjectUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNVisualObjectUpdateHandler_RegisterSubclassTypeInfo(const LNVisualObjectUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNVisualObjectUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// CameraSerializeHandler

LN_FLAT_API LNResult LNCameraSerializeHandler_Create(LNCameraSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNCameraSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCameraSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNCameraSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCameraSerializeHandler_RegisterSubclassTypeInfo(const LNCameraSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCameraSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// CameraUpdateHandler

LN_FLAT_API LNResult LNCameraUpdateHandler_Create(LNCameraUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNCameraUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCameraUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNCameraUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCameraUpdateHandler_RegisterSubclassTypeInfo(const LNCameraUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCameraUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// EnvironmentLightSerializeHandler

LN_FLAT_API LNResult LNEnvironmentLightSerializeHandler_Create(LNEnvironmentLightSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNEnvironmentLightSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEnvironmentLightSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNEnvironmentLightSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEnvironmentLightSerializeHandler_RegisterSubclassTypeInfo(const LNEnvironmentLightSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEnvironmentLightSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// EnvironmentLightUpdateHandler

LN_FLAT_API LNResult LNEnvironmentLightUpdateHandler_Create(LNEnvironmentLightUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNEnvironmentLightUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNEnvironmentLightUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNEnvironmentLightUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNEnvironmentLightUpdateHandler_RegisterSubclassTypeInfo(const LNEnvironmentLightUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNEnvironmentLightUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// DirectionalLightSerializeHandler

LN_FLAT_API LNResult LNDirectionalLightSerializeHandler_Create(LNDirectionalLightSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNDirectionalLightSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNDirectionalLightSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNDirectionalLightSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNDirectionalLightSerializeHandler_RegisterSubclassTypeInfo(const LNDirectionalLightSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNDirectionalLightSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// DirectionalLightUpdateHandler

LN_FLAT_API LNResult LNDirectionalLightUpdateHandler_Create(LNDirectionalLightUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNDirectionalLightUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNDirectionalLightUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNDirectionalLightUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNDirectionalLightUpdateHandler_RegisterSubclassTypeInfo(const LNDirectionalLightUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNDirectionalLightUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// PointLightSerializeHandler

LN_FLAT_API LNResult LNPointLightSerializeHandler_Create(LNPointLightSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPointLightSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPointLightSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPointLightSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPointLightSerializeHandler_RegisterSubclassTypeInfo(const LNPointLightSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPointLightSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// PointLightUpdateHandler

LN_FLAT_API LNResult LNPointLightUpdateHandler_Create(LNPointLightUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPointLightUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPointLightUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPointLightUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPointLightUpdateHandler_RegisterSubclassTypeInfo(const LNPointLightUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPointLightUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpotLightSerializeHandler

LN_FLAT_API LNResult LNSpotLightSerializeHandler_Create(LNSpotLightSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpotLightSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpotLightSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpotLightSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpotLightSerializeHandler_RegisterSubclassTypeInfo(const LNSpotLightSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpotLightSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpotLightUpdateHandler

LN_FLAT_API LNResult LNSpotLightUpdateHandler_Create(LNSpotLightUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpotLightUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpotLightUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpotLightUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpotLightUpdateHandler_RegisterSubclassTypeInfo(const LNSpotLightUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpotLightUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpriteSerializeHandler

LN_FLAT_API LNResult LNSpriteSerializeHandler_Create(LNSpriteSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpriteSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpriteSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpriteSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpriteSerializeHandler_RegisterSubclassTypeInfo(const LNSpriteSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpriteSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// SpriteUpdateHandler

LN_FLAT_API LNResult LNSpriteUpdateHandler_Create(LNSpriteUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNSpriteUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNSpriteUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNSpriteUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNSpriteUpdateHandler_RegisterSubclassTypeInfo(const LNSpriteUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNSpriteUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// CameraOrbitControlComponentSerializeHandler

LN_FLAT_API LNResult LNCameraOrbitControlComponentSerializeHandler_Create(LNCameraOrbitControlComponentSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNCameraOrbitControlComponentSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNCameraOrbitControlComponentSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNCameraOrbitControlComponentSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNCameraOrbitControlComponentSerializeHandler_RegisterSubclassTypeInfo(const LNCameraOrbitControlComponentSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNCameraOrbitControlComponentSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// RaycasterSerializeHandler

LN_FLAT_API LNResult LNRaycasterSerializeHandler_Create(LNRaycasterSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNRaycasterSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRaycasterSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNRaycasterSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRaycasterSerializeHandler_RegisterSubclassTypeInfo(const LNRaycasterSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRaycasterSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// RaycastResultSerializeHandler

LN_FLAT_API LNResult LNRaycastResultSerializeHandler_Create(LNRaycastResultSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNRaycastResultSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNRaycastResultSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNRaycastResultSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNRaycastResultSerializeHandler_RegisterSubclassTypeInfo(const LNRaycastResultSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNRaycastResultSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// WorldRenderViewSerializeHandler

LN_FLAT_API LNResult LNWorldRenderViewSerializeHandler_Create(LNWorldRenderViewSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNWorldRenderViewSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNWorldRenderViewSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNWorldRenderViewSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNWorldRenderViewSerializeHandler_RegisterSubclassTypeInfo(const LNWorldRenderViewSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNWorldRenderViewSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// BoxMeshSerializeHandler

LN_FLAT_API LNResult LNBoxMeshSerializeHandler_Create(LNBoxMeshSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNBoxMeshSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNBoxMeshSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNBoxMeshSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNBoxMeshSerializeHandler_RegisterSubclassTypeInfo(const LNBoxMeshSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNBoxMeshSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// BoxMeshUpdateHandler

LN_FLAT_API LNResult LNBoxMeshUpdateHandler_Create(LNBoxMeshUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNBoxMeshUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNBoxMeshUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNBoxMeshUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNBoxMeshUpdateHandler_RegisterSubclassTypeInfo(const LNBoxMeshUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNBoxMeshUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// PlaneMeshSerializeHandler

LN_FLAT_API LNResult LNPlaneMeshSerializeHandler_Create(LNPlaneMeshSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPlaneMeshSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPlaneMeshSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPlaneMeshSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPlaneMeshSerializeHandler_RegisterSubclassTypeInfo(const LNPlaneMeshSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPlaneMeshSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// PlaneMeshUpdateHandler

LN_FLAT_API LNResult LNPlaneMeshUpdateHandler_Create(LNPlaneMeshUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNPlaneMeshUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNPlaneMeshUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNPlaneMeshUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNPlaneMeshUpdateHandler_RegisterSubclassTypeInfo(const LNPlaneMeshUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNPlaneMeshUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelSerializeHandler

LN_FLAT_API LNResult LNLevelSerializeHandler_Create(LNLevelSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelSerializeHandler_RegisterSubclassTypeInfo(const LNLevelSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelStartHandler

LN_FLAT_API LNResult LNLevelStartHandler_Create(LNLevelStartHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelStartHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelStartHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelStartHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelStartHandler_RegisterSubclassTypeInfo(const LNLevelStartHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelStartHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelStopHandler

LN_FLAT_API LNResult LNLevelStopHandler_Create(LNLevelStopHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelStopHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelStopHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelStopHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelStopHandler_RegisterSubclassTypeInfo(const LNLevelStopHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelStopHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelPauseHandler

LN_FLAT_API LNResult LNLevelPauseHandler_Create(LNLevelPauseHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelPauseHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelPauseHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelPauseHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelPauseHandler_RegisterSubclassTypeInfo(const LNLevelPauseHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelPauseHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelResumeHandler

LN_FLAT_API LNResult LNLevelResumeHandler_Create(LNLevelResumeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelResumeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelResumeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelResumeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelResumeHandler_RegisterSubclassTypeInfo(const LNLevelResumeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelResumeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// LevelUpdateHandler

LN_FLAT_API LNResult LNLevelUpdateHandler_Create(LNLevelUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNLevelUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNLevelUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNLevelUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNLevelUpdateHandler_RegisterSubclassTypeInfo(const LNLevelUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNLevelUpdateHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIEventArgsSerializeHandler

LN_FLAT_API LNResult LNUIEventArgsSerializeHandler_Create(LNUIEventArgsSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIEventArgsSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIEventArgsSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIEventArgsSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIEventArgsSerializeHandler_RegisterSubclassTypeInfo(const LNUIEventArgsSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIEventArgsSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UILayoutElementSerializeHandler

LN_FLAT_API LNResult LNUILayoutElementSerializeHandler_Create(LNUILayoutElementSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUILayoutElementSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUILayoutElementSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUILayoutElementSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUILayoutElementSerializeHandler_RegisterSubclassTypeInfo(const LNUILayoutElementSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUILayoutElementSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UIElementSerializeHandler

LN_FLAT_API LNResult LNUIElementSerializeHandler_Create(LNUIElementSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUIElementSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUIElementSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUIElementSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUIElementSerializeHandler_RegisterSubclassTypeInfo(const LNUIElementSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUIElementSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UITextBlockSerializeHandler

LN_FLAT_API LNResult LNUITextBlockSerializeHandler_Create(LNUITextBlockSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUITextBlockSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUITextBlockSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUITextBlockSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUITextBlockSerializeHandler_RegisterSubclassTypeInfo(const LNUITextBlockSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUITextBlockSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// UISpriteSerializeHandler

LN_FLAT_API LNResult LNUISpriteSerializeHandler_Create(LNUISpriteSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNUISpriteSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNUISpriteSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNUISpriteSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNUISpriteSerializeHandler_RegisterSubclassTypeInfo(const LNUISpriteSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNUISpriteSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// InterpreterCommandSerializeHandler

LN_FLAT_API LNResult LNInterpreterCommandSerializeHandler_Create(LNInterpreterCommandSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNInterpreterCommandSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterCommandSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNInterpreterCommandSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterCommandSerializeHandler_RegisterSubclassTypeInfo(const LNInterpreterCommandSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterCommandSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// InterpreterCommandListSerializeHandler

LN_FLAT_API LNResult LNInterpreterCommandListSerializeHandler_Create(LNInterpreterCommandListSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNInterpreterCommandListSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterCommandListSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNInterpreterCommandListSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterCommandListSerializeHandler_RegisterSubclassTypeInfo(const LNInterpreterCommandListSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterCommandListSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// InterpreterSerializeHandler

LN_FLAT_API LNResult LNInterpreterSerializeHandler_Create(LNInterpreterSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNInterpreterSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNInterpreterSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterSerializeHandler_RegisterSubclassTypeInfo(const LNInterpreterSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// InterpreterUpdateWaitHandler

LN_FLAT_API LNResult LNInterpreterUpdateWaitHandler_Create(LNInterpreterUpdateWaitHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNInterpreterUpdateWaitHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNInterpreterUpdateWaitHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNInterpreterUpdateWaitHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNInterpreterUpdateWaitHandler_RegisterSubclassTypeInfo(const LNInterpreterUpdateWaitHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNInterpreterUpdateWaitHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ApplicationSerializeHandler

LN_FLAT_API LNResult LNApplicationSerializeHandler_Create(LNApplicationSerializeHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNApplicationSerializeHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNApplicationSerializeHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNApplicationSerializeHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNApplicationSerializeHandler_RegisterSubclassTypeInfo(const LNApplicationSerializeHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNApplicationSerializeHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ApplicationInitHandler

LN_FLAT_API LNResult LNApplicationInitHandler_Create(LNApplicationInitHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNApplicationInitHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNApplicationInitHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNApplicationInitHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNApplicationInitHandler_RegisterSubclassTypeInfo(const LNApplicationInitHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNApplicationInitHandler_GetSubinstanceId(LNHandle handle);

//==============================================================================
// ApplicationUpdateHandler

LN_FLAT_API LNResult LNApplicationUpdateHandler_Create(LNApplicationUpdateHandlerCallback callback, LNHandle* outDelegate);
LN_FLAT_API void LNApplicationUpdateHandler_SetManagedTypeInfoId(int64_t id); // deprecated
typedef struct tagLNApplicationUpdateHandler_SubclassRegistrationInfo
{
    int64_t subclassId;	// ManagedTypeInfoId
    LNSubinstanceAllocFunc subinstanceAllocFunc;
    LNSubinstanceFreeFunc subinstanceFreeFunc;

} LNApplicationUpdateHandler_SubclassRegistrationInfo;

extern LN_FLAT_API void LNApplicationUpdateHandler_RegisterSubclassTypeInfo(const LNApplicationUpdateHandler_SubclassRegistrationInfo* info);
extern LN_FLAT_API LNSubinstanceId LNApplicationUpdateHandler_GetSubinstanceId(LNHandle handle);



} // extern "C"

