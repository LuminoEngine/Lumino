
#include "../Core/CodeAnalyzer.hpp"
#include "../Core/SymbolDatabase.hpp"
#include "../Core/Generators/FlatCGenerator.hpp"
#include "../Core/Generators/RubyExtGenerator.hpp"
#include "../Core/Generators/HSP3Generator.hpp"
#include "../Core/Generators/DotNetPInvokeGenerator.hpp"
#include "../Core/Generators/DotnetClassGenerator.hpp"

//#define USE_DEBUG_PARSER 1

int main(int argc, char** argv) {
    ln::Logger::addStdErrAdapter();
    ln::Runtime::initialize();
    auto diag = ln::makeObject<ln::DiagnosticsManager>();
    diag->setOutputToStdErr(true);
    auto pidb = ln::makeRef<PIDatabase>();
    const auto repoRoot = ln::String::fromCString(LUMINO_REPO_ROOT_DIR);

    if (!ln::FileSystem::existsFile(U"pidb.json")) {
        const auto include_LuminoCore = ln::Path(repoRoot, U"lumino/LuminoCore/include");
        const auto include_Runtime = ln::Path(repoRoot, U"lumino/Runtime/include");
        const auto include_Platform = ln::Path(repoRoot, U"lumino/Platform/include");
        const auto include_Graphics = ln::Path(repoRoot, U"lumino/Graphics/include");
        const auto include_ShaderCompiler = ln::Path(repoRoot, U"lumino/ShaderCompiler/include");
        const auto include_vcpkg = ln::Path(repoRoot, U"build/tools/vcpkg/installed/x64-windows/include");

        auto project = makeURef<Project>();

        auto module_Core = makeURef<Module>(ln::String::fromCString(LUMINO_REPO_ROOT_DIR) + U"/lumino/LuminoCore");
        module_Core->addIncludeDirectory(U"include");
        module_Core->addIncludeDirectory(include_vcpkg);
        module_Core->addForceIncludeFile(U"src/LuminoCore.PCH.h");
        module_Core->addInputFile(U"include/LuminoCore/Base/Logger.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Math/Vector3.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Math/Vector4.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Math/Quaternion.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Math/Matrix.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Text/Encoding.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Geometries/GeometryStructs.hpp");
        project->modules.push(std::move(module_Core));

        // Runtime
        {
            auto module = makeURef<Module>(ln::String::fromCString(LUMINO_REPO_ROOT_DIR) + U"/lumino/Runtime");
            module->addIncludeDirectory(U"include");
            module->addIncludeDirectory(include_vcpkg);
            module->addIncludeDirectory(include_LuminoCore);
            module->addForceIncludeFile(U"src/pch.hpp");
            module->addInputFile(U"include/LuminoEngine/Reflection/Object.hpp");
            module->addInputFile(U"include/LuminoEngine/Base/Event.hpp");
            module->addInputFile(U"include/LuminoEngine/Base/Promise.hpp");
            module->addInputFile(U"include/LuminoEngine/Base/Variant.hpp");
            module->addInputFile(U"include/LuminoEngine/Runtime/detail/BindingValidation.hpp");
            module->addInputFile(U"include/LuminoEngine/Runtime/RuntimeLog.hpp");
            module->addInputFile(U"include/LuminoEngine/Base/Collection.hpp");
            module->addInputFile(U"include/LuminoEngine/Base/Serializer.hpp");
            module->addInputFile(U"include/LuminoEngine/Asset/AssetObject.hpp");
            module->addInputFile(U"include/LuminoEngine/Asset/AssetModel.hpp");
            module->addInputFile(U"include/LuminoEngine/Asset/Assets.hpp");
            module->addInputFile(U"include/LuminoEngine/Graphics/ColorStructs.hpp");
            project->modules.push(std::move(module));
        }

        // Platform
        {
            auto module = makeURef<Module>(ln::String::fromCString(LUMINO_REPO_ROOT_DIR) + U"/lumino/Platform");
            module->addIncludeDirectory(U"include");
            module->addIncludeDirectory(include_vcpkg);
            module->addIncludeDirectory(include_LuminoCore);
            module->addIncludeDirectory(include_Runtime);
            module->addForceIncludeFile(U"src/pch.hpp");
            module->addInputFile(U"include/LuminoPlatform/PlatformEvent.hpp");
            project->modules.push(std::move(module));
        }

        // Graphics
        {
            auto module = makeURef<Module>(ln::String::fromCString(LUMINO_REPO_ROOT_DIR) + U"/lumino/Graphics");
            module->addIncludeDirectory(U"include");
            module->addIncludeDirectory(include_vcpkg);
            module->addIncludeDirectory(include_LuminoCore);
            module->addIncludeDirectory(include_Runtime);
            module->addIncludeDirectory(include_Platform);
            module->addIncludeDirectory(include_ShaderCompiler);
            module->addForceIncludeFile(U"src/pch.hpp");
            module->addInputFile(U"include/LuminoGraphics/Common.hpp");
            module->addInputFile(U"include/LuminoGraphics/Graphics.hpp");
            module->addInputFile(U"include/LuminoGraphics/GraphicsResource.hpp");
            module->addInputFile(U"include/LuminoGraphics/Texture.hpp");
            module->addInputFile(U"include/LuminoGraphics/Shader.hpp");
            project->modules.push(std::move(module));
        }

        // LuminoEngine
        {
            auto module = makeURef<Module>(ln::String::fromCString(LUMINO_REPO_ROOT_DIR) + U"/lumino/LuminoEngine");
            module->addIncludeDirectory(U"include");
            module->addIncludeDirectory(include_vcpkg);
            module->addIncludeDirectory(include_LuminoCore);
            module->addIncludeDirectory(include_Runtime);
            module->addIncludeDirectory(include_Platform);
            module->addIncludeDirectory(include_ShaderCompiler);
            module->addIncludeDirectory(include_Graphics);
            module->addForceIncludeFile(U"src/LuminoEngine.PCH.h");
            module->addInputFile(U"include/LuminoEngine/Audio/Sound.hpp");
            module->addInputFile(U"include/LuminoEngine/Audio/Audio.hpp");
            module->addInputFile(U"include/LuminoEngine/Rendering/Common.hpp");
            module->addInputFile(U"include/LuminoEngine/Rendering/RenderView.hpp");
            module->addInputFile(U"include/LuminoEngine/Rendering/Material.hpp");
            module->addInputFile(U"include/LuminoEngine/Mesh/MeshPrimitive.hpp");
            module->addInputFile(U"include/LuminoEngine/Mesh/AnimationController.hpp");
            module->addInputFile(U"include/LuminoEngine/Mesh/MeshModel.hpp");
            module->addInputFile(U"include/LuminoEngine/Mesh/SkinnedMeshModel.hpp");
            module->addInputFile(U"include/LuminoEngine/Physics/CollisionShape.hpp");
            module->addInputFile(U"include/LuminoEngine/Animation/Common.hpp");
            module->addInputFile(U"include/LuminoEngine/Animation/AnimationCurve.hpp");
            module->addInputFile(U"include/LuminoEngine/Animation/AnimationClip.hpp");
            module->addInputFile(U"include/LuminoEngine/Animation/AnimationMixer.hpp");
            module->addInputFile(U"include/LuminoEngine/Effect/EffectModel.hpp");
            module->addInputFile(U"include/LuminoEngine/Effect/ParticleEffectModel.hpp");
            module->addInputFile(U"include/LuminoEngine/Effect/ParticleEffectModel2.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Component.hpp");
            module->addInputFile(U"include/LuminoEngine/Visual/VisualComponent.hpp");
            module->addInputFile(U"include/LuminoEngine/Visual/SpriteComponent.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Common.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/CharacterController.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/World.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/WorldObject.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/VisualObject.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Camera.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Light.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Sprite.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/CameraOrbitControlComponent.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Raycaster.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/WorldRenderView.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Shapes/MeshPrimitives.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Mesh/StaticMesh.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Mesh/MeshComponent.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Physics/Collision.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Physics/RigidBodyComponent.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Effect/ParticleEmitter.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Scene.hpp");
            module->addInputFile(U"include/LuminoEngine/Scene/Level.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/Common.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/UIColors.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/UIEvents.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/UILayoutElement.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/UIElement.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/UIText.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/UISprite.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/UIIcon.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/UIMessageTextArea.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/UI.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/Layout/UILayoutPanel.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/Layout/UIGridLayout.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/Controls/UIControl.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/Controls/UIButton.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/Controls/UIWindow.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/Controls/UIListBox.hpp");
            module->addInputFile(U"include/LuminoEngine/UI/Controls/UIListBoxItem.hpp");
            module->addInputFile(U"include/LuminoEngine/Input/InputBinding.hpp");
            module->addInputFile(U"include/LuminoEngine/Input/Input.hpp");
            module->addInputFile(U"include/LuminoEngine/Input/Mouse.hpp");
            module->addInputFile(U"include/LuminoEngine/Scripting/Interpreter.hpp");
            module->addInputFile(U"include/LuminoEngine/Engine/EngineSettings.hpp");
            module->addInputFile(U"include/LuminoEngine/Engine/Engine.hpp");
            module->addInputFile(U"include/LuminoEngine/Engine/Application.hpp");
            module->addInputFile(U"include/LuminoEngine/Engine/Debug.hpp");
            project->modules.push(std::move(module));
        }


        CodeAnalyzer ca;
        ca.makeInputs(project.get());
#ifdef USE_DEBUG_PARSER
        ca.parserExecutable = ln::Path(ln::Path(ln::Environment::executablePath()).parent(), U"../../Parser/Debug/LuminoTranscoder-Parser.exe").canonicalize();
#else
        ca.parserExecutable = ln::Path(ln::Path(ln::Environment::executablePath()).parent(), U"../../Parser/Release/LuminoTranscoder-Parser.exe").canonicalize();
#endif

        if (!ca.analyze(pidb, diag)) {
            return 1;
        }

        pidb->save(U"pidb.json");
        return 0;
    }
    else {
        pidb->load(U"pidb.json");
    }

    auto db = ln::makeRef<SymbolDatabase>(diag);
    db->initTypes(pidb);
    db->linkTypes();

    diag->dumpToLog();
    if (diag->hasError()) {
        return 1;
    }

    auto config = ln::makeRef<GeneratorConfiguration>();
    config->moduleName = U"Lumino";
    config->outputDir = LN_LOCALFILE("../../../tools/Bindings");
    config->templateDir = LN_LOCALFILE("../Core/Generators/Templates");
    config->targetNamespace = U"ln";
    config->flatCOutputModuleName = U"LN";
    config->flatCCommonHeader = LN_LOCALFILE("../../../include/LuminoEngine/Runtime/FlatCommon.h");
    config->flatCHeaderString = U"#include <LuminoEngine.hpp>\n#include <LuminoEngine/Runtime/detail/BindingValidation.hpp>";
    //config->flatCHeaderOutputDirOverride = LN_LOCALFILE("../../../include/LuminoEngine/Runtime");
    //config->flatCSourceOutputDirOverride = LN_LOCALFILE("../../../src/LuminoEngine/src/Runtime");
    config->versionString = U"0.10.0";
    config->flatC.outputHeaderFile = ln::Path(repoRoot, U"lumino/FFI/include/LuminoFFI/Lumino.FlatC.generated.h");
    config->flatC.outputSourceFile = ln::Path(repoRoot, U"lumino/FFI/src/Lumino.FlatC.generated.cpp");

    //{
    //    FlatCHeaderGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}
    //{
    //    FlatCSourceGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}
    {
        RubyExtGenerator g;
        g.setup(db, config);
        g.generate();
    }
    //{
    //    RubyYARDOCSourceGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}
    //{
    //    HSP3HeaderGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}
    //{
    //    HSP3CommandsGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}
    //{
    //    HSP3HelpGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}

    //{
    //    DotNetPInvokeGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}
    //{
    //    DotnetClassGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}

    return 0;
}
