
#ifdef LN_USE_FBX_IMPORTER
#include "Internal.hpp"
#include <fbxsdk.h>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include "../Asset/AssetManager.hpp"
#include "FbxImporter.hpp"

namespace ln {
namespace detail {

// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html?url=cpp_ref/_file_stream_2main_8cxx-example.html,topicNumber=cpp_ref__file_stream_2main_8cxx_example_html8d5a461e-c3c6-4926-9292-6a9eb7a8d37e,hash=a7
class FbxStreamWrapper : public ::FbxStream
{
    FbxManager* m_manager;
    Ref<Stream> m_device;
    EState m_state = eEmpty;
    int m_readerId;

    public:
    FbxStreamWrapper(FbxManager* fbxManager, Stream* stream)
        : m_manager(fbxManager)
        , m_device(stream)
    {
        const char* format = "FBX (*.fbx)";
        m_readerId = fbxManager->GetIOPluginRegistry()->FindReaderIDByDescription(format);
    }

    EState GetState() override
    {
        return m_device ? eOpen : eClosed;
    }

    bool Open(void* pStreamData) override
    {
        m_device->seek(0, SeekOrigin::Begin);
        return true;
    }

    bool Close() override
    {
        m_device = nullptr;
        return true;
    }

    bool Flush() override
    {
        LN_UNREACHABLE();
        return true;
    }
    
    int Write(const void* pData, int pSize) override
    {
        LN_UNREACHABLE();
        return 0;
    }

    int Read(void* pData, int pSize) const override
    {
        if (LN_REQUIRE(m_device)) return 0;
        return m_device->read(reinterpret_cast<char*>(pData), pSize);
    }

    char* ReadString(char* pBuffer, int pMaxSize, bool pStopAtFirstWhiteSpace)
    {
        LN_UNREACHABLE();
        return nullptr;
    }

    int GetReaderID() const override
    {
        return m_readerId;
    }

    int GetWriterID() const override
    {
        LN_UNREACHABLE();
        return -1;
    }

    void Seek(const FbxInt64& pOffset, const FbxFile::ESeekPos& pSeekPos) override
    {
        if (LN_REQUIRE(m_device)) return;
        switch (pSeekPos) {
            case FbxFile::eBegin:
                m_device->seek(pOffset, SeekOrigin::Begin);
                break;
            case FbxFile::eCurrent:
                m_device->seek(pOffset, SeekOrigin::Current);
                break;
            case FbxFile::eEnd:
                m_device->seek(pOffset, SeekOrigin::End);
                break;
        }
    }

    long GetPosition() const
    {
        if (LN_REQUIRE(m_device)) return 0;
        return m_device->position();
    }

    void SetPosition(long pPosition) override
    {
        if (LN_REQUIRE(m_device)) return;
        return m_device->seek(pPosition, SeekOrigin::Begin);
    }

    int GetError() const override
    {
        return 0;
    }

    void ClearError() override
    {
    }
};

Ref<MeshModel> FbxImporter::import(AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag)
{
	return nullptr;
}

Ref<SkinnedMeshModel> FbxImporter::importSkinnedMesh(AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag)
{
    //auto basedir = assetPath.getParentAssetPath();
    auto stream = assetManager->openStreamFromAssetPath(assetPath);
    //auto data = stream->readToEnd();



    ::FbxManager* fbxManager = ::FbxManager::Create();
    ::FbxIOSettings* ios = ::FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);
    FbxScene* scene = FbxScene::Create(fbxManager, "");

    {
        FbxStreamWrapper stream(fbxManager, stream);


        ::FbxImporter* importer = ::FbxImporter::Create(fbxManager, "");
        //importer->Initialize("D:/Documents/UnityProjects/New Unity Project/Assets/UnityChan/SD_Kohaku_chanz/Models/Misaki_sum_humanoid.fbx", -1, manager->GetIOSettings());
        importer->Initialize(&stream);
        importer->Import(scene);
        importer->Destroy();

        // 三角形化
        FbxGeometryConverter geometryConverter(fbxManager);
        geometryConverter.Triangulate(scene, true);

        //DisplayContent(scene);
    }


    fbxManager->Destroy();

	return nullptr;
}

} // namespace detail
} // namespace ln

#endif // LN_USE_FBX_IMPORTER
