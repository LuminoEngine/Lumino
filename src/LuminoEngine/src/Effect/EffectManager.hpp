#pragma once
#include <LuminoEngine/Effect/Common.hpp>

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
    ::Effekseer::Effect* getOrCreateEffekseerEffect(const Path& filePath);  // The reference count is incremented.
#endif

private:
    GraphicsManager* m_graphicsManager;
    AssetManager* m_assetManager;
    std::unique_ptr<LLGINativeGraphicsExtension> m_nativeGraphicsExtension;
    ::Effekseer::Manager* m_efkManager;
};

} // namespace detail
} // namespace ln

