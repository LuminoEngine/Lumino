#include "RuntimeManager.hpp"
#include "Lumino.FlatC.generated.h"

#include <LuminoEngine.hpp>

class LNWS_ln_Engine : public ln::Engine
{
};

class LNWS_ln_GraphicsResource : public ln::GraphicsResource
{
};

class LNWS_ln_Texture : public ln::Texture
{
};

class LNWS_ln_Texture2D : public ln::Texture2D
{
};



extern "C"
{



LN_API LnResult LnEngine_Initialize()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Engine::initialize());
    LNI_FUNC_TRY_END_RETURN;
}

LN_API LnResult LnEngine_Finalize()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Engine::finalize());
    LNI_FUNC_TRY_END_RETURN;
}

LN_API LnResult LnEngine_Update(LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = LNI_BOOL_TO_LNBOOL(ln::Engine::update());
    LNI_FUNC_TRY_END_RETURN;
}

LN_API LnResult LnTexture2D_Init(LnHandle texture2d, int width, int height)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_TO_OBJECT(LNTexture2D, texture2d)->init(width, height));
    LNI_FUNC_TRY_END_RETURN;
}

LN_API LnResult LnTexture2D_Init(LnHandle texture2d, int width, int height, LnTextureFormat format)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_TO_OBJECT(LNTexture2D, texture2d)->init(width, height, static_cast<TextureFormat>(format)));
    LNI_FUNC_TRY_END_RETURN;
}


	
} // extern "C"
