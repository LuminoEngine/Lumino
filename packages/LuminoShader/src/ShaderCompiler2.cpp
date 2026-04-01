// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#include "pch.hpp"
#include <LuminoShader/UnifiedShader.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/ShaderCompiler2.hpp>
#include "ShaderMetadata.hpp"

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

// Case-insensitive prefix compare
static bool iequalsN(const char* s, const char* prefix, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (tolower(static_cast<unsigned char>(s[i])) != tolower(static_cast<unsigned char>(prefix[i])))
            return false;
        if (s[i] == '\0') return false;
    }
    return true;
}

#ifdef _WIN32
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

} // anonymous namespace

//----------------------------------------------------------------------
// ShaderCompiler2

Result<std::unique_ptr<ShaderCompiler2>> ShaderCompiler2::create() {
    std::unique_ptr<ShaderCompiler2> ref(new ShaderCompiler2());
    auto result = ref->init();
    if (!result) return tl::make_unexpected(result.error());
    return ref;
}

ShaderCompiler2::ShaderCompiler2()
    : m_dump(false) {
}

ShaderCompiler2::~ShaderCompiler2() {
}

VoidResult ShaderCompiler2::init() {
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

VoidResult ShaderCompiler2::build(const fs::path& inputFilePath) {
    m_inputFilePath = inputFilePath;
    m_inputDirPath = inputFilePath.parent_path();
    m_shader = Ref<UnifiedShader2>::adopt(new UnifiedShader2());
    m_parameterBlocksBuilt = false;

    if (m_dump) {
        m_dumpDirPath = inputFilePath;
        m_dumpDirPath += ".dump2";
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
        ShaderMetadata parser;
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

VoidResult ShaderCompiler2::buildModule() {
    std::vector<slang::TargetDesc> targetDescs;
    for (const auto& target : kTargets) {
        auto profile = m_globalSession->findProfile(target.profile);
        slang::TargetDesc targetDesc = {};
        targetDesc.format = toSlangTarget(target.target);
        targetDesc.profile = profile;
        targetDescs.push_back(targetDesc);
    }

    std::vector<slang::CompilerOptionEntry> options;
    options.push_back(
        { slang::CompilerOptionName::VulkanUseEntryPointName, { slang::CompilerOptionValueKind::Int, 1 } });

    slang::SessionDesc sessionDesc = {};
    sessionDesc.targets = targetDescs.data();
    sessionDesc.targetCount = targetDescs.size();
    sessionDesc.compilerOptionEntries = options.data();
    sessionDesc.compilerOptionEntryCount = options.size();
    // CPU Matrix4x4 is column-major; tell Slang to use column-major layout in
    // constant buffers so that the uploaded bytes are interpreted correctly by
    // the GPU.  Without this, Slang defaults to row-major for SPIR-V and the
    // GPU would see the transpose of the intended matrix.
    sessionDesc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR;

    // Include/Module paths
    std::vector<std::string> searchPathStrings;
    for (const auto& p : m_searchPaths) {
        searchPathStrings.push_back(p.string());
    }
    std::vector<const char*> searchPaths;
    for (const auto& s : searchPathStrings) {
        searchPaths.push_back(s.c_str());
    }
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
        auto result1 = buildParameterBlocks(i);
        if (!result1) return result1;
        auto r = buildTarget(target.target, i);
        if (!r) return r;
    }

    {
        auto r1 = mergeTargetBindingLayouts();
        if (!r1) return r1;
    }

    return LNSHADER_OK();
}

VoidResult ShaderCompiler2::buildParameterBlocks(int targetIndex) {
    // Only build parameter block layouts once (they are target-independent).
    // We use the first target's reflection to extract the layout.
    if (m_parameterBlocksBuilt) {
        return LNSHADER_OK();
    }

    slang::ProgramLayout* programLayout = m_program->getLayout(targetIndex);
    int parameterCount = programLayout->getParameterCount();

    for (int i = 0; i < parameterCount; i++) {
        slang::VariableLayoutReflection* parameter = programLayout->getParameterByIndex(i);
        slang::TypeReflection* type = parameter->getType();
        slang::TypeLayoutReflection* typeLayout = parameter->getTypeLayout();
        slang::TypeReflection::Kind kind = type->getKind();

        if (kind != slang::TypeReflection::Kind::ParameterBlock) {
            // v2 only supports ParameterBlock parameters.
            // Skip VaryingInput/VaryingOutput which are entry point parameters.
            slang::ParameterCategory category = parameter->getCategory();
            if (category == slang::ParameterCategory::VaryingInput ||
                category == slang::ParameterCategory::VaryingOutput) {
                continue;
            }
            return LNSHADER_MAKE_ERROR(
                "ShaderCompiler2 requires all resources to be in ParameterBlock. Found bare parameter: " +
                std::string(parameter->getName()));
        }

        ParameterBlockLayout2 layout;
        layout.name = parameter->getName();
        layout.hasImplicitConstantBuffer = false;

        // Try various methods to get the set index.
        // ParameterBlock occupies a RegisterSpace, so use getOffset with SubElementRegisterSpace.
        {
            int categoryCount = parameter->getCategoryCount();
            int16_t setIdx = -1;
            for (int ic = 0; ic < categoryCount; ic++) {
                slang::ParameterCategory cat = parameter->getCategoryByIndex(ic);
                if (cat == slang::ParameterCategory::SubElementRegisterSpace ||
                    cat == slang::ParameterCategory::RegisterSpace) {
                    setIdx = static_cast<int16_t>(parameter->getOffset(cat));
                    break;
                }
            }
            if (setIdx < 0) {
                // Fallback: use the binding space
                setIdx = static_cast<int16_t>(
                    parameter->getBindingSpace(slang::ParameterCategory::RegisterSpace));
            }
            layout.setIndex = setIdx;
        }

        // Get the element type layout (the struct inside ParameterBlock<T>)
        slang::TypeLayoutReflection* elementTypeLayout = typeLayout->getElementTypeLayout();
        slang::TypeReflection* elementType = elementTypeLayout->getType();
        slang::TypeReflection::Kind elementKind = elementType->getKind();

        bool hasExplicitCB = false;
        bool hasPlainData = false;

        if (elementKind == slang::TypeReflection::Kind::Struct) {
            int fieldCount = elementTypeLayout->getFieldCount();
            for (int f = 0; f < fieldCount; f++) {
                slang::VariableLayoutReflection* field = elementTypeLayout->getFieldByIndex(f);
                slang::TypeReflection* fieldType = field->getType();
                slang::TypeReflection::Kind fieldKind = fieldType->getKind();

                switch (fieldKind) {
                    case slang::TypeReflection::Kind::ConstantBuffer: {
                        hasExplicitCB = true;
                        ParameterBlockElement2 elem;
                        elem.name = field->getName();
                        elem.kind = ParameterBlockElementKind_ConstantBuffer;
                        // Get the size of the constant buffer contents
                        slang::TypeLayoutReflection* fieldTypeLayout = field->getTypeLayout();
                        slang::TypeLayoutReflection* cbElementLayout =
                            fieldTypeLayout->getElementTypeLayout();
                        elem.constantBufferSize = static_cast<int16_t>(
                            cbElementLayout->getSize(SLANG_PARAMETER_CATEGORY_UNIFORM));
                        layout.elements.push_back(elem);
                        break;
                    }
                    case slang::TypeReflection::Kind::Resource: {
                        ParameterBlockElement2 elem;
                        elem.name = field->getName();
                        elem.kind = ParameterBlockElementKind_Texture;
                        elem.constantBufferSize = -1;
                        layout.elements.push_back(elem);
                        break;
                    }
                    case slang::TypeReflection::Kind::SamplerState: {
                        ParameterBlockElement2 elem;
                        elem.name = field->getName();
                        elem.kind = ParameterBlockElementKind_SamplerState;
                        elem.constantBufferSize = -1;
                        layout.elements.push_back(elem);
                        break;
                    }
                    default:
                        // Scalar, Vector, Matrix, Struct fields -> plain data (implicit CB)
                        hasPlainData = true;
                        break;
                }
            }
        }
        else {
            // Non-struct element type (e.g., scalar, vector, matrix directly in ParameterBlock)
            hasPlainData = true;
        }

        if (hasPlainData && !hasExplicitCB) {
            // Entire struct is an implicit constant buffer
            layout.hasImplicitConstantBuffer = true;
            ParameterBlockElement2 elem;
            elem.name = "";
            elem.kind = ParameterBlockElementKind_ConstantBuffer;
            elem.constantBufferSize = static_cast<int16_t>(
                elementTypeLayout->getSize(SLANG_PARAMETER_CATEGORY_UNIFORM));
            // Insert at the beginning (implicit CB is typically binding 0)
            layout.elements.insert(layout.elements.begin(), elem);
        }

        m_shader->addParameterBlock(std::move(layout));
    }

    m_parameterBlocksBuilt = true;
    return LNSHADER_OK();
}

VoidResult ShaderCompiler2::buildTarget(ShaderTarget target, int targetIndex) {
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

VoidResult ShaderCompiler2::buildEntryPoint(
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

    // Analyze Varyings (vertex attributes).
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
            traverseVariableSemantic(parameter, callback);
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

    TargetEntryPoint2* entryPoint = m_shader->createEntryPoint();
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

    // Extract per-ParameterBlock binding info for this target.
    int programParameterCount = programLayout->getParameterCount();
    for (int i = 0; i < programParameterCount; i++) {
        slang::VariableLayoutReflection* parameter = programLayout->getParameterByIndex(i);
        slang::TypeReflection* type = parameter->getType();
        slang::TypeLayoutReflection* typeLayout = parameter->getTypeLayout();
        slang::TypeReflection::Kind kind = type->getKind();

        if (kind != slang::TypeReflection::Kind::ParameterBlock) {
            continue;
        }

        int16_t setIndex = -1;
        {
            int categoryCount = parameter->getCategoryCount();
            for (int ic = 0; ic < categoryCount; ic++) {
                slang::ParameterCategory cat = parameter->getCategoryByIndex(ic);
                if (cat == slang::ParameterCategory::SubElementRegisterSpace ||
                    cat == slang::ParameterCategory::RegisterSpace) {
                    setIndex = static_cast<int16_t>(parameter->getOffset(cat));
                    break;
                }
            }
            if (setIndex < 0) {
                setIndex = static_cast<int16_t>(
                    parameter->getBindingSpace(slang::ParameterCategory::RegisterSpace));
            }
        }

        slang::TypeLayoutReflection* elementTypeLayout = typeLayout->getElementTypeLayout();
        slang::TypeReflection* elementType = elementTypeLayout->getType();
        slang::TypeReflection::Kind elementKind = elementType->getKind();

        bool hasExplicitCB = false;
        bool hasPlainData = false;

        if (elementKind == slang::TypeReflection::Kind::Struct) {
            int fieldCount = elementTypeLayout->getFieldCount();
            for (int f = 0; f < fieldCount; f++) {
                slang::VariableLayoutReflection* field = elementTypeLayout->getFieldByIndex(f);
                slang::TypeReflection* fieldType = field->getType();
                slang::TypeLayoutReflection* fieldTypeLayout = field->getTypeLayout();
                slang::TypeReflection::Kind fieldKind = fieldType->getKind();

                switch (fieldKind) {
                    case slang::TypeReflection::Kind::ConstantBuffer: {
                        hasExplicitCB = true;
                        TargetBinding2 binding;
                        binding.name = field->getName();
                        binding.kind = ParameterBlockElementKind_ConstantBuffer;
                        binding.setIndex = setIndex;
                        binding.bindingIndex = static_cast<int16_t>(
                            field->getOffset(slang::ParameterCategory::ConstantBuffer));
                        slang::TypeLayoutReflection* cbElementLayout =
                            fieldTypeLayout->getElementTypeLayout();
                        binding.size = static_cast<int16_t>(
                            cbElementLayout->getSize(SLANG_PARAMETER_CATEGORY_UNIFORM));
                        binding.used = stageFlags;
                        entryPoint->bindingLayout.bindings.push_back(binding);
                        break;
                    }
                    case slang::TypeReflection::Kind::Resource: {
                        TargetBinding2 binding;
                        binding.name = field->getName();
                        binding.kind = ParameterBlockElementKind_Texture;
                        binding.setIndex = setIndex;
                        binding.bindingIndex = static_cast<int16_t>(
                            field->getOffset(slang::ParameterCategory::ShaderResource));
                        binding.size = 0;
                        binding.used = stageFlags;
                        entryPoint->bindingLayout.bindings.push_back(binding);
                        break;
                    }
                    case slang::TypeReflection::Kind::SamplerState: {
                        TargetBinding2 binding;
                        binding.name = field->getName();
                        binding.kind = ParameterBlockElementKind_SamplerState;
                        binding.setIndex = setIndex;
                        binding.bindingIndex = static_cast<int16_t>(
                            field->getOffset(slang::ParameterCategory::SamplerState));
                        binding.size = 0;
                        binding.used = stageFlags;
                        entryPoint->bindingLayout.bindings.push_back(binding);
                        break;
                    }
                    default:
                        hasPlainData = true;
                        break;
                }
            }
        }
        else {
            hasPlainData = true;
        }

        if (hasPlainData && !hasExplicitCB) {
            // Implicit constant buffer for the entire struct
            TargetBinding2 binding;
            binding.name = "";
            binding.kind = ParameterBlockElementKind_ConstantBuffer;
            binding.setIndex = setIndex;
            binding.bindingIndex = 0; // Implicit CB is at binding 0
            binding.size = static_cast<int16_t>(
                elementTypeLayout->getSize(SLANG_PARAMETER_CATEGORY_UNIFORM));
            binding.used = stageFlags;
            entryPoint->bindingLayout.bindings.push_back(binding);
        }
    }

    return LNSHADER_OK();
}

VoidResult ShaderCompiler2::buildTargetShaderPass(
    ShaderTarget target, int targetIndex, GlobalShaderPass2* globalShaderPass) {

    TargetShaderPass2* targetShaderPass = m_shader->createTargetShaderPass();
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

void ShaderCompiler2::traverseVariableSemantic(
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
                    traverseVariableSemantic(structTypeLayout->getFieldByIndex(ff), callback);
                }
            } break;
        }
    }
}

