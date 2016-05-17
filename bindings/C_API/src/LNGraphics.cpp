//==============================================================================
// LNGraphics 
//==============================================================================

#include "LNInternal.h"
#include "../include/LNGraphics.h"

//==============================================================================
// LNTexture
//==============================================================================
LN_TYPE_INFO_IMPL(Texture, LNTexture);

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
LN_TYPE_INFO_IMPL(Texture2D, LNTexture2D);

//------------------------------------------------------------------------------
LNResult LNTexture2D_Create(int width, int height, LNTextureFormat format, LNBool mipmap, LN_OUT LN_HANDLE(LNTexture2D)* outTexture2D)
{
	LN_CHECK_ARG(outTexture2D != nullptr);
	LN_FUNC_TRY_BEGIN;
	auto ptr = RefPtr<LNWITexture2D>::MakeRef();
	ptr->Initialize(LFManager::Engine->GetGraphicsManager(), Size(width, height), TextureFormat_R8G8B8A8, 1);
	*outTexture2D = LFManager::CheckRegisterObject(ptr);
	ptr.SafeAddRef();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNTexture2D_CreateFromFile(const LNChar* filePath, LN_OUT LN_HANDLE(LNTexture2D)* texture2D)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<LNWITexture2D> obj(LN_NEW LNWITexture2D(), false);
	obj->CreateCore(LFManager::Engine->GetGraphicsManager(), filePath, TextureFormat_B8G8R8A8, 1);
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
