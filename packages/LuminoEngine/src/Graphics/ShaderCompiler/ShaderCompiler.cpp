#include <fstream>
#include "Internal.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShader2.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/ShaderCompiler.hpp>
#include "ShaderMetadataParser.hpp"
#include "DescriptorLayoutBuilder.hpp"

#ifdef LN_USE_SLANG
// https://shader-slang.org/slang/user-guide/compiling#using-the-compilation-api
// https://github.com/shader-slang/slang/pull/6679
//#pragma comment(lib, "C:/Proj/LN/Lumino/vcpkg/packages/shader-slang_x64-windows/lib/slang.lib")
#pragma comment(lib, "E:/Proj/Lumino/vcpkg/packages/shader-slang_x64-windows/lib/slang.lib")
static slang::CompilerOptionValue fromInt3(uint8_t v0, int v1, int v2) {
    slang::CompilerOptionValue value;
    value.intValue0 = (v0 << 24) + (v1 & 0xFFFFFF);
    value.intValue1 = v2;
    value.kind = slang::CompilerOptionValueKind::Int;
    return value;
}
namespace ln {
namespace kokage {

static const struct {
    ShaderTarget target;
    const char* profile;
} kTargets[] = {
    // Check tool: dpirv-dis <file>
    { ShaderTarget_SPIRV, "glsl_450" },

    // DXIL は glsl_450 ではダメだったので sm_6_0 としている。 (error X3506: unrecognized compiler target ...)
    // Check tool: dxc -dumpbin <file>
    { ShaderTarget_DXIL, "sm_6_0" },
    { ShaderTarget_WGSL, "" },
    { ShaderTarget_METAL, "glsl_450" },
};
static const int kTargetCount = SLANG_COUNT_OF(kTargets);

struct BindingDumpInfo {
    std::string name;
    SlangParameterCategory category;
    int offset; // for global variable. ($Global)
    int size;   // for global variable. ($Global)
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
        case ShaderTarget::ShaderTarget_SPIRV:
            return SLANG_SPIRV;
        case ShaderTarget::ShaderTarget_DXIL:
            return SLANG_DXIL;
        case ShaderTarget::ShaderTarget_WGSL:
            return SLANG_WGSL;
        case ShaderTarget::ShaderTarget_METAL:
            return SLANG_METAL;
        default:
            LN_UNREACHABLE();
            return SLANG_SPIRV;
    }
}

static RegisterCategory toLuminoCategory(SlangParameterCategory category) {
    switch (category) {
        case SLANG_PARAMETER_CATEGORY_CONSTANT_BUFFER:
            return RegisterCategory_UniformBuffer;
        case SLANG_PARAMETER_CATEGORY_SHADER_RESOURCE:
            return RegisterCategory_TextureOrCombinedSampler;
        case SLANG_PARAMETER_CATEGORY_UNORDERED_ACCESS:
            return RegisterCategory_UnorderdAccess;
        case SLANG_PARAMETER_CATEGORY_SAMPLER_STATE:
            return RegisterCategory_SamplerState;
        default:
            LN_UNREACHABLE();
            return RegisterCategory_Unknown;
    }
}

static ShaderGlobalMemberType toLuminoShaderGlobalMemberType(
    slang::TypeReflection::ScalarType scalarType) {
    switch (scalarType) {
        case slang::TypeReflection::ScalarType::Bool:
            return ShaderGlobalMemberType_Bool;
        case slang::TypeReflection::ScalarType::Int32:
            return ShaderGlobalMemberType_Int;
        case slang::TypeReflection::ScalarType::Float32:
            return ShaderGlobalMemberType_Float;
        default:
            return ShaderGlobalMemberType_Unknown;
    }
}

static const char* getTargetName(ShaderTarget target) {
    switch (target) {
        case ShaderTarget::ShaderTarget_SPIRV:
            return "SPIRV";
        case ShaderTarget::ShaderTarget_DXIL:
            return "DXIL";
        case ShaderTarget::ShaderTarget_WGSL:
            return "WGSL";
        case ShaderTarget::ShaderTarget_METAL:
            return "METAL";
        default:
            LN_UNREACHABLE();
            return "?";
    }
}

static const char* getExt(ShaderTarget target) {
    switch (target) {
        case ShaderTarget::ShaderTarget_SPIRV:
            return ".spv";
        case ShaderTarget::ShaderTarget_DXIL:
            return ".dxil";
        case ShaderTarget::ShaderTarget_WGSL:
            return ".wgsl";
        case ShaderTarget::ShaderTarget_METAL:
            return ".msl";
        default:
            LN_UNREACHABLE();
            return "?";
    }
}

static const char* getSlangCategoryName(SlangParameterCategory category) {
    switch (category) {
        case SLANG_PARAMETER_CATEGORY_NONE:
            return "None";
        case SLANG_PARAMETER_CATEGORY_MIXED:
            return "Mixed";
        case SLANG_PARAMETER_CATEGORY_CONSTANT_BUFFER:
            return "ConstantBuffer";
        case SLANG_PARAMETER_CATEGORY_SHADER_RESOURCE:
            return "ShaderResource";
        case SLANG_PARAMETER_CATEGORY_UNORDERED_ACCESS:
            return "UnorderedAccess";
        case SLANG_PARAMETER_CATEGORY_VARYING_INPUT:
            return "VaryingInput";
        case SLANG_PARAMETER_CATEGORY_VARYING_OUTPUT:
            return "VaryingOutput";
        case SLANG_PARAMETER_CATEGORY_SAMPLER_STATE:
            return "SamplerState";
        case SLANG_PARAMETER_CATEGORY_UNIFORM:
            return "Uniform";
        case SLANG_PARAMETER_CATEGORY_DESCRIPTOR_TABLE_SLOT:
            return "DescriptorTableSlot";
        case SLANG_PARAMETER_CATEGORY_SPECIALIZATION_CONSTANT:
            return "SpecializationConstant";
        case SLANG_PARAMETER_CATEGORY_PUSH_CONSTANT_BUFFER:
            return "PushConstantBuffer";
        case SLANG_PARAMETER_CATEGORY_REGISTER_SPACE:
            return "RegisterSpace";
        case SLANG_PARAMETER_CATEGORY_GENERIC:
            return "Generic";
        case SLANG_PARAMETER_CATEGORY_RAY_PAYLOAD:
            return "RayPayload";
        case SLANG_PARAMETER_CATEGORY_HIT_ATTRIBUTES:
            return "HitAttributes";
        case SLANG_PARAMETER_CATEGORY_CALLABLE_PAYLOAD:
            return "CallablePayload";
        case SLANG_PARAMETER_CATEGORY_SHADER_RECORD:
            return "ShaderRecord";
        case SLANG_PARAMETER_CATEGORY_EXISTENTIAL_TYPE_PARAM:
            return "ExistentialTypeParam";
        case SLANG_PARAMETER_CATEGORY_EXISTENTIAL_OBJECT_PARAM:
            return "ExistentialObjectParam";
        case SLANG_PARAMETER_CATEGORY_SUB_ELEMENT_REGISTER_SPACE:
            return "SubElementRegisterSpace";
        case SLANG_PARAMETER_CATEGORY_SUBPASS:
            return "Subpass";
        case SLANG_PARAMETER_CATEGORY_METAL_ARGUMENT_BUFFER_ELEMENT:
            return "MetalArgumentBufferElement";
        case SLANG_PARAMETER_CATEGORY_METAL_ATTRIBUTE:
            return "MetalAttribute";
        case SLANG_PARAMETER_CATEGORY_METAL_PAYLOAD:
            return "MetalPayload";
        default:
            return "Unknown";
    }
}

Result<URef<ShaderCompiler>> ShaderCompiler::create() {
    URef<ShaderCompiler> ref(LN_NEW ShaderCompiler());
    auto result = ref->init();
    if (!result) LN_TO_ERROR(result);
    return ref;
}

ShaderCompiler::ShaderCompiler()
    : m_globalSession()
    , m_inputDirPath()
    , m_dumpDirPath()
    , m_dump(true) { // TEST
}

ShaderCompiler::~ShaderCompiler() {
}

MaybeResult ShaderCompiler::init() {
    SlangGlobalSessionDesc desc = {};
    SlangResult result = slang::createGlobalSession(&desc, m_globalSession.writeRef());
    if (SLANG_FAILED(result)) {
        return LN_MAKE_ERROR("slang::createGlobalSession failed. (%d)", result);
    }
    return LN_MAKE_SUCCESS();
}

MaybeResult ShaderCompiler::build(const fs::path& inputFilePath) {
    m_inputFilePath = inputFilePath;
    m_inputDirPath = inputFilePath.parent_path();
    m_shader = makeRef<UnifiedShader2>();

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
                return LN_MAKE_ERROR("ifstream failed.");
            }
            stream.read(&code[0], fileSize);
        }
        catch (const fs::filesystem_error& e) {
            return LN_MAKE_ERROR("ifstream failed. (%s)", e.what());
        }
    }

    // Parse metadata.
    {
        ShaderMetadataParser parser;
        auto result = parser.parse(code);
        if (!result) return tl::unexpected(result.unwrapErr());
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
    return LN_MAKE_SUCCESS();
}

