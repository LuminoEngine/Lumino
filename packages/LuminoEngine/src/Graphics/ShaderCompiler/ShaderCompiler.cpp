#include <fstream>
#include "Internal.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShader2.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/ShaderCompiler.hpp>

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
    { ShaderTarget_SPIRV, "sm_6_0" },
    { ShaderTarget_DXBC, "sm_6_0" },
    { ShaderTarget_WGSL, "sm_6_0" },
    { ShaderTarget_METAL, "sm_6_0" },
};
static const int kTargetCount = SLANG_COUNT_OF(kTargets);

static SlangCompileTarget toSlangTarget(ShaderTarget target) {
    switch (target) {
        case ShaderTarget::ShaderTarget_SPIRV:
            return SLANG_SPIRV;
        case ShaderTarget::ShaderTarget_DXBC:
            return SLANG_DXBC;
        case ShaderTarget::ShaderTarget_WGSL:
            return SLANG_WGSL;
        case ShaderTarget::ShaderTarget_METAL:
            return SLANG_METAL;
        default:
            LN_UNREACHABLE();
            return SLANG_SPIRV;
    }
}

static const char* getTargetName(ShaderTarget target) {
    switch (target) {
        case ShaderTarget::ShaderTarget_SPIRV:
            return "SPIRV";
        case ShaderTarget::ShaderTarget_DXBC:
            return "DXBC";
        case ShaderTarget::ShaderTarget_WGSL:
            return "WGSL";
        case ShaderTarget::ShaderTarget_METAL:
            return "METAL";
        default:
            LN_UNREACHABLE();
            return "?";
    }
}

