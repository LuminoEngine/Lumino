
#pragma once
#include "FlatCommon.h"

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
    @brief 4次元のベクトルを定義します。
*/
struct LnVector4
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
struct LnQuaternion
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
struct LnMatrix
{
    LnVector4 row0;
    LnVector4 row1;
    LnVector4 row2;
    LnVector4 row3;
};

/**
    @brief 各要素を 0.0～1.0 の範囲で表す RGBA カラーを定義します。
*/
struct LnColor
{
    float r;
    float g;
    float b;
    float a;
};

/**
    @brief 2次元上の点を表します。
*/
struct LnPoint
{
    float x;
    float y;
};

/**
    @brief 2次元上のオブジェクトサイズを表します。
*/
struct LnSize
{
    float width;
    float height;
};

/**
    @brief 2次元の矩形を表すクラスです。
*/
struct LnRect
{
    float x;
    float y;
    float width;
    float height;
};

/**
    @brief 四角形の枠の太さを表すクラスです。
*/
struct LnThickness
{
    float left;
    float top;
    float right;
    float bottom;
};



/**
    @brief マウスボタン
*/
typedef enum tagLnMouseButtons
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

} LnMouseButtons;

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

/**
    @brief 縦方向の表示位置を示します。
*/
typedef enum tagLnVAlignment
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

} LnVAlignment;

/**
    @brief 横方向の表示位置を示します。
*/
typedef enum tagLnHAlignment
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

} LnHAlignment;


typedef LnResult(*LnPromiseFailureDelegateCallback)(LnHandle promisefailuredelegate);
typedef LnResult(*LnZVTestDelegate1Callback)(LnHandle zvtestdelegate1, int p1);
typedef LnResult(*LnZVTestDelegate2Callback)(LnHandle zvtestdelegate2, int p1, int p2, int* outReturn);
typedef LnResult(*LnZVTestDelegate3Callback)(LnHandle zvtestdelegate3, LnHandle p1);
typedef LnResult(*LnZVTestEventHandler1Callback)(LnHandle zvtesteventhandler1);
typedef LnResult(*LnZVTestEventHandler2Callback)(LnHandle zvtesteventhandler2, LnHandle p1);
typedef LnResult(*LnTestDelegateCallback)(LnHandle testdelegate, int p1, int* outReturn);
typedef LnResult(*LnUIGeneralEventHandlerCallback)(LnHandle uigeneraleventhandler, LnHandle p1);
typedef LnResult(*LnUIEventHandlerCallback)(LnHandle uieventhandler);


//==============================================================================
// ln::Vector3

/**
    @brief すべての要素を 0.0 に設定してインスタンスを初期化します。
    @param[in] vector3 : instance
*/
LN_FLAT_API LnResult LnVector3_SetZeros(LnVector3* vector3);


/**
    @brief 指定した値を使用してインスタンスを初期化します。
    @param[in] vector3 : instance
*/
LN_FLAT_API LnResult LnVector3_Set(LnVector3* vector3, float x, float y, float z);


/**
    @brief ベクトルの長さを返します。
    @param[in] vector3 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnVector3_Length(const LnVector3* vector3, float* outReturn);


/**
    @brief ベクトルの長さの 2 乗を返します。
    @param[in] vector3 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnVector3_LengthSquared(const LnVector3* vector3, float* outReturn);


/**
    @brief このベクトルを正規化します。
    @param[in] vector3 : instance
    @details ベクトルの長さが 0 の場合は正規化を行いません。
*/
LN_FLAT_API LnResult LnVector3_MutatingNormalize(LnVector3* vector3);


/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] x : 処理の基になるベクトルの X 要
    @param[in] y : 処理の基になるベクトルの Y 要
    @param[in] z : 処理の基になるベクトルの Z 要
    @param[out] outReturn : instance.
    @return 正規化されたベクトル
*/
LN_FLAT_API LnResult LnVector3_NormalizeXYZ(float x, float y, float z, LnVector3* outReturn);


/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] vec : 処理の基になるベクトル
    @param[out] outReturn : instance.
    @return 正規化されたベクトル
*/
LN_FLAT_API LnResult LnVector3_Normalize(const LnVector3* vec, LnVector3* outReturn);


//==============================================================================
// ln::Vector4

/**
    @brief すべての要素を 0.0 に設定してインスタンスを初期化します。
    @param[in] vector4 : instance
*/
LN_FLAT_API LnResult LnVector4_SetZeros(LnVector4* vector4);


/**
    @brief 指定した値を使用してインスタンスを初期化します。
    @param[in] vector4 : instance
*/
LN_FLAT_API LnResult LnVector4_Set(LnVector4* vector4, float x, float y, float z, float w);


//==============================================================================
// ln::Quaternion

/**
    @brief 単位クォータニオンを設定してインスタンスを初期化します。
    @param[in] quaternion : instance
*/
LN_FLAT_API LnResult LnQuaternion_SetZeros(LnQuaternion* quaternion);


/**
    @brief 指定した値を使用してインスタンスを初期化します。
    @param[in] quaternion : instance
*/
LN_FLAT_API LnResult LnQuaternion_Set(LnQuaternion* quaternion, float x, float y, float z, float w);


/**
    @brief 回転軸と回転角度を指定してインスタンスを初期化します。
    @param[in] quaternion : instance
    @param[in] axis : 回転軸
    @param[in] r : 回転角度 (ラジアン単位)
    @details axis が単位ベクトルでなければ正規化してから計算を行います。
*/
LN_FLAT_API LnResult LnQuaternion_SetFromAxis(LnQuaternion* quaternion, const LnVector3* axis, float r);


//==============================================================================
// ln::Matrix

/**
    @brief 単位行列を設定してインスタンスを初期化します。
    @param[in] matrix : instance
*/
LN_FLAT_API LnResult LnMatrix_SetZeros(LnMatrix* matrix);


/**
    @brief 各要素を指定してインスタンスを初期化します。
    @param[in] matrix : instance
*/
LN_FLAT_API LnResult LnMatrix_Set(LnMatrix* matrix, float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);


//==============================================================================
// ln::Color

/**
    @brief すべての要素を 0.0 で初期化します。
    @param[in] color : instance
*/
LN_FLAT_API LnResult LnColor_SetZeros(LnColor* color);


/**
    @brief 各要素を指定して初期化します。
    @param[in] color : instance
*/
LN_FLAT_API LnResult LnColor_Set(LnColor* color, float r_, float g_, float b_, float a_);


//==============================================================================
// ln::Point

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] point : instance
*/
LN_FLAT_API LnResult LnPoint_SetZeros(LnPoint* point);


/**
    @brief 位置を指定して初期化します。
    @param[in] point : instance
*/
LN_FLAT_API LnResult LnPoint_Set(LnPoint* point, float x_, float y_);


//==============================================================================
// ln::Size

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] size : instance
*/
LN_FLAT_API LnResult LnSize_SetZeros(LnSize* size);


/**
    @brief 幅と高さを指定して初期化します。
    @param[in] size : instance
*/
LN_FLAT_API LnResult LnSize_Set(LnSize* size, float w, float h);


//==============================================================================
// ln::Rect

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] rect : instance
*/
LN_FLAT_API LnResult LnRect_SetZeros(LnRect* rect);