MaybeResult ShaderCompiler::buildModule() {
    // NOTE: slang の Component をまとめて結合する方法は
    // slang リポジトリの examples\reflection-api\main.cpp が参考になりそう。

    std::vector<slang::TargetDesc> targetDescs;
    for (auto target : kTargets) {
        auto profile = m_globalSession->findProfile(target.profile);
        slang::TargetDesc targetDesc;
        targetDesc.format = toSlangTarget(target.target);
        targetDesc.profile = profile;
        targetDescs.push_back(targetDesc);
    }
    
    slang::SessionDesc sessionDesc = {};
    sessionDesc.targets = targetDescs.data();
    sessionDesc.targetCount = targetDescs.size();

    const char* searchPaths[] = { "myapp/shaders/" };
    sessionDesc.searchPaths = searchPaths;
    sessionDesc.searchPathCount = 1;

    slang::PreprocessorMacroDesc fancyFlag = { "ENABLE_FANCY_FEATURE", "1" };
    sessionDesc.preprocessorMacros = &fancyFlag;
    sessionDesc.preprocessorMacroCount = 1;

    Slang::ComPtr<slang::ISession> session;
    SlangResult result = m_globalSession->createSession(sessionDesc, session.writeRef());
    if (SLANG_FAILED(result)) {
        return LN_MAKE_ERROR("createSession failed. (%d)", result);
    }

    // Load shader file
    std::string inputFilePath = m_inputFilePath.string();
    Slang::ComPtr<slang::IBlob> diagnostics;
    m_module = session->loadModule(inputFilePath.c_str(), diagnostics.writeRef());
    if (diagnostics) {
        LN_LOG_INFO(reinterpret_cast<const char*>(diagnostics->getBufferPointer()));
    }
    if (!m_module) {
        return LN_MAKE_ERROR("ISession::loadModule failed.");
    }

    // [shader("vertex")] などで指定されたエントリポイントを取得するために、一度すべてのエントリポイントを含めてリンクする必要がある。
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
        return LN_MAKE_ERROR("createCompositeComponentType failed. (%d)", result);
    }

    // Link
    Slang::ComPtr<slang::IBlob> linkDiag;
    result = composed->link(m_program.writeRef(), linkDiag.writeRef());
    if (linkDiag) {
        std::string message(static_cast<const char*>(linkDiag->getBufferPointer()), linkDiag->getBufferSize());
        LN_LOG_INFO(message);
    }
    if (SLANG_FAILED(result)) {
        return LN_MAKE_ERROR("link failed. (%d)", result);
    }

    for (int i = 0; i < kTargetCount; i++) {
        auto& target = kTargets[i];
        auto result1 = buildInputResources(i);
        if (!result1) return result1;
        auto result = buildTarget(target.target, i);
        if (!result) return result;
    }

    {
        auto r1 = mergeTargetInputResources();
        if (!r1) return r1;
    }

    {
        DescriptorLayoutBuilder builder;
        auto r1 = builder.buildDescriptorLayout(m_shader);
        if (!r1) return r1;
    }
    return LN_MAKE_SUCCESS();
}

