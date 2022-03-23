#pragma once
#include <LuminoEngine/Effect/Common.hpp>
#include <LuminoEngine/Base/Promise.hpp>
#include <LuminoEngine/Base/detail/RefObjectCache.hpp>

namespace Effekseer {
class Manager;
class Effect;
}

namespace ln {
class EffectEmitter;
class GraphicsContext;
class RenderingContext;
class ParticleModel;

/**
 * Texture2DPromise
 */
LN_PROMISE()
using ParticleModelPromise = Promise<Ref<ParticleModel>>;

namespace detail {
class AssetManager;
class GraphicsManager;
class RenderingManager;
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
        RenderingManager* renderingManager = nullptr;
	};

    EffectManager();
	virtual ~EffectManager() = default;

	void init(const Settings& settings);
	void dispose();

    Ref<ParticleModel> loadParticleModel(const StringView& filePath);
    Ref<ParticleModelPromise> loadParticleModelAsync(const StringView& filePath);

    void testDraw(RenderingContext* renderingContext);


    Ref<EffectEmitter> createEmitterFromFile(const Path& filePath);

#if LN_EFFEKSEER_ENABLED
    ::Effekseer::Manager* effekseerManager() const;
    Ref<EffekseerEffect> getOrCreateEffekseerEffect(const Path& filePath);  // The reference count is incremented.
    void releaseEffekseerEffect(EffekseerEffect* effect);
#endif

    AssetManager* assetManager() const { return m_assetManager; }

    const Ref<SpriteParticleGeometry>& defaultSpriteParticleGeometry() const { return m_defaultSpriteParticleGeometry; }

private:
    GraphicsManager* m_graphicsManager;
    AssetManager* m_assetManager;
    std::unique_ptr<LLGINativeGraphicsExtension> m_nativeGraphicsExtension;
    std::unique_ptr<FileInterface> m_fileInterface;
    ::Effekseer::Manager* m_efkManager;
    detail::ObjectCache<String, EffekseerEffect> m_efkEffectCache;

    Ref<SpriteParticleGeometry> m_defaultSpriteParticleGeometry;
};

} // namespace detail
} // namespace ln

