#pragma once
#include <LuminoEngine/Effect/Common.hpp>

namespace ln {
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
	};

    EffectManager();
	virtual ~EffectManager() = default;

	void init(const Settings& settings);
	void dispose();

    void testDraw(RenderingContext* renderingContext);

private:
    GraphicsManager* m_graphicsManager;
    std::unique_ptr<LLGINativeGraphicsExtension> m_nativeGraphicsExtension;
};

} // namespace detail
} // namespace ln