// targetIndex=0 でのみ取得すれば良さそうな気がするけど、
// 一応全ての Target を調べる。
// なお TargetEntryPoint 側からボトムアップで求めることも出来そうだが、
// TargetEntryPoint からは binding index しか主に取得できず型情報や配列要素数が無い。
MaybeResult ShaderCompiler::buildInputResources(int targetIndex) {
    slang::ProgramLayout* programLayout = m_program->getLayout(targetIndex);
    int parameterCount = programLayout->getParameterCount();

    // Collect $Global ConstantBuffer members.
    // NOTE: getGlobalConstantBufferSize() や getGlobalParamsVarLayout() を使わないのか？
    //   https://shader-slang.org/slang/user-guide/reflection.html#calculating-cumulative-offsets
    //   こちらを見る限り、 getGlobalParamsTypeLayout() よりも getGlobalParamsVarLayout() の方が良いかもしれない。
    //   実際、 getGlobalConstantBufferSize() は Target に応じて count だったり size だったり意味が違っていたので、そのままでは使えない。
    //   また原因不明だが、 getGlobalParamsVarLayout() 経由で取得した 変数情報ではサイズが正しく取得できなかった。これも Target によって異なっていて意味不明。
    //   なので getParameterCount() から uniform である変数を列挙する方法で取得することにした。
    bool hasGlobalConstantBuffer = false;
    for (int i = 0; i < parameterCount; i++) {
        slang::VariableLayoutReflection* parameter = programLayout->getParameterByIndex(i);
        slang::ParameterCategory category = parameter->getCategory();
        if (category != slang::ParameterCategory::Uniform) {
            continue; // GlobalUniform. Containd in $Global.
        }

        std::string name = parameter->getName();
        int32_t arrayElements = 0;
        int32_t vectorElements = 0;
        int32_t matrixRows = 0;
        int32_t matrixColumns = 0;
        ShaderGlobalMemberKind memberKind = ShaderGlobalMemberKind_Unknown;
        ShaderGlobalMemberType memberType = ShaderGlobalMemberType_Unknown;

        // NOTE:
        // - TypeReflection: 型情報のコア
        // - TypeLayoutReflection: TypeReflection をラップするユーティリティ？
        //　　category を使って size 等を取得するが、CombinedSampler のような複数の値の格納を意図している？
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
                return LN_MAKE_ERROR(
                    "Invalid kind. (%s:%d)",
                    name.c_str(), kind);
        }

        slang::TypeReflection::ScalarType scalarType = slang::TypeReflection::ScalarType::None;
        if (kind == slang::TypeReflection::Kind::Array) {
            slang::TypeReflection* elemetType = type->getElementType();
            memberType = toLuminoShaderGlobalMemberType(elemetType->getScalarType());
        }
        else {
            memberType = toLuminoShaderGlobalMemberType(type->getScalarType());
        }
        if (memberType == ShaderGlobalMemberType_Unknown) {
            return LN_MAKE_ERROR("Invalid type. (%s:%d)", name.c_str(), scalarType);
        }

        auto result = m_shader->getOrCreateGlobalMemberWithVerify(
            name,
            memberType,
            memberKind,
            arrayElements,
            vectorElements,
            matrixRows,
            matrixColumns);
        if (!result) return LN_TO_ERROR(result);
        hasGlobalConstantBuffer = true;
    }

    if (hasGlobalConstantBuffer) {
        auto result = m_shader->getOrCreateInputResourceWithVerify(
            kGlobalConstantBufferName,
            RegisterCategory_UniformBuffer,
            -1, // ConstantBuffer は $Global に限り、サイズ無効値としておく（UnifiedShader2 の alignment に関するコメント参照）
            0);
        if (!result) return result;
    }

    // Collect non-$Global parameters.
    for (int i = 0; i < parameterCount; i++) {
        slang::VariableLayoutReflection* parameter = programLayout->getParameterByIndex(i);
        int categoryCount = parameter->getCategoryCount();

        // getCategory() は category が 1 つしかない場合は getCategoryByIndex(0) の値を返し、複数の場合は Mixed を返す。
        slang::ParameterCategory category = parameter->getCategory();
        if (category == slang::ParameterCategory::Uniform) {
            continue; // GlobalUniform. Containd in $Global.
        }

        std::string name;
        RegisterCategory registerCategory = RegisterCategory_Unknown;
        int constantBufferSize = 0;
        int arrayElementCount = 0;
        auto result1 = getBindingResourceInfo(
            parameter,
            &name,
            &registerCategory,
            &constantBufferSize,
            &arrayElementCount);
        if (!result1) return result1;

        auto result = m_shader->getOrCreateInputResourceWithVerify(
            name,
            registerCategory,
            constantBufferSize,
            arrayElementCount);
        if (!result) {
            return result;
        }
    }

    return LN_MAKE_SUCCESS();
}

