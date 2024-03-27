
#pragma once
#include <stack>
#include <LuminoFramework/Scene/Scene.hpp>

namespace ln {
class Shader;
class World;

namespace detail {
class SceneConductor;
struct PathShape_NVGcontext;

class SceneManager
    : public RefObject {
public:
    Ref<Texture2D> SkydomeCloudA;
    Ref<Texture2D> SkydomeCloudB;
    Ref<Texture2D> SkydomeCloudC;
    Ref<Texture2D> SkydomeCloudR;
    Shader* skydomeShader() const;

    SceneManager();
    virtual ~SceneManager();
    void init();
    void dispose();

    Level* loadScene(const StringView& sceneAssetFilePath);
    void unloadScene(Level* scene);

#if 0
	void gotoScene(Level* scene);
	void callScene(Level* scene);
	void returnScene();
	Level* activeScene() const { return m_activeScene; }
#endif

    void updateFrame();

    void setActiveWorld(World* world) { m_activeWorld = world; }
    World* activeWorld() const { return m_activeWorld; }
    const Ref<Shader>& atmosphereShader() const { return m_atmosphereShader; }
    const Ref<Raycaster>& raycasterCache() const { return m_raycasterCache; }
    const std::unique_ptr<PathShape_NVGcontext>& pathContext() const { return m_pathContext; }
    //const Ref<detail::WorldSceneGraphRenderingContext>& renderingContext() const { return m_renderingContext; }

    void setDefaultCamera(Camera* value) { m_defaultCamera = value; }
    Camera* defaultCamera() const { return m_defaultCamera; }

    // テスト用の設定。
    // 0.9.0 以降、WorldObject は World へ自動追加しないようにしたい。
    // ただ、UnitTest が自動追加ありきで組まれているので、そちらの修正が終わるまでこの設定でカバーする。
    // Note: [2020/2/1] 小規模なプログラム作るのにはやっぱり億劫。Lumino で大規模開発するかっていうとまだまだ微妙なところがあるので、
    // のちのメジャーバージョンアップ時でいいだろう。まずは使い勝手重視で。
    bool autoAddingToActiveWorld = true;

    int getWorldObjectId();
    void releaseWorldObjectId(int id);
    bool m_editorMode = false; // ObjectId の書き込みを有効にすると実質バッチ描画ができなくなるので、通常は切っておく

    Ref<WorldObject> instantiateObjectFromAnyFile(const Path& filePath) const;

private:
    World* m_activeWorld;
    Camera* m_defaultCamera = nullptr;
    Ref<Shader> m_atmosphereShader;
    Ref<Raycaster> m_raycasterCache;
    std::unique_ptr<PathShape_NVGcontext> m_pathContext;
    //Ref<detail::WorldSceneGraphRenderingContext> m_renderingContext;

    int m_objectIndexCount = 0;
    std::stack<int> m_objectIndexStack;
};

} // namespace detail
} // namespace ln