Result<URef<ShaderCompiler>> ShaderCompiler::create() {
    URef<ShaderCompiler> ref(LN_NEW ShaderCompiler());
    auto result = ref->init();
    if (!result) {
        return result;
    }
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
    m_shader = makeURef<UnifiedShader2>();

    if (m_dump) {
        m_dumpDirPath = inputFilePath;
        m_dumpDirPath += ".dump";
        fs::create_directories(m_dumpDirPath);
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

        //auto result = buildTarget(inputFilePath.string(), target.target, target.profile);
        //if (!result) {
        //    return result;
        //}
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
    Slang::ComPtr<slang::IComponentType> program;
    Slang::ComPtr<slang::IBlob> linkDiag;
    result = composed->link(program.writeRef(), linkDiag.writeRef());
    if (linkDiag) {
        std::string message(static_cast<const char*>(linkDiag->getBufferPointer()), linkDiag->getBufferSize());
        LN_LOG_INFO(message);
    }
    if (SLANG_FAILED(result)) {
        return LN_MAKE_ERROR("link failed. (%d)", result);
    }

    for (int i = 0; i < kTargetCount; i++) {
        auto& target = kTargets[i];
        auto result = buildTarget(program, target.target, i);
        if (!result) {
            return result;
        }
    }

    return LN_MAKE_SUCCESS();
}

MaybeResult ShaderCompiler::buildTarget(slang::IComponentType* program, ShaderTarget target, int targetIndex) {
    slang::ProgramLayout* layout = program->getLayout();

    
    // Dump module reflection
    // NOTE: この Dump は moduel 全部 Composite した状態で行った方が良いだろう。
    //       そうしないと、EntryPoint の情報が出力されない。
    if (m_dump) {
        fs::path filePath = m_dumpDirPath / (std::string(getTargetName(target)) + ".reflection.json");
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

    // Build module reflection
    ModuleInfo* moduleInfo = m_shader->addModuleInfo();
    switch (target) {
        case ShaderTarget_SPIRV: {
            auto r = buildTargetInfoSPIRV(layout, moduleInfo);
            if (!r) return r;
            break;
        }
        case ShaderTarget_DXBC:
            break;
        default:
            break;
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
    

    //Slang::ComPtr<slang::IEntryPoint> entryPoint;
    //result = module->findAndCheckEntryPoint(
    //    "PS_Main", SlangStage::SLANG_STAGE_VERTEX, entryPoint.writeRef(), diagnostics.writeRef());

    //LN_LOG_INFO("  Name: {}", pp->getName());
    //LN_LOG_INFO("  Stage: {}", (int)pp->getStage());

    
    //Slang::ComPtr<slang::IComponentType> linkedProgram2;
    //Slang::ComPtr<ISlangBlob> diagnosticBlob2;
    //result = program->link(linkedProgram2.writeRef(), diagnosticBlob2.writeRef());

    //slang::ProgramLayout* layout3 = linkedProgram2->getLayout();
    //int ec = layout3->getEntryPointCount();
    //layout3->toJson(diagnosticBlob2.writeRef());
    //std::string code2((const char*)diagnosticBlob2->getBufferPointer(), diagnosticBlob2->getBufferSize());
    


    
    int entryPointCount = m_module->getDefinedEntryPointCount();
    for (int iEntryPoint = 0; iEntryPoint < entryPointCount; iEntryPoint++) {
        auto r = buildEntryPoint(program, targetIndex, iEntryPoint);
        if (!r) return r;


    
    }


    //slang::ProgramLayout* layout = linkedProgram->getLayout();

    //int entryPointCount = m_module->getDefinedEntryPointCount();
    //for (int i = 0; i < entryPointCount; i++) {
    //    slang::EntryPointLayout* entryPointLayout = layout->getEntryPointByIndex(i);
    //    //slang::EntryPointReflection* entryPointReflection = entryPointLayout->getEntryPoint();
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

MaybeResult ShaderCompiler::buildEntryPoint(slang::IComponentType* program, int targetIndex, int entryPointIndex) {
    slang::ProgramLayout* layout = program->getLayout();

    Slang::ComPtr<slang::IMetadata> entryPointMetadata;
    SlangResult result = program->getEntryPointMetadata(
        entryPointIndex,
        targetIndex, // target index
        entryPointMetadata.writeRef());
    if (SLANG_FAILED(result)) {
        return LN_MAKE_ERROR("getEntryPointMetadata failed. (%d)", result);
    }

    // NOTE: slangc -reflection-json では EntryPoint ごとに used な Parameter が出力されるが、
    //   それはレガシーな API を使っているようで、 公開 API からは取得することができなかった。
    //   (toJson() 内部の spReflection_ToJson() の第２引数に Request を指定する必要があるが、それだけだとクラッシュした)
    //   なので同様の JSON は出力できない点に注意。
    {
        slang::EntryPointReflection* entryPoint = layout->getEntryPointByIndex(entryPointIndex);
        printf("%s\n", entryPoint->getName());
        int programParameterCount = layout->getParameterCount();
        for (int i = 0; i < programParameterCount; i++) {
            slang::VariableLayoutReflection* parameter = layout->getParameterByIndex(i);
            int categoryCount = parameter->getCategoryCount();
            for (int iCategory = 0; iCategory < categoryCount; iCategory++) {
                SlangParameterCategory category =
                    static_cast<SlangParameterCategory>(parameter->getCategoryByIndex(iCategory));
                auto space = parameter->getBindingSpace(category);
                auto index = parameter->getOffset(category);

                // NOTE: uniform ($global) だと常に false になる。
                bool used = false;
                bool usedAvailable =
                    entryPointMetadata->isParameterLocationUsed(category, space, index, used) == SLANG_OK;

                //slang::ShaderBindingRange::isUsageTracked(
                printf("  %s: %d %d\n", parameter->getName(), index, space, used, usedAvailable);
            }
        }
    }

    // Generate code.
    Slang::ComPtr<slang::IBlob> kernelBlob;
    Slang::ComPtr<slang::IBlob> generateDiag;
    result = program->getEntryPointCode(entryPointIndex, targetIndex, kernelBlob.writeRef(), generateDiag.writeRef());
    if (generateDiag) {
        std::string message(static_cast<const char*>(generateDiag->getBufferPointer()), generateDiag->getBufferSize());
        LN_LOG_INFO(message);
    }
    if (SLANG_FAILED(result)) {
        return LN_MAKE_ERROR("link failed. (%d)", result);
    }

    //ln::FileSystem::writeAllBytes(
    //    //U"C:/Proj/LN/Lumino/packages/LuminoEngine/shader/CopyScreen.spv",
    //    U"E:/Proj/Lumino/packages/LuminoEngine/shader/CopyScreen.spv",
    //    (const void*)kernelBlob->getBufferPointer(),
    //    kernelBlob->getBufferSize());
    //std::string code((const char*)kernelBlob->getBufferPointer(), kernelBlob->getBufferSize());


    return LN_MAKE_SUCCESS();
}

MaybeResult ShaderCompiler::buildTargetInfoSPIRV(slang::ProgramLayout* layout, ModuleInfo* moduleInfo) {
    moduleInfo->target = ShaderTarget_SPIRV;

    int parameterCount = layout->getParameterCount();
    for (int i = 0; i < parameterCount; i++) {
        slang::VariableLayoutReflection* parameter = layout->getParameterByIndex(i);
        int categoryCount = parameter->getCategoryCount();
        if (categoryCount > 1) {
            return LN_MAKE_ERROR("Multiple category unsupported. (%d)", categoryCount);
        }
    
        ModuleParameterInfo info;
        info.name = parameter->getName();
        slang::ParameterCategory category = parameter->getCategory();
        switch (category) {
            case slang::ParameterCategory::DescriptorTableSlot:
                info.category = DescriptorType_UniformBuffer;
                break;
            case slang::ParameterCategory::ShaderResource:
                info.category = DescriptorType_Texture;
                break;
            case slang::ParameterCategory::SamplerState:
                info.category = DescriptorType_SamplerState;
                break;
            case slang::ParameterCategory::UnorderedAccess:
                info.category = DescriptorType_UnorderdAccess;
                break;
            case slang::ParameterCategory::Uniform: // GlobalUniform
                continue;
            default:
                return LN_MAKE_ERROR("Invalid category. (%d)", category);
        }

        moduleInfo->parameters.push_back(info);
    }

    int b = layout->getGlobalConstantBufferSize();
    int c = layout->getTypeParameterCount();

    return LN_MAKE_SUCCESS();

}

} // namespace kokage
} // namespace ln

#endif // LN_USE_SLANG
