#pragma once
#include <LuminoEngine/Effect/Common.hpp>
#include "../src/Base/RefObjectCache.hpp"

namespace Effekseer {
class Manager;
class Effect;
}

namespace ln {
class EffectEmitter;
class GraphicsContext;
class RenderingContext;
namespace detail {
class GraphicsManager;
class LLGINativeGraphicsExtension;
class FileInterface;
class EffekseerEffect;

class EffectManager
	: public RefObject
{
public:
	struct Settings
	{
        GraphicsManager* graphicsManager = nullptr;
        AssetManager* assetManager = nullptr;
	};

    EffectManager();
	virtual ~EffectManager() = default;

	void init(const Settings& settings);
	void dispose();

    void testDraw(RenderingContext* renderingContext);
    //void testDraw2(GraphicsContext* graphicsContext);

    Ref<EffectEmitter> createEmitterFromFile(const Path& filePath);

#if LN_EFFEKSEER_ENABLED
    ::Effekseer::Manager* effekseerManager() const;
    Ref<EffekseerEffect> getOrCreateEffekseerEffect(const Path& filePath);  // The reference count is incremented.
    void releaseEffekseerEffect(EffekseerEffect* effect);
#endif

    AssetManager* assetManager() const { return m_assetManager; }

private:
    GraphicsManager* m_graphicsManager;
    AssetManager* m_assetManager;
    std::unique_ptr<LLGINativeGraphicsExtension> m_nativeGraphicsExtension;
    std::unique_ptr<FileInterface> m_fileInterface;
    ::Effekseer::Manager* m_efkManager;
    detail::ObjectCache<String, EffekseerEffect> m_efkEffectCache;
};

} // namespace detail
} // namespace ln

