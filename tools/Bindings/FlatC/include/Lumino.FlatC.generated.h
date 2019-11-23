
#pragma once
#include <LuminoEngine/Runtime/FlatCommon.h>

extern "C"
{
/**
    @brief 3次元のベクトルを定義します。
*/
struct LnVector3
{
    float x;
    float y;
    float z;
};
/**
    @brief クォータニオンを定義します。
    @details このクォータニオンクラスの乗算の順番は左から右です。つまり、q1 * q2 は、先に q1、次に q2 の回転を適用する事と同じです。
    この乗算順序は数学的な四元数の定義からは逆行している点に注意してください。
*/
struct LnQuaternion
{
    float x;
    float y;
    float z;
    float w;
};


/**
    @brief ピクセルフォーマット
*/
typedef enum tagLnPixelFormat
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

} LnPixelFormat;

/**
    @brief テクスチャのピクセルフォーマット
*/
typedef enum tagLnTextureFormat
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
        @brief 32bit の符号なし整数フォーマット
    */
    LN_TEXTURE_FORMAT_R32U = 7,

} LnTextureFormat;

/**
    @brief 深度バッファのフォーマット
*/
typedef enum tagLnDepthBufferFormat
{
    /**
        @brief S8 32 ビット
    */
    LN_DEPTH_BUFFER_FORMAT_D24S8 = 0,

} LnDepthBufferFormat;

typedef void(*LnUIEventHandlerCallback)(LnHandle, LnHandle);


/**
    @brief ベクトルの長さを返します。
*/
LN_FLAT_API LnResult LnVector3_Length(const LnVector3* vector3, float* outReturn);


/**
    @brief ベクトルの長さの 2 乗を返します。
*/
LN_FLAT_API LnResult LnVector3_LengthSquared(const LnVector3* vector3, float* outReturn);


/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] x : 処理の基になるベクトルの X 要
    @param[in] y : 処理の基になるベクトルの Y 要
    @param[in] z : 処理の基になるベクトルの Z 要
    @return 正規化されたベクトル
*/
LN_FLAT_API LnResult LnVector3_NormalizeXYZ(float x, float y, float z, LnVector3* outReturn);


/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] vec : 処理の基になるベクトル
    @return 正規化されたベクトル
*/
LN_FLAT_API LnResult LnVector3_Normalize(const LnVector3* vec, LnVector3* outReturn);




//==============================================================================
// ln::Engine

/**
    @brief エンジンの初期化処理を行います。
*/
LN_FLAT_API LnResult LnEngine_Initialize();


/**
    @brief エンジンの終了処理を行います。
*/
LN_FLAT_API LnResult LnEngine_Finalize();


/**
    @brief 
    @return アプリケーションの終了が要求されている場合は false を返します。
*/
LN_FLAT_API LnResult LnEngine_Update(LnBool* outReturn);


/**
    @brief 。
*/
LN_FLAT_API LnResult LnEngine_MainUIView(LnHandle* outReturn);


//==============================================================================
// ln::GraphicsResource

//==============================================================================
// ln::Texture

//==============================================================================
// ln::Texture2D

/**
    @brief テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    @param[in] width : 幅 (px 単位)
    @param[in] height : 高さ (px 単位)
    @return 作成されたテクスチャ
*/
LN_FLAT_API LnResult LnTexture2D_Create(int width, int height, LnHandle* outTexture2D);


/**
    @brief テクスチャを作成します。
    @param[in] width : 幅 (px 単位)
    @param[in] height : 高さ (px 単位)
    @param[in] format : ピクセルフォーマット
*/
LN_FLAT_API LnResult LnTexture2D_CreateWithFormat(int width, int height, LnTextureFormat format, LnHandle* outTexture2D);


/**
    @brief ローカルのファイルを読み込み、テクスチャを作成します。
    @param[in] filePath : 読み込むファイルのパス
    @param[in] format : ピクセルフォーマット
    @details このメソッドは TextureImporter のユーティリティです。
*/
LN_FLAT_API LnResult LnTexture2D_CreateFromFile(const LnChar* filePath, LnTextureFormat format, LnHandle* outTexture2D);
LN_FLAT_API LnResult LnTexture2D_CreateFromFileA(const char* filePath, LnTextureFormat format, LnHandle* outTexture2D);


//==============================================================================
// ln::Component

//==============================================================================
// ln::VisualComponent

