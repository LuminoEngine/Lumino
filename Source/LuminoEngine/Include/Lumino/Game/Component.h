
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN
class DrawList;
class WorldObject;
class Transform;
class VisualComponent;
class UIEventArgs;

enum class SpecialComponentType
{
	None,
	Visual,
};

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
	void attach(WorldObject* owner);
	void detach();
	void UpdateFrame();

LN_INTERNAL_ACCESS:
	virtual void Render(DrawList* context);
	void SetSpecialComponentType(SpecialComponentType type) { m_specialComponentType = type; }
	SpecialComponentType GetSpecialComponentType() const { return m_specialComponentType; }

private:
	WorldObject*	m_owner;
	SpecialComponentType	m_specialComponentType;

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

	Vector3 getFront() const;

	/** 指定した方向と距離に移動します。*/
	void Translate(const Vector3& translation);
	void Translate(float x, float y, float z = 0.0f);

	void LookAt(const Vector3& target, const Vector3& up = Vector3::UnitY);

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
