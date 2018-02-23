
#pragma once
#include <Lumino/FlatCCommon.h>

extern "C"
{

struct LNVector2
{
    float x;
    float y;
};
struct LNVector3
{
    float x;
    float y;
    float z;
};
struct LNQuaternion
{
    float x;
    float y;
    float z;
    float w;
};
struct LNPoint
{
    float x;
    float y;
};
struct LNSize
{
    float width;
    float height;
};
struct LNRect
{
    float x;
    float y;
    float width;
    float height;
};


/**
    @brief テクスチャのピクセルフォーマット
*/
typedef enum tagLNTextureFormat
{
    /**
        @brief Unknown
    */
    LN_TEXTUREFORMAT_UNKNOWN = 0,

    /**
        @brief 32 ビットのアルファ付きフォーマット (GPUネイティブフォーマット。D3D_FMT_A8B8G8R8, DXGI_FORMAT_R8G8B8A8_UNORM)
    */
    LN_TEXTUREFORMAT_R8G8B8A8 = 1,

    /**
        @brief 32 ビットのアルファ無しフォーマット
    */
    LN_TEXTUREFORMAT_R8G8B8X8 = 2,

    /**
        @brief 64 ビットの浮動小数点フォーマット
    */
    LN_TEXTUREFORMAT_R16G16B16A16_FLOAT = 3,

    /**
        @brief 128 ビットの浮動小数点フォーマット
    */
    LN_TEXTUREFORMAT_R32G32B32A32_FLOAT = 4,

    /**
        @brief 16 ビットの浮動小数点フォーマット
    */
    LN_TEXTUREFORMAT_R16_FLOAT = 5,

    /**
        @brief 32 ビットの浮動小数点フォーマット
    */
    LN_TEXTUREFORMAT_R32_FLOAT = 6,

    /**
        @brief S8 32 ビットの深度バッファフォーマット
    */
    LN_TEXTUREFORMAT_D24S8 = 7,

} LNTextureFormat;




/**
    @brief ベクトルの長さを返します。
*/
LN_API LNResultCode LNVector3_GetLength(const LNVector3* vector3, float* outReturn);

/**
    @brief ベクトルの長さの 2 乗を返します。
*/
LN_API LNResultCode LNVector3_GetLengthSquared(const LNVector3* vector3, float* outReturn);

/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] vec : 処理の基になるベクトル
    @return 正規化されたベクトル
*/
LN_API LNResultCode LNVector3_Normalize(const LNVector3* vec, LNVector3* outReturn);

/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] x : 処理の基になるベクトルの X 要
    @param[in] y : 処理の基になるベクトルの Y 要
    @param[in] z : 処理の基になるベクトルの Z 要
    @return 正規化されたベクトル
*/
LN_API LNResultCode LNVector3_NormalizeXYZ(float x, float y, float z, LNVector3* outReturn);

/**
    @brief すべての要素を 0 で初期化します。
*/
LN_API LNResultCode LNRect_Clear(LNRect* rect);

/**
    @brief 位置とサイズを指定して初期化します。
*/
LN_API LNResultCode LNRect_Set(LNRect* rect, float x, float y, float width, float height);

/**
    @brief 左辺の x 軸の値を取得します。
*/
LN_API LNResultCode LNRect_GetLeft(const LNRect* rect, float* outReturn);

/**
    @brief 幅と高さを設定します。
*/
LN_API LNResultCode LNRect_SetSize(LNRect* rect, const LNSize* size);

/**
    @brief 幅と高さを取得します。
*/
LN_API LNResultCode LNRect_GetSize(const LNRect* rect, LNSize* outReturn);



//==============================================================================
// ln::Engine
//==============================================================================

/**
    @brief エンジンの初期化処理を行います。
*/
LN_API LNResultCode LNEngine_Initialize();

/**
    @brief エンジンの終了処理を行います。
*/
LN_API LNResultCode LNEngine_Terminate();

/**
    @brief 1フレーム分の更新処理を行います。
    @return アプリケーションの終了が要求されている場合は false を返します。
    @details この関数はグラフィックスと入力を更新し、指定されたフレームレートになるように待機します。
*/
LN_API LNResultCode LNEngine_Update(LNBool* outReturn);

//==============================================================================
// ln::Sound
//==============================================================================

/**
    @brief Sound クラスのインスタンスを作成します。
*/
LN_API LNResultCode LNSound_Create(const LNChar* filePath, LNHandle* outSound);

/**
    @brief この音声の音量を設定します。
    @param[in] volume : 音量 (0.0～1.0。初期値は 1.0)
*/
LN_API LNResultCode LNSound_SetVolume(LNHandle sound, float volume);

/**
    @brief この音声の音量を取得します。
*/
LN_API LNResultCode LNSound_GetVolume(LNHandle sound, float* outReturn);