/**
    @brief 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
*/
LN_FLAT_API LnResult LnVisualComponent_SetVisible(LnHandle visualcomponent, LnBool value);


/**
    @brief 可視状態を取得します。
*/
LN_FLAT_API LnResult LnVisualComponent_IsVisible(LnHandle visualcomponent, LnBool* outReturn);


//==============================================================================
// ln::SpriteComponent

/**
    @brief スプライトが表示するテクスチャを設定します。
*/
LN_FLAT_API LnResult LnSpriteComponent_SetTexture(LnHandle spritecomponent, LnHandle texture);


//==============================================================================
// ln::ComponentList

/**
    @brief 
*/
LN_FLAT_API LnResult LnComponentList_GetLength(LnHandle componentlist, int* outReturn);


/**
    @brief 
*/
LN_FLAT_API LnResult LnComponentList_GetItem(LnHandle componentlist, int index, LnHandle* outReturn);


//==============================================================================
// ln::WorldObject

/**
    @brief このオブジェクトの位置を設定します。
*/
LN_FLAT_API LnResult LnWorldObject_SetPosition(LnHandle worldobject, const LnVector3* pos);


/**
    @brief このオブジェクトの位置を設定します。
*/
LN_FLAT_API LnResult LnWorldObject_SetPositionXYZ(LnHandle worldobject, float x, float y, float z);


/**
    @brief このオブジェクトの位置を位置を取得します。
*/
LN_FLAT_API LnResult LnWorldObject_GetPosition(LnHandle worldobject, LnVector3* outReturn);


/**
    @brief このオブジェクトの回転を設定します。
*/
LN_FLAT_API LnResult LnWorldObject_SetRotation(LnHandle worldobject, const LnQuaternion* rot);


/**
    @brief このオブジェクトの回転をオイラー角から設定します。(radian)
*/
LN_FLAT_API LnResult LnWorldObject_SetEulerAngles(LnHandle worldobject, float x, float y, float z);


/**
    @brief このオブジェクトの回転を取得します。
*/
LN_FLAT_API LnResult LnWorldObject_GetRotation(LnHandle worldobject, LnQuaternion* outReturn);


/**
    @brief このオブジェクトの拡大率を設定します。
*/
LN_FLAT_API LnResult LnWorldObject_SetScale(LnHandle worldobject, const LnVector3* scale);


/**
    @brief このオブジェクトの拡大率を設定します。
*/
LN_FLAT_API LnResult LnWorldObject_SetScaleS(LnHandle worldobject, float xyz);


/**
    @brief このオブジェクトの拡大率を設定します。
*/
LN_FLAT_API LnResult LnWorldObject_SetScaleXYZ(LnHandle worldobject, float x, float y, float z);


/**
    @brief このオブジェクトの拡大率を取得します。
*/
LN_FLAT_API LnResult LnWorldObject_GetScale(LnHandle worldobject, LnVector3* outReturn);


/**
    @brief このオブジェクトのローカルの中心位置を設定します。
*/
LN_FLAT_API LnResult LnWorldObject_SetCenterPoint(LnHandle worldobject, const LnVector3* value);


/**
    @brief このオブジェクトのローカルの中心位置を設定します。
*/
LN_FLAT_API LnResult LnWorldObject_SetCenterPointXYZ(LnHandle worldobject, float x, float y, float z);


/**
    @brief このオブジェクトのローカルの中心位置を取得します。
*/
LN_FLAT_API LnResult LnWorldObject_GetCenterPoint(LnHandle worldobject, LnVector3* outReturn);


/**
    @brief 
*/
LN_FLAT_API LnResult LnWorldObject_GetComponents(LnHandle worldobject, LnHandle* outReturn);


