
#include "../Core/CodeAnalyzer.hpp"
#include "../Core/SymbolDatabase.hpp"
#include "../Core/Generators/FlatCGenerator.hpp"
#include "../Core/Generators/RubyExtGenerator.hpp"
#include "../Core/Generators/HSP3Generator.hpp"
#include "../Core/Generators/DotNetPInvokeGenerator.hpp"
#include "../Core/Generators/DotnetClassGenerator.hpp"

#define TEST_ROOT U"C:/Proj/LN/Lumino/"

int main(int argc, char** argv) {
    ln::Logger::addStdErrAdapter();
    auto diag = ln::makeObject<ln::DiagnosticsManager>();
    auto pidb = ln::makeRef<PIDatabase>();

    if (!ln::FileSystem::existsFile(U"pidb.json")) {

        ln::List<ln::Path> files_LuminoCore = {
            TEST_ROOT "include/LuminoCore/Base/Logger.hpp",
            //TEST_ROOT "include/LuminoCore/Math/Vector2.hpp",
            TEST_ROOT "include/LuminoCore/Math/Vector3.hpp",
            TEST_ROOT "include/LuminoCore/Math/Vector4.hpp",
            TEST_ROOT "include/LuminoCore/Math/Quaternion.hpp",
            TEST_ROOT "include/LuminoCore/Math/Matrix.hpp",
            TEST_ROOT "include/LuminoCore/Text/Encoding.hpp",
        };

        ln::List<ln::Path> files_LuminoEngine = {
            TEST_ROOT "include/LuminoEngine/Engine/Object.hpp",
            TEST_ROOT "include/LuminoEngine/Base/Event.hpp",
            TEST_ROOT "include/LuminoEngine/Base/Promise.hpp",
            TEST_ROOT "include/LuminoEngine/Base/Variant.hpp",
            TEST_ROOT "src/LuminoEngine/src/Runtime/BindingValidation.hpp",
            TEST_ROOT "include/LuminoEngine/Runtime/RuntimeLog.hpp",
            TEST_ROOT "include/LuminoEngine/Base/Collection.hpp",
            TEST_ROOT "include/LuminoEngine/Base/Serializer.hpp",
            TEST_ROOT "include/LuminoEngine/Platform/PlatformEvent.hpp",
            TEST_ROOT "include/LuminoEngine/Asset/AssetObject.hpp",
            TEST_ROOT "include/LuminoEngine/Asset/AssetModel.hpp",
            TEST_ROOT "include/LuminoEngine/Asset/Assets.hpp",
            TEST_ROOT "include/LuminoEngine/Audio/Sound.hpp",
            TEST_ROOT "include/LuminoEngine/Audio/Audio.hpp",
            TEST_ROOT "include/LuminoEngine/Graphics/Common.hpp",
            TEST_ROOT "include/LuminoEngine/Graphics/Graphics.hpp",
            TEST_ROOT "include/LuminoEngine/Graphics/ColorStructs.hpp",
            TEST_ROOT "include/LuminoEngine/Graphics/GeometryStructs.hpp",
            TEST_ROOT "include/LuminoEngine/Graphics/GraphicsResource.hpp",
            TEST_ROOT "include/LuminoEngine/Graphics/Texture.hpp",
            TEST_ROOT "include/LuminoEngine/Shader/Shader.hpp",
            TEST_ROOT "include/LuminoEngine/Rendering/Common.hpp",
            TEST_ROOT "include/LuminoEngine/Rendering/RenderView.hpp",
            TEST_ROOT "include/LuminoEngine/Rendering/Material.hpp",
            TEST_ROOT "include/LuminoEngine/Mesh/MeshPrimitive.hpp",
            TEST_ROOT "include/LuminoEngine/Mesh/AnimationController.hpp",
            TEST_ROOT "include/LuminoEngine/Mesh/MeshModel.hpp",
            TEST_ROOT "include/LuminoEngine/Mesh/SkinnedMeshModel.hpp",
            TEST_ROOT "include/LuminoEngine/Physics/CollisionShape.hpp",
            TEST_ROOT "include/LuminoEngine/Animation/Common.hpp",
            TEST_ROOT "include/LuminoEngine/Animation/AnimationCurve.hpp",
            TEST_ROOT "include/LuminoEngine/Animation/AnimationClip.hpp",
            TEST_ROOT "include/LuminoEngine/Animation/AnimationMixer.hpp",
            TEST_ROOT "include/LuminoEngine/Effect/EffectModel.hpp",
            TEST_ROOT "include/LuminoEngine/Effect/ParticleEffectModel.hpp",
            TEST_ROOT "include/LuminoEngine/Effect/ParticleEffectModel2.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Component.hpp",
            TEST_ROOT "include/LuminoEngine/Visual/VisualComponent.hpp",
            TEST_ROOT "include/LuminoEngine/Visual/SpriteComponent.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Common.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/CharacterController.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/World.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/WorldObject.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/VisualObject.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Camera.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Light.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Sprite.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/CameraOrbitControlComponent.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Raycaster.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/WorldRenderView.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Shapes/MeshPrimitives.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Mesh/Mesh.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Mesh/MeshComponent.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Physics/Collision.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Physics/RigidBodyComponent.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Effect/ParticleEmitter.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Scene.hpp",
            TEST_ROOT "include/LuminoEngine/Scene/Level.hpp",
            TEST_ROOT "include/LuminoEngine/UI/Common.hpp",
            TEST_ROOT "include/LuminoEngine/UI/UIColors.hpp",
            TEST_ROOT "include/LuminoEngine/UI/UIEvents.hpp",
            TEST_ROOT "include/LuminoEngine/UI/UILayoutElement.hpp",
            TEST_ROOT "include/LuminoEngine/UI/UIElement.hpp",
            TEST_ROOT "include/LuminoEngine/UI/UIText.hpp",
            TEST_ROOT "include/LuminoEngine/UI/UISprite.hpp",
            TEST_ROOT "include/LuminoEngine/UI/UIIcon.hpp",
            TEST_ROOT "include/LuminoEngine/UI/UIMessageTextArea.hpp",
            TEST_ROOT "include/LuminoEngine/UI/UI.hpp",
            TEST_ROOT "include/LuminoEngine/UI/Layout/UILayoutPanel.hpp",
            TEST_ROOT "include/LuminoEngine/UI/Layout/UIGridLayout.hpp",
            TEST_ROOT "include/LuminoEngine/UI/Controls/UIControl.hpp",
            TEST_ROOT "include/LuminoEngine/UI/Controls/UIButton.hpp",
            TEST_ROOT "include/LuminoEngine/UI/Controls/UIWindow.hpp",
            TEST_ROOT "include/LuminoEngine/UI/Controls/UIListBox.hpp",
            TEST_ROOT "include/LuminoEngine/Input/InputBinding.hpp",
            TEST_ROOT "include/LuminoEngine/Input/Input.hpp",
            TEST_ROOT "include/LuminoEngine/Input/Mouse.hpp",
            TEST_ROOT "include/LuminoEngine/Scripting/Interpreter.hpp",
            TEST_ROOT "include/LuminoEngine/Engine/EngineSettings.hpp",
            TEST_ROOT "include/LuminoEngine/Engine/Engine.hpp",
            TEST_ROOT "include/LuminoEngine/Engine/Application.hpp",
            TEST_ROOT "include/LuminoEngine/Engine/Debug.hpp",
        };

        CodeAnalyzer ca;
        ca.parserExecutable = ln::Path(ln::Path(ln::Environment::executablePath()).parent(), U"../../Parser/Release/LuminoTranscoder-Parser.exe").canonicalize();

        for (auto& file : files_LuminoCore) {
            CompilationDatabase cdb;
            cdb.inputFile = file;
            cdb.includeDirectories.add(TEST_ROOT "include");
            cdb.forceIncludeFiles.add(TEST_ROOT "src/LuminoCore/src/LuminoCore.PCH.h");
            ca.inputs.add(cdb);
        }

        for (auto& file : files_LuminoEngine) {
            CompilationDatabase cdb;
            cdb.inputFile = file;
            cdb.includeDirectories.add(TEST_ROOT "include");
            if (file.fileName().str() == U"Object.hpp") // Object class が force include されるものと合わせて再定義扱いになりクラス名が取れなくなるため、特別扱いする
                cdb.forceIncludeFiles.add(TEST_ROOT "include/LuminoCore.hpp");
            else
                cdb.forceIncludeFiles.add(TEST_ROOT "src/LuminoEngine/src/LuminoEngine.PCH.h");
            ca.inputs.add(cdb);
        }

        ca.analyze(pidb, diag);

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
    {
        FlatCSourceGenerator g;
        g.setup(db, config);
        g.generate();
    }
    {
        RubyExtGenerator g;
        g.setup(db, config);
        g.generate();
    }
    {
        RubyYARDOCSourceGenerator g;
        g.setup(db, config);
        g.generate();
    }
    {
        HSP3HeaderGenerator g;
        g.setup(db, config);
        g.generate();
    }
    {
        HSP3CommandsGenerator g;
        g.setup(db, config);
        g.generate();
    }
    {
        HSP3HelpGenerator g;
        g.setup(db, config);
        g.generate();
    }

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