/**
    @brief 位置とサイズを指定して初期化します。
    @param[in] rect : instance
*/
LN_FLAT_API LnResult LnRect_Set(LnRect* rect, float x, float y, float width, float height);


/**
    @brief 左辺の x 軸の値を取得します。
    @param[in] rect : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnRect_GetLeft(const LnRect* rect, float* outReturn);


/**
    @brief 幅と高さを設定します。
    @param[in] rect : instance
*/
LN_FLAT_API LnResult LnRect_SetSize(LnRect* rect, const LnSize* size);


/**
    @brief 幅と高さを取得します。
    @param[in] rect : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnRect_GetSize(const LnRect* rect, LnSize* outReturn);


//==============================================================================
// ln::Thickness

/**
    @brief すべての要素を 0 で初期化します。
    @param[in] thickness : instance
*/
LN_FLAT_API LnResult LnThickness_SetZeros(LnThickness* thickness);


/**
    @brief 各辺の幅を指定して初期化します。
    @param[in] thickness : instance
*/
LN_FLAT_API LnResult LnThickness_Set(LnThickness* thickness, float left_, float top_, float right_, float bottom_);




//==============================================================================
// ln::Object

typedef LnResult(*LnObject_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnObject_OnSerialize_SetOverrideCallback(LnObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnObject_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnObject_GetTypeInfoId();
LN_FLAT_API void LnObject_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::EventConnection

typedef LnResult(*LnEventConnection_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnEventConnection_OnSerialize_SetOverrideCallback(LnEventConnection_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnEventConnection_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnEventConnection_GetTypeInfoId();
LN_FLAT_API void LnEventConnection_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::PromiseFailureDelegate

LN_FLAT_API LnResult LnPromiseFailureDelegate_Create(LnPromiseFailureDelegateCallback callback, LnHandle* outDelegate);
LN_FLAT_API void LnPromiseFailureDelegate_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::ZVTestDelegate1

LN_FLAT_API LnResult LnZVTestDelegate1_Create(LnZVTestDelegate1Callback callback, LnHandle* outDelegate);
LN_FLAT_API void LnZVTestDelegate1_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::ZVTestDelegate2

LN_FLAT_API LnResult LnZVTestDelegate2_Create(LnZVTestDelegate2Callback callback, LnHandle* outDelegate);
LN_FLAT_API void LnZVTestDelegate2_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::ZVTestDelegate3

LN_FLAT_API LnResult LnZVTestDelegate3_Create(LnZVTestDelegate3Callback callback, LnHandle* outDelegate);
LN_FLAT_API void LnZVTestDelegate3_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::ZVTestEventHandler1

LN_FLAT_API LnResult LnZVTestEventHandler1_Create(LnZVTestEventHandler1Callback callback, LnHandle* outDelegate);
LN_FLAT_API void LnZVTestEventHandler1_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::ZVTestEventHandler2

LN_FLAT_API LnResult LnZVTestEventHandler2_Create(LnZVTestEventHandler2Callback callback, LnHandle* outDelegate);
LN_FLAT_API void LnZVTestEventHandler2_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::ZVTestPromise1

/**
    @brief 
    @param[in] zvtestpromise1 : instance
*/
LN_FLAT_API LnResult LnZVTestPromise1_ThenWith(LnHandle zvtestpromise1, LnHandle callback);

/**
    @brief 
    @param[in] zvtestpromise1 : instance
*/
LN_FLAT_API LnResult LnZVTestPromise1_CatchWith(LnHandle zvtestpromise1, LnHandle callback);

extern LN_FLAT_API int LnZVTestPromise1_GetTypeInfoId();
LN_FLAT_API void LnZVTestPromise1_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::ZVTestPromise2

/**
    @brief 
    @param[in] zvtestpromise2 : instance
*/
LN_FLAT_API LnResult LnZVTestPromise2_ThenWith(LnHandle zvtestpromise2, LnHandle callback);

/**
    @brief 
    @param[in] zvtestpromise2 : instance
*/
LN_FLAT_API LnResult LnZVTestPromise2_CatchWith(LnHandle zvtestpromise2, LnHandle callback);

extern LN_FLAT_API int LnZVTestPromise2_GetTypeInfoId();
LN_FLAT_API void LnZVTestPromise2_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::ZVTestClass1

/**
    @brief setTestDelegate1 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LnResult LnZVTestClass1_SetTestDelegate1(LnHandle zvtestclass1, LnHandle value);

/**
    @brief setTestDelegate2 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LnResult LnZVTestClass1_SetTestDelegate2(LnHandle zvtestclass1, LnHandle value);

/**
    @brief setTestDelegate3 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LnResult LnZVTestClass1_SetTestDelegate3(LnHandle zvtestclass1, LnHandle value);

/**
    @brief callTestDelegate1 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LnResult LnZVTestClass1_CallTestDelegate1(LnHandle zvtestclass1, int a);

/**
    @brief callTestDelegate2 method.
    @param[in] zvtestclass1 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnZVTestClass1_CallTestDelegate2(LnHandle zvtestclass1, int a, int b, int* outReturn);

/**
    @brief callTestDelegate3 method. (create object in internal)
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LnResult LnZVTestClass1_CallTestDelegate3(LnHandle zvtestclass1);

/**
    @brief Promise test method. (static)
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LnResult LnZVTestClass1_LoadAsync(const LnChar* filePath, LnHandle* outReturn);
LN_FLAT_API LnResult LnZVTestClass1_LoadAsyncA(const char* filePath, LnHandle* outReturn);

/**
    @brief Promise test method. (instance)
    @param[in] zvtestclass1 : instance
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LnResult LnZVTestClass1_ExecuteAsync(LnHandle zvtestclass1, LnHandle* outReturn);

/**
    @brief Promise test method.
    @param[in] zvtestclass1 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnZVTestClass1_GetFilePath(LnHandle zvtestclass1, const LnChar** outReturn);
LN_FLAT_API LnResult LnZVTestClass1_GetFilePathA(LnHandle zvtestclass1, const char** outReturn);

/**
    @brief connectOnEvent1 method.
    @param[in] zvtestclass1 : instance
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LnResult LnZVTestClass1_ConnectOnEvent1(LnHandle zvtestclass1, LnHandle handler, LnHandle* outReturn);

/**
    @brief raiseEvent1 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LnResult LnZVTestClass1_RaiseEvent1(LnHandle zvtestclass1);

/**
    @brief connectOnEvent2 method.
    @param[in] zvtestclass1 : instance
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LnResult LnZVTestClass1_ConnectOnEvent2(LnHandle zvtestclass1, LnHandle handler, LnHandle* outReturn);

/**
    @brief raiseEvent2 method.
    @param[in] zvtestclass1 : instance
*/
LN_FLAT_API LnResult LnZVTestClass1_RaiseEvent2(LnHandle zvtestclass1);

/**
    @brief init method.
    @param[out] outZVTestClass1 : instance.
*/
LN_FLAT_API LnResult LnZVTestClass1_Create(LnHandle* outZVTestClass1);

typedef LnResult(*LnZVTestClass1_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnZVTestClass1_OnSerialize_SetOverrideCallback(LnZVTestClass1_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnZVTestClass1_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnZVTestClass1_GetTypeInfoId();
LN_FLAT_API void LnZVTestClass1_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::ZVTestEventArgs1

/**
    @brief value method.
    @param[in] zvtesteventargs1 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnZVTestEventArgs1_GetValue(LnHandle zvtesteventargs1, int* outReturn);

/**
    @brief init method.
    @param[out] outZVTestEventArgs1 : instance.
*/
LN_FLAT_API LnResult LnZVTestEventArgs1_Create(LnHandle* outZVTestEventArgs1);

/**
    @brief init method.
    @param[out] outZVTestEventArgs1 : instance.
*/
LN_FLAT_API LnResult LnZVTestEventArgs1_CreateWithValue(int v, LnHandle* outZVTestEventArgs1);

typedef LnResult(*LnZVTestEventArgs1_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnZVTestEventArgs1_OnSerialize_SetOverrideCallback(LnZVTestEventArgs1_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnZVTestEventArgs1_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnZVTestEventArgs1_GetTypeInfoId();
LN_FLAT_API void LnZVTestEventArgs1_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::Serializer

/**
    @brief write
    @param[in] serializer : instance
*/
LN_FLAT_API LnResult LnSerializer_WriteBool(LnHandle serializer, const LnChar* name, LnBool value);
LN_FLAT_API LnResult LnSerializer_WriteBoolA(LnHandle serializer, const char* name, LnBool value);

/**
    @brief write
    @param[in] serializer : instance
*/
LN_FLAT_API LnResult LnSerializer_WriteInt(LnHandle serializer, const LnChar* name, int value);
LN_FLAT_API LnResult LnSerializer_WriteIntA(LnHandle serializer, const char* name, int value);

/**
    @brief write
    @param[in] serializer : instance
*/
LN_FLAT_API LnResult LnSerializer_WriteFloat(LnHandle serializer, const LnChar* name, float value);
LN_FLAT_API LnResult LnSerializer_WriteFloatA(LnHandle serializer, const char* name, float value);

/**
    @brief write
    @param[in] serializer : instance
*/
LN_FLAT_API LnResult LnSerializer_WriteString(LnHandle serializer, const LnChar* name, const LnChar* value);
LN_FLAT_API LnResult LnSerializer_WriteStringA(LnHandle serializer, const char* name, const char* value);

/**
    @brief write
    @param[in] serializer : instance
*/
LN_FLAT_API LnResult LnSerializer_WriteObject(LnHandle serializer, const LnChar* name, LnHandle value);
LN_FLAT_API LnResult LnSerializer_WriteObjectA(LnHandle serializer, const char* name, LnHandle value);

/**
    @brief read
    @param[in] serializer : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSerializer_ReadBool(LnHandle serializer, const LnChar* name, LnBool* outReturn);
LN_FLAT_API LnResult LnSerializer_ReadBoolA(LnHandle serializer, const char* name, LnBool* outReturn);

/**
    @brief read
    @param[in] serializer : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSerializer_ReadInt(LnHandle serializer, const LnChar* name, int* outReturn);
LN_FLAT_API LnResult LnSerializer_ReadIntA(LnHandle serializer, const char* name, int* outReturn);

/**
    @brief read
    @param[in] serializer : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSerializer_ReadFloat(LnHandle serializer, const LnChar* name, float* outReturn);
LN_FLAT_API LnResult LnSerializer_ReadFloatA(LnHandle serializer, const char* name, float* outReturn);

/**
    @brief read
    @param[in] serializer : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSerializer_ReadString(LnHandle serializer, const LnChar* name, const LnChar** outReturn);
LN_FLAT_API LnResult LnSerializer_ReadStringA(LnHandle serializer, const char* name, const char** outReturn);

/**
    @brief read
    @param[in] serializer : instance
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LnResult LnSerializer_ReadObject(LnHandle serializer, const LnChar* name, LnHandle* outReturn);
LN_FLAT_API LnResult LnSerializer_ReadObjectA(LnHandle serializer, const char* name, LnHandle* outReturn);

/**
    @brief serialize
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSerializer_Serialize(LnHandle value, const LnChar* basePath, const LnChar** outReturn);
LN_FLAT_API LnResult LnSerializer_SerializeA(LnHandle value, const char* basePath, const char** outReturn);

/**
    @brief serialize
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LnResult LnSerializer_Deserialize(const LnChar* str, const LnChar* basePath, LnHandle* outReturn);
LN_FLAT_API LnResult LnSerializer_DeserializeA(const char* str, const char* basePath, LnHandle* outReturn);

typedef LnResult(*LnSerializer_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnSerializer_OnSerialize_SetOverrideCallback(LnSerializer_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnSerializer_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnSerializer_GetTypeInfoId();
LN_FLAT_API void LnSerializer_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::AssetModel

/**
    @brief target
    @param[in] assetmodel : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnAssetModel_Target(LnHandle assetmodel, LnHandle* outReturn);

/**
    @brief init
    @param[out] outAssetModel : instance.
*/
LN_FLAT_API LnResult LnAssetModel_Create(LnHandle target, LnHandle* outAssetModel);

typedef LnResult(*LnAssetModel_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnAssetModel_OnSerialize_SetOverrideCallback(LnAssetModel_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnAssetModel_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnAssetModel_GetTypeInfoId();
LN_FLAT_API void LnAssetModel_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::Assets

/**
    @brief Internal
*/
LN_FLAT_API LnResult LnAssets_SaveAssetToLocalFile(LnHandle asset, const LnChar* filePath);
LN_FLAT_API LnResult LnAssets_SaveAssetToLocalFileA(LnHandle asset, const char* filePath);

/**
    @brief Internal
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LnResult LnAssets_LoadAssetFromLocalFile(const LnChar* filePath, LnHandle* outReturn);
LN_FLAT_API LnResult LnAssets_LoadAssetFromLocalFileA(const char* filePath, LnHandle* outReturn);

/**
    @brief 指定したアセットファイルを読み込み、オブジェクト生成します。
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
    @details ファイルの拡張子は .lnasset です。ただし、filePath に指定する値は拡張子を省略可能です。
*/
LN_FLAT_API LnResult LnAssets_LoadAsset(const LnChar* filePath, LnHandle* outReturn);
LN_FLAT_API LnResult LnAssets_LoadAssetA(const char* filePath, LnHandle* outReturn);


//==============================================================================
// ln::GraphicsResource

typedef LnResult(*LnGraphicsResource_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnGraphicsResource_OnSerialize_SetOverrideCallback(LnGraphicsResource_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnGraphicsResource_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnGraphicsResource_GetTypeInfoId();
LN_FLAT_API void LnGraphicsResource_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::Texture

typedef LnResult(*LnTexture_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnTexture_OnSerialize_SetOverrideCallback(LnTexture_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnTexture_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnTexture_GetTypeInfoId();
LN_FLAT_API void LnTexture_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::Texture2D

/**
    @brief アセットからテクスチャを読み込みます。
    @param[in] filePath : 読み込むファイルのパス
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
    @details サポートしているフォーマットは次の通りです。PNG(.png), JPG(.jpg), TGA(.tga), BMP(.bmp), GIF(.gif)
*/
LN_FLAT_API LnResult LnTexture2D_Load(const LnChar* filePath, LnHandle* outReturn);
LN_FLAT_API LnResult LnTexture2D_LoadA(const char* filePath, LnHandle* outReturn);

/**
    @brief loadEmoji
    @param[in] code : xxxx
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LnResult LnTexture2D_LoadEmoji(const LnChar* code, LnHandle* outReturn);
LN_FLAT_API LnResult LnTexture2D_LoadEmojiA(const char* code, LnHandle* outReturn);

/**
    @brief テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    @param[in] width : 幅 (px 単位)
    @param[in] height : 高さ (px 単位)
    @param[out] outTexture2D : instance.
    @return 作成されたテクスチャ
*/
LN_FLAT_API LnResult LnTexture2D_Create(int width, int height, LnHandle* outTexture2D);

/**
    @brief テクスチャを作成します。
    @param[in] width : 幅 (px 単位)
    @param[in] height : 高さ (px 単位)
    @param[in] format : ピクセルフォーマット
    @param[out] outTexture2D : instance.
*/
LN_FLAT_API LnResult LnTexture2D_CreateWithFormat(int width, int height, LnTextureFormat format, LnHandle* outTexture2D);

/**
    @brief ローカルのファイルを読み込み、テクスチャを作成します。
    @param[in] filePath : 読み込むファイルのパス
    @param[in] format : ピクセルフォーマット
    @param[out] outTexture2D : instance.
    @details このメソッドは TextureImporter のユーティリティです。
*/
LN_FLAT_API LnResult LnTexture2D_CreateFromFile(const LnChar* filePath, LnTextureFormat format, LnHandle* outTexture2D);
LN_FLAT_API LnResult LnTexture2D_CreateFromFileA(const char* filePath, LnTextureFormat format, LnHandle* outTexture2D);

typedef LnResult(*LnTexture2D_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnTexture2D_OnSerialize_SetOverrideCallback(LnTexture2D_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnTexture2D_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnTexture2D_GetTypeInfoId();
LN_FLAT_API void LnTexture2D_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::RenderView

typedef LnResult(*LnRenderView_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnRenderView_OnSerialize_SetOverrideCallback(LnRenderView_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnRenderView_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnRenderView_GetTypeInfoId();
LN_FLAT_API void LnRenderView_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::Component

typedef LnResult(*LnComponent_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnComponent_OnSerialize_SetOverrideCallback(LnComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnComponent_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnComponent_GetTypeInfoId();
LN_FLAT_API void LnComponent_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::VisualComponent

/**
    @brief 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    @param[in] visualcomponent : instance
*/
LN_FLAT_API LnResult LnVisualComponent_SetVisible(LnHandle visualcomponent, LnBool value);

/**
    @brief 可視状態を取得します。
    @param[in] visualcomponent : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnVisualComponent_IsVisible(LnHandle visualcomponent, LnBool* outReturn);

typedef LnResult(*LnVisualComponent_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnVisualComponent_OnSerialize_SetOverrideCallback(LnVisualComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnVisualComponent_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnVisualComponent_GetTypeInfoId();
LN_FLAT_API void LnVisualComponent_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::SpriteComponent

/**
    @brief スプライトが表示するテクスチャを設定します。
    @param[in] spritecomponent : instance
*/
LN_FLAT_API LnResult LnSpriteComponent_SetTexture(LnHandle spritecomponent, LnHandle texture);

typedef LnResult(*LnSpriteComponent_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnSpriteComponent_OnSerialize_SetOverrideCallback(LnSpriteComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnSpriteComponent_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnSpriteComponent_GetTypeInfoId();
LN_FLAT_API void LnSpriteComponent_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::World

/**
    @brief オブジェクトを World に追加します。
    @param[in] world : instance
*/
LN_FLAT_API LnResult LnWorld_Add(LnHandle world, LnHandle obj);

typedef LnResult(*LnWorld_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnWorld_OnSerialize_SetOverrideCallback(LnWorld_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnWorld_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnWorld_GetTypeInfoId();
LN_FLAT_API void LnWorld_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::ComponentList

/**
    @brief 
    @param[in] componentlist : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnComponentList_GetLength(LnHandle componentlist, int* outReturn);

/**
    @brief 
    @param[in] componentlist : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnComponentList_GetItem(LnHandle componentlist, int index, LnHandle* outReturn);

typedef LnResult(*LnComponentList_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnComponentList_OnSerialize_SetOverrideCallback(LnComponentList_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnComponentList_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnComponentList_GetTypeInfoId();
LN_FLAT_API void LnComponentList_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::WorldObject

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetPosition(LnHandle worldobject, const LnVector3* pos);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetPositionXYZ(LnHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの位置を位置を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnWorldObject_GetPosition(LnHandle worldobject, LnVector3* outReturn);

/**
    @brief このオブジェクトの回転を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetRotationQuaternion(LnHandle worldobject, const LnQuaternion* rot);

/**
    @brief このオブジェクトの回転をオイラー角から設定します。(radian)
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetRotation(LnHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの回転を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnWorldObject_GetRotation(LnHandle worldobject, LnQuaternion* outReturn);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetScale(LnHandle worldobject, const LnVector3* scale);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetScaleS(LnHandle worldobject, float xyz);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetScaleXYZ(LnHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの拡大率を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnWorldObject_GetScale(LnHandle worldobject, LnVector3* outReturn);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetCenterPoint(LnHandle worldobject, const LnVector3* value);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetCenterPointXYZ(LnHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトのローカルの中心位置を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnWorldObject_GetCenterPoint(LnHandle worldobject, LnVector3* outReturn);

/**
    @brief 指定した座標を向くように、オブジェクトを回転させます。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_LookAt(LnHandle worldobject, const LnVector3* target);

/**
    @brief 指定した座標を向くように、オブジェクトを回転させます。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_LookAtXYZ(LnHandle worldobject, float x, float y, float z);

/**
    @brief Component を追加します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_AddComponent(LnHandle worldobject, LnHandle component);

/**
    @brief 
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnWorldObject_GetComponents(LnHandle worldobject, LnHandle* outReturn);

typedef LnResult(*LnWorldObject_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnWorldObject_OnSerialize_SetOverrideCallback(LnWorldObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnWorldObject_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnWorldObject_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnWorldObject_OnUpdate_SetOverrideCallback(LnWorldObject_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnWorldObject_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

extern LN_FLAT_API int LnWorldObject_GetTypeInfoId();
LN_FLAT_API void LnWorldObject_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::VisualObject

/**
    @brief 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    @param[in] visualobject : instance
*/
LN_FLAT_API LnResult LnVisualObject_SetVisible(LnHandle visualobject, LnBool value);

/**
    @brief 可視状態を取得します。
    @param[in] visualobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnVisualObject_IsVisible(LnHandle visualobject, LnBool* outReturn);

typedef LnResult(*LnVisualObject_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnVisualObject_OnSerialize_SetOverrideCallback(LnVisualObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnVisualObject_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnVisualObject_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnVisualObject_OnUpdate_SetOverrideCallback(LnVisualObject_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnVisualObject_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

extern LN_FLAT_API int LnVisualObject_GetTypeInfoId();
LN_FLAT_API void LnVisualObject_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::Camera

typedef LnResult(*LnCamera_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnCamera_OnSerialize_SetOverrideCallback(LnCamera_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnCamera_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnCamera_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnCamera_OnUpdate_SetOverrideCallback(LnCamera_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnCamera_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

extern LN_FLAT_API int LnCamera_GetTypeInfoId();
LN_FLAT_API void LnCamera_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::DirectionalLight

/**
    @brief ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    @param[in] directionallight : instance
*/
LN_FLAT_API LnResult LnDirectionalLight_SetEnabled(LnHandle directionallight, LnBool enabled);

/**
    @brief ライトの有効状態を取得します。
    @param[in] directionallight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnDirectionalLight_IsEnabled(LnHandle directionallight, LnBool* outReturn);

/**
    @brief ライトカラーを設定します。(default: White)
    @param[in] directionallight : instance
*/
LN_FLAT_API LnResult LnDirectionalLight_SetColor(LnHandle directionallight, const LnColor* color);

/**
    @brief ライトカラーを取得します。
    @param[in] directionallight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnDirectionalLight_GetColor(LnHandle directionallight, LnColor* outReturn);

/**
    @brief ライトの明るさを設定します。(default: 0.5)
    @param[in] directionallight : instance
*/
LN_FLAT_API LnResult LnDirectionalLight_SetIntensity(LnHandle directionallight, float intensity);

/**
    @brief ライトの明るさを取得します。
    @param[in] directionallight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnDirectionalLight_GetIntensity(LnHandle directionallight, float* outReturn);

/**
    @brief 既定の設定でディレクショナルライトを作成します。
    @param[out] outDirectionalLight : instance.
*/
LN_FLAT_API LnResult LnDirectionalLight_Create(LnHandle* outDirectionalLight);

/**
    @brief 色を指定してディレクショナルライトを作成します。
    @param[out] outDirectionalLight : instance.
*/
LN_FLAT_API LnResult LnDirectionalLight_CreateWithColor(const LnColor* color, LnHandle* outDirectionalLight);

typedef LnResult(*LnDirectionalLight_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnDirectionalLight_OnSerialize_SetOverrideCallback(LnDirectionalLight_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnDirectionalLight_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnDirectionalLight_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnDirectionalLight_OnUpdate_SetOverrideCallback(LnDirectionalLight_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnDirectionalLight_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

extern LN_FLAT_API int LnDirectionalLight_GetTypeInfoId();
LN_FLAT_API void LnDirectionalLight_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::PointLight

/**
    @brief ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    @param[in] pointlight : instance
*/
LN_FLAT_API LnResult LnPointLight_SetEnabled(LnHandle pointlight, LnBool enabled);

/**
    @brief ライトの有効状態を取得します。
    @param[in] pointlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnPointLight_IsEnabled(LnHandle pointlight, LnBool* outReturn);

/**
    @brief ライトカラーを設定します。(default: White)
    @param[in] pointlight : instance
*/
LN_FLAT_API LnResult LnPointLight_SetColor(LnHandle pointlight, const LnColor* color);

/**
    @brief ライトカラーを取得します。
    @param[in] pointlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnPointLight_GetColor(LnHandle pointlight, LnColor* outReturn);

/**
    @brief ライトの明るさを設定します。(default: 1.0)
    @param[in] pointlight : instance
*/
LN_FLAT_API LnResult LnPointLight_SetIntensity(LnHandle pointlight, float intensity);

/**
    @brief ライトの明るさを取得します。
    @param[in] pointlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnPointLight_GetIntensity(LnHandle pointlight, float* outReturn);

/**
    @brief ライトの影響範囲を設定します。(default: 10.0)
    @param[in] pointlight : instance
*/
LN_FLAT_API LnResult LnPointLight_SetRange(LnHandle pointlight, float range);

/**
    @brief ライトの影響範囲を取得します。
    @param[in] pointlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnPointLight_GetRange(LnHandle pointlight, float* outReturn);

/**
    @brief ライトの減衰を設定します。(default: 1.0)
    @param[in] pointlight : instance
*/
LN_FLAT_API LnResult LnPointLight_SetAttenuation(LnHandle pointlight, float attenuation);

/**
    @brief ライトの減衰を取得します。
    @param[in] pointlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnPointLight_GetAttenuation(LnHandle pointlight, float* outReturn);

/**
    @brief 既定の設定でポイントライトを作成します。
    @param[out] outPointLight : instance.
*/
LN_FLAT_API LnResult LnPointLight_Create(LnHandle* outPointLight);

/**
    @brief 色と範囲を指定してポイントライトを作成します。
    @param[out] outPointLight : instance.
*/
LN_FLAT_API LnResult LnPointLight_CreateWithColorAndRange(const LnColor* color, float range, LnHandle* outPointLight);

typedef LnResult(*LnPointLight_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnPointLight_OnSerialize_SetOverrideCallback(LnPointLight_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnPointLight_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnPointLight_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnPointLight_OnUpdate_SetOverrideCallback(LnPointLight_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnPointLight_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

extern LN_FLAT_API int LnPointLight_GetTypeInfoId();
LN_FLAT_API void LnPointLight_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::SpotLight

/**
    @brief ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
    @param[in] spotlight : instance
*/
LN_FLAT_API LnResult LnSpotLight_SetEnabled(LnHandle spotlight, LnBool enabled);

/**
    @brief ライトの有効状態を取得します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSpotLight_IsEnabled(LnHandle spotlight, LnBool* outReturn);

/**
    @brief ライトカラーを設定します。(default: White)
    @param[in] spotlight : instance
*/
LN_FLAT_API LnResult LnSpotLight_SetColor(LnHandle spotlight, const LnColor* color);

/**
    @brief ライトカラーを取得します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSpotLight_GetColor(LnHandle spotlight, LnColor* outReturn);

/**
    @brief ライトの明るさを設定します。(default: 1.0)
    @param[in] spotlight : instance
*/
LN_FLAT_API LnResult LnSpotLight_SetIntensity(LnHandle spotlight, float intensity);

/**
    @brief ライトの明るさを取得します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSpotLight_GetIntensity(LnHandle spotlight, float* outReturn);

/**
    @brief ライトの影響範囲を設定します。(default: 10.0)
    @param[in] spotlight : instance
*/
LN_FLAT_API LnResult LnSpotLight_SetRange(LnHandle spotlight, float range);

/**
    @brief ライトの影響範囲を取得します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSpotLight_GetRange(LnHandle spotlight, float* outReturn);

/**
    @brief ライトの減衰を設定します。(default: 1.0)
    @param[in] spotlight : instance
*/
LN_FLAT_API LnResult LnSpotLight_SetAttenuation(LnHandle spotlight, float attenuation);

/**
    @brief ライトの減衰を取得します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSpotLight_GetAttenuation(LnHandle spotlight, float* outReturn);

/**
    @brief スポットライトのコーン角度を設定します。(ラジアン単位、default: PI / 3)
    @param[in] spotlight : instance
*/
LN_FLAT_API LnResult LnSpotLight_SetAngle(LnHandle spotlight, float angle);

/**
    @brief スポットライトのコーン角度を取得します。(ラジアン単位)
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSpotLight_GetAngle(LnHandle spotlight, float* outReturn);

/**
    @brief スポットライトのコーン角度に対する減衰率を設定します。(0..1, default: 0)
    @param[in] spotlight : instance
*/
LN_FLAT_API LnResult LnSpotLight_SetPenumbra(LnHandle spotlight, float penumbra);

/**
    @brief スポットライトのコーン角度に対する減衰率を設定します。
    @param[in] spotlight : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSpotLight_GetPenumbra(LnHandle spotlight, float* outReturn);

/**
    @brief 既定の設定でスポットライトを作成します。
    @param[out] outSpotLight : instance.
*/
LN_FLAT_API LnResult LnSpotLight_Create(LnHandle* outSpotLight);

/**
    @brief 色と範囲を指定してスポットライトを作成します。
    @param[out] outSpotLight : instance.
*/
LN_FLAT_API LnResult LnSpotLight_CreateWithColorAndRange(const LnColor* color, float range, float angle, LnHandle* outSpotLight);

typedef LnResult(*LnSpotLight_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnSpotLight_OnSerialize_SetOverrideCallback(LnSpotLight_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnSpotLight_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnSpotLight_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnSpotLight_OnUpdate_SetOverrideCallback(LnSpotLight_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnSpotLight_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

extern LN_FLAT_API int LnSpotLight_GetTypeInfoId();
LN_FLAT_API void LnSpotLight_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::TestDelegate

LN_FLAT_API LnResult LnTestDelegate_Create(LnTestDelegateCallback callback, LnHandle* outDelegate);
LN_FLAT_API void LnTestDelegate_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::Sprite

/**
    @brief スプライトが表示するテクスチャを設定します。
    @param[in] sprite : instance
*/
LN_FLAT_API LnResult LnSprite_SetTexture(LnHandle sprite, LnHandle value);

/**
    @brief スプライトの大きさを設定します。
    @param[in] sprite : instance
*/
LN_FLAT_API LnResult LnSprite_SetSize(LnHandle sprite, const LnSize* value);

/**
    @brief スプライトの大きさを設定します。
    @param[in] sprite : instance
*/
LN_FLAT_API LnResult LnSprite_SetSizeWH(LnHandle sprite, float width, float height);

/**
    @brief 
    @param[in] sprite : instance
*/
LN_FLAT_API LnResult LnSprite_SetSourceRectXYWH(LnHandle sprite, float x, float y, float width, float height);

/**
    @brief test
    @param[in] sprite : instance
*/
LN_FLAT_API LnResult LnSprite_SetCallerTest(LnHandle sprite, LnHandle callback);

/**
    @brief init
    @param[out] outSprite : instance.
*/
LN_FLAT_API LnResult LnSprite_Create(LnHandle* outSprite);

/**
    @brief init
    @param[out] outSprite : instance.
*/
LN_FLAT_API LnResult LnSprite_CreateWithTexture(LnHandle texture, LnHandle* outSprite);

/**
    @brief init
    @param[out] outSprite : instance.
*/
LN_FLAT_API LnResult LnSprite_CreateWithTextureAndSize(LnHandle texture, float width, float height, LnHandle* outSprite);

typedef LnResult(*LnSprite_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnSprite_OnSerialize_SetOverrideCallback(LnSprite_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnSprite_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnSprite_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnSprite_OnUpdate_SetOverrideCallback(LnSprite_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnSprite_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

extern LN_FLAT_API int LnSprite_GetTypeInfoId();
LN_FLAT_API void LnSprite_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::CameraOrbitControlComponent

/**
    @brief CameraOrbitControlComponent を作成します。
    @param[out] outCameraOrbitControlComponent : instance.
*/
LN_FLAT_API LnResult LnCameraOrbitControlComponent_Create(LnHandle* outCameraOrbitControlComponent);

typedef LnResult(*LnCameraOrbitControlComponent_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnCameraOrbitControlComponent_OnSerialize_SetOverrideCallback(LnCameraOrbitControlComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnCameraOrbitControlComponent_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnCameraOrbitControlComponent_GetTypeInfoId();
LN_FLAT_API void LnCameraOrbitControlComponent_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::Raycaster

/**
    @brief メインのカメラを使用して、指定したスクリーン座標から正面に向かうレイを定義した Raycaster を取得します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnRaycaster_FromScreen(const LnPoint* point, LnHandle* outReturn);

/**
    @brief 指定した向きの平面との交差判定を行います。
    @param[in] raycaster : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnRaycaster_IntersectPlane(LnHandle raycaster, float normalX, float normalY, float normalZ, LnHandle* outReturn);

typedef LnResult(*LnRaycaster_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnRaycaster_OnSerialize_SetOverrideCallback(LnRaycaster_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnRaycaster_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnRaycaster_GetTypeInfoId();
LN_FLAT_API void LnRaycaster_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::RaycastResult

/**
    @brief ワールド座標上の交差点
    @param[in] raycastresult : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnRaycastResult_GetPoint(LnHandle raycastresult, LnVector3* outReturn);

typedef LnResult(*LnRaycastResult_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnRaycastResult_OnSerialize_SetOverrideCallback(LnRaycastResult_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnRaycastResult_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnRaycastResult_GetTypeInfoId();
LN_FLAT_API void LnRaycastResult_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::WorldRenderView

/**
    @brief この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを設定します。
    @param[in] worldrenderview : instance
*/
LN_FLAT_API LnResult LnWorldRenderView_SetGuideGridEnabled(LnHandle worldrenderview, LnBool value);

/**
    @brief この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを取得します。
    @param[in] worldrenderview : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnWorldRenderView_GetGuideGridEnabled(LnHandle worldrenderview, LnBool* outReturn);

typedef LnResult(*LnWorldRenderView_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnWorldRenderView_OnSerialize_SetOverrideCallback(LnWorldRenderView_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnWorldRenderView_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnWorldRenderView_GetTypeInfoId();
LN_FLAT_API void LnWorldRenderView_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::BoxMesh

/**
    @brief 各軸のサイズが 1 である BoxMesh を作成します。
    @param[out] outBoxMesh : instance.
*/
LN_FLAT_API LnResult LnBoxMesh_Create(LnHandle* outBoxMesh);

typedef LnResult(*LnBoxMesh_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnBoxMesh_OnSerialize_SetOverrideCallback(LnBoxMesh_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnBoxMesh_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnBoxMesh_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnBoxMesh_OnUpdate_SetOverrideCallback(LnBoxMesh_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnBoxMesh_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

extern LN_FLAT_API int LnBoxMesh_GetTypeInfoId();
LN_FLAT_API void LnBoxMesh_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::UIEventArgs

/**
    @brief イベントの発生元となった要素を取得します。
    @param[in] uieventargs : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIEventArgs_Sender(LnHandle uieventargs, LnHandle* outReturn);

typedef LnResult(*LnUIEventArgs_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUIEventArgs_OnSerialize_SetOverrideCallback(LnUIEventArgs_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUIEventArgs_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnUIEventArgs_GetTypeInfoId();
LN_FLAT_API void LnUIEventArgs_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::UIGeneralEventHandler

LN_FLAT_API LnResult LnUIGeneralEventHandler_Create(LnUIGeneralEventHandlerCallback callback, LnHandle* outDelegate);
LN_FLAT_API void LnUIGeneralEventHandler_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::UIEventHandler

LN_FLAT_API LnResult LnUIEventHandler_Create(LnUIEventHandlerCallback callback, LnHandle* outDelegate);
LN_FLAT_API void LnUIEventHandler_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::UILayoutElement

typedef LnResult(*LnUILayoutElement_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUILayoutElement_OnSerialize_SetOverrideCallback(LnUILayoutElement_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUILayoutElement_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnUILayoutElement_GetTypeInfoId();
LN_FLAT_API void LnUILayoutElement_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::UIElement

/**
    @brief 要素の margin 値 (外側の余白) を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetMargin(LnHandle uielement, const LnThickness* margin);

/**
    @brief 要素の margin 値 (外側の余白) を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetMargin(LnHandle uielement, LnThickness* outReturn);

/**
    @brief 要素の padding 値 (内側の余白) を設定します。この余白は論理ツリーの子要素のレイアウトに影響します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetPadding(LnHandle uielement, const LnThickness* padding);

/**
    @brief 要素の padding 値 (内側の余白) を取得します。この余白は論理ツリーの子要素のレイアウトに影響します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetPadding(LnHandle uielement, LnThickness* outReturn);

/**
    @brief 要素の横方向の配置方法を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetHAlignment(LnHandle uielement, LnHAlignment value);

/**
    @brief 要素の横方向の配置方法を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetHAlignment(LnHandle uielement, LnHAlignment* outReturn);

/**
    @brief 要素の縦方向の配置方法を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetVAlignment(LnHandle uielement, LnVAlignment value);

/**
    @brief 要素の縦方向の配置方法を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetVAlignment(LnHandle uielement, LnVAlignment* outReturn);

/**
    @brief 要素の配置方法を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetAlignments(LnHandle uielement, LnHAlignment halign, LnVAlignment valign);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetPosition(LnHandle uielement, const LnVector3* pos);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetPositionXYZ(LnHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトの位置を位置を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetPosition(LnHandle uielement, LnVector3* outReturn);

/**
    @brief このオブジェクトの回転を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetRotation(LnHandle uielement, const LnQuaternion* rot);

/**
    @brief このオブジェクトの回転をオイラー角から設定します。(radian)
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetEulerAngles(LnHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトの回転を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetRotation(LnHandle uielement, LnQuaternion* outReturn);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetScale(LnHandle uielement, const LnVector3* scale);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetScaleS(LnHandle uielement, float xyz);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetScaleXY(LnHandle uielement, float x, float y);

/**
    @brief このオブジェクトの拡大率を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetScale(LnHandle uielement, LnVector3* outReturn);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetCenterPoint(LnHandle uielement, const LnVector3* value);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetCenterPointXYZ(LnHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトのローカルの中心位置を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetCenterPoint(LnHandle uielement, LnVector3* outReturn);

/**
    @brief Add element to container.
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_AddChild(LnHandle uielement, LnHandle child);

typedef LnResult(*LnUIElement_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUIElement_OnSerialize_SetOverrideCallback(LnUIElement_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUIElement_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnUIElement_GetTypeInfoId();
LN_FLAT_API void LnUIElement_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::UIControl

typedef LnResult(*LnUIControl_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUIControl_OnSerialize_SetOverrideCallback(LnUIControl_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUIControl_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnUIControl_GetTypeInfoId();
LN_FLAT_API void LnUIControl_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::UIButtonBase

/**
    @brief set text.
    @param[in] uibuttonbase : instance
*/
LN_FLAT_API LnResult LnUIButtonBase_SetText(LnHandle uibuttonbase, const LnChar* text);
LN_FLAT_API LnResult LnUIButtonBase_SetTextA(LnHandle uibuttonbase, const char* text);

typedef LnResult(*LnUIButtonBase_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUIButtonBase_OnSerialize_SetOverrideCallback(LnUIButtonBase_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUIButtonBase_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnUIButtonBase_GetTypeInfoId();
LN_FLAT_API void LnUIButtonBase_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::UIButton

/**
    @brief init.
    @param[out] outUIButton : instance.
*/
LN_FLAT_API LnResult LnUIButton_Create(LnHandle* outUIButton);

/**
    @brief 表示文字列を指定して UIButton を作成します。
    @param[out] outUIButton : instance.
*/
LN_FLAT_API LnResult LnUIButton_CreateWithText(const LnChar* text, LnHandle* outUIButton);
LN_FLAT_API LnResult LnUIButton_CreateWithTextA(const char* text, LnHandle* outUIButton);

/**
    @brief Clicked イベントの通知を受け取るコールバックを登録します。
    @param[in] uibutton : instance
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LnResult LnUIButton_ConnectOnClicked(LnHandle uibutton, LnHandle handler, LnHandle* outReturn);

typedef LnResult(*LnUIButton_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUIButton_OnSerialize_SetOverrideCallback(LnUIButton_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUIButton_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnUIButton_GetTypeInfoId();
LN_FLAT_API void LnUIButton_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::UITextBlock

/**
    @brief UITextBlock を作成します。
    @param[out] outUITextBlock : instance.
*/
LN_FLAT_API LnResult LnUITextBlock_Create(LnHandle* outUITextBlock);

/**
    @brief 表示文字列を指定して、UITextBlock を作成します。
    @param[in] text : 表示文字列
    @param[out] outUITextBlock : instance.
*/
LN_FLAT_API LnResult LnUITextBlock_CreateWithText(const LnChar* text, LnHandle* outUITextBlock);
LN_FLAT_API LnResult LnUITextBlock_CreateWithTextA(const char* text, LnHandle* outUITextBlock);

typedef LnResult(*LnUITextBlock_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUITextBlock_OnSerialize_SetOverrideCallback(LnUITextBlock_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUITextBlock_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnUITextBlock_GetTypeInfoId();
LN_FLAT_API void LnUITextBlock_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::UISprite

/**
    @brief スプライトが表示するテクスチャを設定します。
    @param[in] uisprite : instance
*/
LN_FLAT_API LnResult LnUISprite_SetTexture(LnHandle uisprite, LnHandle texture);

/**
    @brief テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) デフォルトは Rect::Empty で、テクスチャ全体を転送することを示します。
    @param[in] uisprite : instance
*/
LN_FLAT_API LnResult LnUISprite_SetSourceRect(LnHandle uisprite, const LnRect* rect);

/**
    @brief 
    @param[in] uisprite : instance
*/
LN_FLAT_API LnResult LnUISprite_SetSourceRectXYWH(LnHandle uisprite, float x, float y, float width, float height);

/**
    @brief テクスチャのどの部分を表示するかを示す転送矩形を取得します。(ピクセル単位)
    @param[in] uisprite : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUISprite_GetSourceRect(LnHandle uisprite, LnRect* outReturn);

/**
    @brief UISprite を作成します。
    @param[out] outUISprite : instance.
*/
LN_FLAT_API LnResult LnUISprite_Create(LnHandle* outUISprite);

/**
    @brief テクスチャを指定して UISprite を作成します。
    @param[out] outUISprite : instance.
*/
LN_FLAT_API LnResult LnUISprite_CreateWithTexture(LnHandle texture, LnHandle* outUISprite);

typedef LnResult(*LnUISprite_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUISprite_OnSerialize_SetOverrideCallback(LnUISprite_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUISprite_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

extern LN_FLAT_API int LnUISprite_GetTypeInfoId();
LN_FLAT_API void LnUISprite_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::Input

/**
    @brief ボタンが現在押されているかを判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnInput_Pressed(const LnChar* buttonName, LnBool* outReturn);
LN_FLAT_API LnResult LnInput_PressedA(const char* buttonName, LnBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnInput_Triggered(const LnChar* buttonName, LnBool* outReturn);
LN_FLAT_API LnResult LnInput_TriggeredA(const char* buttonName, LnBool* outReturn);

/**
    @brief ボタンが離された瞬間を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnInput_TriggeredOff(const LnChar* buttonName, LnBool* outReturn);
LN_FLAT_API LnResult LnInput_TriggeredOffA(const char* buttonName, LnBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間とリピート状態を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnInput_Repeated(const LnChar* buttonName, LnBool* outReturn);
LN_FLAT_API LnResult LnInput_RepeatedA(const char* buttonName, LnBool* outReturn);

/**
    @brief 指定した軸のアナログ値を取得します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnInput_GetAxisValue(const LnChar* buttonName, float* outReturn);
LN_FLAT_API LnResult LnInput_GetAxisValueA(const char* buttonName, float* outReturn);

/**
    @brief 全てのアクションマッピングを削除します。
*/
LN_FLAT_API LnResult LnInput_ClearAllBindings();


//==============================================================================
// ln::Mouse

/**
    @brief ボタンが現在押されているかを判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnMouse_Pressed(LnMouseButtons button, LnBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnMouse_Triggered(LnMouseButtons button, LnBool* outReturn);

/**
    @brief ボタンが離された瞬間を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnMouse_TriggeredOff(LnMouseButtons button, LnBool* outReturn);

/**
    @brief ボタンが新しく押された瞬間とリピート状態を判定します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnMouse_Repeated(LnMouseButtons button, LnBool* outReturn);

/**
    @brief マウスポインタの位置を取得します。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnMouse_Position(LnPoint* outReturn);


//==============================================================================
// ln::EngineSettings

/**
    @brief メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480)
*/
LN_FLAT_API LnResult LnEngineSettings_SetMainWindowSize(int width, int height);

/**
    @brief メインウィンドウに対して作成される WorldView のサイズを設定します。(default: クライアント領域のサイズと同等)
*/
LN_FLAT_API LnResult LnEngineSettings_SetMainWorldViewSize(int width, int height);

/**
    @brief メインウィンドウのタイトル文字列を設定します。
*/
LN_FLAT_API LnResult LnEngineSettings_SetMainWindowTitle(const LnChar* title);
LN_FLAT_API LnResult LnEngineSettings_SetMainWindowTitleA(const char* title);

/**
    @brief アセットが保存されているディレクトリを登録します。
*/
LN_FLAT_API LnResult LnEngineSettings_AddAssetDirectory(const LnChar* path);
LN_FLAT_API LnResult LnEngineSettings_AddAssetDirectoryA(const char* path);

/**
    @brief アセットファイルを登録します。
*/
LN_FLAT_API LnResult LnEngineSettings_AddAssetArchive(const LnChar* fileFullPath, const LnChar* password);
LN_FLAT_API LnResult LnEngineSettings_AddAssetArchiveA(const char* fileFullPath, const char* password);

/**
    @brief (default: Debug ビルドの場合true、それ以外は false)
*/
LN_FLAT_API LnResult LnEngineSettings_SetDebugToolEnabled(LnBool enabled);

/**
    @brief デバッグ用のログファイルの出力有無を設定します。(default: Debug ビルドの場合true、それ以外は false)
*/
LN_FLAT_API LnResult LnEngineSettings_SetEngineLogEnabled(LnBool enabled);

/**
    @brief デバッグ用のログファイルの出力先ファイルパスを設定します。(default: Empty(実行ファイルのディレクトリへ出力))
*/
LN_FLAT_API LnResult LnEngineSettings_SetEngineLogFilePath(const LnChar* filePath);
LN_FLAT_API LnResult LnEngineSettings_SetEngineLogFilePathA(const char* filePath);


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
    @param[out] outReturn : instance.
    @return アプリケーションの終了が要求されている場合は false を返します。
*/
LN_FLAT_API LnResult LnEngine_Update(LnBool* outReturn);

/**
    @brief アプリケーション開始からの経過時間を取得します。この値はタイムスケールの影響を受けます。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnEngine_Time(double* outReturn);

/**
    @brief デフォルトで作成されるメインの Camera です。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnEngine_GetCamera(LnHandle* outReturn);

/**
    @brief デフォルトで作成されるメインの DirectionalLight です。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnEngine_GetLight(LnHandle* outReturn);

/**
    @brief デフォルトで作成されるメインの RenderView です。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnEngine_GetRenderView(LnHandle* outReturn);


//==============================================================================
// ln::Application

/**
    @brief エンジンの初期化処理が完了した後に呼び出されます。
    @param[in] application : instance
*/
LN_FLAT_API LnResult LnApplication_OnInit(LnHandle application);

/**
    @brief 毎フレーム呼び出されます。
    @param[in] application : instance
*/
LN_FLAT_API LnResult LnApplication_OnUpdate(LnHandle application);

/**
    @brief デフォルトで作成されるメインの World を取得します。
    @param[in] application : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnApplication_World(LnHandle application, LnHandle* outReturn);

/**
    @brief 
    @param[out] outApplication : instance.
*/
LN_FLAT_API LnResult LnApplication_Create(LnHandle* outApplication);

typedef LnResult(*LnApplication_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnApplication_OnSerialize_SetOverrideCallback(LnApplication_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnApplication_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnApplication_OnInit_OverrideCallback)(LnHandle application);
LN_FLAT_API LnResult LnApplication_OnInit_SetOverrideCallback(LnApplication_OnInit_OverrideCallback callback);
LN_FLAT_API LnResult LnApplication_OnInit_CallOverrideBase(LnHandle application);
typedef LnResult(*LnApplication_OnUpdate_OverrideCallback)(LnHandle application);
LN_FLAT_API LnResult LnApplication_OnUpdate_SetOverrideCallback(LnApplication_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnApplication_OnUpdate_CallOverrideBase(LnHandle application);

extern LN_FLAT_API int LnApplication_GetTypeInfoId();
LN_FLAT_API void LnApplication_SetManagedTypeInfoId(int64_t id);

//==============================================================================
// ln::Debug

/**
    @brief ウィンドウ上にデバッグ文字列を表示します。
    @param[in] str : 表示文字列
*/
LN_FLAT_API LnResult LnDebug_Print(const LnChar* str);
LN_FLAT_API LnResult LnDebug_PrintA(const char* str);

/**
    @brief 表示時間を指定して、ウィンドウ上にデバッグ文字列を表示します。
    @param[in] time : 表示時間 (s)
    @param[in] str : 表示文字列
*/
LN_FLAT_API LnResult LnDebug_PrintWithTime(float time, const LnChar* str);
LN_FLAT_API LnResult LnDebug_PrintWithTimeA(float time, const char* str);

/**
    @brief 表示時間と文字色を指定して、ウィンドウ上にデバッグ文字列を表示します。
    @param[in] time : 表示時間 (s)
    @param[in] color : 文字色
    @param[in] str : 表示文字列
*/
LN_FLAT_API LnResult LnDebug_PrintWithTimeAndColor(float time, const LnColor* color, const LnChar* str);
LN_FLAT_API LnResult LnDebug_PrintWithTimeAndColorA(float time, const LnColor* color, const char* str);




} // extern "C"

