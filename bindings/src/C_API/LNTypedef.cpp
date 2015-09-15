//==============================================================================
// LNTypedef
//------------------------------------------------------------------------------
/*
	■ Rect や Size は int であるべき？float であるべき？
		WPF の Rect は double だが…。

		まず、LightNote 的な Rect の意味はレイアウトじゃない点に注意。
		2D のある範囲を定義するためのデータ。

		ただし、コリジョンボックスとも違う。3Dじゃ Rect なんて使わない。Box になる。
		なので、こっちは2DBox。

		とすると、用途はいまの
		・テクスチャへのテキストの描画領域
		・BitBltやSprite用の転送元領域の指定
		で充分。
		というか、要素番号として整数を取るような、例えばまさしく配列の範囲を
		定義するために使用するべき。

		ということで int 型。

		ちなみに Size はいらないと思う。
		今は Window と Texture のサイズ指定・取得にしか使ってないけど、
		Unity とか gameplay 等のエンジンでは直接 Width・Height プロパティで
		int 型で返している。
*/
//==============================================================================

#include "LNInternal.h"
#include "LNTypedef.h"

void LNTypeDef_CheckCommonDefinition()
{
	if (sizeof(LNVariant) >= sizeof(Variant)) goto ERR_EXIT;
	if (LN_VARIANTTYPE__TERMINATOR != VariantType_Max) goto ERR_EXIT;
	if (LN_SOUNDPLAYINGTYPE__TERMINATOR != (int)SoundPlayingMode::TERMINATOR) goto ERR_EXIT;
	if (LN_SOUNDFADEBEHAVIOR__TERMINATOR != (int)SoundFadeBehavior::TERMINATOR) goto ERR_EXIT;

#if 0
	if (LN_OK != LNote::ResultCode_OK) goto ERR_EXIT;
	if (LN_ERR_UNKNOWN != LNote::ResultCode_Unknown) goto ERR_EXIT;
	if (LN_ERR_INVALID_OPERATION != LNote::ResultCode_InvalidOperation) goto ERR_EXIT;
	if (LN_ERR_ARGUMENT != LNote::ResultCode_Argument) goto ERR_EXIT;
	if (LN_ERR_OUT_OF_MEMORY != LNote::ResultCode_OutOfMemory) goto ERR_EXIT;
	if (LN_ERR_FILE_NOT_FOUND != LNote::ResultCode_FileNotFound) goto ERR_EXIT;
	if (LN_ERR_SYSTEM_CALL != LNote::ResultCode_SystemCall) goto ERR_EXIT;
	if (LN_ERR_NOT_IMPLEMENTED != LNote::ResultCode_NotImplemented) goto ERR_EXIT;
	if (LN_ERR_INVALID_FORMAT != LNote::ResultCode_InvalidFormat) goto ERR_EXIT;
	if (LN_ERR_THREAD != LNote::ResultCode_Thread) goto ERR_EXIT;
	if (LN_ERR_FATAL != LNote::ResultCode_Fatal) goto ERR_EXIT;
	if (LN_ERR_COM_CALL != LNote::ResultCode_ComCall) goto ERR_EXIT;
	if (LN_ERR_OPENGL != LNote::ResultCode_OpenGL) goto ERR_EXIT;
	if (LN_ERR_DIRECTX != LNote::ResultCode_DirectX) goto ERR_EXIT;
	if (LN_ERR_WIN32API != LNote::ResultCode_WinAPI) goto ERR_EXIT;

	if (LN_FILETYPE_MAX != LNote::Core::FileIO::FileType_Max) goto ERR_EXIT;
	if ( LN_ASYNCIOSTATE_MAX != LNote::Core::FileIO::AsyncIOState_Max ) goto ERR_EXIT;

	if ( LN_BUTTON_0 != LNote::Core::Input::LN_BUTTON_A ) goto ERR_EXIT;

	if ( LN_DMINITMODE_MAX != LNote::Core::Audio::DMINITMODE_MAX ) goto ERR_EXIT;
	if ( LN_SOUNDPLAYINGTYPE_MAX != LNote::Core::Audio::SOUNDPLAYTYPE_MAX ) goto ERR_EXIT;
	if ( LN_SOUNDPLAYINGSTATE_MAX != LNote::Core::Audio::SOUNDPLAYSTATE_MAX ) goto ERR_EXIT;
	if ( LN_SOUNDFADE_MAX != LNote::Core::Audio::SOUNDFADE_MAX ) goto ERR_EXIT;
	if ( LN_INTERNALGAMESOUND_MAX != LNote::Core::Audio::GameAudio::InternalGameSound_Max ) goto ERR_EXIT;
	
	if (::LN_BLEND_MAX != ::LNote::Core::Graphics::BlendMode_Max) goto ERR_EXIT;
	if (::LN_CULL_MAX != ::LNote::Core::Graphics::CullingMode_Max) goto ERR_EXIT;
	if ( ::LN_PLACEMENTTYPE_MAX != LNote::LN_BILLBOARD_MAX ) goto ERR_EXIT;
	if ( ::LN_FRONTAXIS_MAX != LNote::LN_AADIR_MAX ) goto ERR_EXIT;
	if ( ::LN_MESHCREATEFLAGS_MAX != LNote::Core::Graphics::ModelCreateFlag_Max ) goto ERR_EXIT;
	if ( ::LN_CAMERATYPE_MAX != Graphics::CameraType_Max ) goto ERR_EXIT;
	
	if (LN_DEVICEBUTTON_MOUSE_1 != LN_KEY_MAX) goto ERR_EXIT;
#endif
	return;

ERR_EXIT:
	LN_ASSERT( "type definition error." );
}

