#pragma once
#include "Common.hpp"

namespace ln {
class Material;

/**
 * MeshSurface は Mesh の構成要素のひとつです。
 * DirectXMesh のサブセットや Unity の SubMesh、 Godot の SurfaceData に相当します。
 */
struct MeshSurfaceData {
    // NOTE: Batch とは違うの？
    //   Batch は Transform を含みひとつの描画単位を表すのに対して、
    //   MeshSurfaceData はあくまでメッシュデータ（Resource）の一部です。
    //   MeshSurfaceData は複数の描画で共有できますが、 Batch はそれ自体がひとつの描画単位です。

    // Ref<VertexBuffer> vertexBuffer;    // struct Vertex. (Pos0, Normal0, tangent)
    // Ref<VertexBuffer> attributeBuffer; // struct Vertex. (Color0, UV0, uv2, custom0-3)
    
	Ref<VertexBuffer> mainVertexBuffer;		// struct Vertex. (Pos0, Normal0, UV0, Color0)
    uint32_t vertexCount = 0;
    Ref<IndexBuffer> indexBuffer;
    uint32_t indexCount = 0;
    Ref<VertexLayout> vertexLayout;
    Ref<Material> material;
};

class Mesh : public Object {
public:
    static Ref<Mesh> create();

    // ector<uint8_t> skin_data; // bone index, bone weight
    //AABB aabb;
    //struct LOD {
    //    float edge_length = 0.0f;
    //    Vector<uint8_t> index_data;
    //};
    //Vector<LOD> lods;
    //Vector<AABB> bone_aabbs;

    //Vector<uint8_t> blend_shape_data;

    void addSprite2DSurface(
        Material* material,
        Vector2 size,
        Vector2 anchor,
        Rect uvRect,
        Color color);

public: // TODO: private
    Mesh();
    detail::MeshManager* m_manager;
    Array<MeshSurfaceData> m_surfaces;
};

} // namespace ln
