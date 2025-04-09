#pragma once
#include "Common.hpp"

namespace ln {
namespace kokage {
class TargetBindingLayout;
using GlobalConstantBufferMemberId = int16_t; /**< ID (-1: Invalid, 0~:Valid) */
using GlobalShaderPassId = int16_t; /**< ID (-1: Invalid, 0~:Valid) */
using TargetShaderPassId = int16_t; /**< ID (-1: Invalid, 0~:Valid) */
using TargetEntryPointId = int16_t; /**< ID (-1: Invalid, 0~:Valid) */
using BlobId = int16_t; /**< ID (-1: Invalid, 0~:Valid) */

struct GlobalResourceSlotInfo {
    std::string name;
    int16_t constantBufferSize;
    int16_t arrayElementCount;
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
    int16_t offset;
    int16_t size;
};

struct TargetBindingInfo {
    std::string name;
    BindingResourceCategory category;
    int16_t size;
    int16_t space;
    int16_t index;
    int16_t count;
    ShaderStageFlags used;
    std::vector<TargetBindingConstantBufferMemberInfo> members; // ConstantBuffer のメンバー情報
    
    // 一時情報。保存はしない。
    // この binding が、 CombinedSampler の SamplerState 側である場合、
    // その対応する Texture の name を示す。
    //int combinedSamplerIndex = -1;
    std::string combinedSamplerName;

    // この TargetBindingInfo が TargetShaderPass のものであれば、後処理で設定される。
    kokage::RegisterCategory descriptorEntryCategory = RegisterCategory_UniformBuffer; // GlobalShaderPass::DescriptorLayout 内のどの register に対応するか
    int16_t descriptorEntryIndex = -1; // その register の何番目の要素と対応するか
};

struct TargetBindingLayoutInfo {
    std::vector<TargetBindingInfo> bindings;
};

/**
 * The entry point for the target.
 * 
 * For example, if your shader file has vs_main and fs_main, and you are building with
 * 4 targets (SPIRV, DXIL, METAL, WGSL), a total of eight EntryPoints will be created.
 */
class TargetEntryPoint : public URefObject {
public:
    TargetEntryPointId id;
    ShaderTarget target;
    std::string name;
    BlobId codeBlobId;
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
        std::vector<int16_t> buffers; // Index of GlobalResourceLayout::buffers
        std::vector<int16_t> textures;
        std::vector<int16_t> samplers;
        std::vector<int16_t> storages;
    };

    UnifiedShader2* m_owner;
    GlobalShaderPassId id;
    std::string name;
    std::string vertexEntryPoint;
    std::string fragmentEntryPoint;
    std::string computeEntryPoint;
    std::vector<TargetShaderPassId> targetShaderPassIds; // index is (ShaderTarget - 1).
    DescriptorLayout descriptorLayout;

    GlobalShaderPass(UnifiedShader2* owner)
        : m_owner(owner)
        , id(-1)
        , name()
        , vertexEntryPoint()
        , fragmentEntryPoint()
        , computeEntryPoint()
        , targetShaderPassIds{ -1, -1, -1, -1 } {}

    TargetShaderPassId getTargetShaderPassId(kokage::ShaderTarget target) const {
        return targetShaderPassIds[target - 1];
    }
};

class TargetShaderPass : public URefObject {
public:
    TargetShaderPassId id;
    //GlobalShaderPassId globalShaderPassId;
    TargetEntryPointId vertEntryPointId;
    TargetEntryPointId fragEntryPointId;
    TargetEntryPointId compEntryPointId;
    TargetBindingLayoutInfo bindingLayout; // 各 TargetEntryPoint からマージされたもの。

    TargetShaderPass()
        : id(-1)
        //, globalShaderPassId(-1)
        , vertEntryPointId(-1)
        , fragEntryPointId(-1)
        , compEntryPointId(-1) {}
};

// 任意データ
class Blob : public URefObject {
public:
    BlobId id;
    std::vector<uint8_t> data;
};

// $Global ConstantBuffer のメンバー情報。
class GlobalConstantBufferMember : public URefObject {
public:
    GlobalConstantBufferMemberId id;
    std::string name;
    ShaderGlobalMemberType type;
    ShaderGlobalMemberKind kind;
    int32_t arrayElements;  // Array only.
    int32_t vectorElements; // Vector only.
    int32_t matrixRows;     // Matrix only.
    int32_t matrixColumns;  // Matrix only.
    // NOTE: size は持てない。例えば WGSL と DX12 では float3 のサイズがそれぞれ 12byte, 16byte になる。Target ごとで管理する必要がある。
    // NOTE: Slang ではメンバごとの used を求めることはできない。
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
    const std::vector<URef<GlobalShaderPass>>& globalShaderPasses() const;
    const std::vector<URef<TargetShaderPass>>& targetShaderPasses() const;
    const std::vector<URef<TargetEntryPoint>>& targetEntryPoints() const;

    TargetShaderPass* targetShaderPass(TargetShaderPassId id) const;
    TargetEntryPoint* targetEntryPoint(TargetEntryPointId id) const;
    Blob* blob(BlobId id) const;

    GlobalConstantBufferMember* createGlobalConstantBufferMember();
    GlobalShaderPass* createGlobalShaderPass();
    TargetShaderPass* createTargetShaderPass();
    TargetEntryPoint* createEntryPoint();
    Blob* createBlob();

    Result<GlobalConstantBufferMember*> getOrCreateGlobalMemberWithVerify(
        std::string name,
        ShaderGlobalMemberType type,
        ShaderGlobalMemberKind kind,
        int32_t arrayElements,
        int32_t vectorElements,
        int32_t matrixRows,
        int32_t matrixColumns);

    MaybeResult getOrCreateInputResourceWithVerify(
        const std::string& name,
        RegisterCategory category,
        int constantBufferSize,
        int arrayElementCount);

    Result<TargetEntryPoint*> getTargetEntryPoint(ShaderTarget target, const std::string& name) const;

    static MaybeResult mergeTargetBindingLayoutInfo(
        TargetBindingLayoutInfo& target, const TargetBindingLayoutInfo& other, bool reset);

private:
    URef<GlobalResourceLayout> m_globalResourceLayout;
    std::vector<URef<GlobalConstantBufferMember>> m_globalMembers;
    std::vector<URef<GlobalShaderPass>> m_globalShaderPasses;
    std::vector<URef<TargetShaderPass>> m_targetShaderPasses;
    std::vector<URef<TargetEntryPoint>> m_targetEntryPoints;
    std::vector<URef<Blob>> m_blobs;

    friend class UnifiedShaderSerializer;
};

} // namespace kokage
} // namespace ln
