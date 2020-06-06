
#pragma once
#include "../VisualObject.hpp"

namespace ln {
class BoxMeshComponent;
class SphereMeshComponent;
class PlaneMeshComponent;

/**
 * 直方体のメッシュオブジェクトです。
 */
LN_CLASS()
class BoxMesh
	: public VisualObject
{
public:
	static Ref<BoxMesh> create();

	BoxMeshComponent* boxMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	BoxMesh();
	virtual ~BoxMesh();

	/** 各軸のサイズが 1 である BoxMesh を作成します。 */
	LN_METHOD()
	void init();

private:
    Ref<BoxMeshComponent> m_component;
};

class SphereMesh
	: public VisualObject
{
public:
	static Ref<SphereMesh> create();

	SphereMeshComponent* sphereMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	SphereMesh();
	void init();

private:
	Ref<SphereMeshComponent> m_component;
};

/**
 * 平面 (床・地面) のメッシュオブジェクトです。デフォルトのサイズは、各軸 10 です。
 */
LN_CLASS()
class PlaneMesh
	: public VisualObject
{
public:
	static Ref<PlaneMesh> create();
	static Ref<PlaneMesh> create(Material* material);

	PlaneMeshComponent* planeMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	PlaneMesh();

	/**  */
	LN_METHOD()
	bool init();

	/**  */
	LN_METHOD()
	bool init(Material* material);

private:
	Ref<PlaneMeshComponent> m_component;
};


} // namespace ln
