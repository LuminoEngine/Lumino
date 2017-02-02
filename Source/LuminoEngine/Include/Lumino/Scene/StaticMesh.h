
#pragma once
#include "../Mesh/Mesh.h"
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
class StaticMesh;
class BoxMesh;
class SphereMesh;
class TeapotMesh;
using StaticMeshPtr = RefPtr<StaticMesh>;
using BoxMeshPtr = RefPtr<BoxMesh>;
using SphereMeshPtr = RefPtr<SphereMesh>;
using TeapotMeshPtr = RefPtr<TeapotMesh>;

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

	static StaticMeshPtr CreatePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags = MeshCreationFlags::None);

	static StaticMeshPtr CreateScreenPlane();

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

/**
	@brief		球のメッシュを表示するクラスです。
*/
LN_CLASS()
class SphereMesh
	: public StaticMesh
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		半径が 0.5 である球のメッシュを作成します。
		@details	このメッシュは共有リソースです。
					頂点バッファやインデックスバッファを操作すると、このメソッドで作成したほかのメッシュの形状にも影響します。
					通常はこれらのリソースを変更するべきではありません。
	*/
	LN_METHOD(Document)
	static SphereMeshPtr Create();

	/** 半径と分割数を指定してメッシュを作成します。*/
	LN_METHOD(Document)
	static SphereMeshPtr Create(float radius, int tessellation = 16);

LN_CONSTRUCT_ACCESS:
	SphereMesh();
	virtual ~SphereMesh();

	/** @copydoc Create() */
	LN_METHOD()
	void Initialize();

	/** @copydoc Create(float radius, int tessellation) */
	LN_METHOD()
	void Initialize(float radius, int tessellation);
};

/**
	@brief		ティーポットのメッシュを表示するクラスです。
*/
LN_CLASS()
class TeapotMesh
	: public StaticMesh
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		サイズが 1.0 であるティーポットのメッシュを作成します。
		@details	このメッシュは共有リソースです。
					頂点バッファやインデックスバッファを操作すると、このメソッドで作成したほかのメッシュの形状にも影響します。
					通常はこれらのリソースを変更するべきではありません。
	*/
	LN_METHOD(Document)
	static TeapotMeshPtr Create();

	/** サイズとなめらかさを指定してメッシュを作成します。*/
	LN_METHOD(Document)
	static TeapotMeshPtr Create(float size, int tessellation = 8);

LN_CONSTRUCT_ACCESS:
	TeapotMesh();
	virtual ~TeapotMesh();

	/** @copydoc Create() */
	LN_METHOD()
	void Initialize();

	/** @copydoc Create(float size, int tessellation) */
	LN_METHOD()
	void Initialize(float size, int tessellation);
};

LN_NAMESPACE_END
