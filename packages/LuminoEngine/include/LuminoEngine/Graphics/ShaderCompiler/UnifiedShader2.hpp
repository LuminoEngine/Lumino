#pragma once
#include "Common.hpp"

namespace ln {
namespace kokage {

// 変数ひとつ分の情報。
// 今のところ、 ConstantBuffer のメンバーとしてのみ使用しています。。
struct VariableInfo {
    ShaderUniformType type;
    std::string name;
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
struct ModuleParameterInfo {
    RegisterCategory category; // b, t, s, u...
    std::string name;
    std::vector<VariableInfo> constantBufferMembers; // ConstantBuffer members
    int32_t constantBufferSize;
};

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
    std::vector<ModuleParameterInfo> parameters;
};

class UnifiedShader2 : public URefObject {
public:
    UnifiedShader2();

    ModuleInfo* addModuleInfo();

    EntryPoint* createEntryPoint();
    std::pair<int, ShaderPass*> createShaderPass();
    Blob* createBlob();

private:
    std::vector<URef<ModuleInfo>> m_moduleInfos;
    std::vector<URef<EntryPoint>> m_entryPoints;
    std::vector<URef<ShaderPass>> m_shaderPasses;
    std::vector<URef<Blob>> m_blobs;
};

} // namespace kokage
} // namespace ln