typedef LnResult(*LnWorldObject_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnWorldObject_OnUpdate_SetOverrideCallback(LnWorldObject_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnWorldObject_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

//==============================================================================
// ln::VisualObject

/**
    @brief 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
*/
LN_FLAT_API LnResult LnVisualObject_SetVisible(LnHandle visualobject, LnBool value);


/**
    @brief 可視状態を取得します。
*/
LN_FLAT_API LnResult LnVisualObject_IsVisible(LnHandle visualobject, LnBool* outReturn);


typedef LnResult(*LnVisualObject_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnVisualObject_OnUpdate_SetOverrideCallback(LnVisualObject_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnVisualObject_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

//==============================================================================
// ln::Sprite

/**
    @brief スプライトが表示するテクスチャを設定します。
*/
LN_FLAT_API LnResult LnSprite_SetTexture(LnHandle sprite, LnHandle texture);


/**
    @brief 
*/
LN_FLAT_API LnResult LnSprite_SetSourceRectXYWH(LnHandle sprite, float x, float y, float width, float height);


/**
    @brief init
*/
LN_FLAT_API LnResult LnSprite_Create(LnHandle texture, float width, float height, LnHandle* outSprite);


typedef LnResult(*LnSprite_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnSprite_OnUpdate_SetOverrideCallback(LnSprite_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnSprite_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

//==============================================================================
// ln::UIEventArgs

/**
    @brief イベントの発生元となった要素を取得します。
*/
LN_FLAT_API LnResult LnUIEventArgs_Sender(LnHandle uieventargs, LnHandle* outReturn);


//==============================================================================
// ln::UILayoutElement

//==============================================================================
// ln::UIElement

/**
    @brief このオブジェクトの位置を設定します。
*/
LN_FLAT_API LnResult LnUIElement_SetPosition(LnHandle uielement, const LnVector3* pos);


/**
    @brief このオブジェクトの位置を設定します。
*/
LN_FLAT_API LnResult LnUIElement_SetPositionXYZ(LnHandle uielement, float x, float y, float z);


/**
    @brief このオブジェクトの位置を位置を取得します。
*/
LN_FLAT_API LnResult LnUIElement_GetPosition(LnHandle uielement, LnVector3* outReturn);


/**
    @brief このオブジェクトの回転を設定します。
*/
LN_FLAT_API LnResult LnUIElement_SetRotation(LnHandle uielement, const LnQuaternion* rot);


/**
    @brief このオブジェクトの回転をオイラー角から設定します。(radian)
*/
LN_FLAT_API LnResult LnUIElement_SetEulerAngles(LnHandle uielement, float x, float y, float z);


/**
    @brief このオブジェクトの回転を取得します。
*/
LN_FLAT_API LnResult LnUIElement_GetRotation(LnHandle uielement, LnQuaternion* outReturn);


/**
    @brief このオブジェクトの拡大率を設定します。
*/
LN_FLAT_API LnResult LnUIElement_SetScale(LnHandle uielement, const LnVector3* scale);


/**
    @brief このオブジェクトの拡大率を設定します。
*/
LN_FLAT_API LnResult LnUIElement_SetScaleS(LnHandle uielement, float xyz);


/**
    @brief このオブジェクトの拡大率を設定します。
*/
LN_FLAT_API LnResult LnUIElement_SetScaleXY(LnHandle uielement, float x, float y);


/**
    @brief このオブジェクトの拡大率を取得します。
*/
LN_FLAT_API LnResult LnUIElement_GetScale(LnHandle uielement, LnVector3* outReturn);


/**
    @brief このオブジェクトのローカルの中心位置を設定します。
*/
LN_FLAT_API LnResult LnUIElement_SetCenterPoint(LnHandle uielement, const LnVector3* value);


/**
    @brief このオブジェクトのローカルの中心位置を設定します。
*/
LN_FLAT_API LnResult LnUIElement_SetCenterPointXYZ(LnHandle uielement, float x, float y, float z);


/**
    @brief このオブジェクトのローカルの中心位置を取得します。
*/
LN_FLAT_API LnResult LnUIElement_GetCenterPoint(LnHandle uielement, LnVector3* outReturn);


/**
    @brief Add element to container.
*/
LN_FLAT_API LnResult LnUIElement_AddChild(LnHandle uielement, LnHandle child);


//==============================================================================
// ln::UIControl

//==============================================================================
// ln::UIButtonBase

/**
    @brief set text.
*/
LN_FLAT_API LnResult LnUIButtonBase_SetText(LnHandle uibuttonbase, const LnChar* text);
LN_FLAT_API LnResult LnUIButtonBase_SetTextA(LnHandle uibuttonbase, const char* text);


//==============================================================================
// ln::UIButton

/**
    @brief init.
*/
LN_FLAT_API LnResult LnUIButton_Create(LnHandle* outUIButton);


/**
    @brief Clicked イベントの通知を受け取るコールバックを登録します。
*/
LN_FLAT_API LnResult LnUIButton_ConnectOnClicked(LnHandle uibutton, LnUIEventHandlerCallback handler);




} // extern "C"

