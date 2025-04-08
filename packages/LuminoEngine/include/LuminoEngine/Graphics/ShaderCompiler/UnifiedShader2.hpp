#pragma once
#include "Common.hpp"

namespace ln {
namespace kokage {
class TargetBindingLayout;
using TargetInputResourceInfoId = int32_t;
using TargetShaderPassId = int32_t;
using EntryPointId = int32_t;
using BlobId = int32_t;

// 変数ひとつ分の情報。
// 今のところ、 ConstantBuffer のメンバーとしてのみ使用しています。。
struct VariableInfo {
    std::string name;
    ShaderUniformType type;
    int32_t offset;
    int32_t size;
    int32_t arrayElements;
    int32_t vectorElements;
    int32_t matrixRows;
    int32_t matrixColumns;
};


//struct EntryPointBindingInfo {
//    std::string name;
//    RegisterCategory category;
//    int offset;
//    int size;
//    int space;
//    int index;
//    int count;
//    bool used;
//};

using GlobalResourceSlotInfoId = int32_t;

struct GlobalResourceSlotInfo {
    //GlobalResourceSlotInfoId id;
    std::string name;
    //RegisterCategory category;
    int constantBufferSize;
    int arrayElementCount;
};

// Slang のリフレクション情報を、 Lumino の標準的な分類にマッピングするためのもの。
// 単純に、ひとつの Shader ファイルがどんなリソースを要求するのかを示す。
// Material クラスでセットできる値を示す。
class GlobalResourceLayout : public URefObject {
public:
    std::vector<GlobalResourceSlotInfo> buffers; // $Global 相当の ConstantBuffer が存在する場合、[0] はそれになる。
    std::vector<GlobalResourceSlotInfo> textures;
    std::vector<GlobalResourceSlotInfo> samplers;
    std::vector<GlobalResourceSlotInfo> storages;

    RegisterCategory getRegisterCategoryByName(const std::string& name) const;
};


struct TargetBindingConstantBufferMemberInfo {
    std::string name;
    int offset;
    int size;
};

struct TargetBindingInfo {
    std::string name;
    BindingResourceCategory category;
    int size;
    int space;
    int index;
    int count;
    ShaderStageFlags used;
    std::vector<TargetBindingConstantBufferMemberInfo> members; // ConstantBuffer のメンバー情報
    
    // 一時情報。
    // この binding が、 CombinedSampler の SamplerState 側である場合、
    // その対応する Texture の name を示す。
    //int combinedSamplerIndex = -1;
    std::string combinedSamplerName;

    // 後処理で設定される。
    kokage::RegisterCategory descriptorEntryCategory; // GlobalShaderPass::DescriptorLayout 内のどの register に対応するか
    int descriptorEntryIndex;    // その register の何番目の要素と対応するか
};

struct TargetBindingLayoutInfo {
    std::vector<TargetBindingInfo> bindings;
};

//struct TargetBindingDescripterMapper {
//
//    //std::vector<GlobalResourceSlotInfo> buffers;
//    //std::vector<GlobalResourceSlotInfo> textures;
//    //std::vector<GlobalResourceSlotInfo> samplers;
//    //std::vector<GlobalResourceSlotInfo> storages;
//};

// Pipeline にバインドできる単位の Parameter。
// ConstantBuffer, Texture, SamplerState, StorageBuffer。
//
// なおグローバルスコープに定義された uniform 変数は、
// https://shader-slang.org/slang/user-guide/reflection.html#programs-and-scopes
// こちらように $Global のような ConstantBuffer に含まれる。
//struct ModuleParameterInfo {
//    RegisterCategory category; // b, t, s, u...
//    std::string name;
//    std::vector<VariableInfo> constantBufferMembers; // ConstantBuffer members
//    int32_t constantBufferSize;
//};

struct Reflection {};

struct Component {
    std::vector<uint8_t> code;
};

class EntryPoint : public URefObject {
public:
    EntryPointId index;
    ShaderTarget target;
    std::string name;
    //std::vector<EntryPointBindingInfo> bindings;
    int codeBlobIndex;
    TargetBindingLayoutInfo bindingLayout; // Leaf. これをもとに ShaderPass へマージしてく。
    std::vector<VertexInputAttribute> inputAttributes;
};

class GlobalShaderPass : public URefObject {
public:
    struct DescriptorLayoutEntry {
        int index;                         // その register の何番目の要素から値を取得するか
    };

