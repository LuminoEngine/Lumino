#include "Common.hpp"
#include <LuminoEngine/Runtime/Lumino.FlatC.generated.h>

#if 0
//==============================================================================
class Test_FlatAPI : public LuminoSceneTest {};

//------------------------------------------------------------------------------
static int g_count = 0;

static LnResult Sprite_OnUpdate(LnHandle delegate, LnHandle self, float elapsedSeconds)
{
	g_count++;
	return LN_SUCCESS;
}

TEST_F(Test_FlatAPI, Basic)
{
	LnHandle texture;
	LnTexture2D_Load(LN_ASSETFILE("Sprite1.png"), &texture);

	LnHandle delegate;
	LnSpriteUpdateHandler_Create(Sprite_OnUpdate, &delegate);

	LnHandle sprite;
	LnSprite_CreateWithTexture(texture, &sprite);
	LnWorldObject_SetScaleS(sprite, 5);
	LnSprite_SetPrototype_OnUpdate(sprite, delegate);

	g_count = 0;

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("FlatAPI/Expects/Basic-1.png"));
	LN_TEST_CLEAN_SCENE;

	LnObject_Release(sprite);
	LnObject_Release(delegate);
	LnObject_Release(texture);

	ASSERT_EQ(g_count, 1);
}

#endif
