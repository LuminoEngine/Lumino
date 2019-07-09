#include <LuminoEngine/Runtime/Runtime.hpp>
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

class LNWS_ln_WorldObject : public ln::WorldObject
{
};

class LNWS_ln_VisualObject : public ln::VisualObject
{
};

class LNWS_ln_Sprite : public ln::Sprite
{
public:
	//using ManagedCallerFunc_onUpdate = std::function<void(LnHandle, float)>;

	static LnSprite_SetCaller_OnUpdate_FuncPtr m_ManagedCaller_onUpdate;

	virtual void onUpdate(float elapsedSeconds)
	{
		m_ManagedCaller_onUpdate(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
	}
	void onUpdate_CallBase(float elapsedSeconds) { ln::Sprite::onUpdate(elapsedSeconds); }
};

LnSprite_SetCaller_OnUpdate_FuncPtr LNWS_ln_Sprite::m_ManagedCaller_onUpdate;


extern "C"
{

LN_FLAT_API LnResult LnVector3_Length(const LnVector3* vector3, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const ln::Vector3*>(vector3)->length());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnVector3_Lengthsquared(const LnVector3* vector3, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const ln::Vector3*>(vector3)->lengthSquared());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnVector3_NormalizeXYZ(float x, float y, float z, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LnVector3&>(ln::Vector3::normalize(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnVector3_Normalize(const LnVector3* vec, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LnVector3&>(ln::Vector3::normalize(*reinterpret_cast<const ln::Vector3*>(vec)));
    LNI_FUNC_TRY_END_RETURN;
}



LN_FLAT_API LnResult LnEngine_Initialize()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Engine::initialize());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnEngine_Finalize()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Engine::finalize());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnEngine_Update(LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = LNI_BOOL_TO_LNBOOL(ln::Engine::update());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnTexture2D_Create(int width, int height, LnHandle* outTexture2D)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outTexture2D, LNWS_ln_Texture2D, init, width, height);
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnTexture2D_CreateWithFormat(int width, int height, LnTextureFormat format, LnHandle* outTexture2D)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outTexture2D, LNWS_ln_Texture2D, init, width, height, static_cast<ln::TextureFormat>(format));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnTexture2D_CreateFromFile(const LnChar* filePath, LnTextureFormat format, LnHandle* outTexture2D)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outTexture2D, LNWS_ln_Texture2D, init, filePath, static_cast<ln::TextureFormat>(format));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_Setposition(LnHandle worldobject, const LnVector3* pos)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->setPosition(*reinterpret_cast<const ln::Vector3*>(pos)));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_SetpositionXYZ(LnHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->setPosition(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_Position(LnHandle worldobject, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->position());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_Setrotation(LnHandle worldobject, const LnQuaternion* rot)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->setRotation(*reinterpret_cast<const ln::Quaternion*>(rot)));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_Seteulerangles(LnHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->setEulerAngles(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_Rotation(LnHandle worldobject, LnQuaternion* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LnQuaternion&>(LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->rotation());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_Setscale(LnHandle worldobject, const LnVector3* scale)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->setScale(*reinterpret_cast<const ln::Vector3*>(scale)));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_SetscaleS(LnHandle worldobject, float xyz)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->setScale(xyz));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_SetscaleXYZ(LnHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->setScale(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_Scale(LnHandle worldobject, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->scale());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_Setcenterpoint(LnHandle worldobject, const LnVector3* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->setCenterPoint(*reinterpret_cast<const ln::Vector3*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_SetcenterpointXYZ(LnHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->setCenterPoint(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_Centerpoint(LnHandle worldobject, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(ln::WorldObject, worldobject)->centerPoint());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnVisualObject_Setvisible(LnHandle visualobject, LnBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::VisualObject, visualobject)->setVisible(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnVisualObject_Isvisible(LnHandle visualobject, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(ln::VisualObject, visualobject)->isVisible());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnSprite_Settexture(LnHandle sprite, LnHandle texture)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::Sprite, sprite)->setTexture(LNI_HANDLE_TO_OBJECT(ln::Texture, texture)));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnSprite_SetsourcerectXYWH(LnHandle sprite, float x, float y, float width, float height)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(ln::Sprite, sprite)->setSourceRect(x, y, width, height));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnSprite_Create(LnHandle texture, float width, float height, LnHandle* outSprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSprite, LNWS_ln_Sprite, init, LNI_HANDLE_TO_OBJECT(ln::Texture, texture), width, height);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSprite_CallBase_OnUpdate(LnHandle sprite, float elapsedSeconds)
{
	LNI_FUNC_TRY_BEGIN;
	(LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->onUpdate_CallBase(elapsedSeconds));
	LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnSprite_SetCaller_OnUpdate(LnSprite_SetCaller_OnUpdate_FuncPtr func)
{
	LNWS_ln_Sprite::m_ManagedCaller_onUpdate = func;
	return LN_SUCCESS;
}

} // extern "C"