    // Lumino 用 ShaderDescriptor のレイアウト情報。
    struct DescriptorLayout {
        std::vector<int> buffers; // Index of GlobalResourceLayout::buffers
        std::vector<int> textures;
        std::vector<int> samplers;
        std::vector<int> storages;
    };

    UnifiedShader2* m_owner;
    int index;
    std::string name;
    std::string vertexEntryPoint;
    std::string fragmentEntryPoint;
    std::string computeEntryPoint;
    std::array<TargetShaderPassId, 4> targetShaderPassIndices; // index is (ShaderTarget - 1).
    DescriptorLayout descriptorLayout;

    GlobalShaderPass(UnifiedShader2* owner)
        : m_owner(owner)
        , index(-1)
        , name()
        , vertexEntryPoint()
        , fragmentEntryPoint()
        , computeEntryPoint()
        , targetShaderPassIndices{ -1, -1, -1, -1 } {}

    TargetShaderPassId getTargetShaderPassId(kokage::ShaderTarget target) const {
        return targetShaderPassIndices[target - 1];
    }

    void getOrCreateDescriptorLayoutEntry(
        const TargetBindingInfo& bindingInfo, kokage::RegisterCategory* outCategory,
        int* outIndex);

};

class TargetInputResourceInfo : public URefObject {
public:
    TargetInputResourceInfoId index;
    std::string name;
    RegisterCategory category;
};


class TargetShaderPass : public URefObject {
public:
    TargetShaderPassId index;
    int globalShaderPassIndex;
    EntryPointId vertEntryPointIndex;
    EntryPointId fragEntryPointIndex;
    EntryPointId compEntryPointIndex;
    TargetBindingLayoutInfo bindingLayout; // 各 EntryPoint からマージされたもの。

    TargetShaderPass()
        : index(-1)
        , globalShaderPassIndex(-1)
        , vertEntryPointIndex(-1)
        , fragEntryPointIndex(-1)
        , compEntryPointIndex(-1) {}
};

// 任意データ
class Blob : public URefObject {
public:
    BlobId index;
    std::vector<uint8_t> data;
};

// ひとつの Shader 全体のインスタンス。
class ModuleInfo : public URefObject {
public:
    ShaderTarget target;
    //std::vector<ModuleParameterInfo> parameters;
};

//class ConstantBufferMember : public URefObject {
//public:
//    std::string name;
//    ShaderUniformType type;
//    // uint16_t ownerBufferIndex;
//
//    uint16_t offset; // UniformBuffer 先頭からのオフセットバイト数
//    // uint16_t size;		// Uniform 1つ分の全体サイズ (配列、行列の分も含む)
//
//    uint16_t vectorElements;
//    uint16_t arrayElements; // 配列要素数。配列で廃場合は 0
//    uint16_t matrixRows;
//    uint16_t matrixColumns;
//};

// シェーダファイルひとつ分の、Input Resource.
// グローバルスコープで定義されている、b, t, s, u... などのリソース。
// これは、 Material クラスにセットできる値を示す。
//
//class GlobalInputResourceInfo : public URefObject {
//public:
//    int index;
//    std::string name;
//    RegisterCategory category;
//    //int constantBufferSize;
//    //int spaceIndex;
//    //int bindingIndex;
//    //int arrayElementCount;
//
//
//
//
//private:
//};

// $Global ConstantBuffer のメンバー情報。
class GlobalMemberInfo : public URefObject {
public:
    int index;
    std::string name;
    ShaderGlobalMemberType type;
    ShaderGlobalMemberKind kind;
    int32_t arrayElements;  // Array only.
    int32_t vectorElements; // Vector only.
    int32_t matrixRows;     // Matrix only.
    int32_t matrixColumns;  // Matrix only.
    // NOTE: size は持てない。例えば WGSL と DX12 では float3 のサイズがそれぞれ 12byte, 16byte になる。Target ごとで管理する必要がある。
};

class UnifiedShader2 : public RefObject {
    // NOTE: alignment は Target ごとに異なるため、
    //   Shader 全体で ConstantBuffer のサイズを確定することはできない。
    //   例えば
    //   ```
    //   uniform float value1;
    //   uniform float3 value2;
    //   ```
    //   という $Global があった場合、 value2 の offset は次のようになる。
    //   - DXIL: 4
    //   - SPIRV, WGSL, METAL: 16
    //   こういった事情があるためか、 getGlobal~() の関数は、 Target によって変な値を返すことがある。
    //   https://shader-slang.org/slang/user-guide/reflection.html#calculating-cumulative-offsets
    //   ↑こちらでも説明されているように、使用は推奨されないように見える。
    //
    //   シェーダコードの制約として常に 16byte alignment になるように uniform の定義を調整してもらう案も考えたが、
    //   しかし cbuffer ならともかく普通のグローバル変数でそれを意識するのは大変だと思うので、
    //   $Global に限り、Lumino が内部で調整することにする。
    //
    // NOTE: トップレベルの変数しか扱わない。
    //   例えば、 struct のメンバーや配列インデックス指定での set は扱わない。
    //   セットするデータのサイズはユーザープログラムで守るものとする。
    //   というか、複雑になりすぎるのと、そこまで細かくできるようにする必要は無いと思うので。
public:
    UnifiedShader2();

