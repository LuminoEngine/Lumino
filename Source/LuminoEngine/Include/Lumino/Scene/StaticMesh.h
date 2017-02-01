
#pragma once
#include "../Mesh/Mesh.h"
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
class StaticMesh;
class BoxMesh;
using StaticMeshPtr = RefPtr<StaticMesh>;
using BoxMeshPtr = RefPtr<BoxMesh>;

/**
	@brief		
*/
LN_CLASS()
class StaticMesh
	: public VisualNode
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static StaticMeshPtr Create(const StringRef& filePath);

	static StaticMeshPtr CreateSphere(float radius, int slices, int stacks, MeshCreationFlags = MeshCreationFlags::None);

	static StaticMeshPtr CreatePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags = MeshCreationFlags::None);

	static StaticMeshPtr CreateScreenPlane();

	static StaticMeshPtr CreateTeapot(MeshCreationFlags flags);

public:

	StaticMeshModel* GetStaticMeshModel() const { return m_mesh; }

LN_INTERNAL_ACCESS:
	StaticMesh();
	virtual ~StaticMesh();
	void Initialize(SceneGraph* ownerSceneGraph, StaticMeshModel* meshModel);
	virtual void OnRender2(DrawList* renderer) override;

private:
	RefPtr<StaticMeshModel>	m_mesh;
};


/**
	@brief		軸に沿ったボックスのメッシュを表示するクラスです。
*/
LN_CLASS()
class BoxMesh
	: public StaticMesh
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		サイズが 1.0 であるボックスのメッシュを作成します。
		@details	このメッシュは共有リソースです。
					頂点バッファやインデックスバッファを操作すると、このメソッドで作成したほかのメッシュの形状にも影響します。
					通常はこれらのリソースを変更するべきではありません。
	*/
	LN_METHOD(Document)
	static BoxMeshPtr Create();

	/** 各軸に沿ったサイズを指定してメッシュを作成します。*/
	LN_METHOD(Document)
	static BoxMeshPtr Create(const Vector3& size);

	/** 各軸に沿ったサイズを指定してメッシュを作成します。*/
	LN_METHOD(Document)
	static BoxMeshPtr Create(float width, float height, float depth);

LN_CONSTRUCT_ACCESS:
	BoxMesh();
	virtual ~BoxMesh();

	/** @copydoc Create() */
	LN_METHOD()
	void Initialize();

	/** @copydoc Create(const Vector3& size) */
	LN_METHOD()
	void Initialize(const Vector3& size);

	/** @copydoc Create(float width, float height, float depth) */
	LN_METHOD()
	void Initialize(float width, float height, float depth);
};


LN_NAMESPACE_END
