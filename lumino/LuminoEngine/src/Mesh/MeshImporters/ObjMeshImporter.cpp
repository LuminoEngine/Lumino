
#include "Internal.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include "ObjMeshImporter.hpp"

namespace ln {
namespace detail {

bool hasSmoothingGroup(const tinyobj::shape_t& shape)
{
    return true;
    //for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++) {
    //    if (shape.mesh.smoothing_group_ids[i] > 0) {
    //        return true;
    //    }
    //}
    //return false;
}

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3])
{
    float v10[3];
    v10[0] = v1[0] - v0[0];
    v10[1] = v1[1] - v0[1];
    v10[2] = v1[2] - v0[2];

    float v20[3];
    v20[0] = v2[0] - v0[0];
    v20[1] = v2[1] - v0[1];
    v20[2] = v2[2] - v0[2];

#if 1   // Left hand (Lumino coordinate)
    N[0] = v20[2] * v10[1] - v20[1] * v10[2];
    N[1] = v20[0] * v10[2] - v20[2] * v10[0];
    N[2] = v20[1] * v10[0] - v20[0] * v10[1];
#else   // Right hand
    N[0] = v20[1] * v10[2] - v20[2] * v10[1];
    N[1] = v20[2] * v10[0] - v20[0] * v10[2];
    N[2] = v20[0] * v10[1] - v20[1] * v10[0];
#endif

    float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
    if (len2 > 0.0f) {
        float len = sqrtf(len2);

        N[0] /= len;
        N[1] /= len;
        N[2] /= len;
    }
}

static void computeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape, std::unordered_map<int, Vector3>& smoothVertexNormals)
{
    smoothVertexNormals.clear();
    std::unordered_map<int, Vector3>::iterator iter;

    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
        // Get the three indexes of the face (all faces are triangular)
        tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

        // Get the three vertex indexes and coordinates
        int vi[3];      // indexes
        float v[3][3];  // coordinates

        for (int k = 0; k < 3; k++) {
            vi[0] = idx0.vertex_index;
            vi[1] = idx1.vertex_index;
            vi[2] = idx2.vertex_index;
            assert(vi[0] >= 0);
            assert(vi[1] >= 0);
            assert(vi[2] >= 0);

            v[0][k] = attrib.vertices[3 * vi[0] + k];
            v[1][k] = attrib.vertices[3 * vi[1] + k];
            v[2][k] = attrib.vertices[3 * vi[2] + k];
        }

        // Compute the normal of the face
        float normal[3];
        CalcNormal(normal, v[0], v[1], v[2]);

        // Add the normal to the three vertexes
        for (size_t i = 0; i < 3; ++i) {
            iter = smoothVertexNormals.find(vi[i]);
            if (iter != smoothVertexNormals.end()) {
                // add
                iter->second.x += normal[0];
                iter->second.y += normal[1];
                iter->second.z += normal[2];
            }
            else {
                smoothVertexNormals[vi[i]] = Vector3(normal[0], normal[1], normal[2]);
            }
        }

    }

    // Normalize the normals, that is, make them unit vectors
    for (iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end(); iter++) {
        iter->second.mutatingNormalize();
    }
}


//class LocalBuffer : public std::streambuf
//{
//
//};

class  LocalMaterialFileReader : public tinyobj::MaterialFileReader
{
public:
    LocalMaterialFileReader(const std::string &mtl_basedir)
        : tinyobj::MaterialFileReader(mtl_basedir)
    {}

    virtual bool operator()(const std::string &matId,
        std::vector<tinyobj::material_t> *materials,
        std::map<std::string, int> *matMap, std::string *warn,
        std::string *err) override
    {
        return tinyobj::MaterialFileReader::operator()(matId, materials, matMap, warn, err);
    }
};

//==============================================================================
// ObjMeshImporter

