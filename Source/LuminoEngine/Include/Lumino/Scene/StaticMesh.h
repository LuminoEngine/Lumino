
#pragma once
#include "../Mesh/Mesh.h"
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
class StaticMesh;
using StaticMeshPtr = RefPtr<StaticMesh>;

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

	/**
		@brief		サイズが 1.0 であるボックスのメッシュを作成します。
		@details	このメッシュは共有リソースです。
					頂点バッファやインデックスバッファを操作すると、このメソッドで作成したほかのメッシュの形状にも影響します。
					通常はこれらのリソースを変更するべきではありません。
	*/
	LN_METHOD()
	static StaticMeshPtr CreateBox();

	/** 各軸に沿ったサイズを指定してボックスのメッシュを作成します。*/
	LN_METHOD()
	static StaticMeshPtr CreateBox(const Vector3& size);

	/** 各軸に沿ったサイズを指定してボックスのメッシュを作成します。*/
	LN_METHOD()
	static StaticMeshPtr CreateBox(float width, float height, float depth);
	
	/**
		@brief		半径が 0.5 である球のメッシュを作成します。
		@details	このメッシュは共有リソースです。
					頂点バッファやインデックスバッファを操作すると、このメソッドで作成したほかのメッシュの形状にも影響します。
					通常はこれらのリソースを変更するべきではありません。
	*/
	LN_METHOD()
	static StaticMeshPtr CreateSphere();

	/** 半径と分割数を指定して球のメッシュを作成します。*/
	LN_METHOD()
	static StaticMeshPtr CreateSphere(float radius, int tessellation = 16);

	/**
		@brief		サイズが 1.0 であるティーポットのメッシュを作成します。
		@details	このメッシュは共有リソースです。
					頂点バッファやインデックスバッファを操作すると、このメソッドで作成したほかのメッシュの形状にも影響します。
					通常はこれらのリソースを変更するべきではありません。
	*/
	LN_METHOD()
	static StaticMeshPtr CreateTeapot();

	/** サイズとなめらかさを指定してメッシュを作成します。*/
	LN_METHOD()
	static StaticMeshPtr CreateTeapot(float size, int tessellation = 8);

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

LN_NAMESPACE_END