/**
    @brief この音声のピッチ (音高) を設定します。
    @param[in] volume : ピッチ (0.5～2.0。初期値は 1.0)
*/
LN_API LNResultCode LNSound_SetPitch(LNHandle sound, float pitch);

/**
    @brief この音声のピッチ (音高) を取得します。
*/
LN_API LNResultCode LNSound_GetPitch(LNHandle sound, float* outReturn);

/**
    @brief ループ再生の有無を設定します。
    @param[in] enabled : ループ再生するか
*/
LN_API LNResultCode LNSound_SetLoopEnabled(LNHandle sound, LNBool enabled);

/**
    @brief ループ再生が有効かを確認します。
*/
LN_API LNResultCode LNSound_IsLoopEnabled(LNHandle sound, LNBool* outReturn);

/**
    @brief ループ範囲を設定します。
    @param[in] begin : ループ範囲の開始サンプル
    @param[in] length : ループ範囲のサンプル数
    @details MIDI の場合、ループ範囲はミュージックタイム単位 (四分音符ひとつ分を 768 で表す) で指定します。
*/
LN_API LNResultCode LNSound_SetLoopRange(LNHandle sound, uint32_t begin, uint32_t length);

/**
    @brief この音声の再生を開始します。
*/
LN_API LNResultCode LNSound_Play(LNHandle sound);

/**
    @brief この音声の再生を停止します。
*/
LN_API LNResultCode LNSound_Stop(LNHandle sound);

/**
    @brief この音声の再生を一時停止します。
*/
LN_API LNResultCode LNSound_Pause(LNHandle sound);

/**
    @brief 一時停止中の再生を再開します。
*/
LN_API LNResultCode LNSound_Resume(LNHandle sound);

//==============================================================================
// ln::GraphicsResourceObject
//==============================================================================

//==============================================================================
// ln::Texture
//==============================================================================

//==============================================================================
// ln::Texture2D
//==============================================================================

/**
    @brief 
*/
LN_API LNResultCode LNTexture2D_Create(int width, int height, LNTextureFormat format, LNBool mipmap, LNHandle* outTexture2D);

/**
    @brief 
*/
LN_API LNResultCode LNTexture2D_CreateFromFile(const LNChar* filePath, LNTextureFormat format, LNBool mipmap, LNHandle* outTexture2D);

//==============================================================================
// ln::WorldObject
//==============================================================================

/**
    @brief このオブジェクトの位置を設定します。
*/
LN_API LNResultCode LNWorldObject_SetPosition(LNHandle worldobject, const LNVector3* pos);

