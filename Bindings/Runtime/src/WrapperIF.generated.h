
#pragma once
#include <LuminoEngine.h>
#include "LuminoC.generated.h"
using namespace ln;

typedef LNResultCode (*GameScene_OnStart_OverrideCaller)(LNHandle gamescene);


class LNSound : public Sound
{
public:



};

class LNTexture : public Texture
{
public:



};

class LNTexture2D : public Texture2D
{
public:



};

class LNGraphicsResourceObject : public GraphicsResourceObject
{
public:



};

class LNSceneNode : public SceneNode
{
public:



};

class LNVisualNode : public VisualNode
{
public:



};

class LNSprite : public Sprite
{
public:



};

class LNSprite2D : public Sprite2D
{
public:



};

class LNSprite3D : public Sprite3D
{
public:



};

class LNStaticMesh : public StaticMesh
{
public:



};

class LNComponent : public Component
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
    template<typename... TArgs>
    void OnStart_CallBase(TArgs... args)
    {
        return GameScene::OnStart(args...);
    }

};
GameScene_OnStart_OverrideCaller LNGameScene::m_OnStart_OverrideCaller = nullptr;

class LNCollisionShape : public CollisionShape
{
public:



};

class LNBoxCollisionShape : public BoxCollisionShape
{
public:



};

class LNPhysicsObject : public PhysicsObject
{
public:



};

class LNCollider : public Collider
{
public:



};