//=============================================================================
// Input
//=============================================================================

/** 仮想ボタン */
typedef enum tagLNButton
{
	LN_BUTTON_0 = 0,				/** ボタン0 (キーボード='Z' ジョイパッド=1番ボタン) */
	LN_BUTTON_1,					/** ボタン1 (キーボード='X' ジョイパッド=2) */
	LN_BUTTON_2,					/** ボタン2 (キーボード='C' ジョイパッド=3) */
	LN_BUTTON_3,					/** ボタン3 (キーボード='A' ジョイパッド=4) */
	LN_BUTTON_4,					/** ボタン4 (キーボード='S' ジョイパッド=5) */
	LN_BUTTON_5,					/** ボタン5 (キーボード='D' ジョイパッド=6) */
	LN_BUTTON_6,					/** ボタン6 (キーボード='Q' ジョイパッド=7) */
	LN_BUTTON_7,					/** ボタン7 (キーボード='W' ジョイパッド=8) */
	LN_BUTTON_8,					/** ボタン8 (キーボード=None ジョイパッド=9) */
	LN_BUTTON_9,					/** ボタン9 (キーボード=None ジョイパッド=10) */
	LN_BUTTON_10,					/** ボタン10 (キーボード=None ジョイパッド=11) */
	LN_BUTTON_11,					/** ボタン11 (キーボード=None ジョイパッド=12) */
	LN_BUTTON_12,					/** ボタン12 (キーボード=None ジョイパッド=13) */
	LN_BUTTON_13,					/** ボタン13 (キーボード=None ジョイパッド=14) */
	LN_BUTTON_14,					/** ボタン14 (キーボード=None ジョイパッド=15) */
	LN_BUTTON_15,					/** ボタン15 (キーボード=None ジョイパッド=16) */
	LN_BUTTON_LEFT,					/** 左ボタン (キーボード=LN_KEY_LEFT  ジョイパッド=POV左&第1X軸-) */
	LN_BUTTON_RIGHT,				/** 右ボタン (キーボード=LN_KEY_RIGHT ジョイパッド=POV右&第1X軸+) */
	LN_BUTTON_UP,					/** 上ボタン (キーボード=LN_KEY_UP    ジョイパッド=POV上&第1Y軸-) */
	LN_BUTTON_DOWN,					/** 下ボタン (キーボード=LN_KEY_DOWN  ジョイパッド=POV下&第1Y軸+) */

	LN_BUTTON_AXIS_1X_MINUIS,		/** 第1レバー X- 方向 */
	LN_BUTTON_AXIS_1X_PLUS,			/** 第1レバー X+ 方向 */
	LN_BUTTON_AXIS_1Y_MINUIS,		/** 第1レバー Y- 方向 */
	LN_BUTTON_AXIS_1Y_PLUS,			/** 第1レバー Y+ 方向 */
	LN_BUTTON_AXIS_2X_MINUIS,		/** 第2レバー X- 方向 */
	LN_BUTTON_AXIS_2X_PLUS,			/** 第2レバー X+ 方向 */
	LN_BUTTON_AXIS_2Y_MINUIS,		/** 第2レバー Y- 方向 */
	LN_BUTTON_AXIS_2Y_PLUS,			/** 第2レバー Y+ 方向 */

	LN_BUTTON__TERMINATOR,

} LNButton;

