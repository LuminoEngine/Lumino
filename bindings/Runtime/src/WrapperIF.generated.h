
#pragma once
#include <LuminoEngine.h>
#include "LuminoC.generated.h"
using namespace ln;

typedef LNResultCode (*GameScene_OnStart_OverrideCaller)(LNHandle gamescene);


class LNSound : public Sound
{
public:



};

class LNGameApplication : public GameApplication
{
public:



};

class LNGameScene : public GameScene
{
public:
    static GameScene_OnStart_OverrideCaller m_OnStart_OverrideCaller;


    virtual void OnStart() override
    {
        m_OnStart_OverrideCaller(LWIG_TO_HANDLE(this));
    }

};
GameScene_OnStart_OverrideCaller LNGameScene::m_OnStart_OverrideCaller = nullptr;



