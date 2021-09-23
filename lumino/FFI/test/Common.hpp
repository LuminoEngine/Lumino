#pragma once

#include "TestEnv.hpp"

// テストフィクスチャ終了時に World をクリーンアップする
class LuminoSceneTest : public ::testing::Test
{
    virtual void TearDown() override
    {
        //Engine:()->removeAllObjects();
    }
};

#define LN_NEW_GRAPHICS_MIGRATION
