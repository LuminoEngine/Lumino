// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#include "pch.hpp"
#include <lumino_shader/UnifiedShader.hpp>
#include <lumino_shader/ShaderCompiler.hpp>
#include "ShaderMetadataParser.hpp"
#include "DescriptorLayoutBuilder.hpp"

#ifdef LUMINO_USE_SLANG

#ifdef _WIN32
#include <Windows.h>
#endif

namespace ln {
namespace shader {

namespace {

static const struct {
    ShaderTarget target;
    const char* profile;
} kTargets[] = {
    { ShaderTarget_SPIRV, "glsl_450" },
    { ShaderTarget_DXIL, "sm_6_0" },
    { ShaderTarget_WGSL, "" },
    { ShaderTarget_METAL, "glsl_450" },
};
static const int kTargetCount = sizeof(kTargets) / sizeof(kTargets[0]);

struct BindingDumpInfo {
    std::string name;
    SlangParameterCategory category;
    int offset;
    int size;
    int space;
    int index;
    int count;
    int used;
};

struct VaryingDumpInfo {
    std::string name;
    std::string semanticName;
    int semanticIndex;
    int locationIndex;
};

static SlangCompileTarget toSlangTarget(ShaderTarget target) {
    switch (target) {
        case ShaderTarget_SPIRV:  return SLANG_SPIRV;
        case ShaderTarget_DXIL:   return SLANG_DXIL;
        case ShaderTarget_WGSL:   return SLANG_WGSL;
        case ShaderTarget_METAL:  return SLANG_METAL;
        default:                  return SLANG_SPIRV;
    }
}

static RegisterCategory toLuminoCategory(SlangParameterCategory category) {
    switch (category) {
        case SLANG_PARAMETER_CATEGORY_CONSTANT_BUFFER:   return RegisterCategory_ConstantBuffer;
        case SLANG_PARAMETER_CATEGORY_SHADER_RESOURCE:   return RegisterCategory_TextureOrCombinedSampler;
        case SLANG_PARAMETER_CATEGORY_UNORDERED_ACCESS:  return RegisterCategory_UnorderdAccess;
        case SLANG_PARAMETER_CATEGORY_SAMPLER_STATE:     return RegisterCategory_SamplerState;
        default:                                         return RegisterCategory_Unknown;
    }
}

static ShaderGlobalMemberType toLuminoShaderGlobalMemberType(
    slang::TypeReflection::ScalarType scalarType) {
    switch (scalarType) {
        case slang::TypeReflection::ScalarType::Bool:    return ShaderGlobalMemberType_Bool;
        case slang::TypeReflection::ScalarType::Int32:   return ShaderGlobalMemberType_Int;
        case slang::TypeReflection::ScalarType::Float32: return ShaderGlobalMemberType_Float;
        default:                                         return ShaderGlobalMemberType_Unknown;
    }
}

static const char* getTargetName(ShaderTarget target) {
    switch (target) {
        case ShaderTarget_SPIRV:  return "SPIRV";
        case ShaderTarget_DXIL:   return "DXIL";
        case ShaderTarget_WGSL:   return "WGSL";
        case ShaderTarget_METAL:  return "METAL";
        default:                  return "?";
    }
}

static const char* getExt(ShaderTarget target) {
    switch (target) {
        case ShaderTarget_SPIRV:  return ".spv";
        case ShaderTarget_DXIL:   return ".dxil";
        case ShaderTarget_WGSL:   return ".wgsl";
        case ShaderTarget_METAL:  return ".msl";
        default:                  return "?";
    }
}

static const char* getSlangCategoryName(SlangParameterCategory category) {
    switch (category) {
        case SLANG_PARAMETER_CATEGORY_NONE:                          return "None";
        case SLANG_PARAMETER_CATEGORY_MIXED:                         return "Mixed";
        case SLANG_PARAMETER_CATEGORY_CONSTANT_BUFFER:               return "ConstantBuffer";
        case SLANG_PARAMETER_CATEGORY_SHADER_RESOURCE:               return "ShaderResource";
        case SLANG_PARAMETER_CATEGORY_UNORDERED_ACCESS:              return "UnorderedAccess";
        case SLANG_PARAMETER_CATEGORY_VARYING_INPUT:                 return "VaryingInput";
        case SLANG_PARAMETER_CATEGORY_VARYING_OUTPUT:                return "VaryingOutput";
        case SLANG_PARAMETER_CATEGORY_SAMPLER_STATE:                 return "SamplerState";
        case SLANG_PARAMETER_CATEGORY_UNIFORM:                       return "Uniform";
        case SLANG_PARAMETER_CATEGORY_DESCRIPTOR_TABLE_SLOT:         return "DescriptorTableSlot";
        case SLANG_PARAMETER_CATEGORY_SPECIALIZATION_CONSTANT:       return "SpecializationConstant";
        case SLANG_PARAMETER_CATEGORY_PUSH_CONSTANT_BUFFER:          return "PushConstantBuffer";
        case SLANG_PARAMETER_CATEGORY_REGISTER_SPACE:                return "RegisterSpace";
        case SLANG_PARAMETER_CATEGORY_GENERIC:                       return "Generic";
        case SLANG_PARAMETER_CATEGORY_RAY_PAYLOAD:                   return "RayPayload";
        case SLANG_PARAMETER_CATEGORY_HIT_ATTRIBUTES:                return "HitAttributes";
        case SLANG_PARAMETER_CATEGORY_CALLABLE_PAYLOAD:              return "CallablePayload";
        case SLANG_PARAMETER_CATEGORY_SHADER_RECORD:                 return "ShaderRecord";
        case SLANG_PARAMETER_CATEGORY_EXISTENTIAL_TYPE_PARAM:        return "ExistentialTypeParam";
        case SLANG_PARAMETER_CATEGORY_EXISTENTIAL_OBJECT_PARAM:      return "ExistentialObjectParam";
        case SLANG_PARAMETER_CATEGORY_SUB_ELEMENT_REGISTER_SPACE:    return "SubElementRegisterSpace";
        case SLANG_PARAMETER_CATEGORY_SUBPASS:                       return "Subpass";
        case SLANG_PARAMETER_CATEGORY_METAL_ARGUMENT_BUFFER_ELEMENT: return "MetalArgumentBufferElement";
        case SLANG_PARAMETER_CATEGORY_METAL_ATTRIBUTE:               return "MetalAttribute";
        case SLANG_PARAMETER_CATEGORY_METAL_PAYLOAD:                 return "MetalPayload";
        default:                                                     return "Unknown";
    }
}

#ifdef _WIN32
// dxcompiler.dll のパスを探す (LoadLibrary で読み込み可能なフォルダ)
static std::filesystem::path findDXCompilerDLLPath() {
    const wchar_t* dllNames[] = { L"dxcompiler.dll" };
    for (const auto& dllName : dllNames) {
        HMODULE hModule = ::LoadLibraryW(dllName);
        if (hModule) {
            wchar_t path[MAX_PATH];
            ::GetModuleFileNameW(hModule, path, MAX_PATH);
            ::FreeLibrary(hModule);
            return std::filesystem::path(path).parent_path();
        }
    }
    return std::filesystem::path();
}
#endif

// Case-insensitive prefix compare
static bool iequalsN(const char* s, const char* prefix, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (tolower(static_cast<unsigned char>(s[i])) != tolower(static_cast<unsigned char>(prefix[i])))
            return false;
        if (s[i] == '\0') return false;
    }
    return true;
}

} // anonymous namespace

//----------------------------------------------------------------------
// ShaderCompiler

Result<std::unique_ptr<ShaderCompiler>> ShaderCompiler::create() {
    std::unique_ptr<ShaderCompiler> ref(new ShaderCompiler());
    auto result = ref->init();
    if (!result) return tl::make_unexpected(result.error());
    return ref;
}

ShaderCompiler::ShaderCompiler()
    : m_dump(false) {
}

ShaderCompiler::~ShaderCompiler() {
}

VoidResult ShaderCompiler::init() {
    SlangGlobalSessionDesc desc = {};
    SlangResult result = slang::createGlobalSession(&desc, m_globalSession.writeRef());
    if (SLANG_FAILED(result)) {
        return LNSHADER_MAKE_ERROR("slang::createGlobalSession failed. (" + std::to_string(result) + ")");
    }

#ifdef _WIN32
    const std::filesystem::path path = findDXCompilerDLLPath();
    if (!path.empty()) {
        m_globalSession->setDownstreamCompilerPath(
            SlangPassThrough::SLANG_PASS_THROUGH_DXC, path.string().c_str());
    }
#endif
    return LNSHADER_OK();
}

VoidResult ShaderCompiler::build(const fs::path& inputFilePath) {
    m_inputFilePath = inputFilePath;
    m_inputDirPath = inputFilePath.parent_path();
    m_shader = Ref<UnifiedShader>::adopt(new UnifiedShader());

    if (m_dump) {
        m_dumpDirPath = inputFilePath;
        m_dumpDirPath += ".dump";
        fs::create_directories(m_dumpDirPath);
    }

    // Read all text.
    std::string code;
    {
        try {
            size_t fileSize = std::filesystem::file_size(inputFilePath);
            code = std::string(fileSize, '\0');
            std::ifstream stream(inputFilePath);
            if (stream.fail()) {
                return LNSHADER_MAKE_ERROR("ifstream failed.");
            }
            stream.read(&code[0], fileSize);
        }
        catch (const fs::filesystem_error& e) {
            return LNSHADER_MAKE_ERROR(std::string("ifstream failed. (") + e.what() + ")");
        }
    }

    // Parse metadata.
    {
        ShaderMetadataParser parser;
        auto result = parser.parse(code);
        if (!result) return result;
        for (const auto& pass : parser.passes()) {
            auto shaderPass = m_shader->createGlobalShaderPass();
            shaderPass->name = pass.name;
            shaderPass->vertexEntryPoint = pass.vertexEntryPoint;
            shaderPass->fragmentEntryPoint = pass.fragmentEntryPoint;
            shaderPass->computeEntryPoint = pass.computeEntryPoint;
        }
    }

    auto result = buildModule();
    if (!result) {
        return result;
    }
    return LNSHADER_OK();
}

VoidResult ShaderCompiler::buildModule() {
    std::vector<slang::TargetDesc> targetDescs;
    for (const auto& target : kTargets) {
        auto profile = m_globalSession->findProfile(target.profile);
        slang::TargetDesc targetDesc = {};
        targetDesc.format = toSlangTarget(target.target);
        targetDesc.profile = profile;
        targetDescs.push_back(targetDesc);
    }

    // SPIRV では、エントリポイント名を main ではなくコード上の名前にする。
    std::vector<slang::CompilerOptionEntry> options;
    options.push_back(
        { slang::CompilerOptionName::VulkanUseEntryPointName, { slang::CompilerOptionValueKind::Int, 1 } });

    slang::SessionDesc sessionDesc = {};
    sessionDesc.targets = targetDescs.data();
    sessionDesc.targetCount = targetDescs.size();
    sessionDesc.compilerOptionEntries = options.data();
    sessionDesc.compilerOptionEntryCount = options.size();

    // Include/Module paths
    std::vector<const char*> searchPaths;
    sessionDesc.searchPaths = searchPaths.data();
    sessionDesc.searchPathCount = searchPaths.size();

    Slang::ComPtr<slang::ISession> session;
    SlangResult result = m_globalSession->createSession(sessionDesc, session.writeRef());
    if (SLANG_FAILED(result)) {
        return LNSHADER_MAKE_ERROR("createSession failed. (" + std::to_string(result) + ")");
    }

    // Load shader file
    std::string inputFilePath = m_inputFilePath.string();
    Slang::ComPtr<slang::IBlob> diagnostics;
    m_module = session->loadModule(inputFilePath.c_str(), diagnostics.writeRef());
    if (!m_module) {
        std::string diag;
        if (diagnostics) {
            diag = std::string(static_cast<const char*>(diagnostics->getBufferPointer()), diagnostics->getBufferSize());
        }
        return LNSHADER_MAKE_ERROR("ISession::loadModule failed." + (diag.empty() ? "" : " " + diag));
    }

    // Compose all entry points
    std::vector<Slang::ComPtr<slang::IEntryPoint>> componentsRefs;
    std::vector<slang::IComponentType*> components = { m_module };
    for (int i = 0; i < m_module->getDefinedEntryPointCount(); i++) {
        Slang::ComPtr<slang::IEntryPoint> entryPoint;
        m_module->getDefinedEntryPoint(i, entryPoint.writeRef());
        componentsRefs.push_back(entryPoint);
        components.push_back(entryPoint);
    }
    Slang::ComPtr<slang::IComponentType> composed;
    result = session->createCompositeComponentType(components.data(), components.size(), composed.writeRef());
    if (SLANG_FAILED(result)) {
        return LNSHADER_MAKE_ERROR("createCompositeComponentType failed. (" + std::to_string(result) + ")");
    }

    // Link
    Slang::ComPtr<slang::IBlob> linkDiag;
    result = composed->link(m_program.writeRef(), linkDiag.writeRef());
    if (SLANG_FAILED(result)) {
        std::string message;
        if (linkDiag) {
            message = std::string(static_cast<const char*>(linkDiag->getBufferPointer()), linkDiag->getBufferSize());
        }
        return LNSHADER_MAKE_ERROR("link failed. (" + std::to_string(result) + ")" + (message.empty() ? "" : " " + message));
    }

    for (int i = 0; i < kTargetCount; i++) {
        auto& target = kTargets[i];
        auto result1 = buildInputResources(i);
        if (!result1) return result1;
        auto r = buildTarget(target.target, i);
        if (!r) return r;
    }

    {
        auto r1 = mergeTargetInputResources();
        if (!r1) return r1;
    }

    {
        DescriptorLayoutBuilder builder;
        auto r1 = builder.buildDescriptorLayout(m_shader.get());
        if (!r1) return r1;
    }
    return LNSHADER_OK();
}

VoidResult ShaderCompiler::buildInputResources(int targetIndex) {
    slang::ProgramLayout* programLayout = m_program->getLayout(targetIndex);
    int parameterCount = programLayout->getParameterCount();

    bool hasGlobalConstantBuffer = false;
    for (int i = 0; i < parameterCount; i++) {
        slang::VariableLayoutReflection* parameter = programLayout->getParameterByIndex(i);
        slang::ParameterCategory category = parameter->getCategory();
        if (category != slang::ParameterCategory::Uniform) {
            continue;
        }

        std::string name = parameter->getName();
        int32_t arrayElements = 0;
        int32_t vectorElements = 0;
        int32_t matrixRows = 0;
        int32_t matrixColumns = 0;
        ShaderGlobalMemberKind memberKind = ShaderGlobalMemberKind_Unknown;
        ShaderGlobalMemberType memberType = ShaderGlobalMemberType_Unknown;

        slang::TypeReflection* type = parameter->getType();
        slang::TypeLayoutReflection* typeLayout = parameter->getTypeLayout();
        slang::TypeReflection::Kind kind = type->getKind();
        switch (kind) {
            case slang::TypeReflection::Kind::Array:
                memberKind = ShaderGlobalMemberKind_Array;
                arrayElements = typeLayout->getElementCount();
                break;
            case slang::TypeReflection::Kind::Matrix:
                memberKind = ShaderGlobalMemberKind_Matrix;
                matrixRows = typeLayout->getRowCount();
                matrixColumns = typeLayout->getColumnCount();
                break;
            case slang::TypeReflection::Kind::Vector:
                memberKind = ShaderGlobalMemberKind_Vector;
                vectorElements = typeLayout->getElementCount();
                break;
            case slang::TypeReflection::Kind::Scalar:
                memberKind = ShaderGlobalMemberKind_Scalar;
                break;
            default:
                return LNSHADER_MAKE_ERROR(
                    "Invalid kind. (" + name + ":" + std::to_string(static_cast<int>(kind)) + ")");
        }

        if (kind == slang::TypeReflection::Kind::Array) {
            slang::TypeReflection* elementType = type->getElementType();
            memberType = toLuminoShaderGlobalMemberType(elementType->getScalarType());
        }
        else {
            memberType = toLuminoShaderGlobalMemberType(type->getScalarType());
        }
        if (memberType == ShaderGlobalMemberType_Unknown) {
            return LNSHADER_MAKE_ERROR("Invalid type. (" + name + ")");
        }

        auto result = m_shader->getOrCreateGlobalMemberWithVerify(
            name,
            memberType,
            memberKind,
            arrayElements,
            vectorElements,
            matrixRows,
            matrixColumns);
        if (!result) return tl::make_unexpected(result.error());
        hasGlobalConstantBuffer = true;
    }

    if (hasGlobalConstantBuffer) {
        auto result = m_shader->getOrCreateInputResourceWithVerify(
            kGlobalConstantBufferName,
            RegisterCategory_ConstantBuffer,
            -1,
            0);
        if (!result) return result;
    }

    // Collect non-$Global parameters.
    for (int i = 0; i < parameterCount; i++) {
        slang::VariableLayoutReflection* parameter = programLayout->getParameterByIndex(i);

        slang::ParameterCategory category = parameter->getCategory();
        if (category == slang::ParameterCategory::Uniform) {
            continue;
        }

        std::string name;
        RegisterCategory registerCategory = RegisterCategory_Unknown;
        int constantBufferSize = 0;
        int arrayElementCount = 0;
        auto result1 = getBindingResourceInfo(
            parameter, &name, &registerCategory, &constantBufferSize, &arrayElementCount);
        if (!result1) return result1;

        auto result = m_shader->getOrCreateInputResourceWithVerify(
            name, registerCategory, constantBufferSize, arrayElementCount);
        if (!result) return result;
    }

    return LNSHADER_OK();
}

VoidResult ShaderCompiler::buildTarget(ShaderTarget target, int targetIndex) {
    slang::ProgramLayout* layout = m_program->getLayout(targetIndex);

    // Dump target reflection
    if (m_dump) {
        fs::path filePath = m_dumpDirPath / (std::string(getTargetName(target)) + ".target-reflection.json");
        Slang::ComPtr<slang::IBlob> text;
        SlangResult result = layout->toJson(text.writeRef());
        if (!SLANG_FAILED(result)) {
            std::ofstream stream(filePath);
            if (stream) {
                stream.write(reinterpret_cast<const char*>(text->getBufferPointer()), text->getBufferSize());
            }
        }
    }

    // Build entry points
    int entryPointCount = m_module->getDefinedEntryPointCount();
    for (int iEntryPoint = 0; iEntryPoint < entryPointCount; iEntryPoint++) {
        auto r = buildEntryPoint(target, targetIndex, iEntryPoint);
        if (!r) return r;
    }

    // Link shader passes.
    for (auto& globalShaderPass : m_shader->globalShaderPasses()) {
        auto result = buildTargetShaderPass(target, targetIndex, globalShaderPass.get());
        if (!result) return result;
    }

    return LNSHADER_OK();
}

VoidResult ShaderCompiler::buildEntryPoint(
    ShaderTarget target,
    int targetIndex,
    int entryPointIndex) {
    slang::ProgramLayout* programLayout = m_program->getLayout(targetIndex);
    slang::EntryPointReflection* entryPointReflection = programLayout->getEntryPointByIndex(entryPointIndex);
    SlangStage stage = entryPointReflection->getStage();

    // Get EntryPointMetadata.
    Slang::ComPtr<slang::IMetadata> entryPointMetadata;
    Slang::ComPtr<slang::IBlob> diag;
    SlangResult result = m_program->getEntryPointMetadata(
        entryPointIndex, targetIndex, entryPointMetadata.writeRef(), diag.writeRef());
    if (SLANG_FAILED(result)) {
        std::string message;
        if (diag) {
            message = std::string(static_cast<const char*>(diag->getBufferPointer()), diag->getBufferSize());
        }
        return LNSHADER_MAKE_ERROR("getEntryPointMetadata failed. (" + std::to_string(result) + "): " + message);
    }

    // Analyze Varyings.
    std::vector<VaryingDumpInfo> varyingDumpInfos;
    std::vector<VertexInputAttribute> inputAttributes;
    {
        auto callback = [&varyingDumpInfos](slang::VariableLayoutReflection* var) {
            VaryingDumpInfo info;
            info.name = var->getName();
            info.semanticName = var->getSemanticName();
            info.semanticIndex = var->getSemanticIndex();
            auto category = var->getCategory();
            info.locationIndex = var->getOffset(category);
            varyingDumpInfos.push_back(info);
        };
        int entryPointParameterCount = entryPointReflection->getParameterCount();
        for (int i = 0; i < entryPointParameterCount; i++) {
            auto parameter = entryPointReflection->getParameterByIndex(i);
            traverseVariableSemaintic(parameter, callback);
        }

        if (stage == SLANG_STAGE_VERTEX) {
            for (const auto& info : varyingDumpInfos) {
                auto r = makeVertexInputAttribute(
                    info.name, info.semanticName, info.semanticIndex, info.locationIndex);
                if (!r) return tl::make_unexpected(r.error());
                inputAttributes.push_back(r.value());
            }
        }
    }

    // Dump bindings to JSON.
    if (m_dump) {
        std::vector<BindingDumpInfo> bindingDumpInfos;
        {
            int programParameterCount = programLayout->getParameterCount();
            for (int i = 0; i < programParameterCount; i++) {
                slang::VariableLayoutReflection* parameter = programLayout->getParameterByIndex(i);
                slang::TypeLayoutReflection* typeLayout = parameter->getTypeLayout();
                int categoryCount = parameter->getCategoryCount();
                for (int iCategory = 0; iCategory < categoryCount; iCategory++) {
                    SlangParameterCategory category = static_cast<SlangParameterCategory>(
                        parameter->getCategoryByIndex(iCategory));
                    auto indexOrOffset = parameter->getOffset(category);
                    auto space = parameter->getBindingSpace(category);
                    auto sizeOrCount = typeLayout->getSize(category);

                    BindingDumpInfo dumpInfo;
                    dumpInfo.name = parameter->getName();
                    dumpInfo.category = category;
                    if (category == SLANG_PARAMETER_CATEGORY_UNIFORM) {
                        dumpInfo.offset = indexOrOffset;
                        dumpInfo.size = sizeOrCount;
                        dumpInfo.index = -1;
                        dumpInfo.space = -1;
                        dumpInfo.count = -1;
                    }
                    else {
                        dumpInfo.index = indexOrOffset;
                        dumpInfo.offset = -1;
                        dumpInfo.space = space;
                        dumpInfo.count = sizeOrCount;
                    }

                    bool used = false;
                    bool usedAvailable = entryPointMetadata->isParameterLocationUsed(
                                             category, space, indexOrOffset, used) == SLANG_OK;
                    dumpInfo.used = usedAvailable ? (used ? 1 : 0) : -1;
                    bindingDumpInfos.push_back(dumpInfo);
                }
            }
        }

        const char* name = entryPointReflection->getName();
        fs::path filePath =
            m_dumpDirPath / (std::string(getTargetName(target)) + ".entry-reflection." + name + ".reflection.json");
        std::ofstream stream(filePath);
        if (stream) {
            stream << "{\n";
            stream << "    \"bindings\": [\n";
            for (size_t i = 0; i < bindingDumpInfos.size(); i++) {
                const BindingDumpInfo& info = bindingDumpInfos[i];
                stream << "        { ";
                stream << "\"name\": \"" << info.name << "\", ";
                stream << "\"category\": \"" << getSlangCategoryName(info.category) << "\", ";
                if (info.offset > -1) stream << "\"offset\": " << info.offset << ", ";
                if (info.size > -1) stream << "\"size\": " << info.size << ", ";
                if (info.space > -1) stream << "\"space\": " << info.space << ", ";
                if (info.index > -1) stream << "\"index\": " << info.index << ", ";
                if (info.count > -1) stream << "\"count\": " << info.count << ", ";
                stream << "\"used\": " << info.used << " ";
                stream << (i < bindingDumpInfos.size() - 1 ? "},\n" : "}\n");
            }
            stream << "    ],\n";
            stream << "    \"varyings\": [\n";
            for (size_t i = 0; i < varyingDumpInfos.size(); i++) {
                const VaryingDumpInfo& info = varyingDumpInfos[i];
                stream << "        { ";
                stream << "\"name\": \"" << info.name << "\", ";
                stream << "\"semanticName\": \"" << info.semanticName << "\", ";
                stream << "\"semanticIndex\": " << info.semanticIndex << " ";
                stream << (i < varyingDumpInfos.size() - 1 ? "},\n" : "}\n");
            }
            stream << "    ]\n";
            stream << "}\n";
        }
    }

    // Generate code.
    Blob* codeBlob = nullptr;
    {
        Slang::ComPtr<slang::IBlob> kernelBlob;
        Slang::ComPtr<slang::IBlob> generateDiag;
        result =
            m_program->getEntryPointCode(entryPointIndex, targetIndex, kernelBlob.writeRef(), generateDiag.writeRef());
        if (SLANG_FAILED(result)) {
            std::string message;
            if (generateDiag) {
                message = std::string(
                    static_cast<const char*>(generateDiag->getBufferPointer()), generateDiag->getBufferSize());
            }
            return LNSHADER_MAKE_ERROR("getEntryPointCode failed. (" + std::to_string(result) + ")" +
                                       (message.empty() ? "" : " " + message));
        }

        // Dump code.
        if (m_dump) {
            const char* name = entryPointReflection->getName();
            fs::path filePath = m_dumpDirPath / (std::string(getTargetName(target)) + "." + name + getExt(target));
            std::ofstream dumpStream(filePath, std::ios::binary);
            if (dumpStream) {
                dumpStream.write(
                    reinterpret_cast<const char*>(kernelBlob->getBufferPointer()), kernelBlob->getBufferSize());
            }
        }

        codeBlob = m_shader->createBlob();
        codeBlob->data.resize(kernelBlob->getBufferSize());
        memcpy(codeBlob->data.data(), kernelBlob->getBufferPointer(), kernelBlob->getBufferSize());
    }

    TargetEntryPoint* entryPoint = m_shader->createEntryPoint();
    entryPoint->target = target;
    entryPoint->name = entryPointReflection->getName();
    entryPoint->codeBlobId = codeBlob->id;
    entryPoint->inputAttributes = inputAttributes;

    ShaderStageFlags stageFlags = ShaderStageFlags_None;
    if (stage == SLANG_STAGE_VERTEX) {
        stageFlags = ShaderStageFlags_Vertex;
    }
    else if (stage == SLANG_STAGE_FRAGMENT) {
        stageFlags = ShaderStageFlags_Pixel;
    }
    else if (stage == SLANG_STAGE_COMPUTE) {
        stageFlags = ShaderStageFlags_Compute;
    }
    else {
        return LNSHADER_MAKE_ERROR("Invalid stage. (" + std::to_string(stage) + ")");
    }

    // $Global ConstantBuffer binding info
    int globalConstantBufferSize = programLayout->getGlobalConstantBufferSize();
    if (globalConstantBufferSize > 0) {
        TargetBindingInfo binding;
        binding.name = kGlobalConstantBufferName;
        binding.category = BindingResourceCategory_ConstantBuffer;
        binding.size = globalConstantBufferSize;
        binding.space = 0;
        binding.index = programLayout->getGlobalConstantBufferBinding();
        binding.count = 0;
        binding.used = stageFlags;

        int programParameterCount = programLayout->getParameterCount();
        for (int i = 0; i < programParameterCount; i++) {
            slang::VariableLayoutReflection* parameter = programLayout->getParameterByIndex(i);
            slang::ParameterCategory category = parameter->getCategory();
            if (category != slang::ParameterCategory::Uniform) {
                continue;
            }
            slang::TypeLayoutReflection* typeLayout = parameter->getTypeLayout();
            TargetBindingConstantBufferMemberInfo memberInfo;
            memberInfo.name = parameter->getName();
            memberInfo.offset = parameter->getOffset(category);
            memberInfo.size = typeLayout->getSize(category);
            binding.members.push_back(memberInfo);
        }
        entryPoint->bindingLayout.bindings.push_back(binding);
    }

    // Collect Non-$Global members.
    {
        int programParameterCount = programLayout->getParameterCount();
        for (int i = 0; i < programParameterCount; i++) {
            slang::VariableLayoutReflection* parameter = programLayout->getParameterByIndex(i);
            slang::TypeLayoutReflection* typeLayout = parameter->getTypeLayout();

            slang::ParameterCategory category = parameter->getCategory();
            if (category == slang::ParameterCategory::Uniform) {
                continue;
            }

            int categoryCount = parameter->getCategoryCount();
            for (int ic = 0; ic < categoryCount; ic++) {
                slang::ParameterCategory cat = parameter->getCategoryByIndex(ic);

                TargetBindingInfo bindingInfo;
                bindingInfo.name = parameter->getName();
                bindingInfo.size = 0;
                bindingInfo.space = parameter->getBindingSpace(cat);
                bindingInfo.index = parameter->getBindingIndex();
                bindingInfo.count = typeLayout->getSize(cat);
                bindingInfo.used = stageFlags;

                bool withSampler = false;

                slang::TypeReflection* type = parameter->getType();
                slang::TypeReflection::Kind kind = type->getKind();
                switch (kind) {
                    case slang::TypeReflection::Kind::ConstantBuffer: {
                        bindingInfo.category = BindingResourceCategory_ConstantBuffer;
                        auto* elementLayout = typeLayout->getElementVarLayout();
                        auto* typeLayout2 = elementLayout->getTypeLayout();
                        int categoryCount2 = elementLayout->getCategoryCount();
                        if (categoryCount2 > 1) {
                            return LNSHADER_MAKE_ERROR(
                                "Multiple category unsupported. (" + std::to_string(categoryCount2) + ")");
                        }
                        slang::ParameterCategory elCat = elementLayout->getCategoryByIndex(0);
                        bindingInfo.size = typeLayout2->getSize(elCat);
                        break;
                    }
                    case slang::TypeReflection::Kind::Resource: {
                        if (cat == slang::ParameterCategory::DescriptorTableSlot && bindingInfo.count == 2) {
                            withSampler = true;
                        }
                        bindingInfo.category = BindingResourceCategory_TextureOrCombinedSampler;
                        break;
                    }
                    case slang::TypeReflection::Kind::SamplerState:
                        bindingInfo.category = BindingResourceCategory_SamplerState;
                        break;
                    case slang::TypeReflection::Kind::Array:
                        return LNSHADER_MAKE_ERROR("Array types are not supported.");
                    default:
                        return LNSHADER_MAKE_ERROR(
                            "Invalid type. (" + std::string(bindingInfo.name) + ":" +
                            std::to_string(static_cast<int>(kind)) + ")");
                }

                entryPoint->bindingLayout.bindings.push_back(bindingInfo);

                if (withSampler) {
                    TargetBindingInfo bindingInfo2;
                    bindingInfo2.name = bindingInfo.name;
                    bindingInfo2.category = BindingResourceCategory_SamplerState;
                    bindingInfo2.size = bindingInfo.size;
                    bindingInfo2.space = bindingInfo.space;
                    bindingInfo2.index = bindingInfo.index + 1;
                    bindingInfo2.count = 1;
                    bindingInfo2.used = stageFlags;
                    bindingInfo2.combinedSamplerName = bindingInfo.name;
                    entryPoint->bindingLayout.bindings.push_back(bindingInfo2);
                }
            }
        }
    }

    return LNSHADER_OK();
}

VoidResult ShaderCompiler::buildTargetShaderPass(
    ShaderTarget target, int targetIndex, GlobalShaderPass* globalShaderPass) {

    TargetShaderPass* targetShaderPass = m_shader->createTargetShaderPass();
    globalShaderPass->targetShaderPassIds[target - 1] = targetShaderPass->id;

    if (!globalShaderPass->vertexEntryPoint.empty()) {
        auto result = m_shader->getTargetEntryPoint(target, globalShaderPass->vertexEntryPoint);
        if (!result) return tl::make_unexpected(result.error());
        targetShaderPass->vertEntryPointId = result.value()->id;
    }

    if (!globalShaderPass->fragmentEntryPoint.empty()) {
        auto result = m_shader->getTargetEntryPoint(target, globalShaderPass->fragmentEntryPoint);
        if (!result) return tl::make_unexpected(result.error());
        targetShaderPass->fragEntryPointId = result.value()->id;
    }

    if (!globalShaderPass->computeEntryPoint.empty()) {
        auto result = m_shader->getTargetEntryPoint(target, globalShaderPass->computeEntryPoint);
        if (!result) return tl::make_unexpected(result.error());
        targetShaderPass->compEntryPointId = result.value()->id;
    }

    return LNSHADER_OK();
}

VoidResult ShaderCompiler::getBindingResourceInfo(
    slang::VariableLayoutReflection* parameter,
    std::string* outName,
    RegisterCategory* outRegisterCategory,
    int* outConstantBufferSize,
    int* outArrayElementCount) {
    std::string name = parameter->getName();
    RegisterCategory registerCategory = RegisterCategory_Unknown;
    int constantBufferSize = 0;
    int arrayElementCount = 0;

    slang::ParameterCategory category = parameter->getCategory();
    if (category == slang::ParameterCategory::Mixed) {
        int categoryCount = parameter->getCategoryCount();
        if (categoryCount != 2) {
            return LNSHADER_MAKE_ERROR("Multiple category unsupported. (" + std::to_string(categoryCount) + ")");
        }
        slang::ParameterCategory c0 = parameter->getCategoryByIndex(0);
        slang::ParameterCategory c1 = parameter->getCategoryByIndex(1);
        if ((c0 == slang::ParameterCategory::ShaderResource ||
             c0 == slang::ParameterCategory::SamplerState) &&
            (c1 == slang::ParameterCategory::ShaderResource ||
             c1 == slang::ParameterCategory::SamplerState)) {
            registerCategory = RegisterCategory_TextureOrCombinedSampler;
        }
        else {
            return LNSHADER_MAKE_ERROR(
                "Invalid mixed category. (" + std::to_string(static_cast<int>(c0)) + ":" +
                std::to_string(static_cast<int>(c1)) + ")");
        }
    }
    else {
        slang::TypeReflection* type = parameter->getType();
        slang::TypeLayoutReflection* typeLayout = parameter->getTypeLayout();
        slang::TypeReflection::Kind kind = type->getKind();
        switch (kind) {
            case slang::TypeReflection::Kind::ConstantBuffer: {
                registerCategory = RegisterCategory_ConstantBuffer;
                slang::VariableLayoutReflection* elementLayout = typeLayout->getElementVarLayout();
                slang::TypeLayoutReflection* typeLayout2 = elementLayout->getTypeLayout();
                int categoryCount2 = elementLayout->getCategoryCount();
                if (categoryCount2 > 1) {
                    return LNSHADER_MAKE_ERROR(
                        "Multiple category unsupported. 2 (" + std::to_string(categoryCount2) + ")");
                }
                slang::ParameterCategory elCat = elementLayout->getCategoryByIndex(0);
                constantBufferSize = typeLayout2->getSize(elCat);
                break;
            }
            case slang::TypeReflection::Kind::Resource:
                registerCategory = RegisterCategory_TextureOrCombinedSampler;
                break;
            case slang::TypeReflection::Kind::SamplerState:
                registerCategory = RegisterCategory_SamplerState;
                break;
            case slang::TypeReflection::Kind::Array:
                return LNSHADER_MAKE_ERROR("Array types are not supported. (" + name + ")");
            default:
                return LNSHADER_MAKE_ERROR(
                    "Invalid type. (" + name + ":" + std::to_string(static_cast<int>(kind)) + ")");
        }
    }

    *outName = name;
    *outRegisterCategory = registerCategory;
    *outConstantBufferSize = constantBufferSize;
    *outArrayElementCount = arrayElementCount;
    return LNSHADER_OK();
}

void ShaderCompiler::traverseVariableSemaintic(
    slang::VariableLayoutReflection* var,
    const std::function<void(slang::VariableLayoutReflection* var)>& callback) {
    if (var->getSemanticName()) {
        callback(var);
    }

    slang::TypeLayoutReflection* typeLayout = var->getTypeLayout();
    if (typeLayout) {
        switch (typeLayout->getKind()) {
            default:
                break;
            case slang::TypeReflection::Kind::Struct: {
                auto structTypeLayout = typeLayout;
                auto fieldCount = structTypeLayout->getFieldCount();
                for (uint32_t ff = 0; ff < fieldCount; ++ff) {
                    traverseVariableSemaintic(structTypeLayout->getFieldByIndex(ff), callback);
                }
            } break;
        }
    }
}

VoidResult ShaderCompiler::mergeTargetInputResources() {
    const auto& targetEntryPoints = m_shader->targetEntryPoints();
    for (auto& targetShaderPass : m_shader->targetShaderPasses()) {
        bool reset = true;

        if (targetShaderPass->vertEntryPointId >= 0) {
            const auto& entryPoint = targetEntryPoints[targetShaderPass->vertEntryPointId];
            auto result = UnifiedShader::mergeTargetBindingLayoutInfo(
                targetShaderPass->bindingLayout, entryPoint->bindingLayout, reset);
            if (!result) return result;
            reset = false;
        }

        if (targetShaderPass->fragEntryPointId >= 0) {
            const auto& entryPoint = targetEntryPoints[targetShaderPass->fragEntryPointId];
            auto result = UnifiedShader::mergeTargetBindingLayoutInfo(
                targetShaderPass->bindingLayout, entryPoint->bindingLayout, reset);
            if (!result) return result;
            reset = false;
        }

        if (targetShaderPass->compEntryPointId >= 0) {
            const auto& entryPoint = targetEntryPoints[targetShaderPass->compEntryPointId];
            auto result = UnifiedShader::mergeTargetBindingLayoutInfo(
                targetShaderPass->bindingLayout, entryPoint->bindingLayout, reset);
            if (!result) return result;
            reset = false;
        }
    }

    return LNSHADER_OK();
}

Result<VertexInputAttribute> ShaderCompiler::makeVertexInputAttribute(
    const std::string& varName,
    const std::string& semanticName,
    int semanticIndex,
    int locationIndex) {
    VertexInputAttribute attr;

    if (iequalsN(semanticName.c_str(), "POSITION", 8)) {
        attr.usage = AttributeUsage_Position;
    }
    else if (iequalsN(semanticName.c_str(), "BLENDWEIGHT", 11)) {
        attr.usage = AttributeUsage_BlendWeight;
    }
    else if (iequalsN(semanticName.c_str(), "BLENDINDICES", 12)) {
        attr.usage = AttributeUsage_BlendIndices;
    }
    else if (iequalsN(semanticName.c_str(), "NORMAL", 6)) {
        attr.usage = AttributeUsage_Normal;
    }
    else if (iequalsN(semanticName.c_str(), "TEXCOORD", 8)) {
        attr.usage = AttributeUsage_TexCoord;
    }
    else if (iequalsN(semanticName.c_str(), "TANGENT", 7)) {
        attr.usage = AttributeUsage_Tangent;
    }
    else if (iequalsN(semanticName.c_str(), "BINORMAL", 8)) {
        attr.usage = AttributeUsage_Binormal;
    }
    else if (iequalsN(semanticName.c_str(), "COLOR", 5)) {
        attr.usage = AttributeUsage_Color;
    }
    else if (iequalsN(semanticName.c_str(), "SV_INSTANCEID", 14)) {
        attr.usage = AttributeUsage_InstanceID;
    }
    else {
        return LNSHADER_MAKE_ERROR("Unknown semantic: " + semanticName);
    }

    attr.index = semanticIndex;
    attr.layoutLocation = locationIndex;
    return attr;
}

} // namespace shader
} // namespace ln

#endif // LUMINO_USE_SLANG
