
#pragma once

LN_NAMESPACE_BEGIN
class Camera;
class SceneGraph;
class SceneGraph2D;
class SceneGraph3D;
class PhysicsWorld;

/**
	@brief		
*/
class World
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:


protected:
	virtual SceneGraph* GetSceneGraph() = 0;

LN_CONSTRUCT_ACCESS:
	World();
	virtual ~World();
	void Initialize();

LN_INTERNAL_ACCESS:
	virtual void BeginUpdateFrame();
	virtual void UpdateFrame(float elapsedTime);
	virtual void Render(Camera* camera);
	void ExecuteDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
};

/**
	@brief		
*/
class World2D
	: public World
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

protected:
	virtual SceneGraph* GetSceneGraph() override;

LN_CONSTRUCT_ACCESS:
	World2D();
	virtual ~World2D();
	void Initialize();

LN_INTERNAL_ACCESS:
	SceneGraph2D* GetSceneGraph2D() const;
	virtual void BeginUpdateFrame() override;
	virtual void UpdateFrame(float elapsedTime) override;

private:
	RefPtr<SceneGraph2D>		m_sceneGraph;
};

/**
	@brief		
*/
class World3D
	: public World
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

protected:
	virtual SceneGraph* GetSceneGraph() override;

LN_CONSTRUCT_ACCESS:
	World3D();
	virtual ~World3D();
	void Initialize();

LN_INTERNAL_ACCESS:
	PhysicsWorld* GetPhysicsWorld3D() const;
	SceneGraph3D* GetSceneGraph3D() const;
	virtual void BeginUpdateFrame() override;
	virtual void UpdateFrame(float elapsedTime) override;

private:
	RefPtr<PhysicsWorld>	m_physicsWorld;
	RefPtr<SceneGraph3D>	m_sceneGraph;
};

LN_NAMESPACE_END
