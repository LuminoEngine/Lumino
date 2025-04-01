#include "Internal.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/ShaderCompiler.hpp>

#ifdef LN_USE_SLANG
// https://shader-slang.org/slang/user-guide/compiling#using-the-compilation-api
// https://github.com/shader-slang/slang/pull/6679
#pragma comment(lib, "C:/Proj/LN/Lumino/vcpkg/packages/shader-slang_x64-windows/lib/slang.lib")
//#pragma comment(lib, "E:/Proj/Lumino/vcpkg/packages/shader-slang_x64-windows/lib/slang.lib")
static slang::CompilerOptionValue fromInt3(uint8_t v0, int v1, int v2) {
    slang::CompilerOptionValue value;
    value.intValue0 = (v0 << 24) + (v1 & 0xFFFFFF);
    value.intValue1 = v2;
    value.kind = slang::CompilerOptionValueKind::Int;
    return value;
}
#endif

namespace ln {
namespace kokage {

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
    m_inputDirPath = inputFilePath.parent_path();

    if (m_dump) {
        m_dumpDirPath = inputFilePath;
        m_dumpDirPath += ".dump";
        fs::create_directories(m_dumpDirPath);
    }

    auto result = buildTarget(inputFilePath.string(), SLANG_SPIRV);
    if (!result) {
        return result;
    }
    return LN_MAKE_SUCCESS();
}

MaybeResult ShaderCompiler::buildTarget(const std::string& inputFilePath, SlangCompileTarget target) {
    //slang::CompilerOptionEntry options[] = { { slang::CompilerOptionName::VulkanBindShift, fromInt3(2, 2, 3) } };
    slang::TargetDesc targetDesc = {};
    targetDesc.format = target;
    //targetDesc.format = SLANG_SPIRV;
    //targetDesc.format = SLANG_GLSL;
    //targetDesc.format = SLANG_WGSL;
    //targetDesc.format = SLANG_METAL;
    //targetDesc.compilerOptionEntries = options;
    //targetDesc.compilerOptionEntryCount = 0;
    targetDesc.profile = m_globalSession->findProfile("glsl_450");

    slang::SessionDesc sessionDesc = {};
    /* ... fill in `sessionDesc` ... */
    sessionDesc.targets = &targetDesc;
    sessionDesc.targetCount = 1;

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

    Slang::ComPtr<SlangCompileRequest> compileRequest;
    result = session->createCompileRequest(compileRequest.writeRef());
    if (SLANG_FAILED(result)) {
        return LN_MAKE_ERROR("createCompileRequest failed. (%d)", result);
    }

    Slang::ComPtr<slang::IBlob> diagnostics;
    slang::IModule* module = session->loadModule(inputFilePath.c_str(), diagnostics.writeRef());
    if (diagnostics) {
        LN_LOG_INFO(reinterpret_cast<const char*>(diagnostics->getBufferPointer()));
    }
    if (!module) {
        return LN_MAKE_ERROR("ISession::loadModule failed.");
    }
    //slang::IModule* module =
    //    session->loadModule("C:/Proj/LN/Lumino/packages/LuminoEngine/shader/CopyScreen.slang", diagnostics.writeRef());
    //slang::IModule* module =
    //    session->loadModule("E:/Proj/Lumino/packages/LuminoEngine/shader/CopyScreen.slang", diagnostics.writeRef());
    //slang::ProgramLayout* layout3 = module->getLayout();
    //int pc = layout3->getEntryPointCount();
    //slang::EntryPointReflection* pp = layout3->getEntryPointByIndex(0);

    
    slang::IComponentType* components2[] = { module };
    Slang::ComPtr<slang::IComponentType> program2;
    result = session->createCompositeComponentType(components2, 1, program2.writeRef());

    //Slang::ComPtr<slang::IEntryPoint> entryPoint;
    //result = module->findAndCheckEntryPoint(
    //    "PS_Main", SlangStage::SLANG_STAGE_VERTEX, entryPoint.writeRef(), diagnostics.writeRef());

    //LN_LOG_INFO("  Name: {}", pp->getName());
    //LN_LOG_INFO("  Stage: {}", (int)pp->getStage());

    
    Slang::ComPtr<slang::IComponentType> linkedProgram2;
    Slang::ComPtr<ISlangBlob> diagnosticBlob2;
    result = module->link(linkedProgram2.writeRef(), diagnosticBlob2.writeRef());

    //slang::ProgramLayout* layout3 = linkedProgram2->getLayout();
    //int ec = layout3->getEntryPointCount();
    //layout3->toJson(diagnosticBlob2.writeRef());
    //std::string code2((const char*)diagnosticBlob2->getBufferPointer(), diagnosticBlob2->getBufferSize());
    
    // [shader("vertex")] などで指定されたエントリポイントを取得するために、一度すべてのエントリポイントを含めてリンクする必要がある。
    std::vector<Slang::ComPtr<slang::IEntryPoint>> componentsRefs;
    std::vector<slang::IComponentType*> components = { module };
    for (int i = 0; i < module->getDefinedEntryPointCount(); i++) {
        Slang::ComPtr<slang::IEntryPoint> entryPoint;
        module->getDefinedEntryPoint(i, entryPoint.writeRef());
        componentsRefs.push_back(entryPoint);
        components.push_back(entryPoint);
        //break;
    }

    Slang::ComPtr<slang::IComponentType> program;
    result = session->createCompositeComponentType(components.data(), components.size(), program.writeRef());
    if (SLANG_FAILED(result)) {
        return LN_MAKE_ERROR("createCompositeComponentType failed. (%d)", result);
    }


    

    Slang::ComPtr<slang::IComponentType> linkedProgram;
    Slang::ComPtr<ISlangBlob> diagnosticBlob;
    result = program->link(linkedProgram.writeRef(), diagnosticBlob.writeRef());



    slang::ProgramLayout* layout = linkedProgram->getLayout();

    for (int i = 0; i < layout->getEntryPointCount(); i++) {
        slang::EntryPointLayout* entryPointLayout = layout->getEntryPointByIndex(i);
        //slang::EntryPointReflection* entryPointReflection = entryPointLayout->getEntryPoint();
        //slang::FunctionReflection* functionReflection = entryPointReflection->getFunctionReflection();
        //LN_LOG_INFO("  Name: {}", functionReflection->getName());
        //int att = functionReflection->getUserAttributeCount();
        //for (int j = 0; j < att; j++) {
        //    slang::UserAttribute* ua = functionReflection->getUserAttributeByIndex(j);
        //    LN_LOG_INFO("    UserAttribute: {}", ua->getName());
        //}
    }



    layout->toJson(diagnosticBlob2.writeRef());
    //SlangReflection* reflection = spGetReflection(compileRequest);
    //spReflection_ToJson((SlangReflection*)layout, nullptr, diagnosticBlob2.writeRef());
    std::string code2((const char*)diagnosticBlob2->getBufferPointer(), diagnosticBlob2->getBufferSize());
        

    //Slang::ComPtr<slang::IEntryPoint> computeEntryPoint;
    //result = module->findEntryPointByName("computeMain", computeEntryPoint.writeRef());
    //result = module->findEntryPointByName("PS_Main", computeEntryPoint.writeRef());

    //slang::IComponentType* components[] = { module, computeEntryPoint };
    //Slang::ComPtr<slang::IComponentType> program;
    //result = session->createCompositeComponentType(components, 1, program.writeRef());


    slang::ProgramLayout* layout2 = linkedProgram->getLayout();
    layout->toJson(diagnosticBlob2.writeRef());
    std::string code3((const char*)diagnosticBlob2->getBufferPointer(), diagnosticBlob2->getBufferSize());

    slang::IMetadata* entryPointMetadata;
    int entryPointIndex = 0; // only one entry point
    int targetIndex = 0;     // only one target

    linkedProgram->getEntryPointMetadata(
        entryPointIndex,
        targetIndex, // target index
        &entryPointMetadata);

    // NOTE: slangc -reflection-json では EntryPoint ごとに used な Parameter が出力されるが、
    //   それはレガシーな API を使っているようで、 公開 API からは取得することができなかった。
    //   (toJson() 内部の spReflection_ToJson() の第２引数に Request を指定する必要があるが、それだけだとクラッシュした)
    //   なので同様の JSON は出力できない点に注意。
    {
        slang::EntryPointReflection* entryPoint = layout2->getEntryPointByIndex(entryPointIndex);
        printf("%s\n", entryPoint->getName());
        int programParameterCount = layout2->getParameterCount();
        for (int i = 0; i < programParameterCount; i++) {
            slang::VariableLayoutReflection* parameter = layout2->getParameterByIndex(i);
            int categoryCount = parameter->getCategoryCount();
            for (int iCategory = 0; iCategory < categoryCount; iCategory++) {
                SlangParameterCategory category = static_cast<SlangParameterCategory>(parameter->getCategoryByIndex(iCategory));
                auto space = parameter->getBindingSpace(category);
                auto index = parameter->getOffset(category);

                // NOTE: uniform ($global) だと常に false になる。
                bool used = false;
                bool usedAvailable = entryPointMetadata->isParameterLocationUsed(
                                         category, space, index, used) == SLANG_OK;

                //slang::ShaderBindingRange::isUsageTracked(
                printf("  %s: %d %d\n", parameter->getName(), used, usedAvailable);
            }
        
        }
    }


    Slang::ComPtr<slang::IBlob> kernelBlob;
    result =
        linkedProgram->getEntryPointCode(entryPointIndex, targetIndex, kernelBlob.writeRef(), diagnostics.writeRef());
    if (diagnostics) {
        fprintf(stderr, "%s\n", (const char*)diagnostics->getBufferPointer());
    }

    //ln::FileSystem::writeAllBytes(
    //    //U"C:/Proj/LN/Lumino/packages/LuminoEngine/shader/CopyScreen.spv",
    //    U"E:/Proj/Lumino/packages/LuminoEngine/shader/CopyScreen.spv",
    //    (const void*)kernelBlob->getBufferPointer(),
    //    kernelBlob->getBufferSize());
    //std::string code((const char*)kernelBlob->getBufferPointer(), kernelBlob->getBufferSize());

    

    return LN_MAKE_SUCCESS();
}

MaybeResult ShaderCompiler::buildSingle(
    slang::IComponentType* linkedProgram,
    int entryPointIndex,
    std::vector<uint8_t>* code) {
    return LN_MAKE_SUCCESS();
}

} // namespace kokage
} // namespace ln