    GlobalResourceLayout* globalResourceLayout() const;
    const std::vector<URef<GlobalShaderPass>>& globalShaderPasses() const {
        return m_globalShaderPasses;
    }
    const std::vector<URef<EntryPoint>>& entryPoints() const { return m_entryPoints; }
    const std::vector<URef<TargetShaderPass>>& targetShaderPasses() const {
        return m_targetShaderPasses;
    }

    EntryPoint* entryPoint(EntryPointId id) const { return m_entryPoints[id].get(); }

    TargetShaderPass* targetShaderPass(TargetShaderPassId id) const {
        return m_targetShaderPasses[id].get();
    }
    Blob* blob(BlobId id) const { return m_blobs[id].get(); }

    GlobalShaderPass* createGlobalShaderPass();
    ModuleInfo* addModuleInfo();
    EntryPoint* createEntryPoint();
    TargetShaderPass* createTargetShaderPass();
    TargetInputResourceInfo* createTargetInputResourceInfo();
    Blob* createBlob();

    Result_deprecated<GlobalMemberInfo*> getOrCreateGlobalMemberWithVerify(
        std::string name,
        ShaderGlobalMemberType type,
        ShaderGlobalMemberKind kind,
        int32_t arrayElements,
        int32_t vectorElements,
        int32_t matrixRows,
        int32_t matrixColumns);

    MaybeResult_deprecated getOrCreateInputResourceWithVerify(
        const std::string& name,
        RegisterCategory category,
        int constantBufferSize,
        int arrayElementCount);

    Result_deprecated<EntryPoint*> getEntryPoint(ShaderTarget target, const std::string& name) const;

    static MaybeResult_deprecated mergeTargetBindingLayoutInfo(
        TargetBindingLayoutInfo& target, const TargetBindingLayoutInfo& other, bool reset);

    MaybeResult_deprecated buildDescriptorLayout();

private:
    URef<GlobalResourceLayout> m_globalResourceLayout;
    std::vector<URef<GlobalMemberInfo>> m_globalMembers;
    std::vector<URef<GlobalShaderPass>> m_globalShaderPasses;
    std::vector<URef<ModuleInfo>> m_moduleInfos;
    std::vector<URef<EntryPoint>> m_entryPoints;
    std::vector<URef<TargetShaderPass>> m_targetShaderPasses;
    std::vector<URef<TargetInputResourceInfo>> m_TargetInputResourceInfos;
    std::vector<URef<Blob>> m_blobs;
};

} // namespace kokage
} // namespace ln
