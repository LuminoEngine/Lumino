
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN
class DrawList;
class WorldObject;
class Transform;
class VisualComponent;
class UIEventArgs;

/**
	@brief		
*/
LN_CLASS()
class Component
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	Transform*	transfotm;		/**< アタッチされた WorldObject の transform へのポインタ */

public:
	Component();
	virtual ~Component();
	WorldObject* GetOwnerObject() const;
	Transform* GetTransform() const;

	virtual void OnAttached();
	virtual void OnDetaching();
	virtual void OnUpdate();
	virtual void OnRender(DrawList* context);

protected:
	virtual void OnUIEvent(UIEventArgs* e);

private:
	void Attach(WorldObject* owner);
	void Detach();
	void UpdateFrame();
	virtual void Render(DrawList* context);

	WorldObject*	m_owner;

	friend class WorldObject;
	friend class VisualComponent;
};

/**
	@brief		
*/
class Transform	// TODO: name TransformComponent
	: public Component
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	LN_TR_PROPERTY(Vector3,		position);
	LN_TR_PROPERTY(Quaternion,	rotation);
	LN_TR_PROPERTY(Vector3,		scale);
	LN_TR_PROPERTY(Vector3,		center);
	tr::Property<Vector3>		position;
	tr::Property<Quaternion>	rotation;
	tr::Property<Vector3>		scale;
	tr::Property<Vector3>		center;

public:
	Transform();
	virtual ~Transform();

	/** 指定した方向と距離に移動します。*/
	void Translate(const Vector3& translation);
	void Translate(float x, float y, float z = 0.0f);

	Matrix GetTransformMatrix() const;

	/**
		@brief		ワールド行列を取得します。
		@details	ローカルおよび親トランスフォームから各フレームで自動的に再計算されます。
					UpdateWorldMatrix（）を使用すると、次のフレームの前に強制的に更新することができます。
	*/
	const Matrix& GetWorldMatrix() const { return m_worldMatrix; }
	
	/**
		@brief		ワールド行列を更新します。
	*/
	void UpdateWorldMatrix();

private:
	Transform*	m_parent;
	Matrix		m_worldMatrix;
};

LN_NAMESPACE_END
