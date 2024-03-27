
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/MeshVoxelmap/MeshVoxelset.hpp>

#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/Material.hpp>


namespace ln {
namespace detail {

enum class VoxelMeshFaceKind
{
	Convex,		// 面方向に同種の Voxel が存在しない
	Concave,	// 面方向に同種の Voxel が存在する
};

class VoxelmapMeshBuilder
{
public:
	static const Matrix RotationTransforms[6];
	static const Vector3 TransformOffsets[6];

	void beginSection(MeshTileFaceDirection dir, int autovoxelLocalId, VoxelMeshFaceKind faceKind);
	void endSection();



	void beginSubtile();
	void endSubtile();
	// 四角形を生成する。向きは Z+。中心 (0,0,0) で作ること。
	// Subtile の begin/end は不要
	void putSquare(const Vector3& p0, const Vector2& uv0, const Vector3& p1, const Vector2& uv1, const Vector3& p2, const Vector2& uv2, const Vector3& p3, const Vector2& uv3);
	void putSquare(const Vector3& p0, const Vector3& p1,  const Vector3& p2, const Vector3& p3);

	void putSquareChamfer(float l, float t, float r, float b, SubtileCorner corner, float depth);

	void projectUV(const Rect& uvRect);


	// postprocess
	void applyTileCenterMagnitude();


	void build();
	const Ref<MeshPrimitive>& mesh() const { return m_mesh; }
	const std::array<Ref<InstancedMeshList>, 6 * 48>& convexMeshList() const { return m_convexMeshList; }
	const std::array<Ref<InstancedMeshList>, 6 * 48>& concaveMeshList() const { return m_concaveMeshList; }

private:
	struct SectionInfo
	{
		MeshTileFaceDirection dir;
		int autovoxelLocalId;
		VoxelMeshFaceKind faceKind;
		MeshSection2 section;
	};

	MeshTileFaceDirection m_curDir;
	Rect m_curUVRect;
	int m_curAutovoxelLocalId;
	VoxelMeshFaceKind m_curFaceKind;
	int m_startVertex;
	int m_startIndex;
	int m_offsetIndex;
	int m_subtileStartVertex;

	std::vector<Vertex> m_vertices;
	std::vector<uint16_t> m_indices;
	std::vector<SectionInfo> m_sections;
	Ref<MeshPrimitive> m_mesh;
	std::array<Ref<InstancedMeshList>, 6 * 48> m_convexMeshList;
	std::array<Ref<InstancedMeshList>, 6 * 48> m_concaveMeshList;
};

} // namespace detail
} // namespace ln