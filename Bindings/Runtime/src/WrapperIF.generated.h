
#pragma once
#include <LuminoEngine.h>
#include "LuminoC.generated.h"
using namespace ln;

typedef LNResultCode (*GameScene_OnStart_OverrideCaller)(LNHandle gamescene);


class LNSound : public Sound
{
public:

    void postInitialize()
    {

    }






};

class LNTexture : public Texture
{
public:

    void postInitialize()
    {

    }






};

class LNTexture2D : public Texture2D
{
public:

    void postInitialize()
    {

    }






};

class LNGraphicsResourceObject : public GraphicsResourceObject
{
public:

    void postInitialize()
    {

    }






};

class LNSceneNode : public SceneNode
{
public:

    void postInitialize()
    {

    }






};

class LNVisualNode : public VisualComponent
{
public:

    void postInitialize()
    {

    }






};

class LNSprite : public SpriteComponent
{
public:

    void postInitialize()
    {

    }






};

class LNSprite2D : public Sprite2DComponent
{
public:

    void postInitialize()
    {

    }






};

class LNSprite3D : public Sprite3DComponent
{
public:

    void postInitialize()
    {

    }






};

class LNStaticMesh : public StaticMeshComponent
{
public:

    void postInitialize()
    {

    }






};

class LNComponent : public Component
{
public:

    void postInitialize()
    {

    }






};

class LNGameApplication : public GameApplication
{
public:

    void postInitialize()
    {

    }






};

class LNGameScene : public GameScene
{
public:

    void postInitialize()
    {

    }



    static GameScene_OnStart_OverrideCaller m_OnStart_OverrideCaller;


    virtual void onStart() override
    {
        m_OnStart_OverrideCaller(LWIG_TO_HANDLE(this));
    }
    template<typename... TArgs>
    void OnStart_CallBase(TArgs... args)
    {
        return GameScene::onStart(args...);
    }

};
GameScene_OnStart_OverrideCaller LNGameScene::m_OnStart_OverrideCaller = nullptr;

class LNCollisionShape : public CollisionShape
{
public:

    void postInitialize()
    {

    }






};

class LNBoxCollisionShape : public BoxCollisionShape
{
public:

    void postInitialize()
    {

    }






};

class LNPhysicsObject : public PhysicsObject
{
public:

    void postInitialize()
    {

    }






};

class LNCollider : public CollisionBody
{
public:

    void postInitialize()
    {
        ConnectOnTriggerEnter(createDelegate(this, &LNCollider::ConnectOnTriggerEnter_EventCallback));
        ConnectOnTriggerLeave(createDelegate(this, &LNCollider::ConnectOnTriggerLeave_EventCallback));
        ConnectOnTriggerStay(createDelegate(this, &LNCollider::ConnectOnTriggerStay_EventCallback));

    }

    Event<void(LNHandle self, LNHandle obj)> m_ConnectOnTriggerEnter_EventWrapper;
    void ConnectOnTriggerEnter_EventCallback(PhysicsObject* obj)
    {
        m_ConnectOnTriggerEnter_EventWrapper.raise(LWIG_TO_HANDLE(this), LWIG_TO_HANDLE(obj));
    }
    Event<void(LNHandle self, LNHandle obj)> m_ConnectOnTriggerLeave_EventWrapper;
    void ConnectOnTriggerLeave_EventCallback(PhysicsObject* obj)
    {
        m_ConnectOnTriggerLeave_EventWrapper.raise(LWIG_TO_HANDLE(this), LWIG_TO_HANDLE(obj));
    }
    Event<void(LNHandle self, LNHandle obj)> m_ConnectOnTriggerStay_EventWrapper;
    void ConnectOnTriggerStay_EventCallback(PhysicsObject* obj)
    {
        m_ConnectOnTriggerStay_EventWrapper.raise(LWIG_TO_HANDLE(this), LWIG_TO_HANDLE(obj));
    }





};



