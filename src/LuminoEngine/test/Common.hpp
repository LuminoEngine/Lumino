#pragma once

#include "gtest/gtest.h"

#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
using namespace ln;

#include "../../src/LuminoEngine/src/Engine/EngineDomain.hpp"

#include <LuminoCore/Testing/TestHelper.hpp>
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