/** デバイスボタン番号の分類 */
typedef enum tagLNDeviceButtonType
{
	LN_DEVICEBUTTONTYPE_UNKNOWN = 0,			/** 不明なデバイス種類 */
	LN_DEVICEBUTTONTYPE_KEYBOARD,				/** キーボード */
	LN_DEVICEBUTTONTYPE_MOUSE,					/** マウスボタン */
	LN_DEVICEBUTTONTYPE_JOYPAD_BUTTON,			/** ジョイパッドのボタン */
	LN_DEVICEBUTTONTYPE_JOYPAD_POV,				/** ジョイパッドのPOV */
	LN_DEVICEBUTTONTYPE_JOYPAD_AXIS,			/** ジョイパッドの軸 */

	LN_DEVICEBUTTONTYPE__TERMINATOR,

} LNDeviceButtonType;

/** 全てのデバイス用の仮想ボタン番号 (デバイスボタン番号) (1 ～ 255 は LN_KEY_～と兼用) */
typedef enum tagLNDeviceButton
{
	LN_DEVICEBUTTON_UNKNOWN = 0,

	LN_DEVICEBUTTON_MOUSE_1 = 256,
	LN_DEVICEBUTTON_MOUSE_2,
	LN_DEVICEBUTTON_MOUSE_3,
	LN_DEVICEBUTTON_MOUSE_4,
	LN_DEVICEBUTTON_MOUSE_5,
	LN_DEVICEBUTTON_MOUSE_6,
	LN_DEVICEBUTTON_MOUSE_7,
	LN_DEVICEBUTTON_MOUSE_8,

	LN_DEVICEBUTTON_JOYPAD_BUTTON_1,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_2,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_3,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_4,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_5,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_6,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_7,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_8,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_9,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_10,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_11,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_12,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_13,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_14,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_15,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_16,

	LN_DEVICEBUTTON_JOYPAD_POV_UP,
	LN_DEVICEBUTTON_JOYPAD_POV_RIGHT,
	LN_DEVICEBUTTON_JOYPAD_POV_DOWN,
	LN_DEVICEBUTTON_JOYPAD_POV_LEFT,

	LN_DEVICEBUTTON_JOYPAD_AXIS_1_MINUIS,      /** X1 -1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_1_PLUS,        /** X1  1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_2_MINUIS,      /** Y1 -1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_2_PLUS,        /** Y1  1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_3_MINUIS,      /** X2 -1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_3_PLUS,        /** X2  1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_4_MINUIS,      /** Y2 -1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_4_PLUS,        /** Y2  1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_5_MINUIS,      /** X3 -1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_5_PLUS,        /** X3  1.0     XInput LT */
	LN_DEVICEBUTTON_JOYPAD_AXIS_6_MINUIS,      /** Y3 -1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_6_PLUS,        /** Y3  1.0     XInput RT */
	LN_DEVICEBUTTON_JOYPAD_AXIS_7_MINUIS,      /** X4 -1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_7_PLUS,        /** X4  1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_8_MINUIS,      /** Y4 -1.0 */
	LN_DEVICEBUTTON_JOYPAD_AXIS_8_PLUS,        /** Y4  1.0 */

	LN_DEVICEBUTTON__TERMINATOR,

} LNDeviceButton;