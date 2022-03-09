
#include "../Core/CodeAnalyzer.hpp"
#include "../Core/SymbolDatabase.hpp"
#include "../Core/Generators/FlatCGenerator.hpp"
#include "../Core/Generators/RubyExtGenerator.hpp"
#include "../Core/Generators/HSP3Generator.hpp"
#include "../Core/Generators/DotNetPInvokeGenerator.hpp"
#include "../Core/Generators/DotnetClassGenerator.hpp"

//#define module_Engine->addInputFile(U
#define USE_DEBUG_PARSER 1


int main(int argc, char** argv) {
    ln::Logger::addStdErrAdapter();
    ln::Runtime::initialize();
    auto diag = ln::makeObject<ln::DiagnosticsManager>();
    diag->setOutputToStdErr(true);
    auto pidb = ln::makeRef<PIDatabase>();

    if (!ln::FileSystem::existsFile(U"pidb.json")) {

        auto project = makeURef<Project>();

        auto module_Core = makeURef<Module>(ln::String::fromCString(LUMINO_REPO_ROOT_DIR) + U"/lumino/LuminoCore");
        module_Core->addIncludeDirectory(U"include");
        module_Core->addForceIncludeFile(U"src/LuminoCore.PCH.h");
        module_Core->addInputFile(U"include/LuminoCore/Base/Logger.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Math/Vector3.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Math/Vector4.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Math/Quaternion.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Math/Matrix.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Text/Encoding.hpp");
        module_Core->addInputFile(U"include/LuminoCore/Geometries/GeometryStructs.hpp");
        project->modules.push(std::move(module_Core));

        auto module_Runtime = makeURef<Module>(ln::String::fromCString(LUMINO_REPO_ROOT_DIR) + U"/lumino/Runtime");
        module_Runtime->addIncludeDirectory(U"include");
        module_Runtime->addForceIncludeFile(U"src/pch.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Reflection/Object.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Base/Event.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Base/Promise.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Base/Variant.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Runtime/detail/BindingValidation.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Runtime/RuntimeLog.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Base/Collection.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Base/Serializer.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Asset/AssetObject.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Asset/AssetModel.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Asset/Assets.hpp");
        module_Runtime->addInputFile(U"include/LuminoEngine/Graphics/ColorStructs.hpp");
        project->modules.push(std::move(module_Runtime));

        auto module_Platform = makeURef<Module>(ln::String::fromCString(LUMINO_REPO_ROOT_DIR) + U"/lumino/Platform");
        module_Platform->addIncludeDirectory(U"include");
        module_Platform->addForceIncludeFile(U"src/pch.hpp");
        module_Platform->addInputFile(U"include/LuminoPlatform/PlatformEvent.hpp");
        project->modules.push(std::move(module_Platform));

        auto module_Graphics = makeURef<Module>(ln::String::fromCString(LUMINO_REPO_ROOT_DIR) + U"/lumino/Graphics");
        module_Graphics->addIncludeDirectory(U"include");
        module_Graphics->addForceIncludeFile(U"src/pch.hpp");
        module_Graphics->addInputFile(U"include/LuminoGraphics/Common.hpp");
        module_Graphics->addInputFile(U"include/LuminoGraphics/Graphics.hpp");
        module_Graphics->addInputFile(U"include/LuminoGraphics/GraphicsResource.hpp");
        module_Graphics->addInputFile(U"include/LuminoGraphics/Texture.hpp");
        module_Graphics->addInputFile(U"include/LuminoGraphics/Shader.hpp");
        project->modules.push(std::move(module_Graphics));

        auto module_Engine = makeURef<Module>(ln::String::fromCString(LUMINO_REPO_ROOT_DIR) + U"/lumino/LuminoEngine");
        module_Engine->addIncludeDirectory(U"include");
        module_Engine->addForceIncludeFile(U"src/LuminoEngine.PCH.h");
        module_Engine->addInputFile(U"include/LuminoEngine/Audio/Sound.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Audio/Audio.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Rendering/Common.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Rendering/RenderView.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Rendering/Material.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Mesh/MeshPrimitive.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Mesh/AnimationController.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Mesh/MeshModel.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Mesh/SkinnedMeshModel.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Physics/CollisionShape.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Animation/Common.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Animation/AnimationCurve.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Animation/AnimationClip.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Animation/AnimationMixer.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Effect/EffectModel.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Effect/ParticleEffectModel.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Effect/ParticleEffectModel2.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Component.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Visual/VisualComponent.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Visual/SpriteComponent.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Common.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/CharacterController.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/World.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/WorldObject.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/VisualObject.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Camera.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Light.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Sprite.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/CameraOrbitControlComponent.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Raycaster.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/WorldRenderView.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Shapes/MeshPrimitives.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Mesh/StaticMesh.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Mesh/MeshComponent.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Physics/Collision.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Physics/RigidBodyComponent.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Effect/ParticleEmitter.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Scene.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scene/Level.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/Common.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/UIColors.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/UIEvents.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/UILayoutElement.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/UIElement.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/UIText.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/UISprite.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/UIIcon.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/UIMessageTextArea.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/UI.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/Layout/UILayoutPanel.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/Layout/UIGridLayout.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/Controls/UIControl.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/Controls/UIButton.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/Controls/UIWindow.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/UI/Controls/UIListBox.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Input/InputBinding.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Input/Input.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Input/Mouse.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Scripting/Interpreter.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Engine/EngineSettings.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Engine/Engine.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Engine/Application.hpp");
        module_Engine->addInputFile(U"include/LuminoEngine/Engine/Debug.hpp");
        project->modules.push(std::move(module_Engine));


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
    config->flatCHeaderString = U"#include <LuminoEngine.hpp>\n#include \"BindingValidation.hpp\"";
    config->flatCHeaderOutputDirOverride = LN_LOCALFILE("../../../include/LuminoEngine/Runtime");
    config->flatCSourceOutputDirOverride = LN_LOCALFILE("../../../src/LuminoEngine/src/Runtime");
    config->versionString = U"0.10.0";

    {
        FlatCHeaderGenerator g;
        g.setup(db, config);
        g.generate();
    }
    //{
    //    FlatCSourceGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}
    //{
    //    RubyExtGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}
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