VoidResult ShaderCompiler2::mergeTargetBindingLayouts() {
    const auto& targetEntryPoints = m_shader->targetEntryPoints();
    for (auto& targetShaderPass : m_shader->targetShaderPasses()) {
        bool reset = true;

        if (targetShaderPass->vertEntryPointId >= 0) {
            const auto& entryPoint = targetEntryPoints[targetShaderPass->vertEntryPointId];
            auto result = UnifiedShader2::mergeTargetBindingLayout(
                targetShaderPass->bindingLayout, entryPoint->bindingLayout, reset);
            if (!result) return result;
            reset = false;
        }

        if (targetShaderPass->fragEntryPointId >= 0) {
            const auto& entryPoint = targetEntryPoints[targetShaderPass->fragEntryPointId];
            auto result = UnifiedShader2::mergeTargetBindingLayout(
                targetShaderPass->bindingLayout, entryPoint->bindingLayout, reset);
            if (!result) return result;
            reset = false;
        }

        if (targetShaderPass->compEntryPointId >= 0) {
            const auto& entryPoint = targetEntryPoints[targetShaderPass->compEntryPointId];
            auto result = UnifiedShader2::mergeTargetBindingLayout(
                targetShaderPass->bindingLayout, entryPoint->bindingLayout, reset);
            if (!result) return result;
            reset = false;
        }
    }

    return LNSHADER_OK();
}

Result<VertexInputAttribute> ShaderCompiler2::makeVertexInputAttribute(
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