MaybeResult ShaderCompiler::buildTarget(ShaderTarget target, int targetIndex) {
    slang::ProgramLayout* layout = m_program->getLayout(targetIndex);

    
    // Dump target reflection
    // NOTE: この Dump は moduel 全部 Composite した状態で行った方が良いだろう。
    //       そうしないと、EntryPoint の情報が出力されない。
    if (m_dump) {
        fs::path filePath = m_dumpDirPath / (std::string(getTargetName(target)) + ".target-reflection.json");
        Slang::ComPtr<slang::IBlob> text;
        SlangResult result = layout->toJson(text.writeRef());
        if (SLANG_FAILED(result)) {
            return LN_MAKE_ERROR("toJson failed. (%d)", result);
        }
        std::ofstream stream(filePath);
        if (stream.fail()) {
            return LN_MAKE_ERROR("ofstream failed. (%d)", result);
        }
        stream.write(reinterpret_cast<const char*>(text->getBufferPointer()), text->getBufferSize());
    }

    //{
    //    auto r = buildModuleInfoSPIRV(module->getLayout(), moduleInfo);
    //    if (!r) return r;
    //}

    //slang::IModule* module =
    //    session->loadModule("C:/Proj/LN/Lumino/packages/LuminoEngine/shader/CopyScreen.slang", diagnostics.writeRef());
    //slang::IModule* module =
    //    session->loadModule("E:/Proj/Lumino/packages/LuminoEngine/shader/CopyScreen.slang", diagnostics.writeRef());
    //slang::ProgramLayout* layout3 = module->getLayout();
    //int pc = layout3->getEntryPointCount();
    //slang::EntryPointReflection* pp = layout3->getEntryPointByIndex(0);

    //Slang::ComPtr<SlangCompileRequest> compileRequest;
    //result = session->createCompileRequest(compileRequest.writeRef());
    //if (SLANG_FAILED(result)) {
    //    return LN_MAKE_ERROR("createCompileRequest failed. (%d)", result);
    //}
    

    //Slang::ComPtr<slang::IEntryPoint> targetEntryPoint;
    //result = module->findAndCheckEntryPoint(
    //    "PS_Main", SlangStage::SLANG_STAGE_VERTEX, targetEntryPoint.writeRef(), diagnostics.writeRef());

    //LN_LOG_INFO("  Name: {}", pp->getName());
    //LN_LOG_INFO("  Stage: {}", (int)pp->getStage());

    
    //Slang::ComPtr<slang::IComponentType> linkedProgram2;
    //Slang::ComPtr<ISlangBlob> diagnosticBlob2;
    //result = program->link(linkedProgram2.writeRef(), diagnosticBlob2.writeRef());

    //slang::ProgramLayout* layout3 = linkedProgram2->getLayout();
    //int ec = layout3->getEntryPointCount();
    //layout3->toJson(diagnosticBlob2.writeRef());
    //std::string code2((const char*)diagnosticBlob2->getBufferPointer(), diagnosticBlob2->getBufferSize());
    


    // Build entry points
    int entryPointCount = m_module->getDefinedEntryPointCount();
    for (int iEntryPoint = 0; iEntryPoint < entryPointCount; iEntryPoint++) {
        auto r = buildEntryPoint(target, targetIndex, iEntryPoint);
        if (!r) return r;
    }
    
    // Link shader passes.
    //const auto& entryPoints = m_shader->targetEntryPoints();
    for (auto& globalShaderPass : m_shader->globalShaderPasses()) {
        auto result = buildTargetShaderPass(target, targetIndex, globalShaderPass.get());
        if (!result) {
            return result;
        }
    }



    //slang::ProgramLayout* layout = linkedProgram->getLayout();

    //int entryPointCount = m_module->getDefinedEntryPointCount();
    //for (int i = 0; i < entryPointCount; i++) {
    //    slang::EntryPointLayout* entryPointLayout = layout->getEntryPointByIndex(i);
    //    //slang::EntryPointReflection* entryPointReflection = entryPointLayout->getTargetEntryPoint();
    //    //slang::FunctionReflection* functionReflection = entryPointReflection->getFunctionReflection();
    //    //LN_LOG_INFO("  Name: {}", functionReflection->getName());
    //    //int att = functionReflection->getUserAttributeCount();
    //    //for (int j = 0; j < att; j++) {
    //    //    slang::UserAttribute* ua = functionReflection->getUserAttributeByIndex(j);
    //    //    LN_LOG_INFO("    UserAttribute: {}", ua->getName());
    //    //}
    //}



    //layout->toJson(diagnosticBlob2.writeRef());
    ////SlangReflection* reflection = spGetReflection(compileRequest);
    ////spReflection_ToJson((SlangReflection*)layout, nullptr, diagnosticBlob2.writeRef());
    //std::string code2((const char*)diagnosticBlob2->getBufferPointer(), diagnosticBlob2->getBufferSize());
        

    //Slang::ComPtr<slang::IEntryPoint> computeEntryPoint;
    //result = module->findEntryPointByName("computeMain", computeEntryPoint.writeRef());
    //result = module->findEntryPointByName("PS_Main", computeEntryPoint.writeRef());

    //slang::IComponentType* components[] = { module, computeEntryPoint };
    //Slang::ComPtr<slang::IComponentType> program;
    //result = session->createCompositeComponentType(components, 1, program.writeRef());


    //slang::ProgramLayout* layout2 = linkedProgram->getLayout();
    //layout->toJson(diagnosticBlob2.writeRef());
    //std::string code3((const char*)diagnosticBlob2->getBufferPointer(), diagnosticBlob2->getBufferSize());



    return LN_MAKE_SUCCESS();
}