Ref<MeshModel> ObjMeshImporter::import(const Path& filePath, float scale, DiagnosticsManager* diag)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    std::string warn;
	Path parentDirPath = filePath.parent();
    LocalMaterialFileReader materialReader(parentDirPath.str().toStdString() + "/");


    std::ifstream ifs(filePath.str().toStdString());
    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &ifs, &materialReader, true);

    // TODO: message
    if (!warn.empty()) {
        std::cerr << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    std::unordered_map<int, Vector3> smoothVertexNormals;
    auto meshModel = makeObject<MeshModel>();

    for (tinyobj::shape_t& shape : shapes)
    {
        if (shape.mesh.num_face_vertices.size() != shape.mesh.material_ids.size()) {
            diag->reportWarning(_TT("[wavefront .obj] Invalid or unknown format."));
            return nullptr;
        }
        if (!shape.mesh.tags.empty()) {
            diag->reportWarning(_TT("[wavefront .obj] 'tag' is unsupported."));
        }

        if (hasSmoothingGroup(shape)) {
            computeSmoothingNormals(attrib, shape, smoothVertexNormals);
        }

        auto meshContainer = makeObject<MeshContainer>();
        meshContainer->setName(String::fromStdString(shape.name));

        // TODO: 以下の実装では vertex index が index buffer の内容と同じなのでメモリ効率が悪い。
        // 全く同じ要素の頂点を共有するようにすれば効率よくなる。
        // その場合は index_t が一致するものを検索することになる。

        auto meshResource = makeObject<MeshResource>();
        meshResource->resizeVertexBuffer(shape.mesh.indices.size());
        meshResource->resizeIndexBuffer(shape.mesh.indices.size());

        for (int iVertex = 0; iVertex < shape.mesh.indices.size(); iVertex++)
        {
            tinyobj::index_t& vertexIndex = shape.mesh.indices[iVertex];

            Vertex vertex;
            vertex.color = Color(1, 1, 1, 1);

            int v = vertexIndex.vertex_index;
            vertex.setPosition(Vector3(attrib.vertices[3 * v + 0], attrib.vertices[3 * v + 1], attrib.vertices[3 * v + 2]) * scale);

            int n = vertexIndex.normal_index;
            if (n >= 0) {
                vertex.setNormal(Vector3(attrib.normals[3 * n + 0], attrib.normals[3 * n + 1], attrib.normals[3 * n + 2]));
            }
            else {
                vertex.setNormal(smoothVertexNormals[v]);
            }

            int t = vertexIndex.texcoord_index;
            if (t >= 0) {
                vertex.uv = Vector4(attrib.texcoords[2 * t + 0], attrib.texcoords[2 * t + 1], 0.0f, 0.0f);
				vertex.uv.y = 1.0 - vertex.uv.y;	// OpenGL Coord -> DX Coord
            }

            if (attrib.colors.empty()) {
                vertex.color = Color(1, 1, 1, 1);
            }
            else {
                vertex.color = Color(attrib.colors[3 * v + 0], attrib.colors[3 * v + 1], attrib.colors[3 * v + 2], 1);
            }

            meshResource->setVertex(iVertex, vertex);
            meshResource->setIndex(iVertex, iVertex);
        }

        MeshSection section{ 0, 0, -1 };
        for (int iFace = 0; iFace < shape.mesh.num_face_vertices.size(); iFace++)
        {
            if (section.materialIndex == -1) {
                section.materialIndex = shape.mesh.material_ids[iFace];
            }
            else if (shape.mesh.material_ids[iFace] != section.materialIndex)
            {
                meshResource->addSection(section.startIndex, section.primitiveCount, section.materialIndex);
                section.startIndex = iFace * 3;
                section.primitiveCount = 0;
                section.materialIndex = shape.mesh.material_ids[iFace];
            }
            section.primitiveCount++;
        }
        if (section.materialIndex >= 0) {
            meshResource->addSection(section.startIndex, section.primitiveCount, section.materialIndex);
        }

#if 1
        LN_NOTIMPLEMENTED();
#else
        meshContainer->setMeshResource(meshResource);
#endif

        meshModel->addMeshContainer(meshContainer);
    }

    for (tinyobj::material_t& material : materials)
    {
        detail::PhongMaterialData materialData;
        materialData.diffuse.r = material.diffuse[0];
        materialData.diffuse.g = material.diffuse[1];
        materialData.diffuse.b = material.diffuse[2];
        materialData.diffuse.a = material.dissolve;
        materialData.ambient.r = material.ambient[0];
        materialData.ambient.g = material.ambient[1];
        materialData.ambient.b = material.ambient[2];
        materialData.ambient.a = 1.0f;
        materialData.specular.r = material.specular[0];
        materialData.specular.g = material.specular[1];
        materialData.specular.b = material.specular[2];
        materialData.specular.a = 1.0f;
        materialData.emissive.r = material.emission[0];
        materialData.emissive.g = material.emission[1];
        materialData.emissive.b = material.emission[2];
        materialData.emissive.a = 1.0f;
        materialData.power = material.shininess;

        Ref<Texture2D> texture = nullptr;//makeObject<Texture2D>(_TT("D:/tmp/110220c_as019.png");
        if (!material.diffuse_texname.empty())
            texture = Texture2D::load(Path(parentDirPath, String::fromStdString(material.diffuse_texname)));
        auto m = makeObject<Material>(texture, materialData);
        meshModel->addMaterial(m);
    }

    return meshModel;
}

} // namespace detail
} // namespace ln