/**
    @brief このオブジェクトの位置を設定します。
*/
LN_API LNResultCode LNWorldObject_SetPositionXYZ(LNHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの位置を位置を取得します。
*/
LN_API LNResultCode LNWorldObject_GetPosition(LNHandle worldobject, LNVector3* outReturn);

/**
    @brief このオブジェクトの回転を設定します。
*/
LN_API LNResultCode LNWorldObject_SetRotation(LNHandle worldobject, const LNQuaternion* rot);

/**
    @brief このオブジェクトの回転をオイラー角から設定します。(radian)
*/
LN_API LNResultCode LNWorldObject_SetEulerAngles(LNHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの回転を取得します。
*/
LN_API LNResultCode LNWorldObject_GetRotation(LNHandle worldobject, LNQuaternion* outReturn);

/**
    @brief このオブジェクトの拡大率を設定します。
*/
LN_API LNResultCode LNWorldObject_SetScale(LNHandle worldobject, const LNVector3* scale);

/**
    @brief このオブジェクトの拡大率を設定します。
*/
LN_API LNResultCode LNWorldObject_SetScaleS(LNHandle worldobject, float xyz);

/**
    @brief このオブジェクトの拡大率を設定します。
*/
LN_API LNResultCode LNWorldObject_SetScaleXYZ(LNHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの拡大率を取得します。
*/
LN_API LNResultCode LNWorldObject_GetScale(LNHandle worldobject, LNVector3* outReturn);

/**
    @brief このオブジェクトにタグ文字列を設定します。
*/
LN_API LNResultCode LNWorldObject_SetTag(LNHandle worldobject, const LNChar* tag);

/**
    @brief このオブジェクトのタグ文字列を取得します。
*/
LN_API LNResultCode LNWorldObject_GetTag(LNHandle worldobject, const LNChar** outReturn);

/**
    @brief このオブジェクトが属するレイヤーを設定します。
*/
LN_API LNResultCode LNWorldObject_SetLayer(LNHandle worldobject, int layer);

/**
    @brief このオブジェクトが属するレイヤーを取得します。
*/
LN_API LNResultCode LNWorldObject_GetLayer(LNHandle worldobject, int* outReturn);

//==============================================================================
// ln::Component
//==============================================================================

/**
    @brief このコンポーネントが属するレイヤーを設定します。
*/
LN_API LNResultCode LNComponent_SetLayer(LNHandle component, int layer);

/**
    @brief このコンポーネントが属するレイヤーを取得します。
*/
LN_API LNResultCode LNComponent_GetLayer(LNHandle component, int* outReturn);

/**
    @brief レイヤーの中での前後関係を設定します。小さい番号ほど先に描画され、大きい番号が後から前面に描画されます。
*/
LN_API LNResultCode LNComponent_SetOrderInLayer(LNHandle component, int16_t order);

/**
    @brief レイヤーの中での前後関係を取得します。
*/
LN_API LNResultCode LNComponent_GetOrderInLayer(LNHandle component, int* outReturn);

//==============================================================================
// ln::SceneNode
//==============================================================================

/**
    @brief 可視状態を設定します。false の場合、ノードの描画自体行われません。(default: true)
*/
LN_API LNResultCode LNSceneNode_SetVisible(LNHandle scenenode, LNBool visible);

/**
    @brief 可視状態を取得します。
*/
LN_API LNResultCode LNSceneNode_IsVisible(LNHandle scenenode, LNBool* outReturn);

//==============================================================================
// ln::VisualComponent
//==============================================================================

//==============================================================================
// ln::VisualObject
//==============================================================================

//==============================================================================
// ln::SpriteComponent
//==============================================================================

/**
    @brief スプライトが表示するテクスチャを設定します。
*/
LN_API LNResultCode LNSpriteComponent_SetTexture(LNHandle spritecomponent, LNHandle texture);

/**
    @brief スプライトが表示するテクスチャを取得します。
*/
LN_API LNResultCode LNSpriteComponent_GetTexture(LNHandle spritecomponent, LNHandle* outReturn);

//==============================================================================
// ln::Sprite2DComponent
//==============================================================================

/**
    @brief 
*/
LN_API LNResultCode LNSprite2DComponent_Create(LNHandle* outSprite2DComponent);

/**
    @brief 
*/
LN_API LNResultCode LNSprite2DComponent_CreateFromTexture(LNHandle texture, LNHandle* outSprite2DComponent);

//==============================================================================
// ln::Sprite3DComponent
//==============================================================================

//==============================================================================
// ln::SpriteBase
//==============================================================================

/**
    @brief スプライトが表示するテクスチャを設定します。
*/
LN_API LNResultCode LNSpriteBase_SetTexture(LNHandle spritebase, LNHandle texture);

/**
    @brief スプライトが表示するテクスチャを取得します。
*/
LN_API LNResultCode LNSpriteBase_GetTexture(LNHandle spritebase, LNHandle* outReturn);

/**
    @brief テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) デフォルトは Rect::Empty で、テクスチャ全体を転送することを示します。
*/
LN_API LNResultCode LNSpriteBase_SetSourceRect(LNHandle spritebase, const LNRect* rect);

/**
    @brief @overload setSourceRect *
*/
LN_API LNResultCode LNSpriteBase_SetSourceRectXYWH(LNHandle spritebase, float x, float y, float width, float height);

/**
    @brief テクスチャのどの部分を表示するかを示す転送矩形を取得します。(ピクセル単位)
*/
LN_API LNResultCode LNSpriteBase_GetSourceRect(LNHandle spritebase, LNRect* outReturn);

/**
    @brief スプライトの原点を設定します。
    @details 値は (0,0) から (1,1) の間で指定します。デフォルトは (0,0) で、これはスプライトの左上が原点であることを意味します。(0.5,0.5) はスプライトの中央、(1,1) は右下が原点となります。
*/
LN_API LNResultCode LNSpriteBase_SetAnchorPoint(LNHandle spritebase, const LNVector2* ratio);

/**
    @brief @overload setAnchorPoint *
*/
LN_API LNResultCode LNSpriteBase_SetAnchorPointXY(LNHandle spritebase, float ratioX, float ratioY);

//==============================================================================
// ln::Sprite2D
//==============================================================================

/**
    @brief 
*/
LN_API LNResultCode LNSprite2D_Create(LNHandle* outSprite2D);

/**
    @brief 
*/
LN_API LNResultCode LNSprite2D_CreateFromFile(const LNChar* filePath, LNHandle* outSprite2D);

/**
    @brief 
*/
LN_API LNResultCode LNSprite2D_CreateFromTexture(LNHandle texture, LNHandle* outSprite2D);

//==============================================================================
// ln::Sprite3D
//==============================================================================

/**
    @brief 
*/
LN_API LNResultCode LNSprite3D_Create(LNHandle* outSprite3D);

/**
    @brief 
*/
LN_API LNResultCode LNSprite3D_CreateFromTexture(float width, float height, LNHandle texture, LNHandle* outSprite3D);



} // extern "C"

