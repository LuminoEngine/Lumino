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
	if (LN_VARIANTTYPE_MAX != VariantType_Max) goto ERR_EXIT;

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
