
#pragma once
#include "../VisualObject.hpp"
#include "../Mesh/StaticMesh.hpp"

namespace ln {
//class BoxMeshComponent;
class SphereMeshComponent;
class PlaneMeshComponent;

/**
 */
LN_CLASS()
class ShapeObject
	: public VisualObject
{
	LN_OBJECT;
	LN_BUILDER;
public:
	void setMaterial(Material* material);
	Material* material() const;

LN_CONSTRUCT_ACCESS:
	ShapeObject();
	bool init();
};

/**
 * 平面 (床・地面) のメッシュオブジェクトです。デフォルトのサイズは、各軸 10 です。
 */
LN_CLASS()
class PlaneMesh
	: public ShapeObject
{
	LN_OBJECT;
	LN_BUILDER;
public:
	static Ref<PlaneMesh> create();
	static Ref<PlaneMesh> create(Material* material);

	PlaneMeshComponent* planeMeshComponent() const;

protected:
	void serialize(Serializer2& ar) override;

LN_CONSTRUCT_ACCESS:
	PlaneMesh();

	/**  */
	LN_METHOD()
		bool init();

	/**  */
	bool init(Material* material);

private:

	Ref<PlaneMeshComponent> m_component;
};

/**
 * 直方体のメッシュオブジェクトです。
 */
LN_CLASS()
class BoxMesh
	: public StaticMesh
{
	LN_BUILDER;
public:
	/** サイズを変更します。(メッシュは再構築されます) */
	void setSize(const Vector3& size);

	//BoxMeshComponent* boxMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	BoxMesh();
	virtual ~BoxMesh();
	bool init(const Vector3& size, Material* material);

	/** 各軸のサイズが 1 である BoxMesh を作成します。 */
	LN_METHOD()
	bool init();
	
	/** サイズを指定して BoxMesh を作成します。 */
	LN_METHOD(OverloadPostfix="WithSize")
	bool init(const Vector3& size);

	/** サイズを指定して BoxMesh を作成します。 */
	LN_METHOD(OverloadPostfix="WithSizeWHD")
	bool init(float width, float height, float depth) { return init(Vector3(width, height, depth)); }


private:
    //Ref<BoxMeshComponent> m_component;
};

class SphereMesh
	: public ShapeObject
{
	LN_BUILDER;
public:
	static Ref<SphereMesh> create();

	SphereMeshComponent* sphereMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	SphereMesh();
	bool init();

private:
	Ref<SphereMeshComponent> m_component;
};

//==============================================================================
// ShapeObject::Builder

struct ShapeObject::BuilderDetails : public VisualObject::BuilderDetails
{
	LN_BUILDER_DETAILS(ShapeObject);

	Ref<Material> material;

	void apply(ShapeObject* p) const;
};

template<class T, class B, class D>
struct ShapeObject::BuilderCore : public VisualObject::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(VisualObject::BuilderCore);

	B& material(Material* value) { d()->material = value; return self(); }
};

LN_BUILDER_IMPLEMENT(ShapeObject);

//==============================================================================
// PlaneMesh::Builder

struct PlaneMesh::BuilderDetails : public ShapeObject::BuilderDetails
{
	LN_BUILDER_DETAILS(PlaneMesh);

	void apply(PlaneMesh* p) const;
};

template<class T, class B, class D>
struct PlaneMesh::BuilderCore : public ShapeObject::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(ShapeObject::BuilderCore);
};

LN_BUILDER_IMPLEMENT(PlaneMesh);

//==============================================================================
// BoxMesh::Builder

struct BoxMesh::BuilderDetails : public StaticMesh::BuilderDetails
{
	LN_BUILDER_DETAILS(BoxMesh);

	Vector3 size;
	Ref<Material> material;

	BuilderDetails();
	Ref<Object> create() const;
};

template<class T, class B, class D>
struct BoxMesh::BuilderCore : public StaticMesh::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(StaticMesh::BuilderCore);

	B& size(float x, float y, float z) { d()->size = Vector3(x, y, z); return self(); }
	B& material(Material* value) { d()->material = value; return self(); }
};

LN_BUILDER_IMPLEMENT(BoxMesh);

//==============================================================================
// SphereMesh::Builder

struct SphereMesh::BuilderDetails : public ShapeObject::BuilderDetails
{
	LN_BUILDER_DETAILS(SphereMesh);

	void apply(SphereMesh* p) const;
};

template<class T, class B, class D>
struct SphereMesh::BuilderCore : public ShapeObject::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(ShapeObject::BuilderCore);
};

LN_BUILDER_IMPLEMENT(SphereMesh);

} // namespace ln
