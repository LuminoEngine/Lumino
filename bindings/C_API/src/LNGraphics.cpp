//==============================================================================
// LNGraphics 
//==============================================================================

#include "LNInternal.h"
#include "../include/LNGraphics.h"

//==============================================================================
// LNTexture
//==============================================================================
LN_TYPE_INFO_IMPL(ln::Texture, LNTexture);

//------------------------------------------------------------------------------
LNResult LNTexture_GetSize(LN_HANDLE(LNTexture) texture, LN_OUT LNSize* outSize)
{
	LN_CHECK_ARG_HANDLE(texture);
	LN_CHECK_ARG_RETURN(outSize != NULL);
	LN_FUNC_TRY_BEGIN;
	auto& s = TO_REFOBJ(Texture, texture)->GetSize();
	outSize->Width = s.width;
	outSize->Height = s.height;
	LN_FUNC_TRY_END_RETURN;
}

//==============================================================================
// LNTexture2D
//==============================================================================
LN_TYPE_INFO_IMPL(ln::Texture2D, LNTexture2D);

//------------------------------------------------------------------------------
LNResult LNTexture2D_Create(int width, int height, LNTextureFormat format, LNBool mipmap, LN_OUT LN_HANDLE(LNTexture2D)* outTexture2D)
{
	LN_CHECK_ARG(outTexture2D != nullptr);
	LN_FUNC_TRY_BEGIN;
	auto ptr = RefPtr<LNWITexture2D>::MakeRef();
	ptr->Initialize(LFManager::Engine->GetGraphicsManager(), Size(width, height), (TextureFormat)format, (mipmap) ? 0 : 1);
	*outTexture2D = LFManager::CheckRegisterObject(ptr);
	ptr.SafeAddRef();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNTexture2D_CreateFromFile(const LNChar* filePath, LN_OUT LN_HANDLE(LNTexture2D)* texture2D)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<LNWITexture2D> obj(LN_NEW LNWITexture2D(), false);
	obj->Initialize(LFManager::Engine->GetGraphicsManager(), filePath, TextureFormat::B8G8R8A8, 1);
	*texture2D = LFManager::CheckRegisterObject(obj);
	obj.SafeAddRef();
	LN_FUNC_TRY_END_RETURN;
}

	/**
		@brief		ファイルから2Dテクスチャオブジェクトを作成します。
		@param[in]	filePath	: 画像ファイルのパス
		@param[in]	format		: 作成するテクスチャのフォーマット
		@param[in]	mipmap		: LN_TRUE の場合、ミップマップを作成する
		@param[out]	texture2D	: 作成された2Dテクスチャオブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNTexture2D_CreateEx(const LNChar* filePath, LNTextureFormat foramt, LNBool mipmap, LN_OUT LN_HANDLE(Texture2D)* texture2D);

LNResult LNTexture2D_CreateEx(const LNChar* filePath, LNTextureFormat foramt, LNBool mipmap, LN_OUT LN_HANDLE(Texture2D)* texture2D)
{
	return ::LN_OK;
}

//==============================================================================
// LNViewportLayer
//==============================================================================
LN_TYPE_INFO_IMPL(ln::ViewportLayer, LNViewportLayer);

//==============================================================================
// LNViewport
//==============================================================================
LN_TYPE_INFO_IMPL(ln::Viewport, LNViewport);

//------------------------------------------------------------------------------
LNResult LNViewport_GetMainViewport(LN_OUT LN_HANDLE(LNViewport)* outViewport)
{
	LN_CHECK_ARG(outViewport != nullptr);
	LN_FUNC_TRY_BEGIN;
	*outViewport = TO_HANDLE_ADDREF(ln::Viewport::GetMainWindowViewport());
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNViewport_GetLayers(LN_HANDLE(LNViewport) viewport, LN_OUT LN_GENERIC_HANDLE(LNObjectList, LNViewportLayer)* outList)
{
	LN_CHECK_ARG_HANDLE(viewport);
	LN_CHECK_ARG(outList != nullptr);
	LN_FUNC_TRY_BEGIN;
	*outList = TO_HANDLE_ADDREF(TO_REFOBJ(ln::Viewport, viewport)->GetLayers());
	LN_FUNC_TRY_END_RETURN;
}
