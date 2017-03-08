
#pragma once
#include <LuminoEngine.h>
#include "LuminoC.generated.h"
using namespace ln;

typedef LNResultCode (*GameScene_OnStart_OverrideCaller)(LNHandle gamescene);


class LNSound : public Sound
{
public:

    void PostInitialize()
    {

    }






};

class LNTexture : public Texture
{
public:

    void PostInitialize()
    {

    }






};

class LNTexture2D : public Texture2D
{
public:

    void PostInitialize()
    {

    }






};

class LNGraphicsResourceObject : public GraphicsResourceObject
{
public:

    void PostInitialize()
    {

    }






};

class LNSceneNode : public SceneNode
{
public:

    void PostInitialize()
    {

    }






};

class LNVisualNode : public VisualNode
{
public:

    void PostInitialize()
    {

    }






};

class LNSprite : public Sprite
{
public:

    void PostInitialize()
    {

    }






};

class LNSprite2D : public Sprite2D
{
public:

    void PostInitialize()
    {

    }






};

class LNSprite3D : public Sprite3D
{
public:

    void PostInitialize()
    {

    }






};

class LNStaticMesh : public StaticMesh
{
public:

    void PostInitialize()
    {

    }






};

class LNComponent : public Component
{
public:

    void PostInitialize()
    {

    }






};

class LNGameApplication : public GameApplication
{
public:

    void PostInitialize()
    {

    }






};

class LNGameScene : public GameScene
{
public:

    void PostInitialize()
    {

    }



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

    void PostInitialize()
    {

    }






};

class LNBoxCollisionShape : public BoxCollisionShape
{
public:

    void PostInitialize()
    {

    }






};

class LNPhysicsObject : public PhysicsObject
{
public:

    void PostInitialize()
    {

    }






};

class LNCollider : public Collider
{
public:

    void PostInitialize()
    {
        ConnectOnTriggerEnter(CreateDelegate(this, &LNCollider::ConnectOnTriggerEnter_EventCallback));
        ConnectOnTriggerLeave(CreateDelegate(this, &LNCollider::ConnectOnTriggerLeave_EventCallback));
        ConnectOnTriggerStay(CreateDelegate(this, &LNCollider::ConnectOnTriggerStay_EventCallback));

    }

    Event<void(LNHandle self, PhysicsObject* obj)> m_ConnectOnTriggerEnter_EventWrapper;
    void ConnectOnTriggerEnter_EventCallback(PhysicsObject* obj)
    {
        m_ConnectOnTriggerEnter_EventWrapper.Raise(LWIG_TO_HANDLE(this), obj);
    }
    Event<void(LNHandle self, PhysicsObject* obj)> m_ConnectOnTriggerLeave_EventWrapper;
    void ConnectOnTriggerLeave_EventCallback(PhysicsObject* obj)
    {
        m_ConnectOnTriggerLeave_EventWrapper.Raise(LWIG_TO_HANDLE(this), obj);
    }
    Event<void(LNHandle self, PhysicsObject* obj)> m_ConnectOnTriggerStay_EventWrapper;
    void ConnectOnTriggerStay_EventCallback(PhysicsObject* obj)
    {
        m_ConnectOnTriggerStay_EventWrapper.Raise(LWIG_TO_HANDLE(this), obj);
    }





};



