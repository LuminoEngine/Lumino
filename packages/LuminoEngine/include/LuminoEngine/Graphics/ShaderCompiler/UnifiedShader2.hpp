#pragma once
#include "Common.hpp"

namespace ln {
namespace kokage {

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


struct EntryPointBindingInfo {
    std::string name;
    RegisterCategory category;
    int offset;
    int size;
    int space;
    int index;
    int count;
    bool used;
};

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
    int index;
    std::string name;
    std::vector<EntryPointBindingInfo> bindings;
    int codeBlobIndex;
};

class ShaderPass : public URefObject {
public:
    int index;
    std::string name;
    int vertBlobIndex;
    int fragBlobIndex;
    int compBlobIndex;
};

// 任意データ
class Blob : public URefObject {
public:
    int index;
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
class InputResourceInfo : public URefObject {
public:
    int index;
    std::string name;
    RegisterCategory category;
    //int constantBufferSize;
    //int spaceIndex;
    //int bindingIndex;
    //int arrayElementCount;




private:
};

// $Global ConstantBuffer のメンバー情報。
class GlobalMemberInfo : public URefObject {
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
    //   シェーダコードの制約として常に 16byte alignment になるように
    //   uniform の定義を調整してもらう案も考えたが、
    //   cbuffer ならともかく普通のグローバル変数でそれを意識するのは大変だと思うので、
    //   $Global に限り、Lumino が内部で調整することにする。
    //
    // NOTE: トップレベルの変数しか扱わない。
    //   例えば、 struct のメンバーや配列インデックス指定での set は扱わない。
    //   セットするデータのサイズはユーザープログラムで守るものとする。
    //   というか、複雑になりすぎるのと、そこまで細かくできるようにする必要は無いと思うので。
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

class UnifiedShader2 : public URefObject {
public:
    UnifiedShader2();

    InputResourceInfo* createInputResourceInfo();
    ModuleInfo* addModuleInfo();
    EntryPoint* createEntryPoint();
    std::pair<int, ShaderPass*> createShaderPass();
    Blob* createBlob();

    Result<GlobalMemberInfo*> getOrCreateGlobalMemberWithVerify(
        std::string name,
        ShaderGlobalMemberType type,
        ShaderGlobalMemberKind kind,
        int32_t arrayElements,
        int32_t vectorElements,
        int32_t matrixRows,
        int32_t matrixColumns);

    Result<InputResourceInfo*> getOrCreateInputResourceWithVerify(
        const std::string& name,
        RegisterCategory category,
        int constantBufferSize,
        int arrayElementCount);

private:
    std::vector<URef<GlobalMemberInfo>> m_globalMembers;
    std::vector<URef<InputResourceInfo>> m_inputResourceInfos;
    std::vector<URef<ModuleInfo>> m_moduleInfos;
    std::vector<URef<EntryPoint>> m_entryPoints;
    std::vector<URef<ShaderPass>> m_shaderPasses;
    std::vector<URef<Blob>> m_blobs;
};

} // namespace kokage
} // namespace ln
