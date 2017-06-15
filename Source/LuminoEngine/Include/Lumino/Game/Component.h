
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN
class DrawList;
class WorldObject;
class transform;
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
	transform*	transfotm;		/**< アタッチされた WorldObject の transform へのポインタ */

public:
	Component();
	virtual ~Component();
	WorldObject* getOwnerObject() const;
	transform* getTransform() const;

	virtual void onAttached();
	virtual void onDetaching();
	virtual void onUpdate();
	virtual void onRender(DrawList* context);

protected:
	virtual void onUIEvent(UIEventArgs* e);

private:
	void attach(WorldObject* owner);
	void detach();
	void updateFrame();

LN_INTERNAL_ACCESS:
	virtual void render(DrawList* context);
	void setSpecialComponentType(SpecialComponentType type) { m_specialComponentType = type; }
	SpecialComponentType getSpecialComponentType() const { return m_specialComponentType; }

private:
	WorldObject*	m_owner;
	SpecialComponentType	m_specialComponentType;

	friend class WorldObject;
	friend class VisualComponent;
};

/**
	@brief		
*/
class transform	// TODO: name TransformComponent
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
	transform();
	virtual ~transform();

	Vector3 getFront() const;

	/** 指定した方向と距離に移動します。*/
	void translate(const Vector3& translation);
	void translate(float x, float y, float z = 0.0f);

	void lookAt(const Vector3& target, const Vector3& up = Vector3::UnitY);

	Matrix getTransformMatrix() const;

	/**
		@brief		ワールド行列を取得します。
		@details	ローカルおよび親トランスフォームから各フレームで自動的に再計算されます。
					updateWorldMatrix（）を使用すると、次のフレームの前に強制的に更新することができます。
	*/
	const Matrix& getWorldMatrix() const { return m_worldMatrix; }
	
	/**
		@brief		ワールド行列を更新します。
	*/
	void updateWorldMatrix();

private:
	transform*	m_parent;
	Matrix		m_worldMatrix;
};

LN_NAMESPACE_END
