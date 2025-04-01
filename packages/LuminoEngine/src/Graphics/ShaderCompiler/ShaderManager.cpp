#include "Internal.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/detail/ShaderManager.hpp>
#include "ShaderTranspiler.hpp"
#include <LuminoEngine/Asset/detail/AssetManager.hpp>

#ifdef _WIN32
#include <LuminoEngine/Graphics/ShaderCompiler/detail/D3DCompilerAPI.hpp>
#endif

#ifdef LN_USE_SLANG
// https://shader-slang.org/slang/user-guide/compiling#using-the-compilation-api
// https://github.com/shader-slang/slang/pull/6679
#include "../../../../../vcpkg/packages/shader-slang_x64-windows/include/slang.h"
#include "../../../../../vcpkg/packages/shader-slang_x64-windows/include/slang-com-ptr.h"
//#pragma comment(lib, "C:/Proj/LN/Lumino/vcpkg/packages/shader-slang_x64-windows/lib/slang.lib")
#pragma comment(lib, "E:/Proj/Lumino/vcpkg/packages/shader-slang_x64-windows/lib/slang.lib")
static slang::CompilerOptionValue fromInt3(uint8_t v0, int v1, int v2) {
    slang::CompilerOptionValue value;
    value.intValue0 = (v0 << 24) + (v1 & 0xFFFFFF);
    value.intValue1 = v2;
    value.kind = slang::CompilerOptionValueKind::Int;
    return value;
}
#endif

namespace ln {
namespace detail {

//==============================================================================
// ShaderManager


ShaderManager* ShaderManager::initialize(const Settings& settings)
{
    if (instance()) return instance();

    auto m = Ref<ShaderManager>(LN_NEW detail::ShaderManager(), false);
    if (!m->init(settings)) return nullptr;

    EngineInstance::instance()->registerModule(m);
    EngineInstance::instance()->shaderManager = m;

#ifdef LN_USE_SLANG
    Slang::ComPtr<slang::IGlobalSession> globalSession;
    SlangGlobalSessionDesc desc = {};
    SlangResult result = slang::createGlobalSession(&desc, globalSession.writeRef());
    // SLANG_SUCCEEDED

    slang::CompilerOptionEntry options[] = { { slang::CompilerOptionName::VulkanBindShift, fromInt3(2, 2, 3) } };
    slang::TargetDesc targetDesc = {};
    //targetDesc.format = SLANG_SPIRV;
    //targetDesc.format = SLANG_GLSL;
    targetDesc.format = SLANG_WGSL;
    //targetDesc.format = SLANG_METAL;
    targetDesc.compilerOptionEntries = options;
    targetDesc.compilerOptionEntryCount = 0;
    targetDesc.profile = globalSession->findProfile("glsl_450");

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
    result = globalSession->createSession(sessionDesc, session.writeRef());

    Slang::ComPtr<slang::IBlob> diagnostics;
    //slang::IModule* module =
    //    session->loadModule("C:/Proj/LN/Lumino/packages/LuminoEngine/MyShaders.slang", diagnostics.writeRef());
    //slang::IModule* module =
    //    session->loadModule("C:/Proj/LN/Lumino/packages/LuminoEngine/shader/CopyScreen.slang", diagnostics.writeRef());
    slang::IModule* module =
        session->loadModule("E:/Proj/Lumino/packages/LuminoEngine/shader/CopyScreen.slang", diagnostics.writeRef());

    if (diagnostics) {
        fprintf(stderr, "%s\n", (const char*)diagnostics->getBufferPointer());
    }

    Slang::ComPtr<slang::IEntryPoint> computeEntryPoint;
    //result = module->findEntryPointByName("computeMain", computeEntryPoint.writeRef());
    result = module->findEntryPointByName("PS_Main", computeEntryPoint.writeRef());

    slang::IComponentType* components[] = { module, computeEntryPoint };
    Slang::ComPtr<slang::IComponentType> program;
    result = session->createCompositeComponentType(components, 2, program.writeRef());

    slang::ProgramLayout* layout = program->getLayout();

    Slang::ComPtr<slang::IComponentType> linkedProgram;
    Slang::ComPtr<ISlangBlob> diagnosticBlob;
    result = program->link(linkedProgram.writeRef(), diagnosticBlob.writeRef());

    slang::ProgramLayout* layout2 = linkedProgram->getLayout();

    int entryPointIndex = 0; // only one entry point
    int targetIndex = 0;     // only one target
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
    std::string code((const char*)kernelBlob->getBufferPointer(), kernelBlob->getBufferSize());

#endif

    return m;
}

void ShaderManager::terminate()
{
    if (instance()) {
        instance()->dispose();
        EngineInstance::instance()->unregisterModule(instance());
        EngineInstance::instance()->shaderManager = nullptr;
    }
}

ShaderManager::ShaderManager()
    : m_builtinShaderList()
#ifdef _WIN32
    , m_hD3DCompilerDLL(nullptr)
#endif
{
}

ShaderManager::~ShaderManager()
{
}

bool ShaderManager::init(const Settings& settings)
{
    LN_LOG_DEBUG("ShaderManager Initialization started.");

    // Lumino.fxh.h
    {
        static const unsigned char data[] =
            {
#include "Resource/Lumino.fxh.inl"
            };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({"Lumino.fxh", std::string((const char*)data, size)});
    }

    // LuminoForward.fxh.h
    {
        static const unsigned char data[] =
            {
#include "Resource/LuminoForward.fxh.inl"
            };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({"LuminoForward.fxh", std::string((const char*)data, size)});
    }

    // LuminoPBR.fxh.h
    {
        static const unsigned char data[] =
            {
#include "Resource/LuminoPBR.fxh.inl"
            };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({"LuminoPBR.fxh", std::string((const char*)data, size)});
    }

    // LuminoShadow.fxh.h
    {
        static const unsigned char data[] =
            {
#include "Resource/LuminoShadow.fxh.inl"
            };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({"LuminoShadow.fxh", std::string((const char*)data, size)});
    }

    // LuminoSkinning.fxh.h
    {
        static const unsigned char data[] =
            {
#include "Resource/LuminoSkinning.fxh.inl"
            };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({"LuminoSkinning.fxh", std::string((const char*)data, size)});
    }

    // LuminoNormalMap.fxh.h
    {
        static const unsigned char data[] =
        {
#include "Resource/LuminoNormalMap.fxh.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({ "LuminoNormalMap.fxh", std::string((const char*)data, size) });
    }

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    kokage::ShaderCodeTranspiler::initializeGlobals();
#endif

#ifdef _WIN32
    m_hD3DCompilerDLL = ::LoadLibrary(D3DCOMPILER_DLL_W);
    if (m_hD3DCompilerDLL) {
        D3DCompile2 = reinterpret_cast<PFN_D3DCompile2>(::GetProcAddress(m_hD3DCompilerDLL, "D3DCompile2"));
    }
    else {
        LN_LOG_ERROR("{} not found.", D3DCOMPILER_DLL_A);
    }
    D3DCompilerAPI::Initialize();
#endif

    LN_LOG_DEBUG("ShaderManager Initialization ended.");
    return true;
}

void ShaderManager::dispose()
{
#ifdef _WIN32
    if (m_hD3DCompilerDLL) {
        ::FreeLibrary(m_hD3DCompilerDLL);
        m_hD3DCompilerDLL = nullptr;
    }
    D3DCompile2 = nullptr;
#endif

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    kokage::ShaderCodeTranspiler::finalizeGlobals();
#endif

}

} // namespace detail
} // namespace ln