MaybeResult ShaderCompiler::buildEntryPoint(
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
        entryPointIndex,
        targetIndex, // target index
        entryPointMetadata.writeRef(),
        diag.writeRef());
    if (SLANG_FAILED(result)) {
        std::string message(static_cast<const char*>(diag->getBufferPointer()), diag->getBufferSize());
        return LN_MAKE_ERROR("getEntryPointMetadata failed. (%d): %s", result, message.c_str());
    }

    // Analyze Varyings.
    // see: slang emitReflectionEntryPointJSON()
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

        // fragment でも有効にしてしまうと SV_POSITION とか拾ってしまうので、vertex のみ。
        if (stage == SLANG_STAGE_VERTEX) {
            for (const auto& info : varyingDumpInfos) {
                auto result = makeVertexInputAttribute(
                    info.name,
                    info.semanticName,
                    info.semanticIndex,
                    info.locationIndex);
                if (!result) return LN_TO_ERROR(result);
                inputAttributes.push_back(result.value());
            }
        }
    }

    // Dump bindings to JSON.
    if (m_dump) {
        // NOTE: slangc -reflection-json では TargetEntryPoint ごとに used な Parameter が出力されるが、
        //   それはレガシーな API を使っているようで、 公開 API からは取得することができなかった。
        //   (toJson() 内部の spReflection_ToJson() の第２引数に Request を指定する必要があるが、それだけだとクラッシュした)
        //   なので同様の JSON は出力できない点に注意。
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

                    // see: slang emitReflectionVarBindingInfoJSON()
                    BindingDumpInfo dumpInfo;
                    dumpInfo.name = parameter->getName();
                    dumpInfo.category = category;
                    if (category == SLANG_PARAMETER_CATEGORY_UNIFORM) {
                        dumpInfo.name = parameter->getName();
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
                        dumpInfo.index = indexOrOffset;
                        dumpInfo.count = sizeOrCount;
                    }

                    // NOTE: uniform ($global) だと常に false になる。
                    bool used = false;
                    bool usedAvailable = entryPointMetadata->isParameterLocationUsed(
                                             category,
                                             space,
                                             indexOrOffset,
                                             used) == SLANG_OK;
                    if (usedAvailable) {
                        dumpInfo.used = used ? 1 : 0;
                    }
                    else {
                        dumpInfo.used = -1;
                    }
                    bindingDumpInfos.push_back(dumpInfo);
                }
            }
        }


        const char* name = entryPointReflection->getName();
        fs::path filePath =
            m_dumpDirPath / (std::string(getTargetName(target)) + ".entry-reflection." + name + ".reflection.json");
        std::ofstream stream(filePath);
        if (stream.fail()) {
            return LN_MAKE_ERROR("ofstream failed. (%d)", result);
        }

        stream << "{\n";
        stream << "    \"bindings\": [\n";
        for (int i = 0; i < bindingDumpInfos.size(); i++) {
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
            if (i < bindingDumpInfos.size() - 1) {
                stream << "},\n";
            }
            else {
                stream << "}\n";
            }
        }
        stream << "    ],\n";
        stream << "    \"varyings\": [\n";
        for (int i = 0; i < varyingDumpInfos.size(); i++) {
            const VaryingDumpInfo& info = varyingDumpInfos[i];
            stream << "        { ";
            stream << "\"name\": \"" << info.name << "\", ";
            stream << "\"semanticName\": \"" << info.semanticName << "\", ";
            stream << "\"semanticIndex\": " << info.semanticIndex << " ";
            if (i < varyingDumpInfos.size() - 1) {
                stream << "},\n";
            }
            else {
                stream << "}\n";
            }
        }
        stream << "    ]\n";
        stream << "}\n";
    }

    // Generate code.
    Blob* codeBlob = nullptr; 
    {
        Slang::ComPtr<slang::IBlob> kernelBlob;
        Slang::ComPtr<slang::IBlob> generateDiag;
        result =
            m_program->getEntryPointCode(entryPointIndex, targetIndex, kernelBlob.writeRef(), generateDiag.writeRef());
        if (generateDiag) {
            std::string message(
                static_cast<const char*>(generateDiag->getBufferPointer()), generateDiag->getBufferSize());
            LN_LOG_INFO(message);
        }
        if (SLANG_FAILED(result)) {
            return LN_MAKE_ERROR("link failed. (%d)", result);
        }

        // Dump code.
        if (m_dump) {
            const char* name = entryPointReflection->getName();
            fs::path filePath = m_dumpDirPath / (std::string(getTargetName(target)) + "." + name + getExt(target));
            std::ofstream stream(filePath, std::ios::binary);
            if (stream.fail()) {
                return LN_MAKE_ERROR("ofstream failed. (%d)", result);
            }
            stream.write(reinterpret_cast<const char*>(kernelBlob->getBufferPointer()), kernelBlob->getBufferSize());
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
        return LN_MAKE_ERROR("Invalid stage. (%d)", stage);
    }

    // $Global 相当の ConstantBuffer が生成されていれば、 binding 情報として取り出す。
    // ※ $Global がある場合、 size が 0 より大きい値になる。そのとき、他の ContantBuffer は index が 1 から始まる。（$Global がなければ 0 から始まる）
    int globalConstantBufferSize = programLayout->getGlobalConstantBufferSize();
    if (globalConstantBufferSize > 0) {
        TargetBindingInfo binding;
        binding.name = kGlobalConstantBufferName;
        binding.category = BindingResourceCategory_UniformBuffer;
        binding.size = globalConstantBufferSize;
        binding.space = 0;
        binding.index = programLayout->getGlobalConstantBufferBinding();
        binding.count = 0;
        binding.used = stageFlags;
        
        // $Global members.
        int programParameterCount = programLayout->getParameterCount();
        for (int i = 0; i < programParameterCount; i++) {
            slang::VariableLayoutReflection* parameter = programLayout->getParameterByIndex(i);
            slang::ParameterCategory category = parameter->getCategory();
            if (category != slang::ParameterCategory::Uniform) {
                continue; // Non $Global.
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
                continue; // $Global.
            }

            int categoryCount = parameter->getCategoryCount();
            for (int i = 0; i < categoryCount; i++) {
                slang::ParameterCategory category = parameter->getCategoryByIndex(i);

                TargetBindingInfo bindingInfo;
                bindingInfo.name = parameter->getName();
                bindingInfo.size = 0;
                bindingInfo.space = parameter->getBindingSpace(category);
                bindingInfo.index = parameter
                                        ->getBindingIndex(); // parameter->getOffset(category);
                bindingInfo.count = typeLayout->getSize(category);
                bindingInfo.used = stageFlags;

                bool withSampler = false;

                // ConstantBuffer は中身を見に行かないとサイズがわからない。
                slang::TypeReflection* type = parameter->getType();
                slang::TypeReflection::Kind kind = type->getKind();
                switch (kind) {
                    case slang::TypeReflection::Kind::ConstantBuffer: {
                        bindingInfo.category = BindingResourceCategory_UniformBuffer;
                        auto* elementLayout = typeLayout->getElementVarLayout();
                        auto* typeLayout2 = elementLayout->getTypeLayout();
                        int categoryCount2 = elementLayout->getCategoryCount();
                        if (categoryCount2 > 1) {
                            return LN_MAKE_ERROR(
                                "Multiple category unsupported. (%d)",
                                categoryCount2);
                        }
                        slang::ParameterCategory category = elementLayout->getCategoryByIndex(
                            0);
                        bindingInfo.size = typeLayout2->getSize(category);
                        break;
                    }
                    case slang::TypeReflection::Kind::Resource: {
                        if (category == slang::ParameterCategory::DescriptorTableSlot &&
                            bindingInfo.count == 2) {
                            // WGSL target で reflection-json を確認すると、Combined Sampler の場合
                            // bindings には Texture の分しか出力されない。(DX12 や METAL は両方出力される)
                            // 代わりに count が 2 になって index がその分パディングされているので、
                            // これを検知した場合は SamplerState を追加する。
                            withSampler = true;
                        }
                        bindingInfo.category = BindingResourceCategory_TextureOrCombinedSampler;
                        break;
                    }
                    case slang::TypeReflection::Kind::SamplerState:
                        bindingInfo.category = BindingResourceCategory_SamplerState;
                        break;
                    case slang::TypeReflection::Kind::Array:
                        // TODO:
                        return LN_MAKE_ERROR(
                            "Array types are not supported.",
                            bindingInfo.name.c_str(),
                            kind);
                    default:
                        return LN_MAKE_ERROR(
                            "Invalid type. (%s:%d)",
                            bindingInfo.name.c_str(),
                            kind);
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
    
    return LN_MAKE_SUCCESS();
}

MaybeResult ShaderCompiler::buildTargetShaderPass(
    ShaderTarget target, int targetIndex, GlobalShaderPass* globalShaderPass) {

    TargetShaderPass* targetShaderPass = m_shader->createTargetShaderPass();
    targetShaderPass->globalShaderPassId = globalShaderPass->id;
    globalShaderPass->targetShaderPassIds[target - 1] = targetShaderPass->id;

    // VertexShader
    if (!globalShaderPass->vertexEntryPoint.empty()) {
        auto result = m_shader->getTargetEntryPoint(target, globalShaderPass->vertexEntryPoint);
        if (!result) LN_TO_ERROR(result);
        targetShaderPass->vertEntryPointId = result.value()->id;
    }

    // FragmentShader
    if (!globalShaderPass->fragmentEntryPoint.empty()) {
        auto result = m_shader->getTargetEntryPoint(target, globalShaderPass->fragmentEntryPoint);
        if (!result) LN_TO_ERROR(result);
        targetShaderPass->fragEntryPointId = result.value()->id;
    }

    // ComputeShader
    if (!globalShaderPass->computeEntryPoint.empty()) {
        auto result = m_shader->getTargetEntryPoint(target, globalShaderPass->computeEntryPoint);
        if (!result) LN_TO_ERROR(result);
        targetShaderPass->compEntryPointId = result.value()->id;
    }

    return LN_MAKE_SUCCESS();
}

MaybeResult ShaderCompiler::getBindingResourceInfo(
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
        // CombinedSampler の場合は Mixed になっているので、内容を確認して確定する。
        int categoryCount = parameter->getCategoryCount();
        if (categoryCount != 2) {
            return LN_MAKE_ERROR("Multiple category unsupported. (%d)", categoryCount);
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
            return LN_MAKE_ERROR("Invalid mixed category. (%d:%d)", c0, c1);
        }
    }
    else {
        slang::TypeReflection* type = parameter->getType();
        slang::TypeLayoutReflection* typeLayout = parameter->getTypeLayout();
        slang::TypeReflection::Kind kind = type->getKind();
        switch (kind) {
            case slang::TypeReflection::Kind::ConstantBuffer: {
                registerCategory = RegisterCategory_UniformBuffer;
                slang::VariableLayoutReflection* elementLayout = typeLayout->getElementVarLayout();
                slang::TypeLayoutReflection* typeLayout2 = elementLayout->getTypeLayout();
                int categoryCount2 = elementLayout->getCategoryCount();
                if (categoryCount2 > 1) {
                    return LN_MAKE_ERROR("Multiple category unsupported. 2 (%d)", categoryCount2);
                }
                slang::ParameterCategory category = elementLayout->getCategoryByIndex(0);
                constantBufferSize = typeLayout2->getSize(category);
                break;
            }
            case slang::TypeReflection::Kind::Resource: {
                registerCategory = RegisterCategory_TextureOrCombinedSampler;
                break;
            }
            case slang::TypeReflection::Kind::SamplerState:
                registerCategory = RegisterCategory_SamplerState;
                break;
            case slang::TypeReflection::Kind::Array:
                arrayElementCount = 0; // TODO:
                return LN_MAKE_ERROR("Array types are not supported.", name.c_str(), kind);
            default:
                return LN_MAKE_ERROR("Invalid type. (%s:%d)", name.c_str(), kind);
        }
    }

    *outName = name;
    *outRegisterCategory = registerCategory;
    *outConstantBufferSize = constantBufferSize;
    *outArrayElementCount = arrayElementCount;
    return LN_MAKE_SUCCESS();
}


// see: Slang::emitReflectionTypeLayoutJSON()
// see: Slang::emitReflectionVarLayoutJSON()
void ShaderCompiler::traverseVariableSemaintic(
    slang::VariableLayoutReflection* var,
    const std::function<void(slang::VariableLayoutReflection* var)>& callback) {
    if (var->getSemanticName()) {
        callback(var);
    }

    slang::TypeLayoutReflection* typeLayout = var->getTypeLayout();
    if (typeLayout) {
        // see Slang::emitReflectionTypeLayoutInfoJSON()
        switch (typeLayout->getKind()) {
            default:
                //emitReflectionTypeInfoJSON(writer, typeLayout->getType());
                break;
            case slang::TypeReflection::Kind::Struct: {
                auto structTypeLayout = typeLayout;

                //writer.maybeComma();
                //writer << "\"kind\": \"struct\"";
                //if (auto name = structTypeLayout->getName()) {
                //    writer.maybeComma();
                //    emitReflectionNameInfoJSON(writer, name);
                //}
                //writer.maybeComma();
                //writer << "\"fields\": [\n";
                //writer.indent();

                auto fieldCount = structTypeLayout->getFieldCount();
                for (uint32_t ff = 0; ff < fieldCount; ++ff) {
                    traverseVariableSemaintic(structTypeLayout->getFieldByIndex(ff), callback);
                }
                //writer.dedent();
                //writer << "\n]";
                //emitUserAttributes(writer, structTypeLayout->getType());
            } break;
        }

    }
}

MaybeResult ShaderCompiler::mergeTargetInputResources() {
    const auto& targetEntryPoints = m_shader->targetEntryPoints();
    for (auto& targetShaderPass : m_shader->targetShaderPasses()) {
        bool reset = true;

        if (targetShaderPass->vertEntryPointId >= 0) {
            const auto& entryPoint = targetEntryPoints[targetShaderPass->vertEntryPointId];
            auto result = UnifiedShader2::mergeTargetBindingLayoutInfo(
                targetShaderPass->bindingLayout,
                entryPoint->bindingLayout,
                reset);
            if (!result) return result;
            reset = false;
        }

        if (targetShaderPass->fragEntryPointId >= 0) {
            const auto& entryPoint = targetEntryPoints[targetShaderPass->fragEntryPointId];
            auto result = UnifiedShader2::mergeTargetBindingLayoutInfo(
                targetShaderPass->bindingLayout,
                entryPoint->bindingLayout,
                reset);
            if (!result) return result;
            reset = false;
        }

        if (targetShaderPass->compEntryPointId >= 0) {
            const auto& entryPoint = targetEntryPoints[targetShaderPass->compEntryPointId];
            auto result = UnifiedShader2::mergeTargetBindingLayoutInfo(
                targetShaderPass->bindingLayout,
                entryPoint->bindingLayout,
                reset);
            if (!result) return result;
            reset = false;
        }
    }

    return LN_MAKE_SUCCESS();
}

Result<VertexInputAttribute> ShaderCompiler::makeVertexInputAttribute(
    const std::string& varName,
    const std::string& semanticName,
    int semanticIndex,
    int locationIndex) {
    VertexInputAttribute attr;
    int keywordLen = 0;
    if (StringHelper::compare(
            semanticName.c_str(),
            "POSITION",
            8,
            ln::CaseSensitivity::CaseInsensitive) == 0) {
        attr.usage = AttributeUsage_Position;
        keywordLen = 8;
    }
    else if (
        StringHelper::compare(
            semanticName.c_str(),
            "BLENDWEIGHT",
            11,
            ln::CaseSensitivity::CaseInsensitive) == 0) {
        attr.usage = AttributeUsage_BlendWeight;
        keywordLen = 11;
    }
    else if (
        StringHelper::compare(
            semanticName.c_str(),
            "BLENDINDICES",
            12,
            ln::CaseSensitivity::CaseInsensitive) == 0) {
        attr.usage = AttributeUsage_BlendIndices;
        keywordLen = 12;
    }
    else if (
        StringHelper::compare(
            semanticName.c_str(),
            "NORMAL",
            6,
            ln::CaseSensitivity::CaseInsensitive) == 0) {
        attr.usage = AttributeUsage_Normal;
        keywordLen = 6;
    }
    else if (
        StringHelper::compare(
            semanticName.c_str(),
            "TEXCOORD",
            8,
            ln::CaseSensitivity::CaseInsensitive) == 0) {
        attr.usage = AttributeUsage_TexCoord;
        keywordLen = 8;
    }
    else if (
        StringHelper::compare(
            semanticName.c_str(),
            "TANGENT",
            7,
            ln::CaseSensitivity::CaseInsensitive) == 0) {
        attr.usage = AttributeUsage_Tangent;
        keywordLen = 7;
    }
    else if (
        StringHelper::compare(
            semanticName.c_str(),
            "BINORMAL",
            8,
            ln::CaseSensitivity::CaseInsensitive) == 0) {
        attr.usage = AttributeUsage_Binormal;
        keywordLen = 8;
    }
    else if (
        StringHelper::compare(
            semanticName.c_str(),
            "COLOR",
            5,
            ln::CaseSensitivity::CaseInsensitive) == 0) {
        attr.usage = AttributeUsage_Color;
        keywordLen = 5;
    }
    else if (
        StringHelper::compare(
            semanticName.c_str(),
            "SV_INSTANCEID",
            10,
            ln::CaseSensitivity::CaseInsensitive) == 0) {
        attr.usage = AttributeUsage_InstanceID;
        keywordLen = 10;
    }
    else {
        LN_NOTIMPLEMENTED();
    }

    attr.index = semanticIndex;
    attr.layoutLocation = locationIndex;
    return attr;
}

} // namespace kokage
} // namespace ln

#endif // LN_USE_SLANG
